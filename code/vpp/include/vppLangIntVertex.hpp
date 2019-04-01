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

#ifndef INC_VPPLANGINTVERTEX_HPP
#define INC_VPPLANGINTVERTEX_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGINTBASE_HPP
#include "vppLangIntBase.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 14.2. Vertex Input Interface
// -----------------------------------------------------------------------------

template< ETag TAG, typename C1, typename C2 = no_component, typename C3 = no_component,
          typename C4 = no_component, typename C5 = no_component >
struct Attribute
{
};

// -----------------------------------------------------------------------------

template< typename C1, typename C2, typename C3, typename C4, typename C5 >
struct Attribute< CPU, C1, C2, C3, C4, C5 > :
    public format< C1, C2, C3, C4, C5 >::data_type
{
    VPP_INLINE Attribute (
        typename format< C1, C2, C3, C4, C5 >::data_type&& values ) :
            format< C1, C2, C3, C4, C5 >::data_type ( values )
    {}

    template< typename... Args >
    VPP_INLINE Attribute ( Args... args ) :
        format< C1, C2, C3, C4, C5 >::data_type ( args... )
    {
    }

    template< typename TargetT >
    VPP_INLINE void get ( TargetT* pDest ) const
    {
        std::memcpy ( pDest, this, sizeof ( *this ) );
    }
};

// -----------------------------------------------------------------------------

template< typename C1, typename C2, typename C3, typename C4, typename C5 >
struct Attribute< GPU, C1, C2, C3, C4, C5 > :
    public format< C1, C2, C3, C4, C5 >::data_type
{
    typedef format< C1, C2, C3, C4, C5 > format_type;
    typedef typename shader_type< format_type >::rvalue_type rvalue_type;
    typedef typename shader_type< format_type >::lvalue_type lvalue_type;
    typedef typename rvalue_type::comparison_type comparison_type;

    VPP_INLINE Attribute()
    {
        PipelineConfig* pDataSpec = PipelineConfig::getInstance();
        pDataSpec->createVertexDataField ( this, format_type::code );
    }

    VPP_INLINE operator rvalue_type() const
    {
        PipelineConfig* pDataSpec = PipelineConfig::getInstance();
        PipelineConfig::VertexFieldInfo fieldInfo = pDataSpec->findVertexDataField ( this );
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( ! fieldInfo.first->d_shaderVariableId )
        {
            fieldInfo.first->d_shaderVariableId = KId (
                pTranslator->createVariable (
                    spv::StorageClassInput, rvalue_type::getType() ) );

            pTranslator->addDecoration (
                fieldInfo.first->d_shaderVariableId,
                spv::DecorationBinding,
                static_cast< int >( fieldInfo.second->binding ) );

            pTranslator->addDecoration (
                fieldInfo.first->d_shaderVariableId,
                spv::DecorationLocation,
                static_cast< int >( fieldInfo.second->location ) );

            pTranslator->registerInputOutputVariable (
                fieldInfo.first->d_shaderVariableId );
        }

        return rvalue_type (
            pTranslator->getScopedRValue ( fieldInfo.first->d_shaderVariableId ) );
    }

    template< typename IndexT >
    VPP_INLINE auto operator[] ( const IndexT& index ) const
    {
        const rvalue_type rValue = *this;
        return rValue [ index ];
    }

    template< typename RightT >
    VPP_INLINE auto operator* ( const RightT& rhs ) const
    {
        const rvalue_type rValue1 = *this;
        const typename RightT::rvalue_type rValue2 = rhs;
        return rValue1 * rValue2;
    }

    VPP_DEFINE_VECTOR_OPERATORS1;
    VPP_DEFINE_VECTOR_OPERATORS2;
};

// -----------------------------------------------------------------------------

template< typename ExtType >
struct Attribute< CPU, ext< ExtType >, no_component, no_component, no_component, no_component > :
    public ExtType
{
    VPP_INLINE Attribute()
    {
    }

    VPP_INLINE Attribute ( ExtType&& value ) :
        ExtType ( value )
    {}
};

// -----------------------------------------------------------------------------

