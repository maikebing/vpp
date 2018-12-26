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

class Instance : public TSharedReference< InstanceImpl >
{
public:
    enum EFlags
    {
        NO_FLAGS = 0,
        VALIDATION = 1
    };
    
    VPP_DLLAPI explicit Instance ( std::uint32_t flags = 0 );
    VPP_DLLAPI ~Instance();

    VkInstance handle() const;
    bool valid() const;
    
    VPP_DLLAPI VkResult enumeratePhysicalDevices ( PhysicalDevices* pResult ) const;
    VPP_DLLAPI static VkResult enumerateExtensions ( ExtensionProperties* pResult );

    VPP_DLLAPI static DebugReporter* getDebugReporter();

private:
    friend class DebugReporter;

    void registerDebugReporter ( DebugReporter* pDebugReporter );
    void unregisterDebugReporter ( DebugReporter* pDebugReporter );
};

// -----------------------------------------------------------------------------

class InstanceImpl : public TSharedObject< InstanceImpl >
{
public:
    VPP_DLLAPI InstanceImpl ( std::uint32_t flags );
    VPP_DLLAPI ~InstanceImpl();

private:
    friend class Instance;

    VkInstance d_handle;
    VkResult d_result;

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
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPINSTANCE_HPP
