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
#include "../include/vppInstance.hpp"

#include <sstream>
#include <set>
#include <iterator>

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

/*
    Known instance extensions:
        VK_EXT_debug_report                 
        VK_EXT_debug_utils                  
        VK_EXT_swapchain_colorspace         
        VK_KHR_device_group_creation(p)        
        VK_KHR_external_fence_capabilities(p) 
        VK_KHR_external_memory_capabilities(p)
        VK_KHR_external_semaphore_capabilities(p)
        VK_KHR_get_physical_device_properties2(p)(a)
        VK_KHR_get_surface_capabilities2    
        VK_KHR_surface                      
        VK_KHR_win32_surface                
*/

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

InstanceImpl* InstanceImpl :: s_pInstanceImpl = 0;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

createInstance :: createInstance() :
    d_requiredVersion { 1, 0, 0 },
    d_appInfo { 0, 0, 0, 0 },
    d_bValidation ( false ),
    d_bRaw ( false )
{
}

// -----------------------------------------------------------------------------

createInstance :: operator Instance() const
{
    return Instance ( d_requiredVersion, d_appInfo, d_bValidation, d_bRaw, d_extensions, d_layers );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

Instance :: Instance (
    const SVulkanVersion& requiredVersion,
    const SApplicationInfo& appInfo,
    bool bValidation,
    bool bRaw,
    const std::vector< std::string >& extensions,
    const std::vector< std::string >& layers ) :
        TSharedReference< InstanceImpl >(
            InstanceImpl::s_pInstanceImpl ?
                InstanceImpl::s_pInstanceImpl 
                : new InstanceImpl ( requiredVersion, appInfo, bValidation, bRaw, extensions, layers ) )
{
}

// -----------------------------------------------------------------------------

Instance :: ~Instance()
{
}

// -----------------------------------------------------------------------------

SVulkanVersion Instance :: supportedVersion()
{
    if ( InstanceImpl::s_pInstanceImpl )
        return InstanceImpl::s_pInstanceImpl->d_supportedVersion;
    else
        return getLatestAvailableVulkanVersion();
}

// -----------------------------------------------------------------------------

SVulkanVersion Instance :: getLatestAvailableVulkanVersion()
{
    PFN_vkEnumerateInstanceVersion pEnumerateInstanceVersion =
        reinterpret_cast< PFN_vkEnumerateInstanceVersion >( ::vkGetInstanceProcAddr (
            VK_NULL_HANDLE, "vkEnumerateInstanceVersion" ) );

    if ( pEnumerateInstanceVersion )
    {
        std::uint32_t versionId;

        if ( ( *pEnumerateInstanceVersion )( & versionId ) == VK_SUCCESS )
            return SVulkanVersion::fromId ( versionId );
        else
            return SVulkanVersion { 1, 0, 0 };
    }
    else
        return SVulkanVersion { 1, 0, 0 };
}

// -----------------------------------------------------------------------------

InstanceImpl :: InstanceImpl (
    const SVulkanVersion& requiredVersion,
    const SApplicationInfo& appInfo,
    bool bValidation,
    bool bRaw,
    const std::vector< std::string >& extensions,
    const std::vector< std::string >& layers ) :
        d_handle(),
        d_result ( VK_NOT_READY ),
        d_supportedVersion ( std::min ( requiredVersion, Instance::getLatestAvailableVulkanVersion() ) ),
        d_pDebugReporter ( 0 )
{
    d_enabledExtensions.insert ( extensions.begin(), extensions.end() );

    if ( ! bRaw )
    {
        d_enabledExtensions.emplace ( VK_KHR_SURFACE_EXTENSION_NAME );
        d_enabledExtensions.emplace ( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME );

        #if defined VK_USE_PLATFORM_WIN32_KHR
            d_enabledExtensions.emplace ( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
        #endif
    }

    VkInstanceCreateInfo vkInstanceCreateInfo;
    vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceCreateInfo.pNext = 0;
    vkInstanceCreateInfo.flags = 0;

    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = 0;
    applicationInfo.pApplicationName = appInfo.d_appName;
    applicationInfo.applicationVersion = appInfo.d_appVersion;
    applicationInfo.pEngineName = appInfo.d_engineName;
    applicationInfo.engineVersion = appInfo.d_engineVersion;
    applicationInfo.apiVersion = d_supportedVersion.id();
    
    vkInstanceCreateInfo.pApplicationInfo = & applicationInfo;

    std::set< std::string > layerNames ( layers.begin(), layers.end() );

    if ( bValidation )
    {
        layerNames.emplace ( "VK_LAYER_LUNARG_standard_validation" );

        if ( ! bRaw )
            d_enabledExtensions.emplace ( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
    }
    else
    {
        vkInstanceCreateInfo.enabledLayerCount = 0;
        vkInstanceCreateInfo.ppEnabledLayerNames = 0;
    }

    removePromotedExtensionNames ( d_supportedVersion );
    removeUnsupportedExtensionNames();

    std::vector< const char* > enabledExtensions ( d_enabledExtensions.size(), 0 );
    std::vector< const char* > enabledLayers ( layerNames.size(), 0 );

    std::transform (
        d_enabledExtensions.begin(), d_enabledExtensions.end(), enabledExtensions.begin(),
        []( const std::string& extName ) { return extName.c_str(); }
    );

    std::transform (
        layerNames.begin(), layerNames.end(), enabledLayers.begin(),
        []( const std::string& layerName ) { return layerName.c_str(); }
    );

    vkInstanceCreateInfo.enabledExtensionCount = static_cast< std::uint32_t >( enabledExtensions.size() );
    vkInstanceCreateInfo.ppEnabledExtensionNames = & enabledExtensions [ 0 ];
    vkInstanceCreateInfo.enabledLayerCount = static_cast< std::uint32_t >( enabledLayers.size() );
    vkInstanceCreateInfo.ppEnabledLayerNames = & enabledLayers [ 0 ];

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

void InstanceImpl :: removePromotedExtensionNames ( const SVulkanVersion& currentVersion )
{
    static const SVulkanVersion s_vk11 { 1, 1, 0 };

    static const std::vector< const char* > s_vk11promoted =
    {
        "VK_KHR_device_group_creation",
        "VK_KHR_external_fence_capabilities",
        "VK_KHR_external_memory_capabilities",
        "VK_KHR_external_semaphore_capabilities",
        "VK_KHR_get_physical_device_properties2"
    };

    if ( ! ( currentVersion < s_vk11 ) )
        for ( const char* pExtName : s_vk11promoted )
            d_enabledExtensions.erase ( std::string ( pExtName ) );
}

// -----------------------------------------------------------------------------

void InstanceImpl :: removeUnsupportedExtensionNames()
{
    ExtensionProperties extProps;
    Instance::enumerateExtensions ( & extProps );

    std::set< std::string > supportedExtensions;

    for ( const auto& iExt : extProps )
        supportedExtensions.emplace ( iExt.extensionName );
    
    std::set< std::string > validExtensions;

    std::set_intersection (
        d_enabledExtensions.begin(), d_enabledExtensions.end(), 
        supportedExtensions.begin(), supportedExtensions.end(), 
        std::inserter ( validExtensions, validExtensions.end() ) );

    d_enabledExtensions.swap ( validExtensions );
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
