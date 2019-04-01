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

#ifndef INC_VPPLANGAGGREGATES_HPP
#define INC_VPPLANGAGGREGATES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGMATRIXTYPES_HPP
#include "vppLangMatrixTypes.hpp"
#endif

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

#ifndef INC_VPPLANGCONSTRUCTS_HPP
#include "vppLangConstructs.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum ETag
{
    CPU,
    GPU,
    DEF
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class DefinitionT >
class LocalStruct
{
public:
    typedef DefinitionT definition_type;

    VPP_INLINE LocalStruct()
    {
        const std::type_index index ( typeid ( DefinitionT ) );

        if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        {
            pTranslator->pushStructType ( index, spv::DecorationLocation );
            KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
            structInfo.d_currentMember = 0;
        }
        else if ( PipelineConfig* pConfig = PipelineConfig::getInstance() )
        {
            pConfig->pushStructType ( index );
        }
    }
};

// -----------------------------------------------------------------------------

template< class FieldT, std::uint32_t FLAGS = 0 >
class Field
{
public:
    typedef typename FieldT::rvalue_type rvalue_type;
    typedef typename detail::TComparisonResult< FieldT >::type comparison_type;

    Field();

    VPP_INLINE unsigned int index() const
    {
        return d_memberIndex;
    }

private:
    const Field& operator= ( const Field& rhs ) = delete;

private:
    unsigned int d_memberIndex;
};

// -----------------------------------------------------------------------------

template< class FieldT, std::uint32_t FLAGS >
Field< FieldT, FLAGS > :: Field()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        
        if ( ! structInfo.d_bDefined )
        {
            structInfo.d_memberInfos.emplace_back ( KId ( FieldT::getType() ) );
            structInfo.d_memberInfos.back().d_flags = FLAGS;
        }

        d_memberIndex = structInfo.d_currentMember++;
    }
    else if ( PipelineConfig* pConfig = PipelineConfig::getInstance() )
    {
        detail::SStructLocationInfo& locationInfo = pConfig->currentStructType();

        if ( ! locationInfo.d_bDefined )
            locationInfo.d_occupiedLocations += rvalue_type::location_count;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class StructT >
class StructArrayItem
{
public:
    VPP_INLINE StructArrayItem (
        const KId& id,
        StructT* pStruct,
        const spv::Builder::AccessChain& ac ):
            d_arrayIndexId ( id ),
            d_pStruct ( pStruct ),
            d_accessChain ( ac )
    {}

    template< typename MemberT >
    VPP_INLINE Accessor< typename MemberT::rvalue_type > operator[]( MemberT StructT::* pMember ) const
    {
        typedef typename MemberT::rvalue_type rvalue_type;
        typedef Accessor< rvalue_type > result_type;

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPush ( d_arrayIndexId );

        const MemberT& member = d_pStruct->*pMember;
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( member.index() ) );

        return result_type ( pTranslator->getAccessChain() );
    }

private:
    KId d_arrayIndexId;
    StructT* d_pStruct;
    spv::Builder::AccessChain d_accessChain;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ItemT >
class VArray : public KValue
{
public:
    typedef ItemT item_type;
    typedef VArray< ItemT > rvalue_type;
    static const bool external_linkage = ItemT::external_linkage;
    static const bool indexable = true;

    VPP_INLINE VArray ( int s ) :
        KValue ( KShaderTranslator::get()->acquireCachedLocalVariable (
            getType ( s ), spv::StorageClassFunction, ItemT::component_count ) ),
        d_size ( s ),
        d_bRelease ( true )
    {}

    VArray ( const VArray< ItemT >& rhs ) = delete;

    VPP_INLINE VArray ( VArray< ItemT >&& rhs ) :
        KValue ( rhs.id() ),
        d_size ( rhs.d_size ),
        d_bRelease ( rhs.d_bRelease )
    {
        rhs.d_bRelease = false;
    }

    VPP_INLINE ~VArray()
    {
        if ( d_bRelease )
            KShaderTranslator::get()->releaseCachedLocalVariable ( id(), getType(), spv::StorageClassFunction );
    }

    const VArray& operator= ( const VArray< ItemT >& rhs ) = delete;

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        Accessor< VArray< ItemT >, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

    VPP_INLINE KId getType ( int s )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId (
            pTranslator->makeArrayType ( 
                ItemT::getType(),
                pTranslator->makeIntConstant ( s ),
                0 
        ) );
    }

    VPP_INLINE KId getType()
    {
        return getType ( d_size );
    }

private:
    int d_size;
    bool d_bRelease;
};

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class KSharedVariableBase
{
public:
    KSharedVariableBase();
    KSharedVariableBase ( unsigned int nWords );
};

