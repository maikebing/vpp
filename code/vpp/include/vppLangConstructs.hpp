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

#ifndef INC_VPPLANGCONSTRUCTS_HPP
#define INC_VPPLANGCONSTRUCTS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGVECTORTYPES_HPP
#include "vppLangVectorTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class KFunction
{
public:
    VPP_INLINE KFunction ( KId returnTypeId ) :
        d_pFunction ( 0 ),
        d_returnTypeId ( returnTypeId ),
        d_pParentBlock ( 0 )
    {}

    spv::Function* d_pFunction;
    KId d_returnTypeId;
    spv::Block* d_pParentBlock;
};

// -----------------------------------------------------------------------------

class KParameter
{
public:
    VPP_INLINE KParameter ( KId typeId ) :
        d_typeId ( typeId ),
        d_variableId ( 0 )
    {}

    KId d_typeId;
    KId d_variableId;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

VPP_INLINE void If ( Bool v )
{
    KShaderTranslator::get()->pushIf ( v );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Else()
{
    KShaderTranslator::get()->makeElse();
}

// -----------------------------------------------------------------------------

VPP_INLINE void Fi()
{
    KShaderTranslator::get()->popIf();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE void Do()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    KShaderTranslator::LoopBlocks& blocks = pBuilder->pushLoop();

    pBuilder->createBranch ( & blocks.head );
    pBuilder->setBuildPoint ( & blocks.head );

    pBuilder->createLoopMerge (
        & blocks.merge, & blocks.continue_target, spv::LoopControlMaskNone );
        
    spv::Block& test = pBuilder->makeNewBlock();
    pBuilder->createBranch ( & test );
    pBuilder->setBuildPoint ( & test );
}

// -----------------------------------------------------------------------------

VPP_INLINE void While ( Bool v )
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    KShaderTranslator::LoopBlocks& blocks = pBuilder->currentLoop();

    pBuilder->createConditionalBranch ( v.id(), & blocks.body, & blocks.merge );
    pBuilder->setBuildPoint ( & blocks.body );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Od()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    KShaderTranslator::LoopBlocks& blocks = pBuilder->currentLoop();

    pBuilder->createBranch ( & blocks.continue_target );
    pBuilder->setBuildPoint ( & blocks.continue_target );
    pBuilder->createBranch ( & blocks.head );
    pBuilder->setBuildPoint ( & blocks.merge );
    pBuilder->closeLoop();
    pBuilder->popLoop();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE void For ( VInt& variable, const Int& begin, const Int& end, const Int& step = Int(1) )
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    variable = begin;
    Do();
    While ( variable < end );
    pBuilder->pushFor ( variable.id(), step.id(), false );
}

// -----------------------------------------------------------------------------

VPP_INLINE void For ( VUInt& variable, const UInt& begin, const UInt& end, const UInt& step = UInt(1) )
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    variable = begin;
    Do();
    While ( variable < end );
    pBuilder->pushFor ( variable.id(), step.id(), true );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Rof()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    const auto& forRange = pBuilder->currentFor();

    if ( forRange.d_bUnsigned )
    {
        VUInt var ( KId ( forRange.d_varId ) );
        const UInt step ( KId ( forRange.d_stepId ) );
        var += step;
    }
    else
    {
        VInt var ( KId ( forRange.d_varId ) );
        const Int step ( KId ( forRange.d_stepId ) );
        var += step;
    }

    Od();

