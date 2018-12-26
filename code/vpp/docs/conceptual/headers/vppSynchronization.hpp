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
    \brief Allows the CPU to wait for GPU operation to finish.

    Many of VPP operations (esp. queue submission) accept optional Fence
    argument, allowing to wait on CPU side untile the operation finishes.
    When that happens, the fence is being set to signaled state.

    This object is reference-counted and may be passed by value.
*/

class Fence
{
public:
    /** \brief Constructs null reference. */
    Fence();

    /** \brief Constructs a fence on specified device.
    
        You can optionally specify the initial fence state: signaled or unsignaled.
    */
    Fence ( const Device& hDevice, bool bSignaled = false );

    /** \brief Retrieves the Vulkan handle. */
    VkFence handle() const;

    /** \brief Retrieves the device. */
    const Device& device() const;

    /** \brief Checks whether the fence is currently in signaled state. */
    bool isSignaled() const;

    /** \brief Resets the fence to unsignaled state. */
    void reset();

    /** \brief Resets all given fences to unsignaled state. */
    static void reset ( std::vector< Fence >* pFences );

    /** \brief Waits until the fence becomes signaled.
    
        Optionally allows to specify a timeout in nanoseconds. When omitted,
        the wait will be for indefinite time (may hang the application).

        Returns \c true if the fence has really been signaled. Returns \c false
        if waiting has been interrupted by a timeout.
    */
    bool wait ( std::uint64_t timeoutNs = std::numeric_limits< std::uint64_t >::max() ) const;

    /** \brief Waits until all given fences become signaled.
    
        Optionally allows to specify a timeout in nanoseconds. When omitted,
        the wait will be for indefinite time (may hang the application).

        Returns \c true if fences have really been signaled. Returns \c false
        if waiting has been interrupted by a timeout.
    */
    static bool waitAll (
        std::vector< Fence >* pFences,
        std::uint64_t timeoutNs = std::numeric_limits< std::uint64_t >::max() );

    /** \brief Waits until one of given fences becomes signaled.
    
        Optionally allows to specify a timeout in nanoseconds. When omitted,
        the wait will be for indefinite time (may hang the application).

        Returns \c true if a fence have really been signaled. Returns \c false
        if waiting has been interrupted by a timeout.
    */
    static bool waitOne (
        std::vector< Fence >* pFences,
        std::uint64_t timeoutNs = std::numeric_limits< std::uint64_t >::max() );

    /** \overload */
    static bool waitAll (
        const Device& hDevice,
        std::vector< VkFence >* pFences,
        std::uint64_t timeoutNs = std::numeric_limits< std::uint64_t >::max() );

    /** \overload */
    static bool waitOne (
        const Device& hDevice,
        std::vector< VkFence >* pFences,
        std::uint64_t timeoutNs = std::numeric_limits< std::uint64_t >::max() );
};

// -----------------------------------------------------------------------------
/**
    \brief Allows the GPU to wait for another GPU operation to finish.

    Many of VPP operations (esp. queue submission) accept optional Semaphore
    arguments, allowing to delay a GPU operation to the moment when another
    GPU operation finishes. When that happens, the semaphore is being set
    to signaled state and the operation instructed to wait for it is resumed.

    This object is reference-counted and may be passed by value.
*/

class Semaphore
{
public:
    /** \brief Constructs null reference. */
    Semaphore();

    /** \brief Constructs a semaphore on specified device.
    
        A semaphore is always created in unsignaled state.

        The semaphore will be signaled when the first operation finishes.

        The semaphore will be unsignaled automatically when the second
        operation starts.

        There must be always a pair of operations sharing common semaphore:
        one which signals it and another one which waits for (and unsignals) it.
    */
    Semaphore ( const Device& hDevice );

    /** \brief Retrieves the Vulkan handle. */
    VkSemaphore handle() const;

    /** \brief Retrieves the device. */
    const Device& device() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Allows the GPU to wait for certain condition on CPU or GPU side to occur.

    This object is reference-counted and may be passed by value.
*/

class Event
{
public:
    /** \brief Constructs null reference. */
    Event ( const Device& hDevice );

    /** \brief Retrieves the Vulkan handle. */
    VkEvent handle() const;

    /** \brief Retrieves the device. */
    const Device& device() const;
    
    /** \brief Sets the signaled or unsignaled state of the event. */
    void signal ( bool bSignal );

    /** \brief Checks whether the event is signaled. */
    bool isSignaled() const;

    /**
        \brief Generates a command to set signaled or unsignaled event state.

        This command sets the state of the event during GPU execution. The
        state change is being performed during the pipeline stage specified
        by the \c stageMask parameter.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */

    void cmdSignal (
        bool bSignal,
        VkPipelineStageFlags stageMask,
        CommandBuffer hCommandBuffer = CommandBuffer() );

    /**
        \brief Generates a command to wait for signaled event state.

        This command instructs GPU to wait until the state of the event
        becomes signaled.

        The \c srcStageMask parameter specifies the pipeline stages in which
        an event state change might occur, to be taken into account. This
        corresponds to the \c stageMask parameter in cmdSignal() method.
        In case of events triggered on CPU side, use \c VK_PIPELINE_STAGE_HOST_BIT
        stage flag.

        The \c dstStageMask parameter specifies the pipeline stage in which
        the wait operation shall occur (the stage will be executed after
        the event is signaled).

        The wait and corresponding signal operations should be issued on
        the same queue within the device.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */

    void cmdWait ( 
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        CommandBuffer hCommandBuffer = CommandBuffer() );

    /**
        \brief Generates a command to wait for signaled event state.

        This command instructs GPU to wait until the state of the event
        becomes signaled and produced resources are ready for further processing.

        For the second goal, it defines a pipeline barrier. That means two things:
        - Besides waiting for the event, it synchronizes two pipeline stages.
          It guarantees that data produced by the source stage are available
          when executing the destination stage.
        - You can specify one or more resources to wait for, using the Barriers
          class.

        The wait and corresponding signal operations should be issued on
        the same queue within the device.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */

    void cmdWait ( 
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        const Barriers& barriers,
        CommandBuffer hCommandBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
