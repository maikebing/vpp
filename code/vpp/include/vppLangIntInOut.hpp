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

#ifndef INC_VPPLANGINTINOUT_HPP
#define INC_VPPLANGINTINOUT_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGINTBASE_HPP
#include "vppLangIntBase.hpp"
#endif

#ifndef INC_VPPSPIRVGLSLSTD450_HPP
#include "vppGLSLstd450.hpp"
#endif

#ifndef INC_VPPDEBUGPROBE_HPP
#include "vppDebugProbe.hpp"
#endif

#ifndef INC_VPPLANGCONVERSIONS_HPP
#include "vppLangConversions.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// 14.6. Built-In Variables
// -----------------------------------------------------------------------------

class Shader
{
protected:
    Shader ( spv::ExecutionModel shaderType );

public:
    ~Shader();

    VPP_DLLAPI void DebugCodeDump();
    bool isDebugCodeDumpEnabled() const;

    template< class ValueT >
    void DebugProbe (
        const ValueT& value,
        const IVec2& coords,
        const VkExtent3D& extent );

protected:
    KShaderTranslator* d_pTranslator;
    spv::Function* d_pFunction;
    spv::Instruction* d_pEntryPoint;

private:
    bool d_bDebugCodeDump;
};

// -----------------------------------------------------------------------------

class VertexShader : public Shader
{
public:
    VPP_INLINE VertexShader() :
        Shader ( spv::ExecutionModelVertex )
    {}

    static const EShader shader_type = SH_VERTEX;

    var::inVertexIndex inVertexIndex;
    var::inInstanceIndex inInstanceIndex;

    var::outVertex outVertex;
};

// -----------------------------------------------------------------------------

class TessControlShader : public Shader
{
public:
    TessControlShader (
        ETessellationTopology topology,
        ETessellationSpacing spacing,
        int inputPatchVertices,
        int outputPatchVertices );

    static const EShader shader_type = SH_TESS_CONTROL;

    var::inVertices inVertices;
    var::inInvocationId inInvocationId;
    var::inPatchVertices inPatchVertices;
    var::inPrimitiveId inPrimitiveId;

    var::outVertices outVertices;
    var::outTessLevelOuter outTessLevelOuter;
    var::outTessLevelInner outTessLevelInner;
};

// -----------------------------------------------------------------------------

class TessEvalShader : public Shader
{
public:
    TessEvalShader (
        bool bVertexOrderCw,
        bool bPointMode,
        int inputPatchVertices );

    static const EShader shader_type = SH_TESS_EVAL;

    var::inVertices inVertices;
    var::inPatchVertices inPatchVertices;
    var::inPrimitiveId inPrimitiveId;
    var::inTessCoord inTessCoord;
    var::inTessLevelOuter inTessLevelOuter;
    var::inTessLevelInner inTessLevelInner;

    var::outVertex outVertex;
};

// -----------------------------------------------------------------------------

class GeometryShader : public Shader
{
public:
    GeometryShader (
        EGeometryInputTopology inputTopology,
        EGeometryOutputTopology outputTopology,
        int maxOutputVertices,
        int invocations );

    static const EShader shader_type = SH_GEOMETRY;

    VPP_DLLAPI void EmitVertex();
    VPP_DLLAPI void EndPrimitive();

    var::inVertices inVertices;
    var::inInvocationId inInvocationId;
    var::inPrimitiveId inPrimitiveId;

    var::outVertex outVertex;
    var::outLayer outLayer;
    var::outPrimitiveId outPrimitiveId;
    var::outViewportIndex outViewportIndex;
};

// -----------------------------------------------------------------------------

class FragmentShader : public Shader
{
public:
    VPP_INLINE FragmentShader() :
        Shader ( spv::ExecutionModelFragment ),
        outFragDepth ( d_pFunction )
    {
        d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeOriginUpperLeft );
    }

    static const EShader shader_type = SH_FRAGMENT;

    // special methods specific for fragment shaders

    VPP_DLLAPI void Kill();
    VPP_DLLAPI void EarlyTest();

    template< class ValueT >
    void DebugProbe ( const ValueT& value );

    template< class ValueT >
    void DebugProbe (
        const ValueT& value,
        const IVec2& coords,
        const VkExtent3D& extent );

    // builtin variables

    var::inFragCoord inFragCoord;
    var::inFrontFacing inFrontFacing;
    var::inLayer inLayer;
    var::inSampleId inSampleId;
    var::inSamplePosition inSamplePosition;
    var::inPointCoord inPointCoord;
    var::inHelperInvocation inHelperInvocation;
    var::inPrimitiveId inPrimitiveId;
    var::inViewportIndex inViewportIndex;

    var::outFragDepth outFragDepth;

