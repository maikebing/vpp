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

#ifndef INC_VPPLANGVECTORTYPES_HPP
#define INC_VPPLANGVECTORTYPES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGACCESSOR_HPP
#include "vppLangAccessor.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< class BaseType, size_t COLS, size_t ROWS >
class TRMatrix;

template< class BaseType, size_t COLS, size_t ROWS, spv::StorageClass SCL >
class TLMatrix;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define VPP_DEFINE_VECTOR_OPERATOR1( OP ) \
VPP_INLINE rvalue_type operator OP () const \
{ rvalue_type rhs = *this; return OP rhs; } \

#define VPP_DEFINE_VECTOR_OPERATOR2( OP ) \
template< typename RightT > \
VPP_INLINE rvalue_type operator OP ( const RightT& rhs ) const \
{ rvalue_type lrv = *this; VPP_RVTYPE( RightT ) rrv = rhs; return lrv OP rrv; } \

#define VPP_DEFINE_VECTOR_OPERATORC( OP ) \
template< typename RightT > \
VPP_INLINE comparison_type operator OP ( const RightT& rhs ) const \
{ rvalue_type lrv = *this; VPP_RVTYPE( RightT ) rrv = rhs; return lrv OP rrv; } \

#define VPP_DEFINE_VECTOR_OPERATORS1 \
    VPP_DEFINE_VECTOR_OPERATOR1( - ) \
    VPP_DEFINE_VECTOR_OPERATOR1( ~ ) \
    VPP_DEFINE_VECTOR_OPERATOR1( ! ) \

#define VPP_DEFINE_VECTOR_OPERATORS2 \
    VPP_DEFINE_VECTOR_OPERATOR2( + ) \
    VPP_DEFINE_VECTOR_OPERATOR2( - ) \
    VPP_DEFINE_VECTOR_OPERATOR2( / ) \
    VPP_DEFINE_VECTOR_OPERATOR2( % ) \
    VPP_DEFINE_VECTOR_OPERATOR2( << ) \
    VPP_DEFINE_VECTOR_OPERATOR2( >> ) \
    VPP_DEFINE_VECTOR_OPERATOR2( | ) \
    VPP_DEFINE_VECTOR_OPERATOR2( ^ ) \
    VPP_DEFINE_VECTOR_OPERATOR2( & ) \
    VPP_DEFINE_VECTOR_OPERATOR2( || ) \
    VPP_DEFINE_VECTOR_OPERATOR2( && ) \
    VPP_DEFINE_VECTOR_OPERATORC( == ) \
    VPP_DEFINE_VECTOR_OPERATORC( != ) \
    VPP_DEFINE_VECTOR_OPERATORC( < ) \
    VPP_DEFINE_VECTOR_OPERATORC( > ) \
    VPP_DEFINE_VECTOR_OPERATORC( <= ) \
    VPP_DEFINE_VECTOR_OPERATORC( >= ) \

#define VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( OPM, OP ) \
template< typename RightT > \
VPP_INLINE rvalue_type operator OPM ( const RightT& rhs ) \
{ rvalue_type lhs = *this; *this = lhs OP rhs; return *this; } \

#define VPP_DEFINE_MUTATING_VECTOR_OPERATORS \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( +=, + ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( -=, - ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( *=, * ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( /=, / ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( %=, % ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( <<=, << ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( >>=, >> ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( &=, & ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( |=, | ) \
    VPP_DEFINE_MUTATING_VECTOR_OPERATOR2( ^=, ^ ) \

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename T1, typename T2 >
struct TVectorInitCompatible { enum { VALUE = 0 }; };

template< typename T1 >
struct TVectorInitCompatible< T1, KValue > { enum { VALUE = 1 }; };

template< typename T >
struct TVectorInitCompatible< T, T > { enum { VALUE = 1 }; };

// -----------------------------------------------------------------------------

