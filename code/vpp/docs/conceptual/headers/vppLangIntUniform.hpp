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
    \brief Binding point class for uniform (read-only) data input to shaders.
           Place in your pipeline configuration class to declare a uniform data source.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig or ComputePipelineConfig subclass).

    There are the following steps to consider when using uniform buffers within pipeline config:
    - Definition of data structure to be stored within the buffer. Either use UniformStruct
      template for that, or assume the buffer is an array of simple objects: scalars,
      vectors or matrices. In second case, separate definition is not needed.
    - Declaration of binding point inside PipelineConfig (or ComputePipelineConfig)
      derived class. It can be private member.
    - Supplying actual data buffer on CPU side. For that end, write a helper method in
      your derved class to bind supplied buffers to declared binding points. the method
      should accept a UniformBufferView reference for each buffer. It should also
      get a pointer to ShaderDataBlock, because at this stage the bindings are
      only remembered inside ShaderDataBlock. The block is later selected as a whole
      into rendering pipeline and subsequent draw commands will act on supplied data
      buffers.
    - Reading the data in a shader on the GPU side. All shader types can read uniform
      buffers. This is being done by means of accessor object declared within the shader.
      Declare UniformVar object for a buffer (or multiple buffers in arrayOf)
      containing single structure. Use UniformArray instead for buffers containing
      multiple structures. Finally, use UniformSimpleArray for buffers containing
      multiple simple objects. To read particular data field or value, use
      indexing operators provided by the accessor object.

    Example:

    \code
        // define data structure for the buffer
        template< ETag TAG >
        struct TMyBufferStructure : public UniformStruct< TAG, TMyBufferStructure >
        {
            UniformFld< TAG, glm::mat4 > m_matrixField;
            UniformFld< TAG, glm::vec4 > m_vectorField;
            // ...
        };

        // it is convenient to make these typedefs
        typedef TMyBufferStructure< vpp::CPU > CMyBufferStructure;
        typedef TMyBufferStructure< vpp::GPU > GMyBufferStructure;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // defines the binding point - assume it contains TMyBufferStructure entries
            vpp::inUniformBuffer m_inUniformBuffer;

            // another binding point - assume it contains simple array of floats
            vpp::inUniformBuffer m_inUniformBuffer2;

            // This binds actual buffers to binding points.
            // The binding is stored within ShaderDataBlock object.
            // Later, the ShaderDataBlock must be selected into the rendering pipeline
            // by calling its bind() function from your rendering (Process) sequence.
            // As arguments to the function, you can directly provide \ref gvector
            // references, as UniformBufferView has a constructor accepting \ref gvector
            // objects.

            void bindDataBuffers (
                const vpp::UniformBufferView& buf1,
                const vpp::UniformBufferView& buf2,
                vpp::ShaderDataBlock* pDataBlock )
            {
                pDataBlock->update ((
                    m_inUniformBuffer = buf1,
                    m_inUniformBuffer2 = buf2
                ));
            }

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                // Accessing a buffer containing single structure.

                UniformVar< TMyBufferStructure, decltype ( m_inUniformBuffer ) >
                    varUniformBuffer ( m_inUniformBuffer );

                Mat4 m1 = varUniformBuffer [ & GMyBufferStructure::m_matrixField ];

                // Accessing a buffer containing array of structures.

                UniformArray< TMyBufferStructure, decltype ( m_inUniformBuffer ) >
                    varUniformBufferArr ( m_inUniformBuffer );

                Int arrayIdx = ...; // compute array index

                Mat4 m2 = varUniformBufferArr [ arrayIdx ][ & GMyBufferStructure::m_matrixField ];

                // Accessing a buffer containing array of scalars.

                UniformSimpleArray< float, decltype ( m_inUniformBuffer2 ) >
                    varUniformBufferSimpleArr ( m_inUniformBuffer2 );

                Int arrayIdx = ...; // compute array index

                Float fv = varUniformBufferSimpleArr [ arrayIdx ];
            }
        };
    \endcode

*/

