// -----------------------------------------------------------------------------

#pragma warning (disable: 4503)
#include <vppAll.hpp>

// -----------------------------------------------------------------------------
namespace vpptest {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace {
// -----------------------------------------------------------------------------

void check ( bool bCondition )
{
    if ( ! bCondition )
        std::cerr << "Check failed" << std::endl;
}

// -----------------------------------------------------------------------------

inline bool compareFloats ( float a, float b )
{
    static const float s_eps = 0.00001f;
    return std::abs ( a - b ) <= s_eps;
}

// -----------------------------------------------------------------------------
} // namespace
// -----------------------------------------------------------------------------

class AttTestTypes 
{
public:
    typedef vpp::format< float, float, float, float > format_type;
    typedef typename format_type::data_type data_type;
    typedef vpp::gvector< data_type, vpp::Buf::SOURCE | vpp::Buf::TARGET > buffer_type;

    typedef vpp::ImageAttributes<
        format_type, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::STORAGE | vpp::Img::SAMPLED | vpp::Img::TARGET,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > StgAttrType;

    typedef vpp::Image< StgAttrType > StgImgType;
    typedef vpp::ImageViewAttributes< StgImgType > StgVaType;
    typedef vpp::ImageView< StgVaType > StgViewType;

    typedef vpp::ImageAttributes<
        format_type, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::COLOR | vpp::Img::INPUT | vpp::Img::SOURCE,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > AttAttrType;

    typedef vpp::Image< AttAttrType > AttImgType;
    typedef vpp::ImageViewAttributes< AttImgType > AttVaType;
    typedef vpp::ImageView< AttVaType > AttViewType;

    static const unsigned int WIDTH = 1024;
    static const unsigned int HEIGHT = 1024;
    static const unsigned int LENGTH = WIDTH * HEIGHT;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class AttTestGraph :
    public vpp::RenderGraph,
    public AttTestTypes
{
public:
    AttTestGraph (
        const AttViewType& intBufView,
        const AttViewType& finalBufView );

public:
    vpp::Preprocess d_init;
    vpp::Postprocess d_fini;
    vpp::Process d_render1;
    vpp::Process d_render2;
    vpp::Attachment< format_type > d_intBuffer;
    vpp::Attachment< format_type > d_finalBuffer;
};

// -----------------------------------------------------------------------------

AttTestGraph :: AttTestGraph (
    const AttViewType& intBufView,
    const AttViewType& finalBufView ) :
        d_intBuffer ( intBufView ),
        d_finalBuffer ( finalBufView )
{
    d_render1.addColorOutput ( d_intBuffer, { 0.0f, 0.0f, 0.0f, 0.0f } );
    d_render2.addColorOutput ( d_finalBuffer, { 0.0f, 0.0f, 0.0f, 0.0f } );
    d_render2.addInput ( d_intBuffer );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class AttTestPipeline1 :
    public vpp::WholeScreenPatchPipelineConfig,
    public AttTestTypes
{
public:
    AttTestPipeline1 (
        const vpp::Process& pr,
        const vpp::Device& dev,
        const vpp::Attachment< format_type >& output );

    void setDataBuffers (
        const StgViewType& sourceView,
        vpp::ShaderDataBlock* pDataBlock );

    void fFragmentShader ( vpp::FragmentShader* pShader );

private:
    vpp::inTexture< StgViewType > d_sourceImage;
    vpp::outAttachment< format_type > d_outColor;
    vpp::fragmentShader d_fragmentShader;
};

// -----------------------------------------------------------------------------

AttTestPipeline1 :: AttTestPipeline1 (
    const vpp::Process& pr,
    const vpp::Device& dev,
    const vpp::Attachment< format_type >& output ) :
        vpp::WholeScreenPatchPipelineConfig ( pr, dev ),
        d_sourceImage ( dev ),
        d_outColor ( output ),
        d_fragmentShader ( this, & AttTestPipeline1::fFragmentShader )
{
}

// -----------------------------------------------------------------------------

void AttTestPipeline1 :: setDataBuffers (
    const StgViewType& sourceView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_sourceImage = sourceView
    ));
}

// -----------------------------------------------------------------------------

void AttTestPipeline1 :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    const IVec2 coords = StaticCast< IVec2 >( pShader->inFragCoord [ XY ] );
    const Vec4 pix = TexelFetch ( d_sourceImage, coords );
    d_outColor = pix;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class AttTestPipeline2 :
    public vpp::WholeScreenPatchPipelineConfig,
    public AttTestTypes
{
public:
    AttTestPipeline2 (
        const vpp::Process& pr,
        const vpp::Device& dev,
        const vpp::Attachment< format_type >& input,
        const vpp::Attachment< format_type >& output );

    void setDataBuffers (
        const AttViewType& inputView,
        vpp::ShaderDataBlock* pDataBlock );

    void fFragmentShader ( vpp::FragmentShader* pShader );

private:
    vpp::inAttachment< AttViewType > d_inColor;
    vpp::outAttachment< format_type > d_outColor;
    vpp::fragmentShader d_fragmentShader;
};

// -----------------------------------------------------------------------------

AttTestPipeline2 :: AttTestPipeline2 (
    const vpp::Process& pr,
    const vpp::Device& dev,
    const vpp::Attachment< format_type >& input,
    const vpp::Attachment< format_type >& output ) :
        vpp::WholeScreenPatchPipelineConfig ( pr, dev ),
        d_inColor ( input ),
        d_outColor ( output ),
        d_fragmentShader ( this, & AttTestPipeline2::fFragmentShader )
{
}

// -----------------------------------------------------------------------------

void AttTestPipeline2 :: setDataBuffers (
    const AttViewType& inputView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_inColor = inputView
    ));
}