// -----------------------------------------------------------------------------

VPP_INLINE KSharedVariableBase :: KSharedVariableBase()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KSharedVariableBase :: KSharedVariableBase ( unsigned int nWords )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    pTranslator->registerSharedVariableAllocation ( 4 * nWords );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ValueT, bool ENABLE_READ = true, bool ENABLE_WRITE = true >
class DynamicAccessor
{
public:
    typedef ValueT lvalue_type;
    typedef typename ValueT::item_type item_type;
    typedef Accessor< item_type, ENABLE_READ, ENABLE_WRITE > return_type;

    VPP_INLINE DynamicAccessor ( const spv::Builder::AccessChain& ac, int s ):
        d_accessChain ( ac ),
        d_size ( s )
    {}

    VPP_INLINE operator lvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        return lvalue_type ( KId ( pTranslator->accessChainGetLValue() ), d_size );
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return return_type ( pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) const { return getItem ( index ); }

private:
    spv::Builder::AccessChain d_accessChain;
    int d_size;
};

// -----------------------------------------------------------------------------

template< class ValueT, bool ENABLE_READ = true, bool ENABLE_WRITE = true >
class DynamicAccessor2
{
public:
    typedef ValueT lvalue_type;
    typedef typename ValueT::item_type item_type;
    typedef DynamicAccessor< item_type, ENABLE_READ, ENABLE_WRITE > return_type;

    VPP_INLINE DynamicAccessor2 ( const spv::Builder::AccessChain& ac, int r, int c ):
        d_accessChain ( ac ),
        d_rows ( r ),
        d_cols ( c )
    {}

    VPP_INLINE operator lvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        return lvalue_type ( KId ( pTranslator->accessChainGetLValue() ), r, c );
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->setAccessChain ( d_accessChain );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return return_type ( pTranslator->getAccessChain(), d_rows );
    }

    VPP_INLINE return_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) const { return getItem ( index ); }

private:
    spv::Builder::AccessChain d_accessChain;
    int d_rows;
    int d_cols;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ItemT >
class WArray :
    public detail::KSharedVariableBase,
    public KValue
{
public:
    typedef ItemT base_type;
    typedef Accessor< ItemT, true, true > item_type;
    typedef WArray< ItemT > rvalue_type;
    typedef WArray< ItemT > lvalue_type;
    static const bool indexable = true;
    static const size_t dimensions = 1;

    VPP_INLINE explicit WArray ( int s ) :
        detail::KSharedVariableBase ( static_cast< unsigned int >( s ) * ItemT::component_count ),
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType ( s ), spv::StorageClassWorkgroup ) ),
        d_size ( s )
    {}

    VPP_INLINE WArray ( const KId& id, int s ) :
        detail::KSharedVariableBase(),
        KValue ( id ),
        d_size ( s )
    {}

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        Accessor< WArray< ItemT >, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

    VPP_INLINE KId getType ( int s )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId (
            pTranslator->makeArrayType ( 
                ItemT::getType(),
                pTranslator->makeIntConstant ( s ),
                0 
        ) );
    }

    VPP_INLINE KId getType()
    {
        return getType ( int d_size );
    }

private:
    int d_size;
};

// -----------------------------------------------------------------------------

