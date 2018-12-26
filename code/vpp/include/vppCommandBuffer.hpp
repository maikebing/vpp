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

#ifndef INC_VPPCOMMANDBUFFER_HPP
#define INC_VPPCOMMANDBUFFER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

#ifndef INC_VPPEXCEPTIONS_HPP
#include "vppExceptions.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class CommandBuffer
{
public:
    CommandBuffer();
    CommandBuffer ( VkCommandBuffer hBuffer );
    ~CommandBuffer();

    VkCommandBuffer handle() const;
    operator bool () const;

    VkResult reset();
    VkResult release();

    enum EBeginFlags
    {
        ONE_TIME_SUBMIT = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
        SIMULTANEOUS_USE = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    VkResult begin ( std::uint32_t flags = 0 );
    VkResult end();

private:
    VkCommandBuffer d_handle;
};

// -----------------------------------------------------------------------------

VPP_INLINE CommandBuffer :: CommandBuffer() :
    d_handle ( VK_NULL_HANDLE )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE CommandBuffer :: CommandBuffer ( VkCommandBuffer hBuffer ) :
    d_handle ( hBuffer )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE CommandBuffer :: ~CommandBuffer()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCommandBuffer CommandBuffer :: handle() const
{
    return d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE CommandBuffer :: operator bool () const
{
    return d_handle != VK_NULL_HANDLE;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandBuffer :: reset()
{
    return ::vkResetCommandBuffer ( d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandBuffer :: release()
{
    return ::vkResetCommandBuffer (
        d_handle, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT );
}
 
// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandBuffer :: begin ( std::uint32_t flags )
{
    // FIXME - zrobic wersje z inheritance tez

    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = 0;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = 0;

    return ::vkBeginCommandBuffer ( d_handle, & beginInfo );
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult CommandBuffer :: end()
{
    return ::vkEndCommandBuffer ( d_handle );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct RenderingCommandContext
{
public:
    VPP_DLLAPI RenderingCommandContext ( VkCommandBuffer commandBufferHandle );
    VPP_DLLAPI ~RenderingCommandContext();

    VPP_DLLAPI static VkCommandBuffer getCommandBufferHandle();

private:
    static thread_local RenderingCommandContext* s_pThis;
    VkCommandBuffer d_commandBufferHandle;
    RenderingCommandContext* d_pPrevContext;
};

// -----------------------------------------------------------------------------

struct RenderingFrameBufferContext
{
public:
    VPP_DLLAPI RenderingFrameBufferContext ( const FrameBuffer& frameBuffer );
    VPP_DLLAPI ~RenderingFrameBufferContext();

    VPP_DLLAPI static const FrameBuffer& getFrameBuffer();

private:
    static thread_local RenderingFrameBufferContext* s_pThis;
    const FrameBuffer& d_frameBuffer;
    RenderingFrameBufferContext* d_pPrevContext;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMANDBUFFER_HPP
