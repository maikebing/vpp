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

#ifndef INC_VPPRENDERMANAGER_HPP
#define INC_VPPRENDERMANAGER_HPP

// -----------------------------------------------------------------------------

#include "vppCommon.hpp"
#include "vppSwapChain.hpp"
#include "vppRenderPass.hpp"
#include "vppFramebuffer.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class RenderManagerImpl;

// -----------------------------------------------------------------------------

class RenderManager : public TSharedReference< RenderManagerImpl >
{
public:
    RenderManager();
    RenderManager ( const SwapChain& hSwapChain );

    RenderManager (
        const Device& hDevice,
        const Surface& hSurface,
        std::uint32_t imageCount = 0,
        VkPresentModeKHR imageQueuingMode = VK_PRESENT_MODE_MAILBOX_KHR );

    VPP_DLLAPI void beginFrame();
    VPP_DLLAPI void endFrame();

    enum ECommandsCaching
    {
        CACHE_CMDS,
        REBUILD_CMDS
    };

    VPP_DLLAPI void render (
        const RenderPass& hRenderPass, ECommandsCaching caching = CACHE_CMDS );

    const Device& device() const;
    const Surface& surface() const;
    const Queue& queue() const;

    // lower level
    FrameBuffer getFrameBuffer (
        const RenderPass& renderPass, size_t iSwapImage = 0 );

    CommandBuffer getRenderCommands (
        const RenderPass& hRenderPass, ECommandsCaching caching );
};

// -----------------------------------------------------------------------------

class RenderManagerImpl : public TSharedObject< RenderManagerImpl >
{
public:
    RenderManagerImpl ( const SwapChain& hSwapChain, const Queue& hQueue );
    ~RenderManagerImpl();

private:
    FrameBuffer getFrameBuffer (
        const RenderPass& renderPass, size_t iSwapImage );

private:
    friend class RenderManager;
    SwapChain d_swapChain;
    CommandPool d_commandPool;
    Queue d_queue;

    std::uint32_t d_currentSwapImage;
    std::uint32_t d_rebuildCounter;

    typedef std::map< KAttachmentConfig, FrameBuffers > Config2FrameBuffers;
    Config2FrameBuffers d_config2frameBuffers;

    typedef std::pair< RenderPass, FrameBuffer > RenderPassKey;
    typedef std::map< RenderPassKey, CommandBuffer > RenderPassCommands;
    RenderPassCommands d_renderPassCommands;
};

// -----------------------------------------------------------------------------

VPP_INLINE RenderManagerImpl :: RenderManagerImpl (
    const SwapChain& hSwapChain,
    const Queue& hQueue ) :
        d_swapChain ( hSwapChain ),
        d_commandPool ( hSwapChain.device(), Q_GRAPHICS ),
        d_queue ( hQueue ),
        d_currentSwapImage ( 0 ),
        d_rebuildCounter ( 0 )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderManagerImpl :: ~RenderManagerImpl()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderManager :: RenderManager()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderManager :: RenderManager ( const SwapChain& hSwapChain ) :
    TSharedReference< RenderManagerImpl >(
        new RenderManagerImpl ( hSwapChain, Queue ( hSwapChain.device(), 0 ) ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderManager :: RenderManager (
    const Device& hDevice,
    const Surface& hSurface,
    std::uint32_t imageCount,
    VkPresentModeKHR imageQueuingMode ) :
        RenderManager ( SwapChain ( hDevice, hSurface, imageCount, imageQueuingMode ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameBuffer RenderManager :: getFrameBuffer (
    const RenderPass& renderPass, size_t iSwapImage )
{
    return get()->getFrameBuffer ( renderPass, iSwapImage );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& RenderManager :: device() const
{
    return get()->d_swapChain.device();
}

// -----------------------------------------------------------------------------

VPP_INLINE const Surface& RenderManager :: surface() const
{
    return get()->d_swapChain.surface();
}

// -----------------------------------------------------------------------------

VPP_INLINE const Queue& RenderManager :: queue() const
{
    return get()->d_queue;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPRENDERMANAGER_HPP
