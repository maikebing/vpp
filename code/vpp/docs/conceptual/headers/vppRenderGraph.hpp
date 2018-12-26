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
    \brief Base class for render graphs.

    Inherit from this class to make your custom render graph class. Additionally
    RenderGraph defines some convenience wrappers for Vulkan drawing commands,
    including those meant for operation on the implicit context.

    Since this class serves as a counted reference to internal representation, you
    can pass RenderGraph by value. In order to do be able to that, keep your
    custom render graph class lightweight. Store only graph nodes (e.g. Process,
    Attachment, Preprocess and Postprocess) in the object.

    An example of renderer using a simple render graph:

    \code
        // Define formats you use.
        typedef vpp::format< float, vpp::DT > DepthFormat;

        // The render graph itself.

        class DepthBufferRenderGraph : public vpp::RenderGraph
        {
        public:
            DepthBufferRenderGraph (
                unsigned int width, unsigned int height, vpp::Device hDevice );

        public:
            vpp::Device m_hDevice;

            // Single subpass, as well as initializing/finalizing steps.
            vpp::Process m_renderDepthAttachment;
            vpp::Preprocess m_preprocess;
            vpp::Postprocess m_postprocess;

            // Single output image (attachment).
            vpp::Attachment< DepthFormat > m_depthAttachmentBuffer;
        };

        DepthBufferRenderGraph :: DepthBufferRenderGraph (
            unsigned int width, unsigned int height, vpp::Device hDevice ) :
                m_hDevice ( hDevice ),
                m_depthAttachmentBuffer ( width, height )
        {
            // Register the output image in the process node.
            m_renderDepthAttachment.setDepthOutput (
                m_depthAttachmentBuffer, 1.0f,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
            );
        }

        // The renderer. Not all details are shown.

        class DepthBufferRenderer
        {
        public:
            DepthBufferRenderer (
                DepthBufferView* pBufferView, vpp::Device hDevice );

            // This will create rendering command sequence.
            void createRenderPlan ( vpp::CommandBuffer hCmdBuffer );

        private:
            vpp::Device m_hDevice;
            DepthBufferView* m_pBufferView;
            VkExtent3D m_imageSize;

            // The render graph object.
            std::shared_ptr< DepthBufferRenderGraph > m_renderGraph;

            // The pipeline - not shown in this example for brevity.
            vpp::PipelineLayout< DepthBufferRenderPipeline > m_renderPipeline;

            // The data for the pipeline - not shown in this example for brevity.
            vpp::ShaderDataBlock m_renderShaderDataBlock;

            // The render pass object.
            vpp::RenderPass m_renderPass;

            // Options - not shown in this example for brevity.
            vpp::RenderingOptions m_renderOptions;

            // The frame buffer object.
            vpp::FrameBuffer m_frameBuffer;

            // ...
        };

        DepthBufferRenderer :: DepthBufferRenderer (
            DepthBufferView* pBufferView, vpp::Device hDevice ) :
                m_hDevice ( hDevice ),
                m_pBufferView ( pBufferView ),
                m_imageSize ( pBufferView->image().info().extent ),
                m_renderGraph ( new DepthBufferRenderGraph (
                    m_imageSize.width, m_imageSize.height, hDevice ) ),
                m_renderPipeline ( m_renderGraph->m_renderDepthAttachment, hDevice ),
                m_renderShaderDataBlock ( m_renderPipeline ),
                m_renderPass ( *m_renderGraph, hDevice ),
                m_frameBuffer (
                    std::vector< vpp::FrameImageView > {
                        vpp::FrameImageView ( pBufferView->image() )
                    },
                    m_renderPass ),
                ...
        {
            using namespace vpp;

            // Register the pipeline within render pass.
            m_renderPass.addPipeline (
                m_renderGraph->m_renderDepthAttachment,
                m_renderPipeline, m_renderOptions );

            // Register the command sequence for the rendering process.
            // Note the lambda function - it wil be executed later! 
            m_renderGraph->m_renderDepthAttachment << [ this ]()
            {
                // Select current pipeline.
                m_renderPass.pipeline ( 0, 0 ).cmdBind();

                // Select current data set.
                m_renderShaderDataBlock.cmdBind();

                // Bind the buffer with vertices (details not shown).
                m_renderPipeline->cmdBindVertexInput ( ... );

                // Draw (details not shown).
                m_renderGraph->cmdDraw ( ... );
            };

            // Register the command sequence for the initializing process.
            // Note the lambda function - it will be executed later! 
            m_renderGraph->m_preprocess << [ this ]()
            {
                // Insert here any commands to be run before the render pass begins.
                // ...
            };

            // Register the command sequence for the finalizing process.
            // Note the lambda function - it will be executed later! 
            m_renderGraph->m_postprocess << [ this ]()
            {
                // Insert here any commands to be run after the render pass ends.
                // ...
            };
        }

        // Convenience method.
        void DepthBufferRenderer :: createRenderPlan ( vpp::CommandBuffer hCmdBuffer )
        {
            vpp::CommandBufferRecorder recorder ( hCmdBuffer );

            // This will generate commands into the command buffer.
            // Execution of the lambdas above will be triggered by this call.
            recorder.render ( m_renderPass, m_frameBuffer, false );
        }

    \endcode