class inUniformBuffer
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inUniformBuffer ( std::uint32_t set = 0, int binding = -1 );

    /**
        \brief Binds a buffer to the binding point.

        Accepts single argument which is generic buffer abstraction represented by
        UniformBufferView object. Usually just use \ref gvector object in that place
        (it will be automatically cast to UniformBufferView). Alternatively,
        any Vulkan uniform buffer bound to memory may be used.

        Calling this operator does not bind the buffer immediately. It only
        generates an entry inside ShaderDataBlock. The operator returns an opaque
        value that must be supplied to ShaderDataBlock::update method. Several buffers
        can be bound at once. The binding will actually occur when the ShaderDataBlock
        is selected in the rendering pipeline.

        Example:

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
                vpp::inUniformBuffer m_inUniformBuffer;
                vpp::inUniformBuffer m_inUniformBuffer2;

                void bindDataBuffers (
                    const vpp::UniformBufferView& buf1,
                    const vpp::UniformBufferView& buf2,
                    vpp::ShaderDataBlock* pDataBlock )
                {
                    // Note that multiple bindings may occur simultaneously.
                    // Also note that we use double parentheses, because this is
                    // a list constructed with overloaded comma operator, not multiple
                    // arguments.
                    pDataBlock->update ((
                        m_inUniformBuffer = buf1,
                        m_inUniformBuffer2 = buf2
                    ));
                }
            };

            // later, when defining a render graph (irrelevant details not shown):
            void initializeRenderGraph ( ... )
            {
                // Here fill the data block.
                m_myPipelineConfig.bindDataBuffers ( myBuffer0, myBuffer1, & m_dataBlock );

                // m_render is a Process node
                m_renderGraph.m_render << [ this ]()
                {
                    // this lambda routine defines a rendering sequence (render pass)

                    // select current pipeline
                    m_renderPass.pipeline ( 0, 0 ).cmdBind();

                    // bind the data block
                    m_dataBlock.cmdBind();

                    m_renderGraph.cmdDraw ( ... );

                    // You can select a different block (or pipeline) now and draw again.
                };
            }

        \endcode

    */
    auto operator= ( const UniformBufferView& value );
};

// -----------------------------------------------------------------------------

/**
    \brief Binding point class for storage (read-write) data buffer to shaders.
           Place in your pipeline configuration class to declare a storage data buffer.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig or ComputePipelineConfig subclass).

    There are the following steps to consider when using storage buffers within pipeline config:
    - Definition of data structure to be stored within the buffer. Either use UniformStruct
      template for that, or assume the buffer is an array of simple objects: scalars,
      vectors or matrices. In second case, separate definition is not needed.
    - Declaration of binding point inside PipelineConfig (or ComputePipelineConfig)
      derived class. It can be private member.
    - Supplying actual data buffer on CPU side. For that end, write a helper method in
      your derved class to bind supplied buffers to declared binding points. the method
      should accept a UniformBufferView reference for each buffer. It should also
      get a pointer to ShaderDataBlock, because at this stage the bindings are
      only remembered inside ShaderDataBlock. The block is later selected as a whole
      into rendering pipeline and subsequent draw commands will act on supplied data
      buffers.
    - Reading the data in a shader on the GPU side. All shader types can read storage
      buffers. This is being done by means of accessor object declared within the shader.
      Declare UniformVar object for a buffer (or multiple buffers in arrayOf)
      containing single structure. Use UniformArray instead for buffers containing
      multiple structures. Finally, use UniformSimpleArray for buffers containing
      multiple simple objects. To read particular data field or value, use
      indexing operators provided by the accessor object.
    - Writing the data in a shader. All shader types can also write to storage
      buffers. Use the same indexing operators as for reading, but on the left
      side of the assignment (as in usual C++ syntax).
    - Ensuring that generated buffer contents is ready when next rendering or computing
      stage needs it. Call cmdBufferPipelineBarrier() inside your rendering sequence
      to generate a command which guarantees that.

    Example:

    \code
        // define data structure for the buffer
        template< ETag TAG >
        struct TMyBufferStructure : public UniformStruct< TAG, TMyBufferStructure >
        {
            UniformFld< TAG, glm::mat4 > m_matrixField;
            UniformFld< TAG, glm::vec4 > m_vectorField;
            // ...
        };

        // it is convenient to make these typedefs
        typedef TMyBufferStructure< vpp::CPU > CMyBufferStructure;
        typedef TMyBufferStructure< vpp::GPU > GMyBufferStructure;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // defines the binding point - assume it contains TMyBufferStructure entries
            vpp::ioBuffer m_ioBuffer;

            // another binding point - assume it contains simple array of floats
            vpp::ioBuffer m_ioBuffer2;

            void bindDataBuffers (
                const vpp::UniformBufferView& buf1,
                const vpp::UniformBufferView& buf2,
                vpp::ShaderDataBlock* pDataBlock )
            {
                // Note that multiple bindings may occur simultaneously.
                // Also note that we use double parentheses, because this is
                // a list constructed with overloaded comma operator, not multiple
                // arguments.
                pDataBlock->update ((
                    m_ioBuffer = buf1,
                    m_ioBuffer2 = buf2
                ));
            }

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                // Accessing a buffer containing single structure.

                UniformVar< TMyBufferStructure, decltype ( m_ioBuffer ) >
                    varBuffer ( m_ioBuffer );

                Mat4 m1 = varBuffer [ & GMyBufferStructure::m_matrixField ];

                varBuffer [ & GMyBufferStructure::m_vectorField ] =
                    m1 * Vec4 { 1.0f, 2.0f, 3.0f, 4.0f };

                // Accessing a buffer containing array of structures.

                UniformArray< TMyBufferStructure, decltype ( m_ioBuffer ) >
                    varBufferArr ( m_ioBuffer );

                Int arrayIdx = ...; // compute array index

                Mat4 m2 = varBufferArr [ arrayIdx ][ & GMyBufferStructure::m_matrixField ];

                varBufferArr [ arrayIdx ][ & GMyBufferStructure::m_vectorField ] =
                    m2 * Vec4 { 1.0f, 2.0f, 3.0f, 4.0f };

                // Accessing a buffer containing array of scalars.

                UniformSimpleArray< float, decltype ( m_ioBuffer2 ) >
                    varBufferSimpleArr ( m_ioBuffer2 );

                Int arrayIdx = ...; // compute array index

                Float fv = varBufferSimpleArr [ arrayIdx ];
                varBufferSimpleArr [ arrayIdx ] = fv * 2.0f;
            }
        };

    \endcode
*/

