/*
    Copyright 2016-2019 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppCommandBufferRecorder.hpp"
#include "../include/vppRenderPass.hpp"
#include "../include/vppFramebuffer.hpp"
#include "../include/vppComputePass.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

void CommandBufferRecorder :: render (
    const RenderPass& hRenderPass,
    const FrameBuffer& hFrameBuffer,
    bool bAutoBindPipeline )
{
    RenderPass hCurrentRenderPass = hRenderPass;
    hCurrentRenderPass.beginRendering();

    const RenderGraph& hGraph = hRenderPass.graph();

    RenderingCommandContext context ( d_buffer.handle() );
    RenderingFrameBufferContext fbcontext ( hFrameBuffer );

    const std::uint32_t nProcesses = hGraph.getProcessCount();
    const std::uint32_t nPreprocesses = hGraph.getPreprocessCount();
    const std::uint32_t nPostprocesses = hGraph.getPostprocessCount();

    for ( unsigned int iPreprocess = 0; iPreprocess != nPreprocesses; ++iPreprocess )
    {
        const RenderGraph::Commands& commands = hGraph.getPreprocessCommands ( iPreprocess );
        
        for ( const auto& iCommand : commands )
            iCommand();
    }

    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = 0;
    renderPassBeginInfo.renderPass = hRenderPass.handle();
    renderPassBeginInfo.framebuffer = hFrameBuffer.handle();
    renderPassBeginInfo.renderArea = hFrameBuffer.area();

    const ClearValues& clearValues = hGraph.getClearValues();

    renderPassBeginInfo.clearValueCount = static_cast< std::uint32_t >( clearValues.size() );
    renderPassBeginInfo.pClearValues = & clearValues [ 0 ];

    // FIXME - co z tymi secondary buforami?

    ::vkCmdBeginRenderPass (
        d_buffer.handle(), & renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

    for ( unsigned int iProcess = 0; iProcess != nProcesses; ++iProcess )
    {
        if ( iProcess > 0 )
            ::vkCmdNextSubpass ( d_buffer.handle(), VK_SUBPASS_CONTENTS_INLINE );

        if ( bAutoBindPipeline )
        {
            Pipeline hPipeline = hRenderPass.pipeline ( iProcess, 0 );

            ::vkCmdBindPipeline (
                d_buffer.handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, hPipeline.handle() );
        }

        const RenderGraph::Commands& commands = hGraph.getProcessCommands ( iProcess );
        
        for ( const auto& iCommand : commands )
            iCommand();
    }

    ::vkCmdEndRenderPass ( d_buffer.handle() );

    for ( unsigned int iPostprocess = 0; iPostprocess != nPostprocesses; ++iPostprocess )
    {
        const RenderGraph::Commands& commands = hGraph.getPostprocessCommands ( iPostprocess );
        
        for ( const auto& iCommand : commands )
            iCommand();
    }

    hCurrentRenderPass.endRendering();
}

// -----------------------------------------------------------------------------

void CommandBufferRecorder :: compute (
    const ComputePass& hComputePass,
    bool bAutoBindPipeline )
{
    ComputePass hCurrentRenderPass = hComputePass;
    hCurrentRenderPass.beginComputing();

    RenderingCommandContext context ( d_buffer.handle() );
    const ComputePass::Commands& commands = hCurrentRenderPass.getCommands();

    if ( bAutoBindPipeline )
    {
        ComputePipeline hPipeline = hCurrentRenderPass.pipeline ( 0 );

        ::vkCmdBindPipeline (
            d_buffer.handle(), VK_PIPELINE_BIND_POINT_COMPUTE, hPipeline.handle() );
    }

    for ( const auto& iCommand : commands )
        iCommand();

    hCurrentRenderPass.endComputing();
}

// -----------------------------------------------------------------------------

void CommandBufferRecorder :: perform  (
    const Procedure& hProcedure )
{
    RenderingCommandContext context ( d_buffer.handle() );
    const Procedure::FCommands& commands = hProcedure.getCommands();
    commands();
}

// -----------------------------------------------------------------------------

void CommandBufferRecorder :: presentImage ( VkImage hImage )
{
    VkImageMemoryBarrier vkImageMemoryBarrier;
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    vkImageMemoryBarrier.pNext = 0;
    vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    vkImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkImageMemoryBarrier.image = hImage;

    ::vkCmdPipelineBarrier (
        d_buffer.handle(),
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ,
        0, 0, 0, 0, 0, 1, & vkImageMemoryBarrier );
}

// -----------------------------------------------------------------------------

void CommandBufferRecorder :: unpresentImage ( VkImage hImage )
{
    VkImageMemoryBarrier vkImageMemoryBarrier;
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    vkImageMemoryBarrier.pNext = 0;
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    vkImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    vkImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkImageMemoryBarrier.image = hImage;

    ::vkCmdPipelineBarrier (
        d_buffer.handle(),
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        0, 0, 0, 0, 0, 1, & vkImageMemoryBarrier );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
