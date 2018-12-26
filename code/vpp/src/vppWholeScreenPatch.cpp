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
#include "../include/vppWholeScreenPatch.hpp"
#include "../include/vppRenderGraphNodes.hpp"
#include "../include/vppRenderingOptions.hpp"
#include "../include/vppLangIntInOut.hpp"
#include "../include/vppLangConversions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

WholeScreenPatchPipelineConfig :: WholeScreenPatchPipelineConfig (
    Process hProcess, Device hDevice ) :
        vpp::PipelineConfig ( hProcess ),
        d_pGraph ( hProcess.graph() ),
        d_vertexShader ( this, & WholeScreenPatchPipelineConfig::vertexShader )
{
    using namespace vpp;
    setPrimitiveTopology ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP );
}

// -----------------------------------------------------------------------------

void WholeScreenPatchPipelineConfig :: configureRenderingOptions (
    std::uint32_t width, std::uint32_t height,
    RenderingOptions* pOptions ) const
{
    pOptions->setCullMode ( VK_CULL_MODE_FRONT_BIT );
    pOptions->setFrontFace ( VK_FRONT_FACE_COUNTER_CLOCKWISE );
    pOptions->setEnableDepthTest ( false );
    pOptions->setEnableDepthWrite ( false );
    pOptions->setEnableDepthBoundsTest ( false );
    pOptions->setEnableDepthClamp ( false );
    pOptions->setEnableStencilTest ( false );
    pOptions->setPolygonMode ( VK_POLYGON_MODE_FILL );
    pOptions->addViewport ( Viewport ( width, height ) );
}

// -----------------------------------------------------------------------------

void WholeScreenPatchPipelineConfig :: cmdDrawWholeScreenPatch()
{
    d_pGraph->cmdDraw ( 4, 1, 0, 0 );
}

// -----------------------------------------------------------------------------

void WholeScreenPatchPipelineConfig :: vertexShader ( vpp::VertexShader* pShader )
{
    using namespace vpp;
    const Int vertexIndex = pShader->inVertexIndex;
    const Float x = StaticCast< Float >( ( vertexIndex << 1 ) & 2 );
    const Float y = StaticCast< Float >( ( vertexIndex & 2 ) );
    pShader->outVertex.position = Vec4 ( x - 1.0f, y - 1.0f, 0.0f, 1.0f );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
