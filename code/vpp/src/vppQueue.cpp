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
#include "../include/vppQueue.hpp"
#include "../include/vppCommandBuffer.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

void Queue :: submit (
    const CommandBuffer& singleBuffer,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd,
    const Fence& signalFenceOnEnd ) const
{
    VkCommandBuffer hBuffer = singleBuffer.handle();
    VkSemaphore hWaitOnBegin = VK_NULL_HANDLE;
    VkSemaphore hSignalOnEnd = VK_NULL_HANDLE;
    VkFence hSignalFenceOnEnd = VK_NULL_HANDLE;
    VkPipelineStageFlags stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    VkSubmitInfo vkSubmitInfo;
    vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkSubmitInfo.pNext = 0;

    if ( waitOnBegin )
    {
        hWaitOnBegin = waitOnBegin.handle();
        vkSubmitInfo.waitSemaphoreCount = 1;
        vkSubmitInfo.pWaitSemaphores = & hWaitOnBegin;
        vkSubmitInfo.pWaitDstStageMask = & stageFlags;
    }
    else
    {
        vkSubmitInfo.waitSemaphoreCount = 0;
        vkSubmitInfo.pWaitSemaphores = 0;
        vkSubmitInfo.pWaitDstStageMask = 0;
    }

    vkSubmitInfo.commandBufferCount = 1;
    vkSubmitInfo.pCommandBuffers = & hBuffer;

    if ( signalOnEnd )
    {
        hSignalOnEnd = signalOnEnd.handle();
        vkSubmitInfo.signalSemaphoreCount = 1;
        vkSubmitInfo.pSignalSemaphores = & hSignalOnEnd;
    }
    else
    {
        vkSubmitInfo.signalSemaphoreCount = 0;
        vkSubmitInfo.pSignalSemaphores = 0;
    }

    if ( signalFenceOnEnd )
    {
        VPP_EXTSYNC_MTX_LOCK ( signalFenceOnEnd.get() );
        hSignalFenceOnEnd = signalFenceOnEnd.handle();
    }
    
    {
        VPP_EXTSYNC_MTX_SLOCK ( get() );
        ::vkQueueSubmit ( get()->d_handle, 1, & vkSubmitInfo, hSignalFenceOnEnd );
    }

    if ( signalFenceOnEnd )
        VPP_EXTSYNC_MTX_UNLOCK ( signalFenceOnEnd.get() );
}

// -----------------------------------------------------------------------------

void Queue :: submit (
    const std::vector< CommandBuffer > buffers,
    const Semaphore& waitOnBegin,
    const Semaphore& signalOnEnd,
    const Fence& signalFenceOnEnd ) const
{
    VkSemaphore hWaitOnBegin = VK_NULL_HANDLE;
    VkSemaphore hSignalOnEnd = VK_NULL_HANDLE;
    VkFence hSignalFenceOnEnd = VK_NULL_HANDLE;
    VkPipelineStageFlags stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    VkSubmitInfo vkSubmitInfo;
    vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkSubmitInfo.pNext = 0;

    if ( waitOnBegin )
    {
        hWaitOnBegin = waitOnBegin.handle();
        vkSubmitInfo.waitSemaphoreCount = 1;
        vkSubmitInfo.pWaitSemaphores = & hWaitOnBegin;
        vkSubmitInfo.pWaitDstStageMask = & stageFlags;
    }
    else
    {
        vkSubmitInfo.waitSemaphoreCount = 0;
        vkSubmitInfo.pWaitSemaphores = 0;
        vkSubmitInfo.pWaitDstStageMask = 0;
    }

    std::vector< VkCommandBuffer > handles ( buffers.size() );

    std::transform (
        buffers.begin(), buffers.end(),
        handles.begin(), []( const CommandBuffer& buf ) { return buf.handle(); } );

    vkSubmitInfo.commandBufferCount = static_cast< std::uint32_t >( buffers.size() );
    vkSubmitInfo.pCommandBuffers = & handles [ 0 ];

    if ( signalOnEnd )
    {
        hSignalOnEnd = signalOnEnd.handle();
        vkSubmitInfo.signalSemaphoreCount = 1;
        vkSubmitInfo.pSignalSemaphores = & hSignalOnEnd;
    }
    else
    {
        vkSubmitInfo.signalSemaphoreCount = 0;
        vkSubmitInfo.pSignalSemaphores = 0;
    }

    if ( signalFenceOnEnd )
    {
        VPP_EXTSYNC_MTX_LOCK ( signalFenceOnEnd.get() );
        hSignalFenceOnEnd = signalFenceOnEnd.handle();
    }

    {
        VPP_EXTSYNC_MTX_SLOCK ( get() );
        ::vkQueueSubmit ( get()->d_handle, 1, & vkSubmitInfo, hSignalFenceOnEnd );
    }

    if ( signalFenceOnEnd )
        VPP_EXTSYNC_MTX_UNLOCK ( signalFenceOnEnd.get() );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