class ioBuffer
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    ioBuffer ( std::uint32_t set = 0, int binding = -1 );

    /**
        \brief Binds a buffer to the binding point.

        Accepts single argument which is generic buffer abstraction represented by
        UniformBufferView object. Usually just use \ref gvector object in that place
        (it will be automatically cast to UniformBufferView). Alternatively,
        any Vulkan read-write uniform buffer bound to memory may be used.

        Calling this operator does not bind the buffer immediately. It only
        generates an entry inside ShaderDataBlock. The operator returns an opaque
        value that must be supplied to ShaderDataBlock::update method. Several buffers
        can be bound at once. The binding will actually occur when the ShaderDataBlock
        is selected in the rendering pipeline.

        Example:

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
                vpp::ioBuffer m_ioBuffer;
                vpp::ioBuffer m_ioBuffer2;

                void bindDataBuffers (
                    const vpp::UniformBufferView& buf1,
                    const vpp::UniformBufferView& buf2,
                    vpp::ShaderDataBlock* pDataBlock )
                {
                    // Note that multiple bindings may occur simultaneously.
                    // Also note that we use double parentheses, because this is
                    // a list constructed with overloaded comma operator, not multiple
                    // arguments.
                    pDataBlock->update ((
                        m_ioBuffer = buf1,
                        m_ioBuffer2 = buf2
                    ));
                }
            };

            // later, when defining a render graph (irrelevant details not shown):
            void initializeRenderGraph ( ... )
            {
                // Here fill the data block.
                m_myPipelineConfig.bindDataBuffers ( myBuffer0, myBuffer1, & m_dataBlock );

                // m_render is a Process node
                m_renderGraph.m_render << [ this ]()
                {
                    // this lambda routine defines a rendering sequence (render pass)

                    // select current pipeline
                    m_renderPass.pipeline ( 0, 0 ).cmdBind();

                    // bind the data block
                    m_dataBlock.cmdBind();

                    m_renderGraph.cmdDraw ( ... );

                    // synchronize buffers with recipient

                    cmdBufferPipelineBarrier (
                        m_ioBuffer,
                        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,    // which shader writes
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,  // which shader reads
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT
                    );

                    cmdBufferPipelineBarrier (
                        m_ioBuffer2,
                        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT
                    );

                    // You can select a different block (or pipeline) now and draw again.
                };
            }

        \endcode
    */

    auto operator= ( const UniformBufferView& value );
};

