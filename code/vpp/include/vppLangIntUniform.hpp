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

#ifndef INC_VPPLANGINTUNIFORM_HPP
#define INC_VPPLANGINTUNIFORM_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGINTBASE_HPP
#include "vppLangIntBase.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ShaderT, class HostT >
struct TArrayStride
{
};

// -----------------------------------------------------------------------------

template< class HostT >
struct TArrayStride< TRValue< HostT >, HostT >
{
    VPP_INLINE int operator()() const
    {
        // A scalar of size N has a base alignment of N.
        return static_cast< int >( sizeof ( HostT ) );
    }
};

// -----------------------------------------------------------------------------

template< class ShaderT, class HostT, size_t SIZE >
struct TArrayStride< TRVector< ShaderT, SIZE >, HostT >
{
    VPP_INLINE int operator()() const
    {
        typedef typename ShaderT::scalar_type scalar_type;
        static const int ISIZE = static_cast< int >( sizeof ( scalar_type ) );

        // A two-component vector, with components of size N, has a base alignment of 2 N.
        // A three- or four-component vector, with components of size N, has a base alignment of 4 N.
        const int NC = ( SIZE == 3 ? 4 : SIZE );
        
        static_assert (
            sizeof ( HostT ) == NC * ISIZE,
            "CPU-side vector size is incompatible with GPU vector size."
        );

        return NC * ISIZE;
    }
};

// -----------------------------------------------------------------------------