template< typename ExtType >
struct Attribute< GPU, ext< ExtType >, no_component, no_component, no_component, no_component > :
    public ExtType
{
    typedef typename StructMemberTraits< ExtType >::rvalue_type rvalue_type;
    typedef typename StructMemberTraits< ExtType >::lvalue_type lvalue_type;
    typedef typename rvalue_type::comparison_type comparison_type;

    VPP_INLINE Attribute()
    {
        PipelineConfig* pDataSpec = PipelineConfig::getInstance();

        for ( unsigned int i = 0; i != StructMemberTraits< ExtType >::attrib_count; ++i )
            pDataSpec->createVertexDataField (
                reinterpret_cast< char* >( this ) + i * StructMemberTraits< ExtType >::matrix_stride,
                StructMemberTraits< ExtType >::format_code );
    }

    VPP_INLINE operator rvalue_type() const
    {
        PipelineConfig* pDataSpec = PipelineConfig::getInstance();
        PipelineConfig::VertexFieldInfo fieldInfo = pDataSpec->findVertexDataField ( this );
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( ! fieldInfo.first->d_shaderVariableId )
        {
            fieldInfo.first->d_shaderVariableId = KId (
                pTranslator->createVariable (
                    spv::StorageClassInput, rvalue_type::getType() ) );

            pTranslator->addDecoration (
                fieldInfo.first->d_shaderVariableId,
                spv::DecorationBinding,
                static_cast< int >( fieldInfo.second->binding ) );

            pTranslator->addDecoration (
                fieldInfo.first->d_shaderVariableId,
                spv::DecorationLocation,
                static_cast< int >( fieldInfo.second->location ) );

            pTranslator->registerInputOutputVariable (
                fieldInfo.first->d_shaderVariableId );
        }

        return rvalue_type (
            pTranslator->getScopedRValue ( fieldInfo.first->d_shaderVariableId ) );
    }

    template< typename IndexT >
    VPP_INLINE auto operator[] ( const IndexT& index ) const
    {
        const rvalue_type rValue = *this;
        return rValue [ index ];
    }

    template< typename RightT >
    VPP_INLINE auto operator* ( const RightT& rhs ) const
    {
        const rvalue_type rValue1 = *this;
        const typename RightT::rvalue_type rValue2 = rhs;
        return rValue1 * rValue2;
    }

    VPP_DEFINE_VECTOR_OPERATORS1;
    VPP_DEFINE_VECTOR_OPERATORS2;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KVertexInputBinder
{
public:
    VPP_INLINE KVertexInputBinder (
        const void* pBase, const VertexBufferView& hVertexBufferView ) :
            d_pBase ( pBase ),
            d_hVertexBufferView ( hVertexBufferView )
    {}

    template< class FunctorT >
    VPP_INLINE void for_each ( FunctorT& fOperation ) const
    {
        fOperation.bindVertexInput ( d_pBase, d_hVertexBufferView );
    }

    static const size_t BINDER_COUNT = 1;

private:
    const void* d_pBase;
    VertexBufferView d_hVertexBufferView;
};

// -----------------------------------------------------------------------------

template< class LeftT >
class TVertexInputBinderList
{
public:
    VPP_INLINE TVertexInputBinderList ( const LeftT& left, const KVertexInputBinder& right ) :
        d_left ( left ),
        d_right ( right )
    {}

    template< class FunctorT >
    VPP_INLINE void for_each ( FunctorT& fOperation ) const
    {
        d_left.for_each ( fOperation );
        d_right.for_each ( fOperation );
    }

    static const size_t BINDER_COUNT = LeftT::BINDER_COUNT + 1;

    const LeftT& d_left;
    const KVertexInputBinder& d_right;
};

// -----------------------------------------------------------------------------

template< class LeftT >
VPP_INLINE TVertexInputBinderList< TVertexInputBinderList< LeftT > >
    operator, (
        const TVertexInputBinderList< LeftT >& left,
        const KVertexInputBinder& right )
{
    return TVertexInputBinderList< TVertexInputBinderList< LeftT > >( left, right );
}

// -----------------------------------------------------------------------------

VPP_INLINE TVertexInputBinderList< KVertexInputBinder >
    operator, ( const KVertexInputBinder& left, const KVertexInputBinder& right )
{
    return TVertexInputBinderList< KVertexInputBinder >( left, right );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< ETag TAG, template< vpp::ETag T > class TDef >
struct VertexStruct
{
};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
struct VertexStruct< GPU, TDef >
{
    VPP_INLINE VertexStruct()
    {
        PipelineConfig* pConfig = PipelineConfig::getInstance();
        pConfig->createVertexSource ( this, sizeof ( TDef< CPU > ), false );
    }
};

// -----------------------------------------------------------------------------

template< ETag TAG, template< vpp::ETag T > class TDef >
struct InstanceStruct
{
};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
struct InstanceStruct< GPU, TDef >
{
    VPP_INLINE InstanceStruct()
    {
        PipelineConfig* pConfig = PipelineConfig::getInstance();
        pConfig->createVertexSource ( this, sizeof ( TDef< CPU > ), true );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
struct inVertexData
{
    typedef TDef< GPU > GDefinition;

    VPP_INLINE KVertexInputBinder operator= ( const VertexBufferView& hVertexBufferView )
    {
        return KVertexInputBinder ( & d_data, hVertexBufferView );
    }

    template< typename MemberT >
    VPP_INLINE const auto& operator[]( MemberT GDefinition::* pMember ) const
    {
        const MemberT& member = d_data.*pMember;
        return member;
    }

    GDefinition d_data;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGINTVERTEX_HPP