// -----------------------------------------------------------------------------

/**
    \brief Currently not implemented.
*/

class inUniformBufferDyn
{
public:
    inUniformBufferDyn ( std::uint32_t set = 0, int binding = -1 );
    auto operator= ( const UniformBufferView& value );
};

// -----------------------------------------------------------------------------

/**
    \brief Currently not implemented.
*/

class ioBufferDyn
{
public:
    ioBufferDyn ( std::uint32_t set = 0, int binding = -1 );
    auto operator= ( const UniformBufferView& value );
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for push constant data input to shaders.
           Place in your pipeline configuration class to declare a push constant.

    Specify the name of your uniform data structure template
    as the argument. Accepts templates derived from UniformStruct.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig or ComputePipelineConfig subclass).

    Push constants in VPP are used in pretty much the same way as uniform buffers,
    with some differences. There are the following steps to consider when using push
    constants within pipeline config:
    - Definition of data structure to be stored within the constant. Use UniformStruct
      template for that. Beware that push constant size is limited and it should be
      very small structure.
    - Declaration of binding point inside PipelineConfig (or ComputePipelineConfig)
      derived class. It can be private member.
    - Supplying actual data on CPU side. Here the important thing is that push constants
      bypass ShaderDataBlock completely and you do not need to bind any buffers. Instead,
      the binding point contains its own small data buffer. You can obtrain access to this
      buffer by calling data() method on the binding point. Write the fields directly.
    - Synchronizing CPU side buffer to GPU. This is done by means of a command generated
      to a command buffer. Easiest way is to use one of the overloads of cmdPush() method
      within your rendering sequence. It's exactly in the same place in which you would
      select ShaderDataBlock into the pipeline - now it sends the data instead.
    - Reading the data in a shader on the GPU side. All shader types can read push
      constants. This is being done by means of accessor object declared within the shader.
      Declare UniformVar object, just the same as for uniform buffers.

    Example:

    \code
        // define data structure for the buffer
        template< ETag TAG >
        struct TMyBufferStructure : public UniformStruct< TAG, TMyBufferStructure >
        {
            UniformFld< TAG, glm::mat4 > m_matrixField;
            UniformFld< TAG, glm::vec4 > m_vectorField;
            // ...
        };

        // it is convenient to make these typedefs
        typedef TMyBufferStructure< vpp::CPU > CMyBufferStructure;
        typedef TMyBufferStructure< vpp::GPU > GMyBufferStructure;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // defines the binding point - assume it contains TMyBufferStructure entry
            vpp::inPushConstant< TMyBufferStructure > m_inPushConstant;

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                // Accessing a buffer containing single structure.

                UniformVar< TMyBufferStructure, decltype ( m_inPushConstant ) >
                    varBuffer ( m_inPushConstant );

                Mat4 m1 = varBuffer [ & GMyBufferStructure::m_matrixField ];
            }
        };

        // [jeszcze example na cmdPush]

    \endcode
*/

template< template< vpp::ETag TAG > class DefinitionT >
class inPushConstant
{
public:
    /**
        \brief Constructor. Does not take any arguments.
    */
    inPushConstant();

    /**
        \brief Emits push command to specified command buffer.

        The command will be executed when the command buffer is executed by the queue.
        It will transfer entire push constant structure to the GPU.
    */
    void cmdPush ( CommandBuffer currentBuffer );

    /**
        \brief Emits push command to specified command buffer.

        The command will be executed when the command buffer is executed by the queue.
        It will transfer only specified field contents to the GPU. As the field
        reference, supply a reference to a field inside CPU version of the
        data structure.
    */
    template< typename ValueT >
    void cmdPush ( CommandBuffer currentBuffer, const ValueT& field );

