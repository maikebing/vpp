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
#include "../include/vppInstance.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

InstanceImpl* InstanceImpl :: s_pInstanceImpl = 0;

// -----------------------------------------------------------------------------

Instance :: Instance ( std::uint32_t flags ) :
    TSharedReference< InstanceImpl >(
        InstanceImpl::s_pInstanceImpl ?
            InstanceImpl::s_pInstanceImpl : new InstanceImpl ( flags ) )
{
}

// -----------------------------------------------------------------------------

Instance :: ~Instance()
{
}

// -----------------------------------------------------------------------------

InstanceImpl :: InstanceImpl ( std::uint32_t flags ) :
    d_handle(),
    d_result ( VK_NOT_READY ),
    d_pDebugReporter ( 0 )
{
    std::vector< const char * > enabledExtensions;
    enabledExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );

    #if defined VK_USE_PLATFORM_WIN32_KHR
        enabledExtensions.push_back ( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
    #endif

    VkInstanceCreateInfo vkInstanceCreateInfo;
    vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceCreateInfo.pNext = 0;
    vkInstanceCreateInfo.flags = 0;
    vkInstanceCreateInfo.pApplicationInfo = 0;

    if ( flags & Instance::VALIDATION )
    {
        static const char* validationLayerNames[] = 
        {
            "VK_LAYER_LUNARG_standard_validation"
        };

        vkInstanceCreateInfo.enabledLayerCount = 1;
        vkInstanceCreateInfo.ppEnabledLayerNames = validationLayerNames;

        enabledExtensions.push_back ( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
    }
    else
    {
        vkInstanceCreateInfo.enabledLayerCount = 0;
        vkInstanceCreateInfo.ppEnabledLayerNames = 0;
    }

    vkInstanceCreateInfo.enabledExtensionCount = static_cast< std::uint32_t >( enabledExtensions.size() );
    vkInstanceCreateInfo.ppEnabledExtensionNames = & enabledExtensions [ 0 ];

    d_result = ::vkCreateInstance ( & vkInstanceCreateInfo, 0, & d_handle );

    s_pInstanceImpl = this;
}

// -----------------------------------------------------------------------------

InstanceImpl :: ~InstanceImpl()
{
    VPP_EXTSYNC_MTX_SLOCK ( this );

    if ( d_result == VK_SUCCESS )
    {
        ::vkDestroyInstance ( d_handle, 0 );
        d_result = VK_NOT_READY;
    }

    s_pInstanceImpl = 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VkResult Instance :: enumeratePhysicalDevices ( PhysicalDevices* pResult ) const
{
    VPP_EXTSYNC_MTX_SLOCK ( get() );

    std::uint32_t nDevices = 0;
    VkResult result = vkEnumeratePhysicalDevices ( get()->d_handle, & nDevices, 0 );

    if ( result == VK_SUCCESS )
    {
        std::vector< VkPhysicalDevice > deviceHandles;
        deviceHandles.resize ( nDevices );
        result = vkEnumeratePhysicalDevices ( get()->d_handle, & nDevices, & deviceHandles [ 0 ] );

        if ( result == VK_SUCCESS )
        {
            pResult->reserve ( nDevices );

            for ( const auto& hDevice : deviceHandles )
                pResult->emplace_back ( hDevice );
        }

        return result;
    }
    else
        return result;
}

// -----------------------------------------------------------------------------

VkResult Instance :: enumerateExtensions ( ExtensionProperties* pResult )
{
    std::uint32_t nProperties = 0;

    const VkResult hResult =
        ::vkEnumerateInstanceExtensionProperties ( 0, & nProperties, 0 );

    if ( hResult != VK_SUCCESS )
        return hResult;

    pResult->resize ( nProperties );

    return ::vkEnumerateInstanceExtensionProperties (
        0, & nProperties, & ( *pResult )[ 0 ] );
}

// -----------------------------------------------------------------------------

DebugReporter* Instance :: getDebugReporter()
{
    return InstanceImpl::s_pInstanceImpl->d_pDebugReporter;
}

// -----------------------------------------------------------------------------

void Instance :: registerDebugReporter ( DebugReporter* pDebugReporter )
{
    get()->d_pDebugReporter = pDebugReporter;
}

// -----------------------------------------------------------------------------

void Instance :: unregisterDebugReporter ( DebugReporter* pDebugReporter )
{
    if ( get()->d_pDebugReporter == pDebugReporter )
        get()->d_pDebugReporter = 0;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
