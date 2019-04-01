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
    \brief Binding point class for vertex shaders. Place in your pipeline
    configuration class to declare a vertex shader.

    Example:

    \code
    class MyPipelineConfig : public vpp::PipelineConfig
    {
    public:
        MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

        void fVertexShader ( vpp::VertexShader* pShader );

        vpp::vertexShader m_vertexShader;
    };

    MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
        vpp::PipelineConfig ( pr ),
        m_vertexShader ( this, & MyPipelineConfig::fVertexShader )
    {
    }
    \endcode
*/

class vertexShader
{
public:
    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be pased
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline vertexShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( VertexShader*, Args... ),
        Args... args );
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for tessellation control shaders. Place in your pipeline
    configuration class to declare a tessellation control shader.

    This class also contains several methods allowing to configure operation
    of the tessellator. Call them from your pipeline configuration class constructor.

    Tessellation shaders are optional. If the tessellation control shader is present,
    the tessellation evaluation shader must also be present.

    Example:

    \code
    class MyPipelineConfig : public vpp::PipelineConfig
    {
    public:
        MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

        void fTessControlShader ( vpp::TessControlShader* pShader );

        vpp::tessControlShader m_tessControlShader;
    };

    MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
        vpp::PipelineConfig ( pr ),
        m_tessControlShader ( this, & MyPipelineConfig::fTessControlShader )
    {
    }
    \endcode
*/

class tessControlShader
{
public:
    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be pased
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline tessControlShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( TessControlShader*, Args... ),
        Args... args );

    /**
        \brief Sets the number of control points in the resulting patch.

        The tessellation control shader transforms input patch to an output patch.

        The number of control points within the \b input patch is configured
        using the PipelineConfig::setTessPatchControlPoints() method. Call
        it from your PipelineConfig derived class constructor.

        This method allows to configure how many control points the \b output
        patch will have.
    */
    void setOutputPatchVertices ( int v );

    /**
        \brief Retrieves the number of control points in the resulting patch.
    */
    int getOutputPatchVertices() const;

    /**
        \brief Configures the topology of tessellation.
    */
    void setTopology ( ETessellationTopology topology );

    /**
        \brief Retrieves the topology of tessellation.
    */
    ETessellationTopology getTopology() const;

    /**
        \brief Configures the spacing mode of tessellation.

        See the 21.3 section of official Vulkan docs for details.
    */
    void setSpacing ( ETessellationSpacing spacing );

    /**
        \brief Retrieves the spacing mode of tessellation.

        See the 21.3 section of official Vulkan docs for details.
    */
    ETessellationSpacing getSpacing() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for tessellation evaluation shaders. Place in your pipeline
    configuration class to declare a tessellation evaluation shader.

    This class also contains several methods allowing to configure operation
    of the tessellator. Call them from your pipeline configuration class constructor.

    Tessellation shaders are optional. If the tessellation evaluation shader is present,
    the tessellation control shader must also be present.

    Example:

    \code
    class MyPipelineConfig : public vpp::PipelineConfig
    {
    public:
        MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

        void fTessEvalShader ( vpp::TessEvalShader* pShader, int arg );

        vpp::tessControlShader m_tessControlShader;
        vpp::tessEvalShader m_tessEvalShader;
    };

    MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
        vpp::PipelineConfig ( pr ),
        m_tessEvalShader ( m_tessControlShader, this, & MyPipelineConfig::fTessEvalShader ),
    {
    }

    \endcode
*/

class tessEvalShader
{
public:
    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be passed
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline tessEvalShader (
        const tessControlShader& tcs,
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( TessEvalShader*, Args... ),
        Args... args );

    /**
        \brief Enables or disables the point mode of the tessellator.

        The point mode requests the tessellation primitive generator
        to generate a point for each distinct vertex in the subdivided primitive,
        rather than to generate lines or triangles.
    */
    void setPointMode ( bool v );

    /**
        \brief Retrieves the status of the point mode of the tessellator.
    */
    bool isPointMode() const;

    /**
        \brief Sets the orientation of triangles generated by the tessellator.
    */
    void setVertexOrderCW ( bool v );

    /**
        \brief Retrieves the orientation of triangles generated by the tessellator.
    */
    bool isVertexOrderCW() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for geometry shaders. Place in your pipeline
    configuration class to declare a geometry shader.

    This class also contains several methods allowing to configure operation
    of the shader. Call them from your pipeline configuration class constructor.

    Geometry shader is optional.

    Example:

    \code
    class MyPipelineConfig : public vpp::PipelineConfig
    {
    public:
        MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

        void fGeometryShader ( vpp::GeometryShader* pShader );

        vpp::geometryShader m_geometryShader;
    };

    MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
        vpp::PipelineConfig ( pr ),
        m_geometryShader ( this, & MyPipelineConfig::fGeometryShader )
    {
    }

    \endcode
*/

class geometryShader
{
public:
    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be pased
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline geometryShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( GeometryShader*, Args... ),
        Args... args );

    /**
        \brief Configures the topology of primitives generated by the shader.

        See section 22.2 of the official Vulkan docs for details.
    */
    void setOutputTopology ( EGeometryOutputTopology t );

    /**
        \brief Configures the maximum vertex count this shader can output.
    */
    void setMaxOutputVertices ( int value );

    /** \brief Enables multiple geometry shader invocations feature and sets
               the number of invocations.
               
        Call from your PipelineConfig derived class constructor.

        The value of zero invocations disables the feature.

        See section 22.3 of the official Vulkan docs for details.
    */
    void setInvocations ( int inv );
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for fragment shaders. Place in your pipeline
    configuration class to declare a fragment shader.

    Example:

    \code
    class MyPipelineConfig : public vpp::PipelineConfig
    {
    public:
        MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );

        void fFragmentShader ( vpp::FragmentShader* pShader );

        vpp::fragmentShader m_fragmentShader;
    };

    MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
        vpp::PipelineConfig ( pr ),
        m_fragmentShader ( this, & MyPipelineConfig::fFragmentShader )
    {
    }
    \endcode
*/

class fragmentShader
{
public:
    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be pased
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline fragmentShader (
        ClassT* pParentClass,
        void ( ClassT::* fMethodDef )( FragmentShader*, Args... ),
        Args... args );
};

// -----------------------------------------------------------------------------
/**
    \brief Binding point class for compute shaders. Place in your pipeline
    configuration class to declare a compute shader.
*/

class computeShader
{
public:
    struct SLocalSize
    {
        int x;
        int y;
        int z;
    };

    /**
        \brief Constructor.

        As the first parameter, put the pointer to parent PipelineConfig derived
        class. Usually it is the \c this pointer, as the constructor is being
        called from the pipeline config class constructor.

        The second argument is a pointer to member method implementing the shader.

        Optionally there can be more user-defined arguments. They will be pased
        to the shader method unchanged.
    */

    template< class ClassT, typename... Args >
    inline computeShader (
        ClassT* pParentClass,
        const SLocalSize& localSize,
        void ( ClassT::* fMethodDef )( ComputeShader*, Args... ),
        Args... args );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
