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
#include "../include/vppBarriers.hpp"
#include "../include/vppBuffer.hpp"
#include "../include/vppImage.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

MemoryBarrier :: MemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask ) :
        VkMemoryBarrier
        {
            VK_STRUCTURE_TYPE_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask
        },
        Barriers ( static_cast< const VkMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

BufferMemoryBarrier :: BufferMemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    const Buf& buffer,
    VkDeviceSize offset,
    VkDeviceSize size,
    uint32_t srcQueueFamilyIndex,
    uint32_t dstQueueFamilyIndex ) :
        VkBufferMemoryBarrier
        {
            VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask,
            srcQueueFamilyIndex, dstQueueFamilyIndex,
            buffer.handle(), offset, size
        },
        Barriers ( static_cast< const VkBufferMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

ImageMemoryBarrier :: ImageMemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    VkImageLayout oldLayout,
    VkImageLayout newLayout,
    const Img& image,
    std::uint32_t baseArrayLayer,
    std::uint32_t layerCount,
    std::uint32_t baseMipLevel,
    std::uint32_t levelCount,
    VkImageAspectFlags aspectMask,
    uint32_t srcQueueFamilyIndex,
    uint32_t dstQueueFamilyIndex ) :
        VkImageMemoryBarrier
        {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask,
            oldLayout, newLayout,
            srcQueueFamilyIndex, dstQueueFamilyIndex,
            image.handle(),
            VkImageSubresourceRange {
                aspectMask, baseMipLevel, levelCount, baseArrayLayer, layerCount }
        },
        Barriers ( static_cast< const VkImageMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
