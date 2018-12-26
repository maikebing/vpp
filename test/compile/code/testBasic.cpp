// -----------------------------------------------------------------------------

#include <vppAll.hpp>

// -----------------------------------------------------------------------------
// Basic interface usage
// -----------------------------------------------------------------------------

int testInstance()
{
    {
        vpp::Instance myInstance ( vpp::Instance::VALIDATION );
    }
    {
        vpp::Instance myInstance;

        if ( ! myInstance.valid() )
            return -1;

        VkInstance hInstance = myInstance.handle();

        vpp::PhysicalDevices physicalDevices;

        if ( myInstance.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
             || physicalDevices.empty() )
        {
            return -1;
        }

        const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];

        if ( ! hPhysicalDevice )
            return -1;

        vpp::ExtensionProperties extProps;

        VkResult exres = vpp::Instance::enumerateExtensions ( & extProps );
        vpp::DebugReporter* pReporter = vpp::Instance::getDebugReporter();
    }

    return 0;
}

// -----------------------------------------------------------------------------

void testPhysicalDevice()
{
    vpp::Instance myInstance ( vpp::Instance::VALIDATION );
    vpp::PhysicalDevices physicalDevices;
    myInstance.enumeratePhysicalDevices ( & physicalDevices );
    const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];

    const VkPhysicalDevice hPhysDev = hPhysicalDevice.handle();

    VkPhysicalDeviceProperties pdprops = hPhysicalDevice.getPhysicalDeviceProperties();
    VkPhysicalDeviceMemoryProperties memprops = hPhysicalDevice.getMemoryProperties();
    size_t qfc = hPhysicalDevice.queueFamilyCount();
    VkQueueFamilyProperties famprops = hPhysicalDevice.getQueueFamilyProperties ( 0 );

    const bool bf1 = hPhysicalDevice.supportsFeature ( vpp::fGeometryShader );
    const bool bf2 = hPhysicalDevice.supportsFeature ( vpp::fShaderClipDistance );

    VkFormatFeatureFlags ff1 = hPhysicalDevice.supportsFormat (
        VK_FORMAT_R5G6B5_UNORM_PACK16 );
    VkFormatFeatureFlags ff2 = hPhysicalDevice.supportsFormat (
        VK_FORMAT_R8_UINT, vpp::PhysicalDevice::OPTIMAL_TILING );
    VkFormatFeatureFlags ff3 = hPhysicalDevice.supportsFormat (
        VK_FORMAT_R8G8B8_UNORM, vpp::PhysicalDevice::LINEAR_TILING );
    VkFormatFeatureFlags ff4 = hPhysicalDevice.supportsFormat (
        VK_FORMAT_R8_UINT, vpp::PhysicalDevice::BUFFER );

    std::ostringstream sst;
    hPhysicalDevice.getLimitValuesAsText ( sst );

    bool bsdsf = hPhysicalDevice.supportsDepthStencilFormat ( VK_FORMAT_D32_SFLOAT );
}

// -----------------------------------------------------------------------------

void testDevice()
{
    vpp::Instance myInstance ( vpp::Instance::VALIDATION );
    vpp::PhysicalDevices physicalDevices;
    myInstance.enumeratePhysicalDevices ( & physicalDevices );
    const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];

    const vpp::Device hDevice0;
    const vpp::Device hDevice1 ( hPhysicalDevice );

    vpp::DeviceFeatures feat ( hPhysicalDevice );
    feat.enableIfSupported ( vpp::fDepthBiasClamp );
    feat.enableIfSupported ( vpp::fAlphaToOne );

    vpp::Device hDevice2 ( hPhysicalDevice, feat );

    std::vector< float > qprior;
    vpp::Device hDevice3 ( hPhysicalDevice, qprior );
    vpp::Device hDevice4 ( hPhysicalDevice, qprior, feat );

    if ( hDevice1 )
    {
        VkDevice hDev = hDevice1.handle();
        const vpp::PhysicalDevice hPhysDev = hDevice1.physical();

        const vpp::CommandPool hCmdPool1 =
            hDevice1.defaultCmdPool ( vpp::Q_GRAPHICS );

        const vpp::CommandPool hCmdPool2 =
            hDevice1.defaultCmdPool ( vpp::Q_TRANSFER );

        const vpp::PipelineCache hCache = hDevice1.defaultPipelineCache();

        hDevice1.waitForIdle();
    }
}

// -----------------------------------------------------------------------------

void testSurface()
{
    vpp::Instance myInstance ( vpp::Instance::VALIDATION );
    vpp::PhysicalDevices physicalDevices;
    myInstance.enumeratePhysicalDevices ( & physicalDevices );
    const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];

    vpp::SurfaceInfo surfaceInfo;
    surfaceInfo.hWnd = 0;
    surfaceInfo.hInstance = 0;
    VkSurfaceKHR hSurf = 0;

    const vpp::Surface hSurface ( surfaceInfo, hPhysicalDevice, myInstance );
    const vpp::Surface hSurface2 ( hSurf, hPhysicalDevice, myInstance );

    VkSurfaceKHR hSurf1 = hSurface.handle();

    VkSurfaceCapabilitiesKHR surfCaps;

    if ( hSurface.getCapabilities ( & surfCaps ) )
    {
        surfCaps.currentExtent;
    }
    
    vpp::SurfaceFormats surfFormats;

    if ( hSurface.getFormats ( & surfFormats ) )
    {
        VkFormat fmt = surfFormats [ 0 ].format;
        VkColorSpaceKHR spc = surfFormats [ 0 ].colorSpace;
    }

    vpp::PresentationModes pmodes;

    if ( hSurface.getPresentationModes ( & pmodes ) )
    {
        VkPresentModeKHR pm = pmodes [ 0 ];
    }

    const VkExtent2D surfSize = hSurface.getSize();

    VkColorSpaceKHR cs;

    VkFormat fmt1 = hSurface.getDefaultFormat();
    VkFormat fmt2 = hSurface.getDefaultFormat ( & cs );

    std::ostringstream sst;
    hSurface.getParametersAsText ( sst );
}

// -----------------------------------------------------------------------------

void testSwapChain()
{
    vpp::Instance myInstance ( vpp::Instance::VALIDATION );
    vpp::PhysicalDevices physicalDevices;
    myInstance.enumeratePhysicalDevices ( & physicalDevices );
    const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];
    const vpp::Device hDevice ( hPhysicalDevice );

    vpp::SurfaceInfo surfaceInfo;
    surfaceInfo.hWnd = 0;
    surfaceInfo.hInstance = 0;

    vpp::Surface hSurface ( surfaceInfo, hPhysicalDevice, myInstance );

    vpp::SwapChain hSwapChain1 = vpp::SwapChain ( hDevice, hSurface );
    vpp::SwapChain hSwapChain2 = vpp::SwapChain ( hDevice, hSurface, 3 );
    vpp::SwapChain hSwapChain3 = vpp::SwapChain ( hDevice, hSurface, 3, VK_PRESENT_MODE_IMMEDIATE_KHR );
    vpp::SwapChain hSwapChain4 = vpp::SwapChain ( hDevice, hSurface, 0, VK_PRESENT_MODE_IMMEDIATE_KHR, hSwapChain1 );

    if ( hSwapChain1.valid() )
    {
        VkSwapchainKHR hSwapCh = hSwapChain1.handle();
        vpp::Device hDev = hSwapChain1.device();
        vpp::Surface hSurf = hSwapChain1.surface();
        const size_t nViews = hSwapChain1.views();
        vpp::FrameImageView hView = hSwapChain1.view ( 0 );

        vpp::Queue hQueue ( hDevice );

        const unsigned int iImage = hSwapChain1.acquireDisplayImage ( hQueue );
        hSwapChain1.presentDisplayImage ( hQueue, iImage );
    }
}

