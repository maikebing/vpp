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

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppLangTranslator.hpp"
#include "../include/vppLangScalarTypes.hpp"
#include "../include/vppLangVectorTypes.hpp"
#include "../include/vppLangMatrixTypes.hpp"
#include "../include/vppLangConstructs.hpp"

#include "../src/spirv/disassemble.h"
#include "../src/spirv/Logger.h"
#include "../src/spirv/SpvBuilder.h"

#include "../include/vppBlendOperators.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

thread_local KShaderTranslator* KShaderTranslator :: s_pThis = 0;

// -----------------------------------------------------------------------------

KShaderTranslator :: KShaderTranslator ( VkShaderStageFlagBits stage, const Device& hDevice ) :
    Builder ( 1, this ),
    d_hDevice ( hDevice ),
    d_stage ( stage ),
    d_bDeviceSupportsVulkan11 ( hDevice.supportsVersion ( { 1, 1, 0 } ) ),
    d_pCurrentFunction ( 0 ),
    d_pCurrentFunctionName ( 0 ),
    d_structDecoration ( spv::DecorationBlock ),
    d_builtinFunctions ( import ( "GLSL.std.450" ) ),
    d_shaderInputSize ( 1 ),
    d_shaderOutputSize ( 1 ),
    d_maxSharedVariablesByteCount (
        hDevice.physical().properties().limits.maxComputeSharedMemorySize ),
    d_sharedVariablesByteCount ( 0 )
{
    s_pThis = this;

    for ( const std::string& iExt : hDevice.sourceExtensions() )
        addSourceExtension ( iExt.c_str() );

    d_scopeStack.push_back ( detail::KScope() );
    d_functionScopeStack.push_back ( detail::KFunctionScope() );
}

// -----------------------------------------------------------------------------

KShaderTranslator :: ~KShaderTranslator()
{
    s_pThis = 0;
    d_functionScopeStack.pop_back();
    d_scopeStack.pop_back();
}

// -----------------------------------------------------------------------------

