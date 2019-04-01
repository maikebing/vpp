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
    \brief Base class for shader interfaces.

    This base class provides some utility functions common to all shader
    interfaces.
*/

class Shader
{
public:
    /** \brief Retrieves the device the shader is currently compiled for. */
    const Device& device() const;

    /**
        \brief Enables diagnostic dump of intermediate SPIR-V code for this shader.

        Use anywhere in the shader code to enable the SPIR-V code dump. The code
        is written to currently registered DebugReporter. Also you must
        have called the DebugReporter constructor with DebugReporter::SHADERS
        flag, to globally enable shader compilation output.

        SPIR-V code is useful to diagnose problems in VPP itself. For typical
        operation you will not need this function.
    */

    void DebugCodeDump();    

    /**
        \brief Adds a debug probe to dump an expression value during shader execution.

        Debug probes are useful tool to examine values computed inside shaders.
        It is a counterpart of well-known \c printf method of debugging plain
        C/C++ code in situations when proper debugger is unavailable. The difference
        is that DebugProbe creates a temporary image rather than text dump,
        where the values are being written to. You can view the image in any
        rendering debugger, e.g. RenderDoc.

        The first argument is the value to dump. The type can be either scalar
        (Float, Int, UInt) or 4-element vector of these (Vec4, IVec4, UVec4).
        If you do not need to dump all 4 components, just supply zeros to
        remaining elements.

        The second argument is 2-dimensional coordinate vector (IVec2). It
        specifies where the value will be located in the debug image. The meaning
        depends on your shader. If it is a shader operating on some image, natural
        choice are the coordinates within the image. But this is completely
        arbitrary, the interpretation of these coords is up to you.

        The third argument is the size of the debug image. Here specify maximum
        expected coordinate values, as the VkExtent3D type.

        Example:

        \code
            pShader->DebugProbe (
                val, IVec2 ( currentX, currentY ),
                VkExtent3D { width, height } );
        \endcode

        In order to view the debug image, launch your program withing an rendering
        debugger (RenderDoc is recommended) and capture some frame or frames.

        You can declare multiple debug probes. Each one will generate its own
        storage image.
    */

    template< class ValueT >
    void DebugProbe (
        const ValueT& value,
        const IVec2& coords,
        const VkExtent3D& extent );
};

// -----------------------------------------------------------------------------
/**
    \brief Auxiliary structure holding basic vertex attributes.

    There are predefined members inside several shader objects (VertexShader,
    GeometryShader, TessControlShader, TessEvalShader), called 
    \c inVertices, \c outVertex or \c outVertices, giving access to this structure.
    It is supposed to be either only written into or being read, depending on
    the field and shader. In case of writes, fields of the structure represent
    various results of geometric computation inside shader, mainly the vertex
    position. In case of reads, you can read values computed by previous
    shader in the pipeline.

    You can also compute several other auxiliary values, like rendered point
    size (used in point drawing mode), or additional clipping/culling planes.

    In shaders that write to the structure, writing to any field of it is optional.

    The \c gl_perVertex structure is only conceptual. It is not possible to declare
    a variable of \c gl_perVertex type in your code. Use it only as a member of
    shader interfaces, accessing the fields with dot operator.
*/

struct gl_perVertex
{
    /** \brief In or Out: Position of the vertex in clip space (after projection). */
    Vec4 position;

    /** \brief In or Out: Size of the point primitive. */
    Float pointSize;

    /** \brief In or Out: Array of additional clipping planes. */
    Float clipDistance[];

    /** \brief In or Out: Array of additional culling planes. */
    Float cullDistance[];

    /** \brief Sets the number of additional clipping planes.
    
        Caution: in shaders where \c gl_perVertex is arrayed, this method is accessible
        on the whole array, not single item.
    */
    void setClipPlanes ( int n );

    /** \brief Sets the number of additional culling planes.
    
        Caution: in shaders where \c gl_perVertex is arrayed, this method is accessible
        on the whole array, not single item.
    */
    void setCullPlanes ( int n );

    /** \brief Retrieves the number of additional clipping planes.
    
        Caution: in shaders where \c gl_perVertex is arrayed, this method is accessible
        on the whole array, not single item.
    */
    void getClipPlanes();

    /** \brief Retrieves the number of additional culling planes.
    
        Caution: in shaders where \c gl_perVertex is arrayed, this method is accessible
        on the whole array, not single item.
    */
    void getCullPlanes();
};

