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
#include "../include/vppSurface.hpp"
#include "../include/vppCommon.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

SurfaceImpl :: SurfaceImpl (
    const SurfaceInfo& surfaceInfo,
    const PhysicalDevice& hDevice,
    const Instance& hInstance ) :
        d_instance ( hInstance ),
        d_device ( hDevice ),
        d_handle(),
        d_result()
{
    createSurface ( surfaceInfo );
}

// -----------------------------------------------------------------------------

SurfaceImpl :: SurfaceImpl (
    VkSurfaceKHR hSurface,
    const PhysicalDevice& hDevice,
    const Instance& hInstance ) :
        d_instance ( hInstance ),
        d_device ( hDevice ),
        d_handle ( hSurface ),
        d_result ( VK_SUCCESS )
{
}

// -----------------------------------------------------------------------------

SurfaceImpl :: ~SurfaceImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroySurfaceKHR ( d_instance.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

void SurfaceImpl :: createSurface ( const SurfaceInfo& surfaceInfo )
{
    #ifdef VK_USE_PLATFORM_WIN32_KHR
        VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfoKHR;

        win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        win32SurfaceCreateInfoKHR.pNext = 0;
        win32SurfaceCreateInfoKHR.flags = 0;
        win32SurfaceCreateInfoKHR.hinstance = surfaceInfo.hInstance;
        win32SurfaceCreateInfoKHR.hwnd = surfaceInfo.hWnd;

        d_result = ::vkCreateWin32SurfaceKHR (
            d_instance.handle(), & win32SurfaceCreateInfoKHR, 0, & d_handle );
    #endif
}

// -----------------------------------------------------------------------------

bool Surface :: getCapabilities ( VkSurfaceCapabilitiesKHR* pResult ) const
{
    const VkResult result = ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR (
        get()->d_device.handle(), get()->d_handle, pResult );

    if ( result != VK_SUCCESS )
        get()->d_result = result;

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

bool Surface :: getFormats ( SurfaceFormats* pResult ) const
{
    std::uint32_t nFormats = 0;

    VkResult result = ::vkGetPhysicalDeviceSurfaceFormatsKHR (
        get()->d_device.handle(), get()->d_handle, & nFormats, 0 );

    if ( result != VK_SUCCESS )
        return false;

    pResult->resize ( nFormats );

    result = ::vkGetPhysicalDeviceSurfaceFormatsKHR (
        get()->d_device.handle(), get()->d_handle, & nFormats, & ( *pResult )[ 0 ] );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

bool Surface :: getPresentationModes ( PresentationModes* pResult ) const
{
    std::uint32_t nModes = 0;

    VkResult result = ::vkGetPhysicalDeviceSurfacePresentModesKHR (
        get()->d_device.handle(), get()->d_handle, & nModes, 0 );

    if ( result != VK_SUCCESS )
        return false;

    pResult->resize ( nModes );

    result = ::vkGetPhysicalDeviceSurfacePresentModesKHR (
        get()->d_device.handle(), get()->d_handle, & nModes, & ( *pResult )[ 0 ] );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VkExtent2D Surface :: getSize() const
{
    VkSurfaceCapabilitiesKHR capabilities;

    if ( getCapabilities ( & capabilities ) )
    {
        VkExtent2D resultExtent;

        if ( capabilities.currentExtent.width == -1 )
        {
            resultExtent.width = 640;
            resultExtent.height = 480;

            if ( resultExtent.width < capabilities.minImageExtent.width )
                resultExtent.width = capabilities.minImageExtent.width;
            else if ( resultExtent.width > capabilities.maxImageExtent.width )
                resultExtent.width = capabilities.maxImageExtent.width;

            if ( resultExtent.height < capabilities.minImageExtent.height )
                resultExtent.height = capabilities.minImageExtent.height;
            else if ( resultExtent.height > capabilities.maxImageExtent.height )
                resultExtent.height = capabilities.maxImageExtent.height;
        }
        else
            resultExtent = capabilities.currentExtent;

        return resultExtent;
    }
    else
        return VkExtent2D { 0, 0 };
}

// -----------------------------------------------------------------------------

VkFormat Surface :: getDefaultFormat ( VkColorSpaceKHR* pColorSpace ) const
{
    VkFormat imageFormat = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    SurfaceFormats formats;

    if ( getFormats ( & formats ) )
        for ( const auto& iFormat : formats )
            if ( iFormat.format != VK_FORMAT_UNDEFINED )
            {
                imageFormat = iFormat.format;
                imageColorSpace = iFormat.colorSpace;
                break;
            }

    if ( imageFormat == VK_FORMAT_UNDEFINED )
    {
        imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
        imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }

    if ( pColorSpace )
        *pColorSpace = imageColorSpace;

    return imageFormat;
}

// -----------------------------------------------------------------------------

void Surface :: getParametersAsText ( std::ostream& sst ) const
{
    VkSurfaceCapabilitiesKHR caps;

    if ( getCapabilities ( & caps ) )
    {
        sst << "Surface parameters: " << '\n';
        sst << '\n';
        sst << "minImageCount: " << caps.minImageCount << '\n';
        sst << "maxImageCount: " << caps.maxImageCount << '\n';
        sst << "currentExtent: " << caps.currentExtent.width << ", " << caps.currentExtent.height << '\n';
        sst << "minImageExtent: " << caps.minImageExtent.width << ", " << caps.minImageExtent.height << '\n';
        sst << "maxImageExtent: " << caps.maxImageExtent.width << ", " << caps.maxImageExtent.height << '\n';
        sst << "maxImageArrayLayers: " << caps.maxImageArrayLayers << '\n';
        sst << "supportedTransforms: " << caps.supportedTransforms << '\n';
        sst << "currentTransform: " << caps.currentTransform << '\n';
        sst << "supportedCompositeAlpha: " << caps.supportedCompositeAlpha << '\n';
        sst << "supportedUsageFlags: " << caps.supportedUsageFlags << '\n';
    }
    else
        sst << "Querying surface capabilities has failed." << std::endl;

    PresentationModes pmodes;

    if ( getPresentationModes ( & pmodes ) )
    {
        sst << "presentation modes: ";

        for ( auto iMode : pmodes )
        {
            const char* pName = 0;

            switch ( iMode )
            {
                case VK_PRESENT_MODE_IMMEDIATE_KHR: pName = "immediate"; break;
                case VK_PRESENT_MODE_MAILBOX_KHR: pName = "mailbox"; break;
                case VK_PRESENT_MODE_FIFO_KHR: pName = "fifo"; break;
                case VK_PRESENT_MODE_FIFO_RELAXED_KHR: pName = "fifo_relaxed"; break;
                default: break;
            }

            if ( pName )
                sst << pName << ' ';
        }

        sst << std::endl;
    }
    else
        sst << "Querying surface presentation modes has failed." << std::endl;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
