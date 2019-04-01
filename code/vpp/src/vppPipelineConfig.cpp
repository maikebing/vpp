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
#include "../include/vppPipelineConfig.hpp"
#include "../include/vppRenderGraphNodes.hpp"
#include "../include/vppShader.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

KDescriptor :: KDescriptor (
    VkDescriptorType type,
    std::uint32_t set,
    int binding,
    std::uint32_t count ) :
        d_type ( type ),
        d_id ( PipelineConfig::getInstance()->createNewResource() ),
        d_set ( set ),
        d_count ( count )
{
    if ( binding != -1 )
    {
        d_binding = static_cast< std::uint32_t >( binding );
        d_bAutomaticBinding = false;
        PipelineConfig::getInstance()->setBinding ( this, binding );
    }
    else
    {
        d_binding = PipelineConfig::getInstance()->allocateBinding ( this );
        d_bAutomaticBinding = true;
    }

    SResourceInfo& resInfo = PipelineConfig::getInstance()->getResource ( d_id );
    resInfo.descriptorType = type;
    resInfo.d_set = d_set;
    resInfo.binding = d_binding;
    resInfo.descriptorCount = count;
}

// -----------------------------------------------------------------------------

KDescriptor :: ~KDescriptor()
{
}

// -----------------------------------------------------------------------------