// -----------------------------------------------------------------------------

/**
    \brief Vertex shader interface.

    An object of this class is provided to user-defined vertex shader. It
    is usable only in GPU-side vertex shader code. Its main purpose is
    to give access to built-in shader variables.

    By convention, member names starting with \c in denote read-only variables,
    while the \c out prefix mean that the variable may be only written to.
*/

class VertexShader : public Shader
{
public:
    /** \brief In: index of current vertex. */
    Int inVertexIndex;

    /** \brief In: index of current instance. */
    Int inInstanceIndex;

    /** \brief Out: computed basic vertex attributes. */
    gl_perVertex outVertex;
};

// -----------------------------------------------------------------------------
/**
    \brief Tessellation control shader interface.

    An object of this class is provided to user-defined tesselation control shader. It
    is usable only in GPU-side tessellation control shader code. Its main purpose is
    to give access to built-in shader variables and some special operations.

    Tessellation control shader is a preprocessing shader in tessellation. It
    converts an input patch of M control points to an output patch
    consisting of N control points. One possible application is to decompose
    a NURBS patch to an array of simpler Bezier patches (represented as single
    output patch).

    The tessellation control shader routine is invoked exactly N times for each
    processed patch, that is, for each computed \b vertex of the resulting patch.
    However, Vulkan requires the output vertex data to be declared as an array.
    You must index the \c outVertices array by value read from \c inInvocationId
    variable and write single computed vertex. On the other hand, you have read
    access to all vertices of the source patch simultaneously, in each invocation.

    All inter-shader variables declared between the vertex shader and the tessellation
    control shader are also presented as arrays. You can access these variables
    separately for each input patch vertex, by indexing these arrays.

    By convention, member names starting with \c in denote read-only variables,
    while the \c out prefix mean that the variable may be only written to.
*/

class TessControlShader : public Shader
{
public:
    /** \brief In: vertices of the source patch. */
    gl_perVertex inVertices [];

    /** \brief In: index of currently computed output patch vertex. */
    Int inInvocationId;

    /** \brief In: number of vertices in current input patch (\c inVertices array size). */
    Int inPatchVertices;

    /** \brief In: index of the input patch in currently rendered set of patches. */
    Int inPrimitiveId;

    /** \brief Out: vertices of the resulting patch. */
    gl_perVertex outVertices [];

    /** \brief Out: computed outer tessellation levels. */
    Float outTessLevelOuter [ 4 ];

    /** \brief Out: computed inner tessellation levels. */
    Float outTessLevelInner [ 2 ];
};

// -----------------------------------------------------------------------------
/**
    \brief Tessellation evaluation shader interface.

    An object of this class is provided to user-defined tesselation evaluation shader.
    It is usable only in GPU-side tessellation eval shader code. Its main purpose is
    to give access to built-in shader variables and some special operations.

    By convention, member names starting with \c in denote read-only variables,
    while the \c out prefix mean that the variable may be only written to.
*/

class TessEvalShader : public Shader
{
public:
    /** \brief In: vertices of the evaluated patch. */
    gl_perVertex inVertices [];

    /** \brief In: number of vertices in current input patch (\c inVertices array size). */
    Int inPatchVertices;

    /** \brief In: index of the input patch in currently rendered set of patches. */
    Int inPrimitiveId;

    /** \brief In: coordinates of currently evaluated point, inside triangular or rectangular domain.
    
        In case of a triangular patch, this field contains barycentric coordinates
        relative to the corners. All three components are used.

        In case of a rectangular patch, this field contains rectangular coordinates
        relative to the origin. Only first two components are used, the third one is
        equal to zero.

        In either case, every coordinate falls into [0, 1] range.
    */
    Vec3 inTessCoord;

    /** \brief In: outer tessellation levels. */
    Float inTessLevelOuter [ 4 ];

    /** \brief In: inner tessellation levels. */
    Float inTessLevelInner [ 2 ];

    /** \brief Out: computed vertex data. */
    gl_perVertex outVertex;
};

// -----------------------------------------------------------------------------
/**
    \brief Geometry shader interface.

    An object of this class is provided to user-defined geometry shader. It
    is usable only in GPU-side geometry shader code. Its main purpose is
    to give access to built-in shader variables and some special operations.

    By convention, member names starting with \c in denote read-only variables,
    while the \c out prefix mean that the variable may be only written to.
*/

