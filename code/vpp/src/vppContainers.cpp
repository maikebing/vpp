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
#include "../include/vppContainers.hpp"
#include "../include/vppCommands.hpp"
#include "../include/vppImage.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

VectorBase :: VectorBase (
    size_t memorySize,
    std::uint32_t usage,
    MemProfile::ECharacteristic memProfile,
    Buf* pBuffer,
    DeviceMemory* pMemory,
    const Device& hDevice ) :
        d_memorySize ( memorySize ),
        d_memProfile ( memProfile ),
        d_pBuffer ( pBuffer ),
        d_pMemory ( pMemory ),
        d_device ( hDevice )
{
    if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        std::uint32_t localBufferUsage = Buf::SOURCE;

        if ( usage & Buf::SOURCE )
            localBufferUsage |= Buf::TARGET;

        d_localBuffer = Buf ( memorySize, localBufferUsage, hDevice );

        d_localMemoryBinding =
            LocalMemoryBinding ( d_localBuffer, MemProfile::HOST_STATIC );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: commit (
    EQueueType eQueue,
    const Fence& signalFenceOnEnd,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd )
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    Device hDevice = d_device;
    Queue queue ( hDevice, 0, eQueue );
    CommandBuffer& pushCmdBuffer = d_commitCmdBuffer [ eQueue ];

    if ( ! pushCmdBuffer )
    {
        pushCmdBuffer = hDevice.defaultCmdPool ( eQueue ).createBuffer();
        pushCmdBuffer.begin();
        syncHostToDevice ( pushCmdBuffer.handle(), 0, d_memorySize );
        pushCmdBuffer.end();
    }

    queue.submit ( pushCmdBuffer, waitOnBegin, signalOnEnd, signalFenceOnEnd );
}

// -----------------------------------------------------------------------------

void VectorBase :: commitAndWait ( EQueueType eQueue )
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    Fence fence ( d_device );
    commit ( eQueue, fence );
    fence.wait();
}

// -----------------------------------------------------------------------------

void VectorBase :: load (
    EQueueType eQueue,
    const Fence& signalFenceOnEnd,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd )
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    Device hDevice = d_device;
    Queue queue ( hDevice, 0, eQueue );
    CommandBuffer& pushCmdBuffer = d_loadCmdBuffer [ eQueue ];

    if ( ! pushCmdBuffer )
    {
        pushCmdBuffer = hDevice.defaultCmdPool ( eQueue ).createBuffer();
        pushCmdBuffer.begin();
        syncDeviceToHost ( pushCmdBuffer.handle(), 0, d_memorySize );
        pushCmdBuffer.end();
    }

    queue.submit ( pushCmdBuffer, waitOnBegin, signalOnEnd, signalFenceOnEnd );
}

// -----------------------------------------------------------------------------

void VectorBase :: loadAndWait ( EQueueType eQueue )
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    Fence fence ( d_device );
    load ( eQueue, fence );
    fence.wait();
}

// -----------------------------------------------------------------------------

