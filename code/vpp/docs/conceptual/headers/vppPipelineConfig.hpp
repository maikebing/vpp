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
    \brief Base class for custom rendering pipelines.

    Creating a custom rendering pipeline is one of the most basic tasks
    when using Vulkan. The pipeline consists of the following building blocks:

    - Binding points, which allow to supply required data to GPU shaders.
      The data can be organized in various ways: images, buffers, push
      constants, communication variables, samplers, render graph attachments, etc.
      For more information, see the docs for individual binding point classes:
      inVertexData, inUniformBuffer, inUniformBufferDyn, ioBuffer, ioBufferDyn,
      ioImageBuffer, inTextureBuffer, inPushConstant, ioImage, inSampler,
      inConstSampler, inTexture, inSampledTexture, inConstSampledTexture,
      inAttachment, outAttachment, ioVariable, ioStructure.

    - Shaders, which provide GPU code performing actual computation. In rendering
      pipelines there can be the following shaders: vertexShader, tessControlShader,
      tessEvalShader, geometryShader and fragmentShader.

    You define the pipeline by creating a class derived from PipelineConfig. Inside the
    class, place fields of types corresponding to the building blocks. By convention,
    names of those types starts from lowercase letters, as they logically belong to CPU
    side. You will be able to use these fields to bind data buffers to them.
    
    On the other hand, you also place shader code in the pipeline class,
    as plain C++ methods. In these methods, you have C++ access to the binding points
    mentioned above. With help of some special accessor objects, you can actually
    read data from them on GPU side, perform processing and write results. Entire
    paradigm here is exactly the same as with plain C++, with only difference that
    you need these accessor objects sometimes.

    Pipeline classes can be subject to all object oriented or generic programming
    techniques allowed by C++ like inheritance or templating. You can also call
    external functions from shader code and these functions will be treated
    also like shader code, executable on GPU. This facilitates creating reusable
    libraries of GPU code. Even generic libraries in spirit of STL or boost can
    be created.

    PipelineConfig also provides some configuration methods for parameters
    belonging to fixed function hardware, like blending mode or rasterizer
    logical operations. These methods should be called only from the constructor
    of your subclass.

    You do not instantiate objects of PipelineConfig directly. Instead, supply them
    to the PipelineLayout template, which creates actual Vulkan objects related
    to the pipeline (layout and descriptors).

    Rendering pipelines are associated with rendering processes in
    a render graph (Process nodes). An instance of PipelineConfig object
    is always bound to some render graph and some Process node within it.
    Single render graph can have multiple processes, each with its own pipeline
    or several pipelines.
*/

class PipelineConfig
{
protected:
    /**
        \brief Constructor, called only from subclass constructor.
        
        Do not construct PipelineConfig objects yourself. Use it only as a base
        class for your custom pipeline class. Instantiate the custom pipeline
        through PipelineLayout.
        
        The derived class constructor will be called by PipelineLayout template.
        You must place two mandatory arguments in the constructor: references
        to Process object and Device object. You get these values from PipelineLayout.
        Pass the Process reference to PipelineConfig constructor.

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
            public:
                MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

                // ...
            };

            MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
                vpp::PipelineConfig ( pr ),
                // ...
            {
            }

            class MyRenderer
            {
                // ...
                vpp::PipelineLayout< MyPipelineConfig > m_pipelineLayout;

                // ...
            };

            // in constructor of the renderer class
            MyRenderer() :
                // ...
                m_pipelineLayout ( m_renderGraph.m_process, m_device ),
                // ...
            {
                // ...
            }

        \endcode

        Optionally, you can also pass any number of arbitrary arguments
        to your PipelineConfig subclass. Put them after two mandatory
        process and device arguments.

        Example:

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
            public:
                MyPipelineConfig (
                    const vpp::Process& pr,
                    const vpp::Device& dev,
                    const vpp::Display& outDisplay,
                    unsigned int flags );

                // ...
            };

            MyPipelineConfig :: MyPipelineConfig (
                const vpp::Process& pr,
                const vpp::Device& dev,
                const vpp::Display& outDisplay,
                unsigned int flags )
                    vpp::PipelineConfig ( pr ),
                    // ...
            {
                // do something with outDisplay and flags
            }

            class MyRenderer
            {
                // ...
                vpp::PipelineLayout< MyPipelineConfig > m_pipelineLayout;

                // ...
            };

            // in constructor of the renderer class
            MyRenderer ( unsigned int myFlags ) :
                // ...
                m_pipelineLayout (
                    m_renderGraph.m_process,
                    m_device,
                    m_renderGraph.m_outDisplay,
                    myFlags ),
                // ...
            {
                // ...
            }

        \endcode
    */

    PipelineConfig ( const Process& boundProcess );

