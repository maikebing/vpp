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

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppRenderManager.hpp"
#include "../include/vppCommandBufferRecorder.hpp"
#include "../include/vppImageOperations.hpp"
#include "../include/vppFrameImageView.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

FrameBuffer RenderManagerImpl :: getFrameBuffer (
    const RenderPass& renderPass, size_t iSwapImage )
{
    const RenderGraph& graph = renderPass.graph();

    const auto iConfig = 
        d_config2frameBuffers.find ( graph.getAttachmentConfig() );

    if ( iConfig != d_config2frameBuffers.end() )
    {
        const auto& frameBuffers = iConfig->second;

        if ( iSwapImage < frameBuffers.size() )
            return frameBuffers [ iSwapImage ];
        else
            return FrameBuffer();
    }

    const Device hDevice = renderPass.device();
    const KAttachmentConfig& attachments = graph.getAttachmentConfig();
    const size_t nSwapImages = d_swapChain.views();
    const size_t nAttachments = attachments.getDescriptionCount();

    std::vector< FrameImageView > imageViews;
    imageViews.resize ( nAttachments );

    size_t displayImageIndex = std::numeric_limits< size_t >::max();

    for ( size_t j = 0; j != nAttachments; ++j )
    {
        const ImageInfo& imageInfo = graph.getAttachmentInfo ( j );

        if ( imageInfo.purpose != DISPLAY )
        {
            Img newImage;
            FrameImageView hView = attachments.getPredefinedView ( j );

            if ( ! hView )
            {
                FrameImageView imageView ( imageInfo, hDevice );
                newImage = imageView.image();
                imageViews [ j ] = imageView;
            }
            else
            {
                newImage = hView.image();
                imageViews [ j ] = hView;
            }
        }
        else
            displayImageIndex = j;
    }

    const auto iNew = d_config2frameBuffers.emplace (
        std::piecewise_construct,
            std::forward_as_tuple ( attachments.getInfos(), attachments.getDescriptions() ),
            std::forward_as_tuple() ).first;

    FrameBuffers& frameBuffers = iNew->second;
    frameBuffers.reserve ( nSwapImages );

    for ( size_t i = 0; i != nSwapImages; ++i )
    {
        if ( displayImageIndex != std::numeric_limits< size_t >::max() )
            imageViews [ displayImageIndex ] = d_swapChain.view ( i );

        frameBuffers.emplace_back ( imageViews, renderPass );
    }

    return frameBuffers [ iSwapImage ];
}

// -----------------------------------------------------------------------------

void RenderManager :: beginFrame()
{
    RenderManagerImpl* pImpl = get();
    pImpl->d_queue.waitForIdle();
    pImpl->d_currentSwapImage = pImpl->d_swapChain.acquireDisplayImage ( pImpl->d_queue );
}

// -----------------------------------------------------------------------------

CommandBuffer RenderManager :: getRenderCommands (
    const RenderPass& hRenderPass, ECommandsCaching caching )
{
    RenderManagerImpl* pImpl = get();

    FrameBuffer hFrameBuffer =
        pImpl->getFrameBuffer ( hRenderPass, pImpl->d_currentSwapImage );

    CommandBuffer hCommandBuffer;

    bool bRebuildBuffer = false;
    const size_t nSwapchainImages = pImpl->d_swapChain.views();

    if ( caching == REBUILD_CMDS )
        pImpl->d_rebuildCounter = static_cast< std::uint32_t >( nSwapchainImages );

    if ( pImpl->d_rebuildCounter )
    {
        bRebuildBuffer = true;
        --pImpl->d_rebuildCounter;
    }

    const RenderManagerImpl::RenderPassKey key ( hRenderPass, hFrameBuffer );
    auto iCommands = pImpl->d_renderPassCommands.find ( key );

    if ( iCommands == pImpl->d_renderPassCommands.end() )
    {
        hCommandBuffer = pImpl->d_commandPool.createBuffer();
        pImpl->d_renderPassCommands.emplace ( key, hCommandBuffer ).first;
        bRebuildBuffer = true;
    }
    else
    {
        hCommandBuffer = iCommands->second;

        if ( bRebuildBuffer )
            hCommandBuffer.reset();
    }

    if ( bRebuildBuffer ) 
    {
        CommandBufferRecorder recorder ( hCommandBuffer );
        recorder.render ( hRenderPass, hFrameBuffer );
    }

    return hCommandBuffer;
}

// -----------------------------------------------------------------------------

void RenderManager :: render (
    const RenderPass& hRenderPass, ECommandsCaching caching )
{
    RenderManagerImpl* pImpl = get();
    pImpl->d_queue.submit ( getRenderCommands ( hRenderPass, caching ) );
}

// -----------------------------------------------------------------------------

void RenderManager :: endFrame()
{
    RenderManagerImpl* pImpl = get();
    pImpl->d_swapChain.presentDisplayImage ( pImpl->d_queue, pImpl->d_currentSwapImage );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