class GeometryShader : public Shader
{
public:
    /** \brief Emits one vertex for currently processed primitive and resets the
               \c outVertex and output variables to receive another vertex data.

        This funtion emits the current values of all output variables to the
        current output primitive. After execution, the values of
        all output variables are undefined.
    */
    void EmitVertex();

    /** \brief Finishes the current primitive and starts a new one. No
               vertex is emitted.
    */
    void EndPrimitive();

    /** \brief In: vertices of currently processed primitive. */
    gl_perVertex inVertices [];

    /** \brief In: index of current geometry shader invocation in case when
               multiple geometry shader invocations are enabled.
    
        Geometry shader can be invoked multiple times per primitive. This mode
        is enabled by calling the geometryShader::setInvocations() method and
        specifying the number of invocations.
    . */
    Int inInvocationId;

    /** \brief In: primitive index in current set of primitives. */
    Int inPrimitiveId;

    /** \brief Out: computed vertex data. */
    gl_perVertex outVertex;

    /** \brief Out: allows to direct currently processed primitive to specific layer
         of multi-layer output attachment. */
    Int outLayer;

    /** \brief Out: allows to pass the primitive id to the fragment shader. */
    Int outPrimitiveId;

    /** \brief Out: allows to direct currently processed primitive to specific viewport.
    
        Multiple viewports can be specified for rasterization. They are all
        generating pixels for the same image, but may differ in viewport
        transformation and scissors.
    */
    Int outViewportIndex;
};

// -----------------------------------------------------------------------------
/**
    \brief Fragment shader interface.

    An object of this class is provided to user-defined fragment shader. It
    is usable only in GPU-side fragment shader code. Its main purpose is
    to give access to built-in shader variables and some special operations.

    By convention, member names starting with \c in denote read-only variables,
    while the \c out prefix mean that the variable may be only written to.
*/

class FragmentShader : public Shader
{
public:
    /** \brief Terminates shader execution without writing the fragment. */
    void Kill();

    /** \brief Enables early tests (depth, stencil, etc.) for this shader. */
    void EarlyTest();

    /** \brief Creates a debug probe image and writes given value.
    
        This method is similar to generic Shader::DebugProbe version, but
        is applicable only for fragment shaders and it automatically
        determines coordinates and size of the image. Use for convenience
        only. You can also use the generic version in fragment shaders.
    
    */
    template< class ValueT >
    void DebugProbe ( const ValueT& value );

    /** \brief In: current pixel coordinates in the frame buffer. */
    Vec4 inFragCoord;

    /** \brief In: true if current pixel belongs to a front-facing primitive. */
    Bool inFrontFacing;

    /** \brief In: the layer index of the primitive that the fragment invocation belongs to. */
    Int inLayer;

    /** \brief In: zero-based index of the sample the invocation corresponds to. */
    Int inSampleId;

    /** \brief In: zero-based index of the sample the invocation corresponds to. */
    Vec2 inSamplePosition;

    /** \brief In: the sub-pixel position of the sample being shaded. */
    Vec2 inPointCoord;

    /** \brief In: whether the current invocation is a helper invocation. */
    Bool inHelperInvocation;

    /** \brief In: the index of the current primitive. */
    Int inPrimitiveId;

    /** \brief In: the index of the viewport. */
    Int inViewportIndex;

    /** \brief Out: the new depth value for all samples covered by the fragment. */
    Float outFragDepth;
};

// -----------------------------------------------------------------------------
/**
    \brief Compute shader interface.

    An object of this class is provided to user-defined compute shader. It
    is usable only in GPU-side compute shader code. Its main purpose is
    to give access to built-in shader variables and some special operations.
*/

class ComputeShader : public Shader
{
public:
    /** \brief The global workgroup that the current invocation is a member of. */
    IVec3 inWorkgroupId;

    /** \brief The number of local workgroups that are part of the dispatch that the invocation belongs to. */
    IVec3 inNumWorkgroups;

    /** \brief The location of the current compute shader invocation within the local workgroup. */
    IVec3 inLocalInvocationId;

    /** \brief The location of the current invocation within the global workgroup. */
    IVec3 inGlobalInvocationId;

    /** \brief The dimensions of local workgroup. */
    IVec3 inWorkgroupSize;

    /** \brief Returns CPU-side structure specifying the dimensions of local workgroup. */
    const SLocalGroupSize& localGroupSize() const;

    /** \brief Retrieves total size (in bytes) of the workgroup-scoped shared memory. */
    unsigned int getTotalWorkgroupMemory() const;

