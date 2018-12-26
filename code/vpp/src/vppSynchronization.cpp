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
#include "../include/vppSynchronization.hpp"
#include "../include/vppBarriers.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

void Fence :: reset ( std::vector< Fence >* pFences )
{
    if ( pFences->empty() )
        return;

    std::vector< VkFence > fenceHandles ( pFences->size() );

    std::transform (
        pFences->begin(), pFences->end(), fenceHandles.begin(),
        []( const Fence& fence ){ return fence.handle(); } );

    ::vkResetFences (
        pFences->front().device().handle(),
        static_cast< std::uint32_t >( pFences->size() ),
        & fenceHandles [ 0 ] );
}

// -----------------------------------------------------------------------------

bool Fence :: waitAll ( std::vector< Fence >* pFences, std::uint64_t timeoutNs )
{
    if ( pFences->empty() )
        return true;

    std::vector< VkFence > fenceHandles ( pFences->size() );

    std::transform (
        pFences->begin(), pFences->end(), fenceHandles.begin(),
        []( const Fence& fence ){ return fence.handle(); } );

    const VkResult result = ::vkWaitForFences (
        pFences->front().device().handle(),
        static_cast< std::uint32_t >( pFences->size() ),
        & fenceHandles [ 0 ], VK_TRUE, timeoutNs );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

bool Fence :: waitOne ( std::vector< Fence >* pFences, std::uint64_t timeoutNs )
{
    if ( pFences->empty() )
        return true;

    std::vector< VkFence > fenceHandles ( pFences->size() );

    std::transform (
        pFences->begin(), pFences->end(), fenceHandles.begin(),
        []( const Fence& fence ){ return fence.handle(); } );

    const VkResult result = ::vkWaitForFences (
        pFences->front().device().handle(),
        static_cast< std::uint32_t >( pFences->size() ),
        & fenceHandles [ 0 ], VK_FALSE, timeoutNs );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

bool Fence :: waitAll (
    const Device& hDevice,
    std::vector< VkFence >* pFences,
    std::uint64_t timeoutNs )
{
    if ( pFences->empty() )
        return true;

    const VkResult result = ::vkWaitForFences (
        hDevice.handle(),
        static_cast< std::uint32_t >( pFences->size() ),
        & ( *pFences )[ 0 ], VK_TRUE, timeoutNs );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

bool Fence :: waitOne (
    const Device& hDevice,
    std::vector< VkFence >* pFences,
    std::uint64_t timeoutNs )
{
    if ( pFences->empty() )
        return true;

    const VkResult result = ::vkWaitForFences (
        hDevice.handle(),
        static_cast< std::uint32_t >( pFences->size() ),
        & ( *pFences )[ 0 ], VK_FALSE, timeoutNs );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void Event :: cmdSignal (
    bool bSignal,
    VkPipelineStageFlags stageMask,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const VkEvent hEvent = get()->d_handle;

    if ( bSignal )
        ::vkCmdSetEvent ( hCmdBuffer, hEvent, stageMask );
    else
        ::vkCmdResetEvent ( hCmdBuffer, hEvent, stageMask );
}

// -----------------------------------------------------------------------------

void Event :: cmdWait ( 
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const VkEvent hEvent = get()->d_handle;

    ::vkCmdWaitEvents (
        hCommandBuffer.handle(),
        1u,
        & hEvent,
        srcStageMask,
        dstStageMask,
        0, 0, 0, 0, 0, 0
    );
}

// -----------------------------------------------------------------------------

void Event :: cmdWait ( 
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    const Barriers& barriers,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    const VkEvent hEvent = get()->d_handle;

    ::vkCmdWaitEvents (
        hCommandBuffer.handle(),
        1u,
        & hEvent,
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
} // namespace vpp
// -----------------------------------------------------------------------------
