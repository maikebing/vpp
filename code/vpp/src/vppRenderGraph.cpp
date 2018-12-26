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

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppRenderGraph.hpp"
#include "../include/vppRenderPass.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

thread_local RenderGraph* RenderGraph :: d_pThis = 0;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

RenderGraphImpl :: RenderGraphImpl()
{
}

// -----------------------------------------------------------------------------

RenderGraphImpl :: ~RenderGraphImpl()
{
    for ( auto iProcessNode : d_processNodes )
        delete iProcessNode;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

RenderGraph :: RenderGraph() :
    TSharedReference< RenderGraphImpl >( new RenderGraphImpl() )
{
    d_pThis = this;
}

// -----------------------------------------------------------------------------

RenderGraph :: ~RenderGraph()
{
}

// -----------------------------------------------------------------------------

VkAttachmentDescription& RenderGraph :: getAttachmentDescription ( std::uint32_t index )
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_attachmentConfig.getDescription ( index );
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: getAttachmentCount() const
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_attachmentConfig.getDescriptionCount();
}

// -----------------------------------------------------------------------------

ImageInfo& RenderGraph :: getAttachmentInfo ( size_t index )
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_attachmentConfig.getInfo ( index );
}

// -----------------------------------------------------------------------------

const ImageInfo& RenderGraph :: getAttachmentInfo ( size_t index ) const
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_attachmentConfig.getInfo ( index );
}

// -----------------------------------------------------------------------------

SProcessInfo& RenderGraph :: getProcessInfo ( size_t index )
{
    RenderGraphImpl* pImpl = get();
    return *pImpl->d_processNodes [ index ];
}

// -----------------------------------------------------------------------------

