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
    \brief Interface to the automatic command recording framework.

    This class is the main interface to the framework used by VPP
    to generate sequences of commands for render and compute passes.
    This is the recommended way to fill command buffers.

    The first step is to create the recorder object. You need only
    to provide the target CommandBuffer object to the constructor.

    Next, call one of the methods depending on what kind of operation
    you want to do:
    - \c render: when you have a RenderPass to be rendered into specific FrameBuffer
      (typical operation for graphics rendering).
    - \c compute: when you have a ComputePass to execute
      (typical operation for compute shader execution).
    - \c presentImage: when you want to create commands to display an image
      to the swapchain (physically on screen).
    - \c unpresentImage: when you want to create commands to unlock the
      image displayed on screen, to overwrite it for the next frame.

    In all cases, do not call \c begin() and \c end() methods on the CommandBuffer,
    as the CommandBufferRecorder does it implicitly.

    The CommandBufferRecorder should be temporary, created on stack.
    Do not store it.
*/

class CommandBufferRecorder
{
public:
    /**
        \brief Creates and initializes the recorder for given command buffer.
    */
    CommandBufferRecorder ( CommandBuffer buffer, std::uint32_t flags = 0 );

    /**
        \brief Finalizes the recorder.
    */
    ~CommandBufferRecorder();

    /**
        \brief Generates commands for graphics rendering, for specified render pass
               and framebuffer.

        The CommandBufferRecorder implicitly does all preparation
        and gathers commands for the render preprocessing step, all subpasses
        and postprocessing step.

        Command sequences for these steps are obtained by execution of the
        corresponding lambda functions in the RenderGraph, associated with
        given RenderPass.
    */
    void render (
        const RenderPass& hRenderPass,
        const FrameBuffer& hFrameBuffer,
        bool bAutoBindPipeline = true );

    /**
        \brief Generates commands for computation, for specified compute pass.

        Command sequence for this step is obtained by execution of the
        corresponding lambda function registered in the ComputePass.
    */
    void compute (
        const ComputePass& hComputePass,
        bool bAutoBindPipeline = true );

    /**
        \brief Generates commands for auxiliary processing, for specified compiled procedure.

        This usually is not called directly, as the CompiledProcedures class already does it.

        Compiled procedure is a sequence of Vulkan commands that are not rendering
        nor computing anything. For example, copying images and buffers and other
        tasks not requiring a pipeline.
        
        Compiled procedures do not have render graphs nor pipelines. They have also
        some extra functions allowing easy calling on CPU side. These functions
        perform queuing and synchronization automatically.

        Command sequence for this step is obtained by execution of the
        corresponding lambda function registered in the procedure.
    */
    void perform (
        const Procedure& hProcedure );

    /**
        \brief Generates commands for presentation of a SwapChain image on screen.

        This command sequence is fixed. Normally you do not need to call this,
        because the SwapChain class does it automatically. Call this method only if
        you use low-level Vulkan rather than SwapChain.
    */
    void presentImage ( VkImage hImage );

    /**
        \brief Generates commands for releasing a SwapChain image currently presented 
               on screen.

        This command sequence is fixed. Normally you do not need to call this,
        because the SwapChain class does it automatically. Call this method only if
        you use low-level Vulkan rather than SwapChain.
    */
    void unpresentImage ( VkImage hImage );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
