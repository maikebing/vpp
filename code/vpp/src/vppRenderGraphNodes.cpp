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
#include "../include/vppRenderGraphNodes.hpp"
#include "../include/vppRenderGraph.hpp"
#include "../include/vppSurface.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    const VkExtent2D& imageSize,
    EImagePurpose imagePurpose,
    VkFormat format ) :
        d_pInstance ( RenderGraph::getInstance() ),
        d_index ( d_pInstance->registerAttachment (
            imageSize.width, imageSize.height, imagePurpose, format ) ),
        d_width ( imageSize.width ),
        d_height ( imageSize.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    RenderGraph* pGraph,
    const VkExtent2D& imageSize,
    EImagePurpose imagePurpose,
    VkFormat format ) :
        d_pInstance ( pGraph ),
        d_index ( d_pInstance->registerAttachment (
            imageSize.width, imageSize.height, imagePurpose, format ) ),
        d_width ( imageSize.width ),
        d_height ( imageSize.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    const ImageInfo& imageInfo ) :
        d_pInstance ( RenderGraph::getInstance() ),
        d_index ( d_pInstance->registerAttachment ( imageInfo ) ),
        d_width ( imageInfo.extent.width ),
        d_height ( imageInfo.extent.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    RenderGraph* pGraph,
    const ImageInfo& imageInfo ) :
        d_pInstance ( pGraph ),
        d_index ( d_pInstance->registerAttachment ( imageInfo ) ),
        d_width ( imageInfo.extent.width ),
        d_height ( imageInfo.extent.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    const FrameImageView& imageView ) :
        d_pInstance ( RenderGraph::getInstance() ),
        d_index ( d_pInstance->registerAttachment ( imageView ) ),
        d_width ( imageView.image().info().extent.width ),
        d_height ( imageView.image().info().extent.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: BaseAttachment (
    RenderGraph* pGraph,
    const FrameImageView& imageView ) :
        d_pInstance ( pGraph ),
        d_index ( d_pInstance->registerAttachment ( imageView ) ),
        d_width ( imageView.image().info().extent.width ),
        d_height ( imageView.image().info().extent.height )
{
}

// -----------------------------------------------------------------------------

BaseAttachment :: ~BaseAttachment()
{
}

// -----------------------------------------------------------------------------

const BaseAttachment& BaseAttachment :: aliasesMemory ( const BaseAttachment& rhs )
{
    d_pInstance->getAttachmentDescription ( d_index ).flags |= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
    d_pInstance->getAttachmentDescription ( rhs.index() ).flags |= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
    return *this;
}

// -----------------------------------------------------------------------------

void BaseAttachment :: setFormat ( VkFormat fmt )
{
    d_pInstance->getAttachmentDescription ( d_index ).format = fmt;
}

// -----------------------------------------------------------------------------

VkFormat BaseAttachment :: getSurfaceFormat ( const Surface& hSurface )
{
    return hSurface.getDefaultFormat();
}

// -----------------------------------------------------------------------------

Display :: Display ( const Surface& hSurface ) :
    BaseAttachment ( hSurface.getSize(), DISPLAY, getSurfaceFormat ( hSurface ) )
{
}

// -----------------------------------------------------------------------------

Display :: Display ( RenderGraph* pGraph, const Surface& hSurface ) :
    BaseAttachment ( pGraph, hSurface.getSize(), DISPLAY, getSurfaceFormat ( hSurface ) )
{
}

// -----------------------------------------------------------------------------

Display :: ~Display()
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

Process :: Process() :
    d_pInstance ( RenderGraph::getInstance() ),
    d_index ( d_pInstance->registerProcess() )
{
}

// -----------------------------------------------------------------------------

Process :: Process ( RenderGraph* pGraph ) :
    d_pInstance ( pGraph ),
    d_index ( d_pInstance->registerProcess() )
{
}

// -----------------------------------------------------------------------------

Process :: Process ( RenderGraph* pGraph, std::uint32_t index ) :
    d_pInstance ( pGraph ),
    d_index ( index )
{
}

// -----------------------------------------------------------------------------

Process :: ~Process()
{
}

// -----------------------------------------------------------------------------

void Process :: addColorOutput ( const Display& d )
{
    VkClearValue clearValue;
    clearValue.color.float32 [ 0 ] = 0.0f;
    clearValue.color.float32 [ 1 ] = 0.0f;
    clearValue.color.float32 [ 2 ] = 0.0f;
    clearValue.color.float32 [ 3 ] = 0.0f;

    d_pInstance->addProcessOutputColorNode (
        d_index, d.index(), clearValue, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
}

// -----------------------------------------------------------------------------

void Process :: addColorOutput ( const Display& d, const VkClearColorValue& color )
{
    VkClearValue clearValue;
    clearValue.color = color;

    d_pInstance->addProcessOutputColorNode (
        d_index, d.index(), clearValue, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
}

// -----------------------------------------------------------------------------

void Process :: addPreservedOutput ( const BaseAttachment& d )
{
    d_pInstance->addProcessOutputPreserveNode (
        d_index, d.index() );
}

// -----------------------------------------------------------------------------

void Process :: addDependency (
    const Process& targetProcess,
    const VkSubpassDependency& dependency )
{
    d_pInstance->addProcessDependency ( d_index, targetProcess.index(), & dependency );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

Preprocess :: Preprocess() :
    d_pInstance ( RenderGraph::getInstance() ),
    d_index ( d_pInstance->registerPreprocess() )
{
}

// -----------------------------------------------------------------------------

Preprocess :: Preprocess ( RenderGraph* pGraph ) :
    d_pInstance ( pGraph ),
    d_index ( d_pInstance->registerPreprocess() )
{
}

// -----------------------------------------------------------------------------

Preprocess :: ~Preprocess()
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

Postprocess :: Postprocess() :
    d_pInstance ( RenderGraph::getInstance() ),
    d_index ( d_pInstance->registerPostprocess() )
{
}

// -----------------------------------------------------------------------------

Postprocess :: Postprocess ( RenderGraph* pGraph ) :
    d_pInstance ( pGraph ),
    d_index ( d_pInstance->registerPostprocess() )
{
}

// -----------------------------------------------------------------------------

Postprocess :: ~Postprocess()
{
}

// -----------------------------------------------------------------------------

void operator<< ( const Process& target, const std::function< void () >& cmds )
{
    target.graph()->addProcessCommands ( target.index(), cmds );
}

// -----------------------------------------------------------------------------

void operator<< ( const Preprocess& target, const std::function< void () >& cmds )
{
    target.graph()->addPreprocessCommands ( target.index(), cmds );
}

// -----------------------------------------------------------------------------

void operator<< ( const Postprocess& target, const std::function< void () >& cmds )
{
    target.graph()->addPostprocessCommands ( target.index(), cmds );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