    /**
        \brief Emits push command to default command buffer.

        The command will be executed when the command buffer is executed by the queue.
        It will transfer entire push constant structure to the GPU. This overload
        is meant to be used inside rendering command sequence (Process, Preprocess
        or Postprocess).
    */
    void cmdPush();

    /**
        \brief Emits push command to default command buffer.

        The command will be executed when the command buffer is executed by the queue.
        It will transfer only specified field contents to the GPU. As the field
        reference, supply a reference to a field inside CPU version of the
        data structure.

        This overload is meant to be used inside rendering command sequence
        (Process, Preprocess or Postprocess).
    */
    template< typename ValueT >
    void cmdPush ( const ValueT& field );

    /**
        \brief A typedef to CPU version of the data structure.
    */

    typedef DefinitionT< CPU > DataBlock;

    /**
        \brief Allows to access the fields of the data structure.

        Fields are being written directly. However, they are synchronized
        (sent to GPU) at the moment when cmdPush() command is being executed
        by the queue.

        It is a common practice to issue cmdPush() inside rendering command sequence
        (Process, Preprocess or Postprocess). However, do NOT write push constant
        fields there. Fields should be written before issuing the command buffer
        to the queue -- not when constructing the buffer what the command sequence
        actually does.
    */
    DataBlock& data();
};

// -----------------------------------------------------------------------------
/**
    \brief Base class for uniform data structures.

    Inherit from this class to define your uniform data structure template.
    The first template argument should be passed from your template.
    The second one should be the name of your template which is being
    derived.
    
    Use UniformFld template to define fields inside the structure.

    Instantiating the template with GPU tag argument gives GPU-side version
    of the structure, while CPU tag gives the host version. The CPU version
    can be directly used to fill data buffers meant to be transfered to GPU.

    Unlike VertexStruct and InstanceStruct, CPU flavor of UniformStruct does
    not use Vulkan format to specify the data layout. You provide directly
    the data type. This is because Vulkan does not need to interpret uniform
    data as thoroughly as in case of vertex and instance streams.

    Example of type definition:

    \code
        template< ETag TAG >
        struct TFramePar : public UniformStruct< TAG, TFramePar >
        {
            UniformFld< TAG, glm::mat4 > m_world2projected;
            UniformFld< TAG, glm::mat4 > m_world2view;
            UniformFld< TAG, glm::mat4 > m_view2projected;
            UniformFld< TAG, glm::mat4 > m_projected2vport;
            UniformFld< TAG, glm::mat4 > m_projected2world;
            UniformFld< TAG, glm::mat4 > m_vport2world;
            UniformFld< TAG, glm::mat4 > m_viewPosition;
        };

        typedef TFramePar< GPU > GFramePar;
        typedef TFramePar< CPU > CFramePar;

        // use this vector as a container and transport for the data
        typedef gvector< CFramePar, Buf::UNIFORM > CFrameParBuffer;
    \endcode

    The \c glm::mat4 type in the code above is an example of
    custom matrix data type. GLM types are supported by VPP. You can add your
    own types support by defining the StructMemberTraits template specializations.

    Using the CPU-side flavor to create and fill the vector:

    \code
        CFrameParBuffer frameParBuf ( 2, MemProfile::DEVICE_STATIC, hDevice );

        frameParBuf [ 0 ].m_world2projected = glm::mat4 ( ... );
        frameParBuf [ 0 ].m_world2view = glm::mat4 ( ... );
        // ...

    \endcode

    Example of CPU-side data binding for the defined type:

    \code
        class MyPipeline : public PipelineConfig
        {
        public:
            void bindBuffers (
                ShaderDataBlock* pDataBlock,
                const CFrameParBuffer& frameParBuf )
            {
                pDataBlock->update ( (
                    m_frameParBindingPoint = UniformBufferView ( frameParBuf )
                ) );
            }

        private:
            inUniformBuffer m_frameParBindingPoint;
        };

    \endcode

    Inside shader code, use UniformVar or UniformArray to access the fields.
    The GPU-level type to represent the value is inferred from CPU-level type
    with help of StructMemberTraits specialization. In this case, VPP knows
    that GPU counterpart of \c glm::mat4 is Mat4.

    Example:

    \code
        // ....

        UniformArray< TFramePar, inUniformBuffer > framePar ( m_frameParBindingPoint );

        // Note that you use GFramePar member pointer here, as this is GPU code.
        const Mat4 w2p = framePar [ 0 ][ & GFramePar::m_world2projected ];
    \endcode
*/

