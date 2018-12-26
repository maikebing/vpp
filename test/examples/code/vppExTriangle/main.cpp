// -----------------------------------------------------------------------------

#include <vppAll.hpp>

#include "../vppExCommon/vppExCommonMath.hpp"
#include "../vppExCommon/vppExCommonApp.hpp"

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
{
    typedef vpp::format< float, float, float, float >::data_type data_type;

    inline TVertexAttr()
    {
    }

    inline TVertexAttr ( const data_type& pos, const data_type& col ) :
        m_position ( pos ),
        m_color ( col )
    {}

    vpp::Attribute< TAG, float, float, float, float > m_position;
    vpp::Attribute< TAG, float, float, float, float > m_color;
};

// -----------------------------------------------------------------------------

typedef TVertexAttr< vpp::CPU > CVertexAttr;
typedef TVertexAttr< vpp::GPU > GVertexAttr;
typedef vpp::gvector< CVertexAttr, vpp::Buf::VERTEX > VertexAttrBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TFramePar : public vpp::UniformStruct< TAG, TFramePar >
{
    inline TFramePar()
    {
    }

    inline TFramePar (
        const vppex::Matrix4& m2w, 
        const vppex::Matrix4& w2v, 
        const vppex::Matrix4& v2p ) :
            m_model2world ( m2w ),
            m_world2view ( w2v ),
            m_view2projected ( v2p )
    {
    }

    vpp::UniformFld< TAG, vppex::Matrix4 > m_model2world;
    vpp::UniformFld< TAG, vppex::Matrix4 > m_world2view;
    vpp::UniformFld< TAG, vppex::Matrix4 > m_view2projected;
};

typedef TFramePar< vpp::GPU > GFramePar;
typedef TFramePar< vpp::CPU > CFramePar;

typedef vpp::gvector< CFramePar, vpp::Buf::UNIFORM > CFrameParBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef vpp::format< float, float, float, float > FormatF32x4;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ExRenderGraph : public vpp::RenderGraph
{
public:
    ExRenderGraph ( const vpp::Surface& hSurface );

public:
    vpp::Preprocess m_init;
    vpp::Process m_render;
    vpp::Display m_display;
};

// -----------------------------------------------------------------------------

