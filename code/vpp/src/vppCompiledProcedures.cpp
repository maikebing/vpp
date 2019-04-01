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
#include "../include/vppCompiledProcedures.hpp"
#include "../include/vppCommandBufferRecorder.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

thread_local CompiledProcedures* CompiledProcedures :: s_pThis = 0;

// -----------------------------------------------------------------------------

void CompiledProcedures :: compile()
{
    const std::uint32_t nProcedures =
        static_cast< std::uint32_t >( d_procedures.size() );

    std::vector< CommandBuffer > buffers;
    d_commandPool.createBuffers ( nProcedures, & buffers );

    for ( std::uint32_t i = 0; i != nProcedures; ++i )
    {
        Procedure* pProcedure = d_procedures [ i ];
        pProcedure->d_buffer = buffers [ i ];

        CommandBufferRecorder recorder ( pProcedure->d_buffer );
        recorder.perform ( *pProcedure );
    }
}

// -----------------------------------------------------------------------------

void CompiledProcedures :: wait()
{
    d_queue.waitForIdle();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