template< ETag TAG, template< vpp::ETag TAG > class TDef >
class UniformStruct
{
};

// -----------------------------------------------------------------------------

/**
    \brief Template for defining uniform data fields.

    Place UniformFld inside UniformStruct to define a data field.
    This field can have scalar, vector or matrix type. You must specify one
    type in the template argument list: the host type (CPU-side). The shader
    (GPU-side) type is automatically inferred.

    Example:

    \code
        template< ETag TAG >
        struct TFramePar : public UniformStruct< TAG, TFramePar >
        {
            // This defines a field which will be represented by glm::mat4
            // matrix type on CPU side and vpp::Mat4 on GPU side. The second one
            // is automatically determined from vpp::StructMemberTraits specialization.
            UniformFld< TAG, glm::mat4 > m_world2projected;

            UniformFld< TAG, glm::mat4 > m_world2view;
            // ...
        };
 
    \endcode

    The first argument should be the tag passed from UniformStruct.

    The second argument describes the CPU type. That allows to create
    CPU version of the structure that is compatible with GPU version.
    This argument can be one of the following:
    - Scalar type (int, float, etc.).
    - An instantiation of vpp::format< ... > template specifying
      some structure consisting of several fields. VPP treats these
      specially, so that in CPU version of the structure you get access
      to \c data_type defined with the format.
    - GLM vector or matrix type. You must include \c vppSupportGLM.hpp
      for that to work.
    - Any custom vector or matrix type that has corresponding
      specialization of vpp::StructMemberTraits template. You can define
      your own specializations in order to support your types.

    [ caution! document also alignment issues with uniform buffers! ]
*/

template< ETag TAG, class HostT, int OFFSET = -1 >
struct UniformFld
{
    /** \brief Constructs uninitialized or GPU-side data field. */
    UniformFld();

    /** \brief Constructs initialized CPU-side data field. */
    UniformFld ( const HostT& value );

    /** \brief Allows CPU-side access to the field. */
    operator HostT& ();

    /** \brief Allows CPU-side read-only access to the field. */
    operator const HostT& () const;
};

// -----------------------------------------------------------------------------
/**
    \brief Provides access to scalar or arrayed buffer binding point from shader code.

    Place UniformVar inside your shader code to access the data in uniform,
    storage or push constant buffer.

    The first argument should be the structure template (derived from UniformStruct).

    The second argument should be the \c decltype of accessed binding point.

    It accepts the following binding points: inUniformBuffer, ioBuffer,
    inUniformBufferDyn, ioBufferDyn, inPushConstant and arrayOf based on those.
    
    Also specify the binding point in the constructor of UniformVar. The binding point should
    be an object inside PipelineConfig subclass and the shader should be a method
    in the same subclass - or some code called from it.

    UniformVar gives access to single structure in single or arrayed buffer.
    To access an array embedded inside single buffer, see UniformArray and UniformSimpleArray.

    Example of single buffer:

    \code
        template< ETag TAG >
        struct TFramePar : public UniformStruct< TAG, TFramePar >
        {
            UniformFld< TAG, glm::mat4 > m_world2projected;
            UniformFld< TAG, glm::mat4 > m_world2view;
            // ...
        };

        typedef TFramePar< vpp::CPU > CFramePar;
        typedef TFramePar< vpp::GPU > GFramePar;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::inUniformBuffer m_inUniformBuffer;

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                UniformVar< TFramePar, decltype ( m_inUniformBuffer ) >
                    varUniformBuffer ( m_inUniformBuffer );
                    
                Mat4 w2p = varUniformBuffer [ & GFramePar::m_world2projected ];

                // ...
            }
        };

    \endcode

    Example of arrayed buffer:

    \code
        template< ETag TAG >
        struct TFramePar : public UniformStruct< TAG, TFramePar >
        {
            UniformFld< TAG, glm::mat4 > m_world2projected;
            UniformFld< TAG, glm::mat4 > m_world2view;
            // ...
        };

        typedef TFramePar< vpp::CPU > CFramePar;
        typedef TFramePar< vpp::GPU > GFramePar;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::arrayOf< vpp::inUniformBuffer > m_inUniformBufferArr;

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                UniformVar< TFramePar, decltype ( m_inUniformBufferArr ) >
                    varUniformBufferArr ( m_inUniformBufferArr );
                    
                Int arrayIndex = ...; // compute array index

                Mat4 w2p = varUniformBufferArr [ arrayIndex ][ & GFramePar::m_world2projected ];

                // ...
            }
        };

    \endcode
*/

