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
    \brief Base class for vertex data structures.

    Inherit from this class to define your vertex data structure template.
    The first template argument should be passed from your template.
    The second one should be the name of your template which is being
    derived.
    
    Use Attribute template to define fields inside the structure.

    Structures defined with VertexStruct will be treated in vertex shaders
    as holding the data for single vertex of a mesh.

    Instantiating the template with GPU tag argument gives GPU-side version
    of the structure, while CPU tag gives the host version. The CPU version
    can be directly used to fill data buffers meant to be transfered to GPU.

    CPU flavor uses Vulkan format enumeration internally. Also Attribute
    arguments correspond to vpp::format arguments. This is because Vulkan
    needs to interpret the data in order to construct and rasterize primitives.
    Interpretation of numeric data in Vulkan is based on the format.

    Example of type definition:

    \code

        template< ETag TAG >
        struct TVertexAttr : public VertexStruct< TAG, TVertexAttr >
        {
            Attribute< TAG, float, float, float, float > m_position;
            Attribute< TAG, float16_t, float16_t, float16_t, float16_t > m_normal;
            Attribute< TAG, float16_t, float16_t, float16_t, float16_t > m_tangent;
            Attribute< TAG, float16_t, float16_t, float16_t, float16_t > m_texture01;
            Attribute< TAG, float16_t, float16_t, float16_t, float16_t > m_texture23;
        };

        typedef TVertexAttr< CPU > CVertexAttr;
        typedef TVertexAttr< GPU > GVertexAttr;
        typedef gvector< CVertexAttr, Buf::VERTEX > CVertexAttrBuffer;

    \endcode

*/

template< ETag TAG, template< vpp::ETag TAG > class TDef >
struct VertexStruct
{
};

// -----------------------------------------------------------------------------

/**
    \brief Base class for instance data structures.

    Inherit from this class to define your instance data structure template.
    The first template argument should be passed from your template.
    The second one should be the name of your template which is being
    derived from InstanceStruct.
    
    Use Attribute template to define fields inside the structure.

    Structures defined with InstanceStruct will be treated in vertex shaders
    as holding the data for single instance when performing instanced draw calls.
    You can pass the data common for entire mesh instance here, e.g. a model-world
    transformation matrix.

    Instantiating the template with GPU tag argument gives GPU-side version
    of the structure, while CPU tag gives the host version. The CPU version
    can be directly used to fill data buffers meant to be transfered to GPU.

    Example of type definition:
    
    \code
        template< ETag TAG >
        struct TInstancePar : public InstanceStruct< TAG, TInstancePar >
        {
            Attribute< TAG, ext< glm::mat4 > > m_model2world;
            Attribute< TAG, ext< glm::mat4 > > m_model2worldNorm;
            Attribute< TAG, float, float, float, float > m_auxParams;
        };

        typedef TInstancePar< CPU > CInstancePar;
        typedef TInstancePar< GPU > GInstancePar;
        typedef gvector< CInstancePar, Buf::VERTEX > CInstanceBuffer;

    \endcode

*/

template< ETag TAG, template< vpp::ETag TAG > class TDef >
struct InstanceStruct
{
};

// -----------------------------------------------------------------------------

/**
    \brief Template for defining vertex or instance data fields.

    Place Attribute inside VertexStruct or InstanceStruct to define a data field.

    There are two forms of the attribute definition.

    The first one, used mostly for vertex buffers, can have vector type of up to four
    elements. Fields are defined internally by a Vulkan format. Attribute automatically
    determines appropriate format to use for specified component types.

    The first argument should be the tag passed from VertexStruct or InstanceStruct.
    Rest are component types. Specify one to four components of simple C++ types
    or special types acceptable by the vpp::format template.

    Example:

    \code
        template< vpp::ETag TAG = CPU >
        struct TVertexPos : public vpp::VertexStruct< TAG, TVertexPos >
        {
            vpp::Attribute< TAG, float, float, float, float > m_modelPosition;
        };
    \endcode

    The second form accepts \c vpp::ext template instantiation. Its purpose is to
    allow more complex types (e.g. matrices). As the single argument of \c vpp::ext,
    specify the CPU custom type. This type must have a specialization of
    vpp::StructMemberTraits which contains required information about the type.

    VPP defines StructMemberTraits specializations for common GLM matrix and vector
    types. You can define your own ones also.

    Example:

    \code
        template< ETag TAG >
        struct TInstancePar : public InstanceStruct< TAG, TInstancePar >
        {
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2world;
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2worldNorm;
        };
    \endcode
*/

template< ETag TAG, typename C1, typename C2 = no_component, typename C3 = no_component,
          typename C4 = no_component, typename C5 = no_component >
struct Attribute
{
    typedef format< C1, C2, C3, C4, C5 > format_type;
    typedef <implementation_defined> rvalue_type;

    /** \brief Constructor - does not take any arguments. */
    Attribute();

    /** \brief GPU-side data retrieval operator. */
    operator rvalue_type();
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for vertex and instance input to shaders.
           Place in your pipeline configuration class to declare a vertex or instance data source.

