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

#ifndef INC_VPPDEBUGREPORTER_HPP
#define INC_VPPDEBUGREPORTER_HPP

// -----------------------------------------------------------------------------

#include "vppInstance.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class DebugReporter
{
public:
    static const VkFlags ERRORS = VK_DEBUG_REPORT_ERROR_BIT_EXT;
    static const VkFlags WARNINGS = VK_DEBUG_REPORT_WARNING_BIT_EXT;
    static const VkFlags PERF = VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    static const VkFlags INFO = VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
    static const VkFlags DEBUG = VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    static const VkFlags SHADERS = 0x40000000u;

    static const VkFlags s_defaultFlags = ERRORS | WARNINGS | PERF;
    static const VkFlags s_vulkanFlags = ERRORS | WARNINGS | PERF | INFO | DEBUG;

    VPP_DLLAPI DebugReporter ( const Instance& hInstance, VkFlags flags = s_defaultFlags );
    VPP_DLLAPI virtual ~DebugReporter();

    VPP_DLLAPI virtual VkBool32 debugReport (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage );

    VPP_DLLAPI bool isShaderCompilationLogEnabled() const;

    VPP_DLLAPI virtual void shaderCompilationLog (
        const std::string& shaderCode,
        const char* pShaderType );

public:
    struct Impl;
    std::unique_ptr< Impl > impl;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class StreamDebugReporter : public DebugReporter
{
public:
    VPP_DLLAPI StreamDebugReporter (
        std::ostream& out, const Instance& hInstance, VkFlags flags = s_defaultFlags );

    VPP_DLLAPI virtual ~StreamDebugReporter();

    VPP_DLLAPI virtual VkBool32 debugReport (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage );

    VPP_DLLAPI virtual void shaderCompilationLog (
        const std::string& shaderCode,
        const char* pShaderType );

private:
    std::ostream& d_output;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPDEBUGREPORTER_HPP
