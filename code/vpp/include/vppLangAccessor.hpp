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

#ifndef INC_VPPLANGACCESSOR_HPP
#define INC_VPPLANGACCESSOR_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGSCALARTYPES_HPP
#include "vppLangScalarTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< typename ValueT >
struct TComparisonResult
{
    typedef Bool type;
};

template< typename ScalarT >
struct TComparisonResult< TRValue< ScalarT > >
{
    typedef Bool type;
};

template< typename ScalarT >
struct TComparisonResult< TLValue< ScalarT > >
{
    typedef Bool type;
};

template< class BaseType, size_t SIZE >
struct TComparisonResult< TRVector< BaseType, SIZE > >
{
    typedef TRVector< Bool, SIZE > type;
};

template< class BaseType, size_t SIZE >
struct TComparisonResult< TLVector< BaseType, SIZE > >
{
    typedef TRVector< Bool, SIZE > type;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

#define VPP_DEFINE_ACCESSOR_OPERATOR1( OP ) \
VPP_INLINE rvalue_type operator OP () const \
{ rvalue_type rhs = *this; return OP rhs; } \

#define VPP_DEFINE_ACCESSOR_OPERATOR2( OP ) \
template< typename RightT > \
VPP_INLINE auto operator OP ( const RightT& rhs ) const \
{ rvalue_type lhs = *this; VPP_RVTYPE( RightT ) rrv = rhs; return lhs OP rrv; } \

#define VPP_DEFINE_ACCESSOR_OPERATORC( OP ) \
template< typename RightT > \
VPP_INLINE comparison_type operator OP ( const RightT& rhs ) const \
{ rvalue_type lhs = *this; VPP_RVTYPE( RightT ) rrv = rhs; return lhs OP rrv; } \

#define VPP_DEFINE_ACCESSOR_OPERATORA( OP, OP2 ) \
template< typename RightT > \
VPP_INLINE void operator OP ( const RightT& rhs ) const \
{ VPP_RVTYPE( RightT ) rrv = rhs; *this = *this OP2 rrv; } \

#define VPP_DEFINE_ACCESSOR_OPERATORS \
    VPP_DEFINE_ACCESSOR_OPERATOR2( + ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( - ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( * ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( / ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( % ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( << ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( >> ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( | ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( ^ ) \
    VPP_DEFINE_ACCESSOR_OPERATOR2( & ) \
    VPP_DEFINE_ACCESSOR_OPERATOR1( - ) \
    VPP_DEFINE_ACCESSOR_OPERATOR1( ~ ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( == ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( != ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( < ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( > ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( <= ) \
    VPP_DEFINE_ACCESSOR_OPERATORC( >= ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( +=, + ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( -=, - ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( *=, * ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( /=, / ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( %=, % ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( <<=, << ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( >>=, >> ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( |=, | ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( ^=, ^ ) \
    VPP_DEFINE_ACCESSOR_OPERATORA( &=, & ) \


// -----------------------------------------------------------------------------

enum ESwizzle1
{
    X = 0b00000000,
    Y = 0b00000001,
    Z = 0b00000010,
    W = 0b00000011
};

// -----------------------------------------------------------------------------

enum ESwizzle2
{
    XX = 0b00000000, XY = 0b00000001, XZ = 0b00000010, XW = 0b00000011,
    YX = 0b00000100, YY = 0b00000101, YZ = 0b00000110, YW = 0b00000111,
    ZX = 0b00001000, ZY = 0b00001001, ZZ = 0b00001010, ZW = 0b00001011,
    WX = 0b00001100, WY = 0b00001101, WZ = 0b00001110, WW = 0b00001111
};

// -----------------------------------------------------------------------------

enum ESwizzle3
{
    XXX = 0b00000000, XXY = 0b00000001, XXZ = 0b00000010, XXW = 0b00000011,
    XYX = 0b00000100, XYY = 0b00000101, XYZ = 0b00000110, XYW = 0b00000111,
    XZX = 0b00001000, XZY = 0b00001001, XZZ = 0b00001010, XZW = 0b00001011,
    XWX = 0b00001100, XWY = 0b00001101, XWZ = 0b00001110, XWW = 0b00001111,
    YXX = 0b00010000, YXY = 0b00010001, YXZ = 0b00010010, YXW = 0b00010011,
    YYX = 0b00010100, YYY = 0b00010101, YYZ = 0b00010110, YYW = 0b00010111,
    YZX = 0b00011000, YZY = 0b00011001, YZZ = 0b00011010, YZW = 0b00011011,
    YWX = 0b00011100, YWY = 0b00011101, YWZ = 0b00011110, YWW = 0b00011111,
    ZXX = 0b00100000, ZXY = 0b00100001, ZXZ = 0b00100010, ZXW = 0b00100011,
    ZYX = 0b00100100, ZYY = 0b00100101, ZYZ = 0b00100110, ZYW = 0b00100111,
    ZZX = 0b00101000, ZZY = 0b00101001, ZZZ = 0b00101010, ZZW = 0b00101011,
    ZWX = 0b00101100, ZWY = 0b00101101, ZWZ = 0b00101110, ZWW = 0b00101111,
    WXX = 0b00110000, WXY = 0b00110001, WXZ = 0b00110010, WXW = 0b00110011,
    WYX = 0b00110100, WYY = 0b00110101, WYZ = 0b00110110, WYW = 0b00110111,
    WZX = 0b00111000, WZY = 0b00111001, WZZ = 0b00111010, WZW = 0b00111011,
    WWX = 0b00111100, WWY = 0b00111101, WWZ = 0b00111110, WWW = 0b00111111
};

// -----------------------------------------------------------------------------

enum ESwizzle4
{
    XXXX = 0b00000000, XXXY = 0b00000001, XXXZ = 0b00000010, XXXW = 0b00000011,
    XXYX = 0b00000100, XXYY = 0b00000101, XXYZ = 0b00000110, XXYW = 0b00000111,
    XXZX = 0b00001000, XXZY = 0b00001001, XXZZ = 0b00001010, XXZW = 0b00001011,
    XXWX = 0b00001100, XXWY = 0b00001101, XXWZ = 0b00001110, XXWW = 0b00001111,
    XYXX = 0b00010000, XYXY = 0b00010001, XYXZ = 0b00010010, XYXW = 0b00010011,
    XYYX = 0b00010100, XYYY = 0b00010101, XYYZ = 0b00010110, XYYW = 0b00010111,
    XYZX = 0b00011000, XYZY = 0b00011001, XYZZ = 0b00011010, XYZW = 0b00011011,
    XYWX = 0b00011100, XYWY = 0b00011101, XYWZ = 0b00011110, XYWW = 0b00011111,
    XZXX = 0b00100000, XZXY = 0b00100001, XZXZ = 0b00100010, XZXW = 0b00100011,
    XZYX = 0b00100100, XZYY = 0b00100101, XZYZ = 0b00100110, XZYW = 0b00100111,
    XZZX = 0b00101000, XZZY = 0b00101001, XZZZ = 0b00101010, XZZW = 0b00101011,
    XZWX = 0b00101100, XZWY = 0b00101101, XZWZ = 0b00101110, XZWW = 0b00101111,
    XWXX = 0b00110000, XWXY = 0b00110001, XWXZ = 0b00110010, XWXW = 0b00110011,
    XWYX = 0b00110100, XWYY = 0b00110101, XWYZ = 0b00110110, XWYW = 0b00110111,
    XWZX = 0b00111000, XWZY = 0b00111001, XWZZ = 0b00111010, XWZW = 0b00111011,
    XWWX = 0b00111100, XWWY = 0b00111101, XWWZ = 0b00111110, XWWW = 0b00111111,
    YXXX = 0b01000000, YXXY = 0b01000001, YXXZ = 0b01000010, YXXW = 0b01000011,
    YXYX = 0b01000100, YXYY = 0b01000101, YXYZ = 0b01000110, YXYW = 0b01000111,
    YXZX = 0b01001000, YXZY = 0b01001001, YXZZ = 0b01001010, YXZW = 0b01001011,
    YXWX = 0b01001100, YXWY = 0b01001101, YXWZ = 0b01001110, YXWW = 0b01001111,
    YYXX = 0b01010000, YYXY = 0b01010001, YYXZ = 0b01010010, YYXW = 0b01010011,
    YYYX = 0b01010100, YYYY = 0b01010101, YYYZ = 0b01010110, YYYW = 0b01010111,
    YYZX = 0b01011000, YYZY = 0b01011001, YYZZ = 0b01011010, YYZW = 0b01011011,
    YYWX = 0b01011100, YYWY = 0b01011101, YYWZ = 0b01011110, YYWW = 0b01011111,
    YZXX = 0b01100000, YZXY = 0b01100001, YZXZ = 0b01100010, YZXW = 0b01100011,
    YZYX = 0b01100100, YZYY = 0b01100101, YZYZ = 0b01100110, YZYW = 0b01100111,
    YZZX = 0b01101000, YZZY = 0b01101001, YZZZ = 0b01101010, YZZW = 0b01101011,
    YZWX = 0b01101100, YZWY = 0b01101101, YZWZ = 0b01101110, YZWW = 0b01101111,
    YWXX = 0b01110000, YWXY = 0b01110001, YWXZ = 0b01110010, YWXW = 0b01110011,
    YWYX = 0b01110100, YWYY = 0b01110101, YWYZ = 0b01110110, YWYW = 0b01110111,
    YWZX = 0b01111000, YWZY = 0b01111001, YWZZ = 0b01111010, YWZW = 0b01111011,
    YWWX = 0b01111100, YWWY = 0b01111101, YWWZ = 0b01111110, YWWW = 0b01111111,
    ZXXX = 0b10000000, ZXXY = 0b10000001, ZXXZ = 0b10000010, ZXXW = 0b10000011,
    ZXYX = 0b10000100, ZXYY = 0b10000101, ZXYZ = 0b10000110, ZXYW = 0b10000111,
    ZXZX = 0b10001000, ZXZY = 0b10001001, ZXZZ = 0b10001010, ZXZW = 0b10001011,
    ZXWX = 0b10001100, ZXWY = 0b10001101, ZXWZ = 0b10001110, ZXWW = 0b10001111,
    ZYXX = 0b10010000, ZYXY = 0b10010001, ZYXZ = 0b10010010, ZYXW = 0b10010011,
    ZYYX = 0b10010100, ZYYY = 0b10010101, ZYYZ = 0b10010110, ZYYW = 0b10010111,
    ZYZX = 0b10011000, ZYZY = 0b10011001, ZYZZ = 0b10011010, ZYZW = 0b10011011,
    ZYWX = 0b10011100, ZYWY = 0b10011101, ZYWZ = 0b10011110, ZYWW = 0b10011111,
    ZZXX = 0b10100000, ZZXY = 0b10100001, ZZXZ = 0b10100010, ZZXW = 0b10100011,
    ZZYX = 0b10100100, ZZYY = 0b10100101, ZZYZ = 0b10100110, ZZYW = 0b10100111,
    ZZZX = 0b10101000, ZZZY = 0b10101001, ZZZZ = 0b10101010, ZZZW = 0b10101011,
    ZZWX = 0b10101100, ZZWY = 0b10101101, ZZWZ = 0b10101110, ZZWW = 0b10101111,
    ZWXX = 0b10110000, ZWXY = 0b10110001, ZWXZ = 0b10110010, ZWXW = 0b10110011,
    ZWYX = 0b10110100, ZWYY = 0b10110101, ZWYZ = 0b10110110, ZWYW = 0b10110111,
    ZWZX = 0b10111000, ZWZY = 0b10111001, ZWZZ = 0b10111010, ZWZW = 0b10111011,
    ZWWX = 0b10111100, ZWWY = 0b10111101, ZWWZ = 0b10111110, ZWWW = 0b10111111,
    WXXX = 0b11000000, WXXY = 0b11000001, WXXZ = 0b11000010, WXXW = 0b11000011,
    WXYX = 0b11000100, WXYY = 0b11000101, WXYZ = 0b11000110, WXYW = 0b11000111,
    WXZX = 0b11001000, WXZY = 0b11001001, WXZZ = 0b11001010, WXZW = 0b11001011,
    WXWX = 0b11001100, WXWY = 0b11001101, WXWZ = 0b11001110, WXWW = 0b11001111,
    WYXX = 0b11010000, WYXY = 0b11010001, WYXZ = 0b11010010, WYXW = 0b11010011,
    WYYX = 0b11010100, WYYY = 0b11010101, WYYZ = 0b11010110, WYYW = 0b11010111,
    WYZX = 0b11011000, WYZY = 0b11011001, WYZZ = 0b11011010, WYZW = 0b11011011,
    WYWX = 0b11011100, WYWY = 0b11011101, WYWZ = 0b11011110, WYWW = 0b11011111,
    WZXX = 0b11100000, WZXY = 0b11100001, WZXZ = 0b11100010, WZXW = 0b11100011,
    WZYX = 0b11100100, WZYY = 0b11100101, WZYZ = 0b11100110, WZYW = 0b11100111,
    WZZX = 0b11101000, WZZY = 0b11101001, WZZZ = 0b11101010, WZZW = 0b11101011,
    WZWX = 0b11101100, WZWY = 0b11101101, WZWZ = 0b11101110, WZWW = 0b11101111,
    WWXX = 0b11110000, WWXY = 0b11110001, WWXZ = 0b11110010, WWXW = 0b11110011,
    WWYX = 0b11110100, WWYY = 0b11110101, WWYZ = 0b11110110, WWYW = 0b11110111,
    WWZX = 0b11111000, WWZY = 0b11111001, WWZZ = 0b11111010, WWZW = 0b11111011,
    WWWX = 0b11111100, WWWY = 0b11111101, WWWZ = 0b11111110, WWWW = 0b11111111
};

// -----------------------------------------------------------------------------

template<
    class ValueType, size_t SIZE,
    bool ENABLE_READ = true, bool ENABLE_WRITE = true >
class Swizzle
{
public:
    typedef typename ValueType::rvalue_type rvalue_type;
    typedef typename rvalue_type::item_type item_type;
    typedef typename detail::TComparisonResult< ValueType >::type comparison_type;

    VPP_INLINE Swizzle (
        const spv::Builder::AccessChain& ac,
        KId preSwizzleType ) :
            d_accessChain ( ac ),
            d_preSwizzleType ( preSwizzleType )
    {
    }

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( ENABLE_READ, "This variable is write-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );

        return rvalue_type ( KId (
            pTranslator->accessChainLoad (
                pTranslator->getPrecision(), rvalue_type::getType() ) ) );
    }

    VPP_INLINE const Swizzle& operator= ( const Swizzle& rhs )
    {
        static_assert ( ENABLE_WRITE, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    template< typename RightT >
    VPP_INLINE const Swizzle& operator= ( const RightT& rhs )
    {
        static_assert ( ENABLE_WRITE, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    VPP_INLINE Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE >
        operator[] ( int index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 1 );
        swi [ 0 ] = ( index & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, d_preSwizzleType );

        return Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE > (
            pTranslator->getAccessChain(), d_preSwizzleType );
    }

    VPP_INLINE Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE >
        operator[] ( ESwizzle1 sw ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 1 );
        swi [ 0 ] = ( sw & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, d_preSwizzleType );

        return Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE > (
            pTranslator->getAccessChain(), d_preSwizzleType );
    }

    VPP_INLINE Swizzle< typename rvalue_type::swizzle2_type, 2, ENABLE_READ, ENABLE_WRITE >
        operator[] ( ESwizzle2 sw ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 2 );
        swi [ 1 ] = ( sw & 3 );
        swi [ 0 ] = ( ( sw >> 2 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, d_preSwizzleType );

        return Swizzle< typename rvalue_type::swizzle2_type, 2, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), d_preSwizzleType );
    }

    VPP_INLINE Swizzle< typename rvalue_type::swizzle3_type, 3, ENABLE_READ, ENABLE_WRITE >
        operator[] ( ESwizzle3 sw ) const
    {
        static_assert ( SIZE >= 3, "Too long swizzle applied to the vector" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 3 );
        swi [ 2 ] = ( sw & 3 );
        swi [ 1 ] = ( ( sw >> 2 ) & 3 );
        swi [ 0 ] = ( ( sw >> 4 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, d_preSwizzleType );

        return Swizzle< typename rvalue_type::swizzle3_type, 3, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), d_preSwizzleType );
    }

    VPP_INLINE Swizzle< typename rvalue_type::swizzle4_type, 4, ENABLE_READ, ENABLE_WRITE >
        operator[] ( ESwizzle4 sw ) const
    {
        static_assert ( SIZE >= 4, "Too long swizzle applied to the vector" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 4 );
        swi [ 3 ] = ( sw & 3 );
        swi [ 2 ] = ( ( sw >> 2 ) & 3 );
        swi [ 1 ] = ( ( sw >> 4 ) & 3 );
        swi [ 0 ] = ( ( sw >> 6 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, d_preSwizzleType );

        return Swizzle< typename rvalue_type::swizzle4_type, 4, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), d_preSwizzleType );
    }

    VPP_DEFINE_ACCESSOR_OPERATORS;

private:
    spv::Builder::AccessChain d_accessChain;
    KId d_preSwizzleType;
};

// -----------------------------------------------------------------------------

template< class ValueType, bool ENABLE_READ, bool ENABLE_WRITE >
class Swizzle< ValueType, 1, ENABLE_READ, ENABLE_WRITE >
{
public:
    typedef ValueType rvalue_type;
    typedef typename detail::TComparisonResult< ValueType >::type comparison_type;

    VPP_INLINE Swizzle (
        const spv::Builder::AccessChain& ac,
        KId preSwizzleType ) :
            d_accessChain ( ac ),
            d_preSwizzleType ( preSwizzleType )
    {
    }

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( ENABLE_READ, "This variable is write-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );

        return rvalue_type ( KId (
            pTranslator->accessChainLoad (
                pTranslator->getPrecision(), rvalue_type::getType() ) ) );
    }

    VPP_INLINE const Swizzle& operator= ( const Swizzle& rhs )
    {
        static_assert ( ENABLE_WRITE, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    template< typename RightT >
    VPP_INLINE const Swizzle& operator= ( const RightT& rhs )
    {
        static_assert ( ENABLE_WRITE, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    VPP_DEFINE_ACCESSOR_OPERATORS;

private:
    spv::Builder::AccessChain d_accessChain;
    KId d_preSwizzleType;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ValueT, bool ENABLE_READ = true, bool ENABLE_WRITE = true >
class Accessor
{
public:
    typedef typename ValueT::rvalue_type rvalue_type;
    typedef typename rvalue_type::item_type item_type;
    typedef typename detail::TComparisonResult< ValueT >::type comparison_type;
    typedef Accessor< item_type, ENABLE_READ, ENABLE_WRITE > return_type;

    VPP_INLINE Accessor ( const spv::Builder::AccessChain& ac ):
        d_accessChain ( ac )
    {}

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( ENABLE_READ, "Attempt to read a write-only object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );

        return rvalue_type ( KId (
            pTranslator->accessChainLoad (
                pTranslator->getPrecision(), rvalue_type::getType() ) ) );
    }

    VPP_INLINE const Accessor& operator= ( const Accessor& rhs ) const
    {
        static_assert ( ENABLE_WRITE, "Attempt to write to a read-only object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    template< typename RightT >
    VPP_INLINE const Accessor& operator= ( const RightT& rhs ) const
    {
        static_assert ( ENABLE_WRITE, "Attempt to write to a read-only object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index ) const
    {
        static_assert ( rvalue_type::indexable, "Attempt to index a scalar object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return return_type ( pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) const { return getItem ( index ); }

    VPP_DEFINE_ACCESSOR_OPERATORS;

    VPP_INLINE rvalue_type operator++()
    {
        *this = *this + rvalue_type ( 1 );
        const rvalue_type rt = *this;
        return rt;
    }

    VPP_INLINE rvalue_type operator--()
    {
        *this = *this - rvalue_type ( 1 );
        const rvalue_type rt = *this;
        return rt;
    }

    VPP_INLINE Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE >
        operator[] ( ESwizzle1 sw ) const
    {
        static_assert ( rvalue_type::indexable, "Attempt to index a scalar object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 1 );
        swi [ 0 ] = ( sw & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, rvalue_type::getType() );

        return Swizzle< item_type, 1, ENABLE_READ, ENABLE_WRITE > (
            pTranslator->getAccessChain(), rvalue_type::getType() );
    }

    VPP_INLINE auto operator[] ( ESwizzle2 sw ) const
    {
        static_assert ( rvalue_type::indexable, "Attempt to index a scalar object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 2 );
        swi [ 1 ] = ( sw & 3 );
        swi [ 0 ] = ( ( sw >> 2 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, rvalue_type::getType() );

        return Swizzle< typename rvalue_type::swizzle2_type, 2, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), rvalue_type::getType() );
    }

    VPP_INLINE auto operator[] ( ESwizzle3 sw ) const
    {
        static_assert ( rvalue_type::indexable, "Attempt to index a scalar object" );
        static_assert ( rvalue_type::item_count >= 3, "Too long swizzle applied to the vector" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 3 );
        swi [ 2 ] = ( sw & 3 );
        swi [ 1 ] = ( ( sw >> 2 ) & 3 );
        swi [ 0 ] = ( ( sw >> 4 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, rvalue_type::getType() );

        return Swizzle< typename rvalue_type::swizzle3_type, 3, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), rvalue_type::getType() );
    }

    VPP_INLINE auto operator[] ( ESwizzle4 sw ) const
    {
        static_assert ( rvalue_type::indexable, "Attempt to index a scalar object" );
        static_assert ( rvalue_type::item_count >= 4, "Too long swizzle applied to the vector" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< unsigned int > swi ( 4 );
        swi [ 3 ] = ( sw & 3 );
        swi [ 2 ] = ( ( sw >> 2 ) & 3 );
        swi [ 1 ] = ( ( sw >> 4 ) & 3 );
        swi [ 0 ] = ( ( sw >> 6 ) & 3 );

        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPushSwizzle ( swi, rvalue_type::getType() );

        return Swizzle< typename rvalue_type::swizzle4_type, 4, ENABLE_READ, ENABLE_WRITE >(
            pTranslator->getAccessChain(), rvalue_type::getType() );
    }

    VPP_INLINE KId getPointerId() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        return KId ( pTranslator->accessChainGetLValue() );
    }

    VPP_INLINE Pointer< typename ValueT::rvalue_type > operator& () const
    {
        return Pointer< typename ValueT::rvalue_type >(
            getPointerId(), spv::StorageClassWorkgroup );
    }

private:
    spv::Builder::AccessChain d_accessChain;
};

// -----------------------------------------------------------------------------

template< typename ScalarT, class ValueT, bool ENABLE_READ, bool ENABLE_WRITE >
VPP_INLINE auto operator* (
    const TRValue< ScalarT >& lhs, 
    const Accessor< ValueT, ENABLE_READ, ENABLE_WRITE >& rhs )
{
    typedef Accessor< ValueT, ENABLE_READ, ENABLE_WRITE > source_type;
    VPP_RVTYPE( source_type ) rrv = rhs;
    return lhs * rrv;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class StructT, bool ENABLE_READ = true, bool ENABLE_WRITE = true >
class StructAccessor
{
public:
    VPP_INLINE StructAccessor (
        StructT* pStruct,
        const spv::Builder::AccessChain& ac ):
            d_pStruct ( pStruct ),
            d_accessChain ( ac )
    {}

    template< typename MemberT >
    VPP_INLINE Accessor< typename MemberT::rvalue_type, ENABLE_READ, ENABLE_WRITE >
        operator[]( MemberT StructT::* pMember ) const
    {
        typedef typename MemberT::rvalue_type rvalue_type;
        typedef Accessor< rvalue_type, ENABLE_READ, ENABLE_WRITE > result_type;

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );

        const MemberT& member = d_pStruct->*pMember;
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( member.index() ) );

        return result_type ( pTranslator->getAccessChain() );
    }

private:
    StructT* d_pStruct;
    spv::Builder::AccessChain d_accessChain;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGACCESSOR_HPP