// -----------------------------------------------------------------------------

void AttTestPipeline2 :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    const Vec4 pix = SubpassLoad ( d_inColor );
    d_outColor = pix;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class AttTest : public AttTestTypes
{
public:
    AttTest ( const vpp::Device& hDevice );

    void operator()();

private:
    void prepareVectors();
    void compareResults();

private:
    vpp::Device d_device;

    AttImgType d_intBufImg;
    AttViewType d_intBufView;

    AttImgType d_finalBufImg;
    AttViewType d_finalBufView;

    StgImgType d_sourceImage;
    StgViewType d_sourceView;

    buffer_type d_sourceBuffer;
    buffer_type d_targetBuffer;

    AttTestGraph d_graph;
    vpp::RenderPass d_pass;
    vpp::FrameBuffer d_frameBuffer;
    vpp::PipelineLayout< AttTestPipeline1 > d_pipeline1;
    vpp::PipelineLayout< AttTestPipeline2 > d_pipeline2;
    vpp::ShaderDataBlock d_sdb1;
    vpp::ShaderDataBlock d_sdb2;
};

// -----------------------------------------------------------------------------

AttTest :: AttTest ( const vpp::Device& hDevice ) :
    d_device ( hDevice ),
    d_intBufImg ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_intBufView ( d_intBufImg ),
    d_finalBufImg ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_finalBufView ( d_finalBufImg ),
    d_sourceImage ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceView ( d_sourceImage ),
    d_sourceBuffer ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetBuffer ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_graph ( d_intBufView, d_finalBufView ),
    d_pass ( d_graph, d_device ),
    d_frameBuffer ( d_pass ),
    d_pipeline1 ( d_graph.d_render1, hDevice, d_graph.d_intBuffer ),
    d_pipeline2 ( d_graph.d_render2, hDevice, d_graph.d_intBuffer, d_graph.d_finalBuffer ),
    d_sdb1 ( d_pipeline1 ),
    d_sdb2 ( d_pipeline2 )
{
    using namespace vpp;

    RenderingOptions opt1;
    RenderingOptions opt2;

    d_pipeline1.definition().configureRenderingOptions ( WIDTH, HEIGHT, & opt1 );
    d_pipeline1.definition().configureRenderingOptions ( WIDTH, HEIGHT, & opt2 );

    d_pass.addPipeline ( d_graph.d_render1, d_pipeline1, opt1 );
    d_pass.addPipeline ( d_graph.d_render2, d_pipeline2, opt2 );

    d_pipeline1.definition().setDataBuffers ( d_sourceView, & d_sdb1 );
    d_pipeline2.definition().setDataBuffers ( d_intBufView, & d_sdb2 );

    d_graph.d_init << [ this ]()
    {
        d_sourceBuffer.cmdCopyToImage (
            d_sourceImage,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );
    };

    d_graph.d_render1 << [ this ]()
    {
        d_pass.pipeline ( 0, 0 ).cmdBind();
        d_sdb1.cmdBind();
        d_pipeline1.definition().cmdDrawWholeScreenPatch();
    };

    d_graph.d_render2 << [ this ]()
    {
        d_pass.pipeline ( 1, 0 ).cmdBind();
        d_sdb2.cmdBind();
        d_pipeline2.definition().cmdDrawWholeScreenPatch();
    };

    d_graph.d_fini << [ this ]()
    {
        d_targetBuffer.cmdCopyFromImage (
            d_finalBufImg,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );
    };

    prepareVectors();
}

