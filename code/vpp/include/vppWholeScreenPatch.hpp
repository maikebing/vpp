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

#ifndef INC_VPPWHOLESCREENPATCH_HPP
#define INC_VPPWHOLESCREENPATCH_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

#ifndef INC_VPPSHADER_HPP
#include "vppShader.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class WholeScreenPatchPipelineConfig : public vpp::PipelineConfig
{
public:
    VPP_DLLAPI WholeScreenPatchPipelineConfig (
        Process hProcess, Device hDevice );

    VPP_DLLAPI void configureRenderingOptions (
        std::uint32_t width, std::uint32_t height,
        RenderingOptions* pOptions ) const;

    VPP_DLLAPI void cmdDrawWholeScreenPatch();

private:
    void vertexShader ( vpp::VertexShader* pShader );

private:
    RenderGraph* d_pGraph;
    vpp::vertexShader d_vertexShader;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPWHOLESCREENPATCH_HPP
