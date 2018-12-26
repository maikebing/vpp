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

/**
    \brief Base class for computation engines.

    Derive your own class from ComputationEngine and place one or more
    Computation derived objects within it.
    
    Call \c compile() method once in the constructor of your ComputationEngine
    subclass.

    Every Computation field of the subclass can now launch a computation
    on the GPU and optionally wait for the result. Use one of the \c operator()
    overloads for that purpose. Syntactically, a ComputationEngine derived
    class looks similar to regular C++ class with methods.
*/

class ComputationEngine : public ExtendedCommands
{
public:
    /**
        \brief Construct a computation engine attached to specified device and default command queue.

        This is the simplest constructor, requiring only a device object. It is recommended
        to use this one.

        You can optionally specify a queue type, however currently only \c Q_GRAPHICS
        makes sense, so just leave this parameter as default.
    */

    ComputationEngine ( const Device& hDevice, EQueueType queueType = Q_GRAPHICS );

    /**
        \brief Construct a computation engine attached to specified command queue.

        This constructor can be used when you want to schedule the computation
        to specific command queue. It retrieves the device automatically from the queue.
    */
    ComputationEngine ( const Queue& hQueue );

    /**
        \brief Construct a computation engine attached to specified command queue and command pool.

        This constructor can be used when you want to schedule the computation
        to specific command queue and allocate command buffers from specific command pool.
        
        This is more advanced usage. In most cases, default queue and command pool maintained
        by the device object should be sufficient.
    */
    ComputationEngine ( const Queue& hQueue, const CommandPool& hCommandPool );

    /**
        \brief Construct a computation engine attached to specified command pool.

        This constructor can be used when you want to schedule the computation
        to a command queue created from default family and allocate command buffers
        from specific command pool. Youi provide only the command pool reference.
        
        This is more advanced usage. In most cases, default queue and command pool maintained
        by the device object should be sufficient.
    */
    ComputationEngine ( const CommandPool& hCommandPool );

    /**
        \brief Compiles all computations.

        Call this method as the final operation inside the constructor of your
        ComputationEngine subclass. It compiles all contained Computation objects
        and prpares them for execution.
    */

    void compile();

    /**
        \brief Waits for all computations to finish.

        This is not the most efficient way to synchronize computations. Better use
        fences or semaphores.
    */
    void wait();

    /**
        \brief Retrieves the device.
    */
    const Device& device() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Base class for single computation routine.

    Derive your own subclass from this class instead of a ComputePass, in order
    to obtain a callable object which can be placed within a ComputationEngine
    derived class, and used like a method.
*/

class Computation : public ComputePass
{
public:
    /**
        \brief Creates a computation without dependency.
    */

    Computation();

    /**
        \brief Creates a computation with a dependency.

        Execution of this computation will wait until the predecessor finishes.
    */
    Computation ( Computation& predecessor );

    /**
        \brief Launches a computation on GPU.

        It launches the computation asynchronously and will exit immediately,
        without waiting. You can optionally specify a fence object to signal
        when computation ends. A fence object can be waited for on the CPU side.
    */

    void operator()( const Fence& sigFenceOnEnd = Fence() );

    /**
        \brief Launches a computation on GPU and waits for completion.

        It launches the computation asynchronously, but will wait until it finishes.

        Specify timeout in nanoseconds or \c vpp::NO_TIMEOUT constant for indefinite
        waiting.
        
        Returns true if the operation finished successfully before the timeout
        has expired. Returns false otherwise (timeout has expired or error occurred).
    */

    bool operator()( std::uint64_t waitTimeout );

    /**
        \brief Schedules a computation to specified queue on the GPU.

        It launches the computation asynchronously and will exit immediately,
        without waiting. You can optionally specify a fence object to signal
        when computation ends. A fence object can be waited fro on the CPU side.
    */
    void operator()( const Queue& hQueue, const Fence& sigFenceOnEnd = Fence() );

    /**
        \brief Schedules a synchronized computation to specified queue on the GPU.

        It launches the computation synchronized by a semaphore. GPU will begin
        execution as soon as the semaphore is signaled. This is all done on the
        GPU, the CPU side will not wait and this method will exit immediately.

        You can optionally specify a semaphore object to signal
        when computation ends.
    */
    void operator()(
        const Queue& hQueue,
        const Semaphore& waitSem,
        const Semaphore& sigSem = Semaphore() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