template< class BaseType, size_t SIZE >
class TRVector : public KValue
{
public:
    typedef BaseType base_type;
    typedef typename BaseType::scalar_type scalar_type;
    typedef BaseType item_type;
    typedef TRVector< BaseType, SIZE > rvalue_type;
    typedef TRVector< Bool, SIZE > comparison_type;
    typedef TRVector< BaseType, 4 > swizzle4_type;
    typedef TRVector< BaseType, 3 > swizzle3_type;
    typedef TRVector< BaseType, 2 > swizzle2_type;
    typedef BaseType swizzle1_type;

    static const bool external_linkage = BaseType::external_linkage;
    static const bool indexable = true;
    static const bool is_variable = false;
    static const bool is_pointer = false;
    static const bool is_shared = false;
    static const bool is_64bit = BaseType::is_64bit;
    static const size_t dimensions = 1;
    static const size_t component_count = SIZE * BaseType::component_count;
    static const size_t location_count = ( component_count + 3 ) >> 2;
    static const size_t item_count = SIZE;

    static VPP_INLINE KId getType()
    {
        return KId (
            KShaderTranslator::get()->makeVectorType (
                BaseType::getType(), static_cast< int >( SIZE ) ) );
    }

    static VPP_INLINE KId createConstant ( const scalar_type& init )
    {
        const KId scalarConstant = KId (
            scalar_traits< scalar_type >::makeConstant ( init ) );

        return KShaderTranslator::get()->expandConstant (
            scalarConstant, static_cast< int >( SIZE ) );
    }

    static VPP_INLINE KId createConstant ( const std::initializer_list< scalar_type >& init )
    {
        std::vector< spv::Id > valueIds ( init.size() );

        std::transform (
            init.begin(), init.end(), valueIds.begin(),
            []( const scalar_type& src ) { return scalar_traits< scalar_type >::makeConstant ( src ); }
        );

        if ( valueIds.size() < SIZE )
            valueIds.resize ( SIZE, scalar_traits< scalar_type >::makeConstant ( scalar_type() ) );
        else if ( valueIds.size() > SIZE )
            valueIds.resize ( SIZE );
        
        return KId ( KShaderTranslator::get()->makeCompositeConstant ( getType(), valueIds ) );
    }

    template< class Arg1T, class Arg2T = KValue, class Arg3T = KValue, class Arg4T = KValue >
    static VPP_INLINE KId constructFromVariables (
        const Arg1T& arg1,
        const Arg2T& arg2 = KValue ( KId ( 0 ) ),
        const Arg3T& arg3 = KValue ( KId ( 0 ) ),
        const Arg4T& arg4 = KValue ( KId ( 0 ) ) )
    {
        std::vector< spv::Id > sourceIds;
        sourceIds.reserve ( 4 );

        typedef typename detail::TGetRV< Arg1T >::type Arg1RT;
        typedef typename detail::TGetRV< Arg2T >::type Arg2RT;
        typedef typename detail::TGetRV< Arg3T >::type Arg3RT;
        typedef typename detail::TGetRV< Arg4T >::type Arg4RT;

        typedef typename Arg1RT::scalar_type Arg1ST;
        typedef typename Arg2RT::scalar_type Arg2ST;
        typedef typename Arg3RT::scalar_type Arg3ST;
        typedef typename Arg4RT::scalar_type Arg4ST;

        Arg1RT rArg1 = arg1;
        Arg2RT rArg2 = arg2;
        Arg3RT rArg3 = arg3;
        Arg4RT rArg4 = arg4;

        static const size_t itemCount =
            Arg1RT::item_count + Arg2RT::item_count + Arg3RT::item_count + Arg4RT::item_count;

        sourceIds.push_back ( rArg1.id() );

        if ( rArg2.id() )
            sourceIds.push_back ( rArg2.id() );

        if ( rArg3.id() )
            sourceIds.push_back ( rArg3.id() );

        if ( rArg4.id() )
            sourceIds.push_back ( rArg4.id() );

        if ( itemCount < SIZE )
        {
            const KId paddingConstant = KId (
                scalar_traits< scalar_type >::makeConstant ( Arg1ST() ) );

            for ( size_t i = itemCount; i < SIZE; ++i )
                sourceIds.push_back ( paddingConstant );
        }

        static_assert (
            TVectorInitCompatible< scalar_type, Arg1ST >::VALUE
            && TVectorInitCompatible< scalar_type, Arg2ST >::VALUE
            && TVectorInitCompatible< scalar_type, Arg3ST >::VALUE
            && TVectorInitCompatible< scalar_type, Arg4ST >::VALUE,
            "Argument types do not match constructed vector (use StaticCast<...> to cast)." );

        return KId ( KShaderTranslator::get()->createConstructor (
            KShaderTranslator::get()->getPrecision(), sourceIds, getType() ) );
    }