private:
    VPP_DLLAPI VkExtent3D findAttachmentExtent ( const RenderGraph& renderGraph ) const;
};

// -----------------------------------------------------------------------------

class ComputeShader : public Shader
{
public:
    VPP_INLINE ComputeShader ( const SLocalGroupSize& localSize ) :
        Shader ( spv::ExecutionModelGLCompute ),
        inWorkgroupSize ( localSize, d_pFunction )
    {
        d_pTranslator->addExecutionMode (
            d_pFunction, spv::ExecutionModeLocalSize,
            static_cast< int >( localSize.x ),
            static_cast< int >( localSize.y ),
            static_cast< int >( localSize.z )
        );
    }

    static const EShader shader_type = SH_COMPUTE;

    var::inWorkgroupId inWorkgroupId;
    var::inNumWorkgroups inNumWorkgroups;
    var::inLocalInvocationId inLocalInvocationId;
    var::inGlobalInvocationId inGlobalInvocationId;
    var::inWorkgroupSize inWorkgroupSize;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ValueT >
void Shader :: DebugProbe (
    const ValueT& value,
    const IVec2& coords,
    const VkExtent3D& extent )
{
    PipelineConfig* pConfig = PipelineConfig::getInstance();
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    typedef TDebugProbe< ValueT > KDebugProbe;

    VkExtent3D effectiveExtent = extent;

    if ( effectiveExtent.height == 0 )
        effectiveExtent.height = 1;

    if ( effectiveExtent.depth == 0 )
        effectiveExtent.depth = 1;

    KDebugProbe* pProbe = new KDebugProbe ( effectiveExtent, pTranslator->getDevice() );
    HDebugProbe hProbe = HDebugProbe ( pProbe );
    pConfig->registerDebugProbe ( hProbe );

    pProbe->write ( coords, value );
}

// -----------------------------------------------------------------------------

template< class ValueT >
void FragmentShader :: DebugProbe ( const ValueT& value )
{
    PipelineConfig* pConfig = PipelineConfig::getInstance();
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    RenderGraph& renderGraph = pConfig->getRenderGraph();

    VkExtent3D extent = findAttachmentExtent ( renderGraph );

    const Vec4 fragCoord = inFragCoord;
    const IVec2 pixelCoord = StaticCast< IVec2 >( fragCoord [ XY ] );

    typedef TDebugProbe< ValueT > KDebugProbe;

    KDebugProbe* pProbe = new KDebugProbe ( extent, pTranslator->getDevice() );
    HDebugProbe hProbe = HDebugProbe ( pProbe );
    pConfig->registerDebugProbe ( hProbe );

    pProbe->write ( pixelCoord, value );
}

// -----------------------------------------------------------------------------

template< class ValueT >
void FragmentShader :: DebugProbe (
    const ValueT& value,
    const IVec2& coords,
    const VkExtent3D& extent )
{
    Shader::DebugProbe ( value, coords, extent );
}

// -----------------------------------------------------------------------------
// 14.1. Shader Input and Output Interfaces
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class OutputShader, class InputShader >
struct TIOVariableTraits
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = false;
};

