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
#include "../include/vppCommandPool.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

CommandPoolImpl :: CommandPoolImpl (
    const Device& hDevice, EQueueType queueType, std::uint32_t flags ) :
    d_hDevice ( hDevice ),
    d_handle(),
    d_result()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = 0;
    commandPoolCreateInfo.flags = flags;
    commandPoolCreateInfo.queueFamilyIndex = hDevice.queueFamily ( queueType ); 

    d_result = ::vkCreateCommandPool (
        d_hDevice.handle(), & commandPoolCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

CommandPoolImpl :: ~CommandPoolImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyCommandPool ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VkResult CommandPool :: createBuffers (
    std::uint32_t count,
    std::vector< CommandBuffer >* pBuffers,
    EBufferLevel level )
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = 0;
    commandBufferAllocateInfo.commandPool = get()->d_handle;
    commandBufferAllocateInfo.commandBufferCount = count;
    commandBufferAllocateInfo.level = (
        level == PRIMARY ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY );

    std::vector< VkCommandBuffer > bufferHandles ( count );

    VkResult result = ::vkAllocateCommandBuffers (
        get()->d_hDevice.handle(), & commandBufferAllocateInfo, & bufferHandles [ 0 ] );

    if ( result == VK_SUCCESS )
    {
        pBuffers->reserve ( count );

        for ( const auto& hBuffer : bufferHandles )
            pBuffers->emplace_back ( hBuffer );
    }

    return result;
}

// -----------------------------------------------------------------------------

void CommandPool :: freeBuffers (
    const std::vector< CommandBuffer >& buffers )
{
    std::vector< VkCommandBuffer > bufferHandles;
    bufferHandles.resize ( buffers.size() );

    std::transform (
        buffers.begin(), buffers.end(), bufferHandles.begin(),
        []( const CommandBuffer& buffer ) { return buffer.handle(); } );

    ::vkFreeCommandBuffers (
        get()->d_hDevice.handle(), get()->d_handle,
        static_cast< std::uint32_t >( bufferHandles.size() ), & bufferHandles [ 0 ] );
}

// -----------------------------------------------------------------------------

CommandBuffer CommandPool :: createBuffer ( EBufferLevel level )
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = 0;
    commandBufferAllocateInfo.commandPool = get()->d_handle;
    commandBufferAllocateInfo.commandBufferCount = 1u;
    commandBufferAllocateInfo.level = (
        level == PRIMARY ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY );

    VkCommandBuffer bufferHandle = VK_NULL_HANDLE;

    VkResult result = ::vkAllocateCommandBuffers (
        get()->d_hDevice.handle(), & commandBufferAllocateInfo, & bufferHandle );

    if ( result == VK_SUCCESS )
        return CommandBuffer ( bufferHandle );
    else
        return CommandBuffer();
}

// -----------------------------------------------------------------------------

void CommandPool :: freeBuffer ( const CommandBuffer& buffer )
{
    const VkCommandBuffer bufferHandle = buffer.handle();

    ::vkFreeCommandBuffers (
        get()->d_hDevice.handle(), get()->d_handle, 1u, & bufferHandle );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
