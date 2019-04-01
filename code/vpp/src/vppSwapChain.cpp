/*
    Copyright 2016-2019 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
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
#include "../include/vppSwapChain.hpp"
#include "../include/vppCommandBufferRecorder.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

size_t SwapChain :: views() const
{
    return get()->d_imageViews.size();
}

// -----------------------------------------------------------------------------

FrameImageView SwapChain :: view ( size_t index ) const
{
    return get()->d_imageViews [ index ];
}

// -----------------------------------------------------------------------------

SwapChainImpl :: SwapChainImpl (
    const Device& hDevice,
    const Surface& hSurface,
    std::uint32_t imageCount,
    VkPresentModeKHR imageQueuingMode,
    const SwapChain& hReplacedSwapChain ) :
        d_hDevice ( hDevice ),
        d_hSurface ( hSurface ),
        d_handle(),
        d_result(),
        d_bufferPool ( hDevice, Q_GRAPHICS ),
        d_semImageAcquire ( hDevice )
{
    VkSurfaceCapabilitiesKHR capabilities;
    PresentationModes modes;

    if ( d_hSurface.getCapabilities ( & capabilities )
         && d_hSurface.getPresentationModes ( & modes ) )
    {
        VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR;
        vkSwapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        vkSwapchainCreateInfoKHR.pNext = 0;
        vkSwapchainCreateInfoKHR.flags = 0;
        vkSwapchainCreateInfoKHR.surface = d_hSurface.handle();

        if ( imageCount == 0 )
            imageCount = capabilities.minImageCount + 1;
        else if ( imageCount < capabilities.minImageCount )
            imageCount = capabilities.minImageCount;
        
        vkSwapchainCreateInfoKHR.minImageCount = imageCount;

        if ( capabilities.maxImageCount > 0
             && vkSwapchainCreateInfoKHR.minImageCount > capabilities.maxImageCount )
        {
            vkSwapchainCreateInfoKHR.minImageCount = capabilities.maxImageCount;
        }

        vkSwapchainCreateInfoKHR.imageFormat = d_hSurface.getDefaultFormat (
            & vkSwapchainCreateInfoKHR.imageColorSpace );

        const VkExtent2D swapchainExtent = d_hSurface.getSize();
        vkSwapchainCreateInfoKHR.imageExtent = swapchainExtent;

        // FIXME zamiast 1 dawac liczbe layerow w argumencie
        vkSwapchainCreateInfoKHR.imageArrayLayers = 1;

        vkSwapchainCreateInfoKHR.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        vkSwapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkSwapchainCreateInfoKHR.queueFamilyIndexCount = 0;
        vkSwapchainCreateInfoKHR.pQueueFamilyIndices = 0;
        vkSwapchainCreateInfoKHR.preTransform = capabilities.currentTransform;

        if ( capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR )
            vkSwapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
        else
            vkSwapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        for ( size_t i = 0; i < modes.size(); ++i )
            if ( modes [ i ] == imageQueuingMode )
            {
                swapchainPresentMode = imageQueuingMode;
                break;
            }

        vkSwapchainCreateInfoKHR.presentMode = swapchainPresentMode;
        vkSwapchainCreateInfoKHR.clipped = VK_TRUE;

        if ( hReplacedSwapChain )
            vkSwapchainCreateInfoKHR.oldSwapchain = hReplacedSwapChain.handle();
        else
            vkSwapchainCreateInfoKHR.oldSwapchain = 0;

        d_result = ::vkCreateSwapchainKHR (
            d_hDevice.handle(), & vkSwapchainCreateInfoKHR, 0, & d_handle );

        if ( d_result == VK_SUCCESS )
        {
            std::uint32_t nImages = 0;

            d_result = ::vkGetSwapchainImagesKHR (
                d_hDevice.handle(), d_handle, & nImages, 0 );

            if ( d_result == VK_SUCCESS )
            {
                d_images.resize ( nImages );
                d_imageViews.reserve ( nImages );

                d_result = ::vkGetSwapchainImagesKHR (
                    d_hDevice.handle(), d_handle, & nImages, & d_images [ 0 ] );

                if ( d_result == VK_SUCCESS )
                {
                    for ( std::uint32_t i = 0; i != nImages; ++i )
                    {
                        ImageInfo imageInfo (
                            swapchainExtent.width, swapchainExtent.height,
                            SWAPCHAIN, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                            vkSwapchainCreateInfoKHR.imageFormat );

                        Img image ( imageInfo, d_hDevice, d_images [ i ] );
                        d_imageViews.emplace_back ( image );
                    }

                    d_bufferPool.createBuffers ( nImages, & d_presenters );

                    for ( std::uint32_t i = 0; i != nImages; ++i )
                    {
                        CommandBufferRecorder recorder ( d_presenters [ i ] );
                        recorder.presentImage ( d_images [ i ] );
                    }

                    d_bufferPool.createBuffers ( nImages, & d_unpresenters );

                    for ( std::uint32_t i = 0; i != nImages; ++i )
                    {
                        CommandBufferRecorder recorder ( d_unpresenters [ i ] );
                        recorder.unpresentImage ( d_images [ i ] );
                    }
                }
            }
        }
    }
    else
        d_result = d_hSurface.result();
}

// -----------------------------------------------------------------------------

SwapChainImpl :: ~SwapChainImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroySwapchainKHR ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

std::uint32_t SwapChainImpl :: acquireDisplayImage ( const Queue& hQueue )
{
    std::uint32_t acquiredImageIndex = 0;

    const VkResult result = ::vkAcquireNextImageKHR (
        d_hDevice.handle(),
        d_handle,
        std::numeric_limits< std::uint64_t >::max(),
        d_semImageAcquire.handle(),
        VK_NULL_HANDLE,
        & acquiredImageIndex );

    if ( result == VK_SUCCESS )
        hQueue.submit ( d_unpresenters [ acquiredImageIndex ], d_semImageAcquire );

    return acquiredImageIndex;
}

// -----------------------------------------------------------------------------

void SwapChainImpl :: presentDisplayImage ( const Queue& hQueue, std::uint32_t iImage )
{
    hQueue.submit ( d_presenters [ iImage ] );

    VkResult result = VK_NOT_READY;

    VkPresentInfoKHR vkPresentInfoKHR;
    vkPresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    vkPresentInfoKHR.pNext = 0;
    vkPresentInfoKHR.waitSemaphoreCount = 0;
    vkPresentInfoKHR.pWaitSemaphores = 0;
    vkPresentInfoKHR.swapchainCount = 1;
    vkPresentInfoKHR.pSwapchains = & d_handle;
    vkPresentInfoKHR.pImageIndices = & iImage;
    vkPresentInfoKHR.pResults = & result;

    ::vkQueuePresentKHR ( hQueue.handle(), & vkPresentInfoKHR );
}

// -----------------------------------------------------------------------------

SwapChainImpl* SwapChain :: createImpl (
    const Device& hDevice,
    const Surface& hSurface, 
    std::uint32_t imageCount,
    VkPresentModeKHR imageQueuingMode,
    const SwapChain& hReplacedSwapChain )
{
    PhysicalDevice hPhysical = hDevice.physical();

    const size_t nFamilies = hPhysical.queueFamilyCount();

    for ( size_t iFamily = 0; iFamily != nFamilies; ++iFamily )
        if ( hPhysical.supportsSurface ( hSurface, iFamily ) )
            return new SwapChainImpl (
                hDevice, hSurface, imageCount, imageQueuingMode, hReplacedSwapChain );
        
    throw XSurfaceNotSupportedByDevice();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