template< template< vpp::ETag TAG > class TDef, class BufferT >
class UniformVar
{
public:
    /** \brief Constructs the accessor for provided binding point. */
    UniformVar ( BufferT& buf );

    /** \brief Provides read and write access to specified field. Single buffers only.
    
        Some buffers are read-only. It is an error to attempt to write them.
    */
    template< typename MemberT >
    auto operator[]( MemberT Definition::* pMember ) const;

    /** \brief Provides read and write access to specified item. Arrayed buffers only.
    
        Some buffers are read-only. It is an error to attempt to write them.
    */
    auto operator[]( const Int& index ) const;

    /** \overload */
    auto operator[]( const UInt& index ) const;

    /** \overload */
    auto operator[]( int index ) const;

    /** \overload */
    auto operator[]( unsigned int index ) const;

    /** \brief Returns GPU-side value equal to the size of the array. Arrayed buffers only. */
    Int Size() const;

    /** \brief Returns CPU-side value equal to the size of the array. Arrayed buffers only. */
    int size() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Provides shader code access to a binding point for buffer holding
           array of structures.

    Place UniformArray inside your shader code to access the data in uniform,
    storage or push constant buffer.

    The first argument should be the structure template (derived from UniformStruct).

    The second argument should be the decltype of accessed binding point. 
    It accepts the following binding points: inUniformBuffer, ioBuffer,
    inUniformBufferDyn, ioBufferDyn, inPushConstant. Also specify
    the binding point in the constructor of UniformArray. The binding point should
    be an object inside PipelineConfig subclass and the shader should be a method
    in the same subclass - or some code called from it.

    UniformArray gives access to multiple structures packed in single,
    non-arrayed buffer. For other variants, see arrayOf, UniformVar
    and UniformSimpleArray.

    This should not be confused with arrayed buffers (arrayOf). Array of buffers
    consists of multiple buffers presented to the shader as an array. The UniformArray
    accessor operates on single buffer, but containing multiple data entries.
*/

template< template< vpp::ETag TAG > class TDef, class BufferT >
class UniformArray
{
public:
    /** \brief Constructs the accessor for given binding point, assuming the buffer
               is holding an array of structures.
               
        The constructor of the accessor creates actual variable (of array type)
        on the SPIR-V level.

        As the first argument, provide reference to the binding point.

        You can optionally provide fixed array size as the second argument.

        If you omit array size, the accessor will generate a runtime array.
        Its size will be determined automatically from the size of bound buffer.
        You can read the dynamic size using the Size() method.

        If you specify array size, the accessor will generate statically sized
        array. Its size will be fixed and can be read either by Size() method
        or size() method (they return GPU and CPU level types respectively).

        Warning: it has been observed in practice that drivers for some GPUs
        (notably NVIDIA) have trouble with large size fixed arrays, which may
        lead to crashes or long hangs during shader code compilation. It is
        recommended to use runtime sized arrays unless the size is very small.

        The third argument, also optional, allows to override array stride.
        It is provided just for completeness, as the accessor infers the stride
        automatically from the structure definition.
    */
    UniformArray ( BufferT& buf, int size = 0, int stride = 0 );

    /** \brief Provides read and write access to specified item.
    
        Returns an opaque type serving as temporary object, meant to be indexed
        again with \c operator[]. The second level \c operator[] accepts a pointer
        to member of GPU version of the data structure. This double indexing
        allows to read or write particular field in particular array item.
        
        Some buffers are read-only. It is an error to attempt to write them.
    */
    auto operator[]( const Int& index ) const;

    /** \overload */
    auto operator[]( const UInt& index ) const;

    /** \overload */
    auto operator[]( int index ) const;

