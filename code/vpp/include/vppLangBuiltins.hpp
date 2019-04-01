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

#ifndef INC_VPPLANGBUILTINS_HPP
#define INC_VPPLANGBUILTINS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGAGGREGATES_HPP
#include "vppLangAggregates.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

/*
    Jeszcze do builtinow:
    - SampleMask
*/

// -----------------------------------------------------------------------------
// 14.6. Built-In Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
namespace var {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class RValueT, spv::BuiltIn BUILTIN >
struct TStdInputBuiltIn : public KShaderScopedVariable
{
    typedef RValueT rvalue_type;
    typedef Bool comparison_type;

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        const KId variableId =
            pTranslator->registerSpecialInputVariable (
                this, rvalue_type::getType(), BUILTIN );

        return KId ( pTranslator->createLoad ( variableId ) );
    }

    VPP_DEFINE_SCALAR_OPERATORS1;
    VPP_DEFINE_SCALAR_OPERATORS2;

    VPP_INLINE rvalue_type operator* ( const rvalue_type& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs * rhs;
    }

    template< typename RightT >
    VPP_INLINE auto operator* ( const RightT& rhs ) const
    {
        const rvalue_type rValue1 = *this;
        const typename RightT::rvalue_type rValue2 = rhs;
        return rValue1 * rValue2;
    }

    template< typename IndexT >
    VPP_INLINE auto operator[] ( const IndexT& rhs ) const
    {
        const rvalue_type rValue1 = *this;
        return rValue1 [ rhs ];
    }
};

// -----------------------------------------------------------------------------

template< class RValueT, spv::BuiltIn BUILTIN >
struct TStdOutputBuiltIn : public KShaderScopedVariable
{
    typedef RValueT rvalue_type;

    VPP_INLINE void operator= ( const rvalue_type& value )
    {
        const KId variableId =
            KShaderTranslator::get()->registerSpecialOutputVariable (
                this, rvalue_type::getType(), BUILTIN );

        KShaderTranslator::get()->createStore ( value.id(), variableId );
    }
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

// Types of builtins are specified in (14.6).

typedef detail::TStdInputBuiltIn< Int, spv::BuiltInVertexIndex > inVertexIndex;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInSampleId > inSampleId;
typedef detail::TStdInputBuiltIn< Vec2, spv::BuiltInSamplePosition > inSamplePosition;
typedef detail::TStdInputBuiltIn< Vec2, spv::BuiltInPointCoord > inPointCoord;
typedef detail::TStdInputBuiltIn< Vec4, spv::BuiltInFragCoord > inFragCoord;
typedef detail::TStdInputBuiltIn< Bool, spv::BuiltInFrontFacing > inFrontFacing;
typedef detail::TStdInputBuiltIn< Bool, spv::BuiltInHelperInvocation > inHelperInvocation;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInPatchVertices > inPatchVertices;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInInvocationId > inInvocationId;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInInstanceIndex > inInstanceIndex;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInLayer > inLayer;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInViewportIndex > inViewportIndex;
typedef detail::TStdInputBuiltIn< Int, spv::BuiltInPrimitiveId > inPrimitiveId;
typedef detail::TStdInputBuiltIn< Vec3, spv::BuiltInTessCoord > inTessCoord;
typedef detail::TStdInputBuiltIn< IVec3, spv::BuiltInWorkgroupId > inWorkgroupId;
typedef detail::TStdInputBuiltIn< IVec3, spv::BuiltInNumWorkgroups > inNumWorkgroups;
typedef detail::TStdInputBuiltIn< IVec3, spv::BuiltInLocalInvocationId > inLocalInvocationId;
typedef detail::TStdInputBuiltIn< IVec3, spv::BuiltInGlobalInvocationId > inGlobalInvocationId;

typedef detail::TStdOutputBuiltIn< Int, spv::BuiltInLayer > outLayer;
typedef detail::TStdOutputBuiltIn< Int, spv::BuiltInViewportIndex > outViewportIndex;
typedef detail::TStdOutputBuiltIn< Int, spv::BuiltInPrimitiveId > outPrimitiveId;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct inWorkgroupSize : public KShaderScopedVariable
{
    // (14.6) - WorkgroupSize must be ivec3 type
    typedef IVec3 rvalue_type;

    VPP_INLINE inWorkgroupSize (
        const SLocalGroupSize& localSize,
        spv::Function* pFunction ) :
            d_pFunction ( pFunction ),
            d_localSize ( localSize ),
            d_id ( 0 )
    {}

    VPP_DLLAPI operator rvalue_type() const;

    template< typename IndexT >
    VPP_INLINE auto operator[] ( IndexT&& idx ) const
    {
        rvalue_type rv = *this;
        return rv [ idx ];
    }

private:
    spv::Function* d_pFunction;
    const SLocalGroupSize d_localSize;
    mutable KId d_id;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct outFragDepth : public KShaderScopedVariable
{
    // (14.6) - FragDepth must be float type
    typedef Float rvalue_type;

    VPP_INLINE outFragDepth ( spv::Function* pFunction ) :
        d_pFunction ( pFunction )
    {}

    VPP_INLINE void operator= ( const rvalue_type& value )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeDepthReplacing );

        const KId variableId =
            pTranslator->registerSpecialOutputVariable (
                this, rvalue_type::getType(), spv::BuiltInFragDepth );

        pTranslator->createStore ( value.id(), variableId );
    }

private:
    spv::Function* d_pFunction;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< spv::BuiltIn BUILTIN, int SIZE, bool OUTPUT >
struct TTessellationLevel : public KShaderScopedVariable
{
    typedef Float item_type;
    typedef Accessor< item_type, ! OUTPUT, OUTPUT > return_type;

