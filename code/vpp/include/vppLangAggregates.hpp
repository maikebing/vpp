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

#ifndef INC_VPPLANGAGGREGATES_HPP
#define INC_VPPLANGAGGREGATES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGMATRIXTYPES_HPP
#include "vppLangMatrixTypes.hpp"
#endif

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
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

template< class StructT, int SIZE, int STRIDE = 0 >
class StructArray
{
public:
    typedef StructArrayItem< StructT > item_type;
    typedef typename StructT::definition_type definition_type;

    //template< std::uint32_t COUNT >
    //VPP_INLINE StructArray ( const inUniformBuffer< COUNT >& buf ) :
    //    d_id ( 0 )
    //{
    //    init ( spv::DecorationBlock, buf.set(), buf.binding() );
    //}

    //template< std::uint32_t COUNT >
    //VPP_INLINE StructArray ( const ioBuffer< COUNT >& buf ) :
    //    d_id ( 0 )
    //{
    //    init ( spv::DecorationBufferBlock, buf.set(), buf.binding() );
    //}

    //template< std::uint32_t COUNT >
    //VPP_INLINE StructArray ( const inUniformBufferDyn< COUNT >& buf ) :
    //    d_id ( 0 )
    //{
    //    init ( spv::DecorationBlock, buf.set(), buf.binding() );
    //}

    //template< std::uint32_t COUNT >
    //VPP_INLINE StructArray ( const ioBufferDyn< COUNT >& buf ) :
    //    d_id ( 0 )
    //{
    //    init ( spv::DecorationBufferBlock, buf.set(), buf.binding() );
    //}

    VPP_INLINE const StructArray< StructT, SIZE, STRIDE >& operator= (
        const StructArray< StructT, SIZE, STRIDE >& rhs )
    {
        KShaderTranslator::get()->createStore ( rhs.id(), d_id );
        return *this;
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }
    
    template< typename IndexT >
    VPP_INLINE item_type getItem ( const IndexT& index )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        return item_type (
            pTranslator->getArrayIndex ( index ),
            & d_struct,
            pTranslator->getAccessChain() );
    }

    VPP_INLINE item_type operator[]( const Int& index ) { return getItem ( index ); }
    VPP_INLINE item_type operator[]( const UInt& index ) { return getItem ( index ); }
    VPP_INLINE item_type operator[]( int index ) { return getItem ( index ); }
    VPP_INLINE item_type operator[]( unsigned int index ) { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( SIZE ) ) );
    }

    VPP_INLINE int size() const
    {
        return SIZE;
    }

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const std::type_index index ( typeid ( definition_type ) );

        const KShaderTranslator::SStructInfo* pStructInfo = 
            pTranslator->findStructType ( index, spv::DecorationLocation );

        if ( pStructInfo )
        {
            return KId (
                pTranslator->makeArrayType ( 
                    pStructInfo->d_typeId,
                    pTranslator->makeIntConstant ( SIZE ),
                    STRIDE 
            ) );
        }
        else
            return KId ( 0 );
    }

private:
    void init (
        spv::Decoration decoration,
        std::uint32_t set,
        std::uint32_t binding );

private:
    StructT d_struct;
    KId d_id;
};

// -----------------------------------------------------------------------------

template< class StructT, int SIZE, int STRIDE >
void StructArray< StructT, SIZE, STRIDE > :: init (
    spv::Decoration decoration,
    std::uint32_t set,
    std::uint32_t binding )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
    const bool bAddDecorations = ! structInfo.d_bDefined;

    pTranslator->popStructType();

    const KId arrayType = getType();

    if ( bAddDecorations )
    {
        pTranslator->addDecoration ( structInfo.d_typeId, decoration );

        pTranslator->addDecoration (
            arrayType,
            spv::DecorationArrayStride,
            STRIDE ? STRIDE : structInfo.d_currentMember );
    }

    d_id = pTranslator->registerUniformBuffer ( arrayType, set, binding, spv::StorageClassUniform );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ItemT, int SIZE >
class VArray : public KValue
{
public:
    typedef ItemT item_type;
    typedef VArray< ItemT, SIZE > rvalue_type;
    static const bool external_linkage = ItemT::external_linkage;
    static const bool indexable = true;
    static const size_t location_count = SIZE * ItemT::location_count;

    VPP_INLINE VArray() :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType() ) )
    {}

    template< typename IndexT >
    VPP_INLINE auto operator[]( IndexT index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        Accessor< VArray, true, true > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( SIZE ) ) );
    }

    VPP_INLINE int size() const
    {
        return SIZE;
    }

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId (
            pTranslator->makeArrayType ( 
                ItemT::getType(),
                pTranslator->makeIntConstant ( SIZE ),
                0 
        ) );
    }
};

// -----------------------------------------------------------------------------

template< class ItemT, int COLS, int ROWS >
using VArray2 = VArray< VArray< ItemT, ROWS >, COLS >;

template< class ItemT, int LAYERS, int COLS, int ROWS >
using VArray3 = VArray< VArray< VArray< ItemT, ROWS >, COLS >, LAYERS >;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGAGGREGATES_HPP
