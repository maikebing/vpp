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
#include "../include/vppDebugReporter.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct DebugReporter::Impl
{
    Impl ( DebugReporter* pThis, const Instance& hInstance, VkFlags flags );
    ~Impl();

    static VkBool32 VKAPI_PTR onDebugReport (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData );

    DebugReporter* d_pThis;
    Instance d_hInstance;
    VkResult d_result;
    VkDebugReportCallbackEXT d_handle;
    bool d_bEnableShaderCompilationLog;
    
    static PFN_vkCreateDebugReportCallbackEXT s_vkCreateDebugReportCallbackEXT;
    static PFN_vkDebugReportMessageEXT s_vkDebugReportMessageEXT;
    static PFN_vkDestroyDebugReportCallbackEXT s_vkDestroyDebugReportCallbackEXT;
};

// -----------------------------------------------------------------------------

PFN_vkCreateDebugReportCallbackEXT DebugReporter::Impl :: s_vkCreateDebugReportCallbackEXT = 0;
PFN_vkDebugReportMessageEXT DebugReporter::Impl :: s_vkDebugReportMessageEXT = 0;
PFN_vkDestroyDebugReportCallbackEXT DebugReporter::Impl :: s_vkDestroyDebugReportCallbackEXT = 0;

// -----------------------------------------------------------------------------

DebugReporter::Impl :: Impl (
    DebugReporter* pThis, const Instance& hInstance, VkFlags flags ) :
        d_pThis ( pThis ),
        d_hInstance ( hInstance ),
        d_result ( VK_NOT_READY ),
        d_bEnableShaderCompilationLog ( ( flags & SHADERS ) != 0 )
{
    VkDebugReportCallbackCreateInfoEXT callbackInfo;
    std::memset ( & callbackInfo, 0, sizeof ( callbackInfo ) );

    callbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    callbackInfo.pNext = 0;
    callbackInfo.flags = flags & s_vulkanFlags;
    callbackInfo.pfnCallback = & DebugReporter::Impl::onDebugReport;
    callbackInfo.pUserData = d_pThis;

    if ( ! s_vkCreateDebugReportCallbackEXT )
    {
        s_vkCreateDebugReportCallbackEXT =
            reinterpret_cast< PFN_vkCreateDebugReportCallbackEXT >(
                ::vkGetInstanceProcAddr ( hInstance.handle(), "vkCreateDebugReportCallbackEXT" ) );

        s_vkDebugReportMessageEXT =
            reinterpret_cast< PFN_vkDebugReportMessageEXT >(
                ::vkGetInstanceProcAddr ( hInstance.handle(), "vkDebugReportMessageEXT" ) );

        s_vkDestroyDebugReportCallbackEXT =
            reinterpret_cast< PFN_vkDestroyDebugReportCallbackEXT >(
                ::vkGetInstanceProcAddr ( hInstance.handle(), "vkDestroyDebugReportCallbackEXT" ) );
    }

    if ( s_vkCreateDebugReportCallbackEXT )
        d_result = s_vkCreateDebugReportCallbackEXT (
            hInstance.handle(), & callbackInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

DebugReporter::Impl :: ~Impl()
{
    if ( d_result == VK_SUCCESS )
        s_vkDestroyDebugReportCallbackEXT ( d_hInstance.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VkBool32 DebugReporter::Impl :: onDebugReport (
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char* pLayerPrefix,
    const char* pMessage,
    void* pUserData )
{
    DebugReporter* pThis = reinterpret_cast< DebugReporter* >( pUserData );

    return pThis->debugReport (
        flags, objectType, object, location, messageCode, pLayerPrefix, pMessage );
}

// -----------------------------------------------------------------------------

DebugReporter :: DebugReporter ( const Instance& hInstance, VkFlags flags ) :
    impl ( new Impl ( this, hInstance, flags ) )
{
    impl->d_hInstance.registerDebugReporter ( this );
}

// -----------------------------------------------------------------------------

DebugReporter :: ~DebugReporter()
{
    impl->d_hInstance.unregisterDebugReporter ( this );
}

// -----------------------------------------------------------------------------

VkBool32 DebugReporter :: debugReport (
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char* pLayerPrefix,
    const char* pMessage )
{
    return VK_FALSE;
}

// -----------------------------------------------------------------------------

void DebugReporter :: shaderCompilationLog (
    const std::string& shaderCode,
    const char* pShaderType )
{
}

// -----------------------------------------------------------------------------

bool DebugReporter :: isShaderCompilationLogEnabled() const
{
    return impl->d_bEnableShaderCompilationLog;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

StreamDebugReporter :: StreamDebugReporter (
    std::ostream& out, const Instance& hInstance, VkFlags flags ) :
        DebugReporter ( hInstance, flags ),
        d_output ( out )
{
}

// -----------------------------------------------------------------------------

StreamDebugReporter :: ~StreamDebugReporter()
{
}

// -----------------------------------------------------------------------------

VkBool32 StreamDebugReporter :: debugReport (
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char* pLayerPrefix,
    const char* pMessage )
{
    if ( flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT )
        d_output << 'I';

    if ( flags & VK_DEBUG_REPORT_WARNING_BIT_EXT )
        d_output << 'W';

    if ( flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT )
        d_output << 'P';

    if ( flags & VK_DEBUG_REPORT_ERROR_BIT_EXT )
        d_output << 'E';

    if ( flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT )
        d_output << 'D';

    d_output << ": ";
    d_output << pMessage;
    d_output << std::endl;

    return VK_FALSE;
}

// -----------------------------------------------------------------------------

void StreamDebugReporter :: shaderCompilationLog (
    const std::string& shaderCode,
    const char* pShaderType )
{
    d_output << shaderCode;
    d_output << std::endl;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
