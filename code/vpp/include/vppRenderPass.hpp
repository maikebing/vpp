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

#ifndef INC_VPPRENDERPASS_HPP
#define INC_VPPRENDERPASS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINELAYOUT_HPP
#include "vppPipelineLayout.hpp"
#endif

#ifndef INC_VPPRENDERGRAPH_HPP
#include "vppRenderGraph.hpp"
#endif

#ifndef INC_VPPRENDERGRAPHNODES_HPP
#include "vppRenderGraphNodes.hpp"
#endif

#ifndef INC_VPPPIPELINECACHE_HPP
#include "vppPipelineCache.hpp"
#endif

#ifndef INC_VPPRENDERINGOPTIONS_HPP
#include "vppRenderingOptions.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class RenderPassImpl;

// -----------------------------------------------------------------------------

class RenderPass : public TSharedReference< RenderPassImpl >
{
public:
    RenderPass (
        const RenderGraph& renderGraph,
        const Device& hDevice );

    RenderPass (
        const RenderGraph& renderGraph,
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    ~RenderPass();

    VkRenderPass handle() const;
    RenderGraph& graph() const;
    const Device& device() const;
    const PipelineCache& pipelineCache() const;

    VPP_DLLAPI const Pipeline& pipeline ( std::uint32_t iProcess, std::uint32_t iPipeline ) const;
    VPP_DLLAPI const Pipeline& pipeline ( const Process& hProcess, std::uint32_t iPipeline ) const;

    VPP_DLLAPI std::uint32_t addPipeline (
        std::uint32_t iProcess,
        const PipelineLayoutBase& layout,
        const RenderingOptions& options );

    VPP_DLLAPI std::uint32_t addPipeline (
        const Process& hProcess,
        const PipelineLayoutBase& layout,
        const RenderingOptions& options );

    VPP_DLLAPI void beginRendering();
    VPP_DLLAPI void endRendering();
    VPP_DLLAPI void createPipelines();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class RenderPassImpl : public TSharedObject< RenderPassImpl >
{
public:
    VPP_DLLAPI RenderPassImpl (
        const RenderGraph& renderGraph,
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    VPP_DLLAPI ~RenderPassImpl();

    VPP_INLINE bool compareObjects ( const RenderPassImpl* pRHS ) const
    {
        return this < pRHS;
    }

    VPP_DLLAPI void createPipelines();
    VPP_DLLAPI void preparePipelineCreateInfo ( size_t iProcess, size_t iPipeline, size_t iDest );
    
private:
    friend class RenderPass;
    RenderGraph d_graph;
    Device d_hDevice;
    PipelineCache d_hPipelineCache;
    VkRenderPass d_handle;
    VkResult d_result;

    struct SPipelineData
    {
        SPipelineData();

        std::uint32_t d_processIndex;
        std::uint32_t d_pipelineIndex;

        PipelineLayoutBase d_layout;
        RenderingOptions d_options;
        Pipeline d_pipeline;

        VkPipelineVertexInputStateCreateInfo d_vertexInputInfo;
        VkPipelineInputAssemblyStateCreateInfo d_inputAssemblyInfo;
        VkPipelineTessellationStateCreateInfo d_tessellationInfo;
        VkPipelineViewportStateCreateInfo d_viewportInfo;
        VkPipelineRasterizationStateCreateInfo d_rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo d_multisampleInfo;
        VkPipelineDepthStencilStateCreateInfo d_depthStencilInfo;
        VkPipelineColorBlendStateCreateInfo d_colorBlendInfo;
        VkPipelineDynamicStateCreateInfo d_dynamicInfo;
    };

    typedef std::vector< SPipelineData > ProcessPipelineData;
    
    PipelineCreateInfos d_pipelineCreateInfos;
    std::vector< ProcessPipelineData > d_pipelineData;

    bool d_bPipelinesCreated;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE RenderPass :: RenderPass (
    const RenderGraph& renderGraph,
    const Device& hDevice ) :
        TSharedReference< RenderPassImpl >(
            new RenderPassImpl ( renderGraph, hDevice, hDevice.defaultPipelineCache() ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderPass :: RenderPass (
    const RenderGraph& renderGraph,
    const Device& hDevice,
    const PipelineCache& hPipelineCache ) :
        TSharedReference< RenderPassImpl >(
            new RenderPassImpl ( renderGraph, hDevice, hPipelineCache ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderPass :: ~RenderPass()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkRenderPass RenderPass :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph& RenderPass :: graph() const
{
    return get()->d_graph;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& RenderPass :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineCache& RenderPass :: pipelineCache() const
{
    return get()->d_hPipelineCache;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPRENDERPASS_HPP