const SProcessInfo& RenderGraph :: getProcessInfo ( size_t index ) const
{
    RenderGraphImpl* pImpl = get();
    return *pImpl->d_processNodes [ index ];
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: getProcessCount() const
{
    RenderGraphImpl* pImpl = get();
    return static_cast< std::uint32_t >( pImpl->d_processNodes.size() );
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: getPreprocessCount() const
{
    RenderGraphImpl* pImpl = get();
    return static_cast< std::uint32_t >( pImpl->d_preprocessNodes.size() );
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: getPostprocessCount() const
{
    RenderGraphImpl* pImpl = get();
    return static_cast< std::uint32_t >( pImpl->d_postprocessNodes.size() );
}

// -----------------------------------------------------------------------------

const RenderGraph::Commands& RenderGraph :: getProcessCommands ( std::uint32_t index ) const
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_processNodes [ index ]->d_commands;
}

// -----------------------------------------------------------------------------

const RenderGraph::Commands& RenderGraph :: getPreprocessCommands ( std::uint32_t index ) const
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_preprocessNodes [ index ].d_commands;
}

// -----------------------------------------------------------------------------

const RenderGraph::Commands& RenderGraph :: getPostprocessCommands ( std::uint32_t index ) const
{
    RenderGraphImpl* pImpl = get();
    return pImpl->d_postprocessNodes [ index ].d_commands;
}

// -----------------------------------------------------------------------------

const KAttachmentConfig& RenderGraph :: getAttachmentConfig() const
{
    return get()->d_attachmentConfig;
}

// -----------------------------------------------------------------------------

const SubpassDescriptions& RenderGraph :: getSubpassDescriptions() const
{
    return get()->d_subpassDescriptions;
}

// -----------------------------------------------------------------------------

const SubpassDependencies& RenderGraph :: getSubpassDependencies() const
{
    return get()->d_subpassDependencies;
}

// -----------------------------------------------------------------------------

const ClearValues& RenderGraph :: getClearValues() const
{
    return get()->d_clearValues;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: findInputLocation (
    std::uint32_t processIndex, std::uint32_t nodeIndex ) const
{
    const RenderGraphImpl* pImpl = get();

    const SProcessInfo& processInfo = getProcessInfo ( processIndex );

    for ( size_t i = 0; i < processInfo.d_inputNodes.size(); ++i )
        if ( processInfo.d_inputNodes [ i ].attachment == nodeIndex )
            return static_cast< std::uint32_t >( i );

    return 0xFFFFFFFFu;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: findOutputLocation (
    std::uint32_t processIndex, std::uint32_t nodeIndex ) const
{
    const RenderGraphImpl* pImpl = get();

    const SProcessInfo& processInfo = getProcessInfo ( processIndex );

    for ( size_t i = 0; i < processInfo.d_outputColorNodes.size(); ++i )
        if ( processInfo.d_outputColorNodes [ i ].attachment == nodeIndex )
            return static_cast< std::uint32_t >( i );

    return 0xFFFFFFFFu;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerAttachment ( 
    std::uint32_t width,
    std::uint32_t height,
    EImagePurpose imagePurpose,
    VkFormat fmt ) 
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index = pImpl->d_attachmentConfig.getDescriptionCount();

    VkAttachmentDescription& vdesc =
        pImpl->d_attachmentConfig.addNewAttachment ( width, height, imagePurpose, 0, fmt );

    if ( imagePurpose == DISPLAY )
    {
        vdesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vdesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    vdesc.samples = VK_SAMPLE_COUNT_1_BIT;

    pImpl->d_clearValues.push_back ( VkClearValue() );

    return index;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerAttachment (
    const ImageInfo& imageInfo ) 
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index = pImpl->d_attachmentConfig.getDescriptionCount();

    VkAttachmentDescription& vdesc =
        pImpl->d_attachmentConfig.addNewAttachment ( imageInfo );

    if ( imageInfo.purpose == DISPLAY )
    {
        vdesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vdesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    vdesc.samples = VK_SAMPLE_COUNT_1_BIT;

    pImpl->d_clearValues.push_back ( VkClearValue() );

    return index;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerAttachment (
    const FrameImageView& imageView ) 
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index = pImpl->d_attachmentConfig.getDescriptionCount();

    VkAttachmentDescription& vdesc =
        pImpl->d_attachmentConfig.addNewAttachment ( imageView );

    if ( imageView.image().info().purpose == DISPLAY )
    {
        vdesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vdesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    vdesc.samples = VK_SAMPLE_COUNT_1_BIT;

    pImpl->d_clearValues.push_back ( VkClearValue() );

    return index;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerProcess() 
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index =
        static_cast< std::uint32_t >( pImpl->d_processNodes.size() );

    pImpl->d_processNodes.push_back ( new SProcessInfo ( index ) );

    pImpl->d_subpassDescriptions.push_back ( VkSubpassDescription() );

    VkSubpassDescription& newSubpass = pImpl->d_subpassDescriptions.back();
    std::memset ( & newSubpass, 0, sizeof ( VkSubpassDescription ) );
    newSubpass.flags = 0;
    newSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    return index;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerPreprocess()
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index =
        static_cast< std::uint32_t >( pImpl->d_preprocessNodes.size() );

    pImpl->d_preprocessNodes.push_back ( SPrePostProcessInfo ( index ) );

    return index;
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: registerPostprocess()
{
    RenderGraphImpl* pImpl = get();

    const std::uint32_t index =
        static_cast< std::uint32_t >( pImpl->d_postprocessNodes.size() );

    pImpl->d_postprocessNodes.push_back ( SPrePostProcessInfo ( index ) );

    return index;
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessInputNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex,
    VkImageLayout nodeImageLayout,
    bool bAddDependency )
{
    RenderGraphImpl* pImpl = get();

    auto& inputNodes = pImpl->d_processNodes [ processIndex ]->d_inputNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];

    inputNodes.push_back ( VkAttachmentReference { nodeIndex, nodeImageLayout } );
    ++descriptions.inputAttachmentCount;
    descriptions.pInputAttachments = & inputNodes [ 0 ];

    ImageInfo& imageInfo = pImpl->d_attachmentConfig.getInfo ( nodeIndex );
    imageInfo.usage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    if ( bAddDependency )
    {
        for ( const auto& iProcess : pImpl->d_processNodes )
        {
            const unsigned int pi = iProcess->d_processIndex;

            for ( const auto& iColorOutput : pImpl->d_processNodes [ pi ]->d_outputColorNodes )
                if ( iColorOutput.attachment == nodeIndex )
            {
                VkSubpassDependency newDependency;
                std::memset ( & newDependency, 0, sizeof ( VkSubpassDependency ) );
                newDependency.srcSubpass = pi;
                newDependency.dstSubpass = processIndex;
                newDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                newDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                newDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                newDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                newDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
               
                addProcessDependency ( pi, processIndex, & newDependency );
                break;
            }

            for ( const auto& iResolveOutput : pImpl->d_processNodes [ pi ]->d_outputResolveNodes )
                if ( iResolveOutput.attachment == nodeIndex )
            {
                VkSubpassDependency newDependency;
                std::memset ( & newDependency, 0, sizeof ( VkSubpassDependency ) );
                newDependency.srcSubpass = pi;
                newDependency.dstSubpass = processIndex;
                newDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                newDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                newDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                newDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                newDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
               
                addProcessDependency ( pi, processIndex, & newDependency );
                break;
            }

            for ( const auto& iDepthOutput : pImpl->d_processNodes [ pi ]->d_outputDepthNodes )
                if ( iDepthOutput.attachment == nodeIndex )
            {
                VkSubpassDependency newDependency;
                std::memset ( & newDependency, 0, sizeof ( VkSubpassDependency ) );
                newDependency.srcSubpass = pi;
                newDependency.dstSubpass = processIndex;
                newDependency.srcStageMask =
                    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
                    | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                newDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                newDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                newDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                newDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
               
                addProcessDependency ( pi, processIndex, & newDependency );
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessOutputColorNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex,
    VkImageLayout destLayout )
{
    RenderGraphImpl* pImpl = get();

    auto& outputColorNodes = pImpl->d_processNodes [ processIndex ]->d_outputColorNodes;
    auto& outputResolveNodes = pImpl->d_processNodes [ processIndex ]->d_outputResolveNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];

    outputColorNodes.push_back ( VkAttachmentReference { nodeIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    outputResolveNodes.push_back ( VkAttachmentReference { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    ++descriptions.colorAttachmentCount;
    descriptions.pColorAttachments = & outputColorNodes [ 0 ];
    descriptions.pResolveAttachments = & outputResolveNodes [ 0 ];

    ImageInfo& imageInfo = pImpl->d_attachmentConfig.getInfo ( nodeIndex );
    imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VkAttachmentDescription& description =
        pImpl->d_attachmentConfig.getDescription ( nodeIndex );

    description.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    description.finalLayout = destLayout;

    pImpl->d_clearValues [ nodeIndex ] = VkClearValue();
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessOutputColorNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex,
    const VkClearValue& clearValue,
    VkImageLayout destLayout )
{
    RenderGraphImpl* pImpl = get();

    auto& outputColorNodes = pImpl->d_processNodes [ processIndex ]->d_outputColorNodes;
    auto& outputResolveNodes = pImpl->d_processNodes [ processIndex ]->d_outputResolveNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];

    outputColorNodes.push_back ( VkAttachmentReference { nodeIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    outputResolveNodes.push_back ( VkAttachmentReference { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    ++descriptions.colorAttachmentCount;
    descriptions.pColorAttachments = & outputColorNodes [ 0 ];
    descriptions.pResolveAttachments = & outputResolveNodes [ 0 ];

    ImageInfo& imageInfo = pImpl->d_attachmentConfig.getInfo ( nodeIndex );
    imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VkAttachmentDescription& description =
        pImpl->d_attachmentConfig.getDescription ( nodeIndex );

    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    description.finalLayout = destLayout;

    pImpl->d_clearValues [ nodeIndex ] = clearValue;
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessOutputColorAndResolveNode (
    std::uint32_t processIndex,
    std::uint32_t colorNodeIndex,
    std::uint32_t resolveNodeIndex,
    const VkClearValue& clearValue )
{
    RenderGraphImpl* pImpl = get();

    auto& outputColorNodes = pImpl->d_processNodes [ processIndex ]->d_outputColorNodes;
    auto& outputResolveNodes = pImpl->d_processNodes [ processIndex ]->d_outputResolveNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];

    outputColorNodes.push_back ( VkAttachmentReference { colorNodeIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    outputResolveNodes.push_back ( VkAttachmentReference { resolveNodeIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
    ++descriptions.colorAttachmentCount;
    descriptions.pColorAttachments = & outputColorNodes [ 0 ];
    descriptions.pResolveAttachments = & outputResolveNodes [ 0 ];

    ImageInfo& imageInfoC = pImpl->d_attachmentConfig.getInfo ( colorNodeIndex );
    imageInfoC.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    ImageInfo& imageInfoR = pImpl->d_attachmentConfig.getInfo ( resolveNodeIndex );
    imageInfoR.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VkAttachmentDescription& descriptionC =
        pImpl->d_attachmentConfig.getDescription ( colorNodeIndex );

    descriptionC.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    descriptionC.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    descriptionC.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    descriptionC.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    descriptionC.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    descriptionC.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription& descriptionR =
        pImpl->d_attachmentConfig.getDescription ( resolveNodeIndex );

    descriptionR.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    descriptionR.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    descriptionR.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    descriptionR.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    descriptionR.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    descriptionR.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    pImpl->d_clearValues [ colorNodeIndex ] = clearValue;
    pImpl->d_clearValues [ resolveNodeIndex ] = clearValue;
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessOutputDepthNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex,
    const VkClearValue* pClearValue,
    VkImageLayout preserveInLayout )
{
    RenderGraphImpl* pImpl = get();

    auto& outputDepthNodes = pImpl->d_processNodes [ processIndex ]->d_outputDepthNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];
    
    outputDepthNodes.push_back ( VkAttachmentReference { nodeIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } );
    descriptions.pDepthStencilAttachment = & outputDepthNodes [ 0 ];

    ImageInfo& imageInfo = pImpl->d_attachmentConfig.getInfo ( nodeIndex );
    imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VkAttachmentDescription& description =
        pImpl->d_attachmentConfig.getDescription ( nodeIndex );

    const bool bPreserve = ( preserveInLayout != VK_IMAGE_LAYOUT_UNDEFINED );
    const std::uint32_t imageAspect = imageInfo.getAspect();
    const bool bHasDepth = ( ( imageAspect & VK_IMAGE_ASPECT_DEPTH_BIT ) != 0 );
    const bool bHasStencil = ( ( imageAspect & VK_IMAGE_ASPECT_STENCIL_BIT ) != 0 );

    description.loadOp = (
        bHasDepth ?
            ( pClearValue ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD )
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE );

    description.stencilLoadOp = (
        bHasStencil ?
            ( pClearValue ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD )
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE );

    description.storeOp = ( bPreserve && bHasDepth ?
        VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE );

    description.stencilStoreOp = ( bPreserve && bHasStencil ?
        VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE );

    description.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    description.finalLayout = ( bPreserve ?
        preserveInLayout : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );

    if ( pClearValue )
        pImpl->d_clearValues [ nodeIndex ] = *pClearValue;
    else
        pImpl->d_clearValues [ nodeIndex ] = VkClearValue();
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessInputDepthNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex )
{
    RenderGraphImpl* pImpl = get();

    auto& outputDepthNodes = pImpl->d_processNodes [ processIndex ]->d_outputDepthNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];
    
    outputDepthNodes.push_back ( VkAttachmentReference { nodeIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL } );
    descriptions.pDepthStencilAttachment = & outputDepthNodes [ 0 ];

    ImageInfo& imageInfo = pImpl->d_attachmentConfig.getInfo ( nodeIndex );
    imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VkAttachmentDescription& description =
        pImpl->d_attachmentConfig.getDescription ( nodeIndex );

    const std::uint32_t imageAspect = imageInfo.getAspect();
    const bool bHasDepth = ( ( imageAspect & VK_IMAGE_ASPECT_DEPTH_BIT ) != 0 );
    const bool bHasStencil = ( ( imageAspect & VK_IMAGE_ASPECT_STENCIL_BIT ) != 0 );

    description.loadOp = (
        bHasDepth ?
            VK_ATTACHMENT_LOAD_OP_LOAD
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE );

    description.stencilLoadOp = (
        bHasStencil ?
            VK_ATTACHMENT_LOAD_OP_LOAD
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE );

    description.storeOp = ( bHasDepth ?
        VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE );

    description.stencilStoreOp = ( bHasStencil ?
        VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE );

    description.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    pImpl->d_clearValues [ nodeIndex ] = VkClearValue();
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessOutputPreserveNode (
    std::uint32_t processIndex,
    std::uint32_t nodeIndex )
{
    RenderGraphImpl* pImpl = get();

    auto& outputPreserveNodes = pImpl->d_processNodes [ processIndex ]->d_outputPreserveNodes;
    auto& descriptions = pImpl->d_subpassDescriptions [ processIndex ];

    outputPreserveNodes.emplace_back ( nodeIndex );
    ++descriptions.preserveAttachmentCount;
    descriptions.pPreserveAttachments = & outputPreserveNodes [ 0 ];
}

// -----------------------------------------------------------------------------

std::uint32_t RenderGraph :: addProcessDependency (
    std::uint32_t from, std::uint32_t to,
    const VkSubpassDependency* pDependency )
{
    RenderGraphImpl* pImpl = get();

    std::uint32_t dependencyIndex;

    const auto& key = std::make_pair ( from, to );
    const auto& iKey = pImpl->d_processDependencies.find ( key );

    if ( iKey == pImpl->d_processDependencies.end() )
    {
        dependencyIndex =
            static_cast< std::uint32_t >( pImpl->d_subpassDependencies.size() );

        pImpl->d_processDependencies [ key ] = dependencyIndex;

        pImpl->d_subpassDependencies.push_back ( VkSubpassDependency() );
        VkSubpassDependency& newDependency = pImpl->d_subpassDependencies.back();
        newDependency = *pDependency;
        newDependency.srcSubpass = from;
        newDependency.dstSubpass = to;
    }
    else
    {
        dependencyIndex = iKey->second;
        VkSubpassDependency& dependency = pImpl->d_subpassDependencies [ dependencyIndex ];
        dependency.srcStageMask |= pDependency->srcStageMask;
        dependency.dstStageMask |= pDependency->dstStageMask;
        dependency.srcAccessMask |= pDependency->srcAccessMask;
        dependency.dstAccessMask |= pDependency->dstAccessMask;
        dependency.dependencyFlags |= pDependency->dependencyFlags;
    }

    return dependencyIndex;
}

// -----------------------------------------------------------------------------

void RenderGraph :: setAllGraphicsDependency ( std::uint32_t depIndex )
{
    RenderGraphImpl* pImpl = get();
    VkSubpassDependency& dependency = pImpl->d_subpassDependencies [ depIndex ];
    dependency.srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
}

// -----------------------------------------------------------------------------

void RenderGraph :: setByPixelDependency ( std::uint32_t depIndex )
{
    RenderGraphImpl* pImpl = get();
    VkSubpassDependency& dependency = pImpl->d_subpassDependencies [ depIndex ];
    dependency.dependencyFlags |=  VK_DEPENDENCY_BY_REGION_BIT;
}

// -----------------------------------------------------------------------------

void RenderGraph :: addProcessCommands (
    std::uint32_t processIndex, const std::function< void () >& cmds )
{
    RenderGraphImpl* pImpl = get();
    pImpl->d_processNodes [ processIndex ]->d_commands.emplace_back ( cmds );
}

// -----------------------------------------------------------------------------

void RenderGraph :: addPreprocessCommands (
    std::uint32_t preprocessIndex, const std::function< void () >& cmds )
{
    RenderGraphImpl* pImpl = get();
    pImpl->d_preprocessNodes [ preprocessIndex ].d_commands.emplace_back ( cmds );
}

// -----------------------------------------------------------------------------

void RenderGraph :: addPostprocessCommands (
    std::uint32_t postprocessIndex, const std::function< void () >& cmds )
{
    RenderGraphImpl* pImpl = get();
    pImpl->d_postprocessNodes [ postprocessIndex ].d_commands.emplace_back ( cmds );
}

// -----------------------------------------------------------------------------

void RenderGraph :: cmdDraw (
    std::uint32_t vertexCount,
    std::uint32_t instanceCount,
    std::uint32_t firstVertex,
    std::uint32_t firstInstance,
    CommandBuffer hCommandBuffer )
{
    if ( vertexCount > 0 && instanceCount > 0 )
    {
        const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
            hCommandBuffer.handle()
            : RenderingCommandContext::getCommandBufferHandle();

        ::vkCmdDraw (
            hCmdBuffer,
            vertexCount, instanceCount, firstVertex, firstInstance );
    }
}

// -----------------------------------------------------------------------------

void RenderGraph :: cmdDrawIndexed (
    std::uint32_t indexCount,
    std::uint32_t instanceCount,
    std::uint32_t firstIndex,
    std::int32_t vertexOffset,
    std::uint32_t firstInstance,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdDrawIndexed (
        hCmdBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
}

// -----------------------------------------------------------------------------

void RenderGraph :: cmdDrawIndirect (
    const IndirectBufferView& data,
    VkDeviceSize offset,
    std::uint32_t drawCount,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdDrawIndirect (
        hCmdBuffer,
        data.buffer().handle(),
        offset * sizeof ( VkDrawIndirectCommand ),
        drawCount,
        sizeof ( VkDrawIndirectCommand ) );
}

// -----------------------------------------------------------------------------

void RenderGraph :: cmdDrawIndexedIndirect (
    const IndirectBufferView& data,
    VkDeviceSize offset,
    std::uint32_t drawCount,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdDrawIndexedIndirect (
        hCmdBuffer,
        data.buffer().handle(),
        offset * sizeof ( VkDrawIndexedIndirectCommand ),
        drawCount,
        sizeof ( VkDrawIndexedIndirectCommand ) );
}

// -----------------------------------------------------------------------------

void RenderGraph :: cmdClearImages (
    std::uint32_t attachmentCount,
    const VkClearAttachment* pAttachments,
    std::uint32_t rectCount,
    const VkClearRect* pRects,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdClearAttachments (
        hCmdBuffer, attachmentCount, pAttachments, rectCount, pRects );
}

// -----------------------------------------------------------------------------

VPP_DLLAPI const FrameBuffer& RenderGraph :: framebuffer()
{
    return RenderingFrameBufferContext::getFrameBuffer();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