template< class ShaderT, class HostT, size_t COLS, size_t ROWS >
struct TArrayStride< TRMatrix< ShaderT, COLS, ROWS >, HostT >
{
    VPP_INLINE int operator()() const
    {
        typedef typename ShaderT::scalar_type scalar_type;
        typedef StructMemberTraits< HostT > member_traits;

        static_assert ( 
            ! member_traits::is_unknown,
            "Unknown CPU-side type in uniform array (define StructMemberTraits spec)" );

        return sizeof ( HostT );
    }
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/* 
   CPU-side shader data descriptor objects for uniform buffers.
   Use them to bind CPU data to GPU pipelines.

   inUniformBuffer
   ioBuffer
   inUniformBufferDyn
   ioBufferDyn
   inPushConstant

*/
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Spec 14.5.4 determines which types have Std430 layout.

// -----------------------------------------------------------------------------
// 13.1.7. Uniform Buffer
// -----------------------------------------------------------------------------

class inUniformBuffer : public detail::TDescriptorType<
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, UniformBufferView >
{
public:
    typedef detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, UniformBufferView > descriptor_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        UniformBufferView, UniformBufferView > assigner_type;

    typedef void rvalue_type;

    static const bool isReadOnly = true;
    static const bool isStd430 = false;
    static const spv::Decoration decoration = spv::DecorationBlock;
    static const spv::StorageClass storageClass = spv::StorageClassUniform;
    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inUniformBuffer ( std::uint32_t set = 0, int binding = -1 ) :
        descriptor_type ( set, binding, 1u )
    {}

    VPP_INLINE inUniformBuffer ( std::uint32_t set, int binding, std::uint32_t count ) :
        descriptor_type ( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const UniformBufferView& value )
    {
        return assigner_type ( d_id, d_set, d_binding, value );
    }

    VPP_INLINE void verify ( const Device& ) const
    {
    }
};

// -----------------------------------------------------------------------------
// 13.1.8. Storage Buffer
// -----------------------------------------------------------------------------

class ioBuffer : public detail::TDescriptorType<
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, StorageBufferView >
{
public:
    typedef detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, StorageBufferView > descriptor_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        StorageBufferView, StorageBufferView > assigner_type;

    typedef void rvalue_type;

    static const bool isReadOnly = false;
    static const bool isStd430 = true;
    static const spv::Decoration decoration = spv::DecorationBufferBlock;
    static const spv::StorageClass storageClass = spv::StorageClassUniform;
    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE ioBuffer ( std::uint32_t set = 0, int binding = -1 ) :
        descriptor_type ( set, binding, 1u )
    {}

    VPP_INLINE ioBuffer ( std::uint32_t set, int binding, std::uint32_t count ) :
        descriptor_type ( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const StorageBufferView& value )
    {
        return assigner_type ( d_id, d_set, d_binding, value );
    }

    VPP_INLINE void verify ( const Device& ) const
    {
    }
};

// -----------------------------------------------------------------------------
// 13.1.9. Dynamic Uniform Buffer
// -----------------------------------------------------------------------------

class inUniformBufferDyn : public detail::TDescriptorType<
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, UniformBufferView >
{
public:
    typedef detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, UniformBufferView > descriptor_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        UniformBufferView, UniformBufferView > assigner_type;

    typedef void rvalue_type;

    static const bool isReadOnly = true;
    static const bool isStd430 = false;
    static const spv::Decoration decoration = spv::DecorationBlock;
    static const spv::StorageClass storageClass = spv::StorageClassUniform;
    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inUniformBufferDyn ( std::uint32_t set = 0, int binding = -1 ) :
        descriptor_type ( set, binding, 1u )
    {}

    VPP_INLINE inUniformBufferDyn ( std::uint32_t set, int binding, std::uint32_t count ) :
        descriptor_type ( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const UniformBufferView& value )
    {
        return assigner_type ( d_id, d_set, d_binding, value );
    }

    VPP_INLINE void verify ( const Device& ) const
    {
    }
};

// -----------------------------------------------------------------------------
// 13.1.10. Dynamic Storage Buffer
// -----------------------------------------------------------------------------

class ioBufferDyn : public detail::TDescriptorType<
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, StorageBufferView >
{
public:
    typedef detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, StorageBufferView > descriptor_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
        StorageBufferView, StorageBufferView > assigner_type;

    typedef void rvalue_type;

    static const bool isReadOnly = false;
    static const bool isStd430 = true;
    static const spv::Decoration decoration = spv::DecorationBufferBlock;
    static const spv::StorageClass storageClass = spv::StorageClassUniform;
    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE ioBufferDyn ( std::uint32_t set = 0, int binding = -1 ) :
        descriptor_type ( set, binding, 1u )
    {}

    VPP_INLINE ioBufferDyn ( std::uint32_t set, int binding, std::uint32_t count ) :
        descriptor_type ( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const StorageBufferView& value )
    {
        return assigner_type ( d_id, d_set, d_binding, value );
    }

    VPP_INLINE void verify ( const Device& ) const
    {
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Push constants aren't uniform buffers per se, but they are handled in
// very similar manner, only requiring calling push.

template< template< vpp::ETag TAG > class TDef >
class inPushConstant
{
public:
    inPushConstant();

    static const bool isReadOnly = true;
    static const bool isStd430 = true;
    static const spv::Decoration decoration = spv::DecorationBlock;
    static const spv::StorageClass storageClass = spv::StorageClassPushConstant;
    static const bool isDescriptorArrayAllowed = false;

    typedef TDef< CPU > DataBlock;

    VPP_INLINE void addStageFlag ( std::uint32_t f ) const
    {
        d_pConfig->getConstant ( d_id ).stageFlags |= f;
    }

    VPP_INLINE void cmdPush ( CommandBuffer currentBuffer )
    {
        const VkPushConstantRange& constDef = d_pConfig->getConstant ( d_id );

        ::vkCmdPushConstants (
            currentBuffer.handle(), d_pConfig->getCompiledHandle(),
            constDef.stageFlags,
            0,
            static_cast< std::uint32_t >( sizeof ( d_data ) ),
            & d_data );
    }

    template< typename ValueT >
    VPP_INLINE void cmdPush ( CommandBuffer currentBuffer, const ValueT& field )
    {
        const VkPushConstantRange& constDef = d_pConfig->getConstant ( d_id );

        ::vkCmdPushConstants (
            currentBuffer.handle(), d_pConfig->getCompiledHandle(),
            constDef.stageFlags,
            reinterpret_cast< const char* >( & field )
                - reinterpret_cast< const char* >( & d_data ),
            static_cast< std::uint32_t >( sizeof ( field ) ),
            & field );
    }

    VPP_INLINE void cmdPush()
    {
        CommandBuffer currentBuffer (
            RenderingCommandContext::getCommandBufferHandle() );

        cmdPush ( currentBuffer );
    }

    template< typename ValueT >
    VPP_INLINE void cmdPush ( const ValueT& field )
    {
        CommandBuffer currentBuffer (
            RenderingCommandContext::getCommandBufferHandle() );

        push ( currentBuffer, field );
    }

    VPP_INLINE std::uint32_t set() const
    {
        return 0;
    }

    VPP_INLINE std::uint32_t binding() const
    {
        return 0;
    }

    VPP_INLINE DataBlock& data()
    {
        return d_data;
    }

    VPP_INLINE void verify ( const Device& hDevice ) const
    {
        const auto& properties = hDevice.physical().properties();

        if ( sizeof ( DataBlock ) > properties.limits.maxPushConstantsSize )
        {
            KExceptionThrower ex;
            ex.raisePushConstantTooBig();
        }
    }

private:
    PipelineConfig* d_pConfig;
    std::uint32_t d_id;

    DataBlock d_data;
};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
VPP_INLINE inPushConstant< TDef > :: inPushConstant() :
    d_pConfig ( PipelineConfig::getInstance() ),
    d_id ( d_pConfig->createNewConstant() )
{
    VkPushConstantRange& resInfo = d_pConfig->getConstant ( d_id );
    resInfo.offset = 0;
    resInfo.size = sizeof ( d_data );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// GPU-side structural type definitions.
// Use them to define dual (CPU/GPU) structure types for uniform buffers.
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< ETag TAG, class HostT >
struct UniformFld {};

// -----------------------------------------------------------------------------

template< class HostT >
struct UniformFld< CPU, HostT > : public HostT
{
    typedef StructMemberTraits< HostT > member_traits;

    static_assert ( 
        ! member_traits::is_unknown,
        "Unknown member type in shader structure (define StructMemberTraits spec)" );

    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( const HostT& value ) : HostT ( value ) {}
};

// -----------------------------------------------------------------------------

template<>
struct UniformFld< CPU, int >
{
    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( int value ) : d_value ( value ) {}
    VPP_INLINE operator int& () { return d_value; }
    VPP_INLINE operator int () const { return d_value; }
    int d_value;
};

// -----------------------------------------------------------------------------

template<>
struct UniformFld< CPU, unsigned int >
{
    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( unsigned int value ) : d_value ( value ) {}
    VPP_INLINE operator unsigned int& () { return d_value; }
    VPP_INLINE operator unsigned int () const { return d_value; }
    unsigned int d_value;
};

// -----------------------------------------------------------------------------

template<>
struct UniformFld< CPU, float >
{
    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( float value ) : d_value ( value ) {}
    VPP_INLINE operator float& () { return d_value; }
    VPP_INLINE operator float () const { return d_value; }
    float d_value;
};

// -----------------------------------------------------------------------------

template<>
struct UniformFld< CPU, double >
{
    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( double value ) : d_value ( value ) {}
    VPP_INLINE operator double& () { return d_value; }
    VPP_INLINE operator double () const { return d_value; }
    double d_value;
};

// -----------------------------------------------------------------------------

template< typename C1, typename C2, typename C3, typename C4, typename C5 >
struct UniformFld< CPU, format< C1, C2, C3, C4, C5 > >
{
    typedef typename format< C1, C2, C3, C4, C5 >::data_type data_type;

    VPP_INLINE UniformFld() {}
    VPP_INLINE UniformFld ( const data_type& value ) : d_value ( value ) {}
    VPP_INLINE operator data_type& () { return d_value; }
    VPP_INLINE operator const data_type& () const { return d_value; }
    data_type d_value;
};

// -----------------------------------------------------------------------------

template< class HostT >
struct UniformFld< GPU, HostT >
{
public:
    typedef StructMemberTraits< HostT > member_traits;

    static_assert ( 
        ! member_traits::is_unknown,
        "Unknown member type in shader structure (define StructMemberTraits spec)" );

    typedef typename member_traits::rvalue_type rvalue_type;
    typedef typename detail::TComparisonResult< rvalue_type >::type comparison_type;

    UniformFld();

    VPP_INLINE unsigned int index() const
    {
        return d_memberIndex;
    }

private:
    const UniformFld& operator= ( const UniformFld& rhs ) = delete;

private:
    unsigned int d_memberIndex;
};

// -----------------------------------------------------------------------------

template< class HostT >
UniformFld< GPU, HostT > :: UniformFld()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        d_memberIndex = structInfo.d_currentMember++;
    }
}

// -----------------------------------------------------------------------------

template< class HostT >
struct UniformFld< DEF, HostT >
{
public:
    typedef StructMemberTraits< HostT > member_traits;

    static_assert ( 
        ! member_traits::is_unknown,
        "Unknown member type in shader structure (define StructMemberTraits spec)" );

    typedef typename member_traits::rvalue_type rvalue_type;
    typedef typename detail::TComparisonResult< rvalue_type >::type comparison_type;
    typedef typename member_traits::data_type data_type;

    UniformFld();

private:
    const UniformFld& operator= ( const UniformFld& rhs ) = delete;

private:
    data_type d_value;
};

// -----------------------------------------------------------------------------

template< class HostT >
UniformFld< DEF, HostT > :: UniformFld()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        
        if ( ! structInfo.d_bDefined )
        {
            const int byteOffset = static_cast< int >(
                reinterpret_cast< const unsigned char* >( this ) - structInfo.d_pBase );

            if ( member_traits::has_member_info )
            {
                if ( member_traits::is_matrix ) 
                    structInfo.d_memberInfos.emplace_back (
                        KId ( rvalue_type::getType() ),
                        byteOffset,
                        member_traits::matrix_stride,
                        member_traits::is_col_major
                    );
                else
                    structInfo.d_memberInfos.emplace_back (
                        KId ( rvalue_type::getType() ), byteOffset );
            }
            else
                structInfo.d_memberInfos.emplace_back ( KId ( rvalue_type::getType() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< ETag TAG, template< vpp::ETag T > class TDef >
class UniformStruct {};

template< template< vpp::ETag TAG > class TDef >
class UniformStruct< CPU, TDef > {};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
class UniformStruct< GPU, TDef >
{
public:
    VPP_INLINE UniformStruct()
    {
        if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        {
            KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
            structInfo.d_currentMember = 0;
        }
    }
};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef >
class UniformStruct< DEF, TDef >
{
public:
    VPP_INLINE UniformStruct()
    {
        typedef TDef< DEF > definition_type;
        const std::type_index index ( typeid ( definition_type ) );

        if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        {
            pTranslator->pushStructType ( index, pTranslator->getStructDecoration() );
            KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
            structInfo.d_pBase = reinterpret_cast< const unsigned char* >( this );
        }
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// GPU-side uniform buffer accessors.
// Declare them locally in shaders to gain access to data referred by shader data descriptors.
// Mostly use Uniform definitions as types.
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class KUniformAccess
{
protected:
    VPP_DLLAPI KUniformAccess ( spv::Decoration decoration );

    VPP_DLLAPI KId init (
        spv::Decoration decoration,
        spv::StorageClass storageClass,
        std::uint32_t set,
        std::uint32_t binding );

    VPP_DLLAPI std::pair< KId, KId > init (
        spv::Decoration decoration,
        spv::StorageClass storageClass,
        std::uint32_t set,
        std::uint32_t binding,
        std::uint32_t count );

    VPP_DLLAPI std::pair< KId, KId > init (
        spv::Decoration decoration,
        spv::StorageClass storageClass,
        std::uint32_t set,
        std::uint32_t binding,
        std::uint32_t arraySize,
        std::uint32_t stride );

    VPP_DLLAPI std::pair< KId, KId > init (
        const KId& itemTypeId,
        spv::Decoration decoration,
        spv::StorageClass storageClass,
        std::uint32_t set,
        std::uint32_t binding,
        std::uint32_t arraySize,
        std::uint32_t stride );
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

// Single structure buffer. Good for e.g. global frame data or a push constant.

template< template< vpp::ETag TAG > class TDef, class BufferT >
class UniformVar : public detail::KUniformAccess
{
public:
    typedef TDef< DEF > Definition;
    typedef TDef< GPU > GDefinition;

    static const bool isReadOnly = BufferT::isReadOnly;

    VPP_INLINE UniformVar ( BufferT& buf ) :
        detail::KUniformAccess ( BufferT::decoration ),
        d_id ( 0 )
    {
        buf.verify ( KShaderTranslator::get()->getDevice() );
        buf.addStageFlag ( KShaderTranslator::get()->getStage() );
        d_id = init ( BufferT::decoration, BufferT::storageClass, buf.set(), buf.binding() );
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }
    
    template< typename MemberT >
    VPP_INLINE Accessor< typename MemberT::rvalue_type, true, ! isReadOnly >
        operator[]( MemberT GDefinition::* pMember ) const
    {
        typedef typename MemberT::rvalue_type rvalue_type;
        typedef Accessor< rvalue_type, true, ! isReadOnly > result_type;

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );

        const MemberT& member = d_struct.*pMember;
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( member.index() ) );

        return result_type ( pTranslator->getAccessChain() );
    }

private:
    Definition d_layout;
    GDefinition d_struct;
    KId d_id;
};

// -----------------------------------------------------------------------------

template< template< vpp::ETag TAG > class TDef, class BufferT >
class UniformVar< TDef, arrayOf< BufferT > > : public detail::KUniformAccess
{
public:
    static const bool isReadOnly = BufferT::isReadOnly;

    typedef TDef< DEF > Definition;
    typedef TDef< GPU > GDefinition;

    typedef StructAccessor< GDefinition, true, ! isReadOnly > return_type;

    VPP_INLINE UniformVar ( arrayOf< BufferT >& bufs ) :
        detail::KUniformAccess ( BufferT::decoration ),
        d_id ( 0 ),
        d_typeId ( 0 )
    {
        const BufferT& buf = bufs.resource();
        buf.verify ( KShaderTranslator::get()->getDevice() );
        buf.addStageFlag ( KShaderTranslator::get()->getStage() );
        const auto& ids = 
            init ( BufferT::decoration, BufferT::storageClass, buf.set(), buf.binding(), bufs.count() );
        d_id = ids.first;
        d_typeId = ids.second;
        d_count = bufs.count();
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }

    VPP_INLINE KId getType()
    {
        return d_typeId;
    }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_count ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_count;
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        return return_type ( & d_struct, pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) { return getItem ( index ); }

private:
    Definition d_layout;
    GDefinition d_struct;
    KId d_id;
    KId d_typeId;
    int d_count;
};

// -----------------------------------------------------------------------------

// Array of structures. Because the standard requires uniform buffers to be
// structures, this really defines a structure with single member of array type,
// and this array holds structural typed items. See e.g. SPIR-V spec 3.20
// (Block/BufferBlock requirements) or Vulkan spec: 13.1.7 or 13.1.8 examples.
// The indexing operator transparently handles extra level of indirection.
// Array size can be specified optionally in the constructor - if omitted,
// a runtime-sized array will be assumed. The size will be equal to bound
// buffer size. Use Size() to read it.

template< template< vpp::ETag TAG > class TDef, class BufferT >
class UniformArray : public detail::KUniformAccess
{
public:
    typedef TDef< DEF > Definition;
    typedef TDef< GPU > GDefinition;

    static const bool isReadOnly = BufferT::isReadOnly;
    typedef StructAccessor< GDefinition, true, ! isReadOnly > item_type;

    VPP_INLINE UniformArray ( BufferT& buf, int size, int stride = 0 ) :
        detail::KUniformAccess ( BufferT::decoration ),
        d_id ( 0 ),
        d_typeId ( 0 ),
        d_size ( size )
    {
        buf.addStageFlag ( KShaderTranslator::get()->getStage() );
        buf.verify ( KShaderTranslator::get()->getDevice() );
        const auto& ids = init (
            BufferT::decoration, BufferT::storageClass, buf.set(), buf.binding(),
            d_size, ( stride ? stride : sizeof ( Definition ) ) );
        d_id = ids.first;
        d_typeId = ids.second;
    }

    VPP_INLINE UniformArray ( BufferT& buf ) :
        UniformArray ( buf, 0, 0 )
    {
        static_assert (
            BufferT::decoration == spv::DecorationBufferBlock,
            "Dynamically-sized arrays are allowed only for storage buffers (ioBuffer)" );
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }

    VPP_INLINE KId getType()
    {
        return d_typeId;
    }
    
    template< typename IndexT >
    VPP_INLINE item_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( 0 ) );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return item_type (
            const_cast< GDefinition* >( & d_struct ),
            pTranslator->getAccessChain() );
    }

    VPP_INLINE item_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( unsigned int index ) const { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( d_size )
            return Int ( KId ( pTranslator->makeIntConstant ( d_size ) ) );
        else
            return Int ( KId ( pTranslator->createArrayLength ( d_id, 0 ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

    static_assert (
        ( ( sizeof ( Definition ) & 15 ) == 0 ),
        "Structure inside uniform array must have size being multiplicity of 16 bytes. "
        "Please add dummy members for padding." );

private:
    Definition d_layout;
    GDefinition d_struct;
    KId d_id;
    KId d_typeId;
    int d_size;
};

// -----------------------------------------------------------------------------
// Array of scalars, vectors or matrices. Otherwise the same as UniformArray.

template< class HostT, class BufferT >
class UniformSimpleArray : public detail::KUniformAccess
{
public:
    typedef StructMemberTraits< HostT > member_traits;

    static_assert ( 
        ! member_traits::is_unknown,
        "Unknown value type in shader structure (define StructMemberTraits spec)" );

    typedef typename member_traits::rvalue_type item_type;
    typedef UniformSimpleArray< HostT, BufferT > rvalue_type;

    static const bool isReadOnly = BufferT::isReadOnly;
    static const bool indexable = true;

    VPP_INLINE UniformSimpleArray ( BufferT& buf, int size, int stride = 0 ) :
        detail::KUniformAccess ( BufferT::decoration ),
        d_id ( 0 ),
        d_typeId ( 0 ),
        d_size ( size )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        buf.addStageFlag ( pTranslator->getStage() );
        buf.verify ( pTranslator->getDevice() );
        const int defaultStride = detail::TArrayStride< item_type, HostT >()();
        const auto& ids = init (
            item_type::getType(),
            BufferT::decoration, BufferT::storageClass, buf.set(), buf.binding(),
            d_size, ( stride ? stride : defaultStride ) );
        d_id = ids.first;
        d_typeId = ids.second;
    }

    VPP_INLINE UniformSimpleArray ( BufferT& buf ) :
        UniformSimpleArray ( buf, 0, 0 )
    {
        static_assert (
            BufferT::decoration == spv::DecorationBufferBlock,
            "Dynamically-sized arrays are allowed only for storage buffers (ioBuffer)" );
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }

    VPP_INLINE KId getType()
    {
        return d_typeId;
    }
    
    template< typename IndexT >
    VPP_INLINE auto operator[] ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( 0 ) );

        Accessor< UniformSimpleArray, true, ! isReadOnly > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    VPP_INLINE Int Size() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( d_size )
            return Int ( KId ( pTranslator->makeIntConstant ( d_size ) ) );
        else
            return Int ( KId ( pTranslator->createArrayLength ( d_id, 0 ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

private:
    KId d_id;
    KId d_typeId;
    int d_size;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGINTUNIFORM_HPP
