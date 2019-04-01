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
#include "../include/vppRenderPass.hpp"
#include "../include/vppPipelineLayout.hpp"
#include "../include/vppRenderingOptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

RenderPassImpl::SPipelineData :: SPipelineData() :
    d_processIndex ( 0 ),
    d_pipelineIndex ( 0 )
{
    std::memset ( & d_vertexInputInfo, 0, sizeof ( d_vertexInputInfo ) );
    std::memset ( & d_inputAssemblyInfo, 0, sizeof ( d_inputAssemblyInfo ) );
    std::memset ( & d_tessellationInfo, 0, sizeof ( d_tessellationInfo ) );
    std::memset ( & d_viewportInfo, 0, sizeof ( d_viewportInfo ) );
    std::memset ( & d_rasterizationInfo, 0, sizeof ( d_rasterizationInfo ) );
    std::memset ( & d_multisampleInfo, 0, sizeof ( d_multisampleInfo ) );
    std::memset ( & d_depthStencilInfo, 0, sizeof ( d_depthStencilInfo ) );
    std::memset ( & d_colorBlendInfo, 0, sizeof ( d_colorBlendInfo ) );
    std::memset ( & d_dynamicInfo, 0, sizeof ( d_dynamicInfo ) );
}

// -----------------------------------------------------------------------------

const Pipeline& RenderPass :: pipeline ( std::uint32_t iProcess, std::uint32_t iPipeline ) const
{
    return get()->d_pipelineData [ iProcess ][ iPipeline ].d_pipeline;
}

// -----------------------------------------------------------------------------

const Pipeline& RenderPass :: pipeline ( const Process& hProcess, std::uint32_t iPipeline ) const
{
    return pipeline ( hProcess.index(), iPipeline );
}

// -----------------------------------------------------------------------------