    /**
        \brief Sets blending mode for specified color attachment.

        The first argument should be a reference to output color attachment
        node of the render graph.

        The second argument is a structure describing the configuration of
        blending units on the GPU for that attachment. See chapter 26.1 in
        the official Vulkan specification for details.
    */

    void setBlendingMode (
        const BaseAttachment& dataNode,
        const VkPipelineColorBlendAttachmentState& blendConfig );

    /**
        \brief Sets logical operation mode for the rasterizer.

        Call only from your pipeline class constructor. This function
        configures logical operation mode for the rasterizer hardware.

        The first pareametrer enables or disables logical operation
        mode. When disabled, the rasterizer will simply write color value
        to destination. When enabled, the rasterizer will perform the
        operation selected by the second argument:

        - VK_LOGIC_OP_CLEAR
        - VK_LOGIC_OP_AND
        - VK_LOGIC_OP_AND_REVERSE
        - VK_LOGIC_OP_COPY
        - VK_LOGIC_OP_AND_INVERTED
        - VK_LOGIC_OP_NO_OP
        - VK_LOGIC_OP_XOR
        - VK_LOGIC_OP_OR
        - VK_LOGIC_OP_NOR
        - VK_LOGIC_OP_EQUIVALENT
        - VK_LOGIC_OP_INVERT
        - VK_LOGIC_OP_OR_REVERSE
        - VK_LOGIC_OP_COPY_INVERTED
        - VK_LOGIC_OP_OR_INVERTED
        - VK_LOGIC_OP_NAND
        - VK_LOGIC_OP_SET

        For detailed description of these operations, see chapter 26.2 of Vulkan
        official specification.
    */
    void enableLogicOperation ( bool bEnable, VkLogicOp logicOp );

    /**
        \brief Specifies what kind of geometric primitives are supplied to the pipeline as input.

        The parameter can be one of the following:

        - POINT_LIST
        - LINE_LIST
        - LINE_STRIP
        - TRIANGLE_LIST
        - TRIANGLE_STRIP
        - TRIANGLE_FAN
        - LINE_LIST_ADJ
        - LINE_STRIP_ADJ
        - TRIANGLE_LIST_ADJ
        - TRIANGLE_STRIP_ADJ
        - PATCH_LIST

        These values are convenience aliases for Vulkan-defined enumeration.
        See chapter 19.1 of Vulkan official specification for description of
        each topology.
    */
    void setPrimitiveTopology ( VkPrimitiveTopology v );

    /**
        \brief Enables primitive restart feature for indexed draws.

        This mode allows to use special index value equal to 0xFFFFFFFF in order
        to start a new series of primitives in certain topologies, like line
        or triangle strips, triangle fans. It is not applicable to list topologies.

        See chapter 19 of the official Vulkan specification for more details.
    */

    void setEnablePrimitiveRestart ( bool v );

    void setTessPatchControlPoints ( std::uint32_t v );

public:
    enum
    {
        POINT_LIST = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        LINE_LIST = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
        LINE_STRIP = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        TRIANGLE_LIST = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        TRIANGLE_STRIP = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        TRIANGLE_FAN = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
        LINE_LIST_ADJ = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
        LINE_STRIP_ADJ = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
        TRIANGLE_LIST_ADJ = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
        TRIANGLE_STRIP_ADJ = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
        PATCH_LIST = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
    };

    /**
        \brief Retrieves selected primitive topology.
    */
    VkPrimitiveTopology getPrimitiveTopology() const;

    /**
        \brief Checks if the primitive restart feature is enabled.
    */
    bool getEnablePrimitiveRestart() const;

    std::uint32_t getTessPatchControlPoints() const;

    /**
        \brief Retrieves a reference to the parent render graph of this pipeline.
    */

    RenderGraph& getRenderGraph() const;

    /**
        \brief Retrieves an index of associated Process node in the render graph.
    */
    std::uint32_t getProcessIndex() const;

