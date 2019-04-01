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

#ifndef INC_VPPINTERNALUTILS_HPP
#define INC_VPPINTERNALUTILS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMON_HPP
#include "vppCommon.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

VPP_INLINE unsigned int highestBit ( unsigned int v )
{
    #ifdef _MSC_VER
        unsigned long result;
        _BitScanReverse ( & result, static_cast< unsigned long >( v ) );
        return static_cast< unsigned int >( result );

    #elif defined __GNUC__
        return static_cast< unsigned int >( 31 - __builtin_clz ( v ) );

    #else
        for ( unsigned int i = 32u; i > 0u; )
            if ( v & ( 1u << --i ) )
                return i;
        return 0;

    #endif
}

// ---------------------------------------------------------------------------

VPP_INLINE unsigned int lowestBit ( unsigned int v )
{
    #ifdef _MSC_VER
        unsigned long result;
        _BitScanForward ( & result, static_cast< unsigned long >( v ) );
        return static_cast< unsigned int >( result );

    #elif defined __GNUC__
        return static_cast< unsigned int >( 31 - __builtin_ctz ( v ) );

    #else
        for ( unsigned int i = 0u; i < 32u; ++i )
            if ( v & ( 1u << i ) )
                return i;
        return 0;

    #endif
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int count1bits ( unsigned int value )
{
    #ifdef _MSC_VER
        return __popcnt ( value );

    #elif defined __GNUC__
        return static_cast< unsigned int >( __builtin_popcount ( value ) );

    #else
        value = value - ( ( value >> 1 ) & 0x55555555u );
        value = ( value & 0x33333333 ) + ( ( value >> 2 ) & 0x33333333u );
        return ( ( ( value + ( value >> 4 ) ) & 0x0F0F0F0Fu ) * 0x01010101u ) >> 24;

    #endif
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int ceil2n ( unsigned int value )
{
    return value < 2u ? 1u : 1u << ( highestBit ( value - 1u ) + 1u );
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int floor2n ( unsigned int value )
{
    return value < 1u ? 0u : 1u << highestBit ( value );
}

// -----------------------------------------------------------------------------

VPP_DLLAPI extern const unsigned int s_fastDivisionMultipliers [ 128 ];
VPP_DLLAPI extern const unsigned int s_fastDivisionShifts [ 128 ];

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPINTERNALUTILS_HPP
