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

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Represents compute pass.

    A compute pass is a single pass in GPU computation. It defines a set of
    input, output and intermediate resources and executes one or more
    computing pipelines on it.

    Compute pass is generally similar concept to render pass, but does not
    render any graphics, and has neither a render graph, nor frame buffer.
    It can however generate images, as all types of resources are allowed
    for compute passes.

    Compute passes contain compute pipelines instead of rendering pipelines.
    Use the ComputePipelineLayout template to define a compute pipeline.
    There can be multiple pipelines per pass. They are selected in the same
    way as in render passes (cmdBind() method).

    In VPP, the ComputePass class is usually used in conjunction with
    CommandBufferRecorder class. CommandBufferRecorder produces a command
    buffer from given ComputePass. This command buffer,
    when submitted to a queue, instructs the device to do actual computing.

    The computation involves calling the compute shader repeatedly, like
    in a multi-level loop. Individual executions are parallelized, according
    to device capabilities. As there is usually more calls than physical
    compute units, some calls will be serialized also. 

    There are two main levels of looping during the computation - the local
    level and global level. Each of these levels is further divided into
    three sub-levels, which can represent three dimensions.

    The global looping logically divides computation into workgroups.
    A workgroup is a computation unit that does not share fast communication memory
    with other workgroups. Partition your computation work in such way, that no such
    communication is needed (otherwise the performance will be low). Specifically,
    for most GPUs currently being on the market, different workgroups are usually
    executed on different cores on chip (e.g. CUs). Sometimes they can also
    be executed on same core in parallel, but that should not be relied upon.
    If there are more workgroups than physical units, they will be executed
    sequentially.

    The local level divides each workgroup into local threads. Local threads
    within same workgroup have access to special memory block on GPU allowing
    faster exchange of data than main GPU memory. Local threads are intended
    to be executed on single core, in a SIMD scheme. If there is more local
    threads than SIMD channels, they will be serialized.

    A conceptual pseudocode for computation in Vulkan model is as follows:

    \code
        // { nx, ny, nz } - dimensions of entire problem (counted in workgroups)
        // { ni, nj, nk } - dimensions of single workgroup (counted in threads)

        // This is implicit code executed by Vulkan (it does not really exist).
        // The loops are being iterated in parallel when possible.

        void compute ( int nx, int ny, int nz, int ni, int nj, int nk )
        {
            // global level
            for ( int x = 0; i < nx; ++x )
                for ( int y = 0; y < ny; ++y )
                    for ( int z = 0; z < nz; ++z )
                    {
                        // local level
                        for ( int i = 0; i < ni; ++i )
                            for ( int j = 0; y < nj; ++j )
                                for ( int k = 0; k < nk; ++k )
                                {
                                    // Schedule the call to computeShaderRoutine.
                                    callComputeShader ( x, y, z, i, j, k );
                                }
                    }
        }

        // This is real GPU code that you write for the compute shader,
        // usually as a method in your ComputePass subclass. It works as
        // a callback routine.

        void computeShaderRoutine ( vpp::ComputeShader* pShader )
        {
            VArray< Int > privateArray ( 8 );
            WArray< Int > sharedArray ( 8 );

            const IVec3 workgroupId = pShader->inWorkgroupId;
            const IVec3 localId = pShader->inLocalInvocationId;
            
            // workgroupId == { x, y, z }
            // threadId == { i, j, k }
            
            // Retrieve input data for your computation according to
            // the indices in workgroupId and localId.

            // For each triple { i, j, k } under the same { x, y, z },
            // this will alter the same item. However, for differing
            // { x, y, z } there will be separate copies of sharedArray.
            sharedArray [ 0 ] = 0;

            // This will alter private copy for each call.
            privateArray [ 0 ] = 0;

            // This ensures all threads in same workgroup will meet here
            // (even when executed sequentially).
            WorkgroupBarrier();
        }

    \endcode
*/

