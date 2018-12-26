// -----------------------------------------------------------------------------

#include <vppAll.hpp>

#include "../vppExCommon/vppExCommonMath.hpp"
#include "../vppExCommon/vppExCommonApp.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
{
    vpp::Attribute< TAG, float, float, float, float > m_pos;
    vpp::Attribute< TAG, float, float, float, float > m_normal;
    vpp::Attribute< TAG, float, float, float, float > m_color;
};

// -----------------------------------------------------------------------------

typedef TVertexAttr< vpp::CPU > CVertexAttr;
typedef TVertexAttr< vpp::GPU > GVertexAttr;
typedef vpp::gvector< CVertexAttr, vpp::Buf::VERTEX > VertexAttrBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TGearParams : public vpp::UniformStruct< TAG, TGearParams >
{
    vpp::UniformFld< TAG, vppex::Matrix4 > m_projection;
    vpp::UniformFld< TAG, vppex::Matrix4 > m_model;
    vpp::UniformFld< TAG, vppex::Matrix4 > m_normal;
    vpp::UniformFld< TAG, vppex::Matrix4 > m_view;
    vpp::UniformFld< TAG, vppex::Vector4 > m_lightpos;
};

typedef TGearParams< vpp::GPU > GGearParams;
typedef TGearParams< vpp::CPU > CGearParams;

typedef vpp::gvector< CGearParams, vpp::Buf::UNIFORM > GearParamsBuffer;

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

struct GearInfo
{
	float innerRadius;
	float outerRadius;
	float width;
	int numTeeth;
	float toothDepth;
	Vector4 color;
	Vector4 pos;
	float rotSpeed;
	float rotOffset;
};

// -----------------------------------------------------------------------------

class VulkanGear
{
public:
    template< class PipelineLayoutT >
    VulkanGear (
        const GearInfo& gearInfo,
        PipelineLayoutT& hPipelineLayout,
        const vpp::Device& hDevice );

    template< class PipelineLayoutT >
    void draw (
        PipelineLayoutT& hPipelineLayout,
        vpp::RenderGraph& renderGraph );

    void cmdUpdateParams();

	void generate ( GearInfo *gearInfo );

    void updateParams (
        const Matrix4& perspective, const Vector4& rotation, float zoom, float timer );

private:
	int newVertex ( float x, float y, float z, const Vector4& normal );
	void newFace ( int a, int b, int c );

private:
	Vector4 m_color;
	Vector4 m_pos;
	float m_rotSpeed;
	float m_rotOffset;

    VertexAttrBuffer m_vertices;
    vpp::Indices m_indices;
    GearParamsBuffer m_params;

    vpp::ShaderDataBlock m_dataBlock;
};

// -----------------------------------------------------------------------------