// -----------------------------------------------------------------------------
namespace testRenderGraph {
// -----------------------------------------------------------------------------

typedef vpp::format< float, vpp::DT > DepthFormat;
typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > ColorFormat;

typedef vpp::ImageAttributes<
    DepthFormat, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    false, false > DepthBufferAttr;

typedef vpp::Image< DepthBufferAttr > DepthBufferImage;
typedef vpp::ImageViewAttributes< DepthBufferImage > DepthBufferViewAttr;
typedef vpp::ImageView< DepthBufferViewAttr > DepthBufferView;

// -----------------------------------------------------------------------------

class DepthBufferRenderGraph : public vpp::RenderGraph
{
public:
    DepthBufferRenderGraph (
        unsigned int width, unsigned int height,
        vpp::Device hDevice, vpp::Surface hSurface );

public:
    vpp::Device m_hDevice;
    vpp::Process m_renderDepthAttachment;
    vpp::Process m_renderDepthAttachment2;
    vpp::Preprocess m_preprocess;
    vpp::Postprocess m_postprocess;
    vpp::Attachment< DepthFormat > m_depthAttachmentBuffer;
    vpp::Attachment< ColorFormat > m_colorAttachmentBuffer;
    vpp::Attachment< DepthFormat > m_depthAttachmentBuffer2;
    vpp::Attachment< ColorFormat > m_colorAttachmentBuffer2;
    vpp::Attachment< DepthFormat > m_depthAttachmentBuffer3;
    vpp::Attachment< ColorFormat > m_colorAttachmentBuffer3;
    vpp::Display m_displayAttachment;
    vpp::Process m_renderDepthAttachment3;
    vpp::Preprocess m_preprocess3;
    vpp::Postprocess m_postprocess3;
    vpp::Process m_renderDepthAttachment4;
};

// -----------------------------------------------------------------------------

DepthBufferRenderGraph :: DepthBufferRenderGraph (
    unsigned int width, unsigned int height,
    vpp::Device hDevice, vpp::Surface hSurface ) :
        m_hDevice ( hDevice ),
        m_depthAttachmentBuffer ( width, height ),
        m_colorAttachmentBuffer ( width, height, vpp::DISPLAY ),
        m_depthAttachmentBuffer2 ( this, width, height ),
        m_colorAttachmentBuffer2 ( this, width, height, vpp::DISPLAY ),
        m_depthAttachmentBuffer3 ( vpp::FrameImageView() ),
        m_colorAttachmentBuffer3 ( this, vpp::FrameImageView() ),
        m_renderDepthAttachment3 ( this ),
        m_displayAttachment ( hSurface ),
        m_preprocess3 ( this ),
        m_postprocess3 ( this ),
        m_renderDepthAttachment4 ( this, 1 )
{
    m_renderDepthAttachment.addColorOutput (
        m_colorAttachmentBuffer, { 0, 0, 0, 0 },
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    m_renderDepthAttachment.addColorOutput (
        m_colorAttachmentBuffer, { 0, 0, 0, 0 } );

    m_renderDepthAttachment.addReusedColorOutput (
        m_colorAttachmentBuffer,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    m_renderDepthAttachment.addReusedColorOutput (
        m_colorAttachmentBuffer );

    m_renderDepthAttachment.setDepthOutput (
        m_depthAttachmentBuffer, 1.0f
    );

    m_renderDepthAttachment.setDepthOutput (
        m_depthAttachmentBuffer, 1.0f,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
    );

    m_renderDepthAttachment.setReusedDepthOutput (
        m_depthAttachmentBuffer
    );

    m_renderDepthAttachment.setReusedDepthOutput (
        m_depthAttachmentBuffer,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
    );

    m_renderDepthAttachment.setDepthInput ( m_depthAttachmentBuffer );

    m_renderDepthAttachment.addInput ( m_depthAttachmentBuffer );

    m_renderDepthAttachment.addInput (
        m_depthAttachmentBuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    m_renderDepthAttachment.addDependency (
        m_renderDepthAttachment,
        VkSubpassDependency
        {
            0, 0,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_DEPENDENCY_BY_REGION_BIT
        }
    );

    m_renderDepthAttachment.addColorOutput ( m_displayAttachment );

    const unsigned int idx = m_renderDepthAttachment.index();
    RenderGraph* pGraph = m_renderDepthAttachment.graph();

    const unsigned int idx2 = m_preprocess.index();
    RenderGraph* pGraph2 = m_preprocess.graph();

    const unsigned int idx3 = m_postprocess.index();
    RenderGraph* pGraph3 = m_postprocess.graph();

    const unsigned int idxa = m_depthAttachmentBuffer.index();
    RenderGraph* pGrapha = m_depthAttachmentBuffer.graph();
    const unsigned int wa = m_depthAttachmentBuffer.width();
    const unsigned int ha = m_depthAttachmentBuffer.height();
}

// -----------------------------------------------------------------------------

class DepthBufferRenderer
{
public:
    DepthBufferRenderer (
        DepthBufferView* pBufferView, vpp::Device hDevice, vpp::Surface hSurface );

    void createRenderPlan ( vpp::CommandBuffer hCmdBuffer );

private:
    vpp::Device m_hDevice;
    DepthBufferView* m_pBufferView;
    VkExtent3D m_imageSize;

    std::shared_ptr< DepthBufferRenderGraph > m_renderGraph;
    vpp::RenderPass m_renderPass;

    vpp::RenderingOptions m_renderOptions;
    vpp::FrameBuffer m_frameBuffer;

    vpp::IndirectCommands d_indirectCommands;
    vpp::IndexedIndirectCommands d_indexedIndirectCommands;
};

// -----------------------------------------------------------------------------

DepthBufferRenderer :: DepthBufferRenderer (
    DepthBufferView* pBufferView, vpp::Device hDevice, vpp::Surface hSurface ) :
        m_hDevice ( hDevice ),
        m_pBufferView ( pBufferView ),
        m_imageSize ( pBufferView->image().info().extent ),
        m_renderGraph ( new DepthBufferRenderGraph (
            m_imageSize.width, m_imageSize.height, hDevice, hSurface ) ),
        m_renderPass ( *m_renderGraph, hDevice ),
        m_frameBuffer (
            std::vector< vpp::FrameImageView > {
                vpp::FrameImageView ( pBufferView->image() )
            },
            m_renderPass ),
        d_indirectCommands ( 100, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_indexedIndirectCommands ( 100, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    m_renderGraph->m_renderDepthAttachment << [ this ]()
    {
        m_renderPass.pipeline ( m_renderGraph->m_renderDepthAttachment, 0 ).cmdBind();
        m_renderGraph->cmdDraw ( 10, 10, 0, 0 );
        m_renderGraph->cmdDrawIndexed ( 10, 20, 1, 5, 8 );
        m_renderGraph->cmdDrawIndirect ( d_indirectCommands, 50, 23 );
        m_renderGraph->cmdDrawIndexedIndirect ( d_indexedIndirectCommands, 77, 55 );

        vpp::CommandBuffer hCommandBuffer;
        m_renderGraph->cmdDraw ( 10, 10, 0, 0, hCommandBuffer );
        m_renderGraph->cmdDrawIndexed ( 10, 20, 1, 5, 8, hCommandBuffer );
        m_renderGraph->cmdDrawIndirect ( d_indirectCommands, 50, 23, hCommandBuffer );
        m_renderGraph->cmdDrawIndexedIndirect ( d_indexedIndirectCommands, 77, 55, hCommandBuffer );

        VkCommandBuffer hCmdBuffer = 0;
        m_renderGraph->cmdDraw ( 10, 10, 0, 0, hCmdBuffer );
        m_renderGraph->cmdDrawIndexed ( 10, 20, 1, 5, 8, hCmdBuffer );
        m_renderGraph->cmdDrawIndirect ( d_indirectCommands, 50, 23, hCmdBuffer );
        m_renderGraph->cmdDrawIndexedIndirect ( d_indexedIndirectCommands, 77, 55, hCmdBuffer );
    };

    m_renderGraph->m_preprocess << [ this ]()
    {
        m_renderPass.pipeline ( 0, 0 ).cmdBind();
        m_renderGraph->cmdDraw ( 10, 10, 0, 0 );
    };

    m_renderGraph->m_postprocess << [ this ]()
    {
        m_renderPass.pipeline ( 0, 0 ).cmdBind();
        m_renderGraph->cmdDraw ( 10, 10, 0, 0 );
    };

    VkRenderPass hRenderPass = m_renderPass.handle();
    vpp::RenderGraph& rg = m_renderPass.graph();
    vpp::Device rpdev = m_renderPass.device();
    vpp::PipelineCache rppc = m_renderPass.pipelineCache();

    m_renderPass.beginRendering();
    m_renderPass.endRendering();
}

// -----------------------------------------------------------------------------

void DepthBufferRenderer :: createRenderPlan ( vpp::CommandBuffer hCmdBuffer )
{
    vpp::CommandBufferRecorder recorder ( hCmdBuffer );
    recorder.render ( m_renderPass, m_frameBuffer, false );
}

// -----------------------------------------------------------------------------
} // namespace testRenderGraph
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace testComputePass {
// -----------------------------------------------------------------------------

typedef vpp::format< std::uint8_t > KFormatUInt8;

typedef vpp::ImageAttributes<
    KFormatUInt8, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_STORAGE_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    false, true > KVarU8ImageAttr;

typedef vpp::Image< KVarU8ImageAttr > KVarU8Image;
typedef vpp::ImageViewAttributes< KVarU8Image > KVarU8ViewAttr;
typedef vpp::ImageView< KVarU8ViewAttr > KVarU8View;

// -----------------------------------------------------------------------------

class KPLPartitionReducedSet : public vpp::ComputePipelineConfig
{
public:
    KPLPartitionReducedSet (
        const vpp::Device& hDevice );

    void setDataBuffers (
        vpp::ShaderDataBlock* pDataBlock,
        const KVarU8View& reducedSetInput,
        const KVarU8View& partitionedSetOutput );

    void fComputeShader ( vpp::ComputeShader* pShader );

private:
    vpp::ioImage< KVarU8View > d_reducedSetInput;
    vpp::ioImage< KVarU8View > d_partitionedSetOutput;
    vpp::inUniformBuffer d_lookupTable;

    vpp::computeShader d_computeShader;
};

// -----------------------------------------------------------------------------

KPLPartitionReducedSet :: KPLPartitionReducedSet (
    const vpp::Device& hDevice ) :
        d_computeShader ( this, { 32, 1, 1 }, & KPLPartitionReducedSet::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KPLPartitionReducedSet :: setDataBuffers (
    vpp::ShaderDataBlock* pDataBlock,
    const KVarU8View& reducedSetInput,
    const KVarU8View& partitionedSetOutput )
{
    pDataBlock->update ( (
        d_reducedSetInput = reducedSetInput,
        d_partitionedSetOutput = partitionedSetOutput
    ) );
}

// -----------------------------------------------------------------------------

struct KVector4 { float d_data [ 4 ]; };

// -----------------------------------------------------------------------------
} // namespace
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< ::testComputePass::KVector4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( ::testComputePass::KVector4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = vpp::format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef vpp::Float gscalar_type;
    typedef vpp::Vec4 rvalue_type;
    typedef vpp::VVec4 lvalue_type;
    typedef ::testComputePass::KVector4 data_type;
};

// -----------------------------------------------------------------------------
} // namespace testComputePass
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace testComputePass {
// -----------------------------------------------------------------------------

void KPLPartitionReducedSet :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    UniformSimpleArray< KVector4, decltype ( d_lookupTable ) > lt ( d_lookupTable );

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;

    const Int wg = workgroupId [ X ];
    const Int l = localId [ X ];

    // 0 .. 2047, 11 bits
    const Int threadIndex = ( wg << 5 ) + l;
    const Int lightIndex = threadIndex & 255;
    const Int zoneIndex = threadIndex >> 8;

    Shared(); VArray< Vec3, 512 > lookupTable;
    VArray< Vec3, 8 > clusterCenters;
    VInt i = 0;
    VInt j = 0;
    VInt activeClusters = 1;
    VFloat maxCosine = 0.0f;
    VInt nearestCluster = 0;

    i = 0; Do(); While ( i < 16 );
    {
        const Int idx = ( i << 5 ) + l;
        lookupTable [ idx ] = lt [ idx ][ XYZ ];
        ++i;
    }
    Od();

    WorkgroupBarrier();

    clusterCenters [ 0 ] = lookupTable [ 0 ];

    const Float threshold = 0.0f;
    const Float halfThreshold = 0.70710678f;

    VFloat thr = threshold;

    i = 0; Do(); While ( i < 512 );
    {
        const UInt bPresent = ImageLoad (
            d_reducedSetInput, IVec3 ( i, lightIndex, zoneIndex ) )[ X ];

        If ( bPresent != 0 );
        {
            const Vec3 dirVector = lookupTable [ i ];
            maxCosine = 0.0f;
            nearestCluster = 0;

            j = 0; Do(); While ( j < activeClusters );
            {
                const Vec3 clusterCenter = clusterCenters [ j ];
                const Float d = Dot ( dirVector, clusterCenter );
                const Bool bCloser = d > maxCosine;

                maxCosine = Select ( bCloser, d, maxCosine );
                nearestCluster = Select ( bCloser, j, nearestCluster );

                const Bool bFeasible = ( d >= thr );

                If ( bFeasible );
                {
                    ImageStore (
                        d_partitionedSetOutput,
                        IVec3 ( i, lightIndex, zoneIndex ),
                        StaticCast< UInt >( j ) );

                    clusterCenters [ j ] = ( dirVector + clusterCenter ) * Float ( 0.5f );
                    thr = halfThreshold;
                }
                Fi();

                j = Select ( bFeasible, activeClusters + 1, j + 1 );
            }
            Od();

            If ( j == activeClusters );
            {
                const Bool bCanAddCluster = ( activeClusters < 8 );

                If ( bCanAddCluster );
                {
                    clusterCenters [ activeClusters ] = dirVector;
                    thr = threshold;

                    ImageStore (
                        d_partitionedSetOutput,
                        IVec3 ( i, lightIndex, zoneIndex ),
                        StaticCast< UInt >( j ) );

                    ++activeClusters;
                }
                Else();
                {
                    clusterCenters [ nearestCluster ] =
                        ( dirVector + clusterCenters [ nearestCluster ] ) * Float ( 0.5f );

                    thr = halfThreshold;

                    ImageStore (
                        d_partitionedSetOutput,
                        IVec3 ( i, lightIndex, zoneIndex ),
                        StaticCast< UInt >( nearestCluster ) );
                }
                Fi();
            }
            Fi();
        }
        Else();
            ImageStore (
                d_partitionedSetOutput,
                IVec3 ( i, lightIndex, zoneIndex ),
                UInt ( 255u ) );
        Fi();

        ++i;
    }
    Od();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KGenReducedSet : public vpp::ComputePass
{
public:
    KGenReducedSet ( const vpp::Device& hDevice );
    KVarU8Image* getResultImage();

    void createRenderPlan ( vpp::CommandBuffer hCmdBuffer );

public:
    vpp::Device d_device;
    VkExtent3D d_imageSize;
    std::uint32_t d_maxBlockCount;

    vpp::ComputePipelineLayout< KPLPartitionReducedSet > d_plPartitionReducedSet;
    vpp::ShaderDataBlock d_sdbPartitionReducedSet;

    KVarU8Image d_reducedSetImage;
    KVarU8View d_reducedSetView;

    KVarU8Image d_partitionedSetImage;
    KVarU8Image d_partitionedHullImageF;
    KVarU8Image d_partitionedHullImageL;

    KVarU8View d_partitionedSetView;
    KVarU8View d_partitionedHullViewF;
    KVarU8View d_partitionedHullViewL;

    vpp::DispatchIndirectCommands d_dispatchIndTest;
};

// -----------------------------------------------------------------------------

KGenReducedSet :: KGenReducedSet ( const vpp::Device& hDevice ) :
        vpp::ComputePass ( hDevice ),
        d_device ( hDevice ),
        d_maxBlockCount ( 64*256 + ( d_imageSize.width * d_imageSize.height * 8 ) / 256 ),
        d_plPartitionReducedSet ( d_device ),
        d_sdbPartitionReducedSet ( d_plPartitionReducedSet ),
        d_reducedSetImage ( { 512, 256, 1 }, vpp::MemProfile::DEVICE_STATIC, d_device, 1u, 8u ),
        d_reducedSetView ( d_reducedSetImage ),
        d_partitionedSetImage ( { 512, 256, 1 }, vpp::MemProfile::DEVICE_STATIC, d_device, 1u, 8u ),
        d_partitionedHullImageF ( { 128, 256, 1 }, vpp::MemProfile::DEVICE_STATIC, d_device, 1u, 8u ),
        d_partitionedHullImageL ( { 128, 256, 1 }, vpp::MemProfile::DEVICE_STATIC, d_device, 1u, 8u ),
        d_partitionedSetView ( d_partitionedSetImage ),
        d_partitionedHullViewF ( d_partitionedHullImageF ),
        d_partitionedHullViewL ( d_partitionedHullImageL ),
        d_dispatchIndTest ( 10, vpp::MemProfile::DEVICE_STATIC, d_device )
{
    const unsigned int iPlPartitionReducedSet = addPipeline ( d_plPartitionReducedSet );

    d_plPartitionReducedSet.definition().setDataBuffers (
        & d_sdbPartitionReducedSet,
        d_reducedSetView,
        d_partitionedSetView
    );

    (*this) << [ this, iPlPartitionReducedSet ]()
    {
        // phase 2: partition reduced set

        d_sdbPartitionReducedSet.cmdBind();
        pipeline ( iPlPartitionReducedSet ).cmdBind();

        cmdDispatch ( 64, 1, 1 );
        cmdDispatchIndirect ( d_dispatchIndTest, 3 );

        vpp::CommandBuffer hCmdBuf2;
        cmdDispatch ( 64, 1, 1, hCmdBuf2 );
        cmdDispatchIndirect ( d_dispatchIndTest, 3, hCmdBuf2 );

        VkCommandBuffer hCmdBuf3 = 0;
        cmdDispatch ( 64, 1, 1, hCmdBuf3 );
        cmdDispatchIndirect ( d_dispatchIndTest, 3, hCmdBuf3 );

        cmdImagePipelineBarrier (
            d_partitionedSetImage,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_SHADER_READ_BIT,
            false
        );
    };
}

// -----------------------------------------------------------------------------

inline KVarU8Image* KGenReducedSet :: getResultImage()
{
    return & d_reducedSetImage;
}

// -----------------------------------------------------------------------------

void KGenReducedSet :: createRenderPlan ( vpp::CommandBuffer hCmdBuffer )
{
    using namespace vpp;

    CommandBufferRecorder recorder ( hCmdBuffer );
    recorder.compute ( *this );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void testcp ( const vpp::Device& hDevice )
{
    KGenReducedSet genReducedSet ( hDevice );

    vpp::Device hDev = genReducedSet.device();
    vpp::PipelineCache hCache = genReducedSet.pipelineCache();
    vpp::ComputePipeline hPipl = genReducedSet.pipeline ( 0 );
    VkPipelineCache hpc = hCache.handle();
}

// -----------------------------------------------------------------------------
} // namespace testComputePass
// -----------------------------------------------------------------------------

void testBuffers ( const vpp::Device& hDevice )
{
    using namespace vpp;

    std::vector< unsigned int > qfi;

    Buf nullBuf;
    Buf buf1 ( 200, Buf::UNIFORM | Buf::STORAGE, hDevice );

    Buf buf2 (
        200,
        Buf::SOURCE | Buf::TARGET | Buf::UNITEX | Buf::STORTEX | Buf::UNIFORM
        | Buf::STORAGE | Buf::INDEX | Buf::VERTEX | Buf::INDIRECT,
        hDevice, qfi );

    bool bv = buf1;
    vpp::Device hDev = buf1.device();
    VkBuffer hBuf1 = buf1.handle();
    VkDeviceSize s = buf1.size();
    unsigned int u = buf1.getUsage();

    const DeviceMemory hMem =
        buf1.bindMemory< DeviceMemory > ( MemProfile::DEVICE_STATIC );

    VkDeviceMemory memh = hMem.handle();
    bool memv = hMem.valid();
    unsigned int memp = hMem.properties();
    bool memhv = hMem.isHostVisible();
    bool memhc = hMem.isHostCoherent();

    typedef Buffer<
        Buf::SOURCE | Buf::TARGET | Buf::UNITEX | Buf::STORTEX | Buf::UNIFORM
        | Buf::STORAGE | Buf::INDEX | Buf::VERTEX | Buf::INDIRECT
    > SuperBuffer;

    typedef Buffer< Buf::SOURCE > SrcBuffer;

    SuperBuffer sbuf1 ( 200, hDevice );
    SuperBuffer sbuf2 ( 200, hDevice, 0 );
    SuperBuffer sbuf3 ( 200, hDevice, 0, 0 );
    SuperBuffer sbuf4 ( 200, hDevice, qfi );
    SuperBuffer sbuf5 ( 200, hDevice, qfi, 0 );
    SuperBuffer sbuf6 ( 200, hDevice, qfi, 0, 0 );
    SrcBuffer srcb1 = sbuf1;

    vpp::Device hDev1 = sbuf1.device();
    VkBuffer hBuf11 = sbuf1.handle();
    VkDeviceSize s1 = sbuf1.size();
    unsigned int u1 = sbuf1.getUsage();
}

// -----------------------------------------------------------------------------

void testImages ( const vpp::Device& hDevice )
{
    using namespace vpp;

    std::vector< unsigned int > qfi;

    Img nullimg;

    ImageInfo imgInfo1 (
        vpp::RENDER,
        vpp::IMG_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UINT,
        { 1024, 768, 1 },
        1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
        vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
    );

    Img img1 ( imgInfo1, MemProfile::DEVICE_STATIC, hDevice );
    Img img2 ( imgInfo1, MemProfile::DEVICE_STATIC, hDevice, VK_IMAGE_LAYOUT_GENERAL );

    Img img3 (
        imgInfo1, MemProfile::DEVICE_STATIC, hDevice,
        VK_IMAGE_LAYOUT_GENERAL, qfi );

    VkImage himg1 = img1.handle();
    Device hdev = img1.device();
    VkExtent3D ext = img1.extent();
    VkFormat fmt = img1.format();

    Img img4 ( imgInfo1, hDevice, himg1 );

    typedef format< unorm8_t, unorm8_t, unorm8_t, unorm8_t > KColorFormat;

    typedef ImageAttributes<
        KColorFormat, RENDER, IMG_TYPE_2D,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > KColorBufferAttr;

    typedef Image< KColorBufferAttr > KColorBufferImage;

    KColorBufferImage nullImage;

    KColorBufferImage image1 (
        { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice );

    KColorBufferImage image2 (
        { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u );

    KColorBufferImage image3 (
        { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u );

    KColorBufferImage image4 (
        { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u, VK_IMAGE_LAYOUT_GENERAL );

    KColorBufferImage image5 (
        { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u, VK_IMAGE_LAYOUT_GENERAL, qfi );

    KColorBufferImage image1f (
        VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice );

    KColorBufferImage image2f (
        VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u );

    KColorBufferImage image3f (
        VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u );

    KColorBufferImage image4f (
        VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u, VK_IMAGE_LAYOUT_GENERAL );

    KColorBufferImage image5f (
        VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, MemProfile::DEVICE_STATIC, hDevice, 1u, 1u, VK_IMAGE_LAYOUT_GENERAL, qfi );

    KColorBufferImage image1v ( { 1024, 768, 1 }, hDevice, himg1 );
    KColorBufferImage image2v ( { 1024, 768, 1 }, hDevice, himg1, 2u );
    KColorBufferImage image3v ( { 1024, 768, 1 }, hDevice, himg1, 3u, 4u );
    KColorBufferImage image4v ( VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, hDevice, himg1 );
    KColorBufferImage image5v ( VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, hDevice, himg1, 2u );
    KColorBufferImage image6v ( VK_FORMAT_R8G8B8A8_UINT, { 1024, 768, 1 }, hDevice, himg1, 3u, 4u );

    KColorBufferImage imageRaw = KColorBufferImage ( img1 );

    VkImage himg11 = image1v.handle();
    Device hdev1 = image1v.device();
    VkExtent3D ext1 = image1v.extent();
    VkFormat fmt1 = image1v.format();
}

// -----------------------------------------------------------------------------

void testGVector ( const vpp::Device& hDevice )
{
    using namespace vpp;
    typedef gvector< std::uint32_t, Buf::SOURCE | Buf::STORAGE > SSIVector;

    SSIVector ssivec ( 1024, MemProfile::DEVICE_STATIC, hDevice );

    for ( unsigned int i = 0; i != 1024; ++i )
        ssivec.push_back ( i );

    ssivec.clear();

    for ( unsigned int i = 0; i != 1024; ++i )
        ssivec.emplace_back ( i );

    unsigned int s = 0;

    for ( SSIVector::const_iterator i1 = ssivec.begin(); i1 != ssivec.end(); ++i1 )
        s += *i1;

    for ( SSIVector::const_iterator i2 = ssivec.cbegin(); i2 != ssivec.cend(); ++i2 )
        s += *i2;

    for ( SSIVector::iterator i3 = ssivec.begin(); i3 != ssivec.end(); ++i3 )
        *i3 += *i3;

    bool be = ssivec.empty();
    size_t ss = ssivec.size();
    size_t sc = ssivec.capacity();

    *ssivec.allocate_back() = 4;

    for ( size_t i4 = 0; i4 != ss; ++i4 )
        ssivec [ i4 ] += 5;

    CommandBuffer hCmdBuffer;
    Fence fence1 ( hDevice );
    Semaphore sem1 ( hDevice );
    Semaphore sem2 ( hDevice );

    ssivec.cmdCommit ( hCmdBuffer );
    ssivec.cmdCommitAll ( hCmdBuffer );
    ssivec.cmdCommit();
    ssivec.cmdCommitAll();
    ssivec.cmdLoad ( hCmdBuffer );
    ssivec.cmdLoad ();
    ssivec.cmdCommit ( hCmdBuffer, 1 );
    ssivec.cmdCommitAll ( hCmdBuffer, 1 );
    ssivec.cmdCommit ( 1 );
    ssivec.cmdCommitAll ( 1 );
    ssivec.cmdLoad ( hCmdBuffer, 1 );
    ssivec.cmdLoad ( 1 );
    ssivec.cmdCommit ( hCmdBuffer, 1, 5 );
    ssivec.cmdCommitAll ( hCmdBuffer, 1, 5 );
    ssivec.cmdCommit ( 1, 5 );
    ssivec.cmdCommitAll ( 1, 5 );
    ssivec.cmdLoad ( hCmdBuffer, 1, 5 );
    ssivec.cmdLoad ( 1, 5 );
    ssivec.commit();
    ssivec.commitAndWait();
    ssivec.load();
    ssivec.loadAndWait();
    ssivec.commit ( Q_TRANSFER );
    ssivec.commitAndWait ( Q_TRANSFER );
    ssivec.load ( Q_TRANSFER );
    ssivec.loadAndWait ( Q_TRANSFER );
    ssivec.commit ( Q_TRANSFER, fence1 );
    ssivec.load ( Q_TRANSFER, fence1 );
    ssivec.commit ( Q_TRANSFER, fence1, sem1 );
    ssivec.load ( Q_TRANSFER, fence1, sem1 );
    ssivec.commit ( Q_TRANSFER, fence1, sem1, sem2 );
    ssivec.load ( Q_TRANSFER, fence1, sem1, sem2 );

    ImageInfo imgInfo1 (
        vpp::RENDER,
        vpp::IMG_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UINT,
        { 1024, 768, 1 },
        1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
        vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
    );

    Img img1 ( imgInfo1, MemProfile::DEVICE_STATIC, hDevice );

    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1 );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1 );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2 );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2 );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 } );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 } );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
}

// -----------------------------------------------------------------------------

void testOtherContainers ( const vpp::Device& hDevice )
{
    using namespace vpp;

    IndirectCommands idc1 ( 300, MemProfile::DEVICE_STATIC, hDevice );
    idc1.push_back ( VkDrawIndirectCommand { 1, 2, 3, 4 } );
    VkDrawIndirectCommand& dic = idc1 [ 0 ];

    IndexedIndirectCommands iidc1 ( 300, MemProfile::DEVICE_STATIC, hDevice );
    iidc1.push_back ( VkDrawIndexedIndirectCommand { 1, 2, 3, 4, 5 } );
    VkDrawIndexedIndirectCommand& diic = iidc1 [ 0 ];
    
    DispatchIndirectCommands didc1 ( 300, MemProfile::DEVICE_STATIC, hDevice );
    didc1.push_back ( VkDispatchIndirectCommand { 1, 2, 3 } );
    VkDispatchIndirectCommand& didc = didc1 [ 0 ];

    Indices ind1 ( 300, MemProfile::DEVICE_STATIC, hDevice );
    ind1.push_back ( 7 );
    unsigned int& ind = ind1 [ 0 ];

    typedef dgvector< std::uint32_t, Buf::SOURCE | Buf::STORAGE > SSIVector;

    SSIVector ssivec2 ( 2048, hDevice );

    ImageInfo imgInfo1 (
        vpp::RENDER,
        vpp::IMG_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UINT,
        { 1024, 768, 1 },
        1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
        vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
    );

    Img img1 ( imgInfo1, MemProfile::DEVICE_STATIC, hDevice );

    CommandBuffer hCmdBuffer;
    Fence fence1 ( hDevice );
    Semaphore sem1 ( hDevice );
    Semaphore sem2 ( hDevice );

    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1 );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1 );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1 );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2 );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2 );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2 );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 } );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 } );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 } );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 } );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3 );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4 );
    ssivec2.cmdCopyToImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.cmdCopyToImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.copyToImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.copyToImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.cmdCopyFromImage ( hCmdBuffer, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.cmdCopyFromImage ( img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.copyFromImage ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, fence1, sem1, sem2, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
    ssivec2.copyFromImageAndWait ( Q_TRANSFER, img1, VK_IMAGE_LAYOUT_GENERAL, 1, 2, { 1, 1, 1 }, { 1, 1, 1 }, 3, 4, 5 );
}

// -----------------------------------------------------------------------------
namespace testPipelineConfig {
// -----------------------------------------------------------------------------

template< vpp::ETag TAG = CPU >
struct TVertexPos : public vpp::VertexStruct< TAG, TVertexPos >
{
    vpp::Attribute< TAG, float, float, float, float > d_modelPosition;
};

typedef TVertexPos< vpp::CPU > KVertexPos;
typedef TVertexPos< vpp::GPU > GVertexPos;

// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TFrameSelector : public vpp::UniformStruct< TAG, TFrameSelector >
{
    typedef vpp::format< std::int32_t, std::int32_t, std::int32_t, std::int32_t > FmtIntParams;
    vpp::UniformFld< TAG, FmtIntParams > d_frameSelectorParams;
};

typedef TFrameSelector< vpp::CPU > CFrameSelector;
typedef TFrameSelector< vpp::GPU > GFrameSelector;

// -----------------------------------------------------------------------------

typedef vpp::format< std::uint32_t > KFormatUInt32;
typedef vpp::TexelBufferView< KFormatUInt32, vpp::Buf::UNITEX > KTexelInBufferView;
typedef vpp::TexelBufferView< KFormatUInt32, vpp::Buf::STORTEX > KTexelIoBufferView;

// -----------------------------------------------------------------------------

typedef vpp::ImageAttributes<
    KFormatUInt32, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    false, true > KVarConstU32ImageAttr;

typedef vpp::Image< KVarConstU32ImageAttr > KVarConstU32Image;
typedef vpp::ImageViewAttributes< KVarConstU32Image > KVarConstU32ViewAttr;
typedef vpp::ImageView< KVarConstU32ViewAttr > KVarConstU32View;

// -----------------------------------------------------------------------------

typedef vpp::format< vpp::texture_format > KTextureFormat;

typedef vpp::ImageAttributes<
    KTextureFormat, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    true, false > KStdTextureAttr;

typedef vpp::Image< KStdTextureAttr > KStdTextureImage;
typedef vpp::ImageViewAttributes< KStdTextureImage > KStdTextureViewAttr;
typedef vpp::ImageView< KStdTextureViewAttr > KStdTextureView;

// -----------------------------------------------------------------------------

typedef vpp::format< float, vpp::DT > KDepthFormat;

typedef vpp::ImageAttributes<
    KDepthFormat, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    false, false > KDepthTextureAttr;

typedef vpp::Image< KDepthTextureAttr > KDepthTextureImage;
typedef vpp::ImageViewAttributes< KDepthTextureImage > KDepthTextureViewAttr;
typedef vpp::ImageView< KDepthTextureViewAttr > KDepthTextureView;

// -----------------------------------------------------------------------------

struct GStdPipelineData : public vpp::LocalStruct< GStdPipelineData >
{
    vpp::Field< vpp::Vec4 > d_worldVertex;
    vpp::Field< vpp::Vec4 > d_worldNormal;
    vpp::Field< vpp::Vec4 > d_worldTangent;

    vpp::Field< vpp::Vec4 > d_textureUV01;
    vpp::Field< vpp::Vec4 > d_textureUV23;
    vpp::Field< vpp::Vec4 > d_textureUV45;
    vpp::Field< vpp::Vec4 > d_textureUV67;

    // x: instance id, rest - unused
    vpp::Field< vpp::IVec4 > d_parameters;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class TestPipelineConfig : public vpp::PipelineConfig
{
public:
    TestPipelineConfig (
        const vpp::Process& pr, 
        const vpp::Device& dev, 
        const vpp::Attachment< KFormatUInt32 >& inAtt,
        const vpp::Attachment< KFormatUInt32 >& outAtt );

    void fVertexShader ( vpp::VertexShader* pShader, int arg );
    void fTessControlShader ( vpp::TessControlShader* pShader, int arg );
    void fTessEvalShader ( vpp::TessEvalShader* pShader, int arg );
    void fGeometryShader ( vpp::GeometryShader* pShader, int arg );
    void fFragmentShader ( vpp::FragmentShader* pShader, int arg );

    void setBuffers (
        const vpp::UniformBufferView& inUniformBuffer,
        const KVarConstU32View& imageView,
        const KStdTextureView& textureView,
        const KTexelInBufferView& texbufView1,
        const KTexelIoBufferView& texbufView2,
        vpp::ShaderDataBlock* sdb
    );

    void bindVertices (
        const vpp::VertexBufferView& vert,
        const vpp::VertexIndexBufferView& ind );

private:
    vpp::UnnormalizedSampler d_usamplerData;
    vpp::NormalizedSampler d_samplerData;

    vpp::inVertexData< TVertexPos > d_inVertexData;
    vpp::inUniformBuffer d_inUniformBuffer;
    vpp::inUniformBufferDyn d_inUniformBufferDyn;
    vpp::ioBuffer d_ioBuffer;
    vpp::ioBufferDyn d_ioBufferDyn;
    vpp::ioImageBuffer< KFormatUInt32 > d_ioImageBuffer;
    vpp::inTextureBuffer< KFormatUInt32 > d_inTextureBuffer;
    vpp::inPushConstant< TFrameSelector > d_inPushConstant;
    vpp::ioImage< KVarConstU32View > d_ioImage;
    vpp::inSampler< vpp::NormalizedSampler > d_inSampler;
    vpp::inConstSampler< vpp::UnnormalizedSampler > d_inConstSampler;
    vpp::inTexture< KVarConstU32View > d_inTexture;
    vpp::inSampledTexture< KStdTextureView > d_inSampledTexture;
    vpp::inConstSampledTexture< KStdTextureView > d_inConstSampledTexture;
    vpp::inAttachment< KVarConstU32View > d_inAttachment;
    vpp::outAttachment< KFormatUInt32 > d_outAttachment;
    vpp::ioVariable< vpp::Vec4, vpp::vertexShader, vpp::geometryShader > d_ioVariable;
    vpp::ioStructure< GStdPipelineData, vpp::vertexShader, vpp::geometryShader > d_ioStructure;

    vpp::inSampledTexture< KDepthTextureView > d_inSampledDepthTexture;
    vpp::inConstSampledTexture< KDepthTextureView > d_inConstSampledDepthTexture;

    vpp::arrayOf< vpp::inUniformBuffer > d_inUniformBufferArr;
    vpp::arrayOf< vpp::inUniformBufferDyn > d_inUniformBufferDynArr;
    vpp::arrayOf< vpp::ioBuffer > d_ioBufferArr;
    vpp::arrayOf< vpp::ioBufferDyn > d_ioBufferDynArr;
    vpp::arrayOf< vpp::ioImageBuffer< KFormatUInt32 > > d_ioImageBufferArr;
    vpp::arrayOf< vpp::inTextureBuffer< KFormatUInt32 > > d_inTextureBufferArr;
    vpp::arrayOf< vpp::ioImage< KVarConstU32View > > d_ioImageArr;
    vpp::arrayOf< vpp::inSampler< vpp::NormalizedSampler > > d_inSamplerArr;
    vpp::arrayOf< vpp::inConstSampler< vpp::UnnormalizedSampler > > d_inConstSamplerArr;
    vpp::arrayOf< vpp::inTexture< KVarConstU32View > > d_inTextureArr;
    vpp::arrayOf< vpp::inSampledTexture< KStdTextureView > > d_inSampledTextureArr;
    vpp::arrayOf< vpp::inConstSampledTexture< KStdTextureView > > d_inConstSampledTextureArr;
    vpp::arrayOf< vpp::inSampledTexture< KDepthTextureView > > d_inSampledDepthTextureArr;
    vpp::arrayOf< vpp::inConstSampledTexture< KDepthTextureView > > d_inConstSampledDepthTextureArr;

    vpp::vertexShader d_vertexShader;
    vpp::tessControlShader d_tessControlShader;
    vpp::tessEvalShader d_tessEvalShader;
    vpp::geometryShader d_geometryShader;
    vpp::fragmentShader d_fragmentShader;

    vpp::ioVariable< vpp::Vec4, vpp::vertexShader, vpp::tessControlShader > d_vtx2tc;
    vpp::ioVariable< vpp::Vec4, vpp::tessControlShader, vpp::tessEvalShader > d_tc2te;
    vpp::ioVariable< vpp::Vec4, vpp::tessEvalShader, vpp::geometryShader > d_te2geom;
    vpp::ioVariable< vpp::Vec4, vpp::geometryShader, vpp::fragmentShader > d_geom2fr;

    vpp::ioStructure< GStdPipelineData, vpp::vertexShader, vpp::tessControlShader > d_svtx2tc;
    vpp::ioStructure< GStdPipelineData, vpp::tessControlShader, vpp::tessEvalShader > d_stc2te;
    vpp::ioStructure< GStdPipelineData, vpp::tessEvalShader, vpp::geometryShader > d_ste2geom;
    vpp::ioStructure< GStdPipelineData, vpp::geometryShader, vpp::fragmentShader > d_sgeom2fr;
};

// -----------------------------------------------------------------------------

TestPipelineConfig :: TestPipelineConfig (
    const vpp::Process& pr,
    const vpp::Device& dev, 
    const vpp::Attachment< KFormatUInt32 >& inAtt,
    const vpp::Attachment< KFormatUInt32 >& outAtt ) :
        vpp::PipelineConfig ( pr ),
        d_inConstSampler ( d_usamplerData ),
        d_inConstSampledTexture ( d_samplerData ),
        d_inConstSampledDepthTexture ( d_samplerData ),
        d_inAttachment ( inAtt ),
        d_outAttachment ( outAtt ),
        d_inUniformBufferArr ( 5u ),
        d_inUniformBufferDynArr ( 5u ),
        d_ioBufferArr ( 5u ),
        d_ioBufferDynArr ( 5u ),
        d_ioImageBufferArr ( 5u ),
        d_inTextureBufferArr ( 5u ),
        d_ioImageArr ( 5u ),
        d_inSamplerArr ( 5u ),
        d_inConstSamplerArr ( std::vector< vpp::UnnormalizedSampler >( 1, d_usamplerData ), 5u ),
        d_inTextureArr ( 5u ),
        d_inSampledTextureArr ( 5u ),
        d_inConstSampledTextureArr ( std::vector< vpp::NormalizedSampler >( 1, d_samplerData ), 5u ),
        d_inSampledDepthTextureArr ( 5u ),
        d_inConstSampledDepthTextureArr ( std::vector< vpp::NormalizedSampler >( 1, d_samplerData ), 5u ),
        d_vertexShader ( this, & TestPipelineConfig::fVertexShader, 5 ),
        d_geometryShader ( this, & TestPipelineConfig::fGeometryShader, 5 ),
        d_tessControlShader ( this, & TestPipelineConfig::fTessControlShader, 5 ),
        d_tessEvalShader ( d_tessControlShader, this, & TestPipelineConfig::fTessEvalShader, 5 ),
        d_fragmentShader ( this, & TestPipelineConfig::fFragmentShader, 5 )
{
    VkPipelineColorBlendAttachmentState blendState;

    setBlendingMode ( outAtt, blendState );
    enableLogicOperation ( true, VK_LOGIC_OP_COPY );
    setPrimitiveTopology ( POINT_LIST );
    setEnablePrimitiveRestart ( false );
    setTessPatchControlPoints ( 17u );
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: setBuffers (
    const vpp::UniformBufferView& inUniformBuffer,
    const KVarConstU32View& imageView,
    const KStdTextureView& textureView,
    const KTexelInBufferView& texbufView1,
    const KTexelIoBufferView& texbufView2,
    vpp::ShaderDataBlock* sdb )
{
    sdb->update ( (
        d_inUniformBuffer = inUniformBuffer,
        d_inUniformBufferArr = vpp::multi ( inUniformBuffer, 2 ),
        d_inUniformBufferArr = vpp::multi ( { inUniformBuffer, inUniformBuffer }, 1 ),
        d_inUniformBufferArr = vpp::multi ( { inUniformBuffer, inUniformBuffer } ),
        d_inUniformBufferDyn = inUniformBuffer,
        d_inUniformBufferDynArr = vpp::multi ( inUniformBuffer, 2 ),
        d_inUniformBufferDynArr = vpp::multi ( { inUniformBuffer, inUniformBuffer }, 1 ),
        d_inUniformBufferDynArr = vpp::multi ( { inUniformBuffer, inUniformBuffer } )
    ) );

    sdb->update ( (
        d_ioBuffer = inUniformBuffer,
        d_ioBufferArr = vpp::multi ( inUniformBuffer, 2 ),
        d_ioBufferArr = vpp::multi ( { inUniformBuffer, inUniformBuffer }, 1 ),
        d_ioBufferArr = vpp::multi ( { inUniformBuffer, inUniformBuffer } ),
        d_ioBufferDyn = inUniformBuffer,
        d_ioBufferDynArr = vpp::multi ( inUniformBuffer, 2 ),
        d_ioBufferDynArr = vpp::multi ( { inUniformBuffer, inUniformBuffer }, 1 ),
        d_ioBufferDynArr = vpp::multi ( { inUniformBuffer, inUniformBuffer } )
    ) );

    sdb->update ( (
        d_ioImage = imageView,
        d_inTexture = imageView
    ) );

    sdb->update ( (
        d_inAttachment = imageView
    ) );

    sdb->update ( (
        d_ioImageArr = vpp::multi ( vpp::bind ( imageView ), 2 ),
        d_ioImageArr = vpp::multi ( { vpp::bind ( imageView ), vpp::bind ( imageView ) }, 1 ),
        d_ioImageArr = vpp::multi ( { vpp::bind ( imageView ), vpp::bind ( imageView ) } )
    ) );

    sdb->update ( (
        d_inTextureArr = vpp::multi ( vpp::bind ( imageView ), 2 ),
        d_inTextureArr = vpp::multi ( { vpp::bind ( imageView ), vpp::bind ( imageView ) }, 1 ),
        d_inTextureArr = vpp::multi ( { vpp::bind ( imageView ), vpp::bind ( imageView ) } )
    ) );

    sdb->update ( (
        d_inSampledTexture = vpp::bind ( d_samplerData, textureView ),
        d_inConstSampledTexture = textureView
    ) );

    sdb->update ( (
        d_inSampledTextureArr = vpp::multi ( vpp::bind ( d_samplerData, textureView ), 2 ),
        d_inSampledTextureArr = vpp::multi ( { vpp::bind ( d_samplerData, textureView ), vpp::bind ( d_samplerData, textureView ) }, 1 ),
        d_inSampledTextureArr = vpp::multi ( { vpp::bind ( d_samplerData, textureView ), vpp::bind ( d_samplerData, textureView ) } )
    ) );

    sdb->update ( (
        d_inConstSampledTextureArr = vpp::multi ( vpp::bind ( textureView ), 2 ),
        d_inConstSampledTextureArr = vpp::multi ( { vpp::bind ( textureView ), vpp::bind ( textureView ) }, 1 ),
        d_inConstSampledTextureArr = vpp::multi ( { vpp::bind ( textureView ), vpp::bind ( textureView ) } )
    ) );

    sdb->update ( (
        d_inSampler = d_samplerData,
        d_inSamplerArr = vpp::multi ( d_samplerData, 2 ),
        d_inSamplerArr = vpp::multi ( { d_samplerData, d_samplerData }, 1 ),
        d_inSamplerArr = vpp::multi ( { d_samplerData, d_samplerData } )
    ) );

    sdb->update ( (
        d_ioImageBuffer = texbufView2,
        d_ioImageBufferArr = vpp::multi ( texbufView2, 2 ),
        d_ioImageBufferArr = vpp::multi ( { texbufView2, texbufView2 }, 1 ),
        d_ioImageBufferArr = vpp::multi ( { texbufView2, texbufView2 } )
    ) );

    sdb->update ( (
        d_inTextureBuffer = texbufView1,
        d_inTextureBufferArr = vpp::multi ( texbufView1, 2 ),
        d_inTextureBufferArr = vpp::multi ( { texbufView1, texbufView1 }, 1 ),
        d_inTextureBufferArr = vpp::multi ( { texbufView1, texbufView1 } )
    ) );
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: bindVertices (
    const vpp::VertexBufferView& vert,
    const vpp::VertexIndexBufferView& ind )
{
    cmdBindVertexInput (( d_inVertexData = vert ));
    cmdBindIndexInput ( ind );
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: fVertexShader ( vpp::VertexShader* pShader, int arg )
{
    using namespace vpp;

    pShader->DebugCodeDump();

    const Int vi = pShader->inVertexIndex;
    const Int ii = pShader->inInstanceIndex;

    pShader->outVertex.position = Vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
    pShader->outVertex.pointSize = 0.6f;

    pShader->outVertex.setClipPlanes ( 2 );
    pShader->outVertex.setCullPlanes ( 2 );

    pShader->outVertex.clipDistance [ 0 ] = 0.52f;
    pShader->outVertex.clipDistance [ 1 ] = 0.72f;

    pShader->outVertex.cullDistance [ 0 ] = 0.52f;
    pShader->outVertex.cullDistance [ 1 ] = 0.72f;

    const int cp1 = pShader->outVertex.getClipPlanes();
    const int cp2 = pShader->outVertex.getCullPlanes();

    Vec4 v { 1.0f, 2.0f, 3.0f, 4.0f };

    Output< decltype ( d_vtx2tc ) > vtx2tc ( d_vtx2tc );
    Output< decltype ( d_svtx2tc ) > svtx2tc ( d_svtx2tc );

    vtx2tc = v;
    svtx2tc [ & GStdPipelineData::d_worldVertex ] = v;

    Vec4 vpos = d_inVertexData [ & GVertexPos::d_modelPosition ];

    vpp::UniformVar< TFrameSelector, decltype ( d_inUniformBuffer ) >
        varUniformBuffer ( d_inUniformBuffer );
    vpp::UniformVar< TFrameSelector, decltype ( d_inUniformBufferDyn ) >
        varUniformBufferDyn ( d_inUniformBufferDyn );
    vpp::UniformVar< TFrameSelector, decltype ( d_ioBuffer ) >
        varIoBuffer ( d_ioBuffer );
    vpp::UniformVar< TFrameSelector, decltype ( d_ioBufferDyn ) >
        varIoBufferDyn ( d_ioBufferDyn );
    vpp::UniformVar< TFrameSelector, decltype ( d_inPushConstant ) >
        varPushConstant ( d_inPushConstant );

    vpp::UniformVar< TFrameSelector, decltype ( d_inUniformBufferArr ) >
        varUniformBufferArr ( d_inUniformBufferArr );
    vpp::UniformVar< TFrameSelector, decltype ( d_inUniformBufferDynArr ) >
        varUniformBufferDynArr ( d_inUniformBufferDynArr );
    vpp::UniformVar< TFrameSelector, decltype ( d_ioBufferArr ) >
        varIoBufferArr ( d_ioBufferArr );
    vpp::UniformVar< TFrameSelector, decltype ( d_ioBufferDynArr ) >
        varIoBufferDynArr ( d_ioBufferDynArr );

    IVec4 p1 = varUniformBuffer [ & GFrameSelector::d_frameSelectorParams ];
    Int p1x = varUniformBuffer [ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 p2 = varUniformBufferDyn [ & GFrameSelector::d_frameSelectorParams ];
    Int p2x = varUniformBufferDyn [ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 p3 = varIoBuffer [ & GFrameSelector::d_frameSelectorParams ];
    Int p3x = varIoBuffer [ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 p4 = varIoBufferDyn [ & GFrameSelector::d_frameSelectorParams ];
    Int p4x = varIoBufferDyn [ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 p5 = varPushConstant [ & GFrameSelector::d_frameSelectorParams ];
    Int p5x = varPushConstant [ & GFrameSelector::d_frameSelectorParams ][ X ];
    varIoBuffer [ & GFrameSelector::d_frameSelectorParams ] = p3;
    varIoBufferDyn [ & GFrameSelector::d_frameSelectorParams ] = p4;

    Int idx = 0;

    IVec4 q1 = varUniformBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ];
    Int q1x = varUniformBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 q2 = varUniformBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ];
    Int q2x = varUniformBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 q3 = varIoBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ];
    Int q3x = varIoBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ][ X ];
    IVec4 q4 = varIoBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ];
    Int q4x = varIoBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ][ X ];
    varIoBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ] = q3;
    varIoBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ] = q4;
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: fTessControlShader ( vpp::TessControlShader* pShader, int arg )
{
    using namespace vpp;

    pShader->DebugCodeDump();

    Int idx = pShader->inInvocationId;
    Int nv = pShader->inPatchVertices;
    Int pid = pShader->inPrimitiveId;

    const Vec4 pos = pShader->inVertices [ 0 ].position;
    const Float pts = pShader->inVertices [ idx ].pointSize;

    const Float c1 = pShader->inVertices [ idx ].clipDistance [ idx ];
    const Float c2 = pShader->inVertices [ idx ].cullDistance [ 0u ];
    const Float c3 = pShader->inVertices [ 1 ].clipDistance [ 1u ];
    const Float c4 = pShader->inVertices [ 2u ].cullDistance [ idx ];
    
    pShader->outVertices [ idx ].position = Vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
    pShader->outVertices [ 0 ].pointSize = 0.6f;

    pShader->outVertices.setClipPlanes ( 2 );
    pShader->outVertices.setCullPlanes ( 2 );

    pShader->outVertices [ idx ].clipDistance [ 0 ] = 0.52f;
    pShader->outVertices [ 0 ].clipDistance [ 1 ] = 0.72f;

    pShader->outVertices [ 0 ].cullDistance [ 0 ] = 0.52f;
    pShader->outVertices [ idx ].cullDistance [ 1 ] = 0.72f;

    const int cp1 = pShader->outVertices.getClipPlanes();
    const int cp2 = pShader->outVertices.getCullPlanes();
    const int cp3 = pShader->inVertices.getClipPlanes();
    const int cp4 = pShader->inVertices.getCullPlanes();

    pShader->outTessLevelOuter [ idx ] = 12.6f;
    pShader->outTessLevelOuter [ 0 ] = Float ( 12.6f );
    pShader->outTessLevelOuter [ 1u ] = c1;

    pShader->outTessLevelInner [ idx ] = 12.6f;
    pShader->outTessLevelInner [ 0 ] = Float ( 12.6f );
    pShader->outTessLevelInner [ 1u ] = c1;

    Input< decltype ( d_vtx2tc ) > vtx2tc ( d_vtx2tc );
    Input< decltype ( d_svtx2tc ) > svtx2tc ( d_svtx2tc );
    Vec4 vio = vtx2tc [ cp1 ];
    Vec4 vio2 = svtx2tc [ cp1 ][ & GStdPipelineData::d_worldVertex ];

    Output< decltype ( d_tc2te ) > tc2te ( d_tc2te );
    Output< decltype ( d_stc2te ) > stc2te ( d_stc2te );
    tc2te [ cp1 ] = vio;
    stc2te [ cp1 ][ & GStdPipelineData::d_worldVertex ] = vio2;
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: fTessEvalShader ( vpp::TessEvalShader* pShader, int arg )
{
    using namespace vpp;

    pShader->DebugCodeDump();

    Int nv = pShader->inPatchVertices;
    Int idx = pShader->inPrimitiveId;
    Vec3 coords = pShader->inTessCoord;

    const Vec4 pos = pShader->inVertices [ 0 ].position;
    const Float pts = pShader->inVertices [ idx ].pointSize;

    const Float c1 = pShader->inVertices [ idx ].clipDistance [ idx ];
    const Float c2 = pShader->inVertices [ idx ].cullDistance [ 0u ];
    const Float c3 = pShader->inVertices [ 1 ].clipDistance [ 1u ];
    const Float c4 = pShader->inVertices [ 2u ].cullDistance [ idx ];

    Float l1 = pShader->inTessLevelOuter [ idx ];
    Float l2 = pShader->inTessLevelOuter [ 0 ];
    Float l3 = pShader->inTessLevelOuter [ 1u ];
    Float l4 = pShader->inTessLevelInner [ idx ];
    Float l5 = pShader->inTessLevelInner [ 0 ];
    Float l6 = pShader->inTessLevelInner [ 1u ];

    pShader->outVertex.position = Vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
    pShader->outVertex.pointSize = 0.6f;

    pShader->outVertex.setClipPlanes ( 2 );
    pShader->outVertex.setCullPlanes ( 2 );

    pShader->outVertex.clipDistance [ 0 ] = 0.52f;
    pShader->outVertex.clipDistance [ 1 ] = 0.72f;

    pShader->outVertex.cullDistance [ 0 ] = 0.52f;
    pShader->outVertex.cullDistance [ 1 ] = 0.72f;

    const int cp1 = pShader->outVertex.getClipPlanes();
    const int cp2 = pShader->outVertex.getCullPlanes();

    Input< decltype ( d_tc2te ) > tc2te ( d_tc2te );
    Input< decltype ( d_stc2te ) > stc2te ( d_stc2te );
    Vec4 vio = tc2te [ cp1 ];
    Vec4 vio2 = stc2te [ cp1 ][ & GStdPipelineData::d_worldVertex ];

    Output< decltype ( d_te2geom ) > te2geom ( d_te2geom );
    Output< decltype ( d_ste2geom ) > ste2geom ( d_ste2geom );
    te2geom = vio;
    ste2geom [ & GStdPipelineData::d_worldVertex ] = vio2;
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: fGeometryShader ( vpp::GeometryShader* pShader, int arg )
{
    using namespace vpp;

    pShader->DebugCodeDump();
    pShader->EmitVertex();
    pShader->EndPrimitive();

    Int idx = pShader->inInvocationId;
    Int pid = pShader->inPrimitiveId;

    const Vec4 pos = pShader->inVertices [ 0 ].position;
    const Float pts = pShader->inVertices [ idx ].pointSize;

    const Float c1 = pShader->inVertices [ idx ].clipDistance [ idx ];
    const Float c2 = pShader->inVertices [ idx ].cullDistance [ 0u ];
    const Float c3 = pShader->inVertices [ 1 ].clipDistance [ 1u ];
    const Float c4 = pShader->inVertices [ 2u ].cullDistance [ idx ];

    pShader->outVertex.position = Vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
    pShader->outVertex.pointSize = 0.6f;

    pShader->outVertex.setClipPlanes ( 2 );
    pShader->outVertex.setCullPlanes ( 2 );

    pShader->outVertex.clipDistance [ 0 ] = 0.52f;
    pShader->outVertex.clipDistance [ 1 ] = 0.72f;

    pShader->outVertex.cullDistance [ 0 ] = 0.52f;
    pShader->outVertex.cullDistance [ 1 ] = 0.72f;

    const int cp1 = pShader->outVertex.getClipPlanes();
    const int cp2 = pShader->outVertex.getCullPlanes();

    pShader->outLayer = pid;
    pShader->outPrimitiveId = 6;
    pShader->outViewportIndex = pid;

    Input< decltype ( d_te2geom ) > te2geom ( d_te2geom );
    Input< decltype ( d_ste2geom ) > ste2geom ( d_ste2geom );
    Vec4 vio = te2geom [ cp1 ];
    Vec4 vio2 = ste2geom [ cp1 ][ & GStdPipelineData::d_worldVertex ];

    Output< decltype ( d_geom2fr ) > geom2fr ( d_geom2fr );
    Output< decltype ( d_sgeom2fr ) > sgeom2fr ( d_sgeom2fr );
    geom2fr = vio;
    sgeom2fr [ & GStdPipelineData::d_worldVertex ] = vio2;
}

// -----------------------------------------------------------------------------

void TestPipelineConfig :: fFragmentShader ( vpp::FragmentShader* pShader, int arg )
{
    using namespace vpp;

    pShader->DebugCodeDump();
    pShader->Kill();
    pShader->EarlyTest();

    pShader->DebugProbe ( 15 );
    pShader->DebugProbe ( 15u );
    pShader->DebugProbe ( 15.0f );
    pShader->DebugProbe ( Int(15) );
    pShader->DebugProbe ( UInt(15u) );
    pShader->DebugProbe ( Float(15.0f) );
    pShader->DebugProbe ( IVec4(15,15,15,15) );
    pShader->DebugProbe ( UVec4(15u,15u,15u,15u) );
    pShader->DebugProbe ( Vec4(15.0f,15.0f,15.0f,15.0f) );

    Vec4 fc = pShader->inFragCoord;
    Bool ff = pShader->inFrontFacing;
    Int l = pShader->inLayer;
    Int si = pShader->inSampleId;
    Vec2 sp = pShader->inSamplePosition;
    Vec2 pc = pShader->inPointCoord;
    Bool hi = pShader->inHelperInvocation;
    Int pi = pShader->inPrimitiveId;
    Int vi = pShader->inViewportIndex;
    
    pShader->outFragDepth = Float ( 77.8f );

    IVec2 cc { 66, 77 };
    VkExtent3D ex { 66, 77, 88 };

    pShader->DebugProbe ( 15, cc, ex );
    pShader->DebugProbe ( 15u, cc, ex );
    pShader->DebugProbe ( 15.0f, cc, ex );
    pShader->DebugProbe ( Int(15), cc, ex );
    pShader->DebugProbe ( UInt(15u), cc, ex );
    pShader->DebugProbe ( Float(15.0f), cc, ex );
    pShader->DebugProbe ( IVec4(15,15,15,15), cc, ex );
    pShader->DebugProbe ( UVec4(15u,15u,15u,15u), cc, ex );
    pShader->DebugProbe ( Vec4(15.0f,15.0f,15.0f,15.0f), cc, ex );

    Input< decltype ( d_geom2fr ) > geom2fr ( d_geom2fr );
    Input< decltype ( d_sgeom2fr ) > sgeom2fr ( d_sgeom2fr );
    Vec4 vio = geom2fr;
    Vec4 vio2 = sgeom2fr [ & GStdPipelineData::d_worldVertex ];

    Int n = 0;
    Int arrIdx = 2;
    Vec2 c = { 0.5f, 0.5f };
    Vec3 d = { 0.5f, 0.5f, 0.0f };
    Vec4 e = { 0.5f, 0.5f, 0.0f, 1.0f };
    IVec2 i = { 7, 8 };
    IVec3 ci = { 7, 8, 0 };
    Float b = 0.0f;

    const auto st1 = MakeSampledTexture ( d_inTexture, d_inSampler );
    const auto st2 = MakeSampledTexture ( d_inTextureArr [ arrIdx ], d_inSamplerArr [ arrIdx ] );
    const auto st3 = MakeSampledTexture ( d_inTextureArr [ arrIdx ], d_inSampler );
    const auto st4 = MakeSampledTexture ( d_inTexture, d_inSamplerArr [ arrIdx ] );
    const auto est1 = ExtractSampledTexture ( st1 );

    IVec3 s11 = ImageSize ( d_ioImage );
    IVec3 s12 = ImageSize ( d_inTexture );
    IVec2 s13 = ImageSize ( d_inSampledTexture );
    IVec2 s14 = ImageSize ( d_inConstSampledTexture );
    IVec3 s16 = ImageSize ( d_ioImageArr [ arrIdx ] );
    IVec3 s17 = ImageSize ( d_inTextureArr [ arrIdx ] );
    IVec2 s18 = ImageSize ( d_inSampledTextureArr [ arrIdx ] );
    IVec2 s19 = ImageSize ( d_inConstSampledTextureArr [ arrIdx ] );
    IVec3 s1a = TextureSize ( d_inTexture, n );
    IVec2 s1b = TextureSize ( d_inSampledTexture, n );
    IVec2 s1c = TextureSize ( d_inConstSampledTexture, n );
    IVec3 s1d = TextureSize ( d_inTextureArr [ arrIdx ], n );
    IVec2 s1e = TextureSize ( d_inSampledTextureArr [ arrIdx ], n );
    IVec2 s1f = TextureSize ( d_inConstSampledTextureArr [ arrIdx ], n );

    Int s21 = ImageQuerySamples ( d_ioImage );
    Int s22 = ImageQuerySamples ( d_inTexture );
    Int s23 = ImageQuerySamples ( d_inSampledTexture );
    Int s24 = ImageQuerySamples ( d_inConstSampledTexture );
    Int s26 = ImageQuerySamples ( d_ioImageArr [ arrIdx ] );
    Int s27 = ImageQuerySamples ( d_inTextureArr [ arrIdx ] );
    Int s28 = ImageQuerySamples ( d_inSampledTextureArr [ arrIdx ] );
    Int s29 = ImageQuerySamples ( d_inConstSampledTextureArr [ arrIdx ] );

    Int s31 = ImageQueryLevels ( d_ioImage );
    Int s32 = ImageQueryLevels ( d_inTexture );
    Int s33 = ImageQueryLevels ( d_inSampledTexture );
    Int s34 = ImageQueryLevels ( d_inConstSampledTexture );
    Int s36 = ImageQueryLevels ( d_ioImageArr [ arrIdx ] );
    Int s37 = ImageQueryLevels ( d_inTextureArr [ arrIdx ] );
    Int s38 = ImageQueryLevels ( d_inSampledTextureArr [ arrIdx ] );
    Int s39 = ImageQueryLevels ( d_inConstSampledTextureArr [ arrIdx ] );

    Vec2 s43 = TextureQueryLod ( d_inSampledTexture, c );
    Vec2 s44 = TextureQueryLod ( d_inConstSampledTexture, c );
    Vec2 s48 = TextureQueryLod ( d_inSampledTextureArr [ arrIdx ], c );
    Vec2 s49 = TextureQueryLod ( d_inConstSampledTextureArr [ arrIdx ], c );

    UVec4 s51 = SubpassLoad ( d_inAttachment );
    UVec4 s53 = SubpassLoad ( d_inAttachment, ci );

    UVec4 s61 = ImageLoad ( d_ioImage, ci );
    UVec4 s66 = ImageLoad ( d_ioImageArr [ arrIdx ], ci );

    ImageStore ( d_ioImage, ci, s61 [ X ] );
    ImageStore ( d_ioImageArr [ arrIdx ], ci, s61 [ X ] );

    UVec4 s71 = TexelFetch ( d_inTexture, ci );
    Vec4 s72 = TexelFetch ( d_inSampledTexture, i );
    Vec4 s73 = TexelFetch ( d_inConstSampledTexture, i );
    UVec4 s74 = TexelFetch ( d_inTextureBuffer, n );
    UVec4 s75 = TexelFetch ( d_inTextureArr [ arrIdx ], ci );
    Vec4 s76 = TexelFetch ( d_inSampledTextureArr [ arrIdx ], i );
    Vec4 s77 = TexelFetch ( d_inConstSampledTextureArr [ arrIdx ], i );
    UVec4 s78 = TexelFetch ( d_inTextureBufferArr [ arrIdx ], n );

    UVec4 s81 = TexelFetchLod ( d_inTexture, ci, n );
    Vec4 s82 = TexelFetchLod ( d_inSampledTexture, i, n );
    Vec4 s83 = TexelFetchLod ( d_inConstSampledTexture, i, n );
    UVec4 s84 = TexelFetchLod ( d_inTextureBuffer, n, n );
    UVec4 s85 = TexelFetchLod ( d_inTextureArr [ arrIdx ], ci, n );
    Vec4 s86 = TexelFetchLod ( d_inSampledTextureArr [ arrIdx ], i, n );
    Vec4 s87 = TexelFetchLod ( d_inConstSampledTextureArr [ arrIdx ], i, n );
    UVec4 s88 = TexelFetchLod ( d_inTextureBufferArr [ arrIdx ], n, n );

    UVec4 s91 = TexelFetchOffset ( d_inTexture, ci, i );
    Vec4 s92 = TexelFetchOffset ( d_inSampledTexture, i, i );
    Vec4 s93 = TexelFetchOffset ( d_inConstSampledTexture, i, i );
    UVec4 s94 = TexelFetchOffset ( d_inTextureBuffer, n, n );
    UVec4 s95 = TexelFetchOffset ( d_inTextureArr [ arrIdx ], ci, i );
    Vec4 s96 = TexelFetchOffset ( d_inSampledTextureArr [ arrIdx ], i, i );
    Vec4 s97 = TexelFetchOffset ( d_inConstSampledTextureArr [ arrIdx ], i, i );
    UVec4 s98 = TexelFetchOffset ( d_inTextureBufferArr [ arrIdx ], n, n );

    Vec4 s101 = Texture ( d_inSampledTexture, c );
    Vec4 s102 = Texture ( d_inConstSampledTexture, c );
    Vec4 s103 = Texture ( d_inSampledTextureArr [ arrIdx ], c );
    Vec4 s104 = Texture ( d_inConstSampledTextureArr [ arrIdx ], c );
    Vec4 s105 = Texture ( d_inSampledTexture, c, b );
    Vec4 s106 = Texture ( d_inConstSampledTexture, c, b );
    Vec4 s107 = Texture ( d_inSampledTextureArr [ arrIdx ], c, b );
    Vec4 s108 = Texture ( d_inConstSampledTextureArr [ arrIdx ], c, b );
    Vec4 s109 = Texture ( d_inSampledTexture, c, c, c );
    Vec4 s10a = Texture ( d_inConstSampledTexture, c, c, c );
    Vec4 s10b = Texture ( d_inSampledTextureArr [ arrIdx ], c, c, c );
    Vec4 s10c = Texture ( d_inConstSampledTextureArr [ arrIdx ], c, c, c );
    Vec4 s10d = Texture ( d_inSampledTexture, c, i );
    Vec4 s10e = Texture ( d_inConstSampledTexture, c, i );
    Vec4 s10f = Texture ( d_inSampledTextureArr [ arrIdx ], c, i );
    Vec4 s10g = Texture ( d_inConstSampledTextureArr [ arrIdx ], c, i );
    Vec4 s10h = Texture ( d_inSampledTexture, c, i, b );
    Vec4 s10i = Texture ( d_inConstSampledTexture, c, i, b );
    Vec4 s10j = Texture ( d_inSampledTextureArr [ arrIdx ], c, i, b );
    Vec4 s10k = Texture ( d_inConstSampledTextureArr [ arrIdx ], c, i, b );
    Vec4 s10l = Texture ( d_inSampledTexture, c, i, c, c );
    Vec4 s10m = Texture ( d_inConstSampledTexture, c, i, c, c );
    Vec4 s10n = Texture ( d_inSampledTextureArr [ arrIdx ], c, i, c, c );
    Vec4 s10o = Texture ( d_inConstSampledTextureArr [ arrIdx ], c, i, c, c );

    Vec4 s111 = TextureLod ( d_inSampledTexture, c, b );
    Vec4 s112 = TextureLod ( d_inConstSampledTexture, c, b );
    Vec4 s113 = TextureLod ( d_inSampledTextureArr [ arrIdx ], c, b );
    Vec4 s114 = TextureLod ( d_inConstSampledTextureArr [ arrIdx ], c, b );
    Vec4 s115 = TextureLod ( d_inSampledTexture, c, i, b );
    Vec4 s116 = TextureLod ( d_inConstSampledTexture, c, i, b );
    Vec4 s117 = TextureLod ( d_inSampledTextureArr [ arrIdx ], c, i, b );
    Vec4 s118 = TextureLod ( d_inConstSampledTextureArr [ arrIdx ], c, i, b );

    Vec4 s121 = TextureProj ( d_inSampledTexture, d );
    Vec4 s122 = TextureProj ( d_inConstSampledTexture, d );
    Vec4 s123 = TextureProj ( d_inSampledTextureArr [ arrIdx ], d );
    Vec4 s124 = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d );
    Vec4 s125 = TextureProj ( d_inSampledTexture, d, b );
    Vec4 s126 = TextureProj ( d_inConstSampledTexture, d, b );
    Vec4 s127 = TextureProj ( d_inSampledTextureArr [ arrIdx ], d, b );
    Vec4 s128 = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d, b );
    Vec4 s129 = TextureProj ( d_inSampledTexture, d, c, c );
    Vec4 s12a = TextureProj ( d_inConstSampledTexture, d, c, c );
    Vec4 s12b = TextureProj ( d_inSampledTextureArr [ arrIdx ], d, c, c );
    Vec4 s12c = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d, c, c );
    Vec4 s12d = TextureProj ( d_inSampledTexture, d, i );
    Vec4 s12e = TextureProj ( d_inConstSampledTexture, d, i );
    Vec4 s12f = TextureProj ( d_inSampledTextureArr [ arrIdx ], d, i );
    Vec4 s12g = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d, i );
    Vec4 s12h = TextureProj ( d_inSampledTexture, d, i, b );
    Vec4 s12i = TextureProj ( d_inConstSampledTexture, d, i, b );
    Vec4 s12j = TextureProj ( d_inSampledTextureArr [ arrIdx ], d, i, b );
    Vec4 s12k = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d, i, b );
    Vec4 s12l = TextureProj ( d_inSampledTexture, d, i, c, c );
    Vec4 s12m = TextureProj ( d_inConstSampledTexture, d, i, c, c );
    Vec4 s12n = TextureProj ( d_inSampledTextureArr [ arrIdx ], d, i, c, c );
    Vec4 s12o = TextureProj ( d_inConstSampledTextureArr [ arrIdx ], d, i, c, c );

    Vec4 s131 = TextureLodProj ( d_inSampledTexture, d, b );
    Vec4 s132 = TextureLodProj ( d_inConstSampledTexture, d, b );
    Vec4 s133 = TextureLodProj ( d_inSampledTextureArr [ arrIdx ], d, b );
    Vec4 s134 = TextureLodProj ( d_inConstSampledTextureArr [ arrIdx ], d, b );
    Vec4 s135 = TextureLodProj ( d_inSampledTexture, d, i, b );
    Vec4 s136 = TextureLodProj ( d_inConstSampledTexture, d, i, b );
    Vec4 s137 = TextureLodProj ( d_inSampledTextureArr [ arrIdx ], d, i, b );
    Vec4 s138 = TextureLodProj ( d_inConstSampledTextureArr [ arrIdx ], d, i, b );

    Float s141 = TextureDref ( d_inSampledDepthTexture, c, b );
    Float s142 = TextureDref ( d_inConstSampledDepthTexture, c, b );
    Float s143 = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b );
    Float s144 = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b );
    Float s145 = TextureDref ( d_inSampledDepthTexture, c, b, b );
    Float s146 = TextureDref ( d_inConstSampledDepthTexture, c, b, b );
    Float s147 = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b, b );
    Float s148 = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b, b );
    Float s149 = TextureDref ( d_inSampledDepthTexture, c, c, c, b );
    Float s14a = TextureDref ( d_inConstSampledDepthTexture, c, c, c, b );
    Float s14b = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, c, c, b );
    Float s14c = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, c, c, b );
    Float s14d = TextureDref ( d_inSampledDepthTexture, c, i, b );
    Float s14e = TextureDref ( d_inConstSampledDepthTexture, c, i, b );
    Float s14f = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, i, b );
    Float s14g = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, i, b );
    Float s14h = TextureDref ( d_inSampledDepthTexture, c, i, b, b );
    Float s14i = TextureDref ( d_inConstSampledDepthTexture, c, i, b, b );
    Float s14j = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, i, b, b );
    Float s14k = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, i, b, b );
    Float s14l = TextureDref ( d_inSampledDepthTexture, c, i, c, c, b );
    Float s14m = TextureDref ( d_inConstSampledDepthTexture, c, i, c, c, b );
    Float s14n = TextureDref ( d_inSampledDepthTextureArr [ arrIdx ], c, i, c, c, b );
    Float s14o = TextureDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, i, c, c, b );

    Float s151 = TextureLodDref ( d_inSampledDepthTexture, c, b, b );
    Float s152 = TextureLodDref ( d_inConstSampledDepthTexture, c, b, b );
    Float s153 = TextureLodDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b, b );
    Float s154 = TextureLodDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b, b );
    Float s155 = TextureLodDref ( d_inSampledDepthTexture, c, i, b, b );
    Float s156 = TextureLodDref ( d_inConstSampledDepthTexture, c, i, b, b );
    Float s157 = TextureLodDref ( d_inSampledDepthTextureArr [ arrIdx ], c, i, b, b );
    Float s158 = TextureLodDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, i, b, b );

    Float s161 = TextureProjDref ( d_inSampledDepthTexture, d, b );
    Float s162 = TextureProjDref ( d_inConstSampledDepthTexture, d, b );
    Float s163 = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, b );
    Float s164 = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, b );
    Float s165 = TextureProjDref ( d_inSampledDepthTexture, d, b, b );
    Float s166 = TextureProjDref ( d_inConstSampledDepthTexture, d, b, b );
    Float s167 = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, b, b );
    Float s168 = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, b, b );
    Float s169 = TextureProjDref ( d_inSampledDepthTexture, d, c, c, b );
    Float s16a = TextureProjDref ( d_inConstSampledDepthTexture, d, c, c, b );
    Float s16b = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, c, c, b );
    Float s16c = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, c, c, b );
    Float s16d = TextureProjDref ( d_inSampledDepthTexture, d, i, b );
    Float s16e = TextureProjDref ( d_inConstSampledDepthTexture, d, i, b );
    Float s16f = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, i, b );
    Float s16g = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, i, b );
    Float s16h = TextureProjDref ( d_inSampledDepthTexture, d, i, b, b );
    Float s16i = TextureProjDref ( d_inConstSampledDepthTexture, d, i, b, b );
    Float s16j = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, i, b, b );
    Float s16k = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, i, b, b );
    Float s16l = TextureProjDref ( d_inSampledDepthTexture, d, i, c, c, b );
    Float s16m = TextureProjDref ( d_inConstSampledDepthTexture, d, i, c, c, b );
    Float s16n = TextureProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, i, c, c, b );
    Float s16o = TextureProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, i, c, c, b );

    Float s171 = TextureLodProjDref ( d_inSampledDepthTexture, d, b, b );
    Float s172 = TextureLodProjDref ( d_inConstSampledDepthTexture, d, b, b );
    Float s173 = TextureLodProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, b, b );
    Float s174 = TextureLodProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, b, b );
    Float s175 = TextureLodProjDref ( d_inSampledDepthTexture, d, i, b, b );
    Float s176 = TextureLodProjDref ( d_inConstSampledDepthTexture, d, i, b, b );
    Float s177 = TextureLodProjDref ( d_inSampledDepthTextureArr [ arrIdx ], d, i, b, b );
    Float s178 = TextureLodProjDref ( d_inConstSampledDepthTextureArr [ arrIdx ], d, i, b, b );

    Vec4 s181 = TextureGather ( d_inSampledTexture, c, n );
    Vec4 s182 = TextureGather ( d_inConstSampledTexture, c, n );
    Vec4 s183 = TextureGather ( d_inSampledTextureArr [ arrIdx ], c, n );
    Vec4 s184 = TextureGather ( d_inConstSampledTextureArr [ arrIdx ], c, n );
    Vec4 s185 = TextureGather ( d_inSampledTexture, c, n, i );
    Vec4 s186 = TextureGather ( d_inConstSampledTexture, c, n, i );
    Vec4 s187 = TextureGather ( d_inSampledTextureArr [ arrIdx ], c, n, i );
    Vec4 s188 = TextureGather ( d_inConstSampledTextureArr [ arrIdx ], c, n, i );

    GatherOffsets go;
    Vec4 s189 = TextureGather ( d_inSampledTexture, c, n, go );
    Vec4 s18a = TextureGather ( d_inConstSampledTexture, c, n, go );
    Vec4 s18b = TextureGather ( d_inSampledTextureArr [ arrIdx ], c, n, go );
    Vec4 s18c = TextureGather ( d_inConstSampledTextureArr [ arrIdx ], c, n, go );

    Vec4 s191 = TextureGatherDref ( d_inSampledDepthTexture, c, b );
    Vec4 s192 = TextureGatherDref ( d_inConstSampledDepthTexture, c, b );
    Vec4 s193 = TextureGatherDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b );
    Vec4 s194 = TextureGatherDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b );
    Vec4 s195 = TextureGatherDref ( d_inSampledDepthTexture, c, b, i );
    Vec4 s196 = TextureGatherDref ( d_inConstSampledDepthTexture, c, b, i );
    Vec4 s197 = TextureGatherDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b, i );
    Vec4 s198 = TextureGatherDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b, i );
    Vec4 s199 = TextureGatherDref ( d_inSampledDepthTexture, c, b, go );
    Vec4 s19a = TextureGatherDref ( d_inConstSampledDepthTexture, c, b, go );
    Vec4 s19b = TextureGatherDref ( d_inSampledDepthTextureArr [ arrIdx ], c, b, go );
    Vec4 s19c = TextureGatherDref ( d_inConstSampledDepthTextureArr [ arrIdx ], c, b, go );
}

