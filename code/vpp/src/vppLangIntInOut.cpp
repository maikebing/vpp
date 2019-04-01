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
#include "../include/vppLangIntInOut.hpp"
#include "../include/vppLangTranslator.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

Shader :: Shader ( spv::ExecutionModel shaderType ) :
    d_pTranslator ( KShaderTranslator::get() ),
    d_pFunction ( 0 ),
    d_pEntryPoint ( 0 ),
    d_bDebugCodeDump ( false )
{
    d_pTranslator->setSource ( spv::SourceLanguageGLSL, 450 );
    d_pTranslator->useCapability ( spv::CapabilityShader );

    d_pFunction = d_pTranslator->makeEntrypoint ( "main" );

    d_pEntryPoint = d_pTranslator->addEntryPoint (
        shaderType, d_pFunction, "main" );
}

// -----------------------------------------------------------------------------

Shader :: ~Shader()
{
    d_pTranslator->makeReturn ( true );
    d_pTranslator->generateInputOutputForwards ( d_pEntryPoint );
    d_pTranslator->eliminateDeadDecorations();
}

// -----------------------------------------------------------------------------

void Shader :: DebugCodeDump()
{
    d_bDebugCodeDump = true;
}

// -----------------------------------------------------------------------------

bool Shader :: isDebugCodeDumpEnabled() const
{
    return d_bDebugCodeDump;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

TessControlShader :: TessControlShader (
    ETessellationTopology topology,
    ETessellationSpacing spacing,
    int inputPatchVertices,
    int outputPatchVertices ) :
        Shader ( spv::ExecutionModelTessellationControl )
{
    d_pTranslator->useCapability ( spv::CapabilityTessellation );

    switch ( topology )
    {
        case TT_TRIANGLES:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeTriangles );
            break;
                
        case TT_QUADS:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeQuads );
            break;

        case TT_ISOLINES:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeIsolines );
            break;
    }

    switch ( spacing )
    {
        case TS_EQUAL:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeSpacingEqual );
            break;

        case TS_FRACTIONAL_EVEN:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeSpacingFractionalEven );
            break;

        case TS_FRACTIONAL_ODD:
            d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModeSpacingFractionalOdd );
            break;
    }

    d_pTranslator->addExecutionMode (
        d_pFunction, spv::ExecutionModeOutputVertices, outputPatchVertices );

    inVertices.resize ( inputPatchVertices );
    outVertices.resize ( outputPatchVertices );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

TessEvalShader :: TessEvalShader (
    bool bVertexOrderCw,
    bool bPointMode,
    int inputPatchVertices ) :
        Shader ( spv::ExecutionModelTessellationEvaluation )
{
    d_pTranslator->useCapability ( spv::CapabilityTessellation );

    d_pTranslator->addExecutionMode (
        d_pFunction,
        ( bVertexOrderCw ? spv::ExecutionModeVertexOrderCw : spv::ExecutionModeVertexOrderCcw ) );

    if ( bPointMode )
        d_pTranslator->addExecutionMode ( d_pFunction, spv::ExecutionModePointMode );

    inVertices.resize ( inputPatchVertices );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

GeometryShader :: GeometryShader (
    EGeometryInputTopology inputTopology,
    EGeometryOutputTopology outputTopology,
    int maxOutputVertices,
    int invocations ) :
        Shader ( spv::ExecutionModelGeometry )
{
    int inputVerticesCount = 1;
    spv::ExecutionMode inputExecutionMode = spv::ExecutionModeTriangles;
    spv::ExecutionMode outputExecutionMode = spv::ExecutionModeOutputTriangleStrip;

    switch ( inputTopology )
    {
        case GTI_POINTS:
            inputVerticesCount = 1;
            inputExecutionMode = spv::ExecutionModeInputPoints;
            break;

        case GTI_LINES:
            inputVerticesCount = 2;
            inputExecutionMode = spv::ExecutionModeInputLines;
            break;

        case GTI_ADJLINES:
            inputVerticesCount = 4;
            inputExecutionMode = spv::ExecutionModeInputLinesAdjacency;
            break;

        case GTI_TRIANGLES:
            inputVerticesCount = 3;
            inputExecutionMode = spv::ExecutionModeTriangles;
            break;

        case GTI_ADJTRIANGLES:
            inputVerticesCount = 6;
            inputExecutionMode = spv::ExecutionModeInputTrianglesAdjacency;
            break;
    }

    switch ( outputTopology )
    {
        case GTO_POINTS:
            outputExecutionMode = spv::ExecutionModeOutputPoints;
            break;

        case GTO_LINESTRIP:
            outputExecutionMode = spv::ExecutionModeOutputLineStrip;
            break;

        case GTO_TRIANGLESTRIP:
            outputExecutionMode = spv::ExecutionModeOutputTriangleStrip;
            break;
    }

    d_pTranslator->useCapability ( spv::CapabilityGeometry );
    d_pTranslator->addExecutionMode ( d_pFunction, inputExecutionMode );
    d_pTranslator->addExecutionMode ( d_pFunction, outputExecutionMode );

    d_pTranslator->addExecutionMode (
        d_pFunction, spv::ExecutionModeOutputVertices, maxOutputVertices );

    if ( invocations > 0 )
        d_pTranslator->addExecutionMode (
            d_pFunction, spv::ExecutionModeInvocations, invocations );

    inVertices.resize ( inputVerticesCount );
}

// -----------------------------------------------------------------------------

void GeometryShader :: EmitVertex()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        pTranslator->createNoResultOp ( spv::OpEmitVertex );
}

// -----------------------------------------------------------------------------

void GeometryShader :: EndPrimitive()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        pTranslator->createNoResultOp ( spv::OpEndPrimitive );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void FragmentShader :: Kill()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        pTranslator->makeDiscard();
}

// -----------------------------------------------------------------------------

void FragmentShader :: EarlyTest()
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
        d_pTranslator->addExecutionMode (
            d_pFunction, spv::ExecutionModeEarlyFragmentTests );
}

// -----------------------------------------------------------------------------

VkExtent3D FragmentShader :: findAttachmentExtent (
    const RenderGraph& renderGraph ) const
{
    const unsigned int nAttachments = renderGraph.getAttachmentCount();

    for ( unsigned int i = 0; i != nAttachments; ++i )
    {
        const ImageInfo& attImgInfo = renderGraph.getAttachmentInfo ( i );

        if ( attImgInfo.usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
            return attImgInfo.extent;
    }

    for ( unsigned int i = 0; i != nAttachments; ++i )
    {
        const ImageInfo& attImgInfo = renderGraph.getAttachmentInfo ( i );

        if ( attImgInfo.usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
            return attImgInfo.extent;
    }

    return VkExtent3D { 0, 0, 0 };
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