template<>
struct TIOVariableTraits< VertexShader, TessControlShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< VertexShader, GeometryShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< VertexShader, FragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< TessControlShader, TessEvalShader >
{
    static const bool inArray = true;
    static const bool outArray = true;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< TessEvalShader, GeometryShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< GeometryShader, FragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< TessEvalShader, FragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< vertexShader, tessControlShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< vertexShader, geometryShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< vertexShader, fragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< tessControlShader, tessEvalShader >
{
    static const bool inArray = true;
    static const bool outArray = true;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< tessEvalShader, geometryShader >
{
    static const bool inArray = true;
    static const bool outArray = false;
    static const bool inFragment = false;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< geometryShader, fragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

template<>
struct TIOVariableTraits< tessEvalShader, fragmentShader >
{
    static const bool inArray = false;
    static const bool outArray = false;
    static const bool inFragment = true;
    static const bool valid = true;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class VariableT, bool OUTPUT, bool ARRAY >
class TIOVariable {};

// -----------------------------------------------------------------------------

template< class VariableT, bool OUTPUT >
class TIOVariable< VariableT, OUTPUT, false > : public KValue
{
public:
    typedef typename VariableT::rvalue_type rvalue_type;

    VPP_INLINE TIOVariable ( const KId& variableId, int s ) :
        KValue ( variableId )
    {}

    VPP_INLINE KId getType()
    {
        return rvalue_type::getType();
    }

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( ! OUTPUT, "This variable is write-only" );
        return rvalue_type ( KId ( KShaderTranslator::get()->createLoad ( id() ) ) );
    }

    template< typename RightT >
    VPP_INLINE void assign ( const RightT& rhs )
    {
        static_assert ( OUTPUT, "This variable is read-only" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        rvalue_type value = rhs;
        pTranslator->createStore ( value.id(), id() );
    }

    template< typename IndexT >
    VPP_INLINE auto operator[]( IndexT index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        Accessor< rvalue_type, ! OUTPUT, OUTPUT > accessor ( pTranslator->getAccessChain() );
        return accessor [ index ];
    }

    template< typename RightT > 
    VPP_INLINE rvalue_type operator+ ( const RightT& rhs )
    {
        const rvalue_type lrv = *this;
        const VPP_RVTYPE( RightT ) rrv = rhs;
        return lrv + rrv;
    }

    template< typename RightT > 
    VPP_INLINE rvalue_type operator- ( const RightT& rhs )
    {
        const rvalue_type lrv = *this;
        const VPP_RVTYPE( RightT ) rrv = rhs;
        return lrv - rrv;
    }

    template< typename RightT > 
    VPP_INLINE rvalue_type operator* ( const RightT& rhs )
    {
        const rvalue_type lrv = *this;
        const VPP_RVTYPE( RightT ) rrv = rhs;
        return lrv * rrv;
    }

    template< typename RightT > 
    VPP_INLINE rvalue_type operator/ ( const RightT& rhs )
    {
        const rvalue_type lrv = *this;
        const VPP_RVTYPE( RightT ) rrv = rhs;
        return lrv / rrv;
    }

    VPP_INLINE rvalue_type operator-() const
    {
        const rvalue_type rv = *this;
        return -rv;
    }
};

// -----------------------------------------------------------------------------

template< class VariableT, bool OUTPUT >
class TIOVariable< VariableT, OUTPUT, true > : public KValue
{
public:
    typedef typename VariableT::rvalue_type rvalue_type;

    VPP_INLINE TIOVariable ( const KId& variableId, int s ) :
        KValue ( variableId ),
        d_size ( s )
    {}

    VPP_INLINE Int Size() const
    {
        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        return d_size;
    }

    VPP_INLINE operator rvalue_type() const
    {
        static_assert ( TFalse< VariableT >::value, "The inter-shader variable is an array in this shader. Use [] to index it." );
        return rvalue_type();
    }

    template< typename IndexT >
    VPP_INLINE auto operator[]( IndexT index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( id() );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );
        Accessor< rvalue_type, ! OUTPUT, OUTPUT > accessor ( pTranslator->getAccessChain() );
        return accessor;
    }

private:
    int d_size;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/* 
   CPU-side inter-shader variable descriptor objects.
   Use them to declare communication interfaces between shader stages.

   ioVariable
   ioStructure
*/
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ValueT, class OutputShader, class InputShader >
class ioVariable
{
public:
    typedef typename ValueT::rvalue_type rvalue_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;

    static_assert (
        rvalue_type::external_linkage,
        "VBool types and composites can be used only inside functions" );

    VPP_INLINE ioVariable() :
        d_location ( 0 )
    {
        PipelineConfig* pConfig = PipelineConfig::getInstance();

        detail::SIOVariablesInfo* pVariablesInfo =
            pConfig->findIOVariablesInfo< OutputShader, InputShader >();

        d_location = pVariablesInfo->d_currentLocation;
        pVariablesInfo->d_currentLocation += rvalue_type::location_count;
    }

    VPP_INLINE int location() const
    {
        return d_location;
    }

private:
    int d_location;
};

// -----------------------------------------------------------------------------

template< class StructT, class OutputShader, class InputShader >
class ioStructure
{
public:
    typedef StructT struct_type;
    typedef StructArrayItem< StructT > item_type;
    typedef typename StructT::definition_type definition_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;

    VPP_INLINE ioStructure() :
        d_location ( 0 )
    {
        PipelineConfig* pConfig = PipelineConfig::getInstance();

        detail::SIOVariablesInfo* pVariablesInfo =
            pConfig->findIOVariablesInfo< OutputShader, InputShader >();

        d_location = pVariablesInfo->d_currentLocation;

        StructT defineStruct;

        detail::SStructLocationInfo& locationInfo = pConfig->currentStructType();
        pConfig->popStructType();

        pVariablesInfo->d_currentLocation += locationInfo.d_occupiedLocations;
    }

    VPP_INLINE int location() const
    {
        return d_location;
    }

private:
    int d_location;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/*
    GPU-side inter-shader variable accessors.
    Declare them locally in shaders to gain access to variables passed from
    or to other shader stages.
    For Input< ioVariable > in fragment shaders, you can optionally specify
    interpolation mode.
*/
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class VariableT >
class Input {};

template< class VariableT >
class Output {};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class StructT, class OutputShader, class InputShader >
class Input< ioStructure< StructT, OutputShader, InputShader > > :
    public KShaderScopedVariable
{
public:
    typedef ioStructure< StructT, OutputShader, InputShader > var_type;
    typedef StructT struct_type;
    typedef typename struct_type::definition_type definition_type;
    typedef typename var_type::item_type item_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;
    typedef StructAccessor< definition_type, true, false > return_type;
    static const bool isArray = detail::TIOVariableTraits< OutputShader, InputShader >::inArray;
    static const bool isFragIn = detail::TIOVariableTraits< OutputShader, InputShader >::inFragment;

    VPP_INLINE Input ( const var_type& var ) :
        d_id ( init ( var.location() ) ),
        d_size ( KShaderTranslator::get()->getShaderOutputSize() )
    {
    }

    VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const std::type_index index ( typeid ( definition_type ) );

        const KShaderTranslator::SStructInfo* pStructInfo = 
            pTranslator->findStructType ( index, spv::DecorationLocation );

        if ( pStructInfo )
            return isArray ?
                KId ( pTranslator->makeArrayType ( 
                        pStructInfo->d_typeId,
                        pTranslator->makeIntConstant ( d_size ), 0 ) )
                : pStructInfo->d_typeId;
        else
            return KId ( 0 );
    }

    template< typename MemberT >
    VPP_INLINE Accessor< typename MemberT::rvalue_type, true, false >
        operator[]( MemberT struct_type::* pMember ) const
    {
        typedef typename MemberT::rvalue_type mem_rvalue_type;
        typedef Accessor< mem_rvalue_type, true, false > result_type;

        static_assert (
            ! isArray, "Attempt to access struct member on array-typed object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( d_id );

        const MemberT& member = d_struct.*pMember;
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( member.index() ) );

        return result_type ( pTranslator->getAccessChain() );
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index )
    {
        static_assert (
            isArray, "Attempt to access array item on struct-typed object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( d_id );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return StructAccessor< struct_type, true, false > (
            & d_struct, pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        static_assert (
            isArray, "Attempt to access array size on struct-typed object" );

        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        static_assert (
            isArray, "Attempt to access array size on struct-typed object" );

        return d_size;
    }

private:
    KId init ( int location );
    
private:
    definition_type d_struct;
    KId d_id;
    int d_size;
};

// -----------------------------------------------------------------------------

template< class StructT, class OutputShader, class InputShader >
class Output< ioStructure< StructT, OutputShader, InputShader > > :
    public KShaderScopedVariable
{
public:
    typedef ioStructure< StructT, OutputShader, InputShader > var_type;
    typedef StructT struct_type;
    typedef typename struct_type::definition_type definition_type;
    typedef typename var_type::item_type item_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;
    typedef StructAccessor< definition_type, false, true > return_type;
    static const bool isArray = detail::TIOVariableTraits< OutputShader, InputShader >::outArray;

    VPP_INLINE Output ( const var_type& var ) :
        d_id ( init ( var.location() ) ),
        d_size ( KShaderTranslator::get()->getShaderOutputSize() )
    {
    }

    VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const std::type_index index ( typeid ( definition_type ) );

        const KShaderTranslator::SStructInfo* pStructInfo = 
            pTranslator->findStructType ( index, spv::DecorationLocation );

        if ( pStructInfo )
            return isArray ?
                KId ( pTranslator->makeArrayType ( 
                        pStructInfo->d_typeId,
                        pTranslator->makeIntConstant ( d_size ), 0 ) )
                : pStructInfo->d_typeId;
        else
            return KId ( 0 );
    }

    template< typename MemberT >
    VPP_INLINE Accessor< typename MemberT::rvalue_type, false, true >
        operator[]( MemberT struct_type::* pMember ) const
    {
        typedef typename MemberT::rvalue_type mem_rvalue_type;
        typedef Accessor< mem_rvalue_type, false, true > result_type;

        static_assert (
            ! isArray, "Attempt to access struct member on array-typed object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( d_id );

        const MemberT& member = d_struct.*pMember;
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( member.index() ) );

        return result_type ( pTranslator->getAccessChain() );
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index )
    {
        static_assert (
            isArray, "Attempt to access array item on struct-typed object" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->clearAccessChain();
        pTranslator->setAccessChainLValue ( d_id );
        pTranslator->accessChainPush ( pTranslator->getArrayIndex ( index ) );

        return StructAccessor< struct_type, false, true > (
            & d_struct, pTranslator->getAccessChain() );
    }

    VPP_INLINE return_type operator[]( const Int& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) { return getItem ( index ); }

    VPP_INLINE Int Size() const
    {
        static_assert (
            isArray, "Attempt to access array size on struct-typed object" );

        return Int ( KId ( KShaderTranslator::get()->makeIntConstant ( d_size ) ) );
    }

    VPP_INLINE int size() const
    {
        static_assert (
            isArray, "Attempt to access array size on struct-typed object" );

        return d_size;
    }

private:
    KId init ( int location );
    
private:
    definition_type d_struct;
    KId d_id;
    int d_size;
};

// -----------------------------------------------------------------------------

template< class StructT, class OutputShader, class InputShader >
KId Input< ioStructure< StructT, OutputShader, InputShader > > :: init ( int location )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
    pTranslator->popStructType();

    KShaderTranslator::SVariableInfo& varInfo =
        pTranslator->getShaderScopedVariable ( this );

    if ( ! varInfo )
    {
        varInfo.d_typeId = getType();
        
        varInfo.d_variableId = KId ( pTranslator->createVariable (
            spv::StorageClassInput, varInfo.d_typeId ) );

        pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationLocation, location );

        if ( isFragIn )
        {
            const std::vector< KShaderTranslator::SMemberInfo >& memberInfos = structInfo.d_memberInfos;

            for ( size_t iMember = 0; iMember != memberInfos.size(); ++iMember )
            {
                const KShaderTranslator::SMemberInfo& memInfo = memberInfos [ iMember ];

                const spv::Id memberBaseTypeId =
                    pTranslator->getScalarTypeId ( memInfo.d_typeId );

                const spv::Op opType = pTranslator->getTypeClass ( memberBaseTypeId );

                if ( opType == spv::OpTypeInt
                     || ( opType == spv::OpTypeFloat
                          && pTranslator->getNumTypeBits ( memberBaseTypeId ) > 32 ) )
                {
                    pTranslator->addMemberDecoration (
                        varInfo.d_typeId,
                        static_cast< int >( iMember ),
                        spv::DecorationFlat );
                }
            }
        }
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------

template< class StructT, class OutputShader, class InputShader >
KId Output< ioStructure< StructT, OutputShader, InputShader > > :: init ( int location )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    pTranslator->popStructType();

    KShaderTranslator::SVariableInfo& varInfo =
        pTranslator->getShaderScopedVariable ( this );

    if ( ! varInfo )
    {
        varInfo.d_typeId = getType();
        
        varInfo.d_variableId = KId ( pTranslator->createVariable (
            spv::StorageClassOutput, varInfo.d_typeId ) );

        pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationLocation, location );
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ValueT, class OutputShader, class InputShader >
class Input< ioVariable< ValueT, OutputShader, InputShader > > :
    public KShaderScopedVariable,
    public detail::TIOVariable<
        ioVariable< ValueT, OutputShader, InputShader >,
        false,
        detail::TIOVariableTraits< OutputShader, InputShader >::inArray
    >
{
public:
    typedef ioVariable< ValueT, OutputShader, InputShader > var_type;
    typedef detail::TIOVariableTraits< OutputShader, InputShader > traits_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;
    typedef typename ValueT::rvalue_type interp_type;
    static const bool isArray = traits_type::inArray;
    static const bool isFragIn = traits_type::inFragment;
    typedef detail::TIOVariable< var_type, false, isArray > base_var_type;
    typedef typename base_var_type::rvalue_type rvalue_type;

    VPP_INLINE Input ( const var_type& var ) :
        detail::TIOVariable< var_type, false, isArray >(
            init ( var.location(), 0 ),
            KShaderTranslator::get()->getShaderInputSize()
        )
    {
    }

    VPP_INLINE Input ( const var_type& var, std::uint32_t interpFlags ) :
        detail::TIOVariable< var_type, false, isArray >(
            init ( var.location(), interpFlags ),
            KShaderTranslator::get()->getShaderInputSize()
        )
    {
        static_assert (
            isFragIn, "Interpolation flags may only be given to fragment shader input variables" );
    }

    VPP_INLINE interp_type InterpolateAtCentroid()
    {
        return Interpolate ( GLSLstd450InterpolateAtCentroid, 0 );
    }

    VPP_INLINE interp_type InterpolateAtSample ( const Int& sample )
    {
        return Interpolate ( GLSLstd450InterpolateAtSample, sample.id() );
    }

    VPP_INLINE interp_type InterpolateAtOffset ( const Vec2& offset )
    {
        return Interpolate ( GLSLstd450InterpolateAtOffset, offset.id() );
    }

    VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        if ( isArray )
        {
            const int s = pTranslator->getShaderInputSize();

            return KId ( pTranslator->makeArrayType (
                rvalue_type::getType(),
                pTranslator->getArrayIndex ( s ), 0 ) );
        }
        else
            return interp_type::getType();
    }

private:
    KId init ( int location, std::uint32_t interpFlags );

    interp_type Interpolate ( GLSLstd450 fun, spv::Id arg ) const
    {
        static_assert (
            isFragIn,
            "Interpolation functions may only be called from fragment shaders" );

        KShaderTranslator* pTranslator = KShaderTranslator::get();
        pTranslator->addCapability ( spv::CapabilityInterpolationFunction );

        std::vector< spv::Id > args;
        args.reserve ( 2 );
        args.push_back ( this->id() );

        if ( arg )
            args.push_back ( arg );

        return interp_type ( KId (
            pTranslator->createBuiltinCall (
                interp_type::getType(), pTranslator->getBuiltinFunctions(),
                fun, args ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class ValueT, class OutputShader, class InputShader >
class Output< ioVariable< ValueT, OutputShader, InputShader > > :
    public KShaderScopedVariable,
    public detail::TIOVariable<
        ioVariable< ValueT, OutputShader, InputShader >,
        true,
        detail::TIOVariableTraits< OutputShader, InputShader >::outArray
    >
{
public:
    typedef ioVariable< ValueT, OutputShader, InputShader > var_type;
    typedef OutputShader out_shader_type;
    typedef InputShader in_shader_type;
    static const bool isArray = detail::TIOVariableTraits< OutputShader, InputShader >::outArray;
    typedef detail::TIOVariable< var_type, true, isArray > base_var_type;
    typedef typename base_var_type::rvalue_type rvalue_type;

    VPP_INLINE Output ( const var_type& var ) :
        detail::TIOVariable< var_type, true, isArray >(
            init ( var.location() ),
            KShaderTranslator::get()->getShaderOutputSize() 
        )
    {
    }

    template< typename RightT >
    VPP_INLINE void operator= ( const RightT& rhs )
    {
        base_var_type::assign ( rhs );
    }

    VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        typedef typename ValueT::rvalue_type rv_type;

        if ( isArray )
        {
            const int s = pTranslator->getShaderOutputSize();

            return KId ( pTranslator->makeArrayType (
                rvalue_type::getType(),
                pTranslator->getArrayIndex ( s ), 0 ) );
        }
        else
            return rv_type::getType();
    }

private:
    KId init ( int location );
};

// -----------------------------------------------------------------------------

template< class ValueT, class OutputShader, class InputShader >
KId Input< ioVariable< ValueT, OutputShader, InputShader > > :: init (
    int location, std::uint32_t interpFlags )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    KShaderTranslator::SVariableInfo& varInfo =
        pTranslator->getShaderScopedVariable ( this );

    if ( ! varInfo )
    {
        varInfo.d_typeId = getType();
        
        varInfo.d_variableId = KId ( pTranslator->createVariable (
            spv::StorageClassInput, varInfo.d_typeId ) );

        pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationLocation, location );

        if ( isFragIn )
        {
            typedef typename ValueT::rvalue_type rvalue_type;
            typedef typename rvalue_type::scalar_type sc_type;

            if ( scalar_traits< sc_type >::isFlatShaded )
                pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationFlat );
            else
            {
                if ( interpFlags & IN_FLAT )
                    pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationFlat );
                if ( interpFlags & IN_NOPERSPECTIVE )
                    pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationNoPerspective );
                if ( interpFlags & IN_CENTROID )
                    pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationCentroid );
                if ( interpFlags & IN_SAMPLE )
                    pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationSample );
            }
        }
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------

template< class ValueT, class OutputShader, class InputShader >
KId Output< ioVariable< ValueT, OutputShader, InputShader > > :: init ( int location )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    KShaderTranslator::SVariableInfo& varInfo =
        pTranslator->getShaderScopedVariable ( this );

    if ( ! varInfo )
    {
        varInfo.d_typeId = getType();
        
        varInfo.d_variableId = KId ( pTranslator->createVariable (
            spv::StorageClassOutput, varInfo.d_typeId ) );

        pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        pTranslator->addDecoration ( varInfo.d_variableId, spv::DecorationLocation, location );
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define VPP_DEFINE_INPUT_OPERATOR1( OP ) \
template< class ValueT, class OutputShader, class InputShader > \
VPP_INLINE auto operator OP ( const Input< ioVariable< ValueT, OutputShader, InputShader > >& arg ) \
{ \
    typedef typename Input< ioVariable< ValueT, OutputShader, InputShader > >::rvalue_type rvalue_type; \
    rvalue_type rhs = arg; \
    return OP rhs; \
} \

#define VPP_DEFINE_INPUT_OPERATOR2_SI( OP ) \
template< class ScalarT, class ValueT, class OutputShader, class InputShader > \
VPP_INLINE auto operator OP ( \
    const TRValue< ScalarT >& lhs, \
    const Input< ioVariable< ValueT, OutputShader, InputShader > >& arg ) \
{ \
    typedef typename Input< ioVariable< ValueT, OutputShader, InputShader > >::rvalue_type rvalue_type; \
    rvalue_type rhs = arg; \
    return lhs OP rhs; \
} \

#define VPP_DEFINE_INPUT_OPERATOR2_II( OP ) \
template< class Value1T, class Value2T, class OutputShader, class InputShader > \
VPP_INLINE auto operator OP ( \
    const Input< ioVariable< Value1T, OutputShader, InputShader > >& arg1, \
    const Input< ioVariable< Value2T, OutputShader, InputShader > >& arg2 ) \
{ \
    typedef typename Input< ioVariable< Value1T, OutputShader, InputShader > >::rvalue_type rvalue_type1; \
    typedef typename Input< ioVariable< Value2T, OutputShader, InputShader > >::rvalue_type rvalue_type2; \
    rvalue_type1 lhs = arg1; \
    rvalue_type2 rhs = arg2; \
    return lhs OP rhs; \
} \

#define VPP_DEFINE_INPUT_OPERATORS1 \
    VPP_DEFINE_INPUT_OPERATOR1( - ) \
    VPP_DEFINE_INPUT_OPERATOR1( ~ ) \
    VPP_DEFINE_INPUT_OPERATOR1( ! ) \

#define VPP_DEFINE_INPUT_OPERATORS2_SI \
    VPP_DEFINE_INPUT_OPERATOR2_SI( + ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( - ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( * ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( / ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( % ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( << ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( >> ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( | ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( ^ ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( & ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( || ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( && ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( == ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( != ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( < ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( > ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( <= ) \
    VPP_DEFINE_INPUT_OPERATOR2_SI( >= ) \

#define VPP_DEFINE_INPUT_OPERATORS2_II \
    VPP_DEFINE_INPUT_OPERATOR2_II( + ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( - ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( * ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( / ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( % ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( << ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( >> ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( | ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( ^ ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( & ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( || ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( && ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( == ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( != ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( < ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( > ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( <= ) \
    VPP_DEFINE_INPUT_OPERATOR2_II( >= ) \

VPP_DEFINE_INPUT_OPERATORS1

VPP_DEFINE_INPUT_OPERATORS2_SI

VPP_DEFINE_INPUT_OPERATORS2_II

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGINTINOUT_HPP