// -----------------------------------------------------------------------------

typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > ColorFormat;

// -----------------------------------------------------------------------------

class TestRenderGraph : public vpp::RenderGraph
{
public:
    TestRenderGraph (
        unsigned int width, unsigned int height,
        vpp::Device hDevice  );

public:
    vpp::Device m_hDevice;
    vpp::Process m_render;
    vpp::Attachment< ColorFormat > m_colorAttachmentBuffer;
    vpp::Attachment< KFormatUInt32 > m_att1;
    vpp::Attachment< KFormatUInt32 > m_att2;
};

// -----------------------------------------------------------------------------

TestRenderGraph :: TestRenderGraph (
    unsigned int width, unsigned int height,
    vpp::Device hDevice ) :
        m_hDevice ( hDevice ),
        m_colorAttachmentBuffer ( width, height ),
        m_att1 ( width, height ),
        m_att2 ( width, height )
{
    m_render.addColorOutput (
        m_colorAttachmentBuffer, { 0, 0, 0, 0 },
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
}

// -----------------------------------------------------------------------------

class TestRenderer
{
public:
    TestRenderer ( vpp::Device hDevice );

private:
    vpp::Device m_hDevice;
    VkExtent3D m_imageSize;

    std::shared_ptr< TestRenderGraph > m_renderGraph;
    vpp::PipelineLayout< TestPipelineConfig > m_renderPipeline;
    vpp::ShaderDataBlock m_dataBlock;
    vpp::RenderPass m_renderPass;

    vpp::RenderingOptions m_renderOptions;
    vpp::FrameBuffer m_frameBuffer;
};

// -----------------------------------------------------------------------------

TestRenderer :: TestRenderer ( vpp::Device hDevice ) :
    m_hDevice ( hDevice ),
    m_renderGraph ( new TestRenderGraph (
        m_imageSize.width, m_imageSize.height, hDevice ) ),
    m_renderPipeline (
        m_renderGraph->m_render, hDevice,
        m_renderGraph->m_att1, m_renderGraph->m_att2
    ),
    m_dataBlock ( m_renderPipeline ),
    m_renderPass ( *m_renderGraph, hDevice ),
    m_frameBuffer (
        std::vector< vpp::FrameImageView >(),
        m_renderPass )
{
    using namespace vpp;

    VkPipelineLayout lt = m_renderPipeline.handle();
    vpp::Device hDev = m_renderPipeline.device();

    typedef gvector< std::uint32_t, Buf::UNIFORM | Buf::STORAGE > Vector;

    Vector v1 ( 100, MemProfile::DEVICE_STATIC, hDevice );
    
    KVarConstU32View* pFakeImageView = 0;
    KStdTextureView* pFakeTextureView = 0;
    KTexelInBufferView* pFakeTexelInBufferView = 0;
    KTexelIoBufferView* pFakeTexelIoBufferView = 0;

    m_renderPipeline.definition().setBuffers (
        v1,
        *pFakeImageView, *pFakeTextureView,
        *pFakeTexelInBufferView, *pFakeTexelIoBufferView,
        & m_dataBlock
    );
}

// -----------------------------------------------------------------------------
} // namespace testPipelineConfig
// -----------------------------------------------------------------------------