void KDescriptor :: rebind ( std::uint32_t newBinding )
{
    d_binding = newBinding;

    SResourceInfo& resInfo = PipelineConfig::getInstance()->getResource ( d_id );
    resInfo.binding = d_binding;
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

thread_local PipelineConfig* PipelineConfig :: d_pThis = 0;

// -----------------------------------------------------------------------------

PipelineConfig :: PipelineConfig ( const Process& boundProcess ) :
    TSharedReference< PipelineConfigImpl >( new PipelineConfigImpl ( boundProcess ) )
{
    d_pThis = this;
}

// -----------------------------------------------------------------------------

PipelineConfig :: PipelineConfig ( EComputePipelineTag ) :
    TSharedReference< PipelineConfigImpl >( new PipelineConfigImpl() )
{
    d_pThis = this;
}

// -----------------------------------------------------------------------------

PipelineConfig :: ~PipelineConfig()
{
}

// -----------------------------------------------------------------------------

PipelineConfig* PipelineConfig :: getInstance()
{
    return d_pThis;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: setBlendingMode (
    const BaseAttachment& dataNode,
    const VkPipelineColorBlendAttachmentState& blendConfig )
{
    PipelineConfigImpl* pImpl = get();

    const SProcessInfo& processInfo =
        pImpl->d_renderGraph.getProcessInfo ( pImpl->d_processIndex );

    for ( size_t i = 0;
          i < processInfo.d_outputColorNodes.size() && i < pImpl->d_blendModeTable.size();
          ++i )
    {
        if ( processInfo.d_outputColorNodes [ i ].attachment == dataNode.index() )
        {
            pImpl->d_blendModeTable [ i ] = blendConfig;
            break;
        }
    }
}

// -----------------------------------------------------------------------------

void PipelineConfig :: enableLogicOperation ( bool bEnable, VkLogicOp logicOp )
{
    PipelineConfigImpl* pImpl = get();
    pImpl->d_logicOpEnable = bEnable ? VK_TRUE : VK_FALSE;
    pImpl->d_logicOp = logicOp;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: setPrimitiveTopology ( VkPrimitiveTopology v )
{
    get()->d_topology = v;
}

// -----------------------------------------------------------------------------

VkPrimitiveTopology PipelineConfig :: getPrimitiveTopology() const
{
    return get()->d_topology;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: setEnablePrimitiveRestart ( bool v )
{
    get()->d_bEnablePrimitiveRestart = v;
}

// -----------------------------------------------------------------------------

bool PipelineConfig :: getEnablePrimitiveRestart() const
{
    return get()->d_bEnablePrimitiveRestart;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: setTessPatchControlPoints ( std::uint32_t v )
{
    get()->d_tessPatchControlPoints = v;
}

// -----------------------------------------------------------------------------

std::uint32_t PipelineConfig :: getTessPatchControlPoints() const
{
    return get()->d_tessPatchControlPoints;
}

// -----------------------------------------------------------------------------

std::uint32_t PipelineConfig :: createNewResource()
{
    const std::uint32_t newId = static_cast< std::uint32_t >( get()->d_id2resourceDefinition.size() );
    get()->d_id2resourceDefinition.push_back ( detail::SResourceInfo() );
    detail::SResourceInfo& resourceDef = get()->d_id2resourceDefinition.back();
    std::memset ( & resourceDef, 0, sizeof ( detail::SResourceInfo ) );
    return newId;
}

// -----------------------------------------------------------------------------

std::uint32_t PipelineConfig :: createNewConstant()
{
    const std::uint32_t newId = static_cast< std::uint32_t >( get()->d_id2constant.size() );
    get()->d_id2constant.push_back ( VkPushConstantRange() );
    VkPushConstantRange& constantDef = get()->d_id2constant.back();
    std::memset ( & constantDef, 0, sizeof ( VkPushConstantRange ) );
    return newId;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: createVertexSource (
    const void* pBase, std::uint32_t stride, bool bInstanceScope )
{
    auto pThis = get();
    const std::uint32_t index = static_cast< std::uint32_t >( pThis->d_id2vertexSource.size() );
    pThis->d_id2vertexSource.push_back ( detail::SVertexSourceInfo() );
    pThis->d_id2vertexBinding.push_back ( VkVertexInputBindingDescription() );

    detail::SVertexSourceInfo& vsource = get()->d_id2vertexSource.back();
    VkVertexInputBindingDescription& vbinding = get()->d_id2vertexBinding.back();

    vbinding.binding = index;
    vbinding.stride = stride;
    vbinding.inputRate = bInstanceScope ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
    vsource.d_pBaseOffset = reinterpret_cast< const unsigned char* >( pBase );
}

// -----------------------------------------------------------------------------

const detail::SVertexSourceInfo* PipelineConfig :: findVertexSource ( const void* pBase ) const
{
    auto pThis = get();

    for ( auto& iSource : pThis->d_id2vertexSource )
        if ( iSource.d_pBaseOffset == pBase )
            return & iSource;

    return 0;
}

// -----------------------------------------------------------------------------

const VkVertexInputBindingDescription* PipelineConfig :: findVertexBinding ( const void* pBase ) const
{
    auto pThis = get();
    const size_t nBindings = pThis->d_id2vertexSource.size();

    for ( size_t iBinding = 0; iBinding != nBindings; ++iBinding )
    {
        const detail::SVertexSourceInfo& sourceInfo = pThis->d_id2vertexSource [ iBinding ];

        if ( sourceInfo.d_pBaseOffset == pBase )
            return & pThis->d_id2vertexBinding [ iBinding ];
    }

    return 0;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: createVertexDataField ( const void* pOffset, VkFormat format )
{
    auto pThis = get();
    detail::SVertexSourceInfo& vsource = pThis->d_id2vertexSource.back();

    pThis->d_id2vertexField.push_back ( detail::SVertexFieldInfo ( vsource.d_pBaseOffset ) );
    pThis->d_id2vertexFieldDesc.push_back ( VkVertexInputAttributeDescription() );

    VkVertexInputAttributeDescription& vfield = pThis->d_id2vertexFieldDesc.back();

    vfield.binding = static_cast< std::uint32_t >( pThis->d_id2vertexSource.size() - 1 );
    vfield.format = format;

    vfield.location = pThis->d_vertexInputLocationCounter;

    switch ( format )
    {
        case VK_FORMAT_R64G64B64_UINT:
        case VK_FORMAT_R64G64B64_SINT:
        case VK_FORMAT_R64G64B64_SFLOAT:
        case VK_FORMAT_R64G64B64A64_UINT:
        case VK_FORMAT_R64G64B64A64_SINT:
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            pThis->d_vertexInputLocationCounter += 2;
            break;

        default:
            ++pThis->d_vertexInputLocationCounter;
            break;
    }

    vfield.offset = 
        static_cast< std::uint32_t >( 
            reinterpret_cast< const unsigned char* >( pOffset ) - vsource.d_pBaseOffset );
}

// -----------------------------------------------------------------------------

PipelineConfig::VertexFieldInfo PipelineConfig :: findVertexDataField ( const void* pOffset )
{
    PipelineConfigImpl* pThis = get();

    const size_t nFields = pThis->d_id2vertexField.size();

    for ( size_t iField = 0; iField != nFields; ++iField )
        if ( ( pThis->d_id2vertexField [ iField ].d_pBaseOffset
                + pThis->d_id2vertexFieldDesc [ iField ].offset ) == pOffset )
        {
            return std::make_pair (
                & pThis->d_id2vertexField [ iField ],
                & pThis->d_id2vertexFieldDesc [ iField ] );
        }

    return std::make_pair ( nullptr, nullptr );
}

// -----------------------------------------------------------------------------

std::uint32_t PipelineConfig :: allocateBinding ( detail::KDescriptor* pDescriptor )
{
    PipelineConfigImpl* pImpl = get();
    const std::uint32_t set = pDescriptor->set();
    const std::uint32_t binding = pImpl->findFirstFreeBinding ( set );
    const PipelineConfigImpl::SetBinding key ( set, binding );
    pImpl->d_setBinding2descriptor [ key ] = pDescriptor;
    return binding;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: setBinding (
    detail::KDescriptor* pDescriptor, std::uint32_t binding )
{
    PipelineConfigImpl* pImpl = get();
    const std::uint32_t set = pDescriptor->set();
    const PipelineConfigImpl::SetBinding key ( set, binding );

    const auto iExisting = pImpl->d_setBinding2descriptor.find ( key );

    if ( iExisting != pImpl->d_setBinding2descriptor.end()
         && iExisting->second->isAutomaticBinding() )
    {
        const std::uint32_t newBinding = pImpl->findFirstFreeBinding ( set );
        const PipelineConfigImpl::SetBinding newKey ( set, newBinding );

        iExisting->second->rebind ( newBinding );
        pImpl->d_setBinding2descriptor [ newKey ] = iExisting->second;
    }

    pImpl->d_setBinding2descriptor [ key ] = pDescriptor;
}

// -----------------------------------------------------------------------------

const VkSampler* PipelineConfig :: addImmutableSamplers (
    const std::vector< NormalizedSampler >& samplers, std::uint32_t count )
{
    if ( samplers.empty() )
        return 0;

    PipelineConfigImpl* pImpl = get();
    pImpl->d_immutableSamplers.insert ( samplers.begin(), samplers.end() );

    std::vector< VkSampler > handles ( samplers.size() );

    std::transform (
        samplers.begin(), samplers.end(),
        handles.begin(), []( const NormalizedSampler& s ){ return s.handle(); } );

    if ( handles.size() != count )
        handles.resize ( count, handles.back() );

    auto iInserted = pImpl->d_immutableSamplerHandles.insert ( handles ).first;
    return & ( *iInserted )[ 0 ];
}

// -----------------------------------------------------------------------------

const VkSampler* PipelineConfig :: addImmutableSamplers (
    const std::vector< UnnormalizedSampler >& samplers, std::uint32_t count )
{
    if ( samplers.empty() )
        return 0;

    PipelineConfigImpl* pImpl = get();
    pImpl->d_immutableSamplers.insert ( samplers.begin(), samplers.end() );

    std::vector< VkSampler > handles ( samplers.size() );

    std::transform (
        samplers.begin(), samplers.end(),
        handles.begin(), []( const UnnormalizedSampler& s ){ return s.handle(); } );

    if ( handles.size() != count )
        handles.resize ( count, handles.back() );

    auto iInserted = pImpl->d_immutableSamplerHandles.insert ( handles ).first;
    return & ( *iInserted )[ 0 ];
}

// -----------------------------------------------------------------------------

void PipelineConfig :: pushStructType ( const std::type_index& index )
{
    PipelineConfigImpl* pImpl = get();

    auto iTypeInfo = pImpl->d_struct2LocationInfo.find ( index );

    if ( iTypeInfo != pImpl->d_struct2LocationInfo.end() )
        pImpl->d_structTypeStack.push_back ( & iTypeInfo->second );
    else
    {
        iTypeInfo = pImpl->d_struct2LocationInfo.emplace ( index, detail::SStructLocationInfo() ).first;
        pImpl->d_structTypeStack.push_back ( & iTypeInfo->second );
    }
}

// -----------------------------------------------------------------------------

detail::SStructLocationInfo& PipelineConfig :: currentStructType()
{
    PipelineConfigImpl* pImpl = get();
    return *pImpl->d_structTypeStack.back();
}

// -----------------------------------------------------------------------------

void PipelineConfig :: popStructType()
{
    PipelineConfigImpl* pImpl = get();

    detail::SStructLocationInfo& structInfo = *pImpl->d_structTypeStack.back();
    structInfo.d_bDefined = true;

    pImpl->d_structTypeStack.pop_back();
}

// -----------------------------------------------------------------------------

const detail::SStructLocationInfo* PipelineConfig :: findStructType (
    const std::type_index& index )
{
    PipelineConfigImpl* pImpl = get();

    const auto iTypeInfo = pImpl->d_struct2LocationInfo.find ( index );

    if ( iTypeInfo != pImpl->d_struct2LocationInfo.end() )
        return & iTypeInfo->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: registerDebugProbe ( const HDebugProbe& hDebugProbe )
{
    PipelineConfigImpl* pImpl = get();
    pImpl->d_debugProbes.push_back ( hDebugProbe );
}

// -----------------------------------------------------------------------------

void PipelineConfig :: bindDebugProbes ( const ShaderDataBlock& hDataBlock ) const
{
    PipelineConfigImpl* pImpl = get();

    for ( auto& iProbe : pImpl->d_debugProbes )
        iProbe->bind ( hDataBlock );
}

// -----------------------------------------------------------------------------

void PipelineConfig :: addShader (
    ShaderTable* pShaderTable,
    KShader* pShader,
    Device hDevice,
    SDynamicParameters* pDynamicParameters ) const
{
    KShaderModule hModule = pShader->compile ( hDevice, pDynamicParameters );

    pShaderTable->push_back (
        VkPipelineShaderStageCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            0, 0, pShader->stage(), hModule.handle(), "main", 0
        }
    );

    get()->d_shaderModules.insert ( hModule );
}

// -----------------------------------------------------------------------------

void PipelineConfig :: fillShaderTable ( ShaderTable* pShaderTable, Device hDevice ) const
{
    PipelineConfigImpl* pImpl = get();

    if ( pImpl->d_pGeometryShader )
    {
        // Ensure requirements from:
        // 22.1. Geometry Shader Input Primitives
        EGeometryInputTopology topology = GTI_TRIANGLES;

        if ( pImpl->d_pTessControlShader && pImpl->d_pTessEvalShader )
        {
            if ( pImpl->d_pTessEvalShader->isPointMode() )
                topology = GTI_POINTS;
            else if ( pImpl->d_pTessControlShader->getTopology() == TT_ISOLINES )
                topology = GTI_LINES;
            else
                topology = GTI_TRIANGLES;

            // 9.2. Graphics Pipelines valid usage
            get()->d_topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        }
        else
        {
            switch ( getPrimitiveTopology() )
            {
                case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:
                    topology = GTI_POINTS;
                    break;

                case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
                case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
                    topology = GTI_LINES;
                    break;

                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
                    topology = GTI_TRIANGLES;
                    break;

                case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:
                case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:
                    topology = GTI_ADJLINES;
                    break;

                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY:
                    topology = GTI_ADJTRIANGLES;
                    break;

                case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST:
                    throw XUsageError (
                        "Patch primitive topology has been specified, but no tessellation shaders defined." );
                    break;
            }
        }

        pImpl->d_pGeometryShader->setInputTopology ( topology );
    }

    // Compile the shaders.
    SDynamicParameters dynamicParameters;
    dynamicParameters.d_clipDistancesSize = 0;
    dynamicParameters.d_cullDistancesSize = 0;

    if ( pImpl->d_pVertexShader )
        addShader ( pShaderTable, pImpl->d_pVertexShader, hDevice, & dynamicParameters );

    if ( pImpl->d_pTessControlShader && pImpl->d_pTessEvalShader )
    {
        addShader ( pShaderTable, pImpl->d_pTessControlShader, hDevice, & dynamicParameters );
        addShader ( pShaderTable, pImpl->d_pTessEvalShader, hDevice, & dynamicParameters );
    }

    if ( pImpl->d_pGeometryShader )
        addShader ( pShaderTable, pImpl->d_pGeometryShader, hDevice, & dynamicParameters );

    if ( pImpl->d_pFragmentShader )
        addShader ( pShaderTable, pImpl->d_pFragmentShader, hDevice, & dynamicParameters );

    if ( pImpl->d_pComputeShader )
        addShader ( pShaderTable, pImpl->d_pComputeShader, hDevice, & dynamicParameters );
}

// -----------------------------------------------------------------------------

void PipelineConfig :: initVertexInputCreateInfo (
    VkPipelineVertexInputStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;

    pDest->vertexBindingDescriptionCount =
        static_cast< std::uint32_t >( get()->d_id2vertexSource.size() );
    
    pDest->pVertexBindingDescriptions = (
        pDest->vertexBindingDescriptionCount ? 
            & get()->d_id2vertexBinding [ 0 ] : nullptr );

    pDest->vertexAttributeDescriptionCount =
        static_cast< std::uint32_t >( get()->d_id2vertexField.size() );

    pDest->pVertexAttributeDescriptions = (
        pDest->vertexAttributeDescriptionCount ?
            & get()->d_id2vertexFieldDesc [ 0 ] : nullptr );
}

// -----------------------------------------------------------------------------

void PipelineConfig :: initInputAssemblyStateCreateInfo (
    VkPipelineInputAssemblyStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->topology = get()->d_topology;
    pDest->primitiveRestartEnable = get()->d_bEnablePrimitiveRestart;
}

// -----------------------------------------------------------------------------

bool PipelineConfig :: initTessellationStateCreateInfo (
    VkPipelineTessellationStateCreateInfo* pDest ) const
{
    if ( ! get()->d_pTessControlShader )
        return false;

    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->patchControlPoints = get()->d_tessPatchControlPoints;

    return true;
}

// -----------------------------------------------------------------------------

void PipelineConfig :: initColorBlendStateInfo (
    VkPipelineColorBlendStateCreateInfo* pDest ) const
{
    PipelineConfigImpl* pImpl = get();

    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->logicOpEnable = pImpl->d_logicOpEnable;
    pDest->logicOp = pImpl->d_logicOp;
    pDest->blendConstants [ 0 ] = 0.0f;
    pDest->blendConstants [ 1 ] = 0.0f;
    pDest->blendConstants [ 2 ] = 0.0f;
    pDest->blendConstants [ 3 ] = 0.0f;

    pDest->attachmentCount = static_cast< std::uint32_t >( pImpl->d_blendModeTable.size() );
    pDest->pAttachments = & pImpl->d_blendModeTable [ 0 ];
}

// -----------------------------------------------------------------------------

bool PipelineConfig :: initDynamicStateInfo (
    VkPipelineDynamicStateCreateInfo* pDest ) const
{
    PipelineConfigImpl* pImpl = get();

    if ( pImpl->d_dynamicStates.empty() )
        return false;
    else
    {
        pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pDest->pNext = 0;
        pDest->flags = 0;
        pDest->dynamicStateCount = static_cast< std::uint32_t >( pImpl->d_dynamicStates.size() );
        pDest->pDynamicStates = & pImpl->d_dynamicStates [ 0 ];
        return true;
    }
}

// -----------------------------------------------------------------------------

void PipelineConfig :: cmdBindIndexInput (
    const VertexIndexBufferView& hVertexIndexBufferView,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindIndexBuffer (
        hCmdBuffer,
        hVertexIndexBufferView.buffer().handle(),
        hVertexIndexBufferView.offset(),
        hVertexIndexBufferView.type()
    );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

PipelineConfigImpl :: PipelineConfigImpl() :
    d_processIndex ( 0 ),
    d_pVertexShader ( 0 ),
    d_pTessControlShader ( 0 ),
    d_pTessEvalShader ( 0 ),
    d_pGeometryShader ( 0 ),
    d_pFragmentShader ( 0 ),
    d_pComputeShader ( 0 ),
    d_topology ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST ),
    d_tessPatchControlPoints ( 0 ),
    d_bEnablePrimitiveRestart ( false ),
    d_vertexInputLocationCounter ( 0 ),
    d_logicOpEnable ( VK_FALSE ),
    d_logicOp ( VK_LOGIC_OP_CLEAR )
{
}

// -----------------------------------------------------------------------------

PipelineConfigImpl :: PipelineConfigImpl ( const Process& boundProcess ) :
    d_renderGraph ( *boundProcess.graph() ),
    d_processIndex ( boundProcess.index() ),
    d_pVertexShader ( 0 ),
    d_pTessControlShader ( 0 ),
    d_pTessEvalShader ( 0 ),
    d_pGeometryShader ( 0 ),
    d_pFragmentShader ( 0 ),
    d_pComputeShader ( 0 ),
    d_topology ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST ),
    d_tessPatchControlPoints ( 0 ),
    d_bEnablePrimitiveRestart ( false ),
    d_vertexInputLocationCounter ( 0 ),
    d_logicOpEnable ( VK_FALSE ),
    d_logicOp ( VK_LOGIC_OP_CLEAR )
{
    const SProcessInfo& processInfo = d_renderGraph.getProcessInfo ( d_processIndex );
    d_blendModeTable.resize ( processInfo.d_outputColorNodes.size() );

    for ( size_t i = 0; i != d_blendModeTable.size(); ++i )
        d_blendModeTable [ i ] = BlendConfig();
}

// -----------------------------------------------------------------------------

PipelineConfigImpl :: ~PipelineConfigImpl()
{
}

// -----------------------------------------------------------------------------

std::uint32_t PipelineConfigImpl :: findFirstFreeBinding ( std::uint32_t set ) const
{
    auto iCurrent = d_setBinding2descriptor.lower_bound ( SetBinding ( set, 0 ) );
    auto iEnd = d_setBinding2descriptor.lower_bound ( SetBinding ( set + 1, 0 ) );
    auto iPrev = iEnd;

    while ( iCurrent != iEnd )
    {
        const std::uint32_t contBinding = ( iPrev != iEnd ? iPrev->first.second + 1 : 0 );

        if ( iCurrent->first.second > contBinding )
            return contBinding;

        iPrev = iCurrent++;
    }

    return ( iPrev != iEnd ? iPrev->first.second + 1 : 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
