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

#ifndef INC_VPPSYNCHRONIZATION_HPP
#define INC_VPPSYNCHRONIZATION_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

static const std::uint64_t NO_TIMEOUT = 18446744073709551615ull;

// -----------------------------------------------------------------------------

class KFenceImpl;

// -----------------------------------------------------------------------------

class Fence : public TSharedReference< KFenceImpl >
{
public:
    Fence();
    Fence ( const Device& hDevice, bool bSignaled = false );

    VkFence handle() const;
    const Device& device() const;

    bool isSignaled() const;

    void reset();
    static void reset ( std::vector< Fence >* pFences );

    bool wait ( std::uint64_t timeoutNs = NO_TIMEOUT ) const;

    static bool waitAll (
        std::vector< Fence >* pFences,
        std::uint64_t timeoutNs = NO_TIMEOUT );

    static bool waitOne (
        std::vector< Fence >* pFences,
        std::uint64_t timeoutNs = NO_TIMEOUT );

    static bool waitAll (
        const Device& hDevice,
        std::vector< VkFence >* pFences,
        std::uint64_t timeoutNs = NO_TIMEOUT );

    static bool waitOne (
        const Device& hDevice,
        std::vector< VkFence >* pFences,
        std::uint64_t timeoutNs = NO_TIMEOUT );
};

// -----------------------------------------------------------------------------

class KFenceImpl : public TSharedObject< KFenceImpl >
{
public:
    KFenceImpl ( const Device& hDevice, bool bSignaled );
    ~KFenceImpl();

    VPP_EXTSYNC_METHODS_DECLARE ( this );

private:
    friend class Fence;
    Device d_hDevice;
    VkFence d_handle;
    VkResult d_result;

    VPP_EXTSYNC_MTX_DECLARE;
};

// -----------------------------------------------------------------------------

VPP_INLINE KFenceImpl :: KFenceImpl ( const Device& hDevice, bool bSignaled ) :
    d_hDevice ( hDevice ),
    d_handle(),
    d_result()
{
    VkFenceCreateInfo vkFenceCreateInfo;
    vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkFenceCreateInfo.pNext = 0;
    vkFenceCreateInfo.flags = ( bSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0 );

    d_result = ::vkCreateFence (
        d_hDevice.handle(), & vkFenceCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KFenceImpl :: ~KFenceImpl()
{
    VPP_EXTSYNC_MTX_SLOCK ( this );

    if ( d_result == VK_SUCCESS )
    {
        ::vkDestroyFence ( d_hDevice.handle(), d_handle, 0 );
        d_result = VK_NOT_READY;
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE Fence :: Fence()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Fence :: Fence ( const Device& hDevice, bool bSignaled ) :
    TSharedReference< KFenceImpl >( new KFenceImpl ( hDevice, bSignaled ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkFence Fence :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Fence :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Fence :: isSignaled() const
{
    return ::vkGetFenceStatus (
        get()->d_hDevice.handle(), get()->d_handle ) == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Fence :: reset()
{
    ::vkResetFences ( get()->d_hDevice.handle(), 1, & get()->d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Fence :: wait (
    std::uint64_t timeoutNs ) const
{
    const VkResult result = ::vkWaitForFences (
        get()->d_hDevice.handle(), 1, & get()->d_handle, VK_TRUE, timeoutNs );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KSemaphoreImpl;

// -----------------------------------------------------------------------------

class Semaphore : public TSharedReference< KSemaphoreImpl >
{
public:
    Semaphore();
    Semaphore ( const Device& hDevice );

    VkSemaphore handle() const;
    const Device& device() const;
};

// -----------------------------------------------------------------------------

class KSemaphoreImpl : public TSharedObject< KSemaphoreImpl >
{
public:
    KSemaphoreImpl ( const Device& hDevice );
    ~KSemaphoreImpl();

private:
    friend class Semaphore;
    Device d_hDevice;
    VkSemaphore d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE KSemaphoreImpl :: KSemaphoreImpl ( const Device& hDevice ) :
    d_hDevice ( hDevice ),
    d_handle(),
    d_result()
{
    VkSemaphoreCreateInfo vkSemaphoreCreateInfo;
    vkSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkSemaphoreCreateInfo.pNext = 0;
    vkSemaphoreCreateInfo.flags = 0;

    d_result = ::vkCreateSemaphore (
        d_hDevice.handle(), & vkSemaphoreCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KSemaphoreImpl :: ~KSemaphoreImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroySemaphore ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Semaphore :: Semaphore()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Semaphore :: Semaphore ( const Device& hDevice ) :
    TSharedReference< KSemaphoreImpl >( new KSemaphoreImpl ( hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkSemaphore Semaphore :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Semaphore :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KEventImpl;

// -----------------------------------------------------------------------------

class Event : public TSharedReference< KEventImpl >
{
public:
    Event ( const Device& hDevice );

    VkEvent handle() const;
    const Device& device() const;
    
    void signal ( bool bSignal );
    bool isSignaled() const;

    void cmdSignal (
        bool bSignal,
        VkPipelineStageFlags stageMask,
        CommandBuffer hCommandBuffer = CommandBuffer() );

    void cmdWait ( 
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        CommandBuffer hCommandBuffer = CommandBuffer() );

    void cmdWait ( 
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        const Barriers& barriers,
        CommandBuffer hCommandBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------

class KEventImpl : public TSharedObject< KEventImpl >
{
public:
    KEventImpl ( const Device& hDevice );
    ~KEventImpl();

private:
    friend class Event;
    Device d_hDevice;
    VkEvent d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE KEventImpl :: KEventImpl ( const Device& hDevice ) :
    d_hDevice ( hDevice ),
    d_handle(),
    d_result()
{
    VkEventCreateInfo vkEventCreateInfo;
    vkEventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    vkEventCreateInfo.pNext = 0;
    vkEventCreateInfo.flags = 0;

    d_result = ::vkCreateEvent (
        d_hDevice.handle(), & vkEventCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KEventImpl :: ~KEventImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyEvent ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Event :: Event ( const Device& hDevice ) :
    TSharedReference< KEventImpl >( new KEventImpl ( hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkEvent Event :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Event :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Event :: signal ( bool bSignal )
{
    if ( bSignal )
        ::vkSetEvent ( get()->d_hDevice.handle(), get()->d_handle );
    else
        ::vkResetEvent ( get()->d_hDevice.handle(), get()->d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Event :: isSignaled() const
{
    const VkResult result = ::vkGetEventStatus (
        get()->d_hDevice.handle(), get()->d_handle );
    return result == VK_EVENT_SET;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSYNCHRONIZATION_HPP
