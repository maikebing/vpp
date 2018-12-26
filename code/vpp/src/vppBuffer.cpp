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
#include "../include/vppBuffer.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

std::uint32_t Buf :: barrierDestStageHint() const
{
    const std::uint32_t u = 
        getUsage() & ~VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch ( u )
    {
        case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT:
            return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

        case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT:
        case VK_BUFFER_USAGE_INDEX_BUFFER_BIT:
            return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

        case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT:
        case VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT:
        case VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT:
        case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT:
            return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

        default:
            return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }
}

// -----------------------------------------------------------------------------

std::uint32_t Buf :: barrierDestAccessHint() const
{
    const std::uint32_t u = 
        getUsage() & ~VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch ( u )
    {
        case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT:
            return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

        case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT:
            return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

        case VK_BUFFER_USAGE_INDEX_BUFFER_BIT:
            return VK_ACCESS_INDEX_READ_BIT;

        case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT:
            return VK_ACCESS_UNIFORM_READ_BIT;

        case VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT:
        case VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT:
        case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT:
            return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;

        default:
            return VK_ACCESS_MEMORY_READ_BIT;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

BufferImpl :: BufferImpl (
    VkDeviceSize size,
    std::uint32_t usageMask,
    const Device& hDevice,
    const std::vector< std::uint32_t >& queueFamilyIndices,
    std::uint32_t flags ) :
        d_hDevice ( hDevice ),
        d_size ( size ),
        d_usage ( usageMask )
{
    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = 0;
    bufferCreateInfo.flags = flags;
    bufferCreateInfo.usage = usageMask;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    bufferCreateInfo.size = size;
    bufferCreateInfo.queueFamilyIndexCount = static_cast< std::uint32_t >( queueFamilyIndices.size() );
    bufferCreateInfo.pQueueFamilyIndices = & queueFamilyIndices [ 0 ];

    if ( ::vkCreateBuffer (
            hDevice.handle(), & bufferCreateInfo, 0, & d_handle ) != VK_SUCCESS )
    {
        throw XMemoryAllocationError();
    }
}

// -----------------------------------------------------------------------------

BufferImpl :: BufferImpl (
    VkDeviceSize size,
    std::uint32_t usageMask,
    const Device& hDevice,
    std::uint32_t flags ) :
        d_hDevice ( hDevice ),
        d_size ( size ),
        d_usage ( usageMask )
{
    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = 0;
    bufferCreateInfo.flags = flags;
    bufferCreateInfo.usage = usageMask;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.size = size;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = 0;

    if ( ::vkCreateBuffer (
           hDevice.handle(), & bufferCreateInfo, 0, & d_handle ) != VK_SUCCESS )
    {
        throw XMemoryAllocationError();
    }
}

// -----------------------------------------------------------------------------

BufferImpl :: ~BufferImpl()
{
    ::vkDestroyBuffer ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
