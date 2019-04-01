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
#include "../include/vppComputePass.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

const ComputePipeline& ComputePass :: pipeline ( std::uint32_t iPipeline ) const
{
    return get()->d_pipelineData [ iPipeline ].d_pipeline;
}

// -----------------------------------------------------------------------------

void ComputePass :: operator<< ( const std::function< void () >& cmds )
{
    get()->d_commands.push_back ( cmds );
}

// -----------------------------------------------------------------------------

void ComputePass :: cmdDispatch (
    std::uint32_t x, std::uint32_t y, std::uint32_t z,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdDispatch ( hCmdBuffer, x, y, z );
}

// -----------------------------------------------------------------------------

void ComputePass :: cmdDispatchIndirect (
    const IndirectBufferView& data,
    unsigned int index = 0,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdDispatchIndirect (
        hCmdBuffer, data.buffer().handle(), index * sizeof ( VkDispatchIndirectCommand ) );
}

// -----------------------------------------------------------------------------

ComputePassImpl :: ComputePassImpl (
    const Device& hDevice,
    const PipelineCache& hPipelineCache ) :
        d_hDevice ( hDevice ),
        d_hPipelineCache ( hPipelineCache ),
        d_bPipelinesCreated ( false )
{
}

// -----------------------------------------------------------------------------

ComputePassImpl :: ~ComputePassImpl()
{
}

// -----------------------------------------------------------------------------

void ComputePassImpl :: createPipelines()
{
    if ( d_bPipelinesCreated )
        return;

    d_bPipelinesCreated = true;

    const size_t nPipelines = d_pipelineData.size();
    
    d_pipelineCreateInfos.resize ( nPipelines );

    for ( size_t iPipeline = 0; iPipeline != d_pipelineData.size(); ++iPipeline )
        preparePipelineCreateInfo ( iPipeline );

    std::vector< VkPipeline > pipelineHandles ( nPipelines, VK_NULL_HANDLE );

    VkResult result = ::vkCreateComputePipelines (
        d_hDevice.handle(),
        d_hPipelineCache.handle(),
        static_cast< std::uint32_t >( nPipelines ),
        & d_pipelineCreateInfos [ 0 ],
        0,
        & pipelineHandles [ 0 ] );

    for ( size_t iPipeline = 0; iPipeline != d_pipelineData.size(); ++iPipeline )
        if ( pipelineHandles [ iPipeline ] != VK_NULL_HANDLE )
            d_pipelineData [ iPipeline ].d_pipeline =
                ComputePipeline ( pipelineHandles [ iPipeline ], d_hDevice );
}

// -----------------------------------------------------------------------------

void ComputePassImpl :: preparePipelineCreateInfo ( size_t iPipeline )
{
    SPipelineData& pipelineData = d_pipelineData [ iPipeline ];
    VkComputePipelineCreateInfo* pInfo = & d_pipelineCreateInfos [ iPipeline ];
    const PipelineConfig& config = pipelineData.d_layout.config();

    pInfo->sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pInfo->pNext = 0;
    pInfo->flags = 0;

    pipelineData.d_pipelineIndex = static_cast< std::uint32_t >( iPipeline );

    const PipelineConfig::ShaderTable& shaderTable = pipelineData.d_layout.getShaderTable();
    pInfo->stage = shaderTable [ 0 ];

    pInfo->layout = pipelineData.d_layout.handle();
    pInfo->basePipelineHandle = 0;
    pInfo->basePipelineIndex = 0;
}

// -----------------------------------------------------------------------------

std::uint32_t ComputePassImpl :: createPipelineData ( const PipelineLayoutBase& lt )
{
    const std::uint32_t pipelineIndex =
        static_cast< std::uint32_t >( d_pipelineData.size() );

    d_pipelineData.push_back ( SPipelineData() );

    SPipelineData& data = d_pipelineData.back();
    data.d_layout = lt;

    return pipelineIndex;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
