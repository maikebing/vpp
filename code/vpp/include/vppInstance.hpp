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

#ifndef INC_VPPINSTANCE_HPP
#define INC_VPPINSTANCE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPHYSICALDEVICE_HPP
#include "vppPhysicalDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class InstanceImpl;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SApplicationInfo
{
    const char* d_appName;
    unsigned int d_appVersion;
    const char* d_engineName;
    unsigned int d_engineVersion;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class createInstance
{
public:
    VPP_DLLAPI createInstance();

    VPP_INLINE createInstance& vulkan ( const SVulkanVersion& v ) { d_requiredVersion = v; return *this; }
    VPP_INLINE createInstance& validation ( bool v ) { d_bValidation = v; return *this; }
    VPP_INLINE createInstance& raw ( bool v ) { d_bRaw = v; return *this; }
    VPP_INLINE createInstance& appName ( const char* v ) { d_appInfo.d_appName = v; return *this; }
    VPP_INLINE createInstance& engineName ( const char* v ) { d_appInfo.d_engineName = v; return *this; }
    VPP_INLINE createInstance& appVersion ( unsigned int v ) { d_appInfo.d_appVersion = v; return *this; }
    VPP_INLINE createInstance& engineVersion ( unsigned int v ) { d_appInfo.d_engineVersion = v; return *this; }
    VPP_INLINE createInstance& ext ( const std::string& v ) { d_extensions.push_back ( v ); return *this; }
    VPP_INLINE createInstance& layer ( const std::string& v ) { d_layers.push_back ( v ); return *this; }

    VPP_DLLAPI operator Instance() const;

private:
    SVulkanVersion d_requiredVersion;
    SApplicationInfo d_appInfo;

    bool d_bValidation;
    bool d_bRaw;

    std::vector< std::string > d_extensions;
    std::vector< std::string > d_layers;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Instance : public TSharedReference< InstanceImpl >
{
public:
    VPP_DLLAPI ~Instance();

    VkInstance handle() const;
    bool valid() const;

    const std::set< std::string >& enabledExtensions() const;

    VPP_DLLAPI static SVulkanVersion getLatestAvailableVulkanVersion();
    VPP_DLLAPI static SVulkanVersion supportedVersion();

    VPP_DLLAPI VkResult enumeratePhysicalDevices ( PhysicalDevices* pResult ) const;
    VPP_DLLAPI static VkResult enumerateExtensions ( ExtensionProperties* pResult );

    VPP_DLLAPI static DebugReporter* getDebugReporter();

private:
    friend class DebugReporter;
    friend class createInstance;

    VPP_DLLAPI Instance (
        const SVulkanVersion& requiredVersion,
        const SApplicationInfo& appInfo,
        bool bValidation,
        bool bRaw,
        const std::vector< std::string >& extensions,
        const std::vector< std::string >& layers );

    void registerDebugReporter ( DebugReporter* pDebugReporter );
    void unregisterDebugReporter ( DebugReporter* pDebugReporter );
};

// -----------------------------------------------------------------------------

class InstanceImpl : public TSharedObject< InstanceImpl >
{
public:
    VPP_DLLAPI InstanceImpl (
        const SVulkanVersion& requiredVersion,
        const SApplicationInfo& appInfo,
        bool bValidation,
        bool bRaw,
        const std::vector< std::string >& extensions,
        const std::vector< std::string >& layers );

    VPP_DLLAPI ~InstanceImpl();

private:
    void removePromotedExtensionNames ( const SVulkanVersion& currentVersion );
    void removeUnsupportedExtensionNames();

private:
    friend class Instance;

    VkInstance d_handle;
    VkResult d_result;

    SVulkanVersion d_supportedVersion;
    std::set< std::string > d_enabledExtensions;

    DebugReporter* d_pDebugReporter;

    VPP_EXTSYNC_MTX_DECLARE;

    static InstanceImpl* s_pInstanceImpl;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE VkInstance Instance :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Instance :: valid() const
{
    return get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE const std::set< std::string >& Instance :: enabledExtensions() const
{
    return get()->d_enabledExtensions;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPINSTANCE_HPP
