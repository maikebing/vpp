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

#ifndef INC_VPPCOMMANDPOOL_HPP
#define INC_VPPCOMMANDPOOL_HPP

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

class CommandPoolImpl;

// -----------------------------------------------------------------------------

class CommandPool : public TSharedReference< CommandPoolImpl >
{
public:
    enum EBufferLifetime
    {
        TRANSIENT = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        REUSABLE = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    };

    enum EBufferLevel
    {
        PRIMARY = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        SECONDARY = VK_COMMAND_BUFFER_LEVEL_SECONDARY
    };

    CommandPool ( const Device& hDevice, EQueueType queueType, std::uint32_t flags = 0 );
    ~CommandPool();

    VkResult reset();
    VkResult release();

    bool valid() const;
    const Device& device() const;
    VkCommandPool handle() const;

    VPP_DLLAPI VkResult createBuffers (
        std::uint32_t count,
        std::vector< CommandBuffer >* pBuffers,
        EBufferLevel level = PRIMARY );

    VPP_DLLAPI void freeBuffers (
        const std::vector< CommandBuffer >& buffers );

    VPP_DLLAPI CommandBuffer createBuffer ( EBufferLevel level = PRIMARY );
    VPP_DLLAPI void freeBuffer ( const CommandBuffer& buffer );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class CommandPoolImpl : public TSharedObject< CommandPoolImpl >
{
public:
    VPP_DLLAPI CommandPoolImpl ( const Device& hDevice, EQueueType queueType, std::uint32_t flags );
    VPP_DLLAPI ~CommandPoolImpl();

private:
    friend class CommandPool;
    Device d_hDevice;
    VkCommandPool d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE CommandPool :: CommandPool ( const Device& hDevice, EQueueType queueType, std::uint32_t flags ) :
    TSharedReference< CommandPoolImpl >(
        new CommandPoolImpl ( hDevice, queueType, flags ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE CommandPool :: ~CommandPool()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandPool :: reset()
{
    return ::vkResetCommandPool ( get()->d_hDevice.handle(), get()->d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandPool :: release()
{
    return ::vkResetCommandPool (
        get()->d_hDevice.handle(), get()->d_handle,
        VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool CommandPool :: valid() const
{
    return get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& CommandPool :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCommandPool CommandPool :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMANDPOOL_HPP
