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
namespace vpp {
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
};

// -----------------------------------------------------------------------------

class Procedure
{
public:
    Procedure();
    Procedure ( Procedure& predecessor );

    void operator()( const Fence& sigFenceOnEnd = Fence() );
    void operator()( std::uint64_t waitTimeout );
    void operator()( const Queue& hQueue, const Fence& sigFenceOnEnd = Fence() );
    void operator()( const Queue& hQueue, const Semaphore& waitSem, const Semaphore& sigSem = Semaphore() );
    void operator()( EQueueType eQueue, const Fence& sigFenceOnEnd = Fence() );

    typedef std::function< void () > FCommands;

    void operator<< ( const FCommands& cmds );

    const FCommands& getCommands() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
