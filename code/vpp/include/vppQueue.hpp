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

#ifndef INC_VPPQUEUE_HPP
#define INC_VPPQUEUE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

#ifndef INC_VPPSYNCHRONIZATION_HPP
#include "vppSynchronization.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class QueueImpl;

// -----------------------------------------------------------------------------

class Queue : public TSharedReference< QueueImpl >
{
public:
    Queue();

    explicit Queue (
        const Device& hDevice,
        std::uint32_t iQueue = 0,
        EQueueType eQueue = Q_GRAPHICS );

    Queue (
        std::uint32_t iFamily,
        const Device& hDevice,
        std::uint32_t iQueue );

    VkQueue handle() const;
    const Device& device() const;
    EQueueType type() const;

    operator bool() const;

    void submit (
        const CommandBuffer& singleBuffer,
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        const Fence& signalFenceOnEnd = Fence() ) const;

    void submit (
        const std::vector< CommandBuffer > buffers,
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        const Fence& signalFenceOnEnd = Fence() ) const;

    VkResult waitForIdle();
};

// -----------------------------------------------------------------------------

class QueueImpl : public TSharedObject< QueueImpl >
{
public:
    QueueImpl (
        const Device& hDevice,
        std::uint32_t iQueue,
        EQueueType eQueue );

    QueueImpl (
        std::uint32_t iFamily,
        const Device& hDevice,
        std::uint32_t iQueue );

private:
    friend class Queue;

    Device d_hDevice;
    VkQueue d_handle;
    EQueueType d_type;

    VPP_EXTSYNC_MTX_DECLARE;
};

// -----------------------------------------------------------------------------

VPP_INLINE QueueImpl :: QueueImpl (
    const Device& hDevice,
    std::uint32_t iQueue,
    EQueueType eQueue ) :
        d_hDevice ( hDevice ),
        d_type ( eQueue )
{
    ::vkGetDeviceQueue (
        d_hDevice.handle(),
        hDevice.queueFamily ( eQueue ),
        iQueue, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE QueueImpl :: QueueImpl (
    std::uint32_t iFamily,
    const Device& hDevice,
    std::uint32_t iQueue ) :
        d_hDevice ( hDevice )
{
    ::vkGetDeviceQueue (
        d_hDevice.handle(),
        iFamily, iQueue, & d_handle );

    if ( iFamily == hDevice.queueFamily ( Q_TRANSFER ) )
        d_type = Q_TRANSFER;
    else
        d_type = Q_GRAPHICS;
}

// -----------------------------------------------------------------------------

VPP_INLINE Queue :: Queue()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Queue :: Queue (
    const Device& hDevice,
    std::uint32_t iQueue,
    EQueueType eQueue ) :
        TSharedReference< QueueImpl > (
            new QueueImpl ( hDevice, iQueue, eQueue ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Queue :: Queue (
    std::uint32_t iFamily,
    const Device& hDevice,
    std::uint32_t iQueue ) :
        TSharedReference< QueueImpl > (
            new QueueImpl ( iFamily, hDevice, iQueue ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkQueue Queue :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Queue :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE EQueueType Queue :: type() const
{
    return get()->d_type;
}

// -----------------------------------------------------------------------------

VPP_INLINE Queue :: operator bool() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult Queue :: waitForIdle()
{
    return ::vkQueueWaitIdle ( get()->d_handle );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPQUEUE_HPP
