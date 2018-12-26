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
    \brief Command queue for execution of commands.

    This class represents a command queue. This is the means to execute
    commands recorded into command buffers.

    Queues are contained within the rendering device (represented by a Device
    object). Each device contains at least one queue. Most real devices
    implement several queues.

    Queues are categorized into families. This is because queues can have
    different capabilities, limiting selection of commands that may be
    performed by particular queue, but offering increased performance
    in return. There may be more than one queue within a family. Each
    family has an integer index.

    VPP offers a simplified way to select a queue family, by using the
    EQueueType enumeration. There are currently two capability profiles:
    - \c Q_GRAPHICS - can execute all commands,
    - \c Q_TRANSFER - can execute only transfer commands, but at increased performance.

    In order to access a specific queue from desired family, use one of the
    constructors in Queue class to construct a Queue object. The object can
    be treated as a reference to the queue. Lifetime of the queue itself is
    not affected. You can quickly create Queue objects you need, submit
    commands to them and finally discard these objects.

    Use the Queue::submit() methods to submit actual CommandBuffer
    objects to the queue. This is the topmost level operation in Vulkan
    to initiate rendering or any computation.

    This object is reference-counted and may be passed by value.
*/

class Queue
{
public:
    /** \brief Constructs null reference. */
    Queue();

    /** \brief Constructs a queue reference to specified queue.
    
        You must specify the device to access a queue belonging to.

        Queue index is optional. Also optionally you can specify a queue
        family, using the EQueueType enumeration.
    */
    explicit Queue (
        const Device& hDevice,
        unsigned int iQueue = 0,
        EQueueType eQueue = Q_GRAPHICS );

    /** \brief Constructs a queue reference to specified queue.
    
        You must specify the device to access a queue belonging to.
        Also in this constructor, you must explicitly provide queue
        family index and queue index within the family. This is the
        low-level constructor.

        You can enumerate queue families and query their properties by using
        methods in PhysicalDevice class or Vulkan API directly. This is considered
        low-level usage and is rarely required.
    */
    Queue (
        unsigned int iFamily,
        const Device& hDevice,
        unsigned int iQueue );

    /**
        \brief Retrieves Vulkan handle to the queue object.
    */
    VkQueue handle() const;

    /**
        \brief Retrieves the parent device for the queue.
    */
    const Device& device() const;

    /**
        \brief Retrieves the type of the queue.
    */
    EQueueType type() const;

    /**
        \brief Checks if the queue object is valid (not a null reference).
    */
    operator bool() const;

    /**
        \brief Submits a command buffer (CommandBuffer object) for execution.

        This is the primary method of execution of all Vulkan commands by VPP.

        Commands need to be recorded first into a CommandBuffer, forming
        a sequential program. Then the cxommand sequence is being sent
        for execution to the chosen queue.

        This method does not wait for completion by default. The only required
        argument is the command buffer itself.
        
        Optionally you can specify one or more synchronization primitives.
        None of them is mandatory, provide any set of them depending on your
        needs. These are the following primitives:
        - \c waitOnBegin: a Semaphore to wait on before the execution begins. You can
          ensure this way that some other rendering stage finishes before the
          execution of provided command buffer starts.
        - \c signalOnEnd: a Semaphore to be signaled when the execution ends. This
          allows to make other rendering phase dependent on this phase.
        - \c signalFenceOnEnd: a Fence to be signalled when the execution ends. Allows
          to wait for the finishing of execution on the CPU side. This is different
          from using the semaphores, as semaphores synchronize execution only on
          GPU side.
    */

    void submit (
        const CommandBuffer& singleBuffer,
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        const Fence& signalFenceOnEnd = Fence() );

    /**
        \brief Submits a sequence of command buffers (CommandBuffer objects) for execution.

        Alternative method allowing to set entire vector of command buffers at once.
    */

    void submit (
        const std::vector< CommandBuffer > buffers,
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        const Fence& signalFenceOnEnd = Fence() );

    /**
        \brief Waits until all operations on current queue are finished.

        This is CPU side wait. It is very crude alternative to using fences,
        useful when we have no fence to wait on, but still want all jobs
        one the queue to be done before submitting more work.
    */

    VkResult waitForIdle();
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