    VPP_INLINE TRVector() :
        KValue ( createConstant ( scalar_type() ) )
    {}

    VPP_INLINE TRVector ( KId id ) :
        KValue ( id )
    {}

    VPP_INLINE TRVector ( const std::initializer_list< scalar_type >& init ) :
        KValue ( createConstant ( init ) )
    {}

    VPP_INLINE TRVector ( const scalar_type& init ) :
        KValue ( createConstant ( init ) )
    {}

    VPP_INLINE TRVector ( const TRVector& rhs ) :
        KValue ( rhs.id() )
    {
    }

    template< class Arg1T >
    VPP_INLINE explicit TRVector ( const Arg1T& arg1 ) :
        KValue ( constructFromVariables ( arg1 ) )
    {}

    template< class Arg1T, class Arg2T >
    VPP_INLINE TRVector ( const Arg1T& arg1, const Arg2T& arg2 ) :
        KValue ( constructFromVariables ( arg1, arg2 ) )
    {}

    template< class Arg1T, class Arg2T, class Arg3T >
    VPP_INLINE TRVector ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 ) :
        KValue ( constructFromVariables ( arg1, arg2, arg3 ) )
    {}

    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    VPP_INLINE TRVector ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 ) :
        KValue ( constructFromVariables ( arg1, arg2, arg3, arg4 ) )
    {}

    VPP_INLINE TRVector operator+ ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator + does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opAdd, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator- ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator - does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opSub, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator* ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator * does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opMul, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator/ ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator / does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opDiv, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator% ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator % does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opMod, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator<< ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator << does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opShl, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator>> ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator >> does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opShr, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator| ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator | does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opBitOr, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator^ ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator ^ does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opBitXor, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator& ( const TRVector& rhs ) const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator & does not support BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            scalar_traits< scalar_type >::opBitAnd, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator&& ( const TRVector& rhs ) const
    {
        static_assert (
            scalar_traits< scalar_type >::isBool,
            "Operator && supports only VBool and BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            spv::OpLogicalAnd, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator|| ( const TRVector& rhs ) const
    {
        static_assert (
            scalar_traits< scalar_type >::isBool,
            "Operator || supports only VBool and BVec types" );

        return TOperators< TRVector >::makeBinaryOp (
            spv::OpLogicalOr, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator-() const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator - does not support BVec types" );

        return TOperators< TRVector >::makeUnaryOp (
            scalar_traits< scalar_type >::opNeg, id() );
    }

    VPP_INLINE TRVector operator~() const
    {
        static_assert (
            ! scalar_traits< scalar_type >::isBool,
            "Operator ~ does not support BVec types" );

        return TOperators< TRVector >::makeUnaryOp (
            scalar_traits< scalar_type >::opBitNot, id() );
    }

    VPP_INLINE TRVector operator!() const
    {
        static_assert (
            scalar_traits< scalar_type >::isBool,
            "Operator ! supports only VBool and BVec types" );

        return TOperators< TRVector >::makeUnaryOp (
            spv::OpLogicalNot, id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator== ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opEqual, id(), rhs.id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator!= ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opNotEqual, id(), rhs.id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator> ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opGtr, id(), rhs.id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator>= ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opGtrEq, id(), rhs.id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator< ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opLss, id(), rhs.id() );
    }

    VPP_INLINE TRVector< Bool, SIZE > operator<= ( const TRVector& rhs ) const
    {
        return TOperators< TRVector< Bool, SIZE > >::makeBinaryOp (
            scalar_traits< scalar_type >::opLssEq, id(), rhs.id() );
    }

    VPP_INLINE TRVector operator* ( const BaseType& rhs ) const
    {
        return TRVector ( KId (
            KShaderTranslator::get()->createBinOp (
                spv::OpVectorTimesScalar, getType(), id(), rhs.id() ) ) );
    }

    VPP_INLINE TRVector operator* ( const scalar_type& rhs ) const
    {
        const BaseType rrv = rhs;
        return *this * rrv;
    }

    VPP_INLINE TRVector operator/ ( const BaseType& rhs ) const
    {
        const BaseType one = scalar_type ( 1 );
        const BaseType inv = one / rhs;
        return operator* ( inv );
    }

    template< size_t RCOLS >
    VPP_INLINE TRVector< BaseType, RCOLS > operator* ( const TRMatrix< BaseType, RCOLS, SIZE >& rhs ) const
    {
        return rhs.prependMultiply ( *this );
    }

    VPP_DEFINE_VECTOR_OPERATORS2;

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainRValue ( id() );

        Accessor< TRVector, true, false > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

private:
    const TRVector< BaseType, SIZE >& operator= ( const TRVector< BaseType, SIZE >& ) = delete;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename BaseType, size_t SIZE, spv::StorageClass SCL >
class TLVector : public KValue
{
public:
    typedef TRVector< BaseType, SIZE > rvalue_type;
    typedef TRVector< Bool, SIZE > comparison_type;
    typedef typename BaseType::scalar_type scalar_type;
    static const size_t item_count = SIZE;
    static const bool is_variable = true;
    static const bool is_pointer = false;
    static const bool is_shared = false;
    static const size_t dimensions = 1;
    
    VPP_INLINE TLVector() :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
    }

    VPP_INLINE TLVector ( const TRVector< BaseType, SIZE >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
        KShaderTranslator::get()->createStore ( rhs.id(), id() );
    }

    VPP_INLINE TLVector ( const TLVector< BaseType, SIZE, SCL >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
    }

    VPP_INLINE TLVector ( TLVector< BaseType, SIZE, SCL >&& rhs ) :
        KValue ( rhs.id() ),
        d_bRelease ( rhs.d_bRelease )
    {
        rhs.d_bRelease = false;
    }

    template< spv::StorageClass SCL2 >
    VPP_INLINE TLVector ( const TLVector< BaseType, SIZE, SCL2 >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ) :
        d_bRelease ( true )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
    }

    static VPP_INLINE KId createVariable ( const std::initializer_list< scalar_type >& init )
    {
        const rvalue_type rValue = init;
        const KId id = KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count );
        KShaderTranslator::get()->createStore ( rValue.id(), id );
        return id;
    }

    VPP_INLINE TLVector ( const std::initializer_list< scalar_type >& initValue ) :
        KValue ( createVariable ( initValue ) ),
        d_bRelease ( true )
    {
    }

    template< class Arg1T, class Arg2T >
    VPP_INLINE TLVector ( const Arg1T& arg1, const Arg2T& arg2 ) :
        TLVector ( rvalue_type ( arg1, arg2 ) )
    {}

    template< class Arg1T, class Arg2T, class Arg3T >
    VPP_INLINE TLVector ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 ) :
        TLVector ( rvalue_type ( arg1, arg2, arg3 ) )
    {}

    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    VPP_INLINE TLVector ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 ) :
        TLVector ( rvalue_type ( arg1, arg2, arg3, arg4 ) )
    {}

    VPP_INLINE ~TLVector()
    {
        if ( d_bRelease )
            KShaderTranslator::get()->releaseCachedLocalVariable ( id(), getType(), SCL );
    }

    template< typename RightT >
    VPP_INLINE const TLVector< BaseType, SIZE, SCL >& operator= ( const RightT& rhs )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
        return *this;
    }

    VPP_INLINE const TLVector< BaseType, SIZE, SCL >& operator= ( const TLVector< BaseType, SIZE, SCL >& rhs )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
        return *this;
    }

    VPP_INLINE operator rvalue_type() const
    {
        return rvalue_type ( KId ( KShaderTranslator::get()->createLoad ( id() ) ) );
    }

    VPP_DEFINE_VECTOR_OPERATORS1;
    VPP_DEFINE_VECTOR_OPERATORS2;
    VPP_DEFINE_MUTATING_VECTOR_OPERATORS;

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        Accessor< TLVector, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    static VPP_INLINE KId getType()
    {
        return rvalue_type::getType();
    }

