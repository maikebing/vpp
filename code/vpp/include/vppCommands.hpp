/*
    Copyright 2016-2018 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
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

#ifndef INC_VPPCOMMANDS_HPP
#define INC_VPPCOMMANDS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPBARRIERS_HPP
#include "vppBarriers.hpp"
#endif

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

#ifndef INC_VPPUSAGECHECKS_HPP
#include "vppUsageChecks.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class NonRenderingCommands
{
public:
    VPP_DLLAPI static void cmdFillBuffer (
        const TDBuf& hDstBuffer,
        size_t offset, size_t size, std::uint32_t value,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyBuffer (
        const TSBuf& hSrcBuffer,
        const TDBuf& hDstBuffer,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyBuffer (
        const TSBuf& hSrcBuffer,
        const TDBuf& hDstBuffer,
        const std::vector< VkBufferCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdUpdateBuffer (
        const TDBuf& hDstBuffer,
        size_t offset, size_t size, const std::uint32_t* pData,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyImage (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyImage (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyBufferToImage (
        const TSBuf& hSrcBuffer,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyBufferToImage (
        const TSBuf& hSrcBuffer,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkBufferImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyImageToBuffer (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDBuf& hDstBuffer,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyImageToBuffer (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDBuf& hDstBuffer,
        const std::vector< VkBufferImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBlitImage (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
        const VkOffset3D& dstBegin, const VkOffset3D& dstEnd,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBlitImage (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
        const VkOffset3D& dstBegin,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBlitImage (
        const TSImg& hSrcImage,
        VkImageLayout srcImageLayout,
        const TDImg& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageBlit >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcOffset, const VkOffset3D& dstOffset,
        const VkExtent3D& size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageResolve >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdClearColorImage (
        const TDImg& hImage,
        VkImageLayout imageLayout,
        const VkClearColorValue& color,
        CommandBuffer hCmdBuffer = CommandBuffer() );
    
    VPP_DLLAPI static void cmdClearColorImage (
        const TDImg& hImage,
        VkImageLayout imageLayout,
        const VkClearColorValue& color,
        const std::vector< VkImageSubresourceRange >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdClearDepthStencilImage (
        const TDImg& hImage,
        VkImageLayout imageLayout,
        float depth, std::uint32_t stencil,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdClearDepthStencilImage (
        const TDImg& hImage,
        VkImageLayout imageLayout,
        float depth, std::uint32_t stencil,
        const std::vector< VkImageSubresourceRange >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdResetQueryPool (
        const QueryPool& hPool,
        std::uint32_t first,
        std::uint32_t count,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdResetQueryPool (
        const QueryPool& hPool,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyQueryPoolResults (
        const Buf& hDstBuffer,
        const QueryPool& hPool,
        VkQueryResultFlags flags,
        std::uint32_t count,
        std::uint32_t first,
        VkDeviceSize dstOffset,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdCopyQueryPoolResults (
        const Buf& hDstBuffer,
        const QueryPool& hPool,
        VkQueryResultFlags flags,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class UniversalCommands
{
public:
    // -------------------------------------------------------------------------

    VPP_DLLAPI static void cmdExecuteCommands (
        const VkCommandBuffer* pBuffers,
        size_t size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdExecuteCommands (
        const CommandBuffer* pBuffers,
        size_t size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdExecuteCommands (
        const std::vector< CommandBuffer >& buffers,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdWaitEvents (
        const std::vector< Event >& events,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        const Barriers& barriers = Barriers(),
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdPipelineBarrier (
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkDependencyFlags dependencyFlags,
        const Barriers& barriers,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdImagePipelineBarrier (
        const Img& image,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        bool bByRegion,
        VkImageLayout oldLayout = VK_IMAGE_LAYOUT_GENERAL,
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_GENERAL,
        int mipLevel = -1,
        int arrayLayer = -1,
        CommandBuffer hCmdBuffer = CommandBuffer()
    );

    VPP_DLLAPI static void cmdBufferPipelineBarrier (
        const Buf& hBuffer,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        CommandBuffer hCmdBuffer = CommandBuffer()
    );

    VPP_DLLAPI static void cmdSetViewport (
        const VkViewport& viewport,
        std::uint32_t index = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetViewport (
        const std::vector< VkViewport >& viewports,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetLineWidth (
        float lineWidth,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetDepthBias (
        float constantFactor,
        float clamp,
        float slopeFactor,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetDepthBounds (
        float minDepthBounds,
        float maxDepthBounds,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetScissor (
        const VkRect2D& scissor,
        std::uint32_t index = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetScissor (
        const std::vector< VkRect2D >& scissors,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetStencilCompareMask (
        VkStencilFaceFlags faceMask,
        std::uint32_t compareMask,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetStencilWriteMask ( 
        VkStencilFaceFlags faceMask,
        std::uint32_t writeMask,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetStencilReference ( 
        VkStencilFaceFlags faceMask,
        std::uint32_t reference,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdSetBlendConstants (
        const float blendConstants [ 4 ],
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBeginQuery (
        const QueryPool& hPool,
        std::uint32_t query,
        VkQueryControlFlags flags,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdEndQuery (
        const QueryPool& hPool,
        std::uint32_t query,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdWriteTimestamp (
        const QueryPool& hPool,
        std::uint32_t query,
        VkPipelineStageFlagBits pipelineStage,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBindPipeline (
        const Pipeline& hPipeline,
        VkPipelineBindPoint pipelineBindPoint,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBindDescriptorSets (
        const ShaderDataBlock& hDataBlock,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBindIndexInput (
        const Buf& hBuffer,
        VkIndexType indexType,
        VkDeviceSize offset,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBindVertexBuffers (
        const std::vector< Buf >& buffers,
        const std::vector< VkDeviceSize >& offsets,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdBindVertexBuffers (
        const std::vector< Buf >& buffers,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdPushConstants (
        const PipelineLayoutBase& hPipelineLayout,
        VkShaderStageFlags stageFlags,
        const void* pValues,
        std::uint32_t size,
        std::uint32_t offset = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ExtendedCommands :
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    VPP_DLLAPI static void cmdChangeImageLayout (
        const Img& hImage,
        VkImageLayout oldImageLayout, 
        VkImageLayout newImageLayout,
        std::uint32_t levelCount = VK_REMAINING_MIP_LEVELS,
        std::uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS,
        std::uint32_t baseMipLevel = 0,
        std::uint32_t baseArrayLayer = 0,
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMANDS_HPP
