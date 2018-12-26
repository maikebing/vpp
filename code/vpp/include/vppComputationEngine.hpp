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

#ifndef INC_VPPCOMPUTATIONENGINE_HPP
#define INC_VPPCOMPUTATIONENGINE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMPUTEPASS_HPP
#include "vppComputePass.hpp"
#endif

#ifndef INC_VPPCOMMANDPOOL_HPP
#include "vppCommandPool.hpp"
#endif

#ifndef INC_VPPQUEUE_HPP
#include "vppQueue.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class Computation;

// -----------------------------------------------------------------------------

class ComputationEngine : public ExtendedCommands
{
public:
    ComputationEngine ( const Device& hDevice, EQueueType queueType = Q_GRAPHICS );
    ComputationEngine ( const Queue& hQueue );
    ComputationEngine ( const Queue& hQueue, const CommandPool& hCommandPool );
    ComputationEngine ( const CommandPool& hCommandPool );

    void compile();
    void wait();

    const Device& device() const;

private:
    friend class Computation;

    static ComputationEngine* getInstance();
    void addComputation ( Computation* pComputation );

private:
    ComputationEngine ( const ComputationEngine& ) = delete;
    ComputationEngine ( ComputationEngine&& ) = delete;
    const ComputationEngine& operator= ( const ComputationEngine& ) = delete;

private:
    CommandPool d_commandPool;
    Queue d_queue;
    std::vector< Computation* > d_computations;

    static thread_local ComputationEngine* s_pThis;
};

// -----------------------------------------------------------------------------

VPP_INLINE ComputationEngine* ComputationEngine :: getInstance()
{
    return s_pThis;
}

// -----------------------------------------------------------------------------

VPP_INLINE void ComputationEngine :: addComputation ( Computation* pComputation )
{
    d_computations.push_back ( pComputation );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& ComputationEngine :: device() const
{
    return d_queue.device();
}

// -----------------------------------------------------------------------------

class Computation : public ComputePass
{
public:
    Computation();
    Computation ( Computation& predecessor );

    void operator()( const Fence& sigFenceOnEnd = Fence() );
    bool operator()( std::uint64_t waitTimeout );
    void operator()( const Queue& hQueue, const Fence& sigFenceOnEnd = Fence() );
    void operator()( const Queue& hQueue, const Semaphore& waitSem, const Semaphore& sigSem = Semaphore() );

private:
    friend class ComputationEngine;

    ComputationEngine* d_pOwner;
    CommandBuffer d_buffer;
    Semaphore d_signalOnEnd;
    Computation* d_pPredecessor;
};

// -----------------------------------------------------------------------------

VPP_INLINE ComputationEngine :: ComputationEngine ( const Device& hDevice, EQueueType queueType ) :
    d_commandPool ( hDevice.defaultCmdPool ( queueType ) ),
    d_queue ( hDevice )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputationEngine :: ComputationEngine (
    const Queue& hQueue,
    const CommandPool& hCommandPool ) :
        d_commandPool ( hCommandPool ),
        d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputationEngine :: ComputationEngine ( const Queue& hQueue ) :
    d_commandPool ( hQueue.device().defaultCmdPool ( hQueue.type() ) ),
    d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputationEngine :: ComputationEngine ( const CommandPool& hCommandPool ) :
    d_commandPool ( hCommandPool ),
    d_queue ( hCommandPool.device() )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Computation :: Computation() :
    ComputePass ( ComputationEngine::getInstance()->device() ),
    d_pOwner ( ComputationEngine::getInstance() ),
    d_pPredecessor ( 0 )
{
    d_pOwner->addComputation ( this );
}

// -----------------------------------------------------------------------------

VPP_INLINE Computation :: Computation ( Computation& predecessor ) :
    ComputePass ( ComputationEngine::getInstance()->device() ),
    d_pOwner ( ComputationEngine::getInstance() ),
    d_pPredecessor ( & predecessor )
{
    d_pOwner->addComputation ( this );

    if ( ! predecessor.d_signalOnEnd )
        predecessor.d_signalOnEnd = Semaphore ( d_pOwner->d_commandPool.device() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Computation :: operator()( const Fence& sigFenceOnEnd )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    d_pOwner->d_queue.submit ( d_buffer, waitSem, d_signalOnEnd, sigFenceOnEnd );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Computation :: operator()( std::uint64_t waitTimeout )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    Device hDevice = d_pOwner->d_queue.device();
    Fence fence ( hDevice );

    d_pOwner->d_queue.submit ( d_buffer, waitSem, d_signalOnEnd, fence );

    return fence.wait ( waitTimeout );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Computation :: operator()(
    const Queue& hQueue, const Fence& sigFenceOnEnd )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    hQueue.submit ( d_buffer, waitSem, d_signalOnEnd, sigFenceOnEnd );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Computation :: operator()(
    const Queue& hQueue, const Semaphore& waitSem, const Semaphore& sigSem )
{
    hQueue.submit ( d_buffer, waitSem, sigSem );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMPUTATIONENGINE_HPP