std::uint32_t RenderPass :: addPipeline (
    std::uint32_t iProcess,
    const PipelineLayoutBase& layout,
    const RenderingOptions& options )
{
    RenderPassImpl::ProcessPipelineData& ppd = get()->d_pipelineData [ iProcess ];
    const std::uint32_t pipelineIndex = static_cast< std::uint32_t >( ppd.size() );

    ppd.push_back ( RenderPassImpl::SPipelineData() );
    RenderPassImpl::SPipelineData& data = ppd.back();

    data.d_layout = layout;
    data.d_options = options;

    return pipelineIndex;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderPass :: addPipeline (
    const Process& hProcess,
    const PipelineLayoutBase& layout,
    const RenderingOptions& options )
{
    return addPipeline ( hProcess.index(), layout, options );
}

// -----------------------------------------------------------------------------

void RenderPass :: beginRendering()
{
    get()->createPipelines();
}

// -----------------------------------------------------------------------------

void RenderPass :: endRendering()
{
}

// -----------------------------------------------------------------------------

void RenderPass :: createPipelines()
{
    get()->createPipelines();
}

// -----------------------------------------------------------------------------

RenderPassImpl :: RenderPassImpl (
    const RenderGraph& renderGraph,
    const Device& hDevice,
    const PipelineCache& hPipelineCache ) :
        d_graph ( renderGraph ),
        d_hDevice ( hDevice ),
        d_hPipelineCache ( hPipelineCache ),
        d_handle(),
        d_result(),
        d_bPipelinesCreated ( false )
{
    const size_t nProcesses = d_graph.getProcessCount();
    d_pipelineData.resize ( nProcesses );

    const KAttachmentConfig& attCfg = d_graph.getAttachmentConfig();
    const SubpassDescriptions& spDes = d_graph.getSubpassDescriptions();
    const SubpassDependencies& spDep = d_graph.getSubpassDependencies();

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = 0;
    renderPassCreateInfo.flags = 0;

    renderPassCreateInfo.attachmentCount = attCfg.getDescriptionCount();
    renderPassCreateInfo.pAttachments = attCfg.getDescriptionArray();

    renderPassCreateInfo.subpassCount = static_cast< std::uint32_t >( spDes.size() );
    renderPassCreateInfo.pSubpasses = & spDes [ 0 ];

    renderPassCreateInfo.dependencyCount = static_cast< std::uint32_t >( spDep.size() );
    renderPassCreateInfo.pDependencies = & spDep [ 0 ];

    d_result = ::vkCreateRenderPass (
        d_hDevice.handle(), & renderPassCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

RenderPassImpl :: ~RenderPassImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyRenderPass ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

void RenderPassImpl :: createPipelines()
{
    if ( d_bPipelinesCreated )
        return;

    d_bPipelinesCreated = true;

    size_t nPipelines = 0;
    
    for ( auto iProcessPipelines = d_pipelineData.begin();
          iProcessPipelines != d_pipelineData.end(); ++iProcessPipelines )
    {
        nPipelines += iProcessPipelines->size();
    }

    d_pipelineCreateInfos.resize ( nPipelines );

    for ( size_t iProcess = 0, iDest = 0; iProcess != d_pipelineData.size(); ++iProcess )
        for ( size_t iPipeline = 0; iPipeline != d_pipelineData [ iProcess ].size(); ++iPipeline, ++iDest )
            preparePipelineCreateInfo ( iProcess, iPipeline, iDest );

    std::vector< VkPipeline > pipelineHandles ( nPipelines, VK_NULL_HANDLE );

    VkResult result = ::vkCreateGraphicsPipelines (
        d_hDevice.handle(),
        d_hPipelineCache.handle(),
        static_cast< std::uint32_t >( nPipelines ),
        & d_pipelineCreateInfos [ 0 ],
        0,
        & pipelineHandles [ 0 ] );

    for ( size_t iProcess = 0, iDest = 0; iProcess != d_pipelineData.size(); ++iProcess )
        for ( size_t iPipeline = 0; iPipeline != d_pipelineData [ iProcess ].size(); ++iPipeline, ++iDest )
            if ( pipelineHandles [ iDest ] != VK_NULL_HANDLE )
                d_pipelineData [ iProcess ][ iPipeline ].d_pipeline =
                    Pipeline ( pipelineHandles [ iDest ], d_hDevice );
}

// -----------------------------------------------------------------------------

void RenderPassImpl :: preparePipelineCreateInfo (
    size_t iProcess, size_t iPipeline, size_t iDest )
{
    SPipelineData& pipelineData = d_pipelineData [ iProcess ][ iPipeline ];
    VkGraphicsPipelineCreateInfo* pInfo = & d_pipelineCreateInfos [ iDest ];
    const PipelineConfig& config = pipelineData.d_layout.config();

    pInfo->sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pInfo->pNext = 0;
    pInfo->flags = 0;

    pipelineData.d_processIndex = static_cast< std::uint32_t >( iProcess );
    pipelineData.d_pipelineIndex = static_cast< std::uint32_t >( iPipeline );

    const PipelineConfig::ShaderTable& shaderTable = pipelineData.d_layout.getShaderTable();
    pInfo->stageCount = static_cast< std::uint32_t >( shaderTable.size() );
    pInfo->pStages = & shaderTable [ 0 ];

    config.initVertexInputCreateInfo ( & pipelineData.d_vertexInputInfo );
    pInfo->pVertexInputState = & pipelineData.d_vertexInputInfo;

    config.initInputAssemblyStateCreateInfo ( & pipelineData.d_inputAssemblyInfo );
    pInfo->pInputAssemblyState = & pipelineData.d_inputAssemblyInfo;

    if ( config.initTessellationStateCreateInfo ( & pipelineData.d_tessellationInfo ) )
        pInfo->pTessellationState = & pipelineData.d_tessellationInfo;
    else
        pInfo->pTessellationState = 0;

    RenderingOptions& options = pipelineData.d_options;

    options.initViewportInfo ( & pipelineData.d_viewportInfo );
    pInfo->pViewportState = & pipelineData.d_viewportInfo;

    options.initRasterizationInfo ( & pipelineData.d_rasterizationInfo );
    pInfo->pRasterizationState = & pipelineData.d_rasterizationInfo;

    options.initMultisampleInfo ( & pipelineData.d_multisampleInfo );
    pInfo->pMultisampleState = & pipelineData.d_multisampleInfo;

    options.initDepthStencilInfo ( & pipelineData.d_depthStencilInfo );
    pInfo->pDepthStencilState = & pipelineData.d_depthStencilInfo;

    config.initColorBlendStateInfo ( & pipelineData.d_colorBlendInfo );
    pInfo->pColorBlendState = & pipelineData.d_colorBlendInfo;

    if ( config.initDynamicStateInfo ( & pipelineData.d_dynamicInfo ) )
        pInfo->pDynamicState = & pipelineData.d_dynamicInfo;
    else
        pInfo->pDynamicState = 0;

    pInfo->layout = pipelineData.d_layout.handle();
    pInfo->renderPass = d_handle;
    pInfo->subpass = pipelineData.d_processIndex;
    pInfo->basePipelineHandle = 0;
    pInfo->basePipelineIndex = 0;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
