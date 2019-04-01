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

#ifndef INC_VPPINITIALVALUES_HPP
#define INC_VPPINITIALVALUES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum EInitialValueType { IVT_COLOR, IVT_DEPTH, IVT_STENCIL, IVT_DS };

// -----------------------------------------------------------------------------

template< typename ScalarT, size_t SIZE, EInitialValueType TYPE >
struct TInitialValue {};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 1, IVT_COLOR >
{
    VPP_INLINE TInitialValue ( float _r ) : r ( _r  ) {}

    float r;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.float32 [ 0 ] = r;
        pResult->color.float32 [ 1 ] = 0;
        pResult->color.float32 [ 2 ] = 0;
        pResult->color.float32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 2, IVT_COLOR >
{
    float r;
    float g;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.float32 [ 0 ] = r;
        pResult->color.float32 [ 1 ] = g;
        pResult->color.float32 [ 2 ] = 0;
        pResult->color.float32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 3, IVT_COLOR >
{
    float r;
    float g;
    float b;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.float32 [ 0 ] = r;
        pResult->color.float32 [ 1 ] = g;
        pResult->color.float32 [ 2 ] = b;
        pResult->color.float32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 4, IVT_COLOR >
{
    float r;
    float g;
    float b;
    float a;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.float32 [ 0 ] = r;
        pResult->color.float32 [ 1 ] = g;
        pResult->color.float32 [ 2 ] = b;
        pResult->color.float32 [ 3 ] = a;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::int32_t, 1, IVT_COLOR >
{
    VPP_INLINE TInitialValue ( std::int32_t _r ) : r ( _r  ) {}

    std::int32_t r;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.int32 [ 0 ] = r;
        pResult->color.int32 [ 1 ] = 0;
        pResult->color.int32 [ 2 ] = 0;
        pResult->color.int32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::int32_t, 2, IVT_COLOR >
{
    std::int32_t r;
    std::int32_t g;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.int32 [ 0 ] = r;
        pResult->color.int32 [ 1 ] = g;
        pResult->color.int32 [ 2 ] = 0;
        pResult->color.int32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::int32_t, 3, IVT_COLOR >
{
    std::int32_t r;
    std::int32_t g;
    std::int32_t b;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.int32 [ 0 ] = r;
        pResult->color.int32 [ 1 ] = g;
        pResult->color.int32 [ 2 ] = b;
        pResult->color.int32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::int32_t, 4, IVT_COLOR >
{
    std::int32_t r;
    std::int32_t g;
    std::int32_t b;
    std::int32_t a;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.int32 [ 0 ] = r;
        pResult->color.int32 [ 1 ] = g;
        pResult->color.int32 [ 2 ] = b;
        pResult->color.int32 [ 3 ] = a;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::uint32_t, 1, IVT_COLOR >
{
    VPP_INLINE TInitialValue ( std::uint32_t _r ) : r ( _r  ) {}

    std::uint32_t r;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.uint32 [ 0 ] = r;
        pResult->color.uint32 [ 1 ] = 0;
        pResult->color.uint32 [ 2 ] = 0;
        pResult->color.uint32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::uint32_t, 2, IVT_COLOR >
{
    std::uint32_t r;
    std::uint32_t g;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.uint32 [ 0 ] = r;
        pResult->color.uint32 [ 1 ] = g;
        pResult->color.uint32 [ 2 ] = 0;
        pResult->color.uint32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::uint32_t, 3, IVT_COLOR >
{
    std::uint32_t r;
    std::uint32_t g;
    std::uint32_t b;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.uint32 [ 0 ] = r;
        pResult->color.uint32 [ 1 ] = g;
        pResult->color.uint32 [ 2 ] = b;
        pResult->color.uint32 [ 3 ] = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::uint32_t, 4, IVT_COLOR >
{
    std::uint32_t r;
    std::uint32_t g;
    std::uint32_t b;
    std::uint32_t a;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->color.uint32 [ 0 ] = r;
        pResult->color.uint32 [ 1 ] = g;
        pResult->color.uint32 [ 2 ] = b;
        pResult->color.uint32 [ 3 ] = a;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 1, IVT_DEPTH >
{
    VPP_INLINE TInitialValue ( float _d ) : d ( _d  ) {}

    float d;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->depthStencil.depth = d;
        pResult->depthStencil.stencil = 0;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< std::uint32_t, 1, IVT_STENCIL >
{
    VPP_INLINE TInitialValue ( std::uint32_t _s ) : s ( _s  ) {}

    std::uint32_t s;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->depthStencil.depth = 0;
        pResult->depthStencil.stencil = s;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TInitialValue< float, 2, IVT_DS >
{
    float d;
    std::uint32_t s;

    VPP_INLINE void convert ( VkClearValue* pResult ) const
    {
        pResult->depthStencil.depth = d;
        pResult->depthStencil.stencil = s;
    }
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPINITIALVALUES_HPP