    Specify the name of your vertex or instance data structure template
    as the argument. Accepts templates derived both from VertexStruct and
    InstanceStruct.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig subclass).

    Vertex and instance data has exactly the same syntax, except for the
    structure definition (VertexStruct vs InstanceStruct).

    An example:

    \code
        template< vpp::ETag TAG = CPU >
        struct TVertexPos : public vpp::VertexStruct< TAG, TVertexPos >
        {
            vpp::Attribute< TAG, float, float, float, float > m_modelPosition;
        };

        typedef TVertexPos< vpp::CPU > KVertexPos;
        typedef TVertexPos< vpp::GPU > GVertexPos;

        template< ETag TAG >
        struct TInstancePar : public InstanceStruct< TAG, TInstancePar >
        {
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2world;
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2worldNorm;
            vpp::Attribute< TAG, float, float, float, float > m_auxParams;
        };

        typedef TInstancePar< vpp::CPU > CInstancePar;
        typedef TInstancePar< vpp::GPU > GInstancePar;
        typedef vpp::gvector< CInstancePar, Buf::VERTEX > CInstanceBuffer;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...

            // this defines the binding point
            vpp::inVertexData< TVertexPos > m_inVertexData;
            vpp::inVertexData< TInstancePar > m_inInstanceData;

            void bindInstances (
                const vpp::VertexBufferView& inst )
            {
                // Helper method to bind actual instance buffer view to binding point
                // Call this method from your rendering sequence in render graph.

                cmdBindVertexInput (( m_inInstanceData = inst ));
            }

            void bindVertices (
                const vpp::VertexBufferView& vert,
                const vpp::VertexIndexBufferView& ind )
            {
                // Helper method to bind actual vertex buffer views to binding points.
                // Call this method from your rendering sequence in render graph.

                cmdBindVertexInput (( m_inVertexData = vert ));
                cmdBindIndexInput ( ind );
            }

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;
                // ...

                // This vertex shader processes one vertex from m_inVertexData
                // a time. It can access the data as presented below. The data
                // is read-only.

                // read the value of m_modelPosition attribute for current vertex.
                Vec4 vpos = m_inVertexData [ & GVertexPos::m_modelPosition ];

                // The vertex shader also processes one instance a time. Usually
                // a number of vertices is associated with single instance. Therefore
                // the call below will read the same value for some vertices, until
                // they run out and the next instance is loaded.

                // read the value of m_model2world attribute for current instance.
                Mat4 modelMatrix = m_inInstanceData [ & GInstancePar::m_model2world ];
            }
        };

    \endcode
*/

template< template< vpp::ETag TAG > class TDef >
struct inVertexData
{
    /** \brief CPU-side binding operator. Use to bind actual data buffer to the binding point.

        This operator returns a value that must be passed to cmdBindVertexInput method
        of PipelineConfig class. You can also make a list of more assignments,
        joining them with comma operator. The cmdBindVertexInput method accepts
        such a list.

        The cmdBindVertexInput and cmdBindIndexInput are meant to be used mainly
        inside the rendering sequence within render graph (lambda function supplied
        to the Process object). They generate Vulkan commands into default command
        buffer. Actual binding of the vertex and instance data occurs at command execution time.

        The example below also shows how to bind index data for indexed draws. It is
        very similar to vertex data, but indices do not need to have explicitly
        defined binding point. The binding point for indices is implicitly provided
        by each PipelineConfig instance.

        Example:

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
                // ...

                // this defines the binding point
                vpp::inVertexData< TVertexPos > m_inVertexData;

                void bindVertices (
                    const vpp::VertexBufferView& vert,
                    const vpp::VertexIndexBufferView& ind )
                {
                    // Helper method to bind actual vertex buffer views to binding points
                    // call this method from your rendering sequence in render graph.

                    cmdBindVertexInput (( m_inVertexData = vert ));
                    cmdBindIndexInput ( ind );
                }
            };

        \endcode

        Another example for multiple vertex buffers:

        \code
            class MyPipelineConfig : public vpp::PipelineConfig
            {
                // ...
                vpp::inVertexData< TVertexPos > m_inVertexPosData;
                vpp::inVertexData< TVertexAttr > m_inVertexAttrData;

                void bindVertices (
                    const vpp::VertexBufferView& vertPos,
                    const vpp::VertexBufferView& verAttr,
                    const vpp::VertexIndexBufferView& ind )
                {
                    cmdBindVertexInput ((
                        m_inVertexPosData = vertPos,
                        m_inVertexAttrData = vertAttr
                    ));

                    cmdBindIndexInput ( ind );
                }
            };

        \endcode
    */
    auto operator= ( const VertexBufferView& hVertexBufferView );

    /** \brief GPU-side access operator. Use to read a field (Attribute) in the shader.
    
        Supply a member pointer to the attribute (inside GPU level version of
        the structure) which you wish to read.

        This operator returns a R-value of appropriate GPU-side type, corresponding
        to the format of the attribute.

        Example:

        \code
            template< vpp::ETag TAG = CPU >
            struct TVertexPos : public vpp::VertexStruct< TAG, TVertexPos >
            {
                vpp::Attribute< TAG, float, float, float, float > m_modelPosition;
            };

            typedef TVertexPos< vpp::CPU > KVertexPos;
            typedef TVertexPos< vpp::GPU > GVertexPos;

            class MyPipelineConfig : public vpp::PipelineConfig
            {
                // ...
                vpp::inVertexData< TVertexPos > m_inVertexData;

                void fVertexShader ( vpp::VertexShader* pShader )
                {
                    using namespace vpp;
                    // ...

                    // This vertex shader processes one vertex from m_inVertexData
                    // a time. It can access the data as presented below. The data
                    // is read-only.

                    // read the value of m_modelPosition attribute for current vertex
                    Vec4 vpos = m_inVertexData [ & GVertexPos::m_modelPosition ];
                }
            };

        \endcode
    */
    template< typename MemberT >
    auto operator[]( MemberT YourDefinition::* pMember ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