template< class ItemT >
class WArray2 :
    public detail::KSharedVariableBase,
    public KValue
{
public:
    typedef ItemT base_type;
    typedef DynamicAccessor< WArray< ItemT > > item_type;
    typedef WArray2< ItemT > rvalue_type;
    typedef WArray2< ItemT > lvalue_type;

    static const bool indexable = true;
    static const size_t dimensions = 2;

    VPP_INLINE WArray2 ( int nr, int nc ) :
        detail::KSharedVariableBase ( static_cast< unsigned int >( nr * nc ) * ItemT::component_count ),
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType ( nr, nc ), spv::StorageClassWorkgroup ) ),
        d_rows ( nr ),
        d_cols ( nc )
    {}

    VPP_INLINE WArray2 ( const KId& id, int nr, int nc ) :
        detail::KSharedVariableBase(),
        KValue ( id ),
        d_rows ( nr ),
        d_cols ( nc )
    {}

    template< typename IndexT >
    VPP_INLINE item_type operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return item_type ( pTranslator->getAccessChain(), d_cols );
    }

    template< typename Index1T, typename Index2T >
    VPP_INLINE auto operator()( const Index1T& iRow, const Index2T& iCol ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( iRow ) );

        Accessor< WArray< ItemT >, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ iCol ];
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_rows ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_rows;
    }

    VPP_INLINE Int Rows() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_rows ) ) );
    }

    VPP_INLINE int rows() const
    {
        return d_rows;
    }

    VPP_INLINE Int Cols() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_cols ) ) );
    }

    VPP_INLINE int cols() const
    {
        return d_cols;
    }

    VPP_INLINE KId getType ( int nr, int nc ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        const KId innerType = KId ( pTranslator->makeArrayType ( 
            ItemT::getType(), pTranslator->makeIntConstant ( nc ), 0 ) );

        return KId ( pTranslator->makeArrayType ( 
            innerType, pTranslator->makeIntConstant ( nr ), 0  ) );
    }

    VPP_INLINE KId getType() const
    {
        return getType ( d_rows, d_cols );
    }

private:
    int d_rows;
    int d_cols;
};

// -----------------------------------------------------------------------------

template< class ItemT >
class WArray3 :
    public detail::KSharedVariableBase,
    public KValue
{
public:
    typedef ItemT base_type;
    typedef DynamicAccessor2< WArray2< ItemT > > item_type;
    typedef WArray3< ItemT > rvalue_type;
    typedef WArray3< ItemT > lvalue_type;

    static const bool indexable = true;
    static const size_t dimensions = 3;

    VPP_INLINE WArray3 ( int nl, int nr, int nc ) :
        detail::KSharedVariableBase ( static_cast< unsigned int >( nl * nr * nc ) * ItemT::component_count ),
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType ( nl, nr, nc ), spv::StorageClassWorkgroup ) ),
        d_layers ( nl ),
        d_rows ( nr ),
        d_cols ( nc )
    {}

    template< typename IndexT >
    VPP_INLINE item_type operator[]( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return item_type ( pTranslator->getAccessChain(), d_rows, d_cols );
    }

    template< typename IndexT >
    VPP_INLINE auto operator()( const IndexT& iLayer, const IndexT& iRow, const IndexT& iCol ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( iLayer ) );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( iRow ) );

        Accessor< WArray< ItemT >, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ iCol ];
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_layers ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_layers;
    }

    VPP_INLINE Int Layers() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_layers ) ) );
    }

    VPP_INLINE int layers() const
    {
        return d_layers;
    }

    VPP_INLINE Int Rows() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_rows ) ) );
    }

    VPP_INLINE int rows() const
    {
        return d_rows;
    }

    VPP_INLINE Int Cols() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_cols ) ) );
    }

    VPP_INLINE int cols() const
    {
        return d_cols;
    }

    VPP_INLINE KId getType ( int nl, int nr, int nc ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        const KId innermostType = KId ( pTranslator->makeArrayType ( 
            ItemT::getType(), pTranslator->makeIntConstant ( nc ), 0 ) );

        const KId innerType = KId ( pTranslator->makeArrayType ( 
            innermostType, pTranslator->makeIntConstant ( nr ), 0 ) );

        return KId ( pTranslator->makeArrayType ( 
            innerType, pTranslator->makeIntConstant ( nl ), 0  ) );
    }

    VPP_INLINE KId getType() const
    {
        return getType ( d_layers, d_rows, d_cols );
    }

private:
    int d_layers;
    int d_rows;
    int d_cols;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGAGGREGATES_HPP
