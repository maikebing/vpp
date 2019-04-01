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
#include "../include/vppShader.hpp"
#include "../include/vppLangTranslator.hpp"
#include "../include/vppLangInterface.hpp"
#include "../include/vppInstance.hpp"
#include "../include/vppDebugReporter.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

KShader :: KShader ( VkShaderStageFlagBits stage ) :
    d_stage ( stage ),
    d_pPipelineConfig ( PipelineConfig::getInstance() )
{
}

// -----------------------------------------------------------------------------

KShader :: ~KShader()
{
}

// -----------------------------------------------------------------------------

KShaderModule KShader :: compile (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    d_module = compileShader ( hDevice, pDynamicParameters );
    return d_module;
}

// -----------------------------------------------------------------------------

void KShader :: logCompilation (
    const KShaderTranslator& translator, const char* pShaderName )
{
    if ( DebugReporter* pReporter = Instance::getDebugReporter() )
        if ( pReporter->isShaderCompilationLogEnabled() )
    {
        std::ostringstream sstShaderCode;

        sstShaderCode << "-----------------------------------------------------" << std::endl;
        sstShaderCode << "Shader compilation (" << pShaderName << ")" << std::endl;
        sstShaderCode << " " << std::endl;
        sstShaderCode << "-------------------- Code start ---------------------" << std::endl;

        translator.print ( sstShaderCode );

        sstShaderCode << "--------------------- Code end ----------------------" << std::endl;

        pReporter->shaderCompilationLog ( sstShaderCode.str(), pShaderName );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

KShaderModule vertexShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_VERTEX_BIT, hDevice );
    translator.setShaderInputSize ( 1 );
    translator.setShaderOutputSize ( 1 );

    {
        VertexShader shaderRoot;
        d_definition ( & shaderRoot );

        pDynamicParameters->d_clipDistancesSize = shaderRoot.outVertex.getClipPlanes();
        pDynamicParameters->d_cullDistancesSize = shaderRoot.outVertex.getCullPlanes();

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "vertex" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void tessControlShader :: setOutputPatchVertices ( int v )
{
    d_outputPatchVertices = v;
}

// -----------------------------------------------------------------------------

int tessControlShader :: getOutputPatchVertices() const
{
    return d_outputPatchVertices;
}

// -----------------------------------------------------------------------------

void tessControlShader :: setTopology ( ETessellationTopology topology )
{
    d_topology = topology;
}

// -----------------------------------------------------------------------------

ETessellationTopology tessControlShader :: getTopology() const
{
    return d_topology;
}

// -----------------------------------------------------------------------------

void tessControlShader :: setSpacing ( ETessellationSpacing spacing )
{
    d_spacing = spacing;
}

// -----------------------------------------------------------------------------

ETessellationSpacing tessControlShader :: getSpacing() const
{
    return d_spacing;
}

// -----------------------------------------------------------------------------

KShaderModule tessControlShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, hDevice );

    {
        TessControlShader shaderRoot (
            d_topology,
            d_spacing,
            static_cast< int >( d_pPipelineConfig->getTessPatchControlPoints() ),
            d_outputPatchVertices );

        translator.setShaderInputSize ( shaderRoot.inVertices.size() );
        translator.setShaderOutputSize ( shaderRoot.outVertices.size() );

        shaderRoot.inVertices.setClipPlanes ( pDynamicParameters->d_clipDistancesSize );
        shaderRoot.inVertices.setCullPlanes ( pDynamicParameters->d_cullDistancesSize );

        d_definition ( & shaderRoot );

        pDynamicParameters->d_clipDistancesSize = shaderRoot.outVertices.getClipPlanes();
        pDynamicParameters->d_cullDistancesSize = shaderRoot.outVertices.getCullPlanes();

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "tessellation control" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void tessEvalShader :: setPointMode ( bool v )
{
    d_bPointMode = v;
}

// -----------------------------------------------------------------------------

bool tessEvalShader :: isPointMode() const
{
    return d_bPointMode;
}

// -----------------------------------------------------------------------------

void tessEvalShader :: setVertexOrderCW ( bool v )
{
    d_bVertexOrderCw = v;
}

// -----------------------------------------------------------------------------

bool tessEvalShader :: isVertexOrderCW() const
{
    return d_bVertexOrderCw;
}

// -----------------------------------------------------------------------------

KShaderModule tessEvalShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, hDevice );

    {
        TessEvalShader shaderRoot (
            d_bVertexOrderCw,
            d_bPointMode,
            d_pTessControlShader->getOutputPatchVertices() );

        translator.setShaderInputSize ( shaderRoot.inVertices.size() );
        translator.setShaderOutputSize ( 1 );

        shaderRoot.inVertices.setClipPlanes ( pDynamicParameters->d_clipDistancesSize );
        shaderRoot.inVertices.setCullPlanes ( pDynamicParameters->d_cullDistancesSize );

        d_definition ( & shaderRoot );

        pDynamicParameters->d_clipDistancesSize = shaderRoot.outVertex.getClipPlanes();
        pDynamicParameters->d_cullDistancesSize = shaderRoot.outVertex.getCullPlanes();

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "tessellation evaluation" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void geometryShader :: setInputTopology ( EGeometryInputTopology t )
{
    d_inputTopology = t;
}

// -----------------------------------------------------------------------------

void geometryShader :: setOutputTopology ( EGeometryOutputTopology t )
{
    d_outputTopology = t;
}

// -----------------------------------------------------------------------------

void geometryShader :: setMaxOutputVertices ( int value )
{
    d_maxOutputVertices = value;
}

// -----------------------------------------------------------------------------

void geometryShader :: setInvocations ( int inv )
{
    d_invocations = inv;
}

// -----------------------------------------------------------------------------

KShaderModule geometryShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_GEOMETRY_BIT, hDevice );

    {
        GeometryShader shaderRoot (
            d_inputTopology, d_outputTopology, d_maxOutputVertices, d_invocations );

        translator.setShaderInputSize ( shaderRoot.inVertices.size() );
        translator.setShaderOutputSize ( 1 );

        shaderRoot.inVertices.setClipPlanes ( pDynamicParameters->d_clipDistancesSize );
        shaderRoot.inVertices.setCullPlanes ( pDynamicParameters->d_cullDistancesSize );

        d_definition ( & shaderRoot );

        pDynamicParameters->d_clipDistancesSize = shaderRoot.outVertex.getClipPlanes();
        pDynamicParameters->d_cullDistancesSize = shaderRoot.outVertex.getCullPlanes();

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "geometry" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

KShaderModule fragmentShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_FRAGMENT_BIT, hDevice );
    translator.setShaderInputSize ( 1 );
    translator.setShaderOutputSize ( 1 );

    {
        FragmentShader shaderRoot;
        d_definition ( & shaderRoot );

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "fragment" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

KShaderModule computeShader :: compileShader (
    const Device& hDevice, SDynamicParameters* pDynamicParameters )
{
    KShaderTranslator translator ( VK_SHADER_STAGE_COMPUTE_BIT, hDevice );

    {
        ComputeShader shaderRoot ( d_localSize );
        d_definition ( & shaderRoot );

        if ( shaderRoot.isDebugCodeDumpEnabled() )
            logCompilation ( translator, "compute" );
    }

    return translator.generateModule();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