class ComputePass :
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    /** \brief Construct a compute pass. */
    ComputePass (
        const Device& hDevice );

    /** \brief Construct a compute pass with user-supplied pipeline cache. */
    ComputePass (
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    /** \brief Retrieves the device associated with this compute pass. */
    const Device& device() const;

    /** \brief Retrieves the pipeline cache associated with this compute pass. */
    const PipelineCache& pipelineCache() const;

    /** \brief Retrieves a pipeline associated with this compute pass
         and specified pipeline index.
    */
    const ComputePipeline& pipeline ( std::uint32_t iPipeline ) const;

    /** \brief Registers a pipeline with this compute pass. Returns pipeline index.
    */
    template< class DefinitionT >
    std::uint32_t addPipeline ( const ComputePipelineLayout< DefinitionT> layout );

    /**
        \brief Defines a command sequence for compute pass.
    */
    void operator<< ( const std::function< void () >& cmds );

    /**
        \brief Generates a command which starts execution of currently selected
               pipeline in the compute pass.

        This is a counterpart of RenderGraph::cmdDraw() command. It triggers actual computation.
        Call it inside the command sequence (lambda function passed to the \c << operator).
        Before calling cmdDispatch(), also call cmdBind() methods on selected pipeline
        and shader data block which specifies current resource set.

        The arguments \c nx, \c ny and \c nz specify (in three dimensions) how many
        times the compute shader will be called in separate \b workgroup.
        
        The \c ni, \c nj and \c nk values (local workgroup dimensions) are not
        specified here, but rather in the constructor of compute shader binding
        point - see the computeShader class and SLocalSize structure.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        An example of using cmdDispatch():

        \code
            class MyComputePass : public vpp::ComputePass
            {
            public:

            private:
                ComputePipelineLayout< MyPipeline0 > m_plStep0;
                ComputePipelineLayout< MyPipeline1 > m_plStep1;
                ShaderDataBlock m_myShaderDataBlock0;
                ShaderDataBlock m_myShaderDataBlock1;
            };

            MyComputePass :: MyComputePass ( const Device& dev, ... ) :
                m_plStep0 ( dev ),
                m_plStep1 ( dev ),
                m_myShaderDataBlock0 ( m_plStep0 ),
                m_myShaderDataBlock1 ( m_plStep1 )
            {
                // constructor of your ComputePass subclass

                // ...

                // register and compile pipelines
                const unsigned int ipl0 = addPipeline ( m_plStep0 );
                const unsigned int ipl1 = addPipeline ( m_plStep1 );

                // ...

                // register the command sequence as a lambda function

                (*this) << [ this, ipl0, ipl1 ]()
                {
                    // select resources and pipeline for the computation step
                    m_myShaderDataBlock0.cmdBind();

                    // this retrieves compiled pipeline object for m_plStep0
                    pipeline ( ipl0 ).cmdBind();

                    // execute the computation step
                    cmdDispatch ( 64, 1, 1 );

                    // wait for the results
                    cmdImagePipelineBarrier (
                        m_myGeneratedImage0,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        false
                    );

                    // select resources and pipeline for another computation step
                    m_myShaderDataBlock1.cmdBind();

                    // this retrieves compiled pipeline object for m_plStep1
                    pipeline ( ipl1 ).cmdBind();

                    // execute second computation step
                    cmdDispatch ( width, height, 1 );
                }
            }

        \endcode

    */
    static void cmdDispatch (
        unsigned int nx = 1, unsigned int ny = 1, unsigned int nz = 1,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Generates a command to dispatch specified item of indirect dispatch
        buffer.

        The indirect dispatch buffer contains parameters for implicit dispatch commands
        which are internally generated. The buffer should
        contain an array of VkDispatchIndirectCommand structures. One of possibilities
        is to use the DispatchIndirectCommands container class for it.

        This command triggers execution of single dispatch element, specified
        by index in the buffer.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */

    static void cmdDispatchIndirect (
        const IndirectBufferView& data,
        unsigned int index,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
