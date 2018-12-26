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
    \brief Compiled pipeline class for graphics pipelines.

    Represents compiled rendering pipeline. Usually objects of this class
    are managed by RenderPass class. The RenderPass creates compiled
    pipelines for registered PipelineLayout objects. You can then retrieve
    the pipeline object by index. The reason for doing so is the access to
    cmdBind method, which allows to select active pipeline inside Process
    command sequence. You call this method when the rendering process
    consists of multiple draw commands, using different pipelines.

    Although you can also construct the Pipeline object directly, 
    there is rarely any need to do this.

    This object is reference counted and can be passed by value.
*/

class Pipeline
{
public:
    /** \brief Constructs null reference. */
    Pipeline();

    /** \brief Constructs a pipeline object from Vulkan handle. */
    Pipeline ( VkPipeline hPipeline, const Device& hDevice );

    /** \brief Retrieves the Vulkan handle of the pipeline. */
    VkPipeline handle() const;

    /** \brief Generates a command to select the current pipeline.
    
        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */
    void cmdBind ( CommandBuffer hCmdBuffer = CommandBuffer() ) const;
};

// -----------------------------------------------------------------------------

/**
    \brief Compiled pipeline class for compute pipelines.

    Represents compiled computing pipeline. Usually objects of this class
    are managed by ComputePass class. The ComputePass creates compiled
    pipelines for registered ComputePipelineLayout objects. You can then retrieve
    the pipeline object by index. The reason for doing so is the access to
    cmdBind method, which allows to select active pipeline inside the compute
    pass command sequence. You call this method when the computing process
    consists of multiple dispatch commands, using different pipelines.

    Although you can also construct the ComputePipeline object directly, 
    there is rarely any need to do this.

    This object is reference counted and can be passed by value.
*/

class ComputePipeline
{
public:
    /** \brief Constructs null reference. */
    ComputePipeline();

    /** \brief Constructs a pipeline object from Vulkan handle. */
    ComputePipeline ( VkPipeline hPipeline, const Device& hDevice );

    /** \brief Retrieves the Vulkan handle of the pipeline. */
    VkPipeline handle() const;

    /** \brief Generates a command to select the current pipeline.
    
        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */
    void cmdBind ( CommandBuffer hCmdBuffer = CommandBuffer() ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