    pBuilder->popFor();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE void Switch ( Int expr )
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    pBuilder->pushSwitch ( expr.id() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Case ( int value )
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    pBuilder->makeSwitchCase ( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Default()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    pBuilder->makeSwitchDefault();
}

// -----------------------------------------------------------------------------

VPP_INLINE void Break()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    pBuilder->addSwitchBreak();
}

// -----------------------------------------------------------------------------

VPP_INLINE void EndSwitch()
{
    KShaderTranslator* pBuilder = KShaderTranslator::get();
    pBuilder->popSwitch();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ReturnType, typename... Args >
struct Function : public detail::KFunction
{
    VPP_INLINE Function ( const char* pName = "unnamedFunction" ) :
        KFunction ( ReturnType::getType() )
    {
        KShaderTranslator::get()->registerNewFunction ( this, pName );
    }

    typedef typename ReturnType::rvalue_type RealReturnType;

    static VPP_INLINE void collectArgs (
        std::vector< spv::Id >* pResult )
    {
    }

    template< typename FirstT >
    static void collectArgs (
        std::vector< spv::Id >* pResult, FirstT first )
    {
        pResult->push_back ( first.id() );
    }

    template< typename FirstT, typename... ArgTypes >
    static void collectArgs (
        std::vector< spv::Id >* pResult, FirstT first, ArgTypes... args )
    {
        collectArgs ( pResult, first );
        collectArgs ( pResult, args... );
    }

    VPP_INLINE RealReturnType operator()( typename Args::rvalue_type ... args )
    {
        std::vector< spv::Id > argIds;
        collectArgs ( & argIds, args... );

        return RealReturnType ( KId (
            KShaderTranslator::get()->createFunctionCall ( d_pFunction, argIds ) ) );
    }
};

// -----------------------------------------------------------------------------

#define VPP_DEFINE_PARAM_OPERATORS \
    VPP_DEFINE_SCALAR_OPERATOR2( + ) \
    VPP_DEFINE_SCALAR_OPERATOR2( - ) \
    VPP_DEFINE_SCALAR_OPERATOR2( * ) \
    VPP_DEFINE_SCALAR_OPERATOR2( / ) \
    VPP_DEFINE_SCALAR_OPERATOR2( % ) \
    VPP_DEFINE_SCALAR_OPERATOR2( << ) \
    VPP_DEFINE_SCALAR_OPERATOR2( >> ) \
    VPP_DEFINE_SCALAR_OPERATOR2( | ) \
    VPP_DEFINE_SCALAR_OPERATOR2( ^ ) \
    VPP_DEFINE_SCALAR_OPERATOR2( & ) \
    VPP_DEFINE_SCALAR_OPERATOR1( - ) \
    VPP_DEFINE_SCALAR_OPERATOR1( ~ ) \
    VPP_DEFINE_SCALAR_OPERATORC( == ) \
    VPP_DEFINE_SCALAR_OPERATORC( != ) \
    VPP_DEFINE_SCALAR_OPERATORC( < ) \
    VPP_DEFINE_SCALAR_OPERATORC( > ) \
    VPP_DEFINE_SCALAR_OPERATORC( <= ) \
    VPP_DEFINE_SCALAR_OPERATORC( >= ) \

// -----------------------------------------------------------------------------

template< class ParamType >
struct Par : public detail::KParameter
{
    typedef ParamType param_type;
    typedef typename detail::TGetRV< ParamType >::type rvalue_type;
    typedef typename rvalue_type::comparison_type comparison_type;

    VPP_INLINE Par() :
        KParameter ( ParamType::getType() )
    {
        KShaderTranslator::get()->registerParameter ( this );
    }

    VPP_INLINE operator rvalue_type() const
    {
        ParamType par ( d_variableId );
        return rvalue_type ( par );
    }

    template< typename IndexT >
    VPP_INLINE auto operator[]( const IndexT& idx ) const
    {
        const rvalue_type rValue = *this;
        return rValue [ idx ];
    }

    VPP_INLINE int size() const
    {
        const rvalue_type rValue = *this;
        return rValue.size();
    }

    VPP_INLINE int Size() const
    {
        const rvalue_type rValue = *this;
        return rValue.Size();
    }

    VPP_DEFINE_PARAM_OPERATORS
};

// -----------------------------------------------------------------------------

VPP_INLINE void Begin()
{
    KShaderTranslator::get()->startFunctionCode();
}

// -----------------------------------------------------------------------------

VPP_INLINE void End()
{
    KShaderTranslator::get()->endFunctionCode();
}

// -----------------------------------------------------------------------------

VPP_INLINE void Return()
{
    KShaderTranslator::get()->makeReturn ( false );
}

// -----------------------------------------------------------------------------

template< typename ValueT >
VPP_INLINE void Return ( const ValueT& value )
{
    typedef typename detail::TGetRV< ValueT >::type rvalue_type;
    const rvalue_type rValue = value;
    KShaderTranslator::get()->makeReturn ( false, rValue.id() );
}

// -----------------------------------------------------------------------------
namespace impl {
// -----------------------------------------------------------------------------

template< typename ValueT >
VPP_INLINE TRValue< ValueT > Select (
    const Bool& cond,
    const TRValue< ValueT >& valueT, const TRValue< ValueT >& valueF )
{
    typedef TRValue< ValueT > result_type;

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 3 );
    operands [ 0 ] = cond.id();
    operands [ 1 ] = valueT.id();
    operands [ 2 ] = valueF.id();

    return result_type ( KId ( pTranslator->createOp (
        spv::OpSelect, result_type::getType(), operands ) ) );
};

// -----------------------------------------------------------------------------

template< typename ValueT, size_t SIZE >
VPP_INLINE TRVector< ValueT, SIZE > Select (
    const TRVector< Bool, SIZE >& cond,
    const TRVector< ValueT, SIZE >& valueT,
    const TRVector< ValueT, SIZE >& valueF )
{
    typedef TRVector< ValueT, SIZE > result_type;

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 3 );
    operands [ 0 ] = cond.id();
    operands [ 1 ] = valueT.id();
    operands [ 2 ] = valueF.id();

    return result_type ( KId ( pTranslator->createOp (
        spv::OpSelect, result_type::getType(), operands ) ) );
};

// -----------------------------------------------------------------------------

template< typename ValueT, size_t SIZE >
VPP_INLINE TRVector< ValueT, SIZE > Select (
    const Bool& cond,
    const TRVector< ValueT, SIZE >& valueT, const TRVector< ValueT, SIZE >& valueF )
{
    typedef TRVector< ValueT, SIZE > result_type;
    typedef TRVector< Bool, SIZE > cond_type;

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    const cond_type vcond ( KId (
        pTranslator->smearScalar (
            pTranslator->getPrecision(), cond.id(), cond_type::getType() ) ) );

    std::vector< spv::Id > operands ( 3 );
    operands [ 0 ] = vcond.id();
    operands [ 1 ] = valueT.id();
    operands [ 2 ] = valueF.id();

    return result_type ( KId ( pTranslator->createOp (
        spv::OpSelect, result_type::getType(), operands ) ) );
};

// -----------------------------------------------------------------------------
} // namespace impl
// -----------------------------------------------------------------------------