void VectorBase :: flushHostToDevice (
    VkCommandBuffer hCmdBuffer, const DeviceMemory& mem ) 
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    if ( ! mem.isHostCoherent() )
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = 0;
        memoryRange.memory = mem.handle();
        memoryRange.offset = 0;
        memoryRange.size = d_memorySize;

        ::vkFlushMappedMemoryRanges ( d_device.handle(), 1, & memoryRange );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: flushDeviceToHost (
    VkCommandBuffer hCmdBuffer, const DeviceMemory& mem ) 
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;

    if ( ! mem.isHostCoherent() )
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = 0;
        memoryRange.memory = mem.handle();
        memoryRange.offset = 0;
        memoryRange.size = d_memorySize;

        ::vkInvalidateMappedMemoryRanges ( d_device.handle(), 1, & memoryRange );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: syncHostToDevice (
    VkCommandBuffer hCmdBuffer, VkDeviceSize offset, VkDeviceSize size ) 
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;
    else if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        flushHostToDevice ( hCmdBuffer, d_localMemoryBinding.memory() );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );

        VkBufferCopy vkBufferCopy;
        vkBufferCopy.srcOffset = offset;
        vkBufferCopy.dstOffset = offset;
        vkBufferCopy.size = size;

        ::vkCmdCopyBuffer (
            hCmdBuffer, d_localBuffer.handle(),
            d_pBuffer->handle(), 1u, & vkBufferCopy );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, d_pBuffer->barrierDestStageHint(),
            VK_ACCESS_TRANSFER_WRITE_BIT, d_pBuffer->barrierDestAccessHint(),
            hCmdBuffer );
    }
    else if ( ! d_pMemory->isHostCoherent() )
    {
        flushHostToDevice ( hCmdBuffer, *d_pMemory );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_HOST_BIT, d_pBuffer->barrierDestStageHint(),
            VK_ACCESS_HOST_WRITE_BIT, d_pBuffer->barrierDestAccessHint(),
            hCmdBuffer );
    }
    else
    {
        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_HOST_BIT, d_pBuffer->barrierDestStageHint(),
            VK_ACCESS_HOST_WRITE_BIT, d_pBuffer->barrierDestAccessHint(),
            hCmdBuffer );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: syncDeviceToHost (
    VkCommandBuffer hCmdBuffer, VkDeviceSize offset, VkDeviceSize size ) 
{
    if ( d_memProfile == MemProfile::DEVICE_ONLY )
        return;
    else if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );

        VkBufferCopy vkBufferCopy;
        vkBufferCopy.srcOffset = offset;
        vkBufferCopy.dstOffset = offset;
        vkBufferCopy.size = size;

        ::vkCmdCopyBuffer (
            hCmdBuffer, d_pBuffer->handle(),
            d_localBuffer.handle(), 1u, & vkBufferCopy );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT,
            hCmdBuffer );

        flushDeviceToHost ( hCmdBuffer, d_localMemoryBinding.memory() );
    }
    else
    {
        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_HOST_BIT,
            VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
            hCmdBuffer );

        flushDeviceToHost ( hCmdBuffer, *d_pMemory );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: cmdCopyToImage (
    CommandBuffer hCmdBuffer,
    const TDImg& img,
    VkImageLayout targetLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    const auto& imgInfo = img.imageRef().info();
    const std::uint32_t aspectMask = imgInfo.getAspect();

    ExtendedCommands::cmdChangeImageLayout (
        img.imageRef(),
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, 1, mipLevel, layer, aspectMask, hCmdBuffer );

    if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        flushHostToDevice ( hCmdBuffer.handle(), d_localMemoryBinding.memory() );

        UniversalCommands::cmdBufferPipelineBarrier (
            d_localBuffer,
            VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );

        VkBufferCopy vkBufferCopy;
        vkBufferCopy.srcOffset = 0;
        vkBufferCopy.dstOffset = 0;
        vkBufferCopy.size = d_memorySize;

        ::vkCmdCopyBuffer (
            hCmdBuffer.handle(), d_localBuffer.handle(),
            d_pBuffer->handle(), 1u, & vkBufferCopy );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );
    }
    else
    {
        flushHostToDevice ( hCmdBuffer.handle(), *d_pMemory );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );
    }

    VkBufferImageCopy copyInfo;
    copyInfo.bufferOffset = bufferOffset;
    copyInfo.bufferRowLength = bufferRowLength;
    copyInfo.bufferImageHeight = bufferImageHeight;
   
    // uwaga na depth i stencil jednoczesnie!!
    copyInfo.imageSubresource.aspectMask = aspectMask;
    copyInfo.imageSubresource.mipLevel = mipLevel;
    copyInfo.imageSubresource.baseArrayLayer = layer;
    copyInfo.imageSubresource.layerCount = 1;

    copyInfo.imageOffset = imageOffset;

    if ( imageExtent.width == 0 || imageExtent.height == 0 || imageExtent.depth == 0 )
        copyInfo.imageExtent = imgInfo.extent;
    else
        copyInfo.imageExtent = imageExtent;

    ::vkCmdCopyBufferToImage (
        hCmdBuffer.handle(), d_pBuffer->handle(), img.imageRef().handle(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, & copyInfo );

    if ( targetLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
        UniversalCommands::cmdImagePipelineBarrier (
            img.imageRef(),
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_ACCESS_SHADER_READ_BIT,
            false,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            targetLayout,
            static_cast< int >( mipLevel ),
            static_cast< int >( layer ),
            hCmdBuffer
        );
}

// -----------------------------------------------------------------------------

void VectorBase :: copyToImage (
    EQueueType eQueue,
    const TDImg& img,
    VkImageLayout targetLayout,
    const Fence& signalFenceOnEnd,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    Device hDevice = d_device;
    Queue queue ( hDevice, 0, eQueue );
    CommandBuffer& pushCmdBuffer = d_copyCmdBuffer [ eQueue ];

    if ( ! pushCmdBuffer )
        pushCmdBuffer = hDevice.defaultCmdPool ( eQueue ).createBuffer();
    else
        pushCmdBuffer.reset();

    pushCmdBuffer.begin();

    cmdCopyToImage (
        pushCmdBuffer, img, targetLayout,
        mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );
        
    pushCmdBuffer.end();

    queue.submit ( pushCmdBuffer, waitOnBegin, signalOnEnd, signalFenceOnEnd );
}

// -----------------------------------------------------------------------------

void VectorBase :: copyToImageAndWait (
    EQueueType eQueue,
    const TDImg& img,
    VkImageLayout targetLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    Fence fence ( d_device );

    copyToImage (
        eQueue, img, targetLayout, fence, Semaphore(), Semaphore(),
        mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );

    fence.wait();
}

// -----------------------------------------------------------------------------

void VectorBase :: cmdCopyFromImage (
    CommandBuffer hCmdBuffer,
    const TSImg& img,
    VkImageLayout sourceImageLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    const auto& imgInfo = img.imageRef().info();
    const std::uint32_t aspectMask = imgInfo.getAspect();

    if ( sourceImageLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL )
        UniversalCommands::cmdImagePipelineBarrier (
            img.imageRef(),
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            false, sourceImageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            static_cast< int >( mipLevel ),
            static_cast< int >( layer ),
            hCmdBuffer
        );

    VkBufferImageCopy copyInfo;
    copyInfo.bufferOffset = bufferOffset;
    copyInfo.bufferRowLength = bufferRowLength;
    copyInfo.bufferImageHeight = bufferImageHeight;
   
    // uwaga na depth i stencil jednoczesnie!!
    copyInfo.imageSubresource.aspectMask = aspectMask;
    copyInfo.imageSubresource.mipLevel = mipLevel;
    copyInfo.imageSubresource.baseArrayLayer = layer;
    copyInfo.imageSubresource.layerCount = 1;

    copyInfo.imageOffset = imageOffset;

    if ( imageExtent.width == 0 || imageExtent.height == 0 || imageExtent.depth == 0 )
        copyInfo.imageExtent = imgInfo.extent;
    else
        copyInfo.imageExtent = imageExtent;

    ::vkCmdCopyImageToBuffer (
        hCmdBuffer.handle(), img.imageRef().handle(),
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, d_pBuffer->handle(),
        1, & copyInfo );

    if ( sourceImageLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL )
        UniversalCommands::cmdImagePipelineBarrier (
            img.imageRef(),
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT,
            false, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, sourceImageLayout,
            static_cast< int >( mipLevel ),
            static_cast< int >( layer ),
            hCmdBuffer
        );

    if ( d_memProfile != MemProfile::DEVICE_STATIC )
    {
        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
            hCmdBuffer );

        flushDeviceToHost ( hCmdBuffer.handle(), *d_pMemory );
    }
    else
    {
        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
            hCmdBuffer );

        VkBufferCopy vkBufferCopy;
        vkBufferCopy.srcOffset = 0;
        vkBufferCopy.dstOffset = 0;
        vkBufferCopy.size = d_memorySize;

        ::vkCmdCopyBuffer (
            hCmdBuffer.handle(), d_pBuffer->handle(),
            d_localBuffer.handle(), 1u, & vkBufferCopy );

        UniversalCommands::cmdBufferPipelineBarrier (
            *d_pBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT,
            hCmdBuffer );

        flushDeviceToHost ( hCmdBuffer.handle(), d_localMemoryBinding.memory() );
    }
}