ExRenderGraph :: ExRenderGraph ( const vpp::Surface& hSurface ) :
    m_display ( hSurface )
{
    m_render.addColorOutput ( m_display );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ExRenderPipeline : public vpp::PipelineConfig
{
public:
    ExRenderPipeline (
        const vpp::Process& pr,
        const vpp::Device& dev,
        const vpp::Display& outImage );

    void setDataBuffers (
        const vpp::UniformBufferView& fpv,
        vpp::ShaderDataBlock* pDataBlock );

    void bindVertices (
        const vpp::VertexBufferView& vert,
        const vpp::VertexIndexBufferView& ind
    );

    void fVertexShader ( vpp::VertexShader* pShader );
    void fFragmentShader ( vpp::FragmentShader* pShader );

private:
    vpp::inVertexData< TVertexAttr > m_vertices;
    vpp::inUniformBuffer m_framePar;
    vpp::ioVariable< vpp::Vec4, vpp::vertexShader, vpp::fragmentShader > m_ioColor;
    vpp::outAttachment< FormatF32x4 > m_outColor;

    vpp::vertexShader m_vertexShader;
    vpp::fragmentShader m_fragmentShader;
};

// -----------------------------------------------------------------------------

ExRenderPipeline :: ExRenderPipeline (
    const vpp::Process& pr,
    const vpp::Device& dev,
    const vpp::Display& outImage ) :
        vpp::PipelineConfig ( pr ),
        m_outColor ( outImage ),
        m_vertexShader ( this, & ExRenderPipeline::fVertexShader ),
        m_fragmentShader ( this, & ExRenderPipeline::fFragmentShader )
{
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: setDataBuffers (
    const vpp::UniformBufferView& fpv,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        m_framePar = fpv
    ));
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: bindVertices (
    const vpp::VertexBufferView& vert,
    const vpp::VertexIndexBufferView& ind )
{
    cmdBindVertexInput (( m_vertices = vert ));
    cmdBindIndexInput ( ind );
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: fVertexShader ( vpp::VertexShader* pShader )
{
    using namespace vpp;

    UniformVar< TFramePar, decltype ( m_framePar ) > inFramePar ( m_framePar );

    const Mat4 m2w = inFramePar [ & GFramePar::m_model2world ];
    const Mat4 w2v = inFramePar [ & GFramePar::m_world2view ];
    const Mat4 v2p = inFramePar [ & GFramePar::m_view2projected ];

    const Vec4 inPos = m_vertices [ & GVertexAttr::m_position ];
    const Vec4 inColor = m_vertices [ & GVertexAttr::m_color ];

    const Vec4 result = v2p * w2v * m2w * inPos; 

    pShader->outVertex.position = result;

    Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
    outColor = inColor;
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    Input< decltype ( m_ioColor ) > inColor ( m_ioColor );

    const Vec4 color = inColor;
    m_outColor = color;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class VppExTriangle : public vppex::ExampleApp
{
public:
    VppExTriangle ( const vpp::SurfaceInfo& surfaceInfo );

private:
    vpp::PhysicalDevice getPhysicalDevice() const;
    virtual void render();

private:
    //#ifdef _DEBUG
        //static const unsigned int INSTANCE_FLAGS = vpp::Instance::VALIDATION;
    //#else
        static const unsigned int INSTANCE_FLAGS = 0u;
    //#endif

    std::ostringstream m_validationLog;

    vpp::Instance m_instance;
    vpp::StreamDebugReporter m_debugReporter;
    vpp::PhysicalDevice m_physical;
    vpp::Device m_device;
    vpp::Surface m_surface;
    vpp::SwapChain m_swapchain;
    ExRenderGraph m_renderGraph;
    vpp::PipelineLayout< ExRenderPipeline > m_pipelineLayout;
    vpp::ShaderDataBlock m_dataBlock;
    vpp::RenderingOptions m_options;
    vpp::RenderPass m_renderPass;

    vpp::RenderManager m_renderManager;

    VertexAttrBuffer m_vertexBuffer;
    vpp::Indices m_indexBuffer;
    CFrameParBuffer m_frameParBuffer;
};

// -----------------------------------------------------------------------------

class XInitError : public std::runtime_error
{
public:
    XInitError ( const char* pErrorMsg ) :
        std::runtime_error ( pErrorMsg )
    {}
};

// -----------------------------------------------------------------------------

VppExTriangle :: VppExTriangle ( const vpp::SurfaceInfo& si ) :
    vppex::ExampleApp ( "vppExTriangle", "vppExTriangle", si ),
    m_instance ( INSTANCE_FLAGS ),
    m_debugReporter ( m_validationLog, m_instance ),
    m_physical ( getPhysicalDevice() ),
    m_device ( vpp::Device ( m_physical ) ),
    m_surface ( vpp::Surface ( surfaceInfo(), m_physical, m_instance ) ),
    m_swapchain ( vpp::SwapChain ( m_device, m_surface ) ),
    m_renderGraph ( m_surface ),
    m_pipelineLayout ( m_renderGraph.m_render, m_device, m_renderGraph.m_display ),
    m_dataBlock ( m_pipelineLayout ),
    m_renderPass ( m_renderGraph, m_device ),
    m_vertexBuffer ( 3, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_indexBuffer ( 3, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_frameParBuffer ( 1, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_renderManager ( m_swapchain )
{
    static const float PI = 3.1415926f;

    RECT clientRect;
    ::GetClientRect ( surfaceInfo().hWnd, & clientRect );

    unsigned int width = clientRect.right;
    unsigned int height = clientRect.bottom;
    float zoom = -2.5f;

    m_options.addViewport ( vpp::Viewport ( width, height ) );
    m_options.setEnableDepthTest ( false );
    m_options.setEnableDepthWrite ( false );
    m_options.setEnableDepthBoundsTest ( false );
    m_options.setEnableDepthClamp ( false );
    m_options.setEnableStencilTest ( false );
    m_options.setPolygonMode ( VK_POLYGON_MODE_FILL );
    m_options.setDepthCompareOperator ( VK_COMPARE_OP_LESS_OR_EQUAL );
    m_options.setCullMode ( VK_CULL_MODE_NONE );
    m_options.setFrontFace ( VK_FRONT_FACE_COUNTER_CLOCKWISE );

    m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_options );

    m_vertexBuffer.resize ( 3 );
    m_indexBuffer.resize ( 3 );
    m_frameParBuffer.resize ( 1 );

    // init buf
    CVertexAttr vertData [ 3 ] = {
        CVertexAttr { {  1.0f,  1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        CVertexAttr { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        CVertexAttr { {  0.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    unsigned int indexData [ 3 ] = { 0, 1, 2 };

    CFramePar matrixData [ 1 ];

    matrixData [ 0 ].m_view2projected.setPerspective (
        PI/3, (float)width / (float)height, 0.1f, 256.0f );

    matrixData [ 0 ].m_model2world.setTranslation ( 0.0f, 0.0f, zoom );
    matrixData [ 0 ].m_world2view.setIdentity();

    std::copy ( vertData, vertData + 3, m_vertexBuffer.begin() );
    std::copy ( indexData, indexData + 3, m_indexBuffer.begin() );
    std::copy ( matrixData, matrixData + 1, m_frameParBuffer.begin() );

    m_pipelineLayout.definition().setDataBuffers ( m_frameParBuffer, & m_dataBlock );

    m_renderGraph.m_init << [ this ]()
    {
        m_vertexBuffer.cmdCommit();
        m_indexBuffer.cmdCommit();
        m_frameParBuffer.cmdCommit();
    };

    m_renderGraph.m_render << [ this ]()
    {
        m_renderPass.pipeline ( 0, 0 ).cmdBind();
        m_dataBlock.cmdBind();
        m_pipelineLayout.definition().bindVertices ( m_vertexBuffer, m_indexBuffer ); 

        m_renderGraph.cmdDrawIndexed ( 3, 1, 0, 0, 0 );
    };
}

// -----------------------------------------------------------------------------

vpp::PhysicalDevice VppExTriangle :: getPhysicalDevice() const
{
    vpp::PhysicalDevices physicalDevices;

    if ( m_instance.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
         || physicalDevices.empty() )
    {
        throw XInitError ( "No Vulkan device" );
    }

    return physicalDevices [ 0 ];
}

// -----------------------------------------------------------------------------

void VppExTriangle :: render()
{
    m_renderManager.beginFrame();
    m_renderManager.render ( m_renderPass );
    m_renderManager.endFrame();
}

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------

int APIENTRY WinMain (
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow )
{                    
    vpp::SurfaceInfo surfaceInfo;

    #ifdef PLATFORM_WINDOWS
        surfaceInfo.hInstance = hInstance;
    #endif

    vppex::VppExTriangle exTriangle ( surfaceInfo );
    exTriangle.run();

    return 0;                                                                                        \
}                                                                                                    

// -----------------------------------------------------------------------------
