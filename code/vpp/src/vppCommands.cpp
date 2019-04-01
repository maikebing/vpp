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
#include "../include/vppCommands.hpp"
#include "../include/vppBuffer.hpp"
#include "../include/vppImage.hpp"
#include "../include/vppCommandBuffer.hpp"
#include "../include/vppCommandPool.hpp"
#include "../include/vppQueue.hpp"
#include "../include/vppSynchronization.hpp"
#include "../include/vppGeometry.hpp"
#include "../include/vppQueryPool.hpp"
#include "../include/vppBarriers.hpp"
#include "../include/vppPipelineLayout.hpp"
#include "../include/vppPipeline.hpp"
#include "../include/vppShaderDataBlock.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdFillBuffer (
    const TDBuf& hDstBuffer,
    size_t offset, size_t size, std::uint32_t value,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdFillBuffer (
        hCmdBuffer, hDstBuffer.bufferRef().handle(),
        static_cast< VkDeviceSize >( offset ), static_cast< VkDeviceSize >( size ),
        value 
    );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyBuffer (
    const TSBuf& hSrcBuffer,
    const TDBuf& hDstBuffer,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    VkBufferCopy vkBufferCopy;
    vkBufferCopy.srcOffset = 0;
    vkBufferCopy.dstOffset = 0;
    vkBufferCopy.size = std::min (
        hSrcBuffer.bufferRef().size(), hDstBuffer.bufferRef().size() );

    ::vkCmdCopyBuffer (
        hCmdBuffer,
        hSrcBuffer.bufferRef().handle(), hDstBuffer.bufferRef().handle(),
        1u, & vkBufferCopy );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyBuffer (
    const TSBuf& hSrcBuffer,
    const TDBuf& hDstBuffer,
    const std::vector< VkBufferCopy >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdCopyBuffer (
        hCmdBuffer, hSrcBuffer.bufferRef().handle(), hDstBuffer.bufferRef().handle(), 
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdUpdateBuffer (
    const TDBuf& hDstBuffer,
    size_t offset, size_t size, const std::uint32_t* pData,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdUpdateBuffer (
        hCmdBuffer, hDstBuffer.bufferRef().handle(),
        static_cast< VkDeviceSize >( offset ), static_cast< VkDeviceSize >( size ),
        pData
    );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyImage (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& srcInfo = hSrcImage.imageRef().info();
    const ImageInfo& dstInfo = hDstImage.imageRef().info();
    const VkExtent3D& srcExtent = srcInfo.extent;
    const VkExtent3D& dstExtent = dstInfo.extent;

    VkImageCopy vkImageCopy;
    vkImageCopy.srcOffset.x = 0;
    vkImageCopy.srcOffset.y = 0;
    vkImageCopy.srcOffset.z = 0;
    vkImageCopy.dstOffset.x = 0;
    vkImageCopy.dstOffset.y = 0;
    vkImageCopy.dstOffset.z = 0;
    vkImageCopy.extent.width = std::min ( srcExtent.width, dstExtent.width );
    vkImageCopy.extent.height = std::min ( srcExtent.height, dstExtent.height );
    vkImageCopy.extent.depth = std::min ( srcExtent.depth, dstExtent.depth );

    vkImageCopy.srcSubresource.baseArrayLayer = 0;
    vkImageCopy.dstSubresource.baseArrayLayer = 0;

    vkImageCopy.srcSubresource.layerCount = 
        std::min ( srcInfo.arrayLayers, dstInfo.arrayLayers );

    vkImageCopy.dstSubresource.layerCount =
        vkImageCopy.srcSubresource.layerCount;

    vkImageCopy.srcSubresource.mipLevel = 0;
    vkImageCopy.dstSubresource.mipLevel = 0;

    vkImageCopy.srcSubresource.aspectMask = srcInfo.getAspect();
    vkImageCopy.dstSubresource.aspectMask = vkImageCopy.srcSubresource.aspectMask;

    ::vkCmdCopyImage (
        hCmdBuffer,
        hSrcImage.imageRef().handle(), srcImageLayout,
        hDstImage.imageRef().handle(), dstImageLayout,
        1u, & vkImageCopy );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyImage (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    const std::vector< VkImageCopy >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& srcInfo = hSrcImage.imageRef().info();
    const ImageInfo& dstInfo = hDstImage.imageRef().info();
    const VkExtent3D& srcExtent = srcInfo.extent;
    const VkExtent3D& dstExtent = dstInfo.extent;

    ::vkCmdCopyImage (
        hCmdBuffer,
        hSrcImage.imageRef().handle(), srcImageLayout,
        hDstImage.imageRef().handle(), dstImageLayout,
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyBufferToImage (
    const TSBuf& hSrcBuffer,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& dstInfo = hDstImage.imageRef().info();

    VkBufferImageCopy vkBufferImageCopy;
    vkBufferImageCopy.bufferOffset = 0;
    vkBufferImageCopy.bufferRowLength = 0;
    vkBufferImageCopy.bufferImageHeight = 0;
    vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
    vkBufferImageCopy.imageSubresource.layerCount = dstInfo.arrayLayers;
    vkBufferImageCopy.imageSubresource.mipLevel = 0;
    vkBufferImageCopy.imageSubresource.aspectMask = dstInfo.getAspect();
    vkBufferImageCopy.imageOffset.x = 0;
    vkBufferImageCopy.imageOffset.y = 0;
    vkBufferImageCopy.imageOffset.z = 0;
    vkBufferImageCopy.imageExtent = dstInfo.extent;
    
    ::vkCmdCopyBufferToImage (
        hCmdBuffer,
        hSrcBuffer.bufferRef().handle(), hDstImage.imageRef().handle(),
        dstImageLayout, 1u, & vkBufferImageCopy );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyBufferToImage (
    const TSBuf& hSrcBuffer,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    const std::vector< VkBufferImageCopy >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdCopyBufferToImage (
        hCmdBuffer, hSrcBuffer.bufferRef().handle(), hDstImage.imageRef().handle(),
        dstImageLayout, static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyImageToBuffer (
    const TSImg& hSrcImage,
    VkImageLayout dstImageLayout,
    const TDBuf& hDstBuffer,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& srcInfo = hSrcImage.imageRef().info();

    VkBufferImageCopy vkBufferImageCopy;
    vkBufferImageCopy.bufferOffset = 0;
    vkBufferImageCopy.bufferRowLength = 0;
    vkBufferImageCopy.bufferImageHeight = 0;
    vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
    vkBufferImageCopy.imageSubresource.layerCount = srcInfo.arrayLayers;
    vkBufferImageCopy.imageSubresource.mipLevel = 0;
    vkBufferImageCopy.imageSubresource.aspectMask = srcInfo.getAspect();
    vkBufferImageCopy.imageOffset.x = 0;
    vkBufferImageCopy.imageOffset.y = 0;
    vkBufferImageCopy.imageOffset.z = 0;
    vkBufferImageCopy.imageExtent = srcInfo.extent;
    
    ::vkCmdCopyImageToBuffer (
        hCmdBuffer, hSrcImage.imageRef().handle(), dstImageLayout,
        hDstBuffer.bufferRef().handle(), 1u, & vkBufferImageCopy );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyImageToBuffer (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDBuf& hDstBuffer,
    const std::vector< VkBufferImageCopy >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& srcInfo = hSrcImage.imageRef().info();

    VkBufferImageCopy vkBufferImageCopy;
    vkBufferImageCopy.bufferOffset = 0;
    vkBufferImageCopy.bufferRowLength = 0;
    vkBufferImageCopy.bufferImageHeight = 0;
    vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
    vkBufferImageCopy.imageSubresource.layerCount = srcInfo.arrayLayers;
    vkBufferImageCopy.imageSubresource.mipLevel = 0;
    vkBufferImageCopy.imageSubresource.aspectMask = srcInfo.getAspect();
    vkBufferImageCopy.imageOffset.x = 0;
    vkBufferImageCopy.imageOffset.y = 0;
    vkBufferImageCopy.imageOffset.z = 0;
    vkBufferImageCopy.imageExtent = srcInfo.extent;
    
    ::vkCmdCopyImageToBuffer (
        hCmdBuffer, hSrcImage.imageRef().handle(), srcImageLayout,
        hDstBuffer.bufferRef().handle(),
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdBlitImage (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
    const VkOffset3D& dstBegin, const VkOffset3D& dstEnd,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    VkImageBlit vkImageBlit;
    vkImageBlit.srcOffsets [ 0 ] = srcBegin;
    vkImageBlit.srcOffsets [ 1 ] = srcEnd;
    vkImageBlit.dstOffsets [ 0 ] = dstBegin;
    vkImageBlit.dstOffsets [ 1 ] = dstEnd;
        
    const ImageInfo& srcInfo = hSrcImage.imageRef().info();
    const ImageInfo& dstInfo = hDstImage.imageRef().info();

    vkImageBlit.srcSubresource.baseArrayLayer = 0;
    vkImageBlit.srcSubresource.layerCount = srcInfo.arrayLayers;
    vkImageBlit.srcSubresource.mipLevel = 0;
    vkImageBlit.srcSubresource.aspectMask = srcInfo.getAspect();

    vkImageBlit.dstSubresource.baseArrayLayer = 0;
    vkImageBlit.dstSubresource.layerCount = dstInfo.arrayLayers;
    vkImageBlit.dstSubresource.mipLevel = 0;
    vkImageBlit.dstSubresource.aspectMask = dstInfo.getAspect();

    ::vkCmdBlitImage (
        hCmdBuffer, hSrcImage.imageRef().handle(), srcImageLayout,
        hDstImage.imageRef().handle(), dstImageLayout,
        1u, & vkImageBlit, VK_FILTER_NEAREST );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdBlitImage (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
    const VkOffset3D& dstBegin,
    CommandBuffer hCommandBuffer )
{
    cmdBlitImage (
        hSrcImage, srcImageLayout, hDstImage, dstImageLayout,
        srcBegin, srcEnd, dstBegin, dstBegin + ( srcEnd - srcBegin ),
        hCommandBuffer );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdBlitImage (
    const TSImg& hSrcImage,
    VkImageLayout srcImageLayout,
    const TDImg& hDstImage,
    VkImageLayout dstImageLayout,
    const std::vector< VkImageBlit >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBlitImage (
        hCmdBuffer, hSrcImage.imageRef().handle(), srcImageLayout,
        hDstImage.imageRef().handle(), dstImageLayout,
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ],
        VK_FILTER_NEAREST );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdResolveImage (
    const Img& hSrcImage,
    VkImageLayout srcImageLayout,
    const Img& hDstImage,
    VkImageLayout dstImageLayout,
    const VkOffset3D& srcOffset, const VkOffset3D& dstOffset,
    const VkExtent3D& size,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    VkImageResolve vkImageResolve;
    vkImageResolve.srcOffset = srcOffset;
    vkImageResolve.dstOffset = dstOffset;
    vkImageResolve.extent = size;
        
    const ImageInfo& srcInfo = hSrcImage.info();
    const ImageInfo& dstInfo = hDstImage.info();

    vkImageResolve.srcSubresource.baseArrayLayer = 0;
    vkImageResolve.srcSubresource.layerCount = srcInfo.arrayLayers;
    vkImageResolve.srcSubresource.mipLevel = 0;
    vkImageResolve.srcSubresource.aspectMask = srcInfo.getAspect();

    vkImageResolve.dstSubresource.baseArrayLayer = 0;
    vkImageResolve.dstSubresource.layerCount = dstInfo.arrayLayers;
    vkImageResolve.dstSubresource.mipLevel = 0;
    vkImageResolve.dstSubresource.aspectMask = dstInfo.getAspect();

    ::vkCmdResolveImage (
        hCmdBuffer, hSrcImage.handle(), srcImageLayout,
        hDstImage.handle(), dstImageLayout,
        1u, & vkImageResolve );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdResolveImage (
    const Img& hSrcImage,
    VkImageLayout srcImageLayout,
    const Img& hDstImage,
    VkImageLayout dstImageLayout,
    CommandBuffer hCommandBuffer )
{
    const ImageInfo& srcInfo = hSrcImage.info();
    const ImageInfo& dstInfo = hDstImage.info();

    cmdResolveImage (
        hSrcImage, srcImageLayout, hDstImage, dstImageLayout,
        VkOffset3D { 0, 0, 0 }, VkOffset3D { 0, 0, 0 },
        min ( srcInfo.extent, dstInfo.extent ),
        hCommandBuffer );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdResolveImage (
    const Img& hSrcImage,
    VkImageLayout srcImageLayout,
    const Img& hDstImage,
    VkImageLayout dstImageLayout,
    const std::vector< VkImageResolve >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdResolveImage (
        hCmdBuffer, hSrcImage.handle(), srcImageLayout,
        hDstImage.handle(), dstImageLayout,
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdClearColorImage (
    const TDImg& hImage,
    const VkClearColorValue& color,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& imgInfo = hImage.imageRef().info();

    VkImageSubresourceRange vkImageSubresourceRange;
    vkImageSubresourceRange.aspectMask = imgInfo.getAspect();
    vkImageSubresourceRange.baseArrayLayer = 0;
    vkImageSubresourceRange.layerCount = imgInfo.arrayLayers;
    vkImageSubresourceRange.baseMipLevel = 0;
    vkImageSubresourceRange.levelCount = imgInfo.mipLevels;

    UniversalCommands::cmdPipelineBarrier ( barriers (
        Bar::NONE, Bar::TRANSFER, hImage.imageRef()
    ) );

    ::vkCmdClearColorImage (
        hCmdBuffer, hImage.imageRef().handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        & color, 1u, & vkImageSubresourceRange );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdClearColorImage (
    const TDImg& hImage,
    const VkClearColorValue& color,
    const std::vector< VkImageSubresourceRange >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& imgInfo = hImage.imageRef().info();

    UniversalCommands::cmdPipelineBarrier ( barriers (
        hImage.imageRef(), regions, Bar::NONE, Bar::TRANSFER
    ) );

    ::vkCmdClearColorImage (
        hCmdBuffer, hImage.imageRef().handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        & color, static_cast< std::uint32_t >( regions.size() ), & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdClearDepthStencilImage (
    const TDImg& hImage,
    float depth, std::uint32_t stencil,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& imgInfo = hImage.imageRef().info();

    VkImageSubresourceRange vkImageSubresourceRange;
    vkImageSubresourceRange.aspectMask = imgInfo.getAspect();
    vkImageSubresourceRange.baseArrayLayer = 0;
    vkImageSubresourceRange.layerCount = imgInfo.arrayLayers;
    vkImageSubresourceRange.baseMipLevel = 0;
    vkImageSubresourceRange.levelCount = imgInfo.mipLevels;

    VkClearDepthStencilValue vkClearDepthStencilValue;
    vkClearDepthStencilValue.depth = depth;
    vkClearDepthStencilValue.stencil = stencil;

    UniversalCommands::cmdPipelineBarrier ( barriers (
        Bar::NONE, Bar::TRANSFER, hImage.imageRef()
    ) );

    ::vkCmdClearDepthStencilImage (
        hCmdBuffer, hImage.imageRef().handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        & vkClearDepthStencilValue,
        1u, & vkImageSubresourceRange );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdClearDepthStencilImage (
    const TDImg& hImage,
    float depth, std::uint32_t stencil,
    const std::vector< VkImageSubresourceRange >& regions,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const ImageInfo& imgInfo = hImage.imageRef().info();

    VkClearDepthStencilValue vkClearDepthStencilValue;
    vkClearDepthStencilValue.depth = depth;
    vkClearDepthStencilValue.stencil = stencil;

    UniversalCommands::cmdPipelineBarrier ( barriers (
        hImage.imageRef(), regions, Bar::NONE, Bar::TRANSFER
    ) );

    ::vkCmdClearDepthStencilImage (
        hCmdBuffer, hImage.imageRef().handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        & vkClearDepthStencilValue,
        static_cast< std::uint32_t >( regions.size() ),
        & regions [ 0 ] );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdResetQueryPool (
    const QueryPool& hPool,
    std::uint32_t first,
    std::uint32_t count,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    if ( count == 0 )
        count = hPool.count();

    ::vkCmdResetQueryPool ( hCmdBuffer, hPool.handle(), first, count );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdResetQueryPool (
    const QueryPool& hPool,
    CommandBuffer hCommandBuffer )
{
    cmdResetQueryPool ( hPool, 0, 0, hCommandBuffer );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyQueryPoolResults (
    const Buf& hDstBuffer,
    const QueryPool& hPool,
    VkQueryResultFlags flags,
    std::uint32_t count,
    std::uint32_t first,
    VkDeviceSize dstOffset,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const std::uint32_t stride =
        hPool.valueCount() * ( ( flags & VK_QUERY_RESULT_64_BIT ) ? 8 : 4 );

    if ( count == 0 )
        count = hPool.count();

    ::vkCmdCopyQueryPoolResults (
        hCmdBuffer, hPool.handle(), first, count, hDstBuffer.handle(),
        dstOffset, stride, flags );
}

// -----------------------------------------------------------------------------

void NonRenderingCommands :: cmdCopyQueryPoolResults (
    const Buf& hDstBuffer,
    const QueryPool& hPool,
    VkQueryResultFlags flags,
    CommandBuffer hCommandBuffer )
{
    cmdCopyQueryPoolResults (
        hDstBuffer, hPool, flags, 0, 0, 0, hCommandBuffer );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void UniversalCommands :: cmdExecuteCommands (
    const VkCommandBuffer* pBuffers,
    size_t size,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdExecuteCommands (
        hCmdBuffer, static_cast< std::uint32_t >( size ), pBuffers );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdExecuteCommands (
    const CommandBuffer* pBuffers,
    size_t size,
    CommandBuffer hCommandBuffer )
{
    std::vector< VkCommandBuffer > bufferHandles ( size );

    std::transform (
        pBuffers, pBuffers + size, bufferHandles.begin(),
        []( const CommandBuffer& buf ) { return buf.handle(); } );

    cmdExecuteCommands ( & bufferHandles [ 0 ], size, hCommandBuffer );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdExecuteCommands (
    const std::vector< CommandBuffer >& buffers,
    CommandBuffer hCommandBuffer )
{
    std::vector< VkCommandBuffer > bufferHandles ( buffers.size() );

    std::transform (
        buffers.begin(), buffers.end(), bufferHandles.begin(),
        []( const CommandBuffer& buf ) { return buf.handle(); } );

    cmdExecuteCommands ( & bufferHandles [ 0 ], buffers.size(), hCommandBuffer );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdWaitEvents (
    const std::vector< Event >& events,
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    const Barriers& barriers,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    std::vector< VkEvent > eventHandles ( events.size() );

    std::transform (
        events.begin(), events.end(), eventHandles.begin(),
        []( const Event& evt ) { return evt.handle(); } );

    ::vkCmdWaitEvents (
        hCmdBuffer,
        static_cast< std::uint32_t >( events.size() ),
        & eventHandles [ 0 ],
        srcStageMask,
        dstStageMask,
        barriers.d_memoryBarrierCount,
        barriers.d_pMemoryBarriers,
        barriers.d_bufferMemoryBarrierCount,
        barriers.d_pBufferMemoryBarriers,
        barriers.d_imageMemoryBarrierCount,
        barriers.d_pImageMemoryBarriers
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdPipelineBarrier (
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    VkDependencyFlags dependencyFlags,
    const Barriers& barriers,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdPipelineBarrier (
        hCmdBuffer,
        srcStageMask,
        dstStageMask,
        dependencyFlags,
        barriers.d_memoryBarrierCount,
        barriers.d_pMemoryBarriers,
        barriers.d_bufferMemoryBarrierCount,
        barriers.d_pBufferMemoryBarriers,
        barriers.d_imageMemoryBarrierCount,
        barriers.d_pImageMemoryBarriers
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdPipelineBarrier (
    const BarrierList& barriers,
    CommandBuffer hCommandBuffer )
{
    cmdPipelineBarrier ( barriers.d_sourceStage, barriers.d_targetStage, 0, barriers, hCommandBuffer );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdImagePipelineBarrier (
    const Img& image,
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    bool bByRegion,
    VkImageLayout oldLayout,
    VkImageLayout newLayout,
    int mipLevel,
    int arrayLayer,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    VkImageMemoryBarrier imgBarrier;

    imgBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgBarrier.pNext = 0;
    imgBarrier.srcAccessMask = srcAccessMask;
    imgBarrier.dstAccessMask = dstAccessMask;
    imgBarrier.oldLayout = oldLayout;
    imgBarrier.newLayout = newLayout;
    imgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imgBarrier.image = image.handle();

    const ImageInfo& imgInfo = image.info();

    imgBarrier.subresourceRange.aspectMask = imgInfo.getAspect();

    if ( mipLevel >= 0 ) 
    {
        imgBarrier.subresourceRange.baseMipLevel = static_cast< unsigned int >( mipLevel );
        imgBarrier.subresourceRange.levelCount = 1;
    }
    else
    {
        imgBarrier.subresourceRange.baseMipLevel = 0;
        imgBarrier.subresourceRange.levelCount = imgInfo.mipLevels;
    }

    if ( arrayLayer >= 0 )
    {
        imgBarrier.subresourceRange.baseArrayLayer = static_cast< unsigned int >( arrayLayer );
        imgBarrier.subresourceRange.layerCount = 1;
    }
    else
    {
        imgBarrier.subresourceRange.baseArrayLayer = 0;
        imgBarrier.subresourceRange.layerCount = imgInfo.arrayLayers;
    }

    ::vkCmdPipelineBarrier (
        hCmdBuffer,
        srcStageMask,
        dstStageMask,
        ( bByRegion ? VK_DEPENDENCY_BY_REGION_BIT : 0 ),
        0, 0, 0, 0, 1, & imgBarrier
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBufferPipelineBarrier (
    const Buf& hBuffer,
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    VkBufferMemoryBarrier bufBarrier;

    bufBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufBarrier.pNext = 0;
    bufBarrier.srcAccessMask = srcAccessMask;
    bufBarrier.dstAccessMask = dstAccessMask;
    bufBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufBarrier.buffer = hBuffer.handle();
    bufBarrier.offset = 0;
    bufBarrier.size = VK_WHOLE_SIZE;

    ::vkCmdPipelineBarrier (
        hCmdBuffer,
        srcStageMask,
        dstStageMask,
        0,
        0, 0, 1, & bufBarrier, 0, 0
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetViewport (
    const VkViewport& viewport,
    std::uint32_t index,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetViewport ( hCmdBuffer, index, 1u, & viewport );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetViewport (
    const std::vector< VkViewport >& viewports,
    std::uint32_t first,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetViewport (
        hCmdBuffer,
        first,
        static_cast< std::uint32_t >( viewports.size() ),
        & viewports [ 0 ]
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetLineWidth (
    float lineWidth,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetLineWidth ( hCmdBuffer, lineWidth );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetDepthBias (
    float constantFactor,
    float clamp,
    float slopeFactor,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetDepthBias ( hCmdBuffer, constantFactor, clamp, slopeFactor );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetDepthBounds (
    float minDepthBounds,
    float maxDepthBounds,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetDepthBounds ( hCmdBuffer, minDepthBounds, maxDepthBounds );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetScissor (
    const VkRect2D& scissor,
    std::uint32_t index,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetScissor ( hCmdBuffer, index, 1u, & scissor );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetScissor (
    const std::vector< VkRect2D >& scissors,
    std::uint32_t first,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetScissor (
        hCmdBuffer,
        first,
        static_cast< std::uint32_t >( scissors.size() ),
        & scissors [ 0 ]
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetStencilCompareMask (
    VkStencilFaceFlags faceMask,
    std::uint32_t compareMask,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetStencilCompareMask ( hCmdBuffer, faceMask, compareMask );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetStencilWriteMask ( 
    VkStencilFaceFlags faceMask,
    std::uint32_t writeMask,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetStencilWriteMask ( hCmdBuffer, faceMask, writeMask );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetStencilReference ( 
    VkStencilFaceFlags faceMask,
    std::uint32_t reference,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetStencilReference ( hCmdBuffer, faceMask, reference );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdSetBlendConstants (
    const float blendConstants [ 4 ],
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdSetBlendConstants ( hCmdBuffer, blendConstants );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBeginQuery (
    const QueryPool& hPool,
    std::uint32_t query,
    VkQueryControlFlags flags,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBeginQuery ( hCmdBuffer, hPool.handle(), query, flags );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdEndQuery (
    const QueryPool& hPool,
    std::uint32_t query,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdEndQuery ( hCmdBuffer, hPool.handle(), query );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdWriteTimestamp (
    const QueryPool& hPool,
    std::uint32_t query,
    VkPipelineStageFlagBits pipelineStage,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdWriteTimestamp ( hCmdBuffer, pipelineStage, hPool.handle(), query );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBindPipeline (
    const Pipeline& hPipeline,
    VkPipelineBindPoint pipelineBindPoint,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindPipeline ( hCmdBuffer, pipelineBindPoint, hPipeline.handle() );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBindDescriptorSets (
    const ShaderDataBlock& hDataBlock,
    CommandBuffer hCommandBuffer )
{
    hDataBlock.cmdBind ( hCommandBuffer );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBindIndexInput (
    const Buf& hBuffer,
    VkIndexType indexType,
    VkDeviceSize offset,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindIndexBuffer ( hCmdBuffer, hBuffer.handle(), offset, indexType );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBindVertexBuffers (
    const std::vector< Buf >& buffers,
    const std::vector< VkDeviceSize >& offsets,
    std::uint32_t first,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    std::vector< VkBuffer > bufferHandles ( buffers.size() );

    std::transform (
        buffers.begin(), buffers.end(), bufferHandles.begin(),
        []( const Buf& buf ) { return buf.handle(); } );

    ::vkCmdBindVertexBuffers (
        hCmdBuffer,
        first,
        static_cast< std::uint32_t >( std::min ( buffers.size(), offsets.size() ) ),
        & bufferHandles [ 0 ],
        & offsets [ 0 ]
    );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdBindVertexBuffers (
    const std::vector< Buf >& buffers,
    std::uint32_t first,
    CommandBuffer hCommandBuffer )
{
    std::vector< VkDeviceSize > offsets ( buffers.size() );
    cmdBindVertexBuffers ( buffers, offsets, first, hCommandBuffer );
}

// -----------------------------------------------------------------------------

void UniversalCommands :: cmdPushConstants (
    const PipelineLayoutBase& hPipelineLayout,
    VkShaderStageFlags stageFlags,
    const void* pValues, std::uint32_t size,
    std::uint32_t offset,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdPushConstants (
        hCmdBuffer, hPipelineLayout.handle(), stageFlags, offset, size, pValues );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ExtendedCommands :: cmdChangeImageLayout (
    const Img& hImage,
    VkImageLayout oldImageLayout, 
    VkImageLayout newImageLayout,
    std::uint32_t levelCount,
    std::uint32_t layerCount,
    std::uint32_t baseMipLevel,
    std::uint32_t baseArrayLayer,
    VkImageAspectFlags aspectMask,
    CommandBuffer hCommandBuffer )
{
    std::vector< VkImageMemoryBarrier > barriers ( 1 );
    VkImageMemoryBarrier& imageMemoryBarrier = barriers [ 0 ];

    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = 0;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = hImage.handle();
    imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = baseArrayLayer;
    imageMemoryBarrier.subresourceRange.baseMipLevel = baseMipLevel;
    imageMemoryBarrier.subresourceRange.layerCount = layerCount;
    imageMemoryBarrier.subresourceRange.levelCount = levelCount;
    
    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    switch ( oldImageLayout )
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            imageMemoryBarrier.srcAccessMask = 0;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_HOST_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            break;
    }

    VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    switch ( newImageLayout )
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            destStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            destStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = 
                VK_ACCESS_INPUT_ATTACHMENT_READ_BIT
                | VK_ACCESS_SHADER_READ_BIT
                | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

            destStageFlags = 
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
                | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            break;
    }

    cmdPipelineBarrier ( srcStageFlags, destStageFlags, 0, barriers, hCommandBuffer );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
