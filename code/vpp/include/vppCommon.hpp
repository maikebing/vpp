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

#ifndef INC_VPPCOMMON_HPP
#define INC_VPPCOMMON_HPP

// -----------------------------------------------------------------------------

#ifdef _MSC_VER
    #define VK_USE_PLATFORM_WIN32_KHR
#elif defined __linux
    #define VK_USE_PLATFORM_WAYLAND_KHR
    #define VK_USE_PLATFORM_XCB_KHR
    #define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>

#undef BadValue
#undef Bool

#include "vppDefines.hpp"
#include "../src/spirv/spirv.hpp"

#undef min
#undef max
#undef MemoryBarrier

#include <cmath>
#include <cstring>

#include <string>
#include <vector>
#include <map>
#include <list>

#include <algorithm>
#include <functional>
#include <typeinfo>
#include <typeindex>

#include <atomic>
#include <thread>
#include <mutex>

#include <iostream>

#ifdef _MSC_VER
    #pragma warning (disable: 4100 4127 4189 4714)
#endif

#ifdef __ANDROID__
    #define VPP_INCOMPLETE_STDNS 1
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< typename T >
struct TFalse
{
    static const bool value = false;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#ifdef VPP_INCOMPLETE_STDNS
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace std {
// -----------------------------------------------------------------------------

VPP_INLINE float lround ( float x )
{
    return ::lround ( x );
}

VPP_INLINE double lround ( double x )
{
    return ::lround ( x );
}

VPP_INLINE float powf ( float x, float y )
{
    return ::powf ( x, y );
}

VPP_INLINE void* memset ( void* p, int v, size_t s )
{
    return ::memset ( p, v, s );
}

VPP_INLINE void* memcpy ( void* dest, const void* src, size_t length )
{
    return ::memcpy ( dest, src, length );
}

// -----------------------------------------------------------------------------
} // namespace std
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // VPP_FIX_MISSING_MATH
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMON_HPP