// -----------------------------------------------------------------------------

void VectorBase :: copyFromImage (
    EQueueType eQueue,
    const TSImg& img,
    VkImageLayout sourceImageLayout,
    const Fence& signalFenceOnEnd,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    Device hDevice = d_device;
    Queue queue ( hDevice, 0, eQueue );
    CommandBuffer& pushCmdBuffer = d_copyCmdBuffer [ eQueue ];

    if ( ! pushCmdBuffer )
        pushCmdBuffer = hDevice.defaultCmdPool ( eQueue ).createBuffer();
    else
        pushCmdBuffer.reset();

    pushCmdBuffer.begin();

    cmdCopyFromImage (
        pushCmdBuffer, img, sourceImageLayout, 
        mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );
        
    pushCmdBuffer.end();

    queue.submit ( pushCmdBuffer, waitOnBegin, signalOnEnd, signalFenceOnEnd );
}

// -----------------------------------------------------------------------------

void VectorBase :: copyFromImageAndWait (
    EQueueType eQueue,
    const TSImg& img,
    VkImageLayout sourceImageLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    Fence fence ( d_device );

    copyFromImage (
        eQueue, img, sourceImageLayout, fence, Semaphore(), Semaphore(),
        mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );

    fence.wait();
}

// -----------------------------------------------------------------------------

std::uint32_t VectorBase :: getAdditionalUsage (
    MemProfile::ECharacteristic memProfile )
{
    switch ( memProfile )
    {
        case MemProfile::DEVICE_STATIC:
        case MemProfile::DEVICE_ONLY:
            return Buf::SOURCE | Buf::TARGET;

        default:
            return 0;
    }
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