*/

class RenderGraph :
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    /** \brief Constructs null reference. */
    RenderGraph();

    /** \brief Decreases reference count. */
    ~RenderGraph();

    /**
        \brief Generates a command to draw specified region of vertex/instance buffers.

        Vertex and instance buffers (when applicable) must be bound in currently
        recorded command sequence by calling PipelineConfig::cmdBindVertexInput()
        prior to issuing cmdDraw(). This should be done for all binding points used
        in the vertex shader.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        Usually this command is called from the lambda routine registered for
        the Process node. Do not specify command buffer in such case.
    */
    static void cmdDraw (
        unsigned int vertexCount,
        unsigned int instanceCount,
        unsigned int firstVertex,
        unsigned int firstInstance,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Generates a command to draw specified region of vertex/instance buffers,
        using additional index buffer to address vertices.

        Vertex and instance buffers (when applicable) must be bound in currently
        recorded command sequence by calling PipelineConfig::cmdBindVertexInput()
        and prior to issuing cmdDrawIndexed(). This should be done for all binding points
        used in the vertex shader.

        Also bind the index buffer by calling PipelineConfig::cmdBindIndexInput().
        The index buffer should contain an array of 32-bit integers. One of possibilities
        is to use Indices container class for it.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        Usually this command is called from the lambda routine registered for
        the Process node. Do not specify command buffer in such case.
    */
    static void cmdDrawIndexed (
        unsigned int indexCount,
        unsigned int instanceCount,
        unsigned int firstIndex,
        int vertexOffset,
        unsigned int firstInstance,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Generates a command to draw specified region of indirect drawing
        buffer.

        The indirect drawing buffer contains parameters for implicit draw commands
        which are automatically generated. This allows to draw multiple regions
        of vertex/instance buffers using single draw call. The buffer should
        contain an array of VkDrawIndirectCommand structures. One of possibilities
        is to use the IndirectCommands container class for it.

        Vertex and instance buffers (when applicable) must be bound in currently
        recorded command sequence by calling PipelineConfig::cmdBindVertexInput()
        and prior to issuing cmdDrawIndirect(). This should be done for all binding points
        used in the vertex shader.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        Usually this command is called from the lambda routine registered for
        the Process node. Do not specify command buffer in such case.
    */
    static void cmdDrawIndirect (
        const IndirectBufferView& data,
        VkDeviceSize offset,
        unsigned int drawCount,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Generates a command to draw specified region of indirect drawing
        buffer, also using an index buffer to address vertices.

        The indirect drawing buffer contains parameters for implicit draw commands
        which are automatically generated. This allows to draw multiple regions
        of index/instance buffers using single draw call. The buffer should
        contain an array of VkDrawIndexedIndirectCommand structures. One of possibilities
        is to use the IndexedIndirectCommands container class for it.

        Vertex and instance buffers (when applicable) must be bound in currently
        recorded command sequence by calling PipelineConfig::cmdBindVertexInput()
        and prior to issuing cmdDrawIndexedIndirect(). This should be done for all binding points
        used in the vertex shader.

        Also bind the index buffer by calling PipelineConfig::cmdBindIndexInput().
        The index buffer should contain an array of 32-bit integers. One of possibilities
        is to use Indices container class for it.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        Usually this command is called from the lambda routine registered for
        the Process node. Do not specify command buffer in such case.
    */
    static void cmdDrawIndexedIndirect (
        const IndirectBufferView& data,
        VkDeviceSize offset,
        unsigned int drawCount,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Generates a command to clear specified attachment images.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */
    static void cmdClearImages (
        unsigned int attachmentCount,
        const VkClearAttachment* pAttachments,
        unsigned int rectCount,
        const VkClearRect* pRects,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