template< class PipelineLayoutT >
VulkanGear :: VulkanGear (
    const GearInfo& gearInfo,
    PipelineLayoutT& hPipelineLayout,
    const vpp::Device& hDevice ) :
        m_vertices ( gearInfo.numTeeth * 40, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        m_indices ( gearInfo.numTeeth * 3 * 22, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        m_params ( 1, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        m_dataBlock ( hPipelineLayout )
{
    hPipelineLayout.definition().setDataBuffers ( m_params, & m_dataBlock );
}

// -----------------------------------------------------------------------------

int VulkanGear :: newVertex ( float x, float y, float z, const Vector4& normal )
{
    int index = static_cast< int >( m_vertices.size() );
    
    CVertexAttr* pVertex = m_vertices.allocate_back();
    pVertex->m_pos.set ( x, y, z, 1.0f );
    pVertex->m_normal.set ( normal[0], normal[1], normal[2], normal[3] );
    pVertex->m_color.set ( m_color[0], m_color[1], m_color[2], m_color[3] );

    return index;
}

// -----------------------------------------------------------------------------

void VulkanGear :: newFace ( int a, int b, int c )
{
	m_indices.push_back ( a );
	m_indices.push_back ( b );
	m_indices.push_back ( c );
}

// -----------------------------------------------------------------------------

void VulkanGear :: generate ( GearInfo *gearinfo )
{
    static const float pi = 3.1415926f;

	this->m_color = gearinfo->color;
	this->m_pos = gearinfo->pos;
	this->m_rotOffset = gearinfo->rotOffset;
	this->m_rotSpeed = gearinfo->rotSpeed;

	int i;
	float r0, r1, r2;
	float ta, da;
	float u1, v1, u2, v2, len;
	float cos_ta, cos_ta_1da, cos_ta_2da, cos_ta_3da, cos_ta_4da;
	float sin_ta, sin_ta_1da, sin_ta_2da, sin_ta_3da, sin_ta_4da;
	int32_t ix0, ix1, ix2, ix3, ix4, ix5;

	r0 = gearinfo->innerRadius;
	r1 = gearinfo->outerRadius - gearinfo->toothDepth / 2.0f;
	r2 = gearinfo->outerRadius + gearinfo->toothDepth / 2.0f;
	da = 2.0f * pi / gearinfo->numTeeth / 4.0f;

	Vector4 normal;

	for ( i = 0; i < gearinfo->numTeeth; i++ )
	{
		ta = i * 2.0f * pi / gearinfo->numTeeth;

		cos_ta = cos(ta);
		cos_ta_1da = cos(ta + da);
		cos_ta_2da = cos(ta + 2 * da);
		cos_ta_3da = cos(ta + 3 * da);
		cos_ta_4da = cos(ta + 4 * da);
		sin_ta = sin(ta);
		sin_ta_1da = sin(ta + da);
		sin_ta_2da = sin(ta + 2 * da);
		sin_ta_3da = sin(ta + 3 * da);
		sin_ta_4da = sin(ta + 4 * da);

		u1 = r2 * cos_ta_1da - r1 * cos_ta;
		v1 = r2 * sin_ta_1da - r1 * sin_ta;
		len = sqrt(u1 * u1 + v1 * v1);
		u1 /= len;
		v1 /= len;
		u2 = r1 * cos_ta_3da - r2 * cos_ta_2da;
		v2 = r1 * sin_ta_3da - r2 * sin_ta_2da;

		// front face
		normal = Vector4 ( 0.0f, 0.0f, 1.0f );
		ix0 = newVertex ( r0 * cos_ta, r0 * sin_ta, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r1 * cos_ta, r1 * sin_ta, gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r0 * cos_ta, r0 * sin_ta, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, gearinfo->width * 0.5f, normal );
		ix4 = newVertex ( r0 * cos_ta_4da, r0 * sin_ta_4da, gearinfo->width * 0.5f, normal );
		ix5 = newVertex ( r1 * cos_ta_4da, r1 * sin_ta_4da, gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );
		newFace ( ix2, ix3, ix4 );
		newFace ( ix3, ix5, ix4 );

		// front sides of teeth
		normal = Vector4 ( 0.0f, 0.0f, 1.0f );
		ix0 = newVertex ( r1 * cos_ta, r1 * sin_ta, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		// back face 
		normal = Vector4 ( 0.0f, 0.0f, -1.0f );
		ix0 = newVertex ( r1 * cos_ta, r1 * sin_ta, -gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r0 * cos_ta, r0 * sin_ta, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, -gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r0 * cos_ta, r0 * sin_ta, -gearinfo->width * 0.5f, normal );
		ix4 = newVertex ( r1 * cos_ta_4da, r1 * sin_ta_4da, -gearinfo->width * 0.5f, normal );
		ix5 = newVertex ( r0 * cos_ta_4da, r0 * sin_ta_4da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );
		newFace ( ix2, ix3, ix4 );
		newFace ( ix3, ix5, ix4 );

		// back sides of teeth 
		normal = Vector4 ( 0.0f, 0.0f, -1.0f );
		ix0 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, -gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r1 * cos_ta, r1 * sin_ta, -gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		// draw outward faces of teeth 
		normal = Vector4 ( v1, -u1, 0.0f );
		ix0 = newVertex ( r1 * cos_ta, r1 * sin_ta, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r1 * cos_ta, r1 * sin_ta, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		normal = Vector4 ( cos_ta, sin_ta, 0.0f );
		ix0 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r2 * cos_ta_1da, r2 * sin_ta_1da, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		normal = Vector4 ( v2, -u2, 0.0f );
		ix0 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r2 * cos_ta_2da, r2 * sin_ta_2da, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		normal = Vector4 ( cos_ta, sin_ta, 0.0f );
		ix0 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, gearinfo->width * 0.5f, normal );
		ix1 = newVertex ( r1 * cos_ta_3da, r1 * sin_ta_3da, -gearinfo->width * 0.5f, normal );
		ix2 = newVertex ( r1 * cos_ta_4da, r1 * sin_ta_4da, gearinfo->width * 0.5f, normal );
		ix3 = newVertex ( r1 * cos_ta_4da, r1 * sin_ta_4da, -gearinfo->width * 0.5f, normal );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );

		// draw inside radius cylinder 
		ix0 = newVertex ( r0 * cos_ta, r0 * sin_ta, -gearinfo->width * 0.5f, Vector4 ( -cos_ta, -sin_ta, 0.0f ) );
		ix1 = newVertex ( r0 * cos_ta, r0 * sin_ta, gearinfo->width * 0.5f, Vector4 (-cos_ta, -sin_ta, 0.0f ) );
		ix2 = newVertex ( r0 * cos_ta_4da, r0 * sin_ta_4da, -gearinfo->width * 0.5f, Vector4 (-cos_ta_4da, -sin_ta_4da, 0.0f ) );
		ix3 = newVertex ( r0 * cos_ta_4da, r0 * sin_ta_4da, gearinfo->width * 0.5f, Vector4 (-cos_ta_4da, -sin_ta_4da, 0.0f ) );
		newFace ( ix0, ix1, ix2 );
		newFace ( ix1, ix3, ix2 );
	}

    m_vertices.commitAndWait();
    m_indices.commitAndWait();
}

// -----------------------------------------------------------------------------

void VulkanGear :: updateParams (
    const Matrix4& perspective, const Vector4& rotation, float zoom, float timer )
{
    static const float pi = 3.1415926f;

    m_params.resize ( 1 );

	m_params [ 0 ].m_projection = perspective;

    Matrix4 rotate1;
    Matrix4 rotate2;
    Matrix4 cam;

    rotate1.setRotation ( rotation [ 0 ] * pi / 180.0f, Vector4 ( 1.0f, 0.0f, 0.0f ) );
    rotate2.setRotation ( rotation [ 1 ] * pi / 180.0f, Vector4 ( 0.0f, 1.0f, 0.0f ) );

    cam.setLookAt (
		Vector4 ( 0.0f, 0.0f, -zoom ),
		Vector4 ( -1.0f, -1.5f, 0.0f ),
		Vector4 ( 0.0f, 1.0f, 0.0f )
    );

    m_params [ 0 ].m_view = cam * rotate1 * rotate2;

    const float a = ( m_rotSpeed * timer ) + m_rotOffset;

    Matrix4 translate1;
    Matrix4 rotate;

    translate1.setTranslation ( m_pos );
    rotate.setRotation ( a * pi / 180.0f, Vector4 ( 0.0f, 0.0f, 1.0f ) );

    m_params [ 0 ].m_model = translate1 * rotate;

    m_params [ 0 ].m_normal =
        ( m_params [ 0 ].m_view * m_params [ 0 ].m_model ).inverse().transpose();

    m_params [ 0 ].m_lightpos = Vector4 (
        m_pos [ 0 ] - 8.0f*0.707f,
        m_pos [ 1 ] + 0.0f,
        m_pos [ 2 ] + 8.0f*0.707f,
        1.0f
    );
}

// -----------------------------------------------------------------------------

template< class PipelineLayoutT >
void VulkanGear :: draw (
    PipelineLayoutT& hPipelineLayout,
    vpp::RenderGraph& renderGraph )
{
    hPipelineLayout.definition().bindVertices ( m_vertices, m_indices );
    m_dataBlock.cmdBind();

    renderGraph.cmdDrawIndexed (
        static_cast< unsigned int >( m_indices.size() ),
        1, 0, 0, 0 );
}

// -----------------------------------------------------------------------------

void VulkanGear :: cmdUpdateParams()
{
    m_params.cmdCommit();
}

// -----------------------------------------------------------------------------

typedef std::shared_ptr< VulkanGear > HVulkanGear;

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
    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioEyePos;
    vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioLightVec;

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

    UniformVar< TGearParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

    Output< decltype ( m_ioNormal ) > outNormal ( m_ioNormal );
    Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
    Output< decltype ( m_ioEyePos ) > outEyePos ( m_ioEyePos );
    Output< decltype ( m_ioLightVec ) > outLightVec ( m_ioLightVec );

    const Mat4 tnormal = inFramePar [ & GGearParams::m_normal ];
    const Vec4 vnormal = m_vertices [ & GVertexAttr::m_normal ];

    outNormal = Normalize ( ( tnormal * vnormal )[ XYZ ] );
    outColor = m_vertices [ & GVertexAttr::m_color ][ XYZ ];

    const Mat4 view = inFramePar [ & GGearParams::m_view ];
    const Mat4 model = inFramePar [ & GGearParams::m_model ];
    const Mat4 modelView = view * model;

    const Vec4 pos = modelView * m_vertices [ & GVertexAttr::m_pos ];
    outEyePos = pos [ XYZ ];

    const Vec4 lightPos = view * inFramePar [ & GGearParams::m_lightpos ];
    const Vec3 lightVec = Normalize ( lightPos [ XYZ ] - pos [ XYZ ] );
    outLightVec = lightVec;

    const Mat4 proj = inFramePar [ & GGearParams::m_projection ];
    pShader->outVertex.position = proj * pos;
}

// -----------------------------------------------------------------------------

void ExRenderPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    Input< decltype ( m_ioNormal ) > inNormal ( m_ioNormal );
    Input< decltype ( m_ioColor ) > inColor ( m_ioColor );
    Input< decltype ( m_ioEyePos ) > inEyePos ( m_ioEyePos );
    Input< decltype ( m_ioLightVec ) > inLightVec ( m_ioLightVec );

    const Vec3 eye = Normalize ( -inEyePos );
    const Vec3 reflected = Normalize ( Reflect ( -inLightVec, inNormal ) );
    
    const Vec3 iambient = Vec3 ( 0.2f, 0.2f, 0.2f );
    
    const Vec3 idiffuse =
        Vec3 ( 0.5f, 0.5f, 0.5f )
        * Max ( Dot ( inNormal, inLightVec ), 0.0f );

    const float specular = 0.25f;

    const Vec3 ispecular =
        Vec3 ( 0.5f, 0.5f, 0.5f )
        * Pow ( Max ( Dot ( reflected, eye ), 0.0f ), 0.8f )
        * specular;

    const Vec3 resultColor =
        ( ( iambient + idiffuse ) * inColor + ispecular );

    m_outColor = Vec4 ( resultColor, 1.0f );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class VppExGears : public vppex::ExampleApp
{
public:
    VppExGears ( const vpp::SurfaceInfo& surfaceInfo );

private:
    void prepareModels();
    void updateParams();
    vpp::PhysicalDevice getPhysicalDevice() const;
    virtual void render();

    VkExtent3D getWindowSize();

private:
    //#ifdef _DEBUG
        static const unsigned int INSTANCE_FLAGS = vpp::Instance::VALIDATION;
    //#else
    //    static const unsigned int INSTANCE_FLAGS = 0u;
    //#endif

    std::ostringstream m_validationLog;

    vpp::Instance m_instance;
    vpp::StreamDebugReporter m_debugReporter;
    vpp::PhysicalDevice m_physical;
    vpp::Device m_device;
    vpp::Surface m_surface;
    vpp::SwapChain m_swapchain;

    VkExtent3D m_size;

    DepthBufferImage m_depthBufferImage;
    DepthBufferView m_depthBufferView;

    ExRenderGraph m_renderGraph;
    vpp::PipelineLayout< ExRenderPipeline > m_pipelineLayout;
    
    vpp::RenderingOptions m_options;
    vpp::RenderPass m_renderPass;

    vpp::RenderManager m_renderManager;

    std::vector< HVulkanGear > m_gears;

    Vector4 m_rotation;
    float m_zoom;
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

VppExGears :: VppExGears ( const vpp::SurfaceInfo& si ) :
    vppex::ExampleApp ( "vppExGears", "vppExGears", si ),
    m_instance ( INSTANCE_FLAGS ),
    m_debugReporter ( m_validationLog, m_instance ),
    m_physical ( getPhysicalDevice() ),
    m_device ( vpp::Device ( m_physical ) ),
    m_surface ( vpp::Surface ( surfaceInfo(), m_physical, m_instance ) ),
    m_swapchain ( vpp::SwapChain ( m_device, m_surface ) ),
    m_size ( getWindowSize() ),
    m_depthBufferImage ( m_size, vpp::MemProfile::DEVICE_STATIC, m_device ),
    m_depthBufferView ( m_depthBufferImage ),
    m_renderGraph ( m_surface, m_depthBufferView ),
    m_pipelineLayout ( m_renderGraph.m_render, m_device, m_renderGraph.m_display ),
    m_renderPass ( m_renderGraph, m_device ),
    m_renderManager ( m_swapchain ),
	m_rotation ( -23.75, 41.25, 21.0, 0.0f ),
	m_zoom ( -16.0f )
{
    static const float PI = 3.1415926f;

    setTimerSpeed ( 0.125f );

    float zoom = -2.5f;

    m_options.addViewport ( vpp::Viewport ( m_size.width, m_size.height ) );
    m_options.setEnableDepthTest ( true );
    m_options.setEnableDepthWrite ( true );
    m_options.setEnableDepthBoundsTest ( false );
    m_options.setEnableDepthClamp ( false );
    m_options.setEnableStencilTest ( false );
    m_options.setPolygonMode ( VK_POLYGON_MODE_FILL );
    m_options.setDepthCompareOperator ( VK_COMPARE_OP_LESS_OR_EQUAL );
    m_options.setCullMode ( VK_CULL_MODE_BACK_BIT );
    m_options.setFrontFace ( VK_FRONT_FACE_CLOCKWISE );

    m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_options );

    prepareModels();

    m_renderGraph.m_init << [ this ]()
    {
        for ( unsigned int i = 0; i != m_gears.size(); ++i )
            m_gears [ i ]->cmdUpdateParams();
    };

    m_renderGraph.m_render << [ this ]()
    {
        m_renderPass.pipeline ( 0, 0 ).cmdBind();

        for ( unsigned int i = 0; i != m_gears.size(); ++i )
        {
            m_gears [ i ]->draw ( m_pipelineLayout, m_renderGraph );
        }
    };
}

// -----------------------------------------------------------------------------

VkExtent3D VppExGears :: getWindowSize()
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

void VppExGears :: prepareModels()
{
	// Gear definitions
	std::vector< float > innerRadiuses = { 1.0f, 0.5f, 1.3f };
	std::vector< float > outerRadiuses = { 4.0f, 2.0f, 2.0f };
	std::vector< float > widths = { 1.0f, 2.0f, 0.5f };
	std::vector< int > toothCount = { 20, 10, 10 };
	std::vector< float > toothDepth = { 0.7f, 0.7f, 0.7f };

	std::vector< Vector4 > colors =
    {
		Vector4 ( 1.0f, 0.0f, 0.0f ),
		Vector4 ( 0.0f, 1.0f, 0.2f ),
		Vector4 ( 0.0f, 0.0f, 1.0f )
	};

	std::vector< Vector4 > positions =
    {
		Vector4 ( -3.0f,  0.0f, 0.0f ),
		Vector4 (  3.1f,  0.0f, 0.0f ),
		Vector4 ( -3.1f, -6.2f, 0.0f )
	};

	std::vector< float > rotationSpeeds = { 1.0f, -2.0f, -2.0f };
	std::vector< float > rotationOffsets = { 0.0f, -9.0f, -30.0f };

	m_gears.resize ( positions.size() );

	for ( size_t i = 0; i < m_gears.size(); ++i )
	{
		GearInfo gearInfo;
		gearInfo.innerRadius = innerRadiuses[i];
		gearInfo.outerRadius = outerRadiuses[i];
		gearInfo.width = widths[i];
		gearInfo.numTeeth = toothCount[i];
		gearInfo.toothDepth = toothDepth[i];
		gearInfo.color = colors[i];
		gearInfo.pos = positions[i];
		gearInfo.rotSpeed = rotationSpeeds[i];
		gearInfo.rotOffset = rotationOffsets[i];

		m_gears[i] = HVulkanGear ( new VulkanGear ( gearInfo, m_pipelineLayout, m_device ) );
		m_gears[i]->generate ( & gearInfo );
	}
}

// -----------------------------------------------------------------------------

void VppExGears :: updateParams()
{
    static const float PI = 3.1415926f;

	Matrix4 perspective;
    
    perspective.setPerspective (
        PI / 3.0f,
        static_cast< float >( width() ) / static_cast< float >( height() ),
        0.001f, 256.0f );

	for ( size_t i = 0; i < m_gears.size(); ++i )
		m_gears[i]->updateParams ( perspective, m_rotation, m_zoom, timer() * 360.0f );
}

// -----------------------------------------------------------------------------

vpp::PhysicalDevice VppExGears :: getPhysicalDevice() const
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

void VppExGears :: render()
{
    updateParams();

    m_renderManager.beginFrame();
    m_renderManager.render ( m_renderPass );
    m_renderManager.endFrame();

    vpp::Queue ( m_device ).waitForIdle();
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

    vppex::VppExGears exGears ( surfaceInfo );
    exGears.run();

    return 0;                                                                                        \
}                                                                                                    

// -----------------------------------------------------------------------------
