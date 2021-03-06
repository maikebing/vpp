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

#ifndef INC_VPPLANGMATRIXTYPES_HPP
#define INC_VPPLANGMATRIXTYPES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGVECTORTYPES_HPP
#include "vppLangVectorTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

// Internal matrices are column-major (columns are vectors), as stated
// in the 'OpTypeMatrix' docs.

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define VPP_DEFINE_MATRIX_OPERATOR1( OP ) \
VPP_INLINE rvalue_type operator OP () const \
{ rvalue_type rhs = *this; return OP rhs; } \

#define VPP_DEFINE_MATRIX_OPERATOR2( OP ) \
template< typename RightT > \
VPP_INLINE rvalue_type operator OP ( const RightT& rhs ) const \
{ rvalue_type lhs = *this; VPP_RVTYPE( RightT ) rrv = rhs; return lhs OP rrv; } \

#define VPP_DEFINE_MATRIX_OPERATORS \
    VPP_DEFINE_MATRIX_OPERATOR2( + ) \
    VPP_DEFINE_MATRIX_OPERATOR2( - ) \
    VPP_DEFINE_MATRIX_OPERATOR2( / ) \
    VPP_DEFINE_MATRIX_OPERATOR2( % ) \
    VPP_DEFINE_MATRIX_OPERATOR2( << ) \
    VPP_DEFINE_MATRIX_OPERATOR2( >> ) \
    VPP_DEFINE_MATRIX_OPERATOR2( | ) \
    VPP_DEFINE_MATRIX_OPERATOR2( ^ ) \
    VPP_DEFINE_MATRIX_OPERATOR2( & ) \
    VPP_DEFINE_MATRIX_OPERATOR1( - ) \
    VPP_DEFINE_MATRIX_OPERATOR1( ~ ) \

// -----------------------------------------------------------------------------

template< class BaseType, size_t COLS, size_t ROWS >
class TRMatrix : public KValue
{
public:
    typedef BaseType base_type;
    typedef typename BaseType::scalar_type scalar_type;
    typedef TRVector< BaseType, ROWS > column_type;
    typedef TRVector< BaseType, ROWS > item_type;
    typedef TRMatrix< BaseType, COLS, ROWS > rvalue_type;
    typedef TRMatrix< Bool, COLS, ROWS > comparison_type;
    static const bool external_linkage = BaseType::external_linkage;
    static const bool indexable = true;
    static const bool is_variable = false;
    static const bool is_pointer = false;
    static const bool is_shared = false;
    static const bool is_64bit = BaseType::is_64bit;
    static const size_t dimensions = 2;
    static const size_t component_count = COLS * TRVector< BaseType, ROWS >::component_count;
    static const size_t location_count = COLS * TRVector< BaseType, ROWS >::location_count;
    static const size_t item_count = COLS;

    static KId createConstant ( const std::initializer_list< scalar_type >& init )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > colIds ( COLS );

        const scalar_type* pInit = init.begin();
        const size_t initSize = init.size();

        for ( size_t iCol = 0; iCol != COLS; ++iCol )
        {
            std::vector< spv::Id > componentIds ( ROWS );

            for ( size_t iComp = 0; iComp != ROWS; ++iComp )
            {
                const size_t index = iCol + iComp * COLS;
                const scalar_type value = ( index < initSize ? pInit [ index ] : scalar_type() );
                componentIds [ iComp ] = scalar_traits< scalar_type >::makeConstant ( value );
            }

            colIds [ iCol ] = pTranslator->makeCompositeConstant (
                column_type::getType(), componentIds );
        }

