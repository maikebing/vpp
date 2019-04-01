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

#ifndef INC_VPPSHADER_HPP
#define INC_VPPSHADER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPSHADERMODULE_HPP
#include "vppShaderModule.hpp"
#endif

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class VertexShader;
class TessControlShader;
class TessEvalShader;
class GeometryShader;
class FragmentShader;
class ComputeShader;

class KShaderTranslator;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SDynamicParameters
{
    int d_clipDistancesSize;
    int d_cullDistancesSize;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KShader
{
public:
    VPP_DLLAPI KShader ( VkShaderStageFlagBits stage );
    VPP_DLLAPI ~KShader();

    VPP_DLLAPI KShaderModule compile (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

    VkShaderStageFlagBits stage() const;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters ) = 0;

    VPP_DLLAPI void logCompilation (
        const KShaderTranslator& translator, const char* pShaderName );

protected:
    KShaderModule d_module;
    VkShaderStageFlagBits d_stage;
    PipelineConfig* d_pPipelineConfig;
};

// -----------------------------------------------------------------------------

VPP_INLINE VkShaderStageFlagBits KShader :: stage() const
{
    return d_stage;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class vertexShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE vertexShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( VertexShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_VERTEX_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) )
    {
        PipelineConfig::getInstance()->setVertexShader ( this );
    }

    static const EShader shader_type = SH_VERTEX;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( VertexShader* ) > d_definition;
};

// -----------------------------------------------------------------------------

class tessControlShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE tessControlShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( TessControlShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) ),
            d_topology ( TT_TRIANGLES ),
            d_spacing ( TS_EQUAL ),
            d_outputPatchVertices ( 3 )
    {
    }

    static const EShader shader_type = SH_TESS_CONTROL;

    void setOutputPatchVertices ( int v );
    int getOutputPatchVertices() const;

    void setTopology ( ETessellationTopology topology );
    ETessellationTopology getTopology() const;

    void setSpacing ( ETessellationSpacing spacing );
    ETessellationSpacing getSpacing() const;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( TessControlShader* ) > d_definition;

    ETessellationTopology d_topology;
    ETessellationSpacing d_spacing;
    int d_outputPatchVertices;
};

// -----------------------------------------------------------------------------

class tessEvalShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE tessEvalShader (
        const tessControlShader& tcs,
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( TessEvalShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) ),
            d_pTessControlShader ( & tcs ),
            d_bPointMode ( false ),
            d_bVertexOrderCw ( false )
    {
    }

    static const EShader shader_type = SH_TESS_EVAL;

    void setPointMode ( bool v );
    bool isPointMode() const;

    void setVertexOrderCW ( bool v );
    bool isVertexOrderCW() const;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( TessEvalShader* ) > d_definition;
    const tessControlShader* d_pTessControlShader;
    bool d_bPointMode;
    bool d_bVertexOrderCw;
};

// -----------------------------------------------------------------------------

class geometryShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE geometryShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( GeometryShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_GEOMETRY_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) ),
            d_inputTopology ( GTI_TRIANGLES ),
            d_outputTopology ( GTO_TRIANGLESTRIP ),
            d_maxOutputVertices ( 3 ),
            d_invocations ( 0 )
    {
        PipelineConfig::getInstance()->setGeometryShader ( this );
    }

    static const EShader shader_type = SH_GEOMETRY;

    void setInputTopology ( EGeometryInputTopology t );
    void setOutputTopology ( EGeometryOutputTopology t );
    void setMaxOutputVertices ( int value );
    void setInvocations ( int inv );

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( GeometryShader* ) > d_definition;
   
    EGeometryInputTopology d_inputTopology;
    EGeometryOutputTopology d_outputTopology;
    int d_maxOutputVertices;
    int d_invocations;
};

// -----------------------------------------------------------------------------

class fragmentShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE fragmentShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( FragmentShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_FRAGMENT_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) )
    {
        PipelineConfig::getInstance()->setFragmentShader ( this );
    }

    static const EShader shader_type = SH_FRAGMENT;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( FragmentShader* ) > d_definition;
};

// -----------------------------------------------------------------------------

class computeShader : public KShader
{
public:
    template< class ClassT, typename... Args >
    VPP_INLINE computeShader (
        ClassT* pParentClass,
        const SLocalGroupSize& localSize,
        void ( ClassT::* fMethodDef )( ComputeShader*, Args... ),
        Args... args ) :
            KShader ( VK_SHADER_STAGE_COMPUTE_BIT ),
            d_definition ( std::bind ( fMethodDef, pParentClass, std::placeholders::_1, args... ) ),
            d_localSize ( localSize )
    {
        PipelineConfig::getInstance()->setComputeShader ( this );
    }

    static const EShader shader_type = SH_COMPUTE;

protected:
    VPP_DLLAPI virtual KShaderModule compileShader (
        const Device& hDevice, SDynamicParameters* pDynamicParameters );

private:
    std::function< void ( ComputeShader* ) > d_definition;
    const SLocalGroupSize d_localSize;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHADER_HPP
