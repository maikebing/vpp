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
#include "../include/vppFramebuffer.hpp"
#include "../include/vppRenderPass.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

KFrameBufferImpl :: KFrameBufferImpl (
    const std::vector< FrameImageView >& imageViews,
    const RenderPass& hRenderPass ) :
        d_hDevice ( hRenderPass.device() ),
        d_handle(),
        d_result ( VK_NOT_READY ),
        d_imageViews ( imageViews )
{
    static const std::uint32_t MAX_SIZE =
        std::numeric_limits< std::uint32_t >::max();

    VkFramebufferCreateInfo framebufferCreateInfo;

    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = 0;
    framebufferCreateInfo.flags = 0;
    framebufferCreateInfo.renderPass = hRenderPass.handle();

    const RenderGraph& graph = hRenderPass.graph();
    const KAttachmentConfig& attConfig =  graph.getAttachmentConfig();
    const size_t nSuppliedViews = imageViews.size();
    const size_t nAttachments = graph.getAttachmentCount();

    std::vector< FrameImageView > effectiveViews;
    effectiveViews.reserve ( nAttachments );

    for ( size_t i = 0; i < nSuppliedViews; ++i )
    {
        if ( imageViews [ i ] )
            effectiveViews.push_back ( imageViews [ i ] );
        else
            effectiveViews.push_back ( attConfig.getPredefinedView ( i ) );
    }

    for ( size_t i = nSuppliedViews; i < nAttachments; ++i )
        effectiveViews.push_back ( attConfig.getPredefinedView ( i ) );

    ImageViews imageHandles;
    imageHandles.resize ( nAttachments );

    std::copy ( effectiveViews.begin(), effectiveViews.end(), imageHandles.begin() );

    for ( size_t i = 0; i < nAttachments; ++i )
        if ( imageHandles [ i ] == VK_NULL_HANDLE )
            throw XRuntimeError ( "One of FrameBuffer images is not set." );

    framebufferCreateInfo.attachmentCount = static_cast< std::uint32_t >( nAttachments );
    framebufferCreateInfo.pAttachments = & imageHandles [ 0 ];
    framebufferCreateInfo.width = MAX_SIZE;
    framebufferCreateInfo.height = MAX_SIZE;
    framebufferCreateInfo.layers = MAX_SIZE;

    for ( const auto& iImageView : effectiveViews )
    {
        const ImageInfo& imageInfo = iImageView.image().info();

        if ( imageInfo.extent.width < framebufferCreateInfo.width )
            framebufferCreateInfo.width = imageInfo.extent.width;

        if ( imageInfo.extent.height < framebufferCreateInfo.height )
            framebufferCreateInfo.height = imageInfo.extent.height;

        if ( imageInfo.extent.depth < framebufferCreateInfo.layers )
            framebufferCreateInfo.layers = imageInfo.extent.depth;
    }

    if ( framebufferCreateInfo.width == MAX_SIZE )
        framebufferCreateInfo.width = 0;

    if ( framebufferCreateInfo.height == MAX_SIZE )
        framebufferCreateInfo.height = 0;

    if ( framebufferCreateInfo.layers == MAX_SIZE )
        framebufferCreateInfo.layers = 0;

    d_width = framebufferCreateInfo.width;
    d_height = framebufferCreateInfo.height;
    d_layers = framebufferCreateInfo.layers;

    d_result = ::vkCreateFramebuffer (
        d_hDevice.handle(), & framebufferCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

KFrameBufferImpl :: ~KFrameBufferImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyFramebuffer ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
