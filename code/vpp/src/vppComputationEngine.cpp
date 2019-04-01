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
#include "../include/vppComputationEngine.hpp"
#include "../include/vppCommandBufferRecorder.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

thread_local ComputationEngine* ComputationEngine :: s_pThis = 0;

// -----------------------------------------------------------------------------

ComputationEngine :: ComputationEngine ( const Device& hDevice, EQueueType queueType ) :
    d_commandPool ( hDevice.defaultCmdPool ( queueType ) ),
    d_queue ( hDevice )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

ComputationEngine :: ComputationEngine (
    const Queue& hQueue,
    const CommandPool& hCommandPool ) :
        d_commandPool ( hCommandPool ),
        d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

ComputationEngine :: ComputationEngine ( const Queue& hQueue ) :
    d_commandPool ( hQueue.device().defaultCmdPool ( hQueue.type() ) ),
    d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

ComputationEngine :: ComputationEngine ( const CommandPool& hCommandPool ) :
    d_commandPool ( hCommandPool ),
    d_queue ( hCommandPool.device() )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

ComputationEngine* ComputationEngine :: getInstance()
{
    return s_pThis;
}

// -----------------------------------------------------------------------------

void ComputationEngine :: compile()
{
    const std::uint32_t nProcedures =
        static_cast< std::uint32_t >( d_computations.size() );

    std::vector< CommandBuffer > buffers;
    d_commandPool.createBuffers ( nProcedures, & buffers );

    for ( std::uint32_t i = 0; i != nProcedures; ++i )
    {
        Computation* pComputation = d_computations [ i ];
        pComputation->d_buffer = buffers [ i ];

        CommandBufferRecorder recorder ( pComputation->d_buffer );
        recorder.compute ( *pComputation );
    }
}

// -----------------------------------------------------------------------------

void ComputationEngine :: wait()
{
    d_queue.waitForIdle();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