void testCommandBuffer()
{
    vpp::CommandBuffer emptyCmdBuf;
    VkCommandBuffer hCmdBuf = 0;
    vpp::CommandBuffer cmdBuf ( hCmdBuf );

    const bool bv = cmdBuf;
    VkCommandBuffer hCmdBuf2 = cmdBuf.handle();

    VkResult r1 = cmdBuf.reset();
    VkResult r2 = cmdBuf.release();
    VkResult r3 = cmdBuf.begin();
    VkResult r4a = cmdBuf.begin ( vpp::CommandBuffer::ONE_TIME_SUBMIT );
    VkResult r4b = cmdBuf.begin ( vpp::CommandBuffer::RENDER_PASS_CONTINUE );
    VkResult r4c = cmdBuf.begin ( vpp::CommandBuffer::SIMULTANEOUS_USE  );
    VkResult r5 = cmdBuf.end();
}

// -----------------------------------------------------------------------------

void testCommandPool ( const vpp::Device& hDevice )
{
    using namespace vpp;

    CommandPool cp1 ( hDevice, Q_GRAPHICS );
    CommandPool cp2 ( hDevice, Q_TRANSFER );

    const bool bv = cp1.valid();
    VkResult r1 = cp1.reset();
    VkResult r2 = cp1.release();
    VkCommandPool hp = cp1.handle();

    CommandBuffer b1 = cp1.createBuffer();
    CommandBuffer b2 = cp1.createBuffer ( CommandPool::PRIMARY );
    CommandBuffer b3 = cp1.createBuffer ( CommandPool::SECONDARY );

    cp1.freeBuffer ( b1 );

    std::vector< CommandBuffer > bufs;
    VkResult r3 = cp1.createBuffers ( 5, & bufs );
    VkResult r4 = cp1.createBuffers ( 5, & bufs, CommandPool::SECONDARY );

    cp1.freeBuffers ( bufs );
}

// -----------------------------------------------------------------------------
