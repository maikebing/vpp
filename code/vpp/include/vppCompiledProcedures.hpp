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

#ifndef INC_VPPCOMPILEDPROCEDURES_HPP
#define INC_VPPCOMPILEDPROCEDURES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMANDS_HPP
#include "vppCommands.hpp"
#endif

#ifndef INC_VPPQUEUE_HPP
#include "vppQueue.hpp"
#endif

#ifndef INC_VPPCOMMANDPOOL_HPP
#include "vppCommandPool.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class Procedure;

// -----------------------------------------------------------------------------

class CompiledProcedures : public ExtendedCommands
{
public:
    CompiledProcedures ( const Queue& hQueue, const CommandPool& hCommandPool );
    CompiledProcedures ( const Queue& hQueue );
    CompiledProcedures ( const CommandPool& hCommandPool );
    CompiledProcedures ( const Device& hDevice, EQueueType queueType );

    void compile();
    void wait();

private:
    friend class Procedure;

    static CompiledProcedures* getInstance();
    void addProcedure ( Procedure* pProcedure );

private:
    CompiledProcedures ( const CompiledProcedures& ) = delete;
    CompiledProcedures ( CompiledProcedures&& ) = delete;
    const CompiledProcedures& operator= ( const CompiledProcedures& ) = delete;

private:
    CommandPool d_commandPool;
    Queue d_queue;
    std::vector< Procedure* > d_procedures;

    static thread_local CompiledProcedures* s_pThis;
};

// -----------------------------------------------------------------------------

VPP_INLINE CompiledProcedures* CompiledProcedures :: getInstance()
{
    return s_pThis;
}

// -----------------------------------------------------------------------------

VPP_INLINE void CompiledProcedures :: addProcedure ( Procedure* pProcedure )
{
    d_procedures.push_back ( pProcedure );
}

// -----------------------------------------------------------------------------

class Procedure
{
public:
    Procedure();
    Procedure ( Procedure& predecessor );

    void operator()( const Fence& sigFenceOnEnd = Fence() );
    bool operator()( std::uint64_t waitTimeout );
    void operator()( const Queue& hQueue, const Fence& sigFenceOnEnd = Fence() );
    void operator()( const Queue& hQueue, const Semaphore& waitSem, const Semaphore& sigSem = Semaphore() );
    void operator()( EQueueType eQueue, const Fence& sigFenceOnEnd = Fence() );

    typedef std::function< void () > FCommands;

    void operator<< ( const FCommands& cmds );

    const FCommands& getCommands() const;

private:
    friend class CompiledProcedures;

    CompiledProcedures* d_pOwner;
    FCommands d_commands;
    CommandBuffer d_buffer;
    Semaphore d_signalOnEnd;
    Procedure* d_pPredecessor;
};

// -----------------------------------------------------------------------------

VPP_INLINE CompiledProcedures :: CompiledProcedures (
    const Queue& hQueue,
    const CommandPool& hCommandPool ) :
        d_commandPool ( hCommandPool ),
        d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE CompiledProcedures :: CompiledProcedures ( const Queue& hQueue ) :
    d_commandPool ( hQueue.device().defaultCmdPool ( hQueue.type() ) ),
    d_queue ( hQueue )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE CompiledProcedures :: CompiledProcedures ( const CommandPool& hCommandPool ) :
    d_commandPool ( hCommandPool ),
    d_queue ( hCommandPool.device() )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE CompiledProcedures :: CompiledProcedures ( const Device& hDevice, EQueueType queueType ) :
    d_commandPool ( hDevice.defaultCmdPool ( queueType ) ),
    d_queue ( hDevice )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Procedure :: Procedure() :
    d_pOwner ( CompiledProcedures::getInstance() ),
    d_pPredecessor ( 0 )
{
    d_pOwner->addProcedure ( this );
}

// -----------------------------------------------------------------------------

VPP_INLINE Procedure :: Procedure ( Procedure& predecessor ) :
    d_pOwner ( CompiledProcedures::getInstance() ),
    d_pPredecessor ( & predecessor )
{
    d_pOwner->addProcedure ( this );

    if ( ! predecessor.d_signalOnEnd )
        predecessor.d_signalOnEnd = Semaphore ( d_pOwner->d_commandPool.device() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Procedure :: operator()( const Fence& sigFenceOnEnd )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    d_pOwner->d_queue.submit ( d_buffer, waitSem, d_signalOnEnd, sigFenceOnEnd );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Procedure :: operator()( std::uint64_t waitTimeout )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    Device hDevice = d_pOwner->d_queue.device();
    Fence fence ( hDevice );

    d_pOwner->d_queue.submit ( d_buffer, waitSem, d_signalOnEnd, fence );

    return fence.wait ( waitTimeout );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Procedure :: operator()( const Queue& hQueue, const Fence& sigFenceOnEnd )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    hQueue.submit ( d_buffer, waitSem, d_signalOnEnd, sigFenceOnEnd );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Procedure :: operator()(
    const Queue& hQueue, const Semaphore& waitSem, const Semaphore& sigSem )
{
    hQueue.submit ( d_buffer, waitSem, sigSem );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Procedure :: operator()(
    EQueueType eQueue, const Fence& sigFenceOnEnd )
{
    Semaphore waitSem =
        d_pPredecessor ? d_pPredecessor->d_signalOnEnd : Semaphore();

    Device hDevice = d_pOwner->d_queue.device();
    Queue queue ( hDevice, 0, eQueue );

    queue.submit ( d_buffer, waitSem, d_signalOnEnd, sigFenceOnEnd );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Procedure :: operator<< ( const FCommands& cmds )
{
    d_commands = cmds;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Procedure::FCommands& Procedure :: getCommands() const
{
    return d_commands;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMPILEDPROCEDURES_HPP