    VPP_INLINE TTessellationLevel() :
        d_id ( 0 )
    {}

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId (
            pTranslator->makeArrayType ( 
                Float::getType(), pTranslator->makeIntConstant ( SIZE ), 0 ) );
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( ! d_id )
        {
            if ( OUTPUT )
                d_id = pTranslator->registerSpecialOutputVariable ( this, getType(), BUILTIN );
            else
                d_id = pTranslator->registerSpecialInputVariable ( this, getType(), BUILTIN );

            pTranslator->addDecoration ( d_id, spv::DecorationPatch );
        }

        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( d_id );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return return_type ( pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( SIZE ) ) );
    }

    VPP_INLINE int size() const
    {
        return SIZE;
    }

private:
    KId d_id;
};

// -----------------------------------------------------------------------------

typedef TTessellationLevel< spv::BuiltInTessLevelOuter, 4, false > inTessLevelOuter;
typedef TTessellationLevel< spv::BuiltInTessLevelInner, 2, false > inTessLevelInner;
typedef TTessellationLevel< spv::BuiltInTessLevelOuter, 4, true > outTessLevelOuter;
typedef TTessellationLevel< spv::BuiltInTessLevelInner, 2, true > outTessLevelInner;

// -----------------------------------------------------------------------------
// 14.1.1. Built-in per-vertex Interface Block (gl_perVertex)
// These blocks are single or arrayed, depending on shader/direction.
// That is because shaders read/write either single vertex, or group of vertices.
// Here are the rules governing this: 14.1.3. Interface Matching.
// -----------------------------------------------------------------------------

class KVertexDataBlockBase : public KShaderScopedVariable
{
public:
    VPP_INLINE KVertexDataBlockBase() :
        d_size ( 1 ),
        d_clipPlanes ( 0 ),
        d_cullPlanes ( 0 )
    {}

    VPP_INLINE void resize ( int newSize )
    {
        d_size = newSize;
    }

protected:
    VPP_DLLAPI KId getArrayType() const;
    VPP_DLLAPI KId getSingleType() const;

protected:
    int d_size;
    int d_clipPlanes;
    int d_cullPlanes;
};

// -----------------------------------------------------------------------------

template< typename ParentT, typename FieldT, bool OUTPUT >
class TVertexDataBlockMember
{
public:
    typedef typename FieldT::rvalue_type rvalue_type;
    typedef typename vpp::detail::TComparisonResult< FieldT >::type comparison_type;
    typedef TVertexDataBlockMember< ParentT, FieldT, OUTPUT > self_type;

    VPP_INLINE TVertexDataBlockMember ( int index, ParentT* pParent ) :
        d_memberIndex ( index ),
        d_pParent ( pParent )
    {}

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( ! OUTPUT, "This variable is write-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        d_pParent->initAccessChain();
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( d_memberIndex ) );

        return rvalue_type ( KId (
            pTranslator->accessChainLoad (
                pTranslator->getPrecision(), rvalue_type::getType() ) ) );
    }

    VPP_INLINE const TVertexDataBlockMember& operator= (
        const TVertexDataBlockMember< ParentT, FieldT, OUTPUT >& rhs )
    {
        static_assert ( OUTPUT, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        d_pParent->initAccessChain();
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( d_memberIndex ) );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    template< typename RightT >
    VPP_INLINE const TVertexDataBlockMember& operator= ( const RightT& rhs )
    {
        static_assert ( OUTPUT, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        d_pParent->initAccessChain();
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( d_memberIndex ) );
        pTranslator->accessChainStore ( value.id() );
        return *this;
    }

    VPP_DEFINE_ACCESSOR_OPERATORS;

    template< typename IndexT >
    VPP_INLINE auto operator[] ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        d_pParent->initAccessChain();
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( d_memberIndex ) );

        Accessor< self_type, ! OUTPUT, OUTPUT > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

private:
    int d_memberIndex;
    ParentT* d_pParent;
};

// -----------------------------------------------------------------------------

template< typename ParentT, typename FieldT, bool OUTPUT >
class TVertexDataBlockArrayMember
{
public:
    typedef FieldT item_type;
    typedef Accessor< item_type, ! OUTPUT, OUTPUT > return_type;

