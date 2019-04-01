// -----------------------------------------------------------------------------

#include <vppAll.hpp>

#include "../vppExCommon/vppExCommon.hpp"
#include "../vppExCommon/vppExCommonMath.hpp"
#include "../vppExCommon/vppExCommonApp.hpp"
#include "../vppExCommon/vppExCommonLoader.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <vppSupportGLM.hpp>

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
{
    vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_pos;
    vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_normal;
    vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_uv;
    vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_color;
};

// -----------------------------------------------------------------------------

typedef TVertexAttr< vpp::CPU > CVertexAttr;
typedef TVertexAttr< vpp::GPU > GVertexAttr;
typedef vpp::gvector< CVertexAttr, vpp::Buf::VERTEX > VertexAttrBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TFrameParams : public vpp::UniformStruct< TAG, TFrameParams >
{
    vpp::UniformFld< TAG, glm::mat4 > m_projection;
    vpp::UniformFld< TAG, glm::mat4> m_model;
    vpp::UniformFld< TAG, glm::vec4 > m_lightPos;
};

typedef TFrameParams< vpp::GPU > GFrameParams;
typedef TFrameParams< vpp::CPU > CFrameParams;

typedef vpp::gvector< CFrameParams, vpp::Buf::UNIFORM > FrameParamsBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef vpp::format< float, float, float, float > FormatF32x4;
typedef vpp::format< float, vpp::DT > FormatDepth;

typedef vpp::ImageAttributes<
    FormatDepth, vpp::RENDER, vpp::IMG_TYPE_2D, vpp::Img::DEPTH,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    false, false > DepthBufferAttr;

typedef vpp::Image< DepthBufferAttr > DepthBufferImage;
typedef vpp::ImageViewAttributes< DepthBufferImage > DepthBufferViewAttr;
typedef vpp::ImageView< DepthBufferViewAttr > DepthBufferView;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ExRenderGraph : public vpp::RenderGraph
{
public:
    ExRenderGraph (
        const vpp::Surface& hSurface,
        const DepthBufferView& depthBufferView );

public:
    vpp::Preprocess m_init;
    vpp::Process m_render;
    vpp::Display m_display;
    vpp::Attachment< FormatDepth > m_depthBuffer;
};

// -----------------------------------------------------------------------------