    /** \overload */
    auto operator[]( unsigned int index ) const;

    /** \brief Returns GPU-side value equal to the size of the array.
    
        The size determined by this function may be dynamic. This happens
        if you did not specify explicit size in the constructor. 
    */
    Int Size() const;

    /** \brief Returns CPU-side value equal to the size of the array.
    
        For dynamically sized arrays, returns zero.
    */
    int size() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Provides shader code access to a binding point for buffer holding
           array of simple scalars, vectors or matrices.

    Place UniformSimpleArray inside your shader code to access the data in uniform,
    storage or push constant buffer.

    The first argument should be the CPU type. For scalars
    this is just numeric C++ type, e.g. int, unsigned int, float or double.
    For vectors and matrices, you need to define your own vector or matrix
    class and provide StructMemberTraits template specialization. That permits
    to use your custom types for vectors and matrices. By default, VPP supplies
    such integration for GLM library types.

    Caution: a type allowed to be an element of UniformSimpleArray must not
    be a packed 3-element vector, due to Vulkan alignment constraints
    for uniform buffers. This restriction forbids e.g. \c glm::vec3 vectors
    in that context (use \c glm::vec4 instead). The \ref vpp::vect3 type
    is permitted, because it is aligned to 16 bytes.

    The second argument should be the decltype of accessed binding point. 
    It accepts the following binding points: inUniformBuffer, ioBuffer,
    inUniformBufferDyn, ioBufferDyn, inPushConstant. Also specify
    the binding point in the constructor of UniformSimpleArray. The binding point
    should be an object inside PipelineConfig subclass and the shader should be
    a method in the same subclass - or some code called from it.

    UniformSimpleArray gives access to multiple values (of scalar, vector or matrix)
    packed in single, non-arrayed buffer. For other variants, see arrayOf, UniformVar
    and UniformArray.

    This should not be confused with arrayed buffers (arrayOf). Array of buffers
    consists of multiple buffers presented to the shader as an array. The UniformSimpleArray
    accessor operates on single buffer, but containing multiple data entries.

    The GPU-side type (returned by indexing operators) is automatically inferred
    from the \c HostT type with help of StructMemberTraits template specialization.
    Define your own specialization of StructMemberTraits to use a custom data type
    which is not supported by VPP already.
*/

template< class HostT, class BufferT >
class UniformSimpleArray
{
public:
    /** \brief Constructs accessor for given binding point, assuming the buffer
               is holding an array of simple items.
    
        The constructor of the accessor creates actual variable (of array type)
        on the SPIR-V level.

        As the first argument, provide reference to the binding point.

        You can optionally provide fixed array size as the second argument.

        If you omit array size, the accessor will generate a runtime array.
        Its size will be determined automatically from the size of bound buffer.
        You can read the dynamic size using the Size() method.

        If you specify array size, the accessor will generate statically sized
        array. Its size will be fixed and can be read either by Size() method
        or size() method (they return GPU and CPU level types respectively).

        Warning: it has been observed in practice that drivers for some GPUs
        (notably NVIDIA) have trouble with large size fixed arrays, which may
        lead to crashes or long hangs during shader code compilation. It is
        recommended to use runtime sized arrays unless the size is very small.

        The third argument, also optional, allows to override array stride.
        It is provided just for completeness, as the accessor infers the stride
        automatically from \c HostT and StructMemberTraits.
    */
    UniformSimpleArray ( BufferT& buf, int size = 0, int stride = 0 );

    /** \brief Provides read and write access to specified item.
    
        Returns a type inferred from the host data type, allowing to read or
        write the array.
        
        Some buffers are read-only. It is an error to attempt to write them.
    */
    auto operator[]( const Int& index ) const;

    /** \overload */
    auto operator[]( const UInt& index ) const;

    /** \overload */
    auto operator[]( int index ) const;

    /** \overload */
    auto operator[]( unsigned int index ) const;

    /** \brief Returns GPU-side value equal to the size of the array.
    
        The size determined by this function may be dynamic. This happens
        if you did not specify explicit size in the constructor. 
    */
    Int Size() const;

    /** \brief Returns CPU-side value equal to the size of the array.
    
        For dynamically sized arrays, returns zero.
    */
    int size() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
