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
    \brief Pipeline layout template for a graphics pipeline. Use to create actual
    pipeline layout (Vulkan objects) from abstract representation in the form
    of PipelineConfig object.

    Place an instance of PipelineLayout inside your rendering engine class. As
    the DefinitionT type, supply your custom class derived from PipelineConfig.

    In the constructor, provide references to parent Process (as a pipeline is
    always bound to some Process), target Device and any number of your own
    arguments. Those optional arguments will be passed to the constructor of
    your PipelineConfig subclass. You can parameterize your PipelineConfig
    this way.

    After construction, register the PipelineLayout in your RenderPass object
    by calling RenderPass::addPipeline().

    The PipelineLayout template creates an instance of DefinitionT internally.
    Optionally you can pass your own arguments to the constructor. You can
    retrieve the definition object by calling definition() method.
*/

template< class DefinitionT >
class PipelineLayout
{
public:
    /**
        \brief Constructs a pipeline layout attached to specified Process
               and Device, with optional list of user-specific arguments.
               The constructor pass the extra arguments to the DefinitionT
               constructor.
    */
    template< typename... Args >
    PipelineLayout ( const Process& hProcess, const Device& hDevice, Args... args );

    /**
        \brief Retrieves Vulkan handle for the pipeline layout.
    */

    VkPipelineLayout handle() const;

    /**
        \brief Retrieves the device.
    */
    const Device& device() const;

    /**
        \brief Retrieves the definition object. Useful to call resource binding
        methods defined in your pipeline config from Process command sequence.
    */

    DefinitionT& definition();

    /**
        \overload
    */
    const DefinitionT& definition() const;
};

// -----------------------------------------------------------------------------
/**
    \brief %Pipeline layout template for a compute pipeline. Use to create actual
    pipeline layout (Vulkan objects) from abstract representation in the form
    of ComputePipelineConfig object.

    Place an instance of ComputePipelineLayout inside your computation engine
    class. As the \c DefinitionT type, supply your custom class derived from
    ComputePipelineConfig.

    In the constructor, provide a reference to target Device and any number
    of your own arguments. Those optional arguments will be passed to
    the constructor of your ComputePipelineConfig subclass. You can
    parameterize your ComputePipelineConfig this way.

    After construction, register the ComputePipelineLayout in your
    ComputePass object by calling addPipeline().

    The ComputePipelineLayout template creates an instance of \c DefinitionT internally.
    Optionally you can pass your own arguments to the constructor. You can
    retrieve the definition object by calling the definition() method.

    An example:

    \code
        class MyPipelineConfig : public vpp::ComputePipelineConfig
        {
        public:
            MyPipelineConfig ( const vpp::Device& hDevice )
                m_computeShader ( this, { 32, 1, 1 }, & MyPipelineConfig::fComputeShader )
            {}

            void setDataBuffers (
                vpp::ShaderDataBlock* pDataBlock,
                const vpp::UniformBufferView& dataInput,
                const vpp::UniformBufferView& dataOutput )
            {
                pDataBlock->update ( (
                    m_dataInput = dataInput,
                    m_dataOutput = dataOutput
                ) );
            }

            void fComputeShader ( vpp::ComputeShader* pShader )
            {
                using namespace vpp;

                UniformSimpleArray< Float, float, decltype ( m_dataInput ) > inData ( m_dataInput );
                UniformSimpleArray< Float, float, decltype ( m_dataOutput ) > outData ( m_dataOutput );

                const IVec3 workgroupId = pShader->inWorkgroupId;
                const IVec3 localId = pShader->inLocalInvocationId;
                const Int g = workgroupId [ X ];
                const Int l = localId [ X ];
                const Int i = ( g << 5 ) + l;

                const Float s = inData [ i ];
                const Float d = s*s;
                outData [ i ] = d;
            }

        private:
            vpp::inUniformBuffer m_dataInput;
            vpp::ioBuffer m_dataOutput;
            vpp::computeShader m_computeShader;
        };

        class MyComputePass : public vpp::ComputePass
        {
        public:
            MyComputePass ( const vpp::Device& dev ) :
                m_myPipelineLt ( dev ),
                m_dataBlock ( m_myPipelineLt ),
                m_myDataSrc ( 2048, vpp::MemProfile::DEVICE_STATIC, dev )
                m_myDataDest ( 2048, vpp::MemProfile::DEVICE_STATIC, dev )
            {
                // Register and compile the pipeline.
                addPipeline ( m_myPipelineLt );

                // Obtain MyPipelineConfig object and call the data binding method.
                m_myPipelineLt.definition().setDataBuffers (
                    & m_dataBlock,
                    m_myDataSrc,
                    m_myDataDest
                );
            
                // Provide command sequence as lambda.
                (*this) << [ this ]()
                {
                    // Retrieve compiled pipeline object and select it for subsequent computation.
                    pipeline ( 0 ).cmdBind();

                    // Select the data block for subsequent computation.
                    m_dataBlock.cmdBind();

                    // Launch the computation in 64 workgroups.
                    cmdDispatch ( 64, 1, 1 );
                };            
            }

        private:
            // Declare ComputePipelineLayout instance.
            vpp::ComputePipelineLayout< MyPipelineConfig > m_myPipelineLt;

            // Declare a data block.
            vpp::ShaderDataBlock m_dataBlock;

            // Declare a data source and target.
            vpp::gvector< float, Buf::UNIFORM > m_myDataSrc;
            vpp::gvector< float, Buf::STORAGE > m_myDataDest;
        };

    \endcode
*/

template< class DefinitionT >
class ComputePipelineLayout
{
public:
    /**
        \brief Constructs a pipeline layout attached to specified Device,
        with optional list of user-specific arguments.
        The constructor pass the extra arguments to the DefinitionT
        constructor.
    */
    template< typename... Args >
    ComputePipelineLayout ( const Device& hDevice, Args... args );

    /**
        \brief Retrieves Vulkan handle for the pipeline layout.
    */
    VkPipelineLayout handle() const;

    /**
        \brief Retrieves the device.
    */
    const Device& device() const;

    /**
        \brief Retrieves the definition object. Useful to call resource binding
        methods defined in your pipeline config from the ComputePass command sequence.
    */
    DefinitionT& definition();

    /**
        \overload
    */
    const DefinitionT& definition() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