    /** \brief Retrieves available size (in bytes) of the workgroup-scoped shared memory. */
    unsigned int getFreeWorkgroupMemory() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for inter-shader communication variable of simple
        type (scalar, vector or matrix).
        Place in your pipeline configuration class to declare an inter-shader
        communication variable.

    The first template argument is the type of the variable. It should be
    a scalar (e.g. Int, Float), vector (e.g. Vec3, IVec2) or matrix (e.g. Mat4)
    type.

    The second template argument is the shader binding point class for source
    shader. It can be one of:

    - vertexShader
    - tessControlShader
    - tessEvalShader
    - geometryShader

    The third template argument is the shader binding point class for destination
    shader. It can be one of:

    - tessControlShader
    - tessEvalShader
    - geometryShader
    - fragmentShader

    Source and destination shader should be directly connected. It is an error
    to declare a communication variable from vertexShader to fragmentShader
    when e.g. geometryShader is also present.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig subclass).

    In the source shader, use Output accessor template to write the value.

    In the destination shader, use Input accessor template to read the value.
*/

template< class ValueT, class OutputShader, class InputShader >
class ioVariable
{
public:
    /** \brief Constructor - takes no arguments. */
    ioVariable();
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for inter-shader communication variable of structural
        type. Place in your pipeline configuration class to declare an inter-shader
        communication variable.

    The first template argument is the type of the GPU-side structure. It should
    be your custom subclass of LocalStruct template defining the structure.

    The second template argument is the shader binding point class for source
    shader. It can be one of:

    - vertexShader
    - tessControlShader
    - tessEvalShader
    - geometryShader

    The third template argument is the shader binding point class for destination
    shader. It can be one of:

    - tessControlShader
    - tessEvalShader
    - geometryShader
    - fragmentShader

    Source and destination shader should be directly connected. It is an error
    to declare a communication variable from vertexShader to fragmentShader
    when e.g. geometryShader is also present.

    This class should be used only to define a binding point inside your
    custom pipeline configuration (a PipelineConfig subclass).

    In the source shader code, use Output accessor template to write the value.

    In the destination shader code, use Input accessor template to read the value.
*/

template< class StructT, class OutputShader, class InputShader >
class ioStructure
{
public:
    /** \brief Constructor - takes no arguments. */
    ioStructure();
};

// -----------------------------------------------------------------------------
/**
    \brief Provides input access to communication variable binding point
    from shader code.

    Place Input inside your shader code to read data from a communication
    variable.

    The argument should be decltype of the binding point field. It accepts
    ioVariable and ioStructure binding points. Also provide the binding point
    reference to the constructor.

    You must pay attention yo use Input only in the shader which has been
    declared as destination shader in the binding point.

    Example of usage:

    \code
    struct GStdPipelineData : public LocalStruct< GStdPipelineData >
    {
        Member< Vec4 > m_worldVertex;
        Member< Vec4 > m_worldNormal;
        Member< Vec4 > m_worldTangent;

        Member< Vec4 > m_textureUV01;
        Member< Vec4 > m_textureUV23;
    };

    class MyPipelineConfig : public PipelineConfig
    {
        void fVertexShader ( VertexShader* pShader );
        void fFragmentShader ( FragmentShader* pShader );

        // ...
        typedef ioStructure< GStdPipelineData, vertexShader, fragmentShader > ShaderIO;
        ShaderIO m_shaderIO;

        fragmentShader m_fragmentShader;
    };

    void MyPipelineConfig :: fFragmentShader ( FragmentShader* pShader )
    {
        // GPU code

        Input< decltype ( m_shaderIO ) > input ( m_shaderIO );

        // Caution: as this is fragment shader, worldVertex will be interpolated
        const Vec4 worldVertex = input [ & GStdPipelineData::m_worldVertex ];
        // ...
    }

    \endcode
*/

template< class BindingPointT >
class Input
{
public:
    /** \brief Constructs the input point associated with specified binding point
        (ioVariable or ioStructure).
    */
    Input ( const BindingPointT& var );

    /** \brief Constructs the input point associated with specified binding point
        (ioVariable or ioStructure). Additionally sets interpolation mode.
        The mode is a bitwise OR of the following flags:

        - IN_FLAT: no interpolation will be done.
        - IN_NOPERSPECTIVE: linear, non-perspective correct, interpolation will be used.
        - IN_CENTROID: when used with multi-sampling rasterization, allows a single interpolation location for an entire pixel.
        - IN_SAMPLE: when used with multi-sampling rasterization, requires per-sample interpolation.

        This constructor is allowed only for ioVariable binding points and only
        in fragment shaders.
    */