// -----------------------------------------------------------------------------

void AttTest :: operator()()
{
    using namespace vpp;

    CommandBuffer cmdBuffer = d_device.defaultCmdPool().createBuffer();
    CommandBufferRecorder recorder ( cmdBuffer );

    recorder.render ( d_pass, d_frameBuffer );

    Queue q ( d_device, Q_GRAPHICS );
    q.submit ( cmdBuffer );

    q.waitForIdle();
    compareResults();
}

// -----------------------------------------------------------------------------

void AttTest :: prepareVectors()
{
    d_sourceBuffer.resize ( LENGTH );
    d_targetBuffer.resize ( LENGTH );

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        const float b = float ( k );
        d_sourceBuffer [ k ] = data_type ( b, b + 0.25f, b + 0.5f, b + 0.75f );
        d_targetBuffer [ k ] = data_type ( 0.0f, 0.0f, 0.0f, 0.0f );
    }
}

// -----------------------------------------------------------------------------

void AttTest :: compareResults()
{
    using namespace vpp;

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        const data_type s1 = d_sourceBuffer [ k ];
        const data_type t1 = d_targetBuffer [ k ];
        check ( s1 == t1 );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class SampTestTypes
{
public:
    typedef vpp::format< float, float, float, float > FmtType;

    typedef typename FmtType::data_type DataType;
    typedef vpp::gvector< DataType, vpp::Buf::SOURCE | vpp::Buf::TARGET > BufferType;

    typedef vpp::ImageAttributes<
        FmtType, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::COLOR | vpp::Img::SOURCE,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > OutputAttr;

    typedef vpp::Image< OutputAttr > OutputImage;
    typedef vpp::ImageViewAttributes< OutputImage > OutputViewAttr;
    typedef vpp::ImageView< OutputViewAttr > OutputView;

    typedef vpp::ImageAttributes<
        FmtType, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::SAMPLED | vpp::Img::TARGET,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > TextureAttr;

    typedef vpp::Image< TextureAttr > TextureImage;
    typedef vpp::ImageViewAttributes< TextureImage, vpp::SAMPLER_UNNORMALIZED > TextureViewAttr;
    typedef vpp::ImageView< TextureViewAttr > TextureView;

    static const unsigned int WIDTH = 1024;
    static const unsigned int HEIGHT = 1024;
    static const unsigned int LENGTH = WIDTH * HEIGHT;
};

// -----------------------------------------------------------------------------

class SampTestGraph :
    public vpp::RenderGraph,
    public SampTestTypes
{
public:
    SampTestGraph (
        const OutputView& outView1,
        const OutputView& outView2,
        const OutputView& outView3,
        const OutputView& outView4
    );

public:
    vpp::Preprocess d_init;
    vpp::Postprocess d_fini;
    vpp::Process d_render;
    vpp::Attachment< FmtType > d_output1;
    vpp::Attachment< FmtType > d_output2;
    vpp::Attachment< FmtType > d_output3;
    vpp::Attachment< FmtType > d_output4;
};

// -----------------------------------------------------------------------------

SampTestGraph :: SampTestGraph (
    const OutputView& outView1,
    const OutputView& outView2,
    const OutputView& outView3,
    const OutputView& outView4 ) :
        d_output1 ( outView1 ),
        d_output2 ( outView2 ),
        d_output3 ( outView3 ),
        d_output4 ( outView4 )
{
    d_render.addColorOutput ( d_output1, { 0.0f, 0.0f, 0.0f, 0.0f } );
    d_render.addColorOutput ( d_output2, { 0.0f, 0.0f, 0.0f, 0.0f } );
    d_render.addColorOutput ( d_output3, { 0.0f, 0.0f, 0.0f, 0.0f } );
    d_render.addColorOutput ( d_output4, { 0.0f, 0.0f, 0.0f, 0.0f } );
}

// -----------------------------------------------------------------------------

class SampTestPipeline :
    public vpp::WholeScreenPatchPipelineConfig,
    public SampTestTypes
{
public:
    SampTestPipeline (
        const vpp::Process& pr,
        const vpp::Device& dev,
        const vpp::Attachment< FmtType >& output1,
        const vpp::Attachment< FmtType >& output2,
        const vpp::Attachment< FmtType >& output3,
        const vpp::Attachment< FmtType >& output4,
        const vpp::NormalizedSampler& cnsamp,
        const vpp::UnnormalizedSampler& cusamp );

    void setBuffers (
        const TextureView& tv,
        const vpp::NormalizedSampler& nsamp,
        const vpp::UnnormalizedSampler& usamp,
        vpp::ShaderDataBlock* pDataBlock );

    void fFragmentShader ( vpp::FragmentShader* pShader );

private:
    vpp::inSampler< vpp::NormalizedSampler > d_normSampler;
    vpp::inSampler< vpp::UnnormalizedSampler > d_unnormSampler;
    vpp::inConstSampler< vpp::NormalizedSampler > d_cnormSampler;
    vpp::inConstSampler< vpp::UnnormalizedSampler > d_cunnormSampler;

    vpp::inTexture< TextureView > d_texture;
    vpp::outAttachment< FmtType > d_output1;
    vpp::outAttachment< FmtType > d_output2;
    vpp::outAttachment< FmtType > d_output3;
    vpp::outAttachment< FmtType > d_output4;
    vpp::fragmentShader d_fragmentShader;
};

// -----------------------------------------------------------------------------

SampTestPipeline :: SampTestPipeline (
    const vpp::Process& pr,
    const vpp::Device& dev,
    const vpp::Attachment< FmtType >& output1,
    const vpp::Attachment< FmtType >& output2,
    const vpp::Attachment< FmtType >& output3,
    const vpp::Attachment< FmtType >& output4,
    const vpp::NormalizedSampler& cnsamp,
    const vpp::UnnormalizedSampler& cusamp ) :
        vpp::WholeScreenPatchPipelineConfig ( pr, dev ),
        d_cnormSampler ( cnsamp ),
        d_cunnormSampler ( cusamp ),
        d_output1 ( output1 ),
        d_output2 ( output2 ),
        d_output3 ( output3 ),
        d_output4 ( output4 ),
        d_fragmentShader ( this, & SampTestPipeline::fFragmentShader )
{
}

// -----------------------------------------------------------------------------

void SampTestPipeline :: setBuffers (
    const TextureView& tv,
    const vpp::NormalizedSampler& nsamp,
    const vpp::UnnormalizedSampler& usamp,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_texture = tv,
        d_normSampler = nsamp,
        d_unnormSampler = usamp
    ));
}

