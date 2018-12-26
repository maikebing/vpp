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
    \brief Represents a sequence of Vulkan commands.

    Rendering and computation in Vulkan is performed by means of commands.
    Sequences of these commands are first recorded into CommandBuffer objects. Next,
    these CommandBuffer objects are submitted for execution to command
    queues exposed by the rendering device.

    In order to create a command buffer, you need to have access to a
    CommandPool object first. The CommandPool object manages resources
    and lifetime for command buffers. One possibility is to use the
    default command pool provided by each Device instance. Use the
    Device::defaultCmdPool() method to retrieve the reference.
    
    Next, call the CommandPool::createBuffer() method to obtain a CommandBuffer
    instance.

    Having a CommandBuffer object, you can proceed in two ways:
    - Use the CommandBufferRecorder object to utilise the VPP framework
      to generate commands to be recorded. These commands are taken from
      Process, Preprocess and Postprocess commands sequences of the
      render graph, or the compute pass sequence.
    - Specify commands directly, using overloads which take the CommandBuffer
      object or VkCommandBuffer handle.

    The first way is the more standard (and recommended) usage.

    After execution of the command buffer has finished, the buffer can
    be freed or reset for reusing. Reusing is typical method. You can
    free or reuse entire bunch of command buffers created by common
    CommandPool object. Using the CommandPool to manage command buffers
    in bulk is recommended.

    CommandBuffer objects are lightweight, do not manage resources implicitly
    and can be passed by value.
*/

class CommandBuffer
{
public:
    /** \brief Constructs a null, invalid command buffer. */
    CommandBuffer();

    /** \brief Constructs a command buffer from given Vulkan handle. */
    CommandBuffer ( VkCommandBuffer hBuffer );

    /** \brief Retrieves the Vulkan handle. */
    VkCommandBuffer handle() const;

    /** \brief Checks whether this command buffer object is valid (non-null). */
    operator bool () const;

    /** \brief Resets the state of individual command buffer for reusing. */
    VkResult reset();

    /** \brief Frees individual command buffer.
    
        You usually do not need to call this, as the parent CommandPool will free
        all its buffers when being destroyed.
    */
    VkResult release();

    enum EBeginFlags
    {
        ONE_TIME_SUBMIT = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
        SIMULTANEOUS_USE = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    /** \brief Sets the buffer in the recording state.
    
        Call only in manual recording mode (not using CommandBufferRecorder).
    
    */
    VkResult begin ( std::uint32_t flags = 0 );

    /** \brief Ends command recording for this buffer.
    
        Call only in manual recording mode (not using CommandBufferRecorder).
    */
    VkResult end();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct RenderingCommandContext
{
public:
    inline RenderingCommandContext ( VkCommandBuffer commandBufferHandle ) :
        d_commandBufferHandle ( commandBufferHandle ),
        d_pPrevContext ( s_pThis )
    {
        s_pThis = this;
    }

    inline ~RenderingCommandContext()
    {
        s_pThis = d_pPrevContext;
    }

    static inline VkCommandBuffer getCommandBufferHandle()
    {
        if ( ! s_pThis )
            throw XUsageError (
                "Invalid command call in non-rendering context. "
                "Perhaps you forgot to specify command buffer explicitly." );

        return s_pThis->d_commandBufferHandle;
    }

private:
    static thread_local RenderingCommandContext* s_pThis;
    VkCommandBuffer d_commandBufferHandle;
    RenderingCommandContext* d_pPrevContext;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMMANDBUFFER_HPP