private:
    bool d_bRelease;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename BaseT, size_t SIZE >
VPP_INLINE TRVector< BaseT, SIZE > operator* (
    const BaseT& s, const TRVector< BaseT, SIZE >& v )
{
    return v * s;
}

// -----------------------------------------------------------------------------

template< typename ScalarT, size_t SIZE, spv::StorageClass SCL, typename RightT >
VPP_INLINE auto operator* ( const TLVector< ScalarT, SIZE, SCL >& lhs, const RightT& rhs )
{
    const TRVector< ScalarT, SIZE > lrv = lhs;
    const VPP_RVTYPE( RightT ) rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------

template< typename ScalarT, size_t SIZE, spv::StorageClass SCL, typename LeftT >
VPP_INLINE auto operator* ( const LeftT& lhs, const TLVector< ScalarT, SIZE, SCL >& rhs )
{
    const VPP_RVTYPE( LeftT ) lrv = lhs;
    const TRVector< ScalarT, SIZE > rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------

template< typename ScalarT, size_t SIZE, spv::StorageClass SCL1, spv::StorageClass SCL2, typename LeftT >
VPP_INLINE auto operator* ( const TLVector< ScalarT, SIZE, SCL1 >& lhs, const TLVector< ScalarT, SIZE, SCL2 >& rhs )
{
    const TRVector< ScalarT, SIZE > lrv = lhs;
    const TRVector< ScalarT, SIZE > rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRVector< Float, 2 > Vec2;
typedef TLVector< Float, 2, spv::StorageClassFunction > VVec2;
typedef TLVector< Float, 2, spv::StorageClassWorkgroup > WVec2;
typedef TRVector< Float, 3 > Vec3;
typedef TLVector< Float, 3, spv::StorageClassFunction > VVec3;
typedef TLVector< Float, 3, spv::StorageClassWorkgroup > WVec3;
typedef TRVector< Float, 4 > Vec4;
typedef TLVector< Float, 4, spv::StorageClassFunction > VVec4;
typedef TLVector< Float, 4, spv::StorageClassWorkgroup > WVec4;

typedef TRVector< Double, 2 > DVec2;
typedef TLVector< Double, 2, spv::StorageClassFunction > VDVec2;
typedef TLVector< Double, 2, spv::StorageClassWorkgroup > WDVec2;
typedef TRVector< Double, 3 > DVec3;
typedef TLVector< Double, 3, spv::StorageClassFunction > VDVec3;
typedef TLVector< Double, 3, spv::StorageClassWorkgroup > WDVec3;
typedef TRVector< Double, 4 > DVec4;
typedef TLVector< Double, 4, spv::StorageClassFunction > VDVec4;
typedef TLVector< Double, 4, spv::StorageClassWorkgroup > WDVec4;

typedef TRVector< Half, 2 > HVec2;
typedef TLVector< Half, 2, spv::StorageClassFunction > VHVec2;
typedef TLVector< Half, 2, spv::StorageClassWorkgroup > WHVec2;
typedef TRVector< Half, 3 > HVec3;
typedef TLVector< Half, 3, spv::StorageClassFunction > VHVec3;
typedef TLVector< Half, 3, spv::StorageClassWorkgroup > WHVec3;
typedef TRVector< Half, 4 > HVec4;
typedef TLVector< Half, 4, spv::StorageClassFunction > VHVec4;
typedef TLVector< Half, 4, spv::StorageClassWorkgroup > WHVec4;

typedef TRVector< Int, 2 > IVec2;
typedef TLVector< Int, 2, spv::StorageClassFunction > VIVec2;
typedef TLVector< Int, 2, spv::StorageClassWorkgroup > WIVec2;
typedef TRVector< Int, 3 > IVec3;
typedef TLVector< Int, 3, spv::StorageClassFunction > VIVec3;
typedef TLVector< Int, 3, spv::StorageClassWorkgroup > WIVec3;
typedef TRVector< Int, 4 > IVec4;
typedef TLVector< Int, 4, spv::StorageClassFunction > VIVec4;
typedef TLVector< Int, 4, spv::StorageClassWorkgroup > WIVec4;

typedef TRVector< UInt, 2 > UVec2;
typedef TLVector< UInt, 2, spv::StorageClassFunction > VUVec2;
typedef TLVector< UInt, 2, spv::StorageClassWorkgroup > WUVec2;
typedef TRVector< UInt, 3 > UVec3;
typedef TLVector< UInt, 3, spv::StorageClassFunction > VUVec3;
typedef TLVector< UInt, 3, spv::StorageClassWorkgroup > WUVec3;
typedef TRVector< UInt, 4 > UVec4;
typedef TLVector< UInt, 4, spv::StorageClassFunction > VUVec4;
typedef TLVector< UInt, 4, spv::StorageClassWorkgroup > WUVec4;

typedef TRVector< Bool, 2 > BVec2;
typedef TLVector< Bool, 2, spv::StorageClassFunction > VBVec2;
typedef TLVector< Bool, 2, spv::StorageClassWorkgroup > WBVec2;
typedef TRVector< Bool, 3 > BVec3;
typedef TLVector< Bool, 3, spv::StorageClassFunction > VBVec3;
typedef TLVector< Bool, 3, spv::StorageClassWorkgroup > WBVec3;
typedef TRVector< Bool, 4 > BVec4;
typedef TLVector< Bool, 4, spv::StorageClassFunction > VBVec4;
typedef TLVector< Bool, 4, spv::StorageClassWorkgroup > WBVec4;

// -----------------------------------------------------------------------------
// PHVec types (half-float limited vectors).
// -----------------------------------------------------------------------------

class PHVec4 : public KValue
{
public:
    typedef Int base_type;
    typedef Float::scalar_type scalar_type;
    typedef Int item_type;
    typedef PHVec4 rvalue_type;

    static const bool external_linkage = true;
    static const bool indexable = false;
    static const bool is_variable = false;
    static const bool is_pointer = false;
    static const bool is_shared = false;
    static const size_t dimensions = 1;
    static const size_t component_count = 2;
    static const size_t location_count = 1;
    static const size_t item_count = 2;

    static VPP_INLINE KId getType()
    {
        return KId (
            KShaderTranslator::get()->makeVectorType (
                Int::getType(), static_cast< int >( 2 ) ) );
    }

    VPP_INLINE PHVec4 ( KId id ) :
        KValue ( id )
    {}

    VPP_INLINE PHVec4 ( const PHVec4& rhs ) :
        KValue ( rhs.id() )
    {
    }

    operator Vec4() const;

private:
    const PHVec4& operator= ( const PHVec4& ) = delete;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename NativeType >
struct shader_type {};

template<>
struct shader_type< std::int8_t >
{
    typedef Int rvalue_type;
    typedef VInt lvalue_type;
    typedef Int scalar_type;
};

template<>
struct shader_type< std::uint8_t >
{
    typedef UInt rvalue_type;
    typedef VUInt lvalue_type;
    typedef UInt scalar_type;
};

template<>
struct shader_type< std::int16_t >
{
    typedef Int rvalue_type;
    typedef VInt lvalue_type;
    typedef Int scalar_type;
};

template<>
struct shader_type< std::uint16_t >
{
    typedef UInt rvalue_type;
    typedef VUInt lvalue_type;
    typedef UInt scalar_type;
};

template<>
struct shader_type< std::int32_t >
{
    typedef Int rvalue_type;
    typedef VInt lvalue_type;
    typedef Int scalar_type;
};

template<>
struct shader_type< std::uint32_t >
{
    typedef UInt rvalue_type;
    typedef VUInt lvalue_type;
    typedef UInt scalar_type;
};

template<>
struct shader_type< std::int64_t >
{
    typedef Int64 rvalue_type;
    typedef VInt64 lvalue_type;
    typedef Int64 scalar_type;
};

template<>
struct shader_type< std::uint64_t >
{
    typedef UInt64 rvalue_type;
    typedef VUInt64 lvalue_type;
    typedef UInt64 scalar_type;
};

template<>
struct shader_type< float >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template<>
struct shader_type< double >
{
    typedef Double rvalue_type;
    typedef VDouble lvalue_type;
    typedef Double scalar_type;
};

template<>
struct shader_type< float16_t >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< usint_t< BaseT, OFFSET, BITS > >
{
    typedef UInt rvalue_type;
    typedef VUInt lvalue_type;
    typedef UInt scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< sint_t< BaseT, OFFSET, BITS > >
{
    typedef Int rvalue_type;
    typedef VInt lvalue_type;
    typedef Int scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< unorm_t< BaseT, OFFSET, BITS > >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< snorm_t< BaseT, OFFSET, BITS > >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< uscaled_t< BaseT, OFFSET, BITS > >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< sscaled_t< BaseT, OFFSET, BITS > >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename BaseT, size_t OFFSET, size_t BITS >
struct shader_type< srgb_t< BaseT, OFFSET, BITS > >
{
    typedef Float rvalue_type;
    typedef VFloat lvalue_type;
    typedef Float scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, no_component, no_component, no_component, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type rvalue_type;
    typedef typename shader_type< ItemT >::lvalue_type lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, no_component, no_component, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 2u > rvalue_type;
    typedef TLVector< item_rvalue_type, 2u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, no_component, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 3u > rvalue_type;
    typedef TLVector< item_rvalue_type, 3u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, BGR, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 3u > rvalue_type;
    typedef TLVector< item_rvalue_type, 3u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, ItemT, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 4u > rvalue_type;
    typedef TLVector< item_rvalue_type, 4u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, ItemT, BGRA > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 4u > rvalue_type;
    typedef TLVector< item_rvalue_type, 4u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, ItemT, ARGB > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 4u > rvalue_type;
    typedef TLVector< item_rvalue_type, 4u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, ItemT, ItemT, ItemT, ABGR > >
{
    typedef typename shader_type< ItemT >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 4u > rvalue_type;
    typedef TLVector< item_rvalue_type, 4u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT >
struct shader_type< format< ItemT, DT, no_component, no_component, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type rvalue_type;
    typedef typename shader_type< ItemT >::lvalue_type lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template< typename ItemT, typename Item2T >
struct shader_type< format< ItemT, Item2T, DS, no_component, no_component > >
{
    typedef typename shader_type< ItemT >::rvalue_type rvalue_type;
    typedef typename shader_type< ItemT >::lvalue_type lvalue_type;
    typedef typename shader_type< ItemT >::scalar_type scalar_type;
};

template<>
struct shader_type< format< texture_format, no_component, no_component, no_component, no_component > >
{
    typedef typename shader_type< float >::rvalue_type item_rvalue_type;
    typedef TRVector< item_rvalue_type, 4u > rvalue_type;
    typedef TLVector< item_rvalue_type, 4u, spv::StorageClassFunction > lvalue_type;
    typedef typename shader_type< float >::scalar_type scalar_type;
};

template< typename ExtType >
struct shader_type< format< ext< ExtType >, no_component, no_component, no_component, no_component > >
{
    typedef StructMemberTraits< ExtType > traits_type;
    typedef typename traits_type::scalar_type base_type;
    typedef typename shader_type< base_type >::scalar_type scalar_type;
    typedef scalar_type item_rvalue_type;
    typedef typename traits_type::rvalue_type rvalue_type;
    typedef typename traits_type::lvalue_type lvalue_type;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGVECTORTYPES_HPP