// -----------------------------------------------------------------------------

void SampTestPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
{
    using namespace vpp;

    const Vec2 coords = pShader->inFragCoord [ XY ];
    
    const auto normSampledTexture = MakeSampledTexture ( d_texture, d_normSampler );
    const auto unnormSampledTexture = MakeSampledTexture ( d_texture, d_unnormSampler );
    const auto cnormSampledTexture = MakeSampledTexture ( d_texture, d_cnormSampler );
    const auto cunnormSampledTexture = MakeSampledTexture ( d_texture, d_cunnormSampler );

    const Vec4 pixValue1 = Texture ( unnormSampledTexture, coords );
    d_output1 = pixValue1;

    const Vec2 is = StaticCast< Vec2 >( ImageSize ( d_texture ) );
    const Vec2 scaledCoords = coords / is;

    const Vec4 pixValue2 = Texture ( normSampledTexture, scaledCoords );
    d_output2 = pixValue2;

    const Vec4 pixValue3 = Texture ( cunnormSampledTexture, coords );
    d_output3 = pixValue3;

    const Vec4 pixValue4 = Texture ( cnormSampledTexture, scaledCoords );
    d_output4 = pixValue4;
}

// -----------------------------------------------------------------------------

class SampTest : public SampTestTypes
{
public:
    SampTest ( const vpp::Device& dev );

