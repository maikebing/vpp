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
    \brief Represents Vulkan render pass.

    In VPP, the RenderPass class is usually used in conjunction with
    CommandBufferRecorder class. CommandBufferRecorder produces a command
    buffer from given RenderPass and FrameBuffer. This command buffer,
    when submitted to a queue, instructs the device to do actual rendering.
*/

class RenderPass
{
public:
    /** \brief Construct a render pass from given render graph. */
    RenderPass (
        const RenderGraph& renderGraph,
        const Device& hDevice );

    /** \brief Construct a render pass from given render graph and user-supplied pipeline cache. */
    RenderPass (
        const RenderGraph& renderGraph,
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    /** \brief Retrieves Vulkan handle to the render pass. */
    VkRenderPass handle() const;

    /** \brief Retrieves render graph associated with this render pass. */
    RenderGraph& graph() const;

    /** \brief Retrieves the device associated with this render pass. */
    const Device& device() const;

    /** \brief Retrieves the pipeline cache associated with this render pass. */
    const PipelineCache& pipelineCache() const;

    /** \brief Retrieves a pipeline associated with this render pass,
        specified process and pipeline index.
    */
    const Pipeline& pipeline ( unsigned int iProcess, unsigned int iPipeline ) const;

    /** \brief Retrieves a pipeline associated with this render pass,
        specified process and pipeline index.
    */
    const Pipeline& pipeline ( const Process& hProcess, unsigned int iPipeline ) const;

    /** \brief Registers a pipeline with this render pass and associates it
        with specified process and rendering options. Returns pipeline index.
    */
    unsigned int addPipeline (
        unsigned int iProcess,
        const PipelineLayoutBase& layout,
        const RenderingOptions& options );

    /** \brief Registers a pipeline with this render pass and associates it
        with specified process and rendering options. Returns pipeline index.
    */
    unsigned int addPipeline (
        const Process& hProcess,
        const PipelineLayoutBase& layout,
        const RenderingOptions& options );

    /**
        \brief Call before manually recording commands for this render pass. The
        CommandBufferRecorder class calls this automatically.
    */
    void beginRendering();

    /**
        \brief Call after finishing manually recording commands for this render pass.
        The CommandBufferRecorder class calls this automatically.
    */
    void endRendering();
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
