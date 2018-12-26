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

#ifndef INC_VPPSWAPCHAIN_HPP
#define INC_VPPSWAPCHAIN_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPSURFACE_HPP
#include "vppSurface.hpp"
#endif

#ifndef INC_VPPCOMMANDPOOL_HPP
#include "vppCommandPool.hpp"
#endif

#ifndef INC_VPPQUEUE_HPP
#include "vppQueue.hpp"
#endif

#ifndef INC_VPPFRAMEIMAGEVIEW_HPP
#include "vppFrameImageView.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class SwapChainImpl;

// -----------------------------------------------------------------------------

class SwapChain : public TSharedReference< SwapChainImpl >
{
public:
    SwapChain();

    SwapChain (
        const Device& hDevice,
        const Surface& hSurface,
        std::uint32_t imageCount = 0,
        VkPresentModeKHR imageQueuingMode = VK_PRESENT_MODE_MAILBOX_KHR,
        const SwapChain& hReplacedSwapChain = SwapChain() );

    ~SwapChain();

    bool valid() const;
    VkSwapchainKHR handle() const;
    const Device& device() const;
    const Surface& surface() const;

    VPP_DLLAPI size_t views() const;
    VPP_DLLAPI FrameImageView view ( size_t index ) const;

    std::uint32_t acquireDisplayImage ( const Queue& hQueue );
    void presentDisplayImage ( const Queue& hQueue, std::uint32_t iImage );

private:
    VPP_DLLAPI static SwapChainImpl* createImpl (
        const Device& hDevice,
        const Surface& hSurface,
        std::uint32_t imageCount,
        VkPresentModeKHR imageQueuingMode,
        const SwapChain& hReplacedSwapChain );
};

// -----------------------------------------------------------------------------

class SwapChainImpl : public TSharedObject< SwapChainImpl >
{
public:
    VPP_DLLAPI SwapChainImpl (
        const Device& hDevice,
        const Surface& hSurface,
        std::uint32_t imageCount,
        VkPresentModeKHR imageQueuingMode,
        const SwapChain& hReplacedSwapChain );

    VPP_DLLAPI ~SwapChainImpl();

    VPP_DLLAPI std::uint32_t acquireDisplayImage ( const Queue& hQueue );
    VPP_DLLAPI void presentDisplayImage ( const Queue& hQueue, std::uint32_t iImage );
    
private:
    friend class SwapChain;
    Device d_hDevice;
    Surface d_hSurface;
    VkSwapchainKHR d_handle;
    VkResult d_result;

    CommandPool d_bufferPool;
    Semaphore d_semImageAcquire;

    std::vector< VkImage > d_images;
    std::vector< FrameImageView > d_imageViews;
    std::vector< CommandBuffer > d_presenters;
    std::vector< CommandBuffer > d_unpresenters;
};

// -----------------------------------------------------------------------------

VPP_INLINE SwapChain :: SwapChain()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE SwapChain :: SwapChain (
    const Device& hDevice,
    const Surface& hSurface,
    std::uint32_t imageCount,
    VkPresentModeKHR imageQueuingMode,
    const SwapChain& hReplacedSwapChain ) :
        TSharedReference< SwapChainImpl >(
            createImpl ( hDevice, hSurface, imageCount, imageQueuingMode, hReplacedSwapChain ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE SwapChain :: ~SwapChain()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE bool SwapChain :: valid() const
{
    return get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkSwapchainKHR SwapChain :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& SwapChain :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Surface& SwapChain :: surface() const
{
    return get()->d_hSurface;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t SwapChain :: acquireDisplayImage ( const Queue& hQueue )
{
    return get()->acquireDisplayImage ( hQueue );
}

// -----------------------------------------------------------------------------

VPP_INLINE void SwapChain :: presentDisplayImage ( const Queue& hQueue, std::uint32_t iImage )
{
    return get()->presentDisplayImage ( hQueue, iImage );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSWAPCHAIN_HPP