    /**
        \brief Issues a command which binds some data buffer containing vertex information
               to a binding point declared within the pipeline.

        The buffer will be bound at command execution time. This method accepts
        assignment list, in the following form:

        ( bindingPoint_1 = buffer_1,
          bindingPoint_2 = buffer_2,
          ...
          bindingPoint_n = buffer_n )

        Where:
        - bindingPoint_i is a const reference to a binding point of type
          inVertexData< ... > declared within the pipeline.
        - buffer_i is a const reference to VertexBufferView object, representing
          the data buffer.

        If you need to bind only one buffer, specify single item list by using
        the following syntax:

        ( bindingPoint_1 = buffer_1 )

        In any case, the list should be surrounded by parentheses. Otherwise
        C++ will not recognize overloaded comma operator and confuse list elements
        with method arguments.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        Example:

        \code

        // define first set of vertex attributes
        template< vpp::ETag TAG >
        struct TVertexAttr1 : public vpp::VertexStruct< TAG, TVertexAttr1 >
        { 
            // ...
        };

        // define second set of vertex attributes
        template< vpp::ETag TAG >
        struct TVertexAttr2 : public vpp::VertexStruct< TAG, TVertexAttr2 >
        { 
            // ...
        };

        class MyPipelineConfig : public vpp::PipelineConfig
        {
        public:
            // ...

            // binds first set of attributes
            void bindVertices (
                const vpp::VertexBufferView& vert )
            {
                // note double parentheses!
                cmdBindVertexInput (( m_verticesAttr1 = vert ));
            }

            // binds both sets of attributes
            void bindVertices (
                const vpp::VertexBufferView& vert1,
                const vpp::VertexBufferView& vert2 )
            {
                // note double parentheses!
                cmdBindVertexInput ((
                    m_verticesAttr1 = vert1,
                    m_verticesAttr2 = vert2
                ));
            }

            // binds both sets of attributes and indices for indexed draw
            void bindVertices (
                const vpp::VertexBufferView& vert1,
                const vpp::VertexBufferView& vert2,
                const vpp::VertexIndexBufferView& ind )
            {
                // note double parentheses!
                cmdBindVertexInput ((
                    m_verticesAttr1 = vert1,
                    m_verticesAttr2 = vert2
                ));

                // single parentheses in this case
                cmdBindIndexInput ( ind );
            }

        private:
            vpp::inVertexData< TVertexAttr1 > m_verticesAttr1;
            vpp::inVertexData< TVertexAttr2 > m_verticesAttr2;
        };

        // later use as follows:

        MyRenderer :: MyRenderer()
        {
            // ...

            // This defines a script of rendering commands for specified
            // process within the render graph.
            m_renderGraph.m_render << [ this ]()
            {
                // This rendering script is recorded into default command buffer
                // maintained by VPP.

                // Retrieve reference to your PipelineConfig subclass and call
                // the bindVertices method.
                m_pipelineLayout.definition().bindVertices (
                    m_vertexBuffer11, m_vertexBuffer12 ); 

                // Draw, using current vertex data.
                m_renderGraph.cmdDraw ( ... );

                // Bind a different set of vertex data.
                m_pipelineLayout.definition().bindVertices (
                    m_vertexBuffer21, m_vertexBuffer22 ); 

                // Draw, using these new sets of vertex data.
                m_renderGraph.cmdDraw ( ... );

                // Bind a different set of vertex data and indices.
                m_pipelineLayout.definition().bindVertices (
                    m_vertexBuffer21, m_vertexBuffer22, m_indexBuffer ); 

                // Draw, using these new sets of vertex data.
                m_renderGraph.cmdDrawIndexed ( ... );
            };
        };

        \endcode
    */

    template< class AssignmentListT >
    void cmdBindVertexInput (
        const AssignmentListT& list,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Issues a command which binds some data buffer
        containing indices for indexed draw to the pipeline.

        Indices do not require declaring a binding point, because there is
        only one index buffer per pipeline and the format is always the same.

        Index buffers are provided by using VertexIndexBufferView wrapper object.

        The command will be generated into specified command buffer, or the
        default command buffer if omitted.

        For example, see the cmdBindVertexInput() documentation.
    */

    void cmdBindIndexInput (
        const VertexIndexBufferView& hVertexIndexBufferView,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------

/**
    \brief Base class for custom compute pipelines.

    Compute pipelines are special kind of pipelines, designed for GPU computation.
    The main difference from regular pipelines is that compute pipelines
    are not associated with render graph. Another difference is that the only
    shader type allowed in compute pipelines is computeShader. Otherwise they
    are similar and can accept the same binding point types as regular pipelines.

    You define your own computation pipeline classes in same way as rendering
    pipelines, by deriving from ComputePipelineConfig. You do not need to pass
    any argument to the constructor, though.

    Command and functions specific to graphics inherited from PipelineConfig
    are not applicable to compute pipelines, so avoid calling them.

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
                const vpp::StorageBufferView& dataOutput )
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

    \endcode
*/

class ComputePipelineConfig : public PipelineConfig
{
public:
    ComputePipelineConfig();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Resource binding helper for binding items or ranges of resource arrays.
*/

template< class ValueT >
inline TUpdateMultipleDescriptors< ValueT > multi (
    const ValueT& value, std::uint32_t startIdx = 0 );

//template< class ValueT >
//inline TUpdateMultipleDescriptors< ValueT > multi (
//    const std::vector< ValueT >& values, std::uint32_t startIdx = 0 );
//
//template< class ValueT >
//inline TUpdateMultipleDescriptors< ValueT > multi (
//    const std::initializer_list< ValueT >& values, std::uint32_t startIdx = 0 );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