KShaderTranslator* KShaderTranslator :: get()
{
    return s_pThis;
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: useCapability ( spv::Capability cap )
{
    switch ( cap )
    {
        case spv::CapabilityMatrix:
        case spv::CapabilityShader:
        case spv::CapabilityInputAttachment:
        case spv::CapabilitySampled1D:
        case spv::CapabilityImage1D:
        case spv::CapabilitySampledBuffer:
        case spv::CapabilityImageBuffer:
        case spv::CapabilityImageQuery:
        case spv::CapabilityDerivativeControl:
        case spv::CapabilityStorageImageExtendedFormats:
            // Those are supported unconditionally on all Vulkan versions.
            break;

        case spv::CapabilityTessellationPointSize:
        case spv::CapabilityGeometryPointSize:
            requireFeature ( fShaderTessellationAndGeometryPointSize );
            break;

        case spv::CapabilityStorageImageMultisample:
        case spv::CapabilityImageMSArray:
            requireFeature ( fShaderStorageImageMultisample );
            break;

        case spv::CapabilityInterpolationFunction:
        case spv::CapabilitySampleRateShading:
            requireFeature ( fSampleRateShading );
            break;

        case spv::CapabilityGeometry: requireFeature ( fGeometryShader ); break;
        case spv::CapabilityTessellation: requireFeature ( fTessellationShader ); break;
        case spv::CapabilityFloat64: requireFeature ( fShaderFloat64 ); break;
        case spv::CapabilityInt64: requireFeature ( fShaderInt64 ); break;
        case spv::CapabilityInt16: requireFeature ( fShaderInt16 ); break;
        case spv::CapabilityImageGatherExtended: requireFeature ( fShaderImageGatherExtended ); break;
        case spv::CapabilityUniformBufferArrayDynamicIndexing: requireFeature ( fShaderUniformBufferArrayDynamicIndexing ); break;
        case spv::CapabilitySampledImageArrayDynamicIndexing: requireFeature ( fShaderSampledImageArrayDynamicIndexing ); break;
        case spv::CapabilityStorageBufferArrayDynamicIndexing: requireFeature ( fShaderStorageBufferArrayDynamicIndexing ); break;
        case spv::CapabilityStorageImageArrayDynamicIndexing: requireFeature ( fShaderStorageImageArrayDynamicIndexing ); break;
        case spv::CapabilityClipDistance: requireFeature ( fShaderClipDistance ); break;
        case spv::CapabilityCullDistance: requireFeature ( fShaderCullDistance ); break;
        case spv::CapabilityImageCubeArray: requireFeature ( fImageCubeArray ); break;
        case spv::CapabilitySparseResidency: requireFeature ( fShaderResourceResidency ); break;
        case spv::CapabilityMinLod: requireFeature ( fShaderResourceMinLod ); break;
        case spv::CapabilitySampledCubeArray: requireFeature ( fImageCubeArray ); break;
        case spv::CapabilityStorageImageReadWithoutFormat: requireFeature ( fShaderStorageImageReadWithoutFormat ); break;
        case spv::CapabilityStorageImageWriteWithoutFormat: requireFeature ( fShaderStorageImageWriteWithoutFormat ); break;
        case spv::CapabilityMultiViewport: requireFeature ( fMultiViewport ); break;

        case spv::CapabilityMultiView:
        case spv::CapabilityDeviceGroup:
            requireVersion11();
            break;

        case spv::CapabilityDrawParameters:
            requireVersion11();
            requireFeature ( fShaderDrawParameters );
            break;

        case spv::CapabilityInt64Atomics:
            requireVersion11();
            requireFeature ( fShaderBufferInt64Atomics );
            requireFeature ( fShaderSharedInt64Atomics );
            break;

        default:
            throw XUsageError ( "Unsupported capability has been used" );
    }

    addCapability ( cap );
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: getArrayIndex ( int v )
{
    return KId ( makeIntConstant ( v ) );
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: getArrayIndex ( unsigned int v )
{
    return KId ( makeUintConstant ( v ) );
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: getScopedRValue ( const KId& lValue )
{
    detail::KScope::L2RValue& values = d_scopeStack.back().d_l2rValue;
    const auto& iValue = values.find ( lValue );

    if ( iValue == values.end() )
    {
        const KId newValue ( createLoad ( lValue ) );
        values [ lValue ] = newValue;
        return newValue;
    }
    else
        return KId ( iValue->second );
}

// -----------------------------------------------------------------------------

KShaderModule KShaderTranslator :: generateModule() const
{
    std::vector< unsigned int > fdata;
    dump ( fdata );
    return KShaderModule (
        & fdata [ 0 ], fdata.size() * sizeof ( unsigned int ), d_hDevice );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: print ( std::ostream& output ) const
{
    std::vector< unsigned int > fdata;
    dump ( fdata );
    spv::Disassemble ( output, fdata );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: pushIf ( Bool condition )
{
    d_ifStack.emplace_back ( condition.id(), *this );
    d_scopeStack.push_back ( detail::KScope() );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: popIf()
{
    d_ifStack.back().makeEndIf();
    d_ifStack.pop_back();
    d_scopeStack.pop_back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: makeElse()
{
    d_scopeStack.pop_back();
    d_scopeStack.push_back ( detail::KScope() );
    d_ifStack.back().makeBeginElse();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: pushSwitch ( spv::Id cond )
{
    d_switchStack.emplace_back ( cond, *this );
    d_scopeStack.push_back ( detail::KScope() );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: makeSwitchCase ( int value )
{
    d_switchStack.back().addCase ( value );
    d_scopeStack.pop_back();
    d_scopeStack.push_back ( detail::KScope() );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: makeSwitchDefault()
{
    d_switchStack.back().addDefault();
    d_scopeStack.pop_back();
    d_scopeStack.push_back ( detail::KScope() );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: popSwitch()
{
    d_switchStack.back().createSwitch();
    d_switchStack.pop_back();
    d_scopeStack.pop_back();
}

// -----------------------------------------------------------------------------

KShaderTranslator::LoopBlocks& KShaderTranslator :: pushLoop()
{
    d_loopStack.emplace_back ( makeNewLoop() );
    d_scopeStack.push_back ( detail::KScope() );
    return d_loopStack.back();
}

// -----------------------------------------------------------------------------

KShaderTranslator::LoopBlocks& KShaderTranslator :: currentLoop()
{
    return d_loopStack.back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: popLoop()
{
    d_loopStack.pop_back();
    d_scopeStack.pop_back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: pushFor ( const KId& varId, const KId& stepId, bool bUnsigned )
{
    d_forStack.emplace_back ( varId, stepId, bUnsigned );
}

// -----------------------------------------------------------------------------

const detail::KForRange& KShaderTranslator :: currentFor() const
{
    return d_forStack.back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: popFor()
{
    d_forStack.pop_back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: registerNewFunction (
    detail::KFunction* pFunction, const char* pName )
{
    d_pCurrentFunction = pFunction;
    d_currentFunctionParameters.clear();
    d_pCurrentFunctionName = pName;
    pFunction->d_pParentBlock = getBuildPoint();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: registerParameter ( detail::KParameter* pParameter )
{
    d_currentFunctionParameters.push_back ( pParameter );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: startFunctionCode()
{
    const size_t nParams = d_currentFunctionParameters.size();

    std::vector< spv::Id > paramTypes ( nParams );
    std::vector< spv::Decoration > precisions ( nParams, getPrecision() );

    for ( size_t i = 0; i != d_currentFunctionParameters.size(); ++i )
        paramTypes [ i ] = d_currentFunctionParameters [ i ]->d_typeId;

    spv::Block* pBlock = 0;

    spv::Function* pFunction = makeFunctionEntry (
        getPrecision(), d_pCurrentFunction->d_returnTypeId,
        d_pCurrentFunctionName,
        paramTypes,
        precisions,
        & pBlock );

    for ( size_t i = 0; i != d_currentFunctionParameters.size(); ++i )
        d_currentFunctionParameters [ i ]->d_variableId =
            KId ( pFunction->getParamId ( static_cast< int >( i ) ) );

    d_pCurrentFunction->d_pFunction = pFunction;

    d_scopeStack.push_back ( detail::KScope() );
    d_functionScopeStack.push_back ( detail::KFunctionScope() );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: endFunctionCode()
{
    detail::KFunctionScope& currentScope = d_functionScopeStack.back();
    const bool bUnreleasedVariables = ( currentScope.d_activeLocalVariables != 0 );

    leaveFunction();
    d_functionScopeStack.pop_back();
    d_scopeStack.pop_back();
    setBuildPoint ( d_pCurrentFunction->d_pParentBlock );
    d_pCurrentFunction = 0;
    d_pCurrentFunctionName = 0;
    d_currentFunctionParameters.clear();

    if ( bUnreleasedVariables )
        throw XUsageError (
            "Error: Missing {} scope between Begin() and End() in function definition"
        );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: registerInputOutputVariable ( const KId& id )
{
    d_inputOutputVariables.insert ( id );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: generateInputOutputForwards ( spv::Instruction* pEntryPoint )
{
    for ( const auto iId : d_inputOutputVariables )
        pEntryPoint->addIdOperand ( iId );
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerLocalVariable ( KId type, spv::StorageClass eClass )
{
    return KId ( createVariable ( eClass, type ) );
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerUniformBuffer (
    KId type, std::uint32_t set, std::uint32_t binding, spv::StorageClass eClass )
{
    const KId result ( createVariable ( eClass, type ) );

    if ( eClass != spv::StorageClassPushConstant )
    {
        addDecoration ( result, spv::DecorationDescriptorSet, static_cast< int >( set ) );
        addDecoration ( result, spv::DecorationBinding, static_cast< int >( binding ) );
    }

    return result;
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: acquireCachedLocalVariable (
    KId type, spv::StorageClass eClass, unsigned int nSizeInWords )
{
    if ( eClass == spv::StorageClassFunction )
    {
        detail::KFunctionScope& currentScope = d_functionScopeStack.back();

        ++currentScope.d_activeLocalVariables;

        const auto iCachedVarRange = currentScope.d_cachedLocalVariables.equal_range ( type );

        if ( iCachedVarRange.first != iCachedVarRange.second )
        {
            const KId result = iCachedVarRange.first->second;
            currentScope.d_cachedLocalVariables.erase ( iCachedVarRange.first );
            return result;
        }
        else
            return registerLocalVariable ( type, spv::StorageClassFunction );
    }
    else if ( eClass == spv::StorageClassWorkgroup )
    {
        registerSharedVariableAllocation ( 4*nSizeInWords );
        return registerLocalVariable ( type, eClass );
    }
    else
        return registerLocalVariable ( type, eClass );
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: releaseCachedLocalVariable ( KId type, KId id, spv::StorageClass eClass )
{
    if ( eClass == spv::StorageClassFunction )
    {
        detail::KFunctionScope& currentScope = d_functionScopeStack.back();
        --currentScope.d_activeLocalVariables;
        currentScope.d_cachedLocalVariables.insert ( std::make_pair ( type, id ) );
    }
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerSpecialInputVariable (
    const KShaderScopedVariable* pVariable, KId type, spv::BuiltIn eVariable )
{
    SVariableInfo& varInfo = d_variable2info [ pVariable ];

    if ( ! varInfo )
    {
        varInfo = SVariableInfo ( KId (
            createVariable ( spv::StorageClassInput, type ) ), type );

        addDecoration (
            varInfo.d_variableId, spv::DecorationBuiltIn, eVariable );

        registerInputOutputVariable ( varInfo.d_variableId );
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerSpecialOutputVariable (
    const KShaderScopedVariable* pVariable, KId type, spv::BuiltIn eVariable )
{
    SVariableInfo& varInfo = d_variable2info [ pVariable ];

    if ( ! varInfo )
    {
        varInfo = SVariableInfo ( KId (
            createVariable ( spv::StorageClassOutput, type ) ), type );

        addDecoration (
            varInfo.d_variableId, spv::DecorationBuiltIn, eVariable );

        registerInputOutputVariable ( varInfo.d_variableId );
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerSamplerVariable (
    const KShaderScopedVariable* pVariable, int set, int binding )
{
    SVariableInfo& varInfo = d_variable2info [ pVariable ];

    if ( ! varInfo )
    {
        const KId samplerType ( makeSamplerType() );

        varInfo.d_pointerId = KId (
            createVariable ( spv::StorageClassUniformConstant, samplerType ) );

        varInfo.d_typeId = samplerType;

        varInfo.d_variableId = KId ( createLoad ( varInfo.d_pointerId ) );

        addDecoration ( varInfo.d_pointerId, spv::DecorationDescriptorSet, set );
        addDecoration ( varInfo.d_pointerId, spv::DecorationBinding, binding );
    }

    return varInfo.d_variableId;
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: registerSamplerVariable (
    const KShaderScopedVariable* pVariable, const KId& indexId,
    int set, int binding, int count )
{
    SVariableInfo& varInfo = d_variable2info [ pVariable ];

    if ( varInfo )
    {
        clearAccessChain();
        setAccessChainLValue ( varInfo.d_pointerId );
        accessChainPush ( indexId );
        return KId ( accessChainLoad ( getPrecision(), varInfo.d_typeId ) );
    }
    else
    {
        varInfo.d_typeId = KId ( makeSamplerType() );

        varInfo.d_pointerId = KId ( createVariable (
            spv::StorageClassUniformConstant, makeArrayType (
                varInfo.d_typeId, getArrayIndex ( count ), 0 ) ) );

        addDecoration ( varInfo.d_pointerId, spv::DecorationDescriptorSet, set );
        addDecoration ( varInfo.d_pointerId, spv::DecorationBinding, binding );

        clearAccessChain();
        setAccessChainLValue ( varInfo.d_pointerId );
        accessChainPush ( indexId );

        return KId ( accessChainLoad ( getPrecision(), varInfo.d_typeId ) );
    }
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: pushStructType (
    const std::type_index& index, spv::Decoration decoration )
{
    const StructTypeKey key ( index, decoration );
    auto iTypeInfo = d_structType2Info.find ( key );

    if ( iTypeInfo != d_structType2Info.end() )
        d_structTypeStack.push_back ( & iTypeInfo->second );
    else
    {
        iTypeInfo = d_structType2Info.emplace (
            key, SStructInfo ( index, decoration ) ).first;
        d_structTypeStack.push_back ( & iTypeInfo->second );
    }
}

// -----------------------------------------------------------------------------

KShaderTranslator::SStructInfo& KShaderTranslator :: currentStructType()
{
    return *d_structTypeStack.back();
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: popStructType()
{
    SStructInfo& structInfo = *d_structTypeStack.back();

    if ( ! structInfo.d_bDefined )
    {
        std::vector< spv::Id > memberTypes ( structInfo.d_memberInfos.size() );

        std::transform (
            structInfo.d_memberInfos.begin(),
            structInfo.d_memberInfos.end(),
            memberTypes.begin(),
            []( const SMemberInfo& mi ) { return mi.d_typeId; } );

        structInfo.d_typeId = KId (
            makeStructType ( memberTypes, structInfo.d_index.name() ) );

        for ( unsigned int iMember = 0;
              iMember != structInfo.d_memberInfos.size();
              ++iMember )
        {
            const SMemberInfo& mi = structInfo.d_memberInfos [ iMember ];

            if ( mi.d_bHasHostMember )
            {
                addMemberDecoration (
                    structInfo.d_typeId, iMember,
                    spv::DecorationOffset, static_cast< int >( mi.d_offset ) );

                if ( mi.d_bIsMatrix )
                {
                    addMemberDecoration (
                        structInfo.d_typeId, iMember,
                        ( mi.d_bIsColumnMajor ? spv::DecorationColMajor : spv::DecorationRowMajor ) );

                    addMemberDecoration (
                        structInfo.d_typeId, iMember,
                        spv::DecorationMatrixStride, static_cast< int >( mi.d_matrixStride ) );
                }
            }

            if ( mi.d_flags & IN_FLAT )
                addMemberDecoration ( structInfo.d_typeId, iMember, spv::DecorationFlat );
            if ( mi.d_flags & IN_NOPERSPECTIVE )
                addMemberDecoration ( structInfo.d_typeId, iMember, spv::DecorationNoPerspective );
            if ( mi.d_flags & IN_CENTROID )
                addMemberDecoration ( structInfo.d_typeId, iMember, spv::DecorationCentroid );
            if ( mi.d_flags & IN_SAMPLE )
                addMemberDecoration ( structInfo.d_typeId, iMember, spv::DecorationSample );
            if ( mi.d_flags & VAR_RELAXEDPR )
                addMemberDecoration ( structInfo.d_typeId, iMember, spv::DecorationRelaxedPrecision );
        }

        structInfo.d_bDefined = true;
    }

    d_structTypeStack.pop_back();
}

// -----------------------------------------------------------------------------

const KShaderTranslator::SStructInfo* KShaderTranslator :: findStructType (
    const std::type_index& index, spv::Decoration decoration )
{
    const StructTypeKey key ( index, decoration );
    const auto iTypeInfo = d_structType2Info.find ( key );

    if ( iTypeInfo != d_structType2Info.end() )
        return & iTypeInfo->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------

KShaderTranslator::SVariableInfo& KShaderTranslator :: getShaderScopedVariable (
    const KShaderScopedVariable* pVariable )
{
    return d_variable2info [ pVariable ];
}

// -----------------------------------------------------------------------------

KId KShaderTranslator :: expandConstant ( KId constant, int vectorSize )
{
    if ( vectorSize == 0 )
        return constant;

    std::vector< spv::Id > components ( vectorSize, constant );
    const spv::Id vectorTypeId = makeVectorType ( getTypeId ( constant ), vectorSize );
    return KId ( makeCompositeConstant ( vectorTypeId, components ) );
}

// -----------------------------------------------------------------------------

spv::Decoration KShaderTranslator :: getPrecision() const
{
    return spv::NoPrecision;
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: addMemoryFlags ( const KId& idVariable, std::uint32_t flags )
{
    if ( flags & MEM_COHERENT )
        addDecoration ( idVariable, spv::DecorationCoherent );
    if ( flags & MEM_VOLATILE )
        addDecoration ( idVariable, spv::DecorationVolatile );
    if ( flags & MEM_RESTRICT )
        addDecoration ( idVariable, spv::DecorationRestrict );
    if ( flags & MEM_READONLY )
        addDecoration ( idVariable, spv::DecorationNonWritable );
    if ( flags & MEM_WRITEONLY )
        addDecoration ( idVariable, spv::DecorationNonReadable );
}

// -----------------------------------------------------------------------------

spv::ImageFormat KShaderTranslator :: validateImageFormat ( spv::ImageFormat fmt )
{
    switch ( fmt )
    {
        case spv::ImageFormatRg32f:
        case spv::ImageFormatRg16f:
        case spv::ImageFormatR11fG11fB10f:
        case spv::ImageFormatR16f:
        case spv::ImageFormatRgba16:
        case spv::ImageFormatRgb10A2:
        case spv::ImageFormatRg16:
        case spv::ImageFormatRg8:
        case spv::ImageFormatR16:
        case spv::ImageFormatR8:
        case spv::ImageFormatRgba16Snorm:
        case spv::ImageFormatRg16Snorm:
        case spv::ImageFormatRg8Snorm:
        case spv::ImageFormatR16Snorm:
        case spv::ImageFormatR8Snorm:
        case spv::ImageFormatRg32i:
        case spv::ImageFormatRg16i:
        case spv::ImageFormatRg8i:
        case spv::ImageFormatR16i:
        case spv::ImageFormatR8i:
        case spv::ImageFormatRgb10a2ui:
        case spv::ImageFormatRg32ui:
        case spv::ImageFormatRg16ui:
        case spv::ImageFormatRg8ui:
        case spv::ImageFormatR16ui:
        case spv::ImageFormatR8ui:
            addCapability ( spv::CapabilityStorageImageExtendedFormats );
            break;
    }

    return fmt;
}

// -----------------------------------------------------------------------------

void KShaderTranslator :: requireVersion11()
{
    if ( ! d_bDeviceSupportsVulkan11 )
        throw XUsageError (
            "A feature has been used which requires Vulkan version not supported by the device: 1.1" );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