    void operator()();

    void prepareVectors();
    void compareResults();

private:
    vpp::Device d_device;

    BufferType d_textureSourceBuffer;
    BufferType d_finalResultBuffer1;
    BufferType d_finalResultBuffer2;
    BufferType d_finalResultBuffer3;
    BufferType d_finalResultBuffer4;

    TextureImage d_texture;
    TextureView d_textureView;

    OutputImage d_outputImage1;
    OutputView d_outputView1;

    OutputImage d_outputImage2;
    OutputView d_outputView2;

    OutputImage d_outputImage3;
    OutputView d_outputView3;

    OutputImage d_outputImage4;
    OutputView d_outputView4;

    vpp::SUnnormalizedSampler d_unnormalizedSamplerDef;
    vpp::UnnormalizedSampler d_unnormalizedSampler;

    vpp::SNormalizedSampler d_normalizedSamplerDef;
    vpp::NormalizedSampler d_normalizedSampler;

    SampTestGraph d_graph;

    vpp::PipelineLayout< SampTestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    vpp::RenderingOptions d_options;

    vpp::RenderPass d_pass;
    vpp::FrameBuffer d_frameBuffer;
};

// -----------------------------------------------------------------------------

SampTest :: SampTest ( const vpp::Device& dev ) :
    d_device ( dev ),
    d_textureSourceBuffer ( LENGTH, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_finalResultBuffer1 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_finalResultBuffer2 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_finalResultBuffer3 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_finalResultBuffer4 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_texture ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_textureView ( d_texture ),
    d_outputImage1 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_outputView1 ( d_outputImage1 ),
    d_outputImage2 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_outputView2 ( d_outputImage2 ),
    d_outputImage3 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_outputView3 ( d_outputImage3 ),
    d_outputImage4 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, dev ),
    d_outputView4 ( d_outputImage4 ),
    d_normalizedSampler ( dev, d_normalizedSamplerDef ),
    d_unnormalizedSampler ( dev, d_unnormalizedSamplerDef ),
    d_graph ( d_outputView1, d_outputView2, d_outputView3, d_outputView4 ),
    d_pipeline (
        d_graph.d_render, dev,
        d_graph.d_output1, d_graph.d_output2, d_graph.d_output3, d_graph.d_output4,
        d_normalizedSampler, d_unnormalizedSampler
    ),
    d_dataBlock ( d_pipeline ),
    d_pass ( d_graph, dev ),
    d_frameBuffer ( d_pass )
{
    d_pipeline.definition().configureRenderingOptions ( WIDTH, HEIGHT, & d_options );
    
    d_pipeline.definition().setBuffers (
        d_textureView, d_normalizedSampler, d_unnormalizedSampler, & d_dataBlock );

    d_pass.addPipeline ( d_graph.d_render, d_pipeline, d_options );

    d_graph.d_init << [ this ]()
    {
        d_textureSourceBuffer.cmdCopyToImage (
            d_texture,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );
    };

    d_graph.d_render << [ this ]()
    {
        d_pass.pipeline ( 0, 0 ).cmdBind();
        d_dataBlock.cmdBind();
        d_pipeline.definition().cmdDrawWholeScreenPatch();
    };

    d_graph.d_fini << [ this ]()
    {
        d_finalResultBuffer1.cmdCopyFromImage (
            d_outputImage1,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_finalResultBuffer2.cmdCopyFromImage (
            d_outputImage2,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_finalResultBuffer3.cmdCopyFromImage (
            d_outputImage3,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_finalResultBuffer4.cmdCopyFromImage (
            d_outputImage4,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );
    };

    prepareVectors();
}

// -----------------------------------------------------------------------------

void SampTest :: operator()()
{
    using namespace vpp;

    CommandBuffer cmdBuffer = d_device.defaultCmdPool().createBuffer();
    CommandBufferRecorder recorder ( cmdBuffer );

    recorder.render ( d_pass, d_frameBuffer );

    Queue q ( d_device, Q_GRAPHICS );
    q.submit ( cmdBuffer );

    q.waitForIdle();
    compareResults();
}

// -----------------------------------------------------------------------------

void SampTest :: prepareVectors()
{
    d_textureSourceBuffer.resize ( LENGTH );
    d_finalResultBuffer1.resize ( LENGTH );
    d_finalResultBuffer2.resize ( LENGTH );
    d_finalResultBuffer3.resize ( LENGTH );
    d_finalResultBuffer4.resize ( LENGTH );

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        
        d_textureSourceBuffer [ k ] = DataType ( float ( j ), float ( i ), 0.0f, 0.0f );
        d_finalResultBuffer1 [ k ] = DataType ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_finalResultBuffer2 [ k ] = DataType ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_finalResultBuffer3 [ k ] = DataType ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_finalResultBuffer4 [ k ] = DataType ( 0.0f, 0.0f, 0.0f, 0.0f );
    }
}

// -----------------------------------------------------------------------------

void SampTest :: compareResults()
{
    using namespace vpp;

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        const DataType s1 = d_textureSourceBuffer [ k ];
        const DataType t1 = d_finalResultBuffer1 [ k ];
        const DataType t2 = d_finalResultBuffer2 [ k ];
        const DataType t3 = d_finalResultBuffer3 [ k ];
        const DataType t4 = d_finalResultBuffer4 [ k ];
        check ( s1 == t1 );
        check ( s1 == t2 );
        check ( s1 == t3 );
        check ( s1 == t4 );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KRenderingTests
{
public:
    KRenderingTests ( const vpp::Device& hDevice );

    void runTests();

private:
    AttTest d_attTest;
    SampTest d_sampTest;
};

// -----------------------------------------------------------------------------

KRenderingTests :: KRenderingTests ( const vpp::Device& hDevice ) :
    d_attTest ( hDevice ),
    d_sampTest ( hDevice )
{
}

// -----------------------------------------------------------------------------

void KRenderingTests :: runTests()
{
    d_attTest();
    d_sampTest();
}

// -----------------------------------------------------------------------------
} // namespace vpptest
// -----------------------------------------------------------------------------

int main()
{
    using namespace vpptest;
    using namespace vpp;

    std::ostringstream validationLog;

    Instance inst ( Instance::VALIDATION );
    //Instance inst;
    vpp::StreamDebugReporter m_debugReporter ( validationLog, inst, vpp::DebugReporter::SHADERS );

    PhysicalDevices physicalDevices;

    if ( inst.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
         || physicalDevices.empty() )
    {
        return -1;
    }

    PhysicalDevice phd = physicalDevices [ 0 ];
    Device dev ( phd );

    KRenderingTests tests ( dev );
    tests.runTests();

    return 0;
}

// -----------------------------------------------------------------------------