ExRenderGraph :: ExRenderGraph (
    const vpp::Surface& hSurface,
    const DepthBufferView& depthBufferView ) :
        m_display ( hSurface ),
        m_depthBuffer ( depthBufferView )
{
    m_render.addColorOutput ( m_display );
    m_render.setDepthOutput ( m_depthBuffer, 1.0f );
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
        const TextureLoaderView& texv,
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

    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioNormal;
    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioColor;
    vpp::ioVariable< vpp::Vec2, vpp::vertexShader, vpp::fragmentShader > m_ioUV;
    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioViewVec;
    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioLightVec;

    vpp::NormalizedSampler m_sampler;
    vpp::inConstSampledTexture< TextureLoaderView > m_colorMap;

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
        m_sampler ( dev ),
        m_colorMap ( m_sampler ),
        m_vertexShader ( this, & ExRenderPipeline::fVertexShader ),
        m_fragmentShader ( this, & ExRenderPipeline::fFragmentShader )
{
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: setDataBuffers (
    const vpp::UniformBufferView& fpv,
    const TextureLoaderView& texv,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        m_framePar = fpv,
        m_colorMap = texv
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

    UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

    Output< decltype ( m_ioNormal ) > outNormal ( m_ioNormal );
    Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
    Output< decltype ( m_ioUV ) > outUV ( m_ioUV );
    Output< decltype ( m_ioViewVec ) > outViewVec ( m_ioViewVec );
    Output< decltype ( m_ioLightVec ) > outLightVec ( m_ioLightVec );

    outColor = m_vertices [ & GVertexAttr::m_color ][ XYZ ];
    outUV = m_vertices [ & GVertexAttr::m_uv ][ XY ];

    const Mat4 proj = inFramePar [ & GFrameParams::m_projection ];
    const Mat4 model = inFramePar [ & GFrameParams::m_model ];
    const Mat3 model3 = Mat3 ( model );

    const Vec4 pos = model * m_vertices [ & GVertexAttr::m_pos ];
    pShader->outVertex.position = proj * pos;

    outNormal = model3 * m_vertices [ & GVertexAttr::m_normal ][ XYZ ];

    const Vec3 lPos = model3 * inFramePar [ & GFrameParams::m_lightPos ][ XYZ ];
    outLightVec = lPos - pos [ XYZ ];

    outViewVec = -pos [ XYZ ];
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

    Input< decltype ( m_ioNormal ) > inNormal ( m_ioNormal );
    Input< decltype ( m_ioColor ) > inColor ( m_ioColor );
    Input< decltype ( m_ioUV ) > inUV ( m_ioUV );
    Input< decltype ( m_ioViewVec ) > inViewVec ( m_ioViewVec );
    Input< decltype ( m_ioLightVec ) > inLightVec ( m_ioLightVec );

    const Vec4 color = Texture ( m_colorMap, inUV );
    const Vec3 vecN = Normalize ( inNormal );
    const Vec3 vecL = Normalize ( inLightVec );
    const Vec3 vecV = Normalize ( inViewVec );
    const Vec3 vecR = Reflect ( -vecL, vecN );
    const Vec3 diffuse = Max ( Dot ( vecN, vecL ), 0.0f ) * inColor;
    const Vec3 specular = Pow ( Max ( Dot ( vecR, vecV ), 0.0f ), 16.0f ) * Vec3 ( 0.75f );

    m_outColor = Vec4 ( diffuse * color [ XYZ ] + specular, 1.0f );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class VppExMesh : public vppex::ExampleApp
{
public:
    VppExMesh ( const vpp::SurfaceInfo& surfaceInfo );

private:
    vpp::PhysicalDevice getPhysicalDevice() const;
    virtual void render();
    virtual void onViewChanged();
    virtual void onKeyPressed ( unsigned int keyCode );

    void loadMesh();
    void loadTextures();
    void updateFrameParams();

    VkExtent3D getWindowSize();

    const vpp::DeviceFeatures& enableDeviceFeatures();

private:
    //#ifdef _DEBUG
        //static const bool VALIDATION = true;
    //#else
        static const bool VALIDATION = true;
    //#endif

    std::ostringstream m_validationLog;

    vpp::Instance m_instance;
    vpp::StreamDebugReporter m_debugReporter;
    vpp::PhysicalDevice m_physical;
    vpp::DeviceFeatures m_devFeatures;
    vpp::Device m_device;
    vpp::Surface m_surface;
    vpp::SwapChain m_swapchain;

    TextureLoader m_textureLoader;
    TextureLoader::Texture m_textureInfo;

    std::unique_ptr< VertexAttrBuffer > m_pVertexAttrBuffer;
    std::unique_ptr< vpp::Indices > m_pIndexBuffer;
    
    VkExtent3D m_size;

    FrameParamsBuffer m_frameParamsBuffer;
    DepthBufferImage m_depthBufferImage;
    DepthBufferView m_depthBufferView;

    ExRenderGraph m_renderGraph;
    vpp::PipelineLayout< ExRenderPipeline > m_pipelineLayout;
    
    vpp::ShaderDataBlock m_shaderDataBlock;

    vpp::RenderingOptions m_optionsC;
    vpp::RenderingOptions m_optionsW;

    vpp::RenderPass m_renderPass;

    vpp::RenderManager m_renderManager;

    bool m_bWireframe;
    bool m_bRebuildBuffers;
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

VppExMesh :: VppExMesh ( const vpp::SurfaceInfo& si ) :
    vppex::ExampleApp ( "VppExMesh", "VppExMesh", si ),
    m_instance ( vpp::createInstance().validation ( VALIDATION ) ),
    m_debugReporter ( m_validationLog, m_instance ),
    m_physical ( getPhysicalDevice() ),
    m_device ( vpp::Device ( m_physical, enableDeviceFeatures() ) ),
    m_surface ( vpp::Surface ( surfaceInfo(), m_physical, m_instance ) ),
    m_swapchain ( vpp::SwapChain ( m_device, m_surface ) ),
    m_textureLoader ( m_device ),
    m_size ( getWindowSize() ),
    m_frameParamsBuffer ( 1, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_depthBufferImage ( m_size, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_depthBufferView ( m_depthBufferImage ),
    m_renderGraph ( m_surface, m_depthBufferView ),
    m_pipelineLayout ( m_renderGraph.m_render, m_device, m_renderGraph.m_display ),
    m_shaderDataBlock ( m_pipelineLayout ),
    m_renderPass ( m_renderGraph, m_device ),
    m_renderManager ( m_swapchain ),
    m_bWireframe ( false ),
    m_bRebuildBuffers ( false )
{
	m_zoom = -5.5f;
	m_rotation = glm::vec3 ( -0.5f, -112.75f, 0.0f );
	m_cameraPos = glm::vec3 ( 0.1f, 1.1f, 0.0f );

    m_optionsC.addViewport ( vpp::Viewport ( m_size.width, m_size.height ) );
    m_optionsC.setEnableDepthTest ( true );
    m_optionsC.setEnableDepthWrite ( true );
    m_optionsC.setEnableDepthBoundsTest ( false );
    m_optionsC.setEnableDepthClamp ( false );
    m_optionsC.setEnableStencilTest ( false );
    m_optionsC.setPolygonMode ( VK_POLYGON_MODE_FILL );
    m_optionsC.setDepthCompareOperator ( VK_COMPARE_OP_LESS_OR_EQUAL );
    m_optionsC.setCullMode ( VK_CULL_MODE_BACK_BIT );
    m_optionsC.setFrontFace ( VK_FRONT_FACE_CLOCKWISE );

    m_optionsW.addViewport ( vpp::Viewport ( m_size.width, m_size.height ) );
    m_optionsW.setEnableDepthTest ( true );
    m_optionsW.setEnableDepthWrite ( true );
    m_optionsW.setEnableDepthBoundsTest ( false );
    m_optionsW.setEnableDepthClamp ( false );
    m_optionsW.setEnableStencilTest ( false );
    m_optionsW.setPolygonMode ( VK_POLYGON_MODE_LINE );
    m_optionsW.setDepthCompareOperator ( VK_COMPARE_OP_LESS_OR_EQUAL );
    m_optionsW.setCullMode ( VK_CULL_MODE_BACK_BIT );
    m_optionsW.setFrontFace ( VK_FRONT_FACE_CLOCKWISE );

    const unsigned int colorPipeline =
        m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_optionsC );

    const unsigned int wframePipeline =
        m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_optionsW );

    loadMesh();
    loadTextures();
    updateFrameParams();

    m_pipelineLayout.definition().setDataBuffers (
        m_frameParamsBuffer,
        m_textureInfo.m_view,
        & m_shaderDataBlock );
    
    m_renderGraph.m_init << [ this ]()
    {
        m_frameParamsBuffer.cmdCommit();
        m_pVertexAttrBuffer->cmdCommit();
        m_pIndexBuffer->cmdCommit();
    };

    m_renderGraph.m_render << [ this, colorPipeline, wframePipeline ]()
    {
        const unsigned int pipelineId = m_bWireframe ? wframePipeline : colorPipeline;
        m_renderPass.pipeline ( m_renderGraph.m_render.index(), pipelineId ).cmdBind();

        m_pipelineLayout.definition().bindVertices (
            *m_pVertexAttrBuffer, *m_pIndexBuffer );

        m_shaderDataBlock.cmdBind();

        m_renderGraph.cmdDrawIndexed ( 
            static_cast< unsigned int >( m_pIndexBuffer->size() ), 1, 0, 0, 0 );
    };
}

// -----------------------------------------------------------------------------

const std::string getAssetPath()
{
	return "../../data/";
}

// -----------------------------------------------------------------------------

void VppExMesh :: loadMesh()
{
    using namespace vpp;

    MeshLoader meshLoader;
    meshLoader.loadMesh ( getAssetPath() + "models/voyager/voyager.dae" );

    const MeshLoader::Meshes& meshes = meshLoader.meshes();
    size_t nTotalVertices = 0;
    size_t nTotalIndices = 0;

	for ( uint32_t m = 0; m < meshes.size(); m++ )
	{
        const MeshLoader::Mesh& mesh = meshes [ m ];
        nTotalVertices += mesh.m_vertices.size();
        nTotalIndices += mesh.m_indices.size();
    }

    m_pVertexAttrBuffer.reset ( new VertexAttrBuffer (
        nTotalVertices, MemProfile::DEVICE_STATIC, m_device ) );

    m_pIndexBuffer.reset ( new vpp::Indices (
        nTotalIndices, MemProfile::DEVICE_STATIC, m_device ) );

	// Generate vertex buffer
	float scale = 1.0f;

	// Iterate through all meshes in the file
	// and extract the vertex information used in this demo
	for ( uint32_t m = 0; m < meshes.size(); m++ )
	{
        const MeshLoader::Mesh& mesh = meshes [ m ];

		for ( uint32_t i = 0; i < mesh.m_vertices.size(); i++ )
		{
            const MeshLoader::Vertex& vtx = mesh.m_vertices [ i ];

            CVertexAttr vertex;
            vertex.m_pos = glm::vec4 ( vtx.m_pos * scale, 1.0f );
			vertex.m_normal = glm::vec4 ( vtx.m_normal, 1.0f );
			vertex.m_uv = glm::vec4 ( vtx.m_tex, 0.0f, 0.0f );
			vertex.m_color = glm::vec4 ( vtx.m_color, 1.0f );

			m_pVertexAttrBuffer->push_back ( vertex );
		}
	}

	// Generate index buffer from loaded mesh file
    for ( uint32_t m = 0; m < meshes.size(); m++ )
	{
        const MeshLoader::Mesh& mesh = meshes [ m ];
		const uint32_t indexBase = static_cast< uint32_t >( m_pIndexBuffer->size() );

		for ( uint32_t i = 0; i < mesh.m_indices.size(); i++ )
			m_pIndexBuffer->push_back ( mesh.m_indices [ i ] + indexBase );
	}
}

// -----------------------------------------------------------------------------

void VppExMesh :: loadTextures()
{
	m_textureLoader.loadTexture (
		getAssetPath() + "models/voyager/voyager.ktx",
		VK_FORMAT_BC3_UNORM_BLOCK,
		& m_textureInfo );
}

// -----------------------------------------------------------------------------

void VppExMesh :: updateFrameParams()
{
    unsigned int width = m_size.width;
    unsigned int height = m_size.height;

    m_frameParamsBuffer.resize ( 1 );

    m_frameParamsBuffer [ 0 ].m_projection =
	    glm::perspective (
            glm::radians(60.0f), (float)width / (float)height, 0.1f, 256.0f );

	glm::mat4 viewMatrix =
        glm::translate ( glm::mat4(), glm::vec3 ( 0.0f, 0.0f, m_zoom ) );

    auto& model = m_frameParamsBuffer [ 0 ].m_model;
	model = viewMatrix * glm::translate ( glm::mat4(), m_cameraPos );
	model = glm::rotate ( model, glm::radians ( m_rotation.x ), glm::vec3(1.0f, 0.0f, 0.0f) );
	model = glm::rotate ( model, glm::radians ( m_rotation.y ), glm::vec3(0.0f, 1.0f, 0.0f) );
	model = glm::rotate ( model, glm::radians ( m_rotation.z ), glm::vec3(0.0f, 0.0f, 1.0f) );

    m_frameParamsBuffer [ 0 ].m_lightPos = glm::vec4 ( 25.0f, 5.0f, 5.0f, 1.0f );
}

// -----------------------------------------------------------------------------

VkExtent3D VppExMesh :: getWindowSize()
{
    RECT clientRect;
    ::GetClientRect ( surfaceInfo().hWnd, & clientRect );
    
    return VkExtent3D {
        static_cast< unsigned int >( clientRect.right ),
        static_cast< unsigned int >( clientRect.bottom ),
        1u
    };
}

// -----------------------------------------------------------------------------

const vpp::DeviceFeatures& VppExMesh :: enableDeviceFeatures()
{
    using namespace vpp;
    m_devFeatures.enableIfSupported ( fTextureCompressionBC, m_physical );
    m_devFeatures.enableIfSupported ( fSamplerAnisotropy, m_physical );
    m_devFeatures.enableIfSupported ( fFillModeNonSolid, m_physical );
    return m_devFeatures;
}

// -----------------------------------------------------------------------------

vpp::PhysicalDevice VppExMesh :: getPhysicalDevice() const
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

void VppExMesh :: render()
{
    using namespace vpp;

    updateFrameParams();

    m_renderManager.beginFrame();

    m_renderManager.render ( m_renderPass, m_bRebuildBuffers ?
        RenderManager::REBUILD_CMDS : RenderManager::CACHE_CMDS );

    m_renderManager.endFrame();

    m_bRebuildBuffers = false;

    Queue ( m_device ).waitForIdle();
}

// -----------------------------------------------------------------------------

void VppExMesh :: onViewChanged()
{
    updateFrameParams();
}

// -----------------------------------------------------------------------------

void VppExMesh :: onKeyPressed ( unsigned int keyCode )
{
	switch ( keyCode )
	{
	    case 0x57:
		    m_bWireframe = ! m_bWireframe;
            m_bRebuildBuffers = true;
		    //reBuildCommandBuffers();
		    break;
	}
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

    vppex::VppExMesh exMesh ( surfaceInfo );
    exMesh.run();

    return 0;                                                                                        \
}                                                                                                    

// -----------------------------------------------------------------------------