    VPP_INLINE TVertexDataBlockArrayMember (
        int index,
        ParentT* pParent,
        int length ) :
            d_memberIndex ( index ),
            d_pParent ( pParent ),
            d_length ( length )
    {}

    VPP_INLINE void resize ( int newLength )
    {
        d_length = newLength;
    }

    VPP_INLINE void setMemberIndex ( int newMemberIndex )
    {
        d_memberIndex = newMemberIndex;
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        d_pParent->initAccessChain();
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( d_memberIndex ) );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return return_type ( pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) const { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_length ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_length;
    }

private:
    int d_memberIndex;
    ParentT* d_pParent;
    int d_length;
};

// -----------------------------------------------------------------------------

template< bool OUTPUT >
class TVertexDataArrayedBlock : public KVertexDataBlockBase
{
public:
    class item_type
    {
    public:
        VPP_INLINE item_type (
            const spv::Builder::AccessChain& ac,
            int clipPlanes,
            int cullPlanes ) :
                d_accessChain ( ac ),
                position ( 0u, this ),
                pointSize ( 1u, this ),
                clipDistance ( 2u, this, clipPlanes ),
                cullDistance ( ( clipPlanes != 0 ? 3u : 2u ), this, cullPlanes )
        {}

        VPP_INLINE void initAccessChain()
        {
            KShaderTranslator::get()->setAccessChain ( d_accessChain );
        }

        TVertexDataBlockMember< item_type, Vec4, OUTPUT > position;
        TVertexDataBlockMember< item_type, Float, OUTPUT > pointSize;
        TVertexDataBlockArrayMember< item_type, Float, OUTPUT > clipDistance;
        TVertexDataBlockArrayMember< item_type, Float, OUTPUT > cullDistance;

    private:
        spv::Builder::AccessChain d_accessChain;
    };

    template< typename IndexT >
    item_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( ! varInfo )
        {
            varInfo.d_typeId = getArrayType();

            varInfo.d_variableId = KId (
                pTranslator->createVariable (
                    OUTPUT ? spv::StorageClassOutput : spv::StorageClassInput,
                    varInfo.d_typeId ) );

            pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        }

        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( varInfo.d_variableId );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return item_type ( pTranslator->getAccessChain(), d_clipPlanes, d_cullPlanes );
    }

    VPP_INLINE item_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE item_type operator[]( unsigned int index ) const { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

    VPP_INLINE int getClipPlanes() const
    {
        return d_clipPlanes;
    }

    VPP_INLINE void setClipPlanes ( int v )
    {
        d_clipPlanes = v;
    }

    VPP_INLINE int getCullPlanes() const
    {
        return d_cullPlanes;
    }

    VPP_INLINE void setCullPlanes ( int v )
    {
        d_cullPlanes = v;
    }
};

// -----------------------------------------------------------------------------

template< bool OUTPUT >
class TVertexDataSingleBlock : public KVertexDataBlockBase
{
public:
    typedef TVertexDataSingleBlock< OUTPUT > self_type;

    VPP_INLINE TVertexDataSingleBlock() :
        position ( 0u, this ),
        pointSize ( 1u, this ),
        clipDistance ( 2u, this, 0 ),
        cullDistance ( 3u, this, 0 )
    {}

    VPP_INLINE int getClipPlanes() const
    {
        return d_clipPlanes;
    }

    VPP_INLINE void setClipPlanes ( int v )
    {
        clipDistance.resize ( v );
        d_clipPlanes = v;
    }

    VPP_INLINE int getCullPlanes() const
    {
        return d_cullPlanes;
    }

    VPP_INLINE void setCullPlanes ( int v )
    {
        cullDistance.resize ( v );
        d_cullPlanes = v;
    }

    VPP_INLINE void initAccessChain()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( ! varInfo )
        {
            varInfo.d_typeId = getSingleType();

            varInfo.d_variableId = KId (
                pTranslator->createVariable (
                    OUTPUT ? spv::StorageClassOutput : spv::StorageClassInput,
                    varInfo.d_typeId ) );

            pTranslator->registerInputOutputVariable ( varInfo.d_variableId );

            if ( d_clipPlanes == 0 )
                cullDistance.setMemberIndex ( 2u );
        }

        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( varInfo.d_variableId );
    }

    TVertexDataBlockMember< self_type, Vec4, OUTPUT > position;
    TVertexDataBlockMember< self_type, Float, OUTPUT > pointSize;
    TVertexDataBlockArrayMember< self_type, Float, OUTPUT > clipDistance;
    TVertexDataBlockArrayMember< self_type, Float, OUTPUT > cullDistance;
};

// -----------------------------------------------------------------------------

typedef TVertexDataArrayedBlock< false > inVertices;
typedef TVertexDataArrayedBlock< true > outVertices;
typedef TVertexDataSingleBlock< false > inVertex;
typedef TVertexDataSingleBlock< true > outVertex;

// -----------------------------------------------------------------------------
} // namespace var
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGBUILTINS_HPP
