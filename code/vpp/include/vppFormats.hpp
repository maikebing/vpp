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

#ifndef INC_VPPFORMATS_HPP
#define INC_VPPFORMATS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPINITIALVALUES_HPP
#include "vppInitialValues.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class usint_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1u );
    static const BaseT MASK = ( ( ONE << BITS ) - ONE );

    VPP_INLINE operator std::uint32_t() const
    {
        return static_cast< std::uint32_t >( ( d_data >> OFFSET ) & MASK );
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const usint_t& operator= ( const usint_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const usint_t& operator= ( std::uint32_t value )
    {
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( value & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const usint_t& rhs ) const
    {
        const BaseT t1 = d_data & ~( MASK << OFFSET );
        const BaseT t2 = rhs.d_data & ~( MASK << OFFSET );
        return t1 == t2;
    }

    VPP_INLINE bool operator!= ( const usint_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class sint_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1 );
    static const ptrdiff_t MOD = ( ONE << BITS );
    static const ptrdiff_t SGN = ( ONE << ( BITS - 1 ) );
    static const ptrdiff_t MASK = ( MOD - ONE );
    static const BaseT VMASK = ( SGN - ONE );

    VPP_INLINE operator std::int32_t() const
    {
        std::int32_t v = ( d_data >> OFFSET ) & MASK;
        if ( v & SGN )
            v -= MOD;
        return v;
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const sint_t& operator= ( const sint_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const sint_t& operator= ( std::int32_t value )
    {
        const BaseT v = static_cast< BaseT >( value );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const sint_t& rhs ) const
    {
        const BaseT t1 = d_data & ~( MASK << OFFSET );
        const BaseT t2 = rhs.d_data & ~( MASK << OFFSET );
        return t1 == t2;
    }

    VPP_INLINE bool operator!= ( const sint_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;
};

// -----------------------------------------------------------------------------

typedef usint_t< std::uint8_t, 0, 2 > usint2_t;
typedef usint_t< std::uint8_t, 0, 8 > usint8_t;
typedef usint_t< std::uint16_t, 0, 10 > usint10_t;
typedef usint_t< std::uint16_t, 0, 16 > usint16_t;

typedef sint_t< std::int8_t, 0, 2 > sint2_t;
typedef sint_t< std::int8_t, 0, 8 > sint8_t;
typedef sint_t< std::int16_t, 0, 10 > sint10_t;
typedef sint_t< std::int16_t, 0, 16 > sint16_t;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class unorm_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1u );
    static const BaseT MASK = ( ( ONE << BITS ) - ONE );

    static VPP_INLINE float clamp ( float v )
    {
        if ( v < 0.0f )
            return 0.0f;
        else if ( v > 1.0f )
            return 1.0f;
        else
            return v;
    }

    static VPP_INLINE double clamp ( double v )
    {
        if ( v < 0.0 )
            return 0.0;
        else if ( v > 1.0 )
            return 1.0;
        else
            return v;
    }

    VPP_INLINE operator float() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        return static_cast< float >( v ) / FLOAT_MULT;
    }

    VPP_INLINE operator double() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        return static_cast< double >( v ) / DOUBLE_MULT;
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const unorm_t& operator= ( const unorm_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const unorm_t& operator= ( float value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( FLOAT_MULT * clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const unorm_t& operator= ( double value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( DOUBLE_MULT * clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE void set ( BaseT data )
    {
        d_data = data;
    }

    VPP_INLINE bool operator== ( const unorm_t& rhs ) const
    {
        const BaseT v1 = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        const BaseT v2 = static_cast< BaseT >( ( rhs.d_data >> OFFSET ) & MASK );
        return v1 == v2;
    }

    VPP_INLINE bool operator!= ( const unorm_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;

    static const float FLOAT_MULT;
    static const double DOUBLE_MULT;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class snorm_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1 );
    static const ptrdiff_t MOD = ( ONE << BITS );
    static const ptrdiff_t SGN = ( ONE << ( BITS - 1 ) );
    static const ptrdiff_t MASK = ( MOD - ONE );
    static const BaseT VMASK = ( SGN - ONE );

    static VPP_INLINE float clamp ( float v )
    {
        if ( v < -1.0f )
            return -1.0f;
        else if ( v > 1.0f )
            return 1.0f;
        else
            return v;
    }

    static VPP_INLINE double clamp ( double v )
    {
        if ( v < -1.0 )
            return -1.0;
        else if ( v > 1.0 )
            return 1.0;
        else
            return v;
    }

    VPP_INLINE operator float() const
    {
        ptrdiff_t v = ( d_data >> OFFSET ) & MASK;
        if ( v & SGN )
            v -= MOD;
        return clamp ( static_cast< float >( v ) / FLOAT_MULT );
    }

    VPP_INLINE operator double() const
    {
        ptrdiff_t v = ( d_data >> OFFSET ) & MASK;
        if ( v & SGN )
            v -= MOD;
        return clamp ( static_cast< double >( v ) / DOUBLE_MULT );
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const snorm_t& operator= ( const snorm_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const snorm_t& operator= ( float value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( FLOAT_MULT * clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const snorm_t& operator= ( double value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( DOUBLE_MULT * clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const snorm_t& rhs ) const
    {
        const ptrdiff_t v1 = ( d_data >> OFFSET ) & MASK;
        const ptrdiff_t v2 = ( rhs.d_data >> OFFSET ) & MASK;
        return v1 == v2;
    }

    VPP_INLINE bool operator!= ( const snorm_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;

    static const float FLOAT_MULT;
    static const double DOUBLE_MULT;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
const float unorm_t< BaseT, OFFSET, BITS >::FLOAT_MULT = static_cast< float >( MASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double unorm_t< BaseT, OFFSET, BITS >::DOUBLE_MULT = static_cast< double >( MASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const float snorm_t< BaseT, OFFSET, BITS >::FLOAT_MULT = static_cast< float >( VMASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double snorm_t< BaseT, OFFSET, BITS >::DOUBLE_MULT = static_cast< double >( VMASK );

// -----------------------------------------------------------------------------

typedef unorm_t< std::uint8_t, 0, 1 > unorm1_t;
typedef unorm_t< std::uint8_t, 0, 2 > unorm2_t;
typedef unorm_t< std::uint8_t, 0, 4 > unorm4_t;
typedef unorm_t< std::uint8_t, 0, 5 > unorm5_t;
typedef unorm_t< std::uint8_t, 0, 6 > unorm6_t;
typedef unorm_t< std::uint8_t, 0, 8 > unorm8_t;
typedef unorm_t< std::uint16_t, 0, 10 > unorm10_t;
typedef unorm_t< std::uint16_t, 0, 16 > unorm16_t;
typedef unorm_t< std::uint32_t, 0, 24 > unorm24_t;

typedef snorm_t< std::int8_t, 0, 2 > snorm2_t;
typedef snorm_t< std::int8_t, 0, 8 > snorm8_t;
typedef snorm_t< std::int16_t, 0, 10 > snorm10_t;
typedef snorm_t< std::int16_t, 0, 16 > snorm16_t;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class uscaled_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1u );
    static const BaseT MASK = ( ( ONE << BITS ) - ONE );

    static VPP_INLINE float clamp ( float v )
    {
        if ( v < FLOAT_MIN )
            return FLOAT_MIN;
        else if ( v > FLOAT_MAX )
            return FLOAT_MAX;
        else
            return v;
    }

    static VPP_INLINE double clamp ( double v )
    {
        if ( v < DOUBLE_MIN )
            return DOUBLE_MIN;
        else if ( v > DOUBLE_MIN )
            return DOUBLE_MIN;
        else
            return v;
    }

    VPP_INLINE operator float() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        return static_cast< float >( v );
    }

    VPP_INLINE operator double() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        return static_cast< double >( v );
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const uscaled_t& operator= ( const uscaled_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const uscaled_t& operator= ( float value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const uscaled_t& operator= ( double value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const uscaled_t& rhs ) const
    {
        const BaseT v1 = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        const BaseT v2 = static_cast< BaseT >( ( rhs.d_data >> OFFSET ) & MASK );
        return v1 == v2;
    }

    VPP_INLINE bool operator!= ( const uscaled_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;

    static const float FLOAT_MAX;
    static const float FLOAT_MIN;
    static const double DOUBLE_MAX;
    static const double DOUBLE_MIN;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class sscaled_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1 );
    static const ptrdiff_t MOD = ( ONE << BITS );
    static const ptrdiff_t SGN = ( ONE << ( BITS - 1 ) );
    static const ptrdiff_t MASK = ( MOD - ONE );
    static const BaseT VMASK = ( SGN - ONE );

    static VPP_INLINE float clamp ( float v )
    {
        if ( v < FLOAT_MIN )
            return FLOAT_MIN;
        else if ( v > FLOAT_MAX )
            return FLOAT_MAX;
        else
            return v;
    }

    static VPP_INLINE double clamp ( double v )
    {
        if ( v < DOUBLE_MIN )
            return DOUBLE_MIN;
        else if ( v > DOUBLE_MIN )
            return DOUBLE_MIN;
        else
            return v;
    }

    VPP_INLINE operator float() const
    {
        ptrdiff_t v = ( d_data >> OFFSET ) & MASK;
        if ( v & SGN )
            v -= MOD;
        return static_cast< float >( v );
    }

    VPP_INLINE operator double() const
    {
        ptrdiff_t v = ( d_data >> OFFSET ) & MASK;
        if ( v & SGN )
            v -= MOD;
        return static_cast< double >( v );
    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const sscaled_t& operator= ( const sscaled_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const sscaled_t& operator= ( float value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const sscaled_t& operator= ( double value )
    {
        const BaseT v = static_cast< BaseT >( std::lround ( clamp ( value ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const sscaled_t& rhs ) const
    {
        ptrdiff_t v1 = ( d_data >> OFFSET ) & MASK;
        ptrdiff_t v2 = ( rhs.d_data >> OFFSET ) & MASK;
        return v1 == v2;
    }

    VPP_INLINE bool operator!= ( const sscaled_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;

    static const float FLOAT_MAX;
    static const float FLOAT_MIN;
    static const double DOUBLE_MAX;
    static const double DOUBLE_MIN;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
const float uscaled_t< BaseT, OFFSET, BITS >::FLOAT_MAX = static_cast< float >( MASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const float uscaled_t< BaseT, OFFSET, BITS >::FLOAT_MIN = 0.0f;

template< typename BaseT, size_t OFFSET, size_t BITS >
const double uscaled_t< BaseT, OFFSET, BITS >::DOUBLE_MAX = static_cast< double >( MASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double uscaled_t< BaseT, OFFSET, BITS >::DOUBLE_MIN = 0.0;

template< typename BaseT, size_t OFFSET, size_t BITS >
const float sscaled_t< BaseT, OFFSET, BITS >::FLOAT_MAX = static_cast< float >( VMASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const float sscaled_t< BaseT, OFFSET, BITS >::FLOAT_MIN = -static_cast< float >( VMASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double sscaled_t< BaseT, OFFSET, BITS >::DOUBLE_MAX = static_cast< double >( VMASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double sscaled_t< BaseT, OFFSET, BITS >::DOUBLE_MIN = -static_cast< double >( VMASK );

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef uscaled_t< std::uint8_t, 0, 2 > uscaled2_t;
typedef uscaled_t< std::uint8_t, 0, 8 > uscaled8_t;
typedef uscaled_t< std::uint16_t, 0, 10 > uscaled10_t;
typedef uscaled_t< std::uint16_t, 0, 16 > uscaled16_t;

typedef sscaled_t< std::int8_t, 0, 2 > sscaled2_t;
typedef sscaled_t< std::int8_t, 0, 8 > sscaled8_t;
typedef sscaled_t< std::int16_t, 0, 10 > sscaled10_t;
typedef sscaled_t< std::int16_t, 0, 16 > sscaled16_t;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
class srgb_t
{
public:
    static const BaseT ONE = static_cast< BaseT >( 1u );
    static const BaseT MASK = ( ( ONE << BITS ) - ONE );

    static VPP_INLINE float clamp ( float v )
    {
        if ( v < 0.0f )
            return 0.0f;
        else if ( v > 1.0f )
            return 1.0f;
        else
            return v;
    }

    static VPP_INLINE double clamp ( double v )
    {
        if ( v < 0.0 )
            return 0.0;
        else if ( v > 1.0 )
            return 1.0;
        else
            return v;
    }

    VPP_INLINE operator float() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        const float nl = static_cast< float >( v ) / FLOAT_MULT;
        return ( nl <= 0.04045f ) ?
            ( nl / 12.92f ) : clamp ( std::pow ( ( nl + 0.055f ) / 1.055f, 2.4f ) );    }

    VPP_INLINE operator double() const
    {
        const BaseT v = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        double nl = static_cast< double >( v ) / DOUBLE_MULT;
        return ( nl <= 0.04045 ) ?
            ( nl / 12.92 ) : clamp ( std::pow ( ( nl + 0.055 ) / 1.055, 2.4 ) );    }

    template< typename RBaseT, size_t ROFFSET >
    VPP_INLINE const srgb_t& operator= ( const srgb_t< RBaseT, ROFFSET, BITS >& rhs )
    {
        const BaseT v = static_cast< BaseT >( rhs.data() >> ROFFSET ) & MASK;
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const srgb_t& operator= ( float value )
    {
        const float nl = ( value <= 0.0031308f ) ?
            ( 12.92f * value )
            : static_cast< float >( 1.055 * std::pow ( value, ( 1.0 / 2.4 ) ) - 0.055 );

        const BaseT v = static_cast< BaseT >( std::lround ( FLOAT_MULT * clamp ( nl ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE const srgb_t& operator= ( double value )
    {
        const double nl = ( value <= 0.0031308 ) ?
            ( 12.92 * value )
            : ( 1.055 * std::pow ( value, ( 1.0 / 2.4 ) ) - 0.055 );

        const BaseT v = static_cast< BaseT >( std::lround ( DOUBLE_MULT * clamp ( nl ) ) );
        const BaseT t = d_data & ~( MASK << OFFSET );
        d_data = t | ( ( v & MASK ) << OFFSET );
        return *this;
    }

    VPP_INLINE BaseT data() const
    {
        return d_data;
    }

    VPP_INLINE bool operator== ( const srgb_t& rhs ) const
    {
        const BaseT v1 = static_cast< BaseT >( ( d_data >> OFFSET ) & MASK );
        const BaseT v2 = static_cast< BaseT >( ( rhs.d_data >> OFFSET ) & MASK );
        return v1 == v2;
    }

    VPP_INLINE bool operator!= ( const srgb_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    BaseT d_data;

    static const float FLOAT_MULT;
    static const double DOUBLE_MULT;
};

// -----------------------------------------------------------------------------

template< typename BaseT, size_t OFFSET, size_t BITS >
const float srgb_t< BaseT, OFFSET, BITS >::FLOAT_MULT = static_cast< float >( MASK );

template< typename BaseT, size_t OFFSET, size_t BITS >
const double srgb_t< BaseT, OFFSET, BITS >::DOUBLE_MULT = static_cast< double >( MASK );

typedef srgb_t< std::uint8_t, 0, 8 > srgb8_t;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct float16_t
{
public:
    VPP_INLINE const float16_t& operator= ( float value )
    {
        union
        {
            float source;
            std::uint32_t sourceBits;
        };

        source = value;

        d_data = static_cast< std::uint16_t >(            s_baseTable [ ( sourceBits >> 23 ) & 0x1ff ]            + ( ( sourceBits & 0x007fffff ) >> s_shiftTable [ ( sourceBits >> 23 ) & 0x1ff ] )        );        return *this;    }

    VPP_INLINE operator float() const
    {
        union
        {
            float result;
            std::uint32_t resultBits;
        };

        resultBits =            s_mantissaTable [ s_offsetTable [ d_data >> 10 ] + ( d_data & 0x3ffu ) ]             + s_exponentTable [ d_data >> 10 ];        return result;    }

    VPP_INLINE bool operator== ( const float16_t& rhs ) const
    {
        return d_data == rhs.d_data;
    }

    VPP_INLINE bool operator!= ( const float16_t& rhs ) const
    {
        return ! operator== ( rhs );
    }

private:
    std::uint16_t d_data;

private:
    static const std::uint32_t s_mantissaTable [ 2048 ];
    static const std::uint32_t s_exponentTable [ 64 ];
    static const std::uint16_t s_offsetTable [ 64 ];
    static const std::uint16_t s_baseTable [ 512 ];
    static const std::uint8_t s_shiftTable [ 512 ];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct no_component {};
struct BGRA {};
typedef BGRA BGR;
struct ARGB {};
struct ABGR {};
typedef no_component RGBA;
typedef no_component RGB;
struct DT {};
struct ST {};
struct DS {};

struct texture_format {};
template< typename ExtType > struct ext {};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template<
    typename C1,
    typename C2 = no_component,
    typename C3 = no_component,
    typename C4 = no_component,
    typename C5 = no_component >
struct format
{
    static const VkFormat code = VK_FORMAT_UNDEFINED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef void scalar_type;
    static const unsigned int length = 0;
    
    struct data_type
    {
    };

    struct init_type
    {
    };
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm4_t, unorm4_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R4G4_UNORM_PACK8;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint8_t, 4, 4 > r;
        unorm_t< std::uint8_t, 0, 4 > g;
        std::uint8_t u;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R4G4_UNORM_PACK8 )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm4_t, unorm4_t, unorm4_t, unorm4_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 12, 4 > r;
        unorm_t< std::uint16_t, 8, 4 > g;
        unorm_t< std::uint16_t, 4, 4 > b;
        unorm_t< std::uint16_t, 0, 4 > a;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R4G4B4A4_UNORM_PACK16 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm4_t, unorm4_t, unorm4_t, unorm4_t, BGRA >
{
    static const VkFormat code = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 12, 4 > b;
        unorm_t< std::uint16_t, 8, 4 > g;
        unorm_t< std::uint16_t, 4, 4 > r;
        unorm_t< std::uint16_t, 0, 4 > a;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_B4G4R4A4_UNORM_PACK16 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm5_t, unorm6_t, unorm5_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R5G6B5_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 11, 5 > r;
        unorm_t< std::uint16_t, 5, 6 > g;
        unorm_t< std::uint16_t, 0, 5 > b;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R5G6B5_UNORM_PACK16 )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm5_t, unorm6_t, unorm5_t, BGRA, no_component >
{
    static const VkFormat code = VK_FORMAT_B5G6R5_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 11, 5 > b;
        unorm_t< std::uint16_t, 5, 6 > g;
        unorm_t< std::uint16_t, 0, 5 > r;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_B5G6R5_UNORM_PACK16 )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm5_t, unorm5_t, unorm5_t, unorm1_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 11, 5 > r;
        unorm_t< std::uint16_t, 6, 5 > g;
        unorm_t< std::uint16_t, 1, 5 > b;
        unorm_t< std::uint16_t, 0, 1 > a;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R5G5B5A1_UNORM_PACK16 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm5_t, unorm5_t, unorm5_t, unorm1_t, BGRA >
{
    static const VkFormat code = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 11, 5 > b;
        unorm_t< std::uint16_t, 6, 5 > g;
        unorm_t< std::uint16_t, 1, 5 > r;
        unorm_t< std::uint16_t, 0, 1 > a;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_B5G5R5A1_UNORM_PACK16 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm1_t, unorm5_t, unorm5_t, unorm5_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A1R5G5B5_UNORM_PACK16;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint16_t, 15, 1 > a;
        unorm_t< std::uint16_t, 10, 5 > r;
        unorm_t< std::uint16_t, 5, 5 > g;
        unorm_t< std::uint16_t, 0, 5 > b;
        std::uint16_t u;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_A1R5G5B5_UNORM_PACK16 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatR8;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_UNORM )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatR8Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_SNORM )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_USCALED )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_SSCALED )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR8ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( std::uint8_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_UINT )" );

    typedef TInitialValue< std::uint32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR8i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( std::int8_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_SINT )" );

    typedef TInitialValue< std::int32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t r;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_R8_SRGB )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg8;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t r;
        unorm8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_UNORM )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg8Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t r;
        snorm8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_SNORM )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t r;
        uscaled8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_USCALED )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t r;
        sscaled8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_SSCALED )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, std::uint8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg8ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, std::uint8_t _g = 0, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( std::uint8_t _r, std::uint8_t _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t r;
        std::uint8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_UINT )" );

    typedef TInitialValue< std::uint32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, std::int8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg8i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, std::int8_t _g = 0, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( std::int8_t _r, std::int8_t _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t r;
        std::int8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_SINT )" );

    typedef TInitialValue< std::int32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t r;
        srgb8_t g;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R8G8_SRGB )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, unorm8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t r;
        unorm8_t g;
        unorm8_t b;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_UNORM )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, snorm8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t r;
        snorm8_t g;
        snorm8_t b;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_SNORM )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, uscaled8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t r;
        uscaled8_t g;
        uscaled8_t b;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_USCALED )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, sscaled8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t r;
        sscaled8_t g;
        sscaled8_t b;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_SSCALED )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, std::uint8_t, std::uint8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, std::uint8_t _g = 0, std::uint8_t _b = 0, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::uint8_t _r, std::uint8_t _g, std::uint8_t _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
    };

    typedef TInitialValue< std::uint32_t, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, std::int8_t, std::int8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, std::int8_t _g = 0, std::int8_t _b = 0, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::int8_t _r, std::int8_t _g, std::int8_t _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t r;
        std::int8_t g;
        std::int8_t b;
    };

    typedef TInitialValue< std::int32_t, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, srgb8_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t r;
        srgb8_t g;
        srgb8_t b;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_R8G8B8_SRGB )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, unorm8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t b;
        unorm8_t g;
        unorm8_t r;
    };
    
    typedef TInitialValue< float, 3, IVT_COLOR > init_type;

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_UNORM )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, snorm8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t b;
        snorm8_t g;
        snorm8_t r;
    };
    
    typedef TInitialValue< float, 3, IVT_COLOR > init_type;

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_SNORM )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, uscaled8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t b;
        uscaled8_t g;
        uscaled8_t r;
    };
    
    typedef TInitialValue< float, 3, IVT_COLOR > init_type;

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_USCALED )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, sscaled8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t b;
        sscaled8_t g;
        sscaled8_t r;
    };
    
    typedef TInitialValue< float, 3, IVT_COLOR > init_type;

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_SSCALED )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, std::uint8_t, std::uint8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, std::uint8_t _g = 0, std::uint8_t _b = 0, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::uint8_t _r, std::uint8_t _g, std::uint8_t _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t b;
        std::uint8_t g;
        std::uint8_t r;
    };

    typedef TInitialValue< std::uint32_t, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, std::int8_t, std::int8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, std::int8_t _g = 0, std::int8_t _b = 0, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::int8_t _r, std::int8_t _g, std::int8_t _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t b;
        std::int8_t g;
        std::int8_t r;
    };

    typedef TInitialValue< std::int32_t, 3, IVT_COLOR > init_type;

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, srgb8_t, BGR, no_component >
{
    static const VkFormat code = VK_FORMAT_B8G8R8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g, float _b )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t b;
        srgb8_t g;
        srgb8_t r;
    };

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
    
    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_B8G8R8_SRGB )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, unorm8_t, unorm8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba8;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t r;
        unorm8_t g;
        unorm8_t b;
        unorm8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_UNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, snorm8_t, snorm8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba8Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t r;
        snorm8_t g;
        snorm8_t b;
        snorm8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_SNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t r;
        uscaled8_t g;
        uscaled8_t b;
        uscaled8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_USCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t r;
        sscaled8_t g;
        sscaled8_t b;
        sscaled8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_SSCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba8ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, std::uint8_t _g = 0, std::uint8_t _b = 0, std::uint8_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_UINT )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, std::int8_t, std::int8_t, std::int8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba8i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, std::int8_t _g = 0, std::int8_t _b = 0, std::int8_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int8_t _r, std::int8_t _g, std::int8_t _b, std::int8_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t r;
        std::int8_t g;
        std::int8_t b;
        std::int8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_SINT )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, srgb8_t, srgb8_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R8G8B8A8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t r;
        srgb8_t g;
        srgb8_t b;
        srgb8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R8G8B8A8_SRGB )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, unorm8_t, unorm8_t, BGRA >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm8_t b;
        unorm8_t g;
        unorm8_t r;
        unorm8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_UNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, snorm8_t, snorm8_t, BGR >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm8_t b;
        snorm8_t g;
        snorm8_t r;
        snorm8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_SNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t, BGR >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled8_t b;
        uscaled8_t g;
        uscaled8_t r;
        uscaled8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_USCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t, BGR >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled8_t b;
        sscaled8_t g;
        sscaled8_t r;
        sscaled8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_SSCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, BGR >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _r, std::uint8_t _g = 0, std::uint8_t _b = 0, std::uint8_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t b;
        std::uint8_t g;
        std::uint8_t r;
        std::uint8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_UINT )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int8_t, std::int8_t, std::int8_t, std::int8_t, BGR >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int8_t _r, std::int8_t _g = 0, std::int8_t _b = 0, std::int8_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int8_t _r, std::int8_t _g, std::int8_t _b, std::int8_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int8_t b;
        std::int8_t g;
        std::int8_t r;
        std::int8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_SINT )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, srgb8_t, srgb8_t, BGRA >
{
    static const VkFormat code = VK_FORMAT_B8G8R8A8_SRGB;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb8_t b;
        srgb8_t g;
        srgb8_t r;
        srgb8_t a;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_B8G8R8A8_SRGB )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm8_t, unorm8_t, unorm8_t, unorm8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_UNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint32_t, 24, 8 > a;
        unorm_t< std::uint32_t, 16, 8 > b;
        unorm_t< std::uint32_t, 8, 8 > g;
        unorm_t< std::uint32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_UNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm8_t, snorm8_t, snorm8_t, snorm8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_SNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm_t< std::int32_t, 24, 8 > a;
        snorm_t< std::int32_t, 16, 8 > b;
        snorm_t< std::int32_t, 8, 8 > g;
        snorm_t< std::int32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_SNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_USCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled_t< std::uint32_t, 24, 8 > a;
        uscaled_t< std::uint32_t, 16, 8 > b;
        uscaled_t< std::uint32_t, 8, 8 > g;
        uscaled_t< std::uint32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_USCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled_t< std::int32_t, 24, 8 > a;
        sscaled_t< std::int32_t, 16, 8 > b;
        sscaled_t< std::int32_t, 8, 8 > g;
        sscaled_t< std::int32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_SSCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< usint8_t, usint8_t, usint8_t, usint8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_UINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _r, std::uint32_t _g = 0, std::uint32_t _b = 0, std::uint32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, std::uint32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        usint_t< std::uint32_t, 24, 8 > a;
        usint_t< std::uint32_t, 16, 8 > b;
        usint_t< std::uint32_t, 8, 8 > g;
        usint_t< std::uint32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_UINT_PACK32 )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sint8_t, sint8_t, sint8_t, sint8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_SINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _r, std::int32_t _g = 0, std::int32_t _b = 0, std::int32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g, std::int32_t _b, std::int32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sint_t< std::int32_t, 24, 8 > a;
        sint_t< std::int32_t, 16, 8 > b;
        sint_t< std::int32_t, 8, 8 > g;
        sint_t< std::int32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_SINT_PACK32 )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< srgb8_t, srgb8_t, srgb8_t, srgb8_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A8B8G8R8_SRGB_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        srgb_t< std::uint32_t, 24, 8 > a;
        srgb_t< std::uint32_t, 16, 8 > b;
        srgb_t< std::uint32_t, 8, 8 > g;
        srgb_t< std::uint32_t, 0, 8 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A8B8G8R8_SRGB_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm2_t, unorm10_t, unorm10_t, unorm10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint32_t, 30, 2 > a;
        unorm_t< std::uint32_t, 20, 10 > r;
        unorm_t< std::uint32_t, 10, 10 > g;
        unorm_t< std::uint32_t, 0, 10 > b;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_UNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm2_t, snorm10_t, snorm10_t, snorm10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_SNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm_t< std::int32_t, 30, 2 > a;
        snorm_t< std::int32_t, 20, 10 > r;
        snorm_t< std::int32_t, 10, 10 > g;
        snorm_t< std::int32_t, 0, 10 > b;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_SNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled2_t, uscaled10_t, uscaled10_t, uscaled10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_USCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled_t< std::int32_t, 30, 2 > a;
        uscaled_t< std::int32_t, 20, 10 > r;
        uscaled_t< std::int32_t, 10, 10 > g;
        uscaled_t< std::int32_t, 0, 10 > b;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_USCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled2_t, sscaled10_t, sscaled10_t, sscaled10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled_t< std::int32_t, 30, 2 > a;
        sscaled_t< std::int32_t, 20, 10 > r;
        sscaled_t< std::int32_t, 10, 10 > g;
        sscaled_t< std::int32_t, 0, 10 > b;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_SSCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< usint2_t, usint10_t, usint10_t, usint10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_UINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _r, std::uint32_t _g = 0, std::uint32_t _b = 0, std::uint32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, std::uint32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        usint_t< std::uint32_t, 30, 2 > a;
        usint_t< std::uint32_t, 20, 10 > r;
        usint_t< std::uint32_t, 10, 10 > g;
        usint_t< std::uint32_t, 0, 10 > b;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_UINT_PACK32 )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sint2_t, sint10_t, sint10_t, sint10_t, ARGB >
{
    static const VkFormat code = VK_FORMAT_A2R10G10B10_SINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _r, std::int32_t _g = 0, std::int32_t _b = 0, std::int32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g, std::int32_t _b, std::int32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sint_t< std::int32_t, 30, 2 > a;
        sint_t< std::int32_t, 20, 10 > r;
        sint_t< std::int32_t, 10, 10 > g;
        sint_t< std::int32_t, 0, 10 > b;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2R10G10B10_SINT_PACK32 )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm2_t, unorm10_t, unorm10_t, unorm10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgb10A2;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm_t< std::uint32_t, 30, 2 > a;
        unorm_t< std::uint32_t, 20, 10 > b;
        unorm_t< std::uint32_t, 10, 10 > g;
        unorm_t< std::uint32_t, 0, 10 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_UNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm2_t, snorm10_t, snorm10_t, snorm10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_SNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm_t< std::int32_t, 30, 2 > a;
        snorm_t< std::int32_t, 20, 10 > b;
        snorm_t< std::int32_t, 10, 10 > g;
        snorm_t< std::int32_t, 0, 10 > r;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_SNORM_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled2_t, uscaled10_t, uscaled10_t, uscaled10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_USCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled_t< std::int32_t, 30, 2 > a;
        uscaled_t< std::int32_t, 20, 10 > b;
        uscaled_t< std::int32_t, 10, 10 > g;
        uscaled_t< std::int32_t, 0, 10 > r;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_USCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled2_t, sscaled10_t, sscaled10_t, sscaled10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g, float _b, float _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled_t< std::int32_t, 30, 2 > a;
        sscaled_t< std::int32_t, 20, 10 > b;
        sscaled_t< std::int32_t, 10, 10 > g;
        sscaled_t< std::int32_t, 0, 10 > r;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_SSCALED_PACK32 )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< usint2_t, usint10_t, usint10_t, usint10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_UINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgb10a2ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _r, std::uint32_t _g = 0, std::uint32_t _b = 0, std::uint32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, std::uint32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        usint_t< std::uint32_t, 30, 2 > a;
        usint_t< std::uint32_t, 20, 10 > b;
        usint_t< std::uint32_t, 10, 10 > g;
        usint_t< std::uint32_t, 0, 10 > r;
        std::uint32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_UINT_PACK32 )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sint2_t, sint10_t, sint10_t, sint10_t, ABGR >
{
    static const VkFormat code = VK_FORMAT_A2B10G10R10_SINT_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _r, std::int32_t _g = 0, std::int32_t _b = 0, std::int32_t _a = 0, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g, std::int32_t _b, std::int32_t _a )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return u == rhs.u; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sint_t< std::int32_t, 30, 2 > a;
        sint_t< std::int32_t, 20, 10 > b;
        sint_t< std::int32_t, 10, 10 > g;
        sint_t< std::int32_t, 0, 10 > r;
        std::int32_t u;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_A2B10G10R10_SINT_PACK32 )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_UNORM )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_SNORM )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_USCALED )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_SSCALED )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint16_t _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( std::uint16_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_UINT )" );

    typedef TInitialValue< std::uint32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int16_t _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( std::int16_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_SINT )" );

    typedef TInitialValue< std::int32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float16_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; }

        VPP_INLINE void set ( float _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float16_t r;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_R16_SFLOAT )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, unorm16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg16;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t r;
        unorm16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_UNORM )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm16_t, snorm16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg16Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm16_t r;
        snorm16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_SNORM )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled16_t, uscaled16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled16_t r;
        uscaled16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_USCALED )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled16_t, sscaled16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled16_t r;
        sscaled16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_SSCALED )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint16_t, std::uint16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg16ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint16_t _r, std::uint16_t _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( std::uint16_t _r, std::uint16_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint16_t r;
        std::uint16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_UINT )" );

    typedef TInitialValue< std::uint32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int16_t, std::int16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg16i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int16_t _r, std::int16_t _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( std::int16_t _r, std::int16_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int16_t r;
        std::int16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_SINT )" );

    typedef TInitialValue< std::int32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float16_t, float16_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg16f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; }

        VPP_INLINE void set ( float _r, float _g = 0.0f )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float16_t r;
        float16_t g;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R16G16_SFLOAT )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, unorm16_t, unorm16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t r;
        unorm16_t g;
        unorm16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_UNORM )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm16_t, snorm16_t, snorm16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm16_t r;
        snorm16_t g;
        snorm16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_SNORM )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled16_t, uscaled16_t, uscaled16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled16_t r;
        uscaled16_t g;
        uscaled16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_USCALED )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled16_t, sscaled16_t, sscaled16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled16_t r;
        sscaled16_t g;
        sscaled16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_SSCALED )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint16_t, std::uint16_t, std::uint16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint16_t _r, std::uint16_t _g, std::uint16_t _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::uint16_t _r, std::uint16_t _g = 0, std::uint16_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint16_t r;
        std::uint16_t g;
        std::uint16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_UINT )" );

    typedef TInitialValue< std::uint32_t, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int16_t, std::int16_t, std::int16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int16_t _r, std::int16_t _g, std::int16_t _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::int16_t _r, std::int16_t _g = 0, std::int16_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int16_t r;
        std::int16_t g;
        std::int16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_SINT )" );

    typedef TInitialValue< std::int32_t, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float16_t, float16_t, float16_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, no_component _a = no_component(), no_component _n = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float16_t r;
        float16_t g;
        float16_t b;
    };

    static_assert ( sizeof ( data_type ) == 6, "sizeof ( VK_FORMAT_R16G16B16_SFLOAT )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, unorm16_t, unorm16_t, unorm16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba16;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t r;
        unorm16_t g;
        unorm16_t b;
        unorm16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_UNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< snorm16_t, snorm16_t, snorm16_t, snorm16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_SNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba16Snorm;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        snorm16_t r;
        snorm16_t g;
        snorm16_t b;
        snorm16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_SNORM )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< uscaled16_t, uscaled16_t, uscaled16_t, uscaled16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_USCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        uscaled16_t r;
        uscaled16_t g;
        uscaled16_t b;
        uscaled16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_USCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< sscaled16_t, sscaled16_t, sscaled16_t, sscaled16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_SSCALED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        sscaled16_t r;
        sscaled16_t g;
        sscaled16_t b;
        sscaled16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_SSCALED )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba16ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint16_t _r, std::uint16_t _g, std::uint16_t _b, std::uint16_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint16_t _r, std::uint16_t _g = 0, std::uint16_t _b = 0, std::uint16_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint16_t r;
        std::uint16_t g;
        std::uint16_t b;
        std::uint16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_UINT )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int16_t, std::int16_t, std::int16_t, std::int16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba16i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int16_t _r, std::int16_t _g, std::int16_t _b, std::int16_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int16_t _r, std::int16_t _g = 0, std::int16_t _b = 0, std::int16_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int16_t r;
        std::int16_t g;
        std::int16_t b;
        std::int16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_SINT )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float16_t, float16_t, float16_t, float16_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R16G16B16A16_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba16f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _r, float _g, float _b, float _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( float _r, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float16_t r;
        float16_t g;
        float16_t b;
        float16_t a;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R16G16B16A16_SFLOAT )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint32_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { r = _x; }

        VPP_INLINE void set ( std::uint32_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint32_t r;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R32_UINT )" );

    typedef TInitialValue< std::uint32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int32_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { r = _x; }

        VPP_INLINE void set ( std::int32_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int32_t r;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R32_SINT )" );

    typedef TInitialValue< std::int32_t, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { x = _x; }

        VPP_INLINE void set ( float _x )
        { x = _x; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { float r; float x; };
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_R32_SFLOAT )" );

    typedef TInitialValue< float, 1, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint32_t, std::uint32_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg32ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _x, std::uint32_t _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { r = _x; g = _y; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint32_t r;
        std::uint32_t g;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R32G32_UINT )" );

    typedef TInitialValue< std::uint32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int32_t, std::int32_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg32i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _x, std::int32_t _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { r = _x; g = _y; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int32_t r;
        std::int32_t g;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R32G32_SINT )" );

    typedef TInitialValue< std::int32_t, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, float, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRg32f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _x, float _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { x = _x; y = _y; }

        VPP_INLINE void set ( float _x, float _y = 0.0f )
        { x = _x; y = _y; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { float r; float x; };
        union { float g; float y; };
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R32G32_SFLOAT )" );

    typedef TInitialValue< float, 2, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint32_t, std::uint32_t, std::uint32_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g = 0, std::uint32_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint32_t r;
        std::uint32_t g;
        std::uint32_t b;
    };

    static_assert ( sizeof ( data_type ) == 12, "sizeof ( VK_FORMAT_R32G32B32_UINT )" );

    typedef TInitialValue< std::uint32_t, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int32_t, std::int32_t, std::int32_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _r, std::int32_t _g, std::int32_t _b, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g = 0, std::int32_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int32_t r;
        std::int32_t g;
        std::int32_t b;
    };

    static_assert ( sizeof ( data_type ) == 12, "sizeof ( VK_FORMAT_R32G32B32_SINT )" );

    typedef TInitialValue< std::int32_t, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, float, float, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _x, float _y, float _z, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { x = _x; y = _y; z = _z; }

        VPP_INLINE void set ( float _x, float _y = 0.0f, float _z = 0.0f )
        { x = _x; y = _y; z = _z; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { float r; float x; };
        union { float g; float y; };
        union { float b; float z; };
    };

    static_assert ( sizeof ( data_type ) == 12, "sizeof ( VK_FORMAT_R32G32B32_SFLOAT )" );

    typedef TInitialValue< float, 3, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32A32_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba32ui;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, std::uint32_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint32_t _r, std::uint32_t _g = 0, std::uint32_t _b = 0, std::uint32_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint32_t r;
        std::uint32_t g;
        std::uint32_t b;
        std::uint32_t a;
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R32G32B32A32_UINT )" );

    typedef TInitialValue< std::uint32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int32_t, std::int32_t, std::int32_t, std::int32_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32A32_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba32i;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int32_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int32_t _r, std::int32_t _g, std::int32_t _b, std::int32_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int32_t _r, std::int32_t _g = 0, std::int32_t _b = 0, std::int32_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int32_t r;
        std::int32_t g;
        std::int32_t b;
        std::int32_t a;
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R32G32B32A32_SINT )" );

    typedef TInitialValue< std::int32_t, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, float, float, float, no_component >
{
    static const VkFormat code = VK_FORMAT_R32G32B32A32_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatRgba32f;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef float scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _x, float _y, float _z, float _w, no_component _n = no_component() )
        { x = _x; y = _y; z = _z; w = _w; }

        VPP_INLINE void set ( float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f )
        { x = _x; y = _y; z = _z; w = _w; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { float r; float x; };
        union { float g; float y; };
        union { float b; float z; };
        union { float a; float w; };
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R32G32B32A32_SFLOAT )" );

    typedef TInitialValue< float, 4, IVT_COLOR > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint64_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint64_t scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint64_t _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { r = _x; }

        VPP_INLINE void set ( std::uint64_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint64_t r;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R64_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int64_t, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int64_t scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int64_t _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { r = _x; }

        VPP_INLINE void set ( std::int64_t _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int64_t r;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R64_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< double, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef double scalar_type;
    static const unsigned int length = 1;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( double _x, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component(), no_component _n4 = no_component() )
        { r = _x; }

        VPP_INLINE void set ( double _r )
        { r = _r; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        double r;
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_R64_SFLOAT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint64_t, std::uint64_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint64_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint64_t _x, std::uint64_t _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { r = _x; g = _y; }

        VPP_INLINE void set ( std::uint64_t _r, std::uint64_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint64_t r;
        std::uint64_t g;
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R64G64_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int64_t, std::int64_t, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int64_t scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int64_t _x, std::int64_t _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { r = _x; g = _y; }

        VPP_INLINE void set ( std::int64_t _r, std::int64_t _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int64_t r;
        std::int64_t g;
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R64G64_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< double, double, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef double scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( double _x, double _y, no_component _n1 = no_component(), no_component _n2 = no_component(), no_component _n3 = no_component() )
        { r = _x; g = _y; }

        VPP_INLINE void set ( double _r, double _g = 0 )
        { r = _r; g = _g; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { double r; double x; };
        union { double g; double y; };
    };

    static_assert ( sizeof ( data_type ) == 16, "sizeof ( VK_FORMAT_R64G64_SFLOAT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint64_t, std::uint64_t, std::uint64_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint64_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint64_t _r, std::uint64_t _g, std::uint64_t _b, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::uint64_t _r, std::uint64_t _g = 0, std::uint64_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint64_t r;
        std::uint64_t g;
        std::uint64_t b;
    };

    static_assert ( sizeof ( data_type ) == 24, "sizeof ( VK_FORMAT_R64G64B64_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int64_t, std::int64_t, std::int64_t, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int64_t scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int64_t _r, std::int64_t _g, std::int64_t _b, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( std::int64_t _r, std::int64_t _g = 0, std::int64_t _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int64_t r;
        std::int64_t g;
        std::int64_t b;
    };

    static_assert ( sizeof ( data_type ) == 24, "sizeof ( VK_FORMAT_R64G64B64_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< double, double, double, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef double scalar_type;
    static const unsigned int length = 3;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( double _r, double _g, double _b, no_component _n1 = no_component(), no_component _n2 = no_component() )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE void set ( double _r, double _g = 0, double _b = 0 )
        { r = _r; g = _g; b = _b; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { double r; double x; };
        union { double g; double y; };
        union { double b; double z; };
    };

    static_assert ( sizeof ( data_type ) == 24, "sizeof ( VK_FORMAT_R64G64B64_SFLOAT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64A64_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::uint64_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint64_t _r, std::uint64_t _g, std::uint64_t _b, std::uint64_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::uint64_t _r, std::uint64_t _g = 0, std::uint64_t _b = 0, std::uint64_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint64_t r;
        std::uint64_t g;
        std::uint64_t b;
        std::uint64_t a;
    };

    static_assert ( sizeof ( data_type ) == 32, "sizeof ( VK_FORMAT_R64G64B64A64_UINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< std::int64_t, std::int64_t, std::int64_t, std::int64_t, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64A64_SINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef std::int64_t scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::int64_t _r, std::int64_t _g, std::int64_t _b, std::int64_t _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( std::int64_t _r, std::int64_t _g = 0, std::int64_t _b = 0, std::int64_t _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::int64_t r;
        std::int64_t g;
        std::int64_t b;
        std::int64_t a;
    };

    static_assert ( sizeof ( data_type ) == 32, "sizeof ( VK_FORMAT_R64G64B64A64_SINT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< double, double, double, double, no_component >
{
    static const VkFormat code = VK_FORMAT_R64G64B64A64_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;

    typedef double scalar_type;
    static const unsigned int length = 4;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( double _r, double _g, double _b, double _a, no_component _n = no_component() )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE void set ( double _r, double _g = 0, double _b = 0, double _a = 0 )
        { r = _r; g = _g; b = _b; a = _a; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        union { double r; double x; };
        union { double g; double y; };
        union { double b; double z; };
        union { double a; double w; };
    };

    static_assert ( sizeof ( data_type ) == 32, "sizeof ( VK_FORMAT_R64G64B64A64_SFLOAT )" );
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, DT, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_D16_UNORM;
    static const spv::ImageFormat spvCode = spv::ImageFormatR16;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; }

        VPP_INLINE void set ( float _d )
        { d = _d; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t d;
    };

    static_assert ( sizeof ( data_type ) == 2, "sizeof ( VK_FORMAT_D16_UNORM )" );

    typedef TInitialValue< float, 1, IVT_DEPTH > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm24_t, DT, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_X8_D24_UNORM_PACK32;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; }

        VPP_INLINE void set ( float _d )
        { d = _d; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm24_t d;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_X8_D24_UNORM_PACK32 )" );

    typedef TInitialValue< float, 1, IVT_DEPTH > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, DT, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_D32_SFLOAT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32f;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT;

    typedef float scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; }

        VPP_INLINE void set ( float _d )
        { d = _d; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float d;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_D32_SFLOAT )" );

    typedef TInitialValue< float, 1, IVT_DEPTH > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< std::uint8_t, ST, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_S8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR8ui;
    static const bool isColor = false;
    static const bool isDepth = false;
    static const bool isStencil = true;
    static const unsigned int aspects = VK_IMAGE_ASPECT_STENCIL_BIT;

    typedef std::uint32_t scalar_type;
    static const unsigned int length = 1;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( std::uint8_t _s, no_component _g = no_component(), no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { s = _s; }

        VPP_INLINE void set ( std::uint8_t _s )
        { s = _s; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return s == rhs.s; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        std::uint8_t s;
    };

    static_assert ( sizeof ( data_type ) == 1, "sizeof ( VK_FORMAT_S8_UINT )" );

    typedef TInitialValue< std::uint32_t, 1, IVT_STENCIL > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm16_t, std::uint8_t, DS, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_D16_UNORM_S8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32f;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = true;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    // FIXME - what is portable pragma pack?
    #pragma pack ( 1 )
    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, std::uint8_t _s, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; s = _s; }

        VPP_INLINE void set ( float _d, std::uint8_t _s = 0 )
        { d = _d; s = _s; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d && s == rhs.s; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm16_t d;
        std::uint8_t s;
    };

    static_assert ( sizeof ( data_type ) == 3, "sizeof ( VK_FORMAT_D16_UNORM_S8_UINT )" );

    typedef TInitialValue< float, 2, IVT_DS > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< unorm24_t, std::uint8_t, DS, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_D24_UNORM_S8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32f;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = true;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    union data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, std::uint8_t _s, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; s = _s; }

        VPP_INLINE void set ( float _d, std::uint8_t _s = 0 )
        { d = _d; s = _s; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d && s == rhs.s; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        unorm24_t d;
        usint_t< std::uint32_t, 24, 8 > s;
    };

    static_assert ( sizeof ( data_type ) == 4, "sizeof ( VK_FORMAT_D24_UNORM_S8_UINT )" );

    typedef TInitialValue< float, 2, IVT_DS > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< float, std::uint8_t, DS, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_D32_SFLOAT_S8_UINT;
    static const spv::ImageFormat spvCode = spv::ImageFormatR32f;
    static const bool isColor = false;
    static const bool isDepth = true;
    static const bool isStencil = true;
    static const unsigned int aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    typedef float scalar_type;
    static const unsigned int length = 2;

    struct data_type
    {
        VPP_INLINE data_type()
        {}

        VPP_INLINE data_type ( float _d, std::uint8_t _s, no_component _b = no_component(), no_component _a = no_component(), no_component _n = no_component() )
        { d = _d; s = _s; }

        VPP_INLINE void set ( float _d, std::uint8_t _s = 0 )
        { d = _d; s = _s; }

        VPP_INLINE bool operator== ( const data_type& rhs ) const
        { return d == rhs.d && s == rhs.s; }

        VPP_INLINE bool operator!= ( const data_type& rhs ) const
        { return ! operator== ( rhs ); }

        float d;
        union { std::uint8_t s; std::uint32_t x; };
    };

    static_assert ( sizeof ( data_type ) == 8, "sizeof ( VK_FORMAT_D32_SFLOAT_S8_UINT )" );

    typedef TInitialValue< float, 2, IVT_DS > init_type;
};

// -----------------------------------------------------------------------------

template<>
struct format< texture_format, no_component, no_component, no_component, no_component >
{
    static const VkFormat code = VK_FORMAT_UNDEFINED;
    static const spv::ImageFormat spvCode = spv::ImageFormatUnknown;
    static const bool isColor = true;
    static const bool isDepth = false;
    static const bool isStencil = false;
    static const unsigned int aspects = VK_IMAGE_ASPECT_COLOR_BIT;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< VkFormat FMT >
struct get_format {};

template<> struct get_format< VK_FORMAT_R4G4_UNORM_PACK8 >
{ typedef format< unorm4_t, unorm4_t > fmt; };

template<> struct get_format< VK_FORMAT_R4G4B4A4_UNORM_PACK16 >
{ typedef format< unorm4_t, unorm4_t, unorm4_t, unorm4_t > fmt; };

template<> struct get_format< VK_FORMAT_B4G4R4A4_UNORM_PACK16 >
{ typedef format< unorm4_t, unorm4_t, unorm4_t, unorm4_t, BGRA > fmt; };

template<> struct get_format< VK_FORMAT_R5G6B5_UNORM_PACK16 >
{ typedef format< unorm5_t, unorm6_t, unorm5_t > fmt; };

template<> struct get_format< VK_FORMAT_B5G6R5_UNORM_PACK16 >
{ typedef format< unorm5_t, unorm6_t, unorm5_t, BGRA > fmt; };

template<> struct get_format< VK_FORMAT_R5G5B5A1_UNORM_PACK16 >
{ typedef format< unorm5_t, unorm5_t, unorm5_t, unorm1_t > fmt; };

template<> struct get_format< VK_FORMAT_B5G5R5A1_UNORM_PACK16 >
{ typedef format< unorm5_t, unorm5_t, unorm5_t, unorm1_t, BGRA > fmt; };

template<> struct get_format< VK_FORMAT_A1R5G5B5_UNORM_PACK16 >
{ typedef format< unorm1_t, unorm5_t, unorm5_t, unorm5_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_R8_UNORM >
{ typedef format< unorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_SNORM >
{ typedef format< snorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_USCALED >
{ typedef format< uscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_SSCALED >
{ typedef format< sscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_UINT >
{ typedef format< std::uint8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_SINT >
{ typedef format< std::int8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8_SRGB >
{ typedef format< srgb8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_UNORM >
{ typedef format< unorm8_t, unorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_SNORM >
{ typedef format< snorm8_t, snorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_USCALED >
{ typedef format< uscaled8_t, uscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_SSCALED >
{ typedef format< sscaled8_t, sscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_UINT >
{ typedef format< std::uint8_t, std::uint8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_SINT >
{ typedef format< std::int8_t, std::int8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8_SRGB >
{ typedef format< srgb8_t, srgb8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_UNORM >
{ typedef format< unorm8_t, unorm8_t, unorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_SNORM >
{ typedef format< snorm8_t, snorm8_t, snorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_USCALED >
{ typedef format< uscaled8_t, uscaled8_t, uscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_SSCALED >
{ typedef format< sscaled8_t, sscaled8_t, sscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_UINT >
{ typedef format< std::uint8_t, std::uint8_t, std::uint8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_SINT >
{ typedef format< std::int8_t, std::int8_t, std::int8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8_SRGB >
{ typedef format< srgb8_t, srgb8_t, srgb8_t > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_UNORM >
{ typedef format< unorm8_t, unorm8_t, unorm8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_SNORM >
{ typedef format< snorm8_t, snorm8_t, snorm8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_USCALED >
{ typedef format< uscaled8_t, uscaled8_t, uscaled8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_SSCALED >
{ typedef format< sscaled8_t, sscaled8_t, sscaled8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_UINT >
{ typedef format< std::uint8_t, std::uint8_t, std::uint8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_SINT >
{ typedef format< std::int8_t, std::int8_t, std::int8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8_SRGB >
{ typedef format< srgb8_t, srgb8_t, srgb8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_UNORM >
{ typedef format< unorm8_t, unorm8_t, unorm8_t, unorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_SNORM >
{ typedef format< snorm8_t, snorm8_t, snorm8_t, snorm8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_USCALED >
{ typedef format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_SSCALED >
{ typedef format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_UINT >
{ typedef format< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_SINT >
{ typedef format< std::int8_t, std::int8_t, std::int8_t, std::int8_t > fmt; };

template<> struct get_format< VK_FORMAT_R8G8B8A8_SRGB >
{ typedef format< srgb8_t, srgb8_t, srgb8_t, srgb8_t > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_UNORM >
{ typedef format< unorm8_t, unorm8_t, unorm8_t, unorm8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_SNORM >
{ typedef format< snorm8_t, snorm8_t, snorm8_t, snorm8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_USCALED >
{ typedef format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_SSCALED >
{ typedef format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_UINT >
{ typedef format< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_SINT >
{ typedef format< std::int8_t, std::int8_t, std::int8_t, std::int8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_B8G8R8A8_SRGB >
{ typedef format< srgb8_t, srgb8_t, srgb8_t, srgb8_t, BGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_UNORM_PACK32 >
{ typedef format< unorm8_t, unorm8_t, unorm8_t, unorm8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_SNORM_PACK32 >
{ typedef format< snorm8_t, snorm8_t, snorm8_t, snorm8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_USCALED_PACK32 >
{ typedef format< uscaled8_t, uscaled8_t, uscaled8_t, uscaled8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_SSCALED_PACK32 >
{ typedef format< sscaled8_t, sscaled8_t, sscaled8_t, sscaled8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_UINT_PACK32 >
{ typedef format< usint8_t, usint8_t, usint8_t, usint8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_SINT_PACK32 >
{ typedef format< sint8_t, sint8_t, sint8_t, sint8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A8B8G8R8_SRGB_PACK32 >
{ typedef format< srgb8_t, srgb8_t, srgb8_t, srgb8_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_UNORM_PACK32 >
{ typedef format< unorm2_t, unorm10_t, unorm10_t, unorm10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_SNORM_PACK32 >
{ typedef format< snorm2_t, snorm10_t, snorm10_t, snorm10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_USCALED_PACK32 >
{ typedef format< uscaled2_t, uscaled10_t, uscaled10_t, uscaled10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_SSCALED_PACK32 >
{ typedef format< sscaled2_t, sscaled10_t, sscaled10_t, sscaled10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_UINT_PACK32 >
{ typedef format< usint2_t, usint10_t, usint10_t, usint10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2R10G10B10_SINT_PACK32 >
{ typedef format< sint2_t, sint10_t, sint10_t, sint10_t, ARGB > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_UNORM_PACK32 >
{ typedef format< unorm2_t, unorm10_t, unorm10_t, unorm10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_SNORM_PACK32 >
{ typedef format< snorm2_t, snorm10_t, snorm10_t, snorm10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_USCALED_PACK32 >
{ typedef format< uscaled2_t, uscaled10_t, uscaled10_t, uscaled10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_SSCALED_PACK32 >
{ typedef format< sscaled2_t, sscaled10_t, sscaled10_t, sscaled10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_UINT_PACK32 >
{ typedef format< usint2_t, usint10_t, usint10_t, usint10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_A2B10G10R10_SINT_PACK32 >
{ typedef format< sint2_t, sint10_t, sint10_t, sint10_t, ABGR > fmt; };

template<> struct get_format< VK_FORMAT_R16_UNORM >
{ typedef format< unorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_SNORM >
{ typedef format< snorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_USCALED >
{ typedef format< uscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_SSCALED >
{ typedef format< sscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_UINT >
{ typedef format< std::uint16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_SINT >
{ typedef format< std::int16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16_SFLOAT >
{ typedef format< float16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_UNORM >
{ typedef format< unorm16_t, unorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_SNORM >
{ typedef format< snorm16_t, snorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_USCALED >
{ typedef format< uscaled16_t, uscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_SSCALED >
{ typedef format< sscaled16_t, sscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_UINT >
{ typedef format< std::uint16_t, std::uint16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_SINT >
{ typedef format< std::int16_t, std::int16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16_SFLOAT >
{ typedef format< float16_t, float16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_UNORM >
{ typedef format< unorm16_t, unorm16_t, unorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_SNORM >
{ typedef format< snorm16_t, snorm16_t, snorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_USCALED >
{ typedef format< uscaled16_t, uscaled16_t, uscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_SSCALED >
{ typedef format< sscaled16_t, sscaled16_t, sscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_UINT >
{ typedef format< std::uint16_t, std::uint16_t, std::uint16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_SINT >
{ typedef format< std::int16_t, std::int16_t, std::int16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16_SFLOAT >
{ typedef format< float16_t, float16_t, float16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_UNORM >
{ typedef format< unorm16_t, unorm16_t, unorm16_t, unorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_SNORM >
{ typedef format< snorm16_t, snorm16_t, snorm16_t, snorm16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_USCALED >
{ typedef format< uscaled16_t, uscaled16_t, uscaled16_t, uscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_SSCALED >
{ typedef format< sscaled16_t, sscaled16_t, sscaled16_t, sscaled16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_UINT >
{ typedef format< std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_SINT >
{ typedef format< std::int16_t, std::int16_t, std::int16_t, std::int16_t > fmt; };

template<> struct get_format< VK_FORMAT_R16G16B16A16_SFLOAT >
{ typedef format< float16_t, float16_t, float16_t, float16_t > fmt; };

template<> struct get_format< VK_FORMAT_R32_UINT >
{ typedef format< std::uint32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32_SINT >
{ typedef format< std::int32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32_SFLOAT >
{ typedef format< float > fmt; };

template<> struct get_format< VK_FORMAT_R32G32_UINT >
{ typedef format< std::uint32_t, std::uint32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32_SINT >
{ typedef format< std::int32_t, std::int32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32_SFLOAT >
{ typedef format< float, float > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32_UINT >
{ typedef format< std::uint32_t, std::uint32_t, std::uint32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32_SINT >
{ typedef format< std::int32_t, std::int32_t, std::int32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32_SFLOAT >
{ typedef format< float, float, float > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32A32_UINT >
{ typedef format< std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32A32_SINT >
{ typedef format< std::int32_t, std::int32_t, std::int32_t, std::int32_t > fmt; };

template<> struct get_format< VK_FORMAT_R32G32B32A32_SFLOAT >
{ typedef format< float, float, float, float > fmt; };

template<> struct get_format< VK_FORMAT_R64_UINT >
{ typedef format< std::uint64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64_SINT >
{ typedef format< std::int64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64_SFLOAT >
{ typedef format< double > fmt; };

template<> struct get_format< VK_FORMAT_R64G64_UINT >
{ typedef format< std::uint64_t, std::uint64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64_SINT >
{ typedef format< std::int64_t, std::int64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64_SFLOAT >
{ typedef format< double, double > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64_UINT >
{ typedef format< std::uint64_t, std::uint64_t, std::uint64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64_SINT >
{ typedef format< std::int64_t, std::int64_t, std::int64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64_SFLOAT >
{ typedef format< double, double, double > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64A64_UINT >
{ typedef format< std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64A64_SINT >
{ typedef format< std::int64_t, std::int64_t, std::int64_t, std::int64_t > fmt; };

template<> struct get_format< VK_FORMAT_R64G64B64A64_SFLOAT >
{ typedef format< double, double, double, double > fmt; };

template<> struct get_format< VK_FORMAT_D16_UNORM >
{ typedef format< unorm16_t, DT > fmt; };

template<> struct get_format< VK_FORMAT_X8_D24_UNORM_PACK32 >
{ typedef format< unorm24_t, DT > fmt; };

template<> struct get_format< VK_FORMAT_D32_SFLOAT >
{ typedef format< float, DT > fmt; };

template<> struct get_format< VK_FORMAT_S8_UINT >
{ typedef format< std::uint8_t, ST > fmt; };

template<> struct get_format< VK_FORMAT_D16_UNORM_S8_UINT >
{ typedef format< unorm16_t, std::uint8_t, DS > fmt; };

template<> struct get_format< VK_FORMAT_D24_UNORM_S8_UINT >
{ typedef format< unorm24_t, std::uint8_t, DS > fmt; };

template<> struct get_format< VK_FORMAT_D32_SFLOAT_S8_UINT >
{ typedef format< float, std::uint8_t, DS > fmt; };

// -----------------------------------------------------------------------------

VPP_INLINE VkClearColorValue clearColor (
    float r, float g = 0.0f, float b = 0.0f, float a = 0.0f )
{
    VkClearColorValue result;
    result.float32 [ 0 ] = r;
    result.float32 [ 1 ] = g;
    result.float32 [ 2 ] = b;
    result.float32 [ 3 ] = a;
    return result;
}

VPP_INLINE VkClearColorValue clearColor (
    int r, int g = 0, int b = 0, int a = 0 )
{
    VkClearColorValue result;
    result.int32 [ 0 ] = r;
    result.int32 [ 1 ] = g;
    result.int32 [ 2 ] = b;
    result.int32 [ 3 ] = a;
    return result;
}

VPP_INLINE VkClearColorValue clearColor (
    unsigned int r, unsigned int g = 0u, unsigned int b = 0u, unsigned int a = 0u )
{
    VkClearColorValue result;
    result.uint32 [ 0 ] = r;
    result.uint32 [ 1 ] = g;
    result.uint32 [ 2 ] = b;
    result.uint32 [ 3 ] = a;
    return result;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPFORMATS_HPP
