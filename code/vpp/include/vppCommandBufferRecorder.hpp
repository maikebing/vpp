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

#ifndef INC_VPPCOMMANDBUFFERRECORDER_HPP
#define INC_VPPCOMMANDBUFFERRECORDER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

#ifndef INC_VPPCOMPILEDPROCEDURES_HPP
#include "vppCompiledProcedures.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class CommandBufferRecorder
{
public:
    CommandBufferRecorder ( CommandBuffer buffer, std::uint32_t flags = 0 );
    ~CommandBufferRecorder();

    VPP_DLLAPI void render (
        const RenderPass& hRenderPass,
        const FrameBuffer& hFrameBuffer,
        bool bAutoBindPipeline = true );

    VPP_DLLAPI void compute (
        const ComputePass& hComputePass,
        bool bAutoBindPipeline = true );

    VPP_DLLAPI void perform (
        const Procedure& hProcedure );

    VPP_DLLAPI void presentImage ( VkImage hImage );
    VPP_DLLAPI void unpresentImage ( VkImage hImage );

private:
    CommandBufferRecorder ( const CommandBufferRecorder& ) = delete;
    const CommandBufferRecorder& operator= ( const CommandBufferRecorder& ) = delete;

private:
    CommandBuffer d_buffer;
};

// -----------------------------------------------------------------------------

VPP_INLINE CommandBufferRecorder :: CommandBufferRecorder (
    CommandBuffer buffer, std::uint32_t flags ) :
        d_buffer ( buffer )
{
    d_buffer.begin ( flags );
}

// -----------------------------------------------------------------------------

VPP_INLINE CommandBufferRecorder :: ~CommandBufferRecorder()
{
    d_buffer.end();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMANDBUFFERRECORDER_HPP
