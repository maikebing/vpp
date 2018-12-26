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
    \brief Represents a group of resources bound to rendering or compute pipeline.

    Resources can be bound to a pipeline by means of binding points inside the
    pipeline. However, these binding points do not hold the actual data objects
    (images, buffers) directly. Binding points only represent metadata defining
    the resources. Actual references to data objects are grouped and stored inside
    ShaderDataBlock instances.
    
    A ShaderDataBlock is a special kind of vector, holding the references to buffer
    and image views. First, you fill that vector by calling the update() method,
    then you bind entire vector at once by means of cmdBind() method.

    Filling up the ShaderDataBlock happens instantly and should be done in
    frame preparation phase. Binding is a command, which should be placed
    in Process command sequence or recorded explicitly to a command buffer.
    Execution of the command happens during rendering phase and bound resources
    are used for subsequent drawing commands, until next binding happens or
    rendering phase ends.

    A ShaderDataBlock instance is associated with particular pipeline, represented
    by a PipelineLayout<> object. It holds references to all relevant resources
    defined inside the pipeline, particularly all images and buffers (but not
    push constants, those are updated in different way). You can create multiple
    shader data blocks, holding different sets of resources.

    ShaderDataBlock is a high-level counterpart of a descriptor set, low-level
    Vulkan object.

    Updating a shader data block involves calling the update template method.
    You specify an assignment list as the argument. This is a list of assignments
    separated by the comma operator and enclosed in extra parentheses. On the
    left hand side of each assignment you place a binding point name. On the
    right hand side there should be appropriate view object, denoting a buffer
    or image. You can bind any number of resources at once.

    A good practice is to create a special method inside your PipelineConfig
    subclass. This method takes a ShaderDataBlock pointer (or reference),
    as well as references to resource views to be bound to that data block.
    This way you have easy access to binding points and may make the private
    in the class. Now, in order to initialize a ShaderDataBlock, you only need
    to call this method from the outside.

    An example of updating (for graphics pipelines it is the same):

    \code
        class KPLGenReducedSet : public vpp::ComputePipelineConfig
        {
        public:
            // ...

            // This is the method whid initializes ShaderDataBlock for this pipeline.
            void setDataBuffers (
                vpp::ShaderDataBlock* pDataBlock,
                const vpp::UniformBufferView& frameStdInput,
                const vpp::UniformBufferView& lightDataInput,
                const KDepthValuesView& depthValuesView,
                const KConstU32View& lightList,
                const KVarU8View& reducedSetOutput );

            // ...

        private:
            vpp::inPushConstant< TFrameSelector > d_frameSelector;
            vpp::inUniformBuffer d_frameStdInput;
            vpp::ioBuffer d_lightDataInput;
            vpp::inTexture< KDepthValuesView > d_depthValues;
            vpp::inTexture< KConstU32View > d_lightList;

            vpp::ioImage< KVarU8View > d_reducedSetOutput;

            vpp::computeShader d_computeShader;
        };

        void KPLGenReducedSet :: setDataBuffers (
            vpp::ShaderDataBlock* pDataBlock,
            const vpp::UniformBufferView& frameStdInput,
            const vpp::UniformBufferView& lightDataInput,
            const KDepthValuesView& depthValuesView,
            const KConstU32View& lightList,
            const KVarU8View& reducedSetOutput )
        {
            // Note that we do not update the push constant.

            pDataBlock->update ( (
                d_frameStdInput = frameStdInput,
                d_lightDataInput = lightDataInput,
                d_depthValues = depthValuesView,
                d_lightList = lightList,
                d_reducedSetOutput = reducedSetOutput
            ) );
        }

    \endcode

    ShaderDataBlock objects are reference-counted and may be passed by value.
*/

class ShaderDataBlock
{
public:
    /** \brief Constructs null reference. */
    ShaderDataBlock();

    /** \brief Constructs data block for specified PipelineLayout object.
    
        The reference to opaque vpp::PipelineLayoutBase type represents any
        PipelineLayout< ... > object (it is a base class). You can use this type
        to pass layout references into your own functions while delegating
        ShaderDataBlock construction.
    */
    ShaderDataBlock ( const PipelineLayoutBase& hLayout );

    /** \brief Retrieves Vulkan handle of the descriptor set. */
    VkDescriptorSet getDescriptorSet ( std::uint32_t iSet ) const;

    /** \brief Retrieves the device. */
    const Device& device() const;

    /**
        \brief Updates resources specified in the assignment list.

        See the description of the class ShaderDataBlock for more details on
        the assignment list syntax.
    */
    template< class AssignmentListT >
    void update ( const AssignmentListT& list );

    /**
        \brief Generates a command that binds the data block to the pipeline.

        Subsequent draw commands will use resources stored in this data block.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.
    */
    void cmdBind ( CommandBuffer hCmdBuffer = CommandBuffer() ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