        return KId ( pTranslator->makeCompositeConstant ( getType(), colIds ) );
    }

    static KId createFromColumns ( const std::initializer_list< column_type >& init )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > colIds ( COLS );

        const column_type* pInit = init.begin();
        const size_t initSize = init.size();
        unsigned int dummyColumnId = 0;

        for ( size_t iCol = 0; iCol != COLS; ++iCol )
        {
            if ( iCol < initSize )
                colIds [ iCol ] = pInit [ iCol ].id();
            else
            {
                if ( ! dummyColumnId )
                {
                    scalar_type zeroScalar = scalar_type();
                    column_type dummyColumn ( zeroScalar );
                    dummyColumnId = dummyColumn.id();
                }

                colIds [ iCol ] = dummyColumnId;
            }
        }

        return KId (
            pTranslator->createCompositeConstruct ( getType(), colIds ) );
    }

    static KId createFromItems ( const std::initializer_list< base_type >& init )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > colIds ( COLS );

        const base_type* pInit = init.begin();
        const size_t initSize = init.size();
        unsigned int dummyItemId = 0;

        for ( size_t iCol = 0; iCol != COLS; ++iCol )
        {
            std::vector< spv::Id > componentIds ( ROWS );

            for ( size_t iComp = 0; iComp != ROWS; ++iComp )
            {
                const size_t index = iCol + iComp * COLS;

                if ( index < initSize )
                    componentIds [ iComp ] = pInit [ index ].id();
                else
                {
                    if ( ! dummyItemId )
                    {
                        base_type dummyItem = base_type ( scalar_type() );
                        dummyItemId = dummyItem.id();
                    }

                    componentIds [ iComp ] = dummyItemId;
                }
            }

            colIds [ iCol ] = pTranslator->createCompositeConstruct (
                column_type::getType(), componentIds );
        }

        return KId (
            pTranslator->createCompositeConstruct ( getType(), colIds ) );
    }

    template< size_t RCOLS, size_t RROWS >
    static VPP_INLINE KId createFromMatrix ( const TRMatrix< BaseType, RCOLS, RROWS >& init )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const std::vector< spv::Id > ids ( 1, init.id() );

        return KId (
            pTranslator->createMatrixConstructor (
                KShaderTranslator::get()->getPrecision(), ids, getType() ) );
    }

    VPP_INLINE TRMatrix ( KId id ) :
        KValue ( id )
    {
    }

    VPP_INLINE TRMatrix ( const TRMatrix< BaseType, COLS, ROWS >& init ) :
        KValue ( init.id() )
    {
    }

    VPP_INLINE TRMatrix ( const std::initializer_list< scalar_type >& init ) :
        KValue ( createConstant ( init ) )
    {}

    VPP_INLINE TRMatrix ( const std::initializer_list< column_type >& init ) :
        KValue ( createFromColumns ( init ) )
    {}

    VPP_INLINE TRMatrix ( const std::initializer_list< base_type >& init ) :
        KValue ( createFromItems ( init ) )
    {}

    template< size_t RCOLS, size_t RROWS >
    explicit VPP_INLINE TRMatrix ( const TRMatrix< BaseType, RCOLS, RROWS >& init ) :
        KValue ( createFromMatrix ( init ) )
    {}

    VPP_INLINE column_type getColumn ( unsigned int index ) const
    {
        return KId (
            KShaderTranslator::get()->createCompositeExtract (
                id(), column_type::getType(), index ) );
    }

    template< class FunctorT >
    VPP_INLINE TRMatrix componentWiseOperation ( const TRMatrix& rhs, FunctorT fVecOp ) const
    {
        std::vector< spv::Id > newColIds ( COLS );

        for ( unsigned int iCol = 0; iCol != COLS; ++iCol )
        {
            column_type lhsCol = getColumn ( iCol );
            column_type rhsCol = rhs.getColumn ( iCol );

            TLVector< BaseType, ROWS > resultCol;
            resultCol = fVecOp ( lhsCol, rhsCol );

            newColIds [ iCol ] = resultCol.id();
        }

        return TRMatrix ( KId (
            KShaderTranslator::get()->createCompositeConstruct (
                getType(), newColIds ) ) );
    }

    template< class FunctorT >
    VPP_INLINE TRMatrix componentWiseOperation ( FunctorT fVecOp ) const
    {
        std::vector< spv::Id > newColIds ( COLS );

        for ( unsigned int iCol = 0; iCol != COLS; ++iCol )
        {
            column_type rhsCol = getColumn ( iCol );

            TLVector< BaseType, ROWS > resultCol;
            resultCol = fVecOp ( rhsCol );

            newColIds [ iCol ] = resultCol.id();
        }

        return TRMatrix ( KId (
            KShaderTranslator::get()->createCompositeConstruct (
                getType(), newColIds ) ) );
    }

    VPP_INLINE TRMatrix operator+ ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 + v2; } );
    }

    VPP_INLINE TRMatrix operator- ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 - v2; } );
    }

    template< size_t RCOLS >
    VPP_INLINE TRMatrix< BaseType, RCOLS, ROWS > operator* ( const TRMatrix< BaseType, RCOLS, COLS >& rhs ) const
    {
        return TRMatrix< BaseType, RCOLS, ROWS > ( KId (
            KShaderTranslator::get()->createBinOp (
                spv::OpMatrixTimesMatrix,
                TRMatrix< BaseType, RCOLS, ROWS >::getType(), id(), rhs.id() ) ) );
    }

    VPP_INLINE TRVector< BaseType, ROWS > operator* ( const TRVector< BaseType, COLS >& rhs ) const
    {
        return TRVector< BaseType, ROWS > ( KId (
            KShaderTranslator::get()->createBinOp (
                spv::OpMatrixTimesVector,
                TRVector< BaseType, ROWS >::getType(), id(), rhs.id() ) ) );
    }

    VPP_INLINE TRMatrix< BaseType, COLS, ROWS > operator* ( const BaseType& rhs ) const
    {
        return TRMatrix< BaseType, COLS, ROWS > ( KId (
            KShaderTranslator::get()->createBinOp (
                spv::OpMatrixTimesScalar,
                TRMatrix< BaseType, COLS, ROWS >::getType(), id(), rhs.id() ) ) );
    }

    template< typename RightT >
    VPP_INLINE auto operator* ( const RightT& rhs ) const
    {
        const typename detail::TGetRV< RightT >::type rValue = rhs;
        return *this * rValue;
    }

    VPP_INLINE TRMatrix operator/ ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 / v2; } );
    }

    VPP_INLINE TRMatrix operator% ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 % v2; } );
    }

    VPP_INLINE TRMatrix operator<< ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 << v2; } );
    }

    VPP_INLINE TRMatrix operator>> ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 >> v2; } );
    }

    VPP_INLINE TRMatrix operator| ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 | v2; } );
    }

    VPP_INLINE TRMatrix operator^ ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 ^ v2; } );
    }

    VPP_INLINE TRMatrix operator& ( const TRMatrix& rhs ) const
    {
        return componentWiseOperation (
            rhs, []( const TRVector< BaseType, ROWS >& v1, 
                     const TRVector< BaseType, ROWS >& v2 ) { return v1 & v2; } );
    }

    VPP_INLINE TRMatrix operator-() const
    {
        return componentWiseOperation (
            []( const TRVector< BaseType, ROWS >& v ) { return -v; } );
    }

    VPP_INLINE TRMatrix operator~() const
    {
        return componentWiseOperation (
            []( const TRVector< BaseType, ROWS >& v ) { return ~v; } );
    }

    VPP_INLINE TRVector< BaseType, COLS > prependMultiply ( const TRVector< BaseType, ROWS >& lhs ) const
    {
        return TRVector< BaseType, ROWS > ( KId (
            KShaderTranslator::get()->createBinOp (
                spv::OpVectorTimesMatrix,
                TRVector< BaseType, COLS >::getType(), lhs.id(), id() ) ) );
    }

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainRValue ( id() );

        Accessor< TRMatrix, true, false > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    static VPP_INLINE KId getType()
    {
        return KId (
            KShaderTranslator::get()->makeMatrixType (
                BaseType::getType(),
                static_cast< int >( COLS ),
                static_cast< int >( ROWS ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class BaseType, size_t COLS, size_t ROWS, spv::StorageClass SCL >
class TLMatrix : public KValue
{
public:
    typedef TRMatrix< BaseType, COLS, ROWS > rvalue_type;
    static const bool is_variable = true;
    static const bool is_pointer = false;
    static const bool is_shared = false;
    static const size_t dimensions = 2;

    VPP_INLINE TLMatrix() :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
    }

    VPP_INLINE TLMatrix ( const TRMatrix< BaseType, COLS, ROWS >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
        KShaderTranslator::get()->createStore ( rhs.id(), id() );
    }

    VPP_INLINE TLMatrix ( const TLMatrix< BaseType, COLS, ROWS, SCL >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
    }

    VPP_INLINE TLMatrix ( TLMatrix< BaseType, COLS, ROWS, SCL >&& rhs ) :
        KValue ( rhs.id() ),
        d_bRelease ( rhs.d_bRelease )
    {
        rhs.d_bRelease = false;
    }

    template< spv::StorageClass SCL2 >
    VPP_INLINE TLMatrix ( const TLMatrix< BaseType, COLS, ROWS, SCL2 >& rhs ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType(), SCL, rvalue_type::component_count ) ),
        d_bRelease ( true )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
    }

    template< typename RightT >
    VPP_INLINE const TLMatrix< BaseType, COLS, ROWS, SCL >& operator= ( const RightT& rhs )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
        return *this;
    }

    VPP_INLINE const TLMatrix< BaseType, COLS, ROWS, SCL >& operator= ( const TLMatrix< BaseType, COLS, ROWS, SCL >& rhs )
    {
        const rvalue_type value = rhs;
        KShaderTranslator::get()->createStore ( value.id(), id() );
        return *this;
    }

    VPP_INLINE ~TLMatrix()
    {
        if ( d_bRelease )
            KShaderTranslator::get()->releaseCachedLocalVariable ( id(), getType(), SCL );
    }

    VPP_INLINE operator rvalue_type() const
    {
        return rvalue_type ( KId ( KShaderTranslator::get()->createLoad ( id() ) ) );
    }

    template< size_t RCOLS >
    VPP_INLINE TRMatrix< BaseType, RCOLS, ROWS > operator* ( const TRMatrix< BaseType, RCOLS, COLS >& rhs ) const
    {
        const rvalue_type lhs = *this;
        return lhs * rhs;
    }

    template< size_t RCOLS, spv::StorageClass SCL2 >
    VPP_INLINE TRMatrix< BaseType, RCOLS, ROWS > operator* ( const TLMatrix< BaseType, RCOLS, COLS, SCL2 >& rhs ) const
    {
        const rvalue_type lhs = *this;
        const TRMatrix< BaseType, RCOLS, COLS > rrhs = rhs;
        return lhs * rrhs;
    }

    VPP_INLINE TRVector< BaseType, ROWS > operator* ( const TRVector< BaseType, COLS >& rhs ) const
    {
        const rvalue_type lhs = *this;
        return lhs * rhs;
    }

    VPP_INLINE TRVector< BaseType, COLS > prependMultiply ( const TRVector< BaseType, ROWS >& lhs ) const
    {
        const rvalue_type rrv = *this;
        return rrv.prependMultiply ( lhs );
    }

    VPP_DEFINE_MATRIX_OPERATORS;

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        Accessor< TLMatrix, true, true > accessor ( pTranslator->getAccessChain() );
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

typedef TRMatrix< Float, 2, 2 > Mat2;
typedef TLMatrix< Float, 2, 2, spv::StorageClassFunction > VMat2;
typedef TLMatrix< Float, 2, 2, spv::StorageClassWorkgroup > WMat2;
typedef TRMatrix< Float, 3, 3 > Mat3;
typedef TLMatrix< Float, 3, 3, spv::StorageClassFunction > VMat3;
typedef TLMatrix< Float, 3, 3, spv::StorageClassWorkgroup > WMat3;
typedef TRMatrix< Float, 4, 4 > Mat4;
typedef TLMatrix< Float, 4, 4, spv::StorageClassFunction > VMat4;
typedef TLMatrix< Float, 4, 4, spv::StorageClassWorkgroup > WMat4;
typedef TRMatrix< Float, 2, 3 > Mat2x3;
typedef TLMatrix< Float, 2, 3, spv::StorageClassFunction > VMat2x3;
typedef TLMatrix< Float, 2, 3, spv::StorageClassWorkgroup > WMat2x3;
typedef TRMatrix< Float, 2, 4 > Mat2x4;
typedef TLMatrix< Float, 2, 4, spv::StorageClassFunction > VMat2x4;
typedef TLMatrix< Float, 2, 4, spv::StorageClassWorkgroup > WMat2x4;
typedef TRMatrix< Float, 3, 2 > Mat3x2;
typedef TLMatrix< Float, 3, 2, spv::StorageClassFunction > VMat3x2;
typedef TLMatrix< Float, 3, 2, spv::StorageClassWorkgroup > WMat3x2;
typedef TRMatrix< Float, 3, 4 > Mat3x4;
typedef TLMatrix< Float, 3, 4, spv::StorageClassFunction > VMat3x4;
typedef TLMatrix< Float, 3, 4, spv::StorageClassWorkgroup > WMat3x4;
typedef TRMatrix< Float, 4, 2 > Mat4x2;
typedef TLMatrix< Float, 4, 2, spv::StorageClassFunction > VMat4x2;
typedef TLMatrix< Float, 4, 2, spv::StorageClassWorkgroup > WMat4x2;
typedef TRMatrix< Float, 4, 3 > Mat4x3;
typedef TLMatrix< Float, 4, 3, spv::StorageClassFunction > VMat4x3;
typedef TLMatrix< Float, 4, 3, spv::StorageClassWorkgroup > WMat4x3;

typedef TRMatrix< Double, 2, 2 > DMat2;
typedef TLMatrix< Double, 2, 2, spv::StorageClassFunction > VDMat2;
typedef TLMatrix< Double, 2, 2, spv::StorageClassWorkgroup > WDMat2;
typedef TRMatrix< Double, 3, 3 > DMat3;
typedef TLMatrix< Double, 3, 3, spv::StorageClassFunction > VDMat3;
typedef TLMatrix< Double, 3, 3, spv::StorageClassWorkgroup > WDMat3;
typedef TRMatrix< Double, 4, 4 > DMat4;
typedef TLMatrix< Double, 4, 4, spv::StorageClassFunction > VDMat4;
typedef TLMatrix< Double, 4, 4, spv::StorageClassWorkgroup > WDMat4;
typedef TRMatrix< Double, 2, 3 > DMat2x3;
typedef TLMatrix< Double, 2, 3, spv::StorageClassFunction > VDMat2x3;
typedef TLMatrix< Double, 2, 3, spv::StorageClassWorkgroup > WDMat2x3;
typedef TRMatrix< Double, 2, 4 > DMat2x4;
typedef TLMatrix< Double, 2, 4, spv::StorageClassFunction > VDMat2x4;
typedef TLMatrix< Double, 2, 4, spv::StorageClassWorkgroup > WDMat2x4;
typedef TRMatrix< Double, 3, 2 > DMat3x2;
typedef TLMatrix< Double, 3, 2, spv::StorageClassFunction > VDMat3x2;
typedef TLMatrix< Double, 3, 2, spv::StorageClassWorkgroup > WDMat3x2;
typedef TRMatrix< Double, 3, 4 > DMat3x4;
typedef TLMatrix< Double, 3, 4, spv::StorageClassFunction > VDMat3x4;
typedef TLMatrix< Double, 3, 4, spv::StorageClassWorkgroup > WDMat3x4;
typedef TRMatrix< Double, 4, 2 > DMat4x2;
typedef TLMatrix< Double, 4, 2, spv::StorageClassFunction > VDMat4x2;
typedef TLMatrix< Double, 4, 2, spv::StorageClassWorkgroup > WDMat4x2;
typedef TRMatrix< Double, 4, 3 > DMat4x3;
typedef TLMatrix< Double, 4, 3, spv::StorageClassFunction > VDMat4x3;
typedef TLMatrix< Double, 4, 3, spv::StorageClassWorkgroup > WDMat4x3;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGMATRIXTYPES_HPP