    Input ( const BindingPointT& var, std::uint32_t interpFlags );

    /**
        \brief Provides access to structure members for ioStructure binding points.
        Provide a pointer to member of your structure (a subclass of LocalStruct).

        Not applicable for ioVariable binding points.
    */
    template< typename MemberT >
    auto operator[]( MemberT StructT::* pMember ) const;

    /**
        \brief Provides access to an item in case if the variable is an array.

        Communication variables are automatically converted to arrays in certain
        types of shaders, i.e.:
        - geometry shaders,
        - tessellation control shaders,
        - tessellation evaluation shaders.

        This is because these shaders operate on multiple vertices at once,
        forming a geometric primitive or patch. You can access values
        for individual vertices, by using one of indexing operators.
    */

    auto operator[]( const Int& index );

    /** \overload */
    auto operator[]( const UInt& index );

    /** \overload */
    auto operator[]( int index );

    /** \overload */
    auto operator[]( unsigned int index );

    /** \brief Returns GPU-side value equal to the array size (number of vertices). */
    Int Size() const;

    /** \brief Returns CPU-side value equal to the array size (number of vertices). */
    int size() const;

    // te 3 tylko dla ioVariable< ValueT >
    ValueT InterpolateAtCentroid();
    ValueT InterpolateAtSample ( const Int& sample );
    ValueT InterpolateAtOffset ( const Vec2& offset );
};

// -----------------------------------------------------------------------------
/**
    \brief Provides output access to communication variable binding point
    from shader code.

    Place Output inside your shader code to write data to a communication
    variable.

    The argument should be decltype of the binding point field. It accepts
    ioVariable and ioStructure binding points. Also provide the binding point
    reference to the constructor.

    You must pay attention yo use Output only in the shader which has been
    declared as source shader in the binding point.

    Example of usage:

    \code
    struct GStdPipelineData : public LocalStruct< GStdPipelineData >
    {
        Member< Vec4 > m_worldVertex;
        Member< Vec4 > m_worldNormal;
        Member< Vec4 > m_worldTangent;

        Member< Vec4 > m_textureUV01;
        Member< Vec4 > m_textureUV23;
    };

    class MyPipelineConfig : public PipelineConfig
    {
        void fVertexShader ( VertexShader* pShader );
        void fFragmentShader ( FragmentShader* pShader );

        // ...
        typedef ioStructure< GStdPipelineData, vertexShader, fragmentShader > ShaderIO;
        ShaderIO m_shaderIO;

        fragmentShader m_fragmentShader;
    };

    void MyPipelineConfig :: fVertexShader ( VertexShader* pShader )
    {
        // GPU code
        Output< decltype ( m_shaderIO ) > output ( m_shaderIO );

        const Vec4 worldVtx = ...;   // compute world vertex position
        output [ & GStdPipelineData::m_worldVertex ] = worldVtx;

        // ...
    }

    \endcode
*/

template< class BindingPointT >
class Output
{
public:
    /** \brief Constructs the output point associated with specified binding point
        (ioVariable or ioStructure).
    */
    Output ( const BindingPointT& var );

    /**
        \brief Provides access to structure members for ioStructure binding points.
        Provide a pointer to member of your structure (a subclass of LocalStruct).

        Not applicable for ioVariable binding points.
    */
    template< typename MemberT >
    auto operator[]( MemberT struct_type::* pMember ) const;

    /**
        \brief Provides access to an item in case if the variable is an array.

        Communication variables are automatically converted to arrays in certain
        types of shaders, i.e.:
        - geometry shaders,
        - tessellation control shaders,
        - tessellation evaluation shaders.

        This is because these shaders operate on multiple vertices at once,
        forming a geometric primitive or patch. You can access values
        for individual vertices, by using one of indexing operators.
    */

    auto operator[]( const Int& index );

    /** \overload */
    auto operator[]( const UInt& index );

    /** \overload */
    auto operator[]( int index );

    /** \overload */
    auto operator[]( unsigned int index );

    // tylko iovariable
    template< typename RightT >
    void operator= ( const RightT& rhs );

    /** \brief Returns GPU-side value equal to the array size (number of vertices). */
    Int Size() const;

    /** \brief Returns CPU-side value equal to the array size (number of vertices). */
    int size() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