template< class CondT, class Arg1T, class Arg2T >
VPP_INLINE auto Select ( const CondT& cond, const Arg1T& argIfTrue, const Arg2T& argIfFalse )
{
    const typename detail::TGetRV< CondT >::type rCond = cond;
    const typename detail::TGetRV< Arg1T >::type rArg1 = argIfTrue;
    const typename detail::TGetRV< Arg2T >::type rArg2 = argIfFalse;
    return impl::Select ( rCond, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef spv::MemorySemanticsMask EMemorySemantics;
static const spv::MemorySemanticsMask MSM_NONE = spv::MemorySemanticsMaskNone;
static const spv::MemorySemanticsMask MSM_ACQ = spv::MemorySemanticsAcquireMask;
static const spv::MemorySemanticsMask MSM_REL = spv::MemorySemanticsReleaseMask;
static const spv::MemorySemanticsMask MSM_ACQREL = spv::MemorySemanticsAcquireReleaseMask;
static const spv::MemorySemanticsMask MSM_UNIFORM = spv::MemorySemanticsUniformMemoryMask;
static const spv::MemorySemanticsMask MSM_SUBGROUP = spv::MemorySemanticsSubgroupMemoryMask;
static const spv::MemorySemanticsMask MSM_WORKGROUP = spv::MemorySemanticsWorkgroupMemoryMask;
static const spv::MemorySemanticsMask MSM_IMAGE = spv::MemorySemanticsImageMemoryMask;

// -----------------------------------------------------------------------------

VPP_INLINE void WorkgroupBarrier ( 
    unsigned int memoryClass = spv::MemorySemanticsWorkgroupMemoryMask,
    unsigned int memorySem = spv::MemorySemanticsAcquireReleaseMask )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    pTranslator->createControlBarrier (
        spv::ScopeWorkgroup, spv::ScopeWorkgroup,
        static_cast< spv::MemorySemanticsMask >( memoryClass | memorySem ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE void SubgroupBarrier ( 
    unsigned int memoryClass = spv::MemorySemanticsSubgroupMemoryMask,
    unsigned int memorySem = spv::MemorySemanticsAcquireReleaseMask )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    pTranslator->createControlBarrier (
        spv::ScopeSubgroup, spv::ScopeSubgroup,
        static_cast< spv::MemorySemanticsMask >( memoryClass | memorySem ) );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGCONSTRUCTS_HPP
