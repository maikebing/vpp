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

template< typename T >
struct TTestTypeTraits
{
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< float >
{
    typedef vpp::Float shader_type;

    inline float create ( float v ) const
    {
        return v;
    }

    inline vpp::Float create ( const vpp::Float& v ) const
    {
        return v;
    }

    inline bool compare ( float a, float b ) const
    {
        return compareFloats ( a, b );
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::vect2 >
{
    typedef vpp::Vec2 shader_type;

    inline vpp::vect2 create ( float v ) const
    {
        vpp::vect2 r;
        r.set ( v, v );
        return r;
    }

    inline vpp::Vec2 create ( const vpp::Float& v ) const
    {
        return vpp::Vec2 ( v, v );
    }

    inline bool compare ( const vpp::vect2& a, const vpp::vect2& b ) const
    {
        return compareFloats ( a.x, b.x ) && compareFloats ( a.y, b.y );
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::vect3 >
{
    typedef vpp::Vec3 shader_type;

    inline vpp::vect3 create ( float v ) const
    {
        vpp::vect3 r;
        r.set ( v, v, v );
        return r;
    }

    inline vpp::Vec3 create ( const vpp::Float& v ) const
    {
        return vpp::Vec3 ( v, v, v );
    }

    inline bool compare ( const vpp::vect3& a, const vpp::vect3& b ) const
    {
        return
            compareFloats ( a.x, b.x )
            && compareFloats ( a.y, b.y )
            && compareFloats ( a.z, b.z );
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::vect4 >
{
    typedef vpp::Vec4 shader_type;

    inline vpp::vect4 create ( float v ) const
    {
        vpp::vect4 r;
        r.set ( v, v, v, v );
        return r;
    }

    inline vpp::Vec4 create ( const vpp::Float& v ) const
    {
        return vpp::Vec4 ( v, v, v, v );
    }

    inline bool compare ( const vpp::vect4& a, const vpp::vect4& b ) const
    {
        return
            compareFloats ( a.x, b.x )
            && compareFloats ( a.y, b.y )
            && compareFloats ( a.z, b.z )
            && compareFloats ( a.w, b.w );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TParams : public vpp::UniformStruct< TAG, TParams >
{
    vpp::UniformFld< TAG, vpp::format< int, int, int, int > > d_params;
};

typedef TParams< vpp::GPU > GParams;
typedef TParams< vpp::CPU > CParams;
typedef vpp::gvector< CParams, vpp::Buf::UNIFORM > CParamsBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                Test scalar (or by-component) float functions

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum EFloatTests
{
    FT_ADD, FT_SUB, FT_MUL, FT_DIV, FT_REM, FT_MOD,

    FT_EQU1, FT_NEQ1, FT_LSS1, FT_GTR1, FT_LEQ1, FT_GEQ1,
    FT_EQU0, FT_NEQ0, FT_LSS0, FT_GTR0, FT_LEQ0, FT_GEQ0,

    FT_ROUND, FT_ROUND_EVEN, FT_TRUNC,
    FT_ABS, FT_SIGN, FT_FLOOR, FT_CEIL, FT_FRACT,
    FT_RADIANS, FT_DEGREES,

    FT_SIN, FT_COS, FT_TAN, FT_ASIN, FT_ACOS, FT_ATAN,
    FT_SINH, FT_COSH, FT_TANH, FT_ASINH, FT_ACOSH, FT_ATANH,
    FT_ATAN2,

    FT_POW, FT_EXP, FT_LOG, FT_EXP2, FT_LOG2, FT_SQRT, FT_INVSQRT, FT_MIN, FT_MAX,
    FT_CLAMP0, FT_CLAMP1, FT_CLAMP2,
    FT_MIX0, FT_MIX1, FT_MIX2, FT_STEP0, FT_STEP1,
    FT_SMOOTHSTEP0, FT_SMOOTHSTEP1, FT_SMOOTHSTEP2,

    FT_MODF,

    FT_count
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename HostT >
class TFloatTestPipeline : public vpp::ComputePipelineConfig
{
public:
    TFloatTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::UniformBufferView& vs1,
        const vpp::UniformBufferView& vs2,
        const vpp::UniformBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::inPushConstant< TParams > d_params;

    vpp::inUniformBuffer d_source1;
    vpp::inUniformBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

template< typename HostT >
TFloatTestPipeline< HostT > :: TFloatTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & TFloatTestPipeline< HostT >::fComputeShader )
{
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TFloatTestPipeline< HostT > :: setData (
    const vpp::UniformBufferView& vs1,
    const vpp::UniformBufferView& vs2,
    const vpp::UniformBufferView& vt,
    int size,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_source1 = vs1,
        d_source2 = vs2,
        d_target = vt
    ));

    d_params.data().d_params.d_value.r = size;
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TFloatTestPipeline< HostT > :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    typedef typename TTestTypeTraits< HostT >::shader_type shader_type;
    TTestTypeTraits< HostT > tt;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformSimpleArray< HostT, decltype ( d_source1 ) > inSource1 ( d_source1 );
    UniformSimpleArray< HostT, decltype ( d_source2 ) > inSource2 ( d_source2 );
    UniformSimpleArray< HostT, decltype ( d_target ) > outTarget ( d_target );

    const shader_type v0 = tt.create ( Float ( 0.0f ) );
    const shader_type v1 = tt.create ( Float ( 1.0f ) );
    const shader_type v2 = tt.create ( Float ( 2.0f ) );

    If ( g == 0 && l == 0 );
        outTarget [ FT_ADD ] = inSource1 [ FT_ADD ] + inSource2 [ FT_ADD ];
        outTarget [ FT_SUB ] = inSource1 [ FT_SUB ] - inSource2 [ FT_SUB ];
        outTarget [ FT_MUL ] = inSource1 [ FT_MUL ] * inSource2 [ FT_MUL ];
        outTarget [ FT_DIV ] = inSource1 [ FT_DIV ] / inSource2 [ FT_DIV ];
        outTarget [ FT_REM ] = inSource1 [ FT_REM ] % inSource2 [ FT_REM ];
        outTarget [ FT_MOD ] = Mod ( inSource1 [ FT_MOD ], inSource2 [ FT_MOD ] );

        outTarget [ FT_EQU1 ] = Select ( inSource1 [ FT_EQU1 ] == inSource2 [ FT_EQU1 ], v1, v0 );
        outTarget [ FT_NEQ1 ] = Select ( inSource1 [ FT_NEQ1 ] != inSource2 [ FT_NEQ1 ], v1, v0 );
        outTarget [ FT_LSS1 ] = Select ( inSource1 [ FT_LSS1 ] < inSource2 [ FT_LSS1 ], v1, v0 );
        outTarget [ FT_GTR1 ] = Select ( inSource1 [ FT_GTR1 ] > inSource2 [ FT_GTR1 ], v1, v0 );
        outTarget [ FT_LEQ1 ] = Select ( inSource1 [ FT_LEQ1 ] <= inSource2 [ FT_LEQ1 ], v1, v0 );
        outTarget [ FT_GEQ1 ] = Select ( inSource1 [ FT_GEQ1 ] >= inSource2 [ FT_GEQ1 ], v1, v0 );
        outTarget [ FT_EQU0 ] = Select ( inSource1 [ FT_EQU0 ] == inSource2 [ FT_EQU0 ], v1, v0 );
        outTarget [ FT_NEQ0 ] = Select ( inSource1 [ FT_NEQ0 ] != inSource2 [ FT_NEQ0 ], v1, v0 );
        outTarget [ FT_LSS0 ] = Select ( inSource1 [ FT_LSS0 ] < inSource2 [ FT_LSS0 ], v1, v0 );
        outTarget [ FT_GTR0 ] = Select ( inSource1 [ FT_GTR0 ] > inSource2 [ FT_GTR0 ], v1, v0 );
        outTarget [ FT_LEQ0 ] = Select ( inSource1 [ FT_LEQ0 ] <= inSource2 [ FT_LEQ0 ], v1, v0 );
        outTarget [ FT_GEQ0 ] = Select ( inSource1 [ FT_GEQ0 ] >= inSource2 [ FT_GEQ0 ], v1, v0 );

        outTarget [ FT_ROUND ] = Round ( inSource1 [ FT_ROUND ] );
        outTarget [ FT_ROUND_EVEN ] = RoundEven ( inSource1 [ FT_ROUND_EVEN ] );
        outTarget [ FT_TRUNC ] = Trunc ( inSource1 [ FT_TRUNC ] );
        outTarget [ FT_ABS ] = Abs ( inSource1 [ FT_ABS ] );
        outTarget [ FT_SIGN ] = Sign ( inSource1 [ FT_SIGN ] );
        outTarget [ FT_FLOOR ] = Floor ( inSource1 [ FT_FLOOR ] );
        outTarget [ FT_CEIL ] = Ceil ( inSource1 [ FT_CEIL ] );
        outTarget [ FT_FRACT ] = Fract ( inSource1 [ FT_FRACT ] );
        outTarget [ FT_RADIANS ] = Radians ( inSource1 [ FT_RADIANS ] );
        outTarget [ FT_DEGREES ] = Degrees ( inSource1 [ FT_DEGREES ] );
        outTarget [ FT_SIN ] = Sin ( inSource1 [ FT_SIN ] );
        outTarget [ FT_COS ] = Cos ( inSource1 [ FT_COS ] );
        outTarget [ FT_TAN ] = Tan ( inSource1 [ FT_TAN ] );
        outTarget [ FT_ASIN ] = Asin ( inSource1 [ FT_ASIN ] );
        outTarget [ FT_ACOS ] = Acos ( inSource1 [ FT_ACOS ] );
        outTarget [ FT_ATAN ] = Atan ( inSource1 [ FT_ATAN ] );
        outTarget [ FT_SINH ] = Sinh ( inSource1 [ FT_SINH ] );
        outTarget [ FT_COSH ] = Cosh ( inSource1 [ FT_COSH ] );
        outTarget [ FT_TANH ] = Tanh ( inSource1 [ FT_TANH ] );
        outTarget [ FT_ASINH ] = Asinh ( inSource1 [ FT_ASINH ] );
        outTarget [ FT_ACOSH ] = Acosh ( inSource1 [ FT_ACOSH ] );
        outTarget [ FT_ATANH ] = Atanh ( inSource1 [ FT_ATANH ] );
        outTarget [ FT_ATAN2 ] = Atan2 ( inSource1 [ FT_ATAN2 ], inSource2 [ FT_ATAN2 ] );
        outTarget [ FT_POW ] = Pow ( inSource1 [ FT_POW ], inSource2 [ FT_POW ] );
        outTarget [ FT_EXP ] = Exp ( inSource1 [ FT_EXP ] );
        outTarget [ FT_LOG ] = Log ( inSource1 [ FT_LOG ] );
        outTarget [ FT_EXP2 ] = Exp2 ( inSource1 [ FT_EXP2 ] );
        outTarget [ FT_LOG2 ] = Log2 ( inSource1 [ FT_LOG2 ] );
        outTarget [ FT_SQRT ] = Sqrt ( inSource1 [ FT_SQRT ] );
        outTarget [ FT_INVSQRT ] = InverseSqrt ( inSource1 [ FT_INVSQRT ] );
        outTarget [ FT_MIN ] = Min ( inSource1 [ FT_MIN ], inSource2 [ FT_MIN ] );
        outTarget [ FT_MAX ] = Max ( inSource1 [ FT_MAX ], inSource2 [ FT_MAX ] );
        outTarget [ FT_CLAMP0 ] = Clamp ( v0, inSource1 [ FT_CLAMP0 ], inSource2 [ FT_CLAMP0 ] );
        outTarget [ FT_CLAMP1 ] = Clamp ( v1, inSource1 [ FT_CLAMP1 ], inSource2 [ FT_CLAMP1 ] );
        outTarget [ FT_CLAMP2 ] = Clamp ( v2, inSource1 [ FT_CLAMP2 ], inSource2 [ FT_CLAMP2 ] );
        outTarget [ FT_MIX0 ] = Mix ( v0, v2, inSource1 [ FT_MIX0 ] );
        outTarget [ FT_MIX1 ] = Mix ( v0, v2, inSource1 [ FT_MIX1 ] );
        outTarget [ FT_MIX2 ] = Mix ( v0, v2, inSource1 [ FT_MIX2 ] );
        outTarget [ FT_STEP0 ] = Step ( inSource1 [ FT_STEP0 ], inSource2 [ FT_STEP0 ] );
        outTarget [ FT_STEP1 ] = Step ( inSource1 [ FT_STEP1 ], inSource2 [ FT_STEP1 ] );
        outTarget [ FT_SMOOTHSTEP0 ] = SmoothStep ( v0, v2, inSource1 [ FT_SMOOTHSTEP0 ] );
        outTarget [ FT_SMOOTHSTEP1 ] = SmoothStep ( v0, v2, inSource1 [ FT_SMOOTHSTEP1 ] );
        outTarget [ FT_SMOOTHSTEP2 ] = SmoothStep ( v0, v2, inSource1 [ FT_SMOOTHSTEP2 ] );
        outTarget [ FT_MODF ] = Modf ( inSource1 [ FT_MODF ] ).first;
    Fi();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename HostT >
class TFloatTest : public vpp::Computation
{
public:
    TFloatTest ( const vpp::Device& hDevice );
    TFloatTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    void prepareTest ( EFloatTests testId, float s1, float s2, float exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef TFloatTestPipeline< HostT > TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< HostT, vpp::Buf::STORAGE > FloatVector;

    FloatVector d_source1;
    FloatVector d_source2;
    FloatVector d_target;

    std::vector< float > d_expected;
};

// -----------------------------------------------------------------------------

template< typename HostT >
TFloatTest< HostT > :: TFloatTest ( const vpp::Device& hDevice ) :
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        d_pipeline.definition().d_params.cmdPush();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

template< typename HostT >
TFloatTest< HostT > :: TFloatTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        d_pipeline.definition().d_params.cmdPush();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TFloatTest< HostT > :: prepareTest (
    EFloatTests testId, float s1, float s2, float exp )
{
    TTestTypeTraits< HostT > tt;

    d_source1 [ testId ] = tt.create ( s1 );
    d_source2 [ testId ] = tt.create ( s2 );
    d_target [ testId ] = tt.create ( 0.0f );
    d_expected [ testId ] = exp;
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TFloatTest< HostT > :: prepareVectors()
{
    const unsigned int nItems = FT_count;
    static const float pi = 3.1415926f;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    prepareTest ( FT_ADD, 17.0f, 2.0f, 19.0f );
    prepareTest ( FT_SUB, 17.0f, 2.0f, 15.0f );
    prepareTest ( FT_MUL, 17.0f, 2.0f, 34.0f );
    prepareTest ( FT_DIV, 17.0f, 2.0f, 8.5f );

    prepareTest ( FT_REM, -17.0f, 10.0f, -7.0f );
    prepareTest ( FT_MOD, -17.0f, 10.0f, 3.0f );

    prepareTest ( FT_EQU1, 17.0f, 17.0f, 1.0f );
    prepareTest ( FT_NEQ1, 17.0f, 16.0f, 1.0f );
    prepareTest ( FT_LSS1, 17.0f, 18.0f, 1.0f );
    prepareTest ( FT_GTR1, 17.0f, 15.0f, 1.0f );
    prepareTest ( FT_LEQ1, 17.0f, 17.0f, 1.0f );
    prepareTest ( FT_GEQ1, 17.0f, 17.0f, 1.0f );

    prepareTest ( FT_EQU0, 17.0f, 18.0f, 0.0f );
    prepareTest ( FT_NEQ0, 17.0f, 17.0f, 0.0f );
    prepareTest ( FT_LSS0, 18.0f, 17.0f, 0.0f );
    prepareTest ( FT_GTR0, 15.0f, 15.0f, 0.0f );
    prepareTest ( FT_LEQ0, 18.0f, 17.0f, 0.0f );
    prepareTest ( FT_GEQ0, 17.0f, 18.0f, 0.0f );

    prepareTest ( FT_ROUND, 17.2f, 0.0f, 17.0f );
    prepareTest ( FT_ROUND_EVEN, 17.5f, 0.0f, 18.0f );
    prepareTest ( FT_TRUNC, -17.2f, 0.0f, -17.0f );
    prepareTest ( FT_ABS, -23.0f, 0.0f, 23.0f );
    prepareTest ( FT_SIGN, -23.0f, 0.0f, -1.0f );
    prepareTest ( FT_FLOOR, -17.1f, 0.0f, -18.0f );
    prepareTest ( FT_CEIL, -17.1f, 0.0f, -17.0f );
    prepareTest ( FT_FRACT, -17.1f, 0.0f, 0.9f );
    prepareTest ( FT_RADIANS, 180.0f, 0.0f, pi );
    prepareTest ( FT_DEGREES, pi, 0, 180.0f );
    
    prepareTest ( FT_SIN, pi/2, 0.0f, 1.0f );
    prepareTest ( FT_COS, 0.0f, 0.0f, 1.0f );
    prepareTest ( FT_TAN, pi/4, 0.0f, 1.0f );
    prepareTest ( FT_ASIN, 1.0f, 0.0f, pi/2 );
    prepareTest ( FT_ACOS, 1.0f, 0.0f, 0.0f );
    prepareTest ( FT_ATAN, 1.0f, 0.0f, pi/4 );
    prepareTest ( FT_SINH, 1.0f, 0.0f, std::sinh ( 1.0f ) );
    prepareTest ( FT_COSH, 1.0f, 0.0f, std::cosh ( 1.0f ) );
    prepareTest ( FT_TANH, 1.0f, 0.0f, std::tanh ( 1.0f ) );
    prepareTest ( FT_ASINH, 1.0f, 0.0f, std::asinh ( 1.0f ) );
    prepareTest ( FT_ACOSH, 1.0f, 0.0f, std::acosh ( 1.0f ) );
    prepareTest ( FT_ATANH, 0.5f, 0.0f, std::atanh ( 0.5f ) );

    prepareTest ( FT_ATAN2, 2.0f, 2.0f, pi/4 );
    prepareTest ( FT_POW, 3.0f, 3.0f, 27.0f );
    prepareTest ( FT_EXP, 0.0f, 0.0f, 1.0f );
    prepareTest ( FT_LOG, 1.0f, 0.0f, 0.0f );
    prepareTest ( FT_EXP2, 1.0f, 0.0f, 2.0f );
    prepareTest ( FT_LOG2, 16.0f, 0.0f, 4.0f );
    prepareTest ( FT_SQRT, 16.0f, 0.0f, 4.0f );
    prepareTest ( FT_INVSQRT, 16.0f, 0.0f, 0.25f );

    prepareTest ( FT_MIN, 16.0f, 5.0f, 5.0f );
    prepareTest ( FT_MAX, 16.0f, 5.0f, 16.0f );
    prepareTest ( FT_CLAMP0, 0.5f, 1.5f, 0.5f );
    prepareTest ( FT_CLAMP1, 0.5f, 1.5f, 1.0f );
    prepareTest ( FT_CLAMP2, 0.5f, 1.5f, 1.5f );
    prepareTest ( FT_MIX0, 0.0f, 0.0f, 0.0f );
    prepareTest ( FT_MIX1, 0.5f, 0.0f, 1.0f );
    prepareTest ( FT_MIX2, 1.0f, 0.0f, 2.0f );
    prepareTest ( FT_STEP0, 1.0f, 0.0f, 0.0f );
    prepareTest ( FT_STEP1, 1.0f, 2.0f, 1.0f );
    prepareTest ( FT_SMOOTHSTEP0, 0.0f, 0.0f, 0.0f );
    prepareTest ( FT_SMOOTHSTEP1, 1.0f, 0.0f, 0.5f );
    prepareTest ( FT_SMOOTHSTEP2, 2.0f, 0.0f, 1.0f );

    prepareTest ( FT_MODF, 2.7f, 0.0f, 0.7f );
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TFloatTest< HostT > :: compareResults()
{
    TTestTypeTraits< HostT > tt;

    for ( size_t i = 0; i != FT_count; ++i )
    {
        const HostT t = d_target [ i ];
        const HostT e = tt.create ( d_expected [ i ] );
        check ( tt.compare ( t, e ) );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                        Test vector float functions

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum EVectorTests
{
    VT_DOT2, VT_DOT3, VT_DOT4,
    VT_OPDET2, VT_OPDET3, VT_OPDET4,
    VT_DISTANCE2, VT_DISTANCE3, VT_DISTANCE4,
    VT_LENGTH2, VT_LENGTH3, VT_LENGTH4,
    VT_NORMALIZE2, VT_NORMALIZE3, VT_NORMALIZE4,
    VT_REFRACT2, VT_REFRACT3, VT_REFRACT4,
    VT_REFLECT2, VT_REFLECT3, VT_REFLECT4,
    VT_FACEFORWARD2, VT_FACEFORWARD3, VT_FACEFORWARD4,

    VT_count
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KVectorTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KVectorTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::UniformBufferView& vs1,
        const vpp::UniformBufferView& vs2,
        const vpp::UniformBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    //vpp::inPushConstant< TParams > d_params;

    vpp::inUniformBuffer d_source1;
    vpp::inUniformBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KVectorTestPipeline :: KVectorTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KVectorTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KVectorTestPipeline :: setData (
    const vpp::UniformBufferView& vs1,
    const vpp::UniformBufferView& vs2,
    const vpp::UniformBufferView& vt,
    int size,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_source1 = vs1,
        d_source2 = vs2,
        d_target = vt
    ));

    //d_params.data().d_params.d_value.r = size;
}

// -----------------------------------------------------------------------------

void KVectorTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    TTestTypeTraits< vect4 > tt;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformSimpleArray< vect4, decltype ( d_source1 ) > inSource1 ( d_source1 );
    UniformSimpleArray< vect4, decltype ( d_source2 ) > inSource2 ( d_source2 );
    UniformSimpleArray< vect4, decltype ( d_target ) > outTarget ( d_target );

    If ( g == 0 && l == 0 );
    {
        // ----------------------------------------------------------------------

        const Float dot2 = Dot ( inSource1 [ VT_DOT2 ][ XY ], inSource2 [ VT_DOT2 ][ XY ] );
        outTarget [ VT_DOT2 ] = tt.create ( dot2 );

        const Float dot3 = Dot ( inSource1 [ VT_DOT3 ][ XYZ ], inSource2 [ VT_DOT3 ][ XYZ ] );
        outTarget [ VT_DOT3 ] = tt.create ( dot3 );

        const Float dot4 = Dot ( inSource1 [ VT_DOT4 ], inSource2 [ VT_DOT4 ] );
        outTarget [ VT_DOT4 ] = tt.create ( dot4 );

        // ----------------------------------------------------------------------

        const Mat4 op4 = OuterProduct ( inSource1 [ VT_OPDET4 ], inSource2 [ VT_OPDET4 ] );
        const Float opdet4 = Determinant ( op4 );
        outTarget [ VT_OPDET4 ] = tt.create ( opdet4 );

        const Mat3 op3 = OuterProduct ( inSource1 [ VT_OPDET3 ][ XYZ ], inSource2 [ VT_OPDET3 ][ XYZ ] );
        const Float opdet3 = Determinant ( op3 );
        outTarget [ VT_OPDET3 ] = tt.create ( opdet3 );

        const Mat2 op2 = OuterProduct ( inSource1 [ VT_OPDET2 ][ XY ], inSource2 [ VT_OPDET2 ][ XY ] );
        const Float opdet2 = Determinant ( op2 );
        outTarget [ VT_OPDET2 ] = tt.create ( opdet2 );

        // ----------------------------------------------------------------------

        const Float dist4 = Distance ( inSource1 [ VT_DISTANCE4 ], inSource2 [ VT_DISTANCE4 ] );
        outTarget [ VT_DISTANCE4 ] = tt.create ( dist4 );

        const Float dist3 = Distance ( inSource1 [ VT_DISTANCE3 ], inSource2 [ VT_DISTANCE3 ] );
        outTarget [ VT_DISTANCE3 ] = tt.create ( dist3 );

        const Float dist2 = Distance ( inSource1 [ VT_DISTANCE2 ], inSource2 [ VT_DISTANCE2 ] );
        outTarget [ VT_DISTANCE2 ] = tt.create ( dist2 );

        // ----------------------------------------------------------------------

        const Float len4 = Length ( inSource1 [ VT_LENGTH4 ] );
        outTarget [ VT_LENGTH4 ] = tt.create ( len4 );

        const Float len3 = Length ( inSource1 [ VT_LENGTH3 ] );
        outTarget [ VT_LENGTH3 ] = tt.create ( len3 );

        const Float len2 = Length ( inSource1 [ VT_LENGTH2 ] );
        outTarget [ VT_LENGTH2 ] = tt.create ( len2 );

        // ----------------------------------------------------------------------

        const Vec4 norm4 = Normalize ( inSource1 [ VT_NORMALIZE4 ] );
        outTarget [ VT_NORMALIZE4 ] = norm4;

        const Vec3 norm3 = Normalize ( inSource1 [ VT_NORMALIZE3 ][ XYZ ] );
        outTarget [ VT_NORMALIZE3 ] = Vec4 ( norm3, 0.0f );

        const Vec2 norm2 = Normalize ( inSource1 [ VT_NORMALIZE2 ][ XY ] );
        outTarget [ VT_NORMALIZE2 ] = Vec4 ( norm2, 0.0f, 0.0f );

        // ----------------------------------------------------------------------

        const Vec4 refr4 = Refract ( inSource1 [ VT_REFRACT4 ], inSource2 [ VT_REFRACT4 ], 0.5f );
        outTarget [ VT_REFRACT4 ] = refr4;

        const Vec3 refr3 = Refract ( inSource1 [ VT_REFRACT3 ][ XYZ ], inSource2 [ VT_REFRACT3 ][ XYZ ], 0.5f );
        outTarget [ VT_REFRACT3 ] = Vec4 ( refr3, 0.0f );

        const Vec2 refr2 = Refract ( inSource1 [ VT_REFRACT2 ][ XY ], inSource2 [ VT_REFRACT2 ][ XY ], 0.5f );
        outTarget [ VT_REFRACT2 ] = Vec4 ( refr2, 0.0f, 0.0f );

        // ----------------------------------------------------------------------

        const Vec4 refl4 = Reflect ( inSource1 [ VT_REFLECT4 ], inSource2 [ VT_REFLECT4 ] );
        outTarget [ VT_REFLECT4 ] = refl4;

        const Vec3 refl3 = Reflect ( inSource1 [ VT_REFLECT3 ][ XYZ ], inSource2 [ VT_REFLECT3 ][ XYZ ] );
        outTarget [ VT_REFLECT3 ] = Vec4 ( refl3, 0.0f );

        const Vec2 refl2 = Reflect ( inSource1 [ VT_REFLECT2 ][ XY ], inSource2 [ VT_REFLECT2 ][ XY ] );
        outTarget [ VT_REFLECT2 ] = Vec4 ( refl2, 0.0f, 0.0f );

        // ----------------------------------------------------------------------

        const Vec4 ffwd4 = Reflect ( inSource1 [ VT_FACEFORWARD4 ], inSource2 [ VT_FACEFORWARD4 ] );
        outTarget [ VT_FACEFORWARD4 ] = ffwd4;

        const Vec3 ffwd3 = Reflect ( inSource1 [ VT_FACEFORWARD3 ][ XYZ ], inSource2 [ VT_FACEFORWARD3 ][ XYZ ] );
        outTarget [ VT_FACEFORWARD3 ] = Vec4 ( ffwd3, 0.0f );

        const Vec2 ffwd2 = Reflect ( inSource1 [ VT_FACEFORWARD2 ][ XY ], inSource2 [ VT_FACEFORWARD2 ][ XY ] );
        outTarget [ VT_FACEFORWARD2 ] = Vec4 ( ffwd2, 0.0f, 0.0f );

        // ----------------------------------------------------------------------
    }
    Fi();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KVectorTest : public vpp::Computation
{
public:
    KVectorTest ( const vpp::Device& hDevice );
    KVectorTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    void prepareTest (
        EVectorTests testId,
        const vpp::vect4& s1,
        const vpp::vect4& s2,
        const vpp::vect4& exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef KVectorTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< vpp::vect4, vpp::Buf::STORAGE > FloatVector;

    FloatVector d_source1;
    FloatVector d_source2;
    FloatVector d_target;

    std::vector< vpp::vect4 > d_expected;
};

// -----------------------------------------------------------------------------

KVectorTest :: KVectorTest ( const vpp::Device& hDevice ) :
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        //d_pipeline.definition().d_params.cmdPush();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

KVectorTest :: KVectorTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        //d_pipeline.definition().d_params.cmdPush();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KVectorTest :: prepareTest (
    EVectorTests testId,
    const vpp::vect4& s1,
    const vpp::vect4& s2,
    const vpp::vect4& exp )
{
    d_source1 [ testId ] = s1;
    d_source2 [ testId ] = s2;
    d_target [ testId ].set ( 0.0f, 0.0f, 0.0f, 0.0f );
    d_expected [ testId ] = exp;
}

// -----------------------------------------------------------------------------

void KVectorTest :: prepareVectors()
{
    using namespace vpp;

    const unsigned int nItems = VT_count;
    static const float pi = 3.1415926f;
    static const float r22 = std::sqrt ( 2.0f ) / 2.0f;
    static const float r33 = std::sqrt ( 3.0f ) / 3.0f;
    static const float r44 = std::sqrt ( 4.0f ) / 4.0f;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    prepareTest (
        VT_DOT2,
        vect4 ( 2.0f, 3.0f, 0.0f, 0.0f ),
        vect4 ( 5.0f, 7.0f, 0.0f, 0.0f ),
        vect4 ( 31.0f, 31.0f, 31.0f, 31.0f )
    );

    prepareTest (
        VT_DOT3,
        vect4 ( 2.0f, 4.0f, 6.0f, 0.0f ),
        vect4 ( 3.0f, 5.0f, 7.0f, 0.0f ),
        vect4 ( 68.0f, 68.0f, 68.0f, 68.0f )
    );

    prepareTest (
        VT_DOT4,
        vect4 ( 2.0f, 4.0f, 6.0f, 8.0f ),
        vect4 ( 3.0f, 5.0f, 7.0f, 9.0f ),
        vect4 ( 140.0f, 140.0f, 140.0f, 140.0f )
    );

    prepareTest (
        VT_OPDET2,
        vect4 ( 2.0f, 4.0f, 0.0f, 0.0f ),
        vect4 ( 3.0f, 5.0f, 0.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f )
    );

    prepareTest (
        VT_OPDET3,
        vect4 ( 2.0f, 4.0f, 6.0f, 0.0f ),
        vect4 ( 3.0f, 5.0f, 7.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f )
    );

    prepareTest (
        VT_OPDET4,
        vect4 ( 2.0f, 4.0f, 6.0f, 8.0f ),
        vect4 ( 3.0f, 5.0f, 7.0f, 9.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f )
    );

    prepareTest (
        VT_DISTANCE2,
        vect4 ( 5.0f, 7.0f, 0.0f, 0.0f ),
        vect4 ( 2.0f, 3.0f, 0.0f, 0.0f ),
        vect4 ( 5.0f, 5.0f, 5.0f, 5.0f )
    );

    prepareTest (
        VT_DISTANCE3,
        vect4 ( 5.0f, 7.0f, 19.0f, 0.0f ),
        vect4 ( 2.0f, 3.0f, 7.0f, 0.0f ),
        vect4 ( 13.0f, 13.0f, 13.0f, 13.0f )
    );

    prepareTest (
        VT_DISTANCE4,
        vect4 ( 5.0f, 5.0f, 5.0f, 5.0f ),
        vect4 ( 2.0f, 2.0f, 2.0f, 2.0f ),
        vect4 ( 6.0f, 6.0f, 6.0f, 6.0f )
    );

    prepareTest (
        VT_LENGTH2,
        vect4 ( 3.0f, 4.0f, 0.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( 5.0f, 5.0f, 5.0f, 5.0f )
    );

    prepareTest (
        VT_LENGTH3,
        vect4 ( 3.0f, 4.0f, 12.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( 13.0f, 13.0f, 13.0f, 13.0f )
    );

    prepareTest (
        VT_LENGTH4,
        vect4 ( 3.0f, 3.0f, 3.0f, 3.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( 6.0f, 6.0f, 6.0f, 6.0f )
    );

    prepareTest (
        VT_NORMALIZE2,
        vect4 ( 1.0f, 1.0f, 0.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( r22, r22, 0.0f, 0.0f )
    );

    prepareTest (
        VT_NORMALIZE3,
        vect4 ( 1.0f, 1.0f, 1.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( r33, r33, r33, 0.0f )
    );

    prepareTest (
        VT_NORMALIZE4,
        vect4 ( 1.0f, 1.0f, 1.0f, 1.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( r44, r44, r44, r44 )
    );

    prepareTest (
        VT_REFRACT2,
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( -r22, -r22, 0.0f, 0.0f )
    );

    prepareTest (
        VT_REFRACT3,
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( -r33, -r33, -r33, 0.0f )
    );

    prepareTest (
        VT_REFRACT4,
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( -r44, -r44, -r44, -r44 )
    );

    prepareTest (
        VT_REFLECT2,
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( -r22, -r22, 0.0f, 0.0f )
    );

    prepareTest (
        VT_REFLECT3,
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( -r33, -r33, -r33, 0.0f )
    );

    prepareTest (
        VT_REFLECT4,
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( -r44, -r44, -r44, -r44 )
    );

    prepareTest (
        VT_FACEFORWARD2,
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( r22, r22, 0.0f, 0.0f ),
        vect4 ( -r22, -r22, 0.0f, 0.0f )
    );

    prepareTest (
        VT_FACEFORWARD3,
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( r33, r33, r33, 0.0f ),
        vect4 ( -r33, -r33, -r33, 0.0f )
    );

    prepareTest (
        VT_FACEFORWARD4,
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( r44, r44, r44, r44 ),
        vect4 ( -r44, -r44, -r44, -r44 )
    );
}

// -----------------------------------------------------------------------------

void KVectorTest :: compareResults()
{
    using namespace vpp;

    TTestTypeTraits< vect4 > tt;

    for ( size_t i = 0; i != VT_count; ++i )
    {
        const vect4 t = d_target [ i ];
        const vect4 e = d_expected [ i ];
        check ( tt.compare ( t, e ) );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                         Programming constructs tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum EConstructTests
{
    CT_SELECT1, CT_SELECT2, CT_SELECT3,
    CT_IF0, CT_IF1,
    CT_DOWHILE,
    CT_FOR0, CT_FOR1, CT_FOR2,
    CT_SWITCH0, CT_SWITCH1, CT_SWITCH2,
    CT_CAST0, CT_CAST1,

    CT_FUNCTION1, CT_FUNCTION2,

    CT_count
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class GFunGCD : public vpp::Function< vpp::Int, vpp::Int, vpp::Int >
{
public:
    GFunGCD();

private:
    vpp::Par< vpp::Int > _n;
    vpp::Par< vpp::Int > _k;
};

// -----------------------------------------------------------------------------

GFunGCD :: GFunGCD() :
    vpp::Function< vpp::Int, vpp::Int, vpp::Int >( "gcd" )
{
    using namespace vpp;

    Begin();

        VInt x = _n;
        VInt y = _k;
        VInt s = 0;
        VInt t = 0;

        If ( x < y );
            t = x;
            x = y;
            y = t;
        Fi();

        Do(); While ( ( ( x & 1 ) | ( y & 1 ) ) == 0 );
            x >>= 1u;
            y >>= 1u;
            ++s;
        Od();

        Do(); While ( x != 0 );
        {
            Do(); While ( ( x & 1 ) == 0 );
                x >>= 1u;
            Od();

            Do(); While ( ( y & 1 ) == 0 );
                y >>= 1u;
            Od();

            If ( x >= y );
                x = ( x - y ) >> 1u;
            Else();
                y = ( y - x ) >> 1u;
            Fi();
        }
        Od();

        Return ( y << s );

    End();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KConstructTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KConstructTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::UniformBufferView& vs1,
        const vpp::UniformBufferView& vs2,
        const vpp::UniformBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::inUniformBuffer d_source1;
    vpp::inUniformBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KConstructTestPipeline :: KConstructTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KConstructTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KConstructTestPipeline :: setData (
    const vpp::UniformBufferView& vs1,
    const vpp::UniformBufferView& vs2,
    const vpp::UniformBufferView& vt,
    int size,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_source1 = vs1,
        d_source2 = vs2,
        d_target = vt
    ));
}

// -----------------------------------------------------------------------------

void KConstructTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformSimpleArray< int, decltype ( d_source1 ) > inSource1 ( d_source1 );
    UniformSimpleArray< int, decltype ( d_source2 ) > inSource2 ( d_source2 );
    UniformSimpleArray< int, decltype ( d_target ) > outTarget ( d_target );

    Function< Int, Int > factorial ( "factorial" );
        Par< Int > factX;
    Begin();
    {
        VInt t = 0;
        VInt r = 1;

        For ( t, 2, factX+1 );
            r *= t;
        Rof();

        Return ( r );
    }
    End();

    Function< Int, Int, Int > binomial ( "binomial" );
    {
        Par< Int > n;
        Par< Int > k;
        Begin();
        {
            Return ( factorial ( n ) / ( factorial ( k )*factorial ( n-k ) ) );
        }
        End();
    }

    GFunGCD gcd;

    If ( g == 0 && l == 0 );
    {
        // ----------------------------------------------------------------------

        outTarget [ CT_SELECT1 ] = Select ( true, inSource1 [ CT_SELECT1 ], inSource2 [ CT_SELECT1 ] );
        outTarget [ CT_SELECT2 ] = Select ( false, inSource1 [ CT_SELECT2 ], inSource2 [ CT_SELECT2 ] );
        outTarget [ CT_SELECT3 ] = Select (
            inSource1 [ CT_SELECT3 ] < inSource2 [ CT_SELECT3 ],
            inSource1 [ CT_SELECT3 ], inSource2 [ CT_SELECT3 ] );

        // ----------------------------------------------------------------------

        for ( unsigned int i = CT_IF0; i <= CT_IF1; ++i )
        {
            If ( inSource1 [ i ] < 100 );
            {
                const Int t = inSource2 [ i ];
                const Int result = t + t + t;
                outTarget [ i ] = result;
            }
            Else();
            {
                const Int t = inSource2 [ i ];
                const Int result = t * t;
                outTarget [ i ] = result;
            }
            Fi();
        }

        // ----------------------------------------------------------------------

        // Binary GCD algorithm.

        VInt x = inSource1 [ CT_DOWHILE ];
        VInt y = inSource2 [ CT_DOWHILE ];
        VInt s = 0;
        VInt t = 0;

        If ( x < y );
            t = x;
            x = y;
            y = t;
        Fi();

        Do(); While ( ( ( x & 1 ) | ( y & 1 ) ) == 0 );
            x >>= 1u;
            y >>= 1u;
            ++s;
        Od();

        Do(); While ( x != 0 );
        {
            Do(); While ( ( x & 1 ) == 0 );
                x >>= 1u;
            Od();

            Do(); While ( ( y & 1 ) == 0 );
                y >>= 1u;
            Od();

            If ( x >= y );
                x = ( x - y ) >> 1u;
            Else();
                y = ( y - x ) >> 1u;
            Fi();
        }
        Od();

        outTarget [ CT_DOWHILE ] = y << s;

        // ----------------------------------------------------------------------

        for ( unsigned int i = CT_FOR0; i <= CT_FOR2; ++i )
        {
            const Int max = inSource1 [ i ];
            const Int step = inSource2 [ i ];
            y = 0;

            For ( x, 0, max, step );
                y += x;
            Rof();

            outTarget [ i ] = y;
        }

        // ----------------------------------------------------------------------

        for ( unsigned int i = CT_SWITCH0; i <= CT_SWITCH2; ++i )
        {
            const Int c = inSource1 [ i ];

            Switch ( c );
                Case ( 0 );
                    x = factorial ( 5 );
                    Break();

                Case ( 1 );
                    x = binomial ( 6, 2 );
                    Break();

                Default();
                    x = gcd ( 17*19, 17*23 );
                    Break();
            EndSwitch();

            outTarget [ i ] = x;
        }

        // ----------------------------------------------------------------------

        outTarget [ CT_CAST0 ] = StaticCast< Int >( 
            StaticCast< Float >( inSource1 [ CT_CAST0 ] ) * 0.1f );

        outTarget [ CT_CAST1 ] = ReinterpretCast< Int >( Float ( 1.0f ) );

        // ----------------------------------------------------------------------

        outTarget [ CT_FUNCTION1 ] = factorial ( inSource1 [ CT_FUNCTION1 ] );
        outTarget [ CT_FUNCTION2 ] = binomial ( inSource1 [ CT_FUNCTION2 ], inSource2 [ CT_FUNCTION2 ] );
    }
    Fi();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KConstructTest : public vpp::Computation
{
public:
    KConstructTest ( const vpp::Device& hDevice );
    KConstructTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    void prepareTest ( EConstructTests testId, int s1, int s2, int exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef KConstructTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< int, vpp::Buf::STORAGE > IntVector;

    IntVector d_source1;
    IntVector d_source2;
    IntVector d_target;

    std::vector< int > d_expected;
};

// -----------------------------------------------------------------------------

KConstructTest :: KConstructTest ( const vpp::Device& hDevice ) :
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

KConstructTest :: KConstructTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KConstructTest :: prepareTest (
    EConstructTests testId, int s1, int s2, int exp )
{
    d_source1 [ testId ] = s1;
    d_source2 [ testId ] = s2;
    d_target [ testId ] = 0;
    d_expected [ testId ] = exp;
}

// -----------------------------------------------------------------------------

void KConstructTest :: prepareVectors()
{
    using namespace vpp;

    const unsigned int nItems = CT_count;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    prepareTest ( CT_SELECT1, 5, 7, 5 );
    prepareTest ( CT_SELECT2, 5, 7, 7 );
    prepareTest ( CT_SELECT3, 100, 200, 100 );

    prepareTest ( CT_IF0, 99, 7, 21 );
    prepareTest ( CT_IF1, 101, 11, 121 );

    prepareTest ( CT_DOWHILE, 17*19, 17*23, 17 );

    prepareTest ( CT_FOR0, 0, 1, 0 );
    prepareTest ( CT_FOR1, 5, 1, 10 );
    prepareTest ( CT_FOR2, 10, 2, 20 );

    prepareTest ( CT_SWITCH0, 0, 0, 120 );
    prepareTest ( CT_SWITCH1, 1, 0, 15 );
    prepareTest ( CT_SWITCH2, 2, 0, 17 );

    prepareTest ( CT_CAST0, 23, 0, 2 );
    prepareTest ( CT_CAST1, 0, 0, 0x3f800000 );

    prepareTest ( CT_FUNCTION1, 5, 0, 120 );
    prepareTest ( CT_FUNCTION2, 5, 2, 10 );
}

// -----------------------------------------------------------------------------

void KConstructTest :: compareResults()
{
    using namespace vpp;

    for ( size_t i = 0; i != CT_count; ++i )
    {
        const int t = d_target [ i ];
        const int e = d_expected [ i ];
        check ( t == e );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                 Indexing tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum EIndexingTests
{
    IT_LOCALARR1, IT_LOCALARR2, IT_LOCALARR3, IT_LOCALARR4, IT_LOCALARR5,
    IT_SHAREDARR1, IT_SHAREDARR2, IT_SHAREDARR3,

    IT_VECTORS1, IT_VECTORS2, IT_VECTORS3, IT_VECTORS4, IT_VECTORS5, IT_VECTORS6,
    IT_VECTORS7, IT_VECTORS8, IT_VECTORS9, IT_VECTORS10, IT_VECTORS11, IT_VECTORS12,
    IT_VECTORS13, IT_VECTORS14, IT_VECTORS15, IT_VECTORS16, IT_VECTORS17, IT_VECTORS18,

    IT_VECSWIZZLE1_begin,
    IT_VECSWIZZLE1_end = IT_VECSWIZZLE1_begin + 2 + 3 + 4,

    IT_VVECSWIZZLE1_begin = IT_VECSWIZZLE1_end,
    IT_VVECSWIZZLE1_end = IT_VVECSWIZZLE1_begin + 2 + 3 + 4,

    IT_VECSWIZZLE2_begin = IT_VVECSWIZZLE1_end,
    IT_VECSWIZZLE2_end = IT_VECSWIZZLE2_begin + 29,

    IT_VVECSWIZZLE2_begin = IT_VECSWIZZLE2_end,
    IT_VVECSWIZZLE2_end = IT_VVECSWIZZLE2_begin + 3,

    IT_VECSWIZZLE3_begin = IT_VVECSWIZZLE2_end,
    IT_VECSWIZZLE3_end = IT_VECSWIZZLE3_begin + 64,

    IT_VVECSWIZZLE3_begin = IT_VECSWIZZLE3_end,
    IT_VVECSWIZZLE3_end = IT_VVECSWIZZLE3_begin + 4,

    IT_VECSWIZZLE4_begin = IT_VVECSWIZZLE3_end,
    IT_VECSWIZZLE4_end = IT_VECSWIZZLE4_begin + 8,

    IT_VVECSWIZZLE4_begin = IT_VECSWIZZLE4_end,
    IT_VVECSWIZZLE4_end = IT_VVECSWIZZLE4_begin + 4,

    IT_WRVECSWIZZLE4_begin = IT_VVECSWIZZLE4_end,
    IT_WRVECSWIZZLE4_end = IT_WRVECSWIZZLE4_begin + 7,

    IT_SWIZZLEIND_begin = IT_WRVECSWIZZLE4_end,
    IT_SWIZZLEIND_end = IT_SWIZZLEIND_begin + 4,

    IT_MAT_2x2_0 = IT_SWIZZLEIND_end,
    IT_MAT_2x2_1, IT_MAT_2x2_2, IT_MAT_2x2_3,

    IT_MAT_2x2_D0, IT_MAT_2x2_D1, IT_MAT_2x2_D2, IT_MAT_2x2_D3,
    IT_MAT_2x2_C0, IT_MAT_2x2_C1, IT_MAT_2x2_C2, IT_MAT_2x2_C3,
    IT_MAT_2x2_V0, IT_MAT_2x2_V1, IT_MAT_2x2_V2, IT_MAT_2x2_V3,
    IT_MAT_2x2_V10, IT_MAT_2x2_V11, IT_MAT_2x2_V12, IT_MAT_2x2_V13,
    IT_MAT_2x2_V20, IT_MAT_2x2_V21, IT_MAT_2x2_V22, IT_MAT_2x2_V23,
    
    IT_count
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TTestStruct : public vpp::UniformStruct< TAG, TTestStruct >
{
    vpp::UniformFld< TAG, vpp::vect2 > d_vect2;
    vpp::UniformFld< TAG, vpp::vect2 > d_vect2a;
    vpp::UniformFld< TAG, vpp::vect3 > d_vect3;
    vpp::UniformFld< TAG, vpp::vect4 > d_vect4;
    vpp::UniformFld< TAG, vpp::matr2 > d_matr2;
    vpp::UniformFld< TAG, vpp::matr3 > d_matr3;
    vpp::UniformFld< TAG, vpp::matr4 > d_matr4;
};

typedef TTestStruct< vpp::GPU > GTestStruct;
typedef TTestStruct< vpp::CPU > CTestStruct;
typedef vpp::gvector< CTestStruct, vpp::Buf::UNIFORM | vpp::Buf::STORAGE > CTestStructBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KIndexingTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KIndexingTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::UniformBufferView& vs1,
        const vpp::UniformBufferView& vs2,
        const vpp::UniformBufferView& vt,
        int size,
        const vpp::UniformBufferView& tb1,
        const vpp::UniformBufferView& tb2,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::inUniformBuffer d_source1;
    vpp::inUniformBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::inUniformBuffer d_uniTestBuf1;
    vpp::ioBuffer d_storTestBuf1;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KIndexingTestPipeline :: KIndexingTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KIndexingTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KIndexingTestPipeline :: setData (
    const vpp::UniformBufferView& vs1,
    const vpp::UniformBufferView& vs2,
    const vpp::UniformBufferView& vt,
    int size,
    const vpp::UniformBufferView& tb1,
    const vpp::UniformBufferView& tb2,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_source1 = vs1,
        d_source2 = vs2,
        d_target = vt,
        d_uniTestBuf1 = tb1,
        d_storTestBuf1 = tb2
    ));
}

// -----------------------------------------------------------------------------

template< class VecT >
inline vpp::Int testSwizzle ( const VecT& v, vpp::ESwizzle2 s )
{
    using namespace vpp;
    const IVec2 swizzled = v [ s ];
    return swizzled [ 0 ] | ( swizzled [ 1 ] << 2 );
}

// -----------------------------------------------------------------------------

template< class VecT >
inline vpp::Int testSwizzle ( const VecT& v, vpp::ESwizzle3 s )
{
    using namespace vpp;
    const IVec3 swizzled = v [ s ];
    return swizzled [ 0 ] | ( swizzled [ 1 ] << 2 ) | ( swizzled [ 2 ] << 4 );
}

// -----------------------------------------------------------------------------

template< class VecT >
inline vpp::Int testSwizzle ( const VecT& v, vpp::ESwizzle4 s )
{
    using namespace vpp;
    const IVec4 swizzled = v [ s ];
    return swizzled [ 0 ] | ( swizzled [ 1 ] << 2 ) | ( swizzled [ 2 ] << 4 ) | ( swizzled [ 3 ] << 6 );
}

// -----------------------------------------------------------------------------

void KIndexingTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformSimpleArray< int, decltype ( d_source1 ) > inSource1 ( d_source1 );
    UniformSimpleArray< int, decltype ( d_source2 ) > inSource2 ( d_source2 );
    UniformSimpleArray< int, decltype ( d_target ) > outTarget ( d_target );

    UniformVar< TTestStruct, decltype ( d_uniTestBuf1 ) > inUniTestBuf1 ( d_uniTestBuf1 );
    UniformVar< TTestStruct, decltype ( d_storTestBuf1 ) > inStorTestBuf1 ( d_storTestBuf1 );

    VArray< Int, 5 > intArray;
    intArray [ 0 ] = 0;
    intArray [ 1 ] = 1;
    intArray [ 2 ] = 4;
    intArray [ 3 ] = 9;
    intArray [ 4 ] = 16;

    Shared(); VArray< Int, 5 > sharedIntArray;
    sharedIntArray [ 0 ] = 0;
    sharedIntArray [ 1 ] = 0;
    sharedIntArray [ 2 ] = 0;
    sharedIntArray [ 3 ] = 0;
    sharedIntArray [ 4 ] = 0;

    VInt vidx;

    If ( g == 0 && l == 0 );
    {
        const Int idx1 = inSource1 [ IT_LOCALARR1 ];
        outTarget [ IT_LOCALARR1 ] = intArray [ idx1 ];

        const UInt idx2 = StaticCast< UInt >( inSource1 [ IT_LOCALARR2 ] );
        outTarget [ IT_LOCALARR2 ] = intArray [ idx2 ];

        outTarget [ IT_LOCALARR3 ] = intArray [ inSource1 [ IT_LOCALARR3 ] ];

        intArray [ inSource1 [ IT_LOCALARR4 ] ] = intArray [ inSource2 [ IT_LOCALARR4 ] ];
        outTarget [ IT_LOCALARR4 ] = intArray [ inSource1 [ IT_LOCALARR4 ] ];

        outTarget [ IT_LOCALARR5 ] =
            intArray [ inSource1 [ IT_LOCALARR5 ] ]
            + intArray [ inSource2 [ IT_LOCALARR5 ] ];

        sharedIntArray [ 0 ] = 1;
        sharedIntArray [ 1 ] = 2;
        sharedIntArray [ 2 ] = 3;
        sharedIntArray [ 3 ] = 5;
        sharedIntArray [ 4 ] = 7;
    }
    Fi();

    If ( g == 0 && l == 1 );
    {
        outTarget [ IT_SHAREDARR1 ] = sharedIntArray [ inSource1 [ IT_SHAREDARR1 ] ];
        outTarget [ IT_SHAREDARR2 ] = sharedIntArray [ inSource1 [ IT_SHAREDARR2 ] ];
        outTarget [ IT_SHAREDARR3 ] = sharedIntArray [ inSource1 [ IT_SHAREDARR3 ] ];
    }
    Fi();

    If ( g == 0 && l == 0 );
    {
        // normal

        IVec2 ivec2 { 11, 13 };
        outTarget [ IT_VECTORS1 ] = ivec2 [ 1 ];
        const Int idx2 = inSource1 [ IT_VECTORS2 ];
        outTarget [ IT_VECTORS2 ] = ivec2 [ idx2 ];
        outTarget [ IT_VECTORS3 ] = ivec2 [ inSource1 [ IT_VECTORS3 ] ];

        IVec3 ivec3 { 17, 19, 23 };
        outTarget [ IT_VECTORS4 ] = ivec3 [ 1 ];
        const Int idx3 = inSource1 [ IT_VECTORS5 ];
        outTarget [ IT_VECTORS5 ] = ivec3 [ idx3 ];
        outTarget [ IT_VECTORS6 ] = ivec3 [ inSource1 [ IT_VECTORS6 ] ];

        IVec4 ivec4 { 29, 31, 37, 41 };
        outTarget [ IT_VECTORS7 ] = ivec4 [ 1 ];
        const Int idx4 = inSource1 [ IT_VECTORS8 ];
        outTarget [ IT_VECTORS8 ] = ivec4 [ idx4 ];
        outTarget [ IT_VECTORS9 ] = ivec4 [ inSource1 [ IT_VECTORS9 ] ];

        VIVec2 vivec2 = ivec2;
        outTarget [ IT_VECTORS10 ] = vivec2 [ 1 ];
        const Int idx5 = inSource1 [ IT_VECTORS11 ];
        outTarget [ IT_VECTORS11 ] = vivec2 [ idx5 ];
        outTarget [ IT_VECTORS12 ] = vivec2 [ inSource1 [ IT_VECTORS12 ] ];

        VIVec3 vivec3;
        vivec3 = ivec3;
        outTarget [ IT_VECTORS13 ] = vivec3 [ 1 ];
        const Int idx6 = inSource1 [ IT_VECTORS14 ];
        outTarget [ IT_VECTORS14 ] = vivec3 [ idx6 ];
        outTarget [ IT_VECTORS15 ] = vivec3 [ inSource1 [ IT_VECTORS15 ] ];

        VIVec4 vivec4 { 1, 1, 1, 1 };

        For ( vidx, 0, 4 );
            vivec4 [ vidx ] = ivec4 [ vidx ];
        Rof();

        outTarget [ IT_VECTORS16 ] = vivec4 [ 1 ];
        const Int idx7 = inSource1 [ IT_VECTORS17 ];
        outTarget [ IT_VECTORS17 ] = vivec4 [ idx7 ];
        outTarget [ IT_VECTORS18 ] = vivec4 [ inSource1 [ IT_VECTORS18 ] ];

        // swizzled reads

        outTarget [ IT_VECSWIZZLE1_begin + 0 ] = ivec2 [ X ];
        outTarget [ IT_VECSWIZZLE1_begin + 1 ] = ivec2 [ Y ];
        outTarget [ IT_VECSWIZZLE1_begin + 2 ] = ivec3 [ X ];
        outTarget [ IT_VECSWIZZLE1_begin + 3 ] = ivec3 [ Y ];
        outTarget [ IT_VECSWIZZLE1_begin + 4 ] = ivec3 [ Z ];
        outTarget [ IT_VECSWIZZLE1_begin + 5 ] = ivec4 [ X ];
        outTarget [ IT_VECSWIZZLE1_begin + 6 ] = ivec4 [ Y ];
        outTarget [ IT_VECSWIZZLE1_begin + 7 ] = ivec4 [ Z ];
        outTarget [ IT_VECSWIZZLE1_begin + 8 ] = ivec4 [ W ];
        outTarget [ IT_VVECSWIZZLE1_begin + 0 ] = vivec2 [ X ];
        outTarget [ IT_VVECSWIZZLE1_begin + 1 ] = vivec2 [ Y ];
        outTarget [ IT_VVECSWIZZLE1_begin + 2 ] = vivec3 [ X ];
        outTarget [ IT_VVECSWIZZLE1_begin + 3 ] = vivec3 [ Y ];
        outTarget [ IT_VVECSWIZZLE1_begin + 4 ] = vivec3 [ Z ];
        outTarget [ IT_VVECSWIZZLE1_begin + 5 ] = vivec4 [ X ];
        outTarget [ IT_VVECSWIZZLE1_begin + 6 ] = vivec4 [ Y ];
        outTarget [ IT_VVECSWIZZLE1_begin + 7 ] = vivec4 [ Z ];
        outTarget [ IT_VVECSWIZZLE1_begin + 8 ] = vivec4 [ W ];

        IVec2 swtest2 { 0, 1 };
        IVec3 swtest3 { 0, 1, 2 };
        IVec4 swtest4 { 0, 1, 2, 3 };

        outTarget [ IT_VECSWIZZLE2_begin +  0 ] = testSwizzle ( swtest2, XX );
        outTarget [ IT_VECSWIZZLE2_begin +  1 ] = testSwizzle ( swtest2, YX );
        outTarget [ IT_VECSWIZZLE2_begin +  2 ] = testSwizzle ( swtest2, XY );
        outTarget [ IT_VECSWIZZLE2_begin +  3 ] = testSwizzle ( swtest2, YY );
        outTarget [ IT_VECSWIZZLE2_begin +  4 ] = testSwizzle ( swtest3, XX );
        outTarget [ IT_VECSWIZZLE2_begin +  5 ] = testSwizzle ( swtest3, YX );
        outTarget [ IT_VECSWIZZLE2_begin +  6 ] = testSwizzle ( swtest3, ZX );
        outTarget [ IT_VECSWIZZLE2_begin +  7 ] = testSwizzle ( swtest3, XY );
        outTarget [ IT_VECSWIZZLE2_begin +  8 ] = testSwizzle ( swtest3, YY );
        outTarget [ IT_VECSWIZZLE2_begin +  9 ] = testSwizzle ( swtest3, ZY );
        outTarget [ IT_VECSWIZZLE2_begin + 10 ] = testSwizzle ( swtest3, XZ );
        outTarget [ IT_VECSWIZZLE2_begin + 11 ] = testSwizzle ( swtest3, YZ );
        outTarget [ IT_VECSWIZZLE2_begin + 12 ] = testSwizzle ( swtest3, ZZ );
        outTarget [ IT_VECSWIZZLE2_begin + 13 ] = testSwizzle ( swtest4, XX );
        outTarget [ IT_VECSWIZZLE2_begin + 14 ] = testSwizzle ( swtest4, YX );
        outTarget [ IT_VECSWIZZLE2_begin + 15 ] = testSwizzle ( swtest4, ZX );
        outTarget [ IT_VECSWIZZLE2_begin + 16 ] = testSwizzle ( swtest4, WX );
        outTarget [ IT_VECSWIZZLE2_begin + 17 ] = testSwizzle ( swtest4, XY );
        outTarget [ IT_VECSWIZZLE2_begin + 18 ] = testSwizzle ( swtest4, YY );
        outTarget [ IT_VECSWIZZLE2_begin + 19 ] = testSwizzle ( swtest4, ZY );
        outTarget [ IT_VECSWIZZLE2_begin + 20 ] = testSwizzle ( swtest4, WY );
        outTarget [ IT_VECSWIZZLE2_begin + 21 ] = testSwizzle ( swtest4, XZ );
        outTarget [ IT_VECSWIZZLE2_begin + 22 ] = testSwizzle ( swtest4, YZ );
        outTarget [ IT_VECSWIZZLE2_begin + 23 ] = testSwizzle ( swtest4, ZZ );
        outTarget [ IT_VECSWIZZLE2_begin + 24 ] = testSwizzle ( swtest4, WZ );
        outTarget [ IT_VECSWIZZLE2_begin + 25 ] = testSwizzle ( swtest4, XW );
        outTarget [ IT_VECSWIZZLE2_begin + 26 ] = testSwizzle ( swtest4, YW );
        outTarget [ IT_VECSWIZZLE2_begin + 27 ] = testSwizzle ( swtest4, ZW );
        outTarget [ IT_VECSWIZZLE2_begin + 28 ] = testSwizzle ( swtest4, WW );

        vivec2 = swtest2;
        vivec3 = swtest3;
        vivec4 = swtest4;

        outTarget [ IT_VVECSWIZZLE2_begin + 0 ] = testSwizzle ( vivec2, YY );
        outTarget [ IT_VVECSWIZZLE2_begin + 1 ] = testSwizzle ( vivec3, ZZ );
        outTarget [ IT_VVECSWIZZLE2_begin + 2 ] = testSwizzle ( vivec4, WW );

        outTarget [ IT_VECSWIZZLE3_begin +  0 ] = testSwizzle ( swtest4, XXX );
        outTarget [ IT_VECSWIZZLE3_begin +  1 ] = testSwizzle ( swtest4, YXX );
        outTarget [ IT_VECSWIZZLE3_begin +  1 ] = testSwizzle ( swtest4, ZXX );
        outTarget [ IT_VECSWIZZLE3_begin +  3 ] = testSwizzle ( swtest4, WXX );
        outTarget [ IT_VECSWIZZLE3_begin +  4 ] = testSwizzle ( swtest4, XYX );
        outTarget [ IT_VECSWIZZLE3_begin +  5 ] = testSwizzle ( swtest4, YYX );
        outTarget [ IT_VECSWIZZLE3_begin +  6 ] = testSwizzle ( swtest4, ZYX );
        outTarget [ IT_VECSWIZZLE3_begin +  7 ] = testSwizzle ( swtest4, WYX );
        outTarget [ IT_VECSWIZZLE3_begin +  8 ] = testSwizzle ( swtest4, XZX );
        outTarget [ IT_VECSWIZZLE3_begin +  9 ] = testSwizzle ( swtest4, YZX );
        outTarget [ IT_VECSWIZZLE3_begin + 10 ] = testSwizzle ( swtest4, ZZX );
        outTarget [ IT_VECSWIZZLE3_begin + 11 ] = testSwizzle ( swtest4, WZX );
        outTarget [ IT_VECSWIZZLE3_begin + 11 ] = testSwizzle ( swtest4, XWX );
        outTarget [ IT_VECSWIZZLE3_begin + 13 ] = testSwizzle ( swtest4, YWX );
        outTarget [ IT_VECSWIZZLE3_begin + 14 ] = testSwizzle ( swtest4, ZWX );
        outTarget [ IT_VECSWIZZLE3_begin + 15 ] = testSwizzle ( swtest4, WWX );
        outTarget [ IT_VECSWIZZLE3_begin + 16 ] = testSwizzle ( swtest4, XXY );
        outTarget [ IT_VECSWIZZLE3_begin + 17 ] = testSwizzle ( swtest4, YXY );
        outTarget [ IT_VECSWIZZLE3_begin + 18 ] = testSwizzle ( swtest4, ZXY );
        outTarget [ IT_VECSWIZZLE3_begin + 19 ] = testSwizzle ( swtest4, WXY );
        outTarget [ IT_VECSWIZZLE3_begin + 20 ] = testSwizzle ( swtest4, XYY );
        outTarget [ IT_VECSWIZZLE3_begin + 21 ] = testSwizzle ( swtest4, YYY );
        outTarget [ IT_VECSWIZZLE3_begin + 21 ] = testSwizzle ( swtest4, ZYY );
        outTarget [ IT_VECSWIZZLE3_begin + 23 ] = testSwizzle ( swtest4, WYY );
        outTarget [ IT_VECSWIZZLE3_begin + 24 ] = testSwizzle ( swtest4, XZY );
        outTarget [ IT_VECSWIZZLE3_begin + 25 ] = testSwizzle ( swtest4, YZY );
        outTarget [ IT_VECSWIZZLE3_begin + 26 ] = testSwizzle ( swtest4, ZZY );
        outTarget [ IT_VECSWIZZLE3_begin + 27 ] = testSwizzle ( swtest4, WZY );
        outTarget [ IT_VECSWIZZLE3_begin + 28 ] = testSwizzle ( swtest4, XWY );
        outTarget [ IT_VECSWIZZLE3_begin + 29 ] = testSwizzle ( swtest4, YWY );
        outTarget [ IT_VECSWIZZLE3_begin + 30 ] = testSwizzle ( swtest4, ZWY );
        outTarget [ IT_VECSWIZZLE3_begin + 31 ] = testSwizzle ( swtest4, WWY );
        outTarget [ IT_VECSWIZZLE3_begin + 31 ] = testSwizzle ( swtest4, XXZ );
        outTarget [ IT_VECSWIZZLE3_begin + 33 ] = testSwizzle ( swtest4, YXZ );
        outTarget [ IT_VECSWIZZLE3_begin + 34 ] = testSwizzle ( swtest4, ZXZ );
        outTarget [ IT_VECSWIZZLE3_begin + 35 ] = testSwizzle ( swtest4, WXZ );
        outTarget [ IT_VECSWIZZLE3_begin + 36 ] = testSwizzle ( swtest4, XYZ );
        outTarget [ IT_VECSWIZZLE3_begin + 37 ] = testSwizzle ( swtest4, YYZ );
        outTarget [ IT_VECSWIZZLE3_begin + 38 ] = testSwizzle ( swtest4, ZYZ );
        outTarget [ IT_VECSWIZZLE3_begin + 39 ] = testSwizzle ( swtest4, WYZ );
        outTarget [ IT_VECSWIZZLE3_begin + 40 ] = testSwizzle ( swtest4, XZZ );
        outTarget [ IT_VECSWIZZLE3_begin + 41 ] = testSwizzle ( swtest4, YZZ );
        outTarget [ IT_VECSWIZZLE3_begin + 41 ] = testSwizzle ( swtest4, ZZZ );
        outTarget [ IT_VECSWIZZLE3_begin + 43 ] = testSwizzle ( swtest4, WZZ );
        outTarget [ IT_VECSWIZZLE3_begin + 44 ] = testSwizzle ( swtest4, XWZ );
        outTarget [ IT_VECSWIZZLE3_begin + 45 ] = testSwizzle ( swtest4, YWZ );
        outTarget [ IT_VECSWIZZLE3_begin + 46 ] = testSwizzle ( swtest4, ZWZ );
        outTarget [ IT_VECSWIZZLE3_begin + 47 ] = testSwizzle ( swtest4, WWZ );
        outTarget [ IT_VECSWIZZLE3_begin + 48 ] = testSwizzle ( swtest4, XXW );
        outTarget [ IT_VECSWIZZLE3_begin + 49 ] = testSwizzle ( swtest4, YXW );
        outTarget [ IT_VECSWIZZLE3_begin + 50 ] = testSwizzle ( swtest4, ZXW );
        outTarget [ IT_VECSWIZZLE3_begin + 51 ] = testSwizzle ( swtest4, WXW );
        outTarget [ IT_VECSWIZZLE3_begin + 51 ] = testSwizzle ( swtest4, XYW );
        outTarget [ IT_VECSWIZZLE3_begin + 53 ] = testSwizzle ( swtest4, YYW );
        outTarget [ IT_VECSWIZZLE3_begin + 54 ] = testSwizzle ( swtest4, ZYW );
        outTarget [ IT_VECSWIZZLE3_begin + 55 ] = testSwizzle ( swtest4, WYW );
        outTarget [ IT_VECSWIZZLE3_begin + 56 ] = testSwizzle ( swtest4, XZW );
        outTarget [ IT_VECSWIZZLE3_begin + 57 ] = testSwizzle ( swtest4, YZW );
        outTarget [ IT_VECSWIZZLE3_begin + 58 ] = testSwizzle ( swtest4, ZZW );
        outTarget [ IT_VECSWIZZLE3_begin + 59 ] = testSwizzle ( swtest4, WZW );
        outTarget [ IT_VECSWIZZLE3_begin + 60 ] = testSwizzle ( swtest4, XWW );
        outTarget [ IT_VECSWIZZLE3_begin + 61 ] = testSwizzle ( swtest4, YWW );
        outTarget [ IT_VECSWIZZLE3_begin + 62 ] = testSwizzle ( swtest4, ZWW );
        outTarget [ IT_VECSWIZZLE3_begin + 63 ] = testSwizzle ( swtest4, WWW );

        outTarget [ IT_VVECSWIZZLE3_begin + 0 ] = testSwizzle ( vivec4, YYY );
        outTarget [ IT_VVECSWIZZLE3_begin + 1 ] = testSwizzle ( vivec4, WYX );
        outTarget [ IT_VVECSWIZZLE3_begin + 2 ] = testSwizzle ( vivec4, ZYW );
        outTarget [ IT_VVECSWIZZLE3_begin + 3 ] = testSwizzle ( vivec4, WWW );

        outTarget [ IT_VECSWIZZLE4_begin + 0 ] = testSwizzle ( swtest4, XYZW );
        outTarget [ IT_VECSWIZZLE4_begin + 1 ] = testSwizzle ( swtest4, WZYX );
        outTarget [ IT_VECSWIZZLE4_begin + 2 ] = testSwizzle ( swtest4, YXWZ );
        outTarget [ IT_VECSWIZZLE4_begin + 3 ] = testSwizzle ( swtest4, ZWXY );
        outTarget [ IT_VECSWIZZLE4_begin + 4 ] = testSwizzle ( swtest4, XXXX );
        outTarget [ IT_VECSWIZZLE4_begin + 5 ] = testSwizzle ( swtest4, YYYY );
        outTarget [ IT_VECSWIZZLE4_begin + 6 ] = testSwizzle ( swtest4, ZZZZ );
        outTarget [ IT_VECSWIZZLE4_begin + 7 ] = testSwizzle ( swtest4, WWWW );

        outTarget [ IT_VVECSWIZZLE4_begin + 0 ] = testSwizzle ( vivec4, XYZW );
        outTarget [ IT_VVECSWIZZLE4_begin + 1 ] = testSwizzle ( vivec4, WZYX );
        outTarget [ IT_VVECSWIZZLE4_begin + 2 ] = testSwizzle ( vivec4, YXWZ );
        outTarget [ IT_VVECSWIZZLE4_begin + 3 ] = testSwizzle ( vivec4, ZWXY );

        // swizzled writes
        vivec4 [ X ] = 3;
        vivec4 [ Y ] = 2;
        vivec4 [ Z ] = 1;
        vivec4 [ W ] = 0;

        outTarget [ IT_WRVECSWIZZLE4_begin ] = testSwizzle ( vivec4, XYZW );

        vivec4 [ WZYX ] = vivec4;
        outTarget [ IT_WRVECSWIZZLE4_begin + 1 ] = testSwizzle ( vivec4, XYZW );

        vivec4 [ ZX ] = swtest4 [ WW ];
        vivec4 [ WY ] = swtest4 [ YY ];
        outTarget [ IT_WRVECSWIZZLE4_begin + 2 ] = testSwizzle ( vivec4, XYZW );

        vivec3 [ X ] = 3;
        vivec3 [ Y ] = 2;
        vivec3 [ Z ] = 1;

        outTarget [ IT_WRVECSWIZZLE4_begin + 3 ] = testSwizzle ( vivec3, XYZ );

        vivec3 [ ZYX ] = vivec3;
        outTarget [ IT_WRVECSWIZZLE4_begin + 4 ] = testSwizzle ( vivec3, XYZ );

        vivec2 [ X ] = 2;
        vivec2 [ Y ] = 1;

        outTarget [ IT_WRVECSWIZZLE4_begin + 5 ] = testSwizzle ( vivec2, XY );

        vivec2 [ YX ] = vivec2;
        outTarget [ IT_WRVECSWIZZLE4_begin + 6 ] = testSwizzle ( vivec2, XY );

        // Swizzles combined with normal indexing
        outTarget [ IT_SWIZZLEIND_begin + 0 ] = swtest4 [ WZYX ][ 0 ];
        outTarget [ IT_SWIZZLEIND_begin + 1 ] = swtest4 [ WZYX ][ 1 ];
        outTarget [ IT_SWIZZLEIND_begin + 2 ] = swtest4 [ WZYX ][ 2 ];
        outTarget [ IT_SWIZZLEIND_begin + 3 ] = swtest4 [ WZYX ][ 3 ];

        // 2x2 matrices
        IMat2 imat2 { 11, 12, 21, 22 };

        outTarget [ IT_MAT_2x2_0 ] = imat2 [ 0 ][ 0 ];
        outTarget [ IT_MAT_2x2_1 ] = imat2 [ 1 ][ 0 ];
        outTarget [ IT_MAT_2x2_2 ] = imat2 [ 0 ][ 1 ];
        outTarget [ IT_MAT_2x2_3 ] = imat2 [ 1 ][ 1 ];

        outTarget [ IT_MAT_2x2_D0 ] = imat2 [ inSource1 [ IT_MAT_2x2_D0 ] ][ inSource2 [ IT_MAT_2x2_D0 ] ];
        outTarget [ IT_MAT_2x2_D1 ] = imat2 [ inSource1 [ IT_MAT_2x2_D1 ] ][ inSource2 [ IT_MAT_2x2_D1 ] ];
        outTarget [ IT_MAT_2x2_D2 ] = imat2 [ inSource1 [ IT_MAT_2x2_D2 ] ][ inSource2 [ IT_MAT_2x2_D2 ] ];
        outTarget [ IT_MAT_2x2_D3 ] = imat2 [ inSource1 [ IT_MAT_2x2_D3 ] ][ inSource2 [ IT_MAT_2x2_D3 ] ];

        IVec2 imat20 = imat2 [ 0 ];
        IVec2 imat21 = imat2 [ 1 ];

        outTarget [ IT_MAT_2x2_C0 ] = imat20 [ 0 ];
        outTarget [ IT_MAT_2x2_C1 ] = imat21 [ 0 ];
        outTarget [ IT_MAT_2x2_C2 ] = imat20 [ 1 ];
        outTarget [ IT_MAT_2x2_C3 ] = imat21 [ 1 ];

        VIMat2 vimat2 = imat2;
        outTarget [ IT_MAT_2x2_V0 ] = vimat2 [ 0 ][ 0 ];
        outTarget [ IT_MAT_2x2_V1 ] = vimat2 [ 1 ][ 0 ];
        outTarget [ IT_MAT_2x2_V2 ] = vimat2 [ 0 ][ 1 ];
        outTarget [ IT_MAT_2x2_V3 ] = vimat2 [ 1 ][ 1 ];

        vimat2 [ 0 ][ 0 ] = 111;
        vimat2 [ 1 ][ 0 ] = 112;
        vimat2 [ 0 ][ 1 ] = 121;
        vimat2 [ 1 ][ 1 ] = 122;
        outTarget [ IT_MAT_2x2_V10 ] = vimat2 [ 0 ][ 0 ];
        outTarget [ IT_MAT_2x2_V11 ] = vimat2 [ 1 ][ 0 ];
        outTarget [ IT_MAT_2x2_V12 ] = vimat2 [ 0 ][ 1 ];
        outTarget [ IT_MAT_2x2_V13 ] = vimat2 [ 1 ][ 1 ];

        vimat2 [ 0 ] = imat2 [ 0 ];
        vimat2 [ 1 ] = imat2 [ 1 ];
        outTarget [ IT_MAT_2x2_V20 ] = vimat2 [ 0 ][ 0 ];
        outTarget [ IT_MAT_2x2_V21 ] = vimat2 [ 1 ][ 0 ];
        outTarget [ IT_MAT_2x2_V22 ] = vimat2 [ 0 ][ 1 ];
        outTarget [ IT_MAT_2x2_V23 ] = vimat2 [ 1 ][ 1 ];

        // ---------------------------------------------------------

        inStorTestBuf1 [ & GTestStruct::d_matr2 ][ 0 ][ 0 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr2 ][ 0 ][ 0 ];
        inStorTestBuf1 [ & GTestStruct::d_matr2 ][ 0 ][ 1 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr2 ][ 0 ][ 1 ];
        inStorTestBuf1 [ & GTestStruct::d_matr2 ][ 1 ][ 0 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr2 ][ 1 ][ 0 ];
        inStorTestBuf1 [ & GTestStruct::d_matr2 ][ 1 ][ 1 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr2 ][ 1 ][ 1 ];

        inStorTestBuf1 [ & GTestStruct::d_matr3 ][ 0 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr3 ][ 0 ];
        inStorTestBuf1 [ & GTestStruct::d_matr3 ][ 1 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr3 ][ 1 ];
        inStorTestBuf1 [ & GTestStruct::d_matr3 ][ 2 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr3 ][ 2 ];

        inStorTestBuf1 [ & GTestStruct::d_matr4 ] =
            inUniTestBuf1 [ & GTestStruct::d_matr4 ];

        inStorTestBuf1 [ & GTestStruct::d_vect2 ][ 0 ] =
            inUniTestBuf1 [ & GTestStruct::d_vect2 ][ 0 ];
        inStorTestBuf1 [ & GTestStruct::d_vect2 ][ 1 ] =
            inUniTestBuf1 [ & GTestStruct::d_vect2 ][ 1 ];
        inStorTestBuf1 [ & GTestStruct::d_vect2a ] =
            inUniTestBuf1 [ & GTestStruct::d_vect2a ];
        inStorTestBuf1 [ & GTestStruct::d_vect3 ] =
            inUniTestBuf1 [ & GTestStruct::d_vect3 ];
        inStorTestBuf1 [ & GTestStruct::d_vect4 ] =
            inUniTestBuf1 [ & GTestStruct::d_vect4 ];
    }
    Fi();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KIndexingTest : public vpp::Computation
{
public:
    KIndexingTest ( const vpp::Device& hDevice );
    KIndexingTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    void prepareTest ( int testId, int s1, int s2, int exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef KIndexingTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< int, vpp::Buf::STORAGE > IntVector;

    IntVector d_source1;
    IntVector d_source2;
    IntVector d_target;

    std::vector< int > d_expected;

    CTestStructBuffer d_uniTestBuf1;
    CTestStructBuffer d_storTestBuf1;
};

// -----------------------------------------------------------------------------

KIndexingTest :: KIndexingTest ( const vpp::Device& hDevice ) :
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_uniTestBuf1 ( 1, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_storTestBuf1 ( 1, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        d_uniTestBuf1, d_storTestBuf1,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        d_uniTestBuf1.cmdCommit();
        d_storTestBuf1.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        cmdBufferPipelineBarrier (
            d_storTestBuf1,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
        d_storTestBuf1.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

KIndexingTest :: KIndexingTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_uniTestBuf1 ( 1, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_storTestBuf1 ( 1, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData (
        d_source1, d_source2, d_target, static_cast< int >( d_target.size() ),
        d_uniTestBuf1, d_storTestBuf1,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    prepareVectors();

    ( *this ) << [ this ]()
    {
        d_source1.cmdCommit();
        d_source2.cmdCommit();
        d_target.cmdCommit();
        d_uniTestBuf1.cmdCommit();
        d_storTestBuf1.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        cmdBufferPipelineBarrier (
            d_storTestBuf1,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
        d_storTestBuf1.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KIndexingTest :: prepareTest (
    int testId, int s1, int s2, int exp )
{
    d_source1 [ testId ] = s1;
    d_source2 [ testId ] = s2;
    d_target [ testId ] = 0;
    d_expected [ testId ] = exp;
}

// -----------------------------------------------------------------------------

void KIndexingTest :: prepareVectors()
{
    using namespace vpp;

    const unsigned int nItems = IT_count;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    d_uniTestBuf1.resize ( 1 );
    d_storTestBuf1.resize ( 1 );

    prepareTest ( IT_LOCALARR1, 3, 0, 9 );
    prepareTest ( IT_LOCALARR2, 4, 0, 16 );
    prepareTest ( IT_LOCALARR3, 2, 0, 4 );
    prepareTest ( IT_LOCALARR4, 0, 4, 16 );
    prepareTest ( IT_LOCALARR5, 3, 4, 25 );
    prepareTest ( IT_SHAREDARR1, 0, 0, 1 );
    prepareTest ( IT_SHAREDARR2, 2, 0, 3 );
    prepareTest ( IT_SHAREDARR3, 4, 0, 7 );
    prepareTest ( IT_VECTORS1, 0, 0, 13 );
    prepareTest ( IT_VECTORS2, 1, 0, 13 );
    prepareTest ( IT_VECTORS3, 1, 0, 13 );
    prepareTest ( IT_VECTORS4, 0, 0, 19 );
    prepareTest ( IT_VECTORS5, 1, 0, 19 );
    prepareTest ( IT_VECTORS6, 2, 0, 23 );
    prepareTest ( IT_VECTORS7, 0, 0, 31 );
    prepareTest ( IT_VECTORS8, 2, 0, 37 );
    prepareTest ( IT_VECTORS9, 3, 0, 41 );
    prepareTest ( IT_VECTORS10, 0, 0, 13 );
    prepareTest ( IT_VECTORS11, 1, 0, 13 );
    prepareTest ( IT_VECTORS12, 1, 0, 13 );
    prepareTest ( IT_VECTORS13, 0, 0, 19 );
    prepareTest ( IT_VECTORS14, 1, 0, 19 );
    prepareTest ( IT_VECTORS15, 2, 0, 23 );
    prepareTest ( IT_VECTORS16, 0, 0, 31 );
    prepareTest ( IT_VECTORS17, 2, 0, 37 );
    prepareTest ( IT_VECTORS18, 3, 0, 41 );

    prepareTest ( IT_VECSWIZZLE1_begin + 0, 0, 0, 11 );
    prepareTest ( IT_VECSWIZZLE1_begin + 1, 0, 0, 13 );
    prepareTest ( IT_VECSWIZZLE1_begin + 2, 0, 0, 17 );
    prepareTest ( IT_VECSWIZZLE1_begin + 3, 0, 0, 19 );
    prepareTest ( IT_VECSWIZZLE1_begin + 4, 0, 0, 23 );
    prepareTest ( IT_VECSWIZZLE1_begin + 5, 0, 0, 29 );
    prepareTest ( IT_VECSWIZZLE1_begin + 6, 0, 0, 31 );
    prepareTest ( IT_VECSWIZZLE1_begin + 7, 0, 0, 37 );
    prepareTest ( IT_VECSWIZZLE1_begin + 8, 0, 0, 41 );

    prepareTest ( IT_VVECSWIZZLE1_begin + 0, 0, 0, 11 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 1, 0, 0, 13 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 2, 0, 0, 17 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 3, 0, 0, 19 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 4, 0, 0, 23 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 5, 0, 0, 29 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 6, 0, 0, 31 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 7, 0, 0, 37 );
    prepareTest ( IT_VVECSWIZZLE1_begin + 8, 0, 0, 41 );

    prepareTest ( IT_VECSWIZZLE2_begin + 0, 0, 0, 0b0000 );
    prepareTest ( IT_VECSWIZZLE2_begin + 1, 0, 0, 0b0001 );
    prepareTest ( IT_VECSWIZZLE2_begin + 2, 0, 0, 0b0100 );
    prepareTest ( IT_VECSWIZZLE2_begin + 3, 0, 0, 0b0101 );
    prepareTest ( IT_VECSWIZZLE2_begin +  4, 0, 0, 0b0000 );
    prepareTest ( IT_VECSWIZZLE2_begin +  5, 0, 0, 0b0001 );
    prepareTest ( IT_VECSWIZZLE2_begin +  6, 0, 0, 0b0010 );
    prepareTest ( IT_VECSWIZZLE2_begin +  7, 0, 0, 0b0100 );
    prepareTest ( IT_VECSWIZZLE2_begin +  8, 0, 0, 0b0101 );
    prepareTest ( IT_VECSWIZZLE2_begin +  9, 0, 0, 0b0110 );
    prepareTest ( IT_VECSWIZZLE2_begin + 10, 0, 0, 0b1000 );
    prepareTest ( IT_VECSWIZZLE2_begin + 11, 0, 0, 0b1001 );
    prepareTest ( IT_VECSWIZZLE2_begin + 12, 0, 0, 0b1010 );
    prepareTest ( IT_VECSWIZZLE2_begin + 13, 0, 0, 0b0000 );
    prepareTest ( IT_VECSWIZZLE2_begin + 14, 0, 0, 0b0001 );
    prepareTest ( IT_VECSWIZZLE2_begin + 15, 0, 0, 0b0010 );
    prepareTest ( IT_VECSWIZZLE2_begin + 16, 0, 0, 0b0011 );
    prepareTest ( IT_VECSWIZZLE2_begin + 17, 0, 0, 0b0100 );
    prepareTest ( IT_VECSWIZZLE2_begin + 18, 0, 0, 0b0101 );
    prepareTest ( IT_VECSWIZZLE2_begin + 19, 0, 0, 0b0110 );
    prepareTest ( IT_VECSWIZZLE2_begin + 20, 0, 0, 0b0111 );
    prepareTest ( IT_VECSWIZZLE2_begin + 21, 0, 0, 0b1000 );
    prepareTest ( IT_VECSWIZZLE2_begin + 22, 0, 0, 0b1001 );
    prepareTest ( IT_VECSWIZZLE2_begin + 23, 0, 0, 0b1010 );
    prepareTest ( IT_VECSWIZZLE2_begin + 24, 0, 0, 0b1011 );
    prepareTest ( IT_VECSWIZZLE2_begin + 25, 0, 0, 0b1100 );
    prepareTest ( IT_VECSWIZZLE2_begin + 26, 0, 0, 0b1101 );
    prepareTest ( IT_VECSWIZZLE2_begin + 27, 0, 0, 0b1110 );
    prepareTest ( IT_VECSWIZZLE2_begin + 28, 0, 0, 0b1111 );

    prepareTest ( IT_VVECSWIZZLE2_begin + 0, 0, 0, 0b0101 );
    prepareTest ( IT_VVECSWIZZLE2_begin + 1, 0, 0, 0b1010 );
    prepareTest ( IT_VVECSWIZZLE2_begin + 2, 0, 0, 0b1111 );

    prepareTest ( IT_VECSWIZZLE3_begin +  0, 0, 0, 0b000000 );
    prepareTest ( IT_VECSWIZZLE3_begin +  1, 0, 0, 0b000001 );
    prepareTest ( IT_VECSWIZZLE3_begin +  1, 0, 0, 0b000010 );
    prepareTest ( IT_VECSWIZZLE3_begin +  3, 0, 0, 0b000011 );
    prepareTest ( IT_VECSWIZZLE3_begin +  4, 0, 0, 0b000100 );
    prepareTest ( IT_VECSWIZZLE3_begin +  5, 0, 0, 0b000101 );
    prepareTest ( IT_VECSWIZZLE3_begin +  6, 0, 0, 0b000110 );
    prepareTest ( IT_VECSWIZZLE3_begin +  7, 0, 0, 0b000111 );
    prepareTest ( IT_VECSWIZZLE3_begin +  8, 0, 0, 0b001000 );
    prepareTest ( IT_VECSWIZZLE3_begin +  9, 0, 0, 0b001001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 10, 0, 0, 0b001010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 11, 0, 0, 0b001011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 11, 0, 0, 0b001100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 13, 0, 0, 0b001101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 14, 0, 0, 0b001110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 15, 0, 0, 0b001111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 16, 0, 0, 0b010000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 17, 0, 0, 0b010001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 18, 0, 0, 0b010010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 19, 0, 0, 0b010011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 20, 0, 0, 0b010100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 21, 0, 0, 0b010101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 21, 0, 0, 0b010110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 23, 0, 0, 0b010111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 24, 0, 0, 0b011000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 25, 0, 0, 0b011001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 26, 0, 0, 0b011010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 27, 0, 0, 0b011011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 28, 0, 0, 0b011100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 29, 0, 0, 0b011101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 30, 0, 0, 0b011110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 31, 0, 0, 0b011111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 31, 0, 0, 0b100000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 33, 0, 0, 0b100001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 34, 0, 0, 0b100010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 35, 0, 0, 0b100011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 36, 0, 0, 0b100100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 37, 0, 0, 0b100101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 38, 0, 0, 0b100110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 39, 0, 0, 0b100111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 40, 0, 0, 0b101000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 41, 0, 0, 0b101001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 41, 0, 0, 0b101010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 43, 0, 0, 0b101011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 44, 0, 0, 0b101100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 45, 0, 0, 0b101101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 46, 0, 0, 0b101110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 47, 0, 0, 0b101111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 48, 0, 0, 0b110000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 49, 0, 0, 0b110001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 50, 0, 0, 0b110010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 51, 0, 0, 0b110011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 51, 0, 0, 0b110100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 53, 0, 0, 0b110101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 54, 0, 0, 0b110110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 55, 0, 0, 0b110111 );
    prepareTest ( IT_VECSWIZZLE3_begin + 56, 0, 0, 0b111000 );
    prepareTest ( IT_VECSWIZZLE3_begin + 57, 0, 0, 0b111001 );
    prepareTest ( IT_VECSWIZZLE3_begin + 58, 0, 0, 0b111010 );
    prepareTest ( IT_VECSWIZZLE3_begin + 59, 0, 0, 0b111011 );
    prepareTest ( IT_VECSWIZZLE3_begin + 60, 0, 0, 0b111100 );
    prepareTest ( IT_VECSWIZZLE3_begin + 61, 0, 0, 0b111101 );
    prepareTest ( IT_VECSWIZZLE3_begin + 62, 0, 0, 0b111110 );
    prepareTest ( IT_VECSWIZZLE3_begin + 63, 0, 0, 0b111111 );

    prepareTest ( IT_VVECSWIZZLE3_begin + 0, 0, 0, 0b010101 );
    prepareTest ( IT_VVECSWIZZLE3_begin + 1, 0, 0, 0b000111 );
    prepareTest ( IT_VVECSWIZZLE3_begin + 2, 0, 0, 0b110110 );
    prepareTest ( IT_VVECSWIZZLE3_begin + 3, 0, 0, 0b111111 );

    prepareTest ( IT_VECSWIZZLE4_begin + 0, 0, 0, 0b11100100 );
    prepareTest ( IT_VECSWIZZLE4_begin + 1, 0, 0, 0b00011011 );
    prepareTest ( IT_VECSWIZZLE4_begin + 2, 0, 0, 0b10110001 );
    prepareTest ( IT_VECSWIZZLE4_begin + 3, 0, 0, 0b01001110 );
    prepareTest ( IT_VECSWIZZLE4_begin + 4, 0, 0, 0b00000000 );
    prepareTest ( IT_VECSWIZZLE4_begin + 5, 0, 0, 0b01010101 );
    prepareTest ( IT_VECSWIZZLE4_begin + 6, 0, 0, 0b10101010 );
    prepareTest ( IT_VECSWIZZLE4_begin + 7, 0, 0, 0b11111111 );

    prepareTest ( IT_VVECSWIZZLE4_begin + 0, 0, 0, 0b11100100 );
    prepareTest ( IT_VVECSWIZZLE4_begin + 1, 0, 0, 0b00011011 );
    prepareTest ( IT_VVECSWIZZLE4_begin + 2, 0, 0, 0b10110001 );
    prepareTest ( IT_VVECSWIZZLE4_begin + 3, 0, 0, 0b01001110 );

    prepareTest ( IT_WRVECSWIZZLE4_begin + 0, 0, 0, 0b00011011 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 1, 0, 0, 0b11100100 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 2, 0, 0, 0b01110111 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 3, 0, 0, 0b011011 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 4, 0, 0, 0b111001 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 5, 0, 0, 0b0110 );
    prepareTest ( IT_WRVECSWIZZLE4_begin + 6, 0, 0, 0b1001 );

    prepareTest ( IT_SWIZZLEIND_begin + 0, 0, 0, 3 );
    prepareTest ( IT_SWIZZLEIND_begin + 1, 0, 0, 2 );
    prepareTest ( IT_SWIZZLEIND_begin + 2, 0, 0, 1 );
    prepareTest ( IT_SWIZZLEIND_begin + 3, 0, 0, 0 );

    prepareTest ( IT_MAT_2x2_0, 0, 0, 11 );
    prepareTest ( IT_MAT_2x2_1, 0, 0, 12 );
    prepareTest ( IT_MAT_2x2_2, 0, 0, 21 );
    prepareTest ( IT_MAT_2x2_3, 0, 0, 22 );

    prepareTest ( IT_MAT_2x2_D0, 0, 0, 11 );
    prepareTest ( IT_MAT_2x2_D1, 1, 0, 12 );
    prepareTest ( IT_MAT_2x2_D2, 0, 1, 21 );
    prepareTest ( IT_MAT_2x2_D3, 1, 1, 22 );

    prepareTest ( IT_MAT_2x2_C0, 0, 0, 11 );
    prepareTest ( IT_MAT_2x2_C1, 0, 0, 12 );
    prepareTest ( IT_MAT_2x2_C2, 0, 0, 21 );
    prepareTest ( IT_MAT_2x2_C3, 0, 0, 22 );

    prepareTest ( IT_MAT_2x2_V0, 0, 0, 11 );
    prepareTest ( IT_MAT_2x2_V1, 0, 0, 12 );
    prepareTest ( IT_MAT_2x2_V2, 0, 0, 21 );
    prepareTest ( IT_MAT_2x2_V3, 0, 0, 22 );

    prepareTest ( IT_MAT_2x2_V10, 0, 0, 111 );
    prepareTest ( IT_MAT_2x2_V11, 0, 0, 112 );
    prepareTest ( IT_MAT_2x2_V12, 0, 0, 121 );
    prepareTest ( IT_MAT_2x2_V13, 0, 0, 122 );

    prepareTest ( IT_MAT_2x2_V20, 0, 0, 11 );
    prepareTest ( IT_MAT_2x2_V21, 0, 0, 12 );
    prepareTest ( IT_MAT_2x2_V22, 0, 0, 21 );
    prepareTest ( IT_MAT_2x2_V23, 0, 0, 22 );

    CTestStruct* pSourceTestStruct1 = & d_uniTestBuf1 [ 0 ];
    CTestStruct* pTargetTestStruct1 = & d_storTestBuf1 [ 0 ];

    pSourceTestStruct1->d_matr2 = matr2 (
        vect2 ( 1.0f, 2.0f ), vect2 ( 3.0f, 4.0f ) );

    pSourceTestStruct1->d_matr3 = matr3 (
        vect3 ( 5.0f, 6.0f, 7.0f ), vect3 ( 8.0f, 9.0f, 10.0f ), vect3 ( 11.0f, 12.0f, 13.0f ) );

    pSourceTestStruct1->d_matr4 = matr4 (
        vect4 ( 14.0f, 15.0f, 16.0f, 17.0f ), vect4 ( 18.0f, 19.0f, 20.0f, 21.0f ),
        vect4 ( 22.0f, 23.0f, 24.0f, 25.0f ), vect4 ( 26.0f, 27.0f, 28.0f, 29.0f ) );

    pSourceTestStruct1->d_vect2 = vect2 ( 30.0f, 31.0f );
    pSourceTestStruct1->d_vect2a = vect2 ( 30.5f, 31.5f );
    pSourceTestStruct1->d_vect3 = vect3 ( 32.0f, 33.0f, 34.0f );
    pSourceTestStruct1->d_vect4 = vect4 ( 35.0f, 36.0f, 37.0f, 38.0f );

    pTargetTestStruct1->d_matr2 = matr2 (
        vect2 ( 0.0f, 0.0f ), vect2 ( 0.0f, 0.0f ) );

    pTargetTestStruct1->d_matr3 = matr3 (
        vect3 ( 0.0f, 0.0f, 0.0f ), vect3 ( 0.0f, 0.0f, 0.0f ), vect3 ( 0.0f, 0.0f, 0.0f ) );

    pTargetTestStruct1->d_matr4 = matr4 (
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ), vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ),
        vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ), vect4 ( 0.0f, 0.0f, 0.0f, 0.0f ) );

    pTargetTestStruct1->d_vect2 = vect2 ( 0.0f, 0.0f );
    pTargetTestStruct1->d_vect2a = vect2 ( 0.0f, 0.0f );
    pTargetTestStruct1->d_vect3 = vect3 ( 0.0f, 0.0f, 0.0f );
    pTargetTestStruct1->d_vect4 = vect4 ( 0.0f, 0.0f, 0.0f, 0.0f );
}

// -----------------------------------------------------------------------------

void KIndexingTest :: compareResults()
{
    using namespace vpp;

    for ( size_t i = 0; i != IT_count; ++i )
    {
        const int t = d_target [ i ];
        const int e = d_expected [ i ];
        check ( t == e );
    }

    const int cmpTestBuf1 = std::memcmp (
        & d_uniTestBuf1 [ 0 ],
        & d_storTestBuf1 [ 0 ],
        sizeof ( CTestStruct ) );

    check ( cmpTestBuf1 == 0 );
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                              Push constant tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< vpp::ETag TAG >
struct TPushConstantTestStruct : public vpp::UniformStruct< TAG, TPushConstantTestStruct >
{
    vpp::UniformFld< TAG, int > d_outIdx;
    vpp::UniformFld< TAG, int > d_unused1;
    vpp::UniformFld< TAG, int > d_unused2;
    vpp::UniformFld< TAG, int > d_unused3;
    vpp::UniformFld< TAG, vpp::matr2 > d_matr2;
};

typedef TPushConstantTestStruct< vpp::GPU > GPushConstantTestStruct;
typedef TPushConstantTestStruct< vpp::CPU > CPushConstantTestStruct;

// -----------------------------------------------------------------------------

class KPushConstantTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KPushConstantTestPipeline ( const vpp::Device& hDevice );

    void setOutBuffer (
        const vpp::UniformBufferView& rb,
        vpp::ShaderDataBlock* pDataBlock );

    CPushConstantTestStruct& data();
    void push();

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::inPushConstant< TPushConstantTestStruct > d_sourceData;
    vpp::ioBuffer d_resultBuffer;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KPushConstantTestPipeline :: KPushConstantTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KPushConstantTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KPushConstantTestPipeline :: setOutBuffer (
    const vpp::UniformBufferView& rb,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_resultBuffer = rb
    ));
}

// -----------------------------------------------------------------------------

CPushConstantTestStruct& KPushConstantTestPipeline :: data()
{
    return d_sourceData.data();
}

// -----------------------------------------------------------------------------

void KPushConstantTestPipeline :: push()
{
    d_sourceData.cmdPush();
}

// -----------------------------------------------------------------------------

void KPushConstantTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformVar< TPushConstantTestStruct, decltype ( d_sourceData ) > inSourceData ( d_sourceData );
    UniformSimpleArray< matr2, decltype ( d_resultBuffer ) > outResultBuffer ( d_resultBuffer );

    If ( g == 0 && l == 0 );
    {
        const Int idx = inSourceData [ & GPushConstantTestStruct::d_outIdx ];
        const Mat2 v = inSourceData [ & GPushConstantTestStruct::d_matr2 ];
        outResultBuffer [ idx ] = v;
    }
    Fi();
}

// -----------------------------------------------------------------------------

class KPushConstantTest : public vpp::Computation
{
public:
    KPushConstantTest ( const vpp::Device& hDevice );
    KPushConstantTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void compareResults();

private:
    static const size_t VECTOR_SIZE = 8;

    typedef KPushConstantTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< vpp::matr2, vpp::Buf::STORAGE > MatrixVector;
    MatrixVector d_target;
};

// -----------------------------------------------------------------------------

KPushConstantTest :: KPushConstantTest ( const vpp::Device& hDevice ) :
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setOutBuffer ( d_target, & d_dataBlock );
    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_target.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();

        auto& d = d_pipeline.definition().data();

        for ( int i = 0; i != VECTOR_SIZE; ++i )
        {
            const float b = static_cast< float >( 4*i + 1 );
            d.d_outIdx = i;
            d.d_matr2 = matr2 ( vect2 ( b, b + 1.0f ), vect2 ( b + 2.0f, b + 3.0f ) );
            d_pipeline.definition().push();
            cmdDispatch ( 64, 1, 1 );
        }

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

KPushConstantTest :: KPushConstantTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setOutBuffer ( d_target, & d_dataBlock );
    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_target.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();

        auto& d = d_pipeline.definition().data();

        for ( int i = 0; i != VECTOR_SIZE; ++i )
        {
            const float b = static_cast< float >( 4*i + 1 );
            d.d_outIdx = i;
            d.d_matr2 = matr2 ( vect2 ( b, b + 1.0f ), vect2 ( b + 2.0f, b + 3.0f ) );
            d_pipeline.definition().push();
            cmdDispatch ( 64, 1, 1 );
        }

        cmdBufferPipelineBarrier (
            d_target,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_target.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KPushConstantTest :: compareResults()
{
    using namespace vpp;

    for ( int i = 0; i != VECTOR_SIZE; ++i )
    {
        const float b = static_cast< float >( 4*i + 1 );
        const matr2& v = d_target [ i ];
        check ( v.d_columns [ 0 ].x == b );
        check ( v.d_columns [ 0 ].y == b + 1.0f );
        check ( v.d_columns [ 1 ].x == b + 2.0f );
        check ( v.d_columns [ 1 ].y == b + 3.0f );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                              Texel buffer tests

// -----------------------------------------------------------------------------

class KTexelBufferTestTypes
{
public:
    typedef vpp::format< float, float, float, float > Fmt4xF32;
    typedef vpp::TexelBufferView< Fmt4xF32, vpp::Buf::UNITEX | vpp::Buf::STORTEX > View4xF32;
    typedef Fmt4xF32::data_type Data4xF32;
    typedef vpp::gvector< Data4xF32, vpp::Buf::UNITEX | vpp::Buf::STORTEX > Vec4xF32;
};

// -----------------------------------------------------------------------------

class KTexelBufferTestPipeline :
    public vpp::ComputePipelineConfig,
    public KTexelBufferTestTypes
{
public:
    typedef vpp::format< float, float, float, float > Fmt4xF32;

public:
    KTexelBufferTestPipeline ( const vpp::Device& hDevice );

    void setOutBuffer (
        const View4xF32& inTexBuf4xF32,
        const View4xF32& ioImgBuf4xF32,
        const View4xF32& inTexBuf4xF32a,
        const View4xF32& ioImgBuf4xF32a,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::inTextureBuffer< Fmt4xF32 > m_inTexBuf4xF32;
    vpp::ioImageBuffer< Fmt4xF32 > m_ioImgBuf4xF32;

    vpp::inTextureBuffer< Fmt4xF32 > m_inTexBuf4xF32a;
    vpp::ioImageBuffer< Fmt4xF32 > m_ioImgBuf4xF32a;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KTexelBufferTestPipeline :: KTexelBufferTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KTexelBufferTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KTexelBufferTestPipeline :: setOutBuffer (
    const KTexelBufferTestPipeline::View4xF32& inTexBuf4xF32,
    const KTexelBufferTestPipeline::View4xF32& ioImgBuf4xF32,
    const KTexelBufferTestPipeline::View4xF32& inTexBuf4xF32a,
    const KTexelBufferTestPipeline::View4xF32& ioImgBuf4xF32a,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        m_inTexBuf4xF32 = inTexBuf4xF32,
        m_ioImgBuf4xF32 = ioImgBuf4xF32,
        m_inTexBuf4xF32a = inTexBuf4xF32a,
        m_ioImgBuf4xF32a = ioImgBuf4xF32a
    ));
}

// -----------------------------------------------------------------------------

void KTexelBufferTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];
    const Int index = ( g << 5 ) | l;

    TexelArray< decltype ( m_inTexBuf4xF32a ) > inTexBuf4xF32a ( m_inTexBuf4xF32a );
    TexelArray< decltype ( m_ioImgBuf4xF32a ) > ioImgBuf4xF32a ( m_ioImgBuf4xF32a );
    
    UseImage ( m_inTexBuf4xF32 );
    UseImage ( m_ioImgBuf4xF32 );

    const Int bufferLength = inTexBuf4xF32a.Size();

    If ( index < bufferLength );
    {
        const Vec4 v = TexelFetch ( m_inTexBuf4xF32, index );
        ImageStore ( m_ioImgBuf4xF32, index, v );

        const Vec4 va = inTexBuf4xF32a [ index ];
        ioImgBuf4xF32a [ index ] = va;
    }
    Fi();
}

// -----------------------------------------------------------------------------

class KTexelBufferTest :
    public vpp::Computation,
    public KTexelBufferTestTypes
{
public:
    KTexelBufferTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    static const size_t VECTOR_SIZE = 1000;

    typedef KTexelBufferTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    Vec4xF32 d_sourceVec4xF32;
    View4xF32 d_sourceView4xF32;
    Vec4xF32 d_targetVec4xF32;
    View4xF32 d_targetView4xF32;

    Vec4xF32 d_sourceVec4xF32a;
    View4xF32 d_sourceView4xF32a;
    Vec4xF32 d_targetVec4xF32a;
    View4xF32 d_targetView4xF32a;
};

// -----------------------------------------------------------------------------

KTexelBufferTest :: KTexelBufferTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_sourceVec4xF32 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_sourceView4xF32 ( d_sourceVec4xF32 ),
        d_targetVec4xF32 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_targetView4xF32 ( d_targetVec4xF32 ),
        d_sourceVec4xF32a ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_sourceView4xF32a ( d_sourceVec4xF32a ),
        d_targetVec4xF32a ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_targetView4xF32a ( d_targetVec4xF32a )
{
    using namespace vpp;

    d_pipeline.definition().setOutBuffer (
        d_sourceView4xF32,
        d_targetView4xF32,
        d_sourceView4xF32a,
        d_targetView4xF32a,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_sourceVec4xF32.cmdCommit();
        d_targetVec4xF32.cmdCommit();
        d_sourceVec4xF32a.cmdCommit();
        d_targetVec4xF32a.cmdCommit();

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        
        cmdDispatch ( ( VECTOR_SIZE + 31 ) >> 5, 1, 1 );

        cmdBufferPipelineBarrier (
            d_targetVec4xF32,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        cmdBufferPipelineBarrier (
            d_targetVec4xF32a,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT
        );

        d_targetVec4xF32.cmdLoadAll();
        d_targetVec4xF32a.cmdLoadAll();
    };

    prepareVectors();
}

// -----------------------------------------------------------------------------

void KTexelBufferTest :: prepareVectors()
{
    d_sourceVec4xF32.resize ( VECTOR_SIZE );
    d_targetVec4xF32.resize ( VECTOR_SIZE );
    d_sourceVec4xF32a.resize ( VECTOR_SIZE );
    d_targetVec4xF32a.resize ( VECTOR_SIZE );

    for ( int i = 0; i != VECTOR_SIZE; ++i )
    {
        const float b = static_cast< float >( 4*i + 1 );
        const Data4xF32 s ( b, b + 1.0f, b + 2.0f, b + 3.0f );
        const Data4xF32 t ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_sourceVec4xF32 [ i ] = s;
        d_targetVec4xF32 [ i ] = t;
        d_sourceVec4xF32a [ i ] = s;
        d_targetVec4xF32a [ i ] = t;
    }
}

// -----------------------------------------------------------------------------

void KTexelBufferTest :: compareResults()
{
    using namespace vpp;

    for ( int i = 0; i != VECTOR_SIZE; ++i )
    {
        const float b = static_cast< float >( 4*i + 1 );
        const Data4xF32& s = d_sourceVec4xF32 [ i ];
        const Data4xF32& t = d_targetVec4xF32 [ i ];
        check ( s == t );
    }

    for ( int i = 0; i != VECTOR_SIZE; ++i )
    {
        const float b = static_cast< float >( 4*i + 1 );
        const Data4xF32& s = d_sourceVec4xF32a [ i ];
        const Data4xF32& t = d_targetVec4xF32a [ i ];
        check ( s == t );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Images tests

// -----------------------------------------------------------------------------

template< class FormatT >
struct TImageTestTypeSet
{
    typedef typename FormatT::data_type data_type;
    typedef vpp::gvector< data_type, vpp::Buf::SOURCE | vpp::Buf::TARGET > buffer_type;

    typedef vpp::ImageAttributes<
        FormatT, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::STORAGE | vpp::Img::SAMPLED | vpp::Img::SOURCE | vpp::Img::TARGET,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > AttributesT;

    typedef vpp::Image< AttributesT > image_type;
    typedef vpp::ImageViewAttributes< image_type > ViewAttrT;
    typedef vpp::ImageView< ViewAttrT > view_type;
};

// -----------------------------------------------------------------------------

struct KImageTestTypes
{
    typedef vpp::format< float, float, float, float > Fmt4xF32;
    typedef TImageTestTypeSet< Fmt4xF32 > Types4xF32;
    typedef typename Types4xF32::image_type Img4xF32;
    typedef typename Types4xF32::view_type View4xF32;
    typedef typename Types4xF32::buffer_type Buf4xF32;
    typedef typename Types4xF32::data_type Data4xF32;

    typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > Fmt4xFU8;
    typedef TImageTestTypeSet< Fmt4xFU8 > Types4xFU8;
    typedef typename Types4xFU8::image_type Img4xFU8;
    typedef typename Types4xFU8::view_type View4xFU8;
    typedef typename Types4xFU8::buffer_type Buf4xFU8;
    typedef typename Types4xFU8::data_type Data4xFU8;

    typedef vpp::format< vpp::snorm8_t, vpp::snorm8_t > Fmt2xFS8;
    typedef TImageTestTypeSet< Fmt2xFS8 > Types2xFS8;
    typedef typename Types2xFS8::image_type Img2xFS8;
    typedef typename Types2xFS8::view_type View2xFS8;
    typedef typename Types2xFS8::buffer_type Buf2xFS8;
    typedef typename Types2xFS8::data_type Data2xFS8;

    typedef vpp::format< vpp::srgb8_t, vpp::srgb8_t, vpp::srgb8_t, vpp::srgb8_t > Fmt4xSR8;
    typedef TImageTestTypeSet< Fmt4xSR8 > Types4xSR8;
    typedef typename Types4xSR8::image_type Img4xSR8;
    typedef typename Types4xSR8::view_type View4xSR8;
    typedef typename Types4xSR8::buffer_type Buf4xSR8;
    typedef typename Types4xSR8::data_type Data4xSR8;

    static const unsigned int WIDTH = 500;
    static const unsigned int HEIGHT = 377;
    static const unsigned int LENGTH = WIDTH * HEIGHT;
};

// -----------------------------------------------------------------------------

class KImageTestPipeline :
    public vpp::ComputePipelineConfig,
    public KImageTestTypes
{
public:
    KImageTestPipeline ( const vpp::Device& hDevice );

    void setBuffers (
        const View4xF32& inTexBuf4xF32,
        const View4xF32& ioImgBuf4xF32,
        const View4xFU8& inTexBuf4xFU8,
        const View4xFU8& ioImgBuf4xFU8,
        const View2xFS8& inTexBuf2xFS8,
        const View2xFS8& ioImgBuf2xFS8,
        const View4xSR8& inTexBuf4xSR8,
        const View4xSR8& ioImgBuf4xSR8,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioImage< View4xF32 > m_inImgBuf4xF32;
    vpp::ioImage< View4xF32 > m_ioImgBuf4xF32;
    vpp::ioImage< View4xFU8 > m_inImgBuf4xFU8;
    vpp::ioImage< View4xFU8 > m_ioImgBuf4xFU8;
    vpp::ioImage< View2xFS8 > m_inImgBuf2xFS8;
    vpp::ioImage< View2xFS8 > m_ioImgBuf2xFS8;
    vpp::ioImage< View4xSR8 > m_inImgBuf4xSR8;
    vpp::ioImage< View4xSR8 > m_ioImgBuf4xSR8;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KImageTestPipeline :: KImageTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KImageTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KImageTestPipeline :: setBuffers (
    const KImageTestPipeline::View4xF32& inImgBuf4xF32,
    const KImageTestPipeline::View4xF32& ioImgBuf4xF32,
    const KImageTestPipeline::View4xFU8& inImgBuf4xFU8,
    const KImageTestPipeline::View4xFU8& ioImgBuf4xFU8,
    const KImageTestPipeline::View2xFS8& inImgBuf2xFS8,
    const KImageTestPipeline::View2xFS8& ioImgBuf2xFS8,
    const KImageTestPipeline::View4xSR8& inImgBuf4xSR8,
    const KImageTestPipeline::View4xSR8& ioImgBuf4xSR8,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        m_inImgBuf4xF32 = inImgBuf4xF32,
        m_ioImgBuf4xF32 = ioImgBuf4xF32,
        m_inImgBuf4xFU8 = inImgBuf4xFU8,
        m_ioImgBuf4xFU8 = ioImgBuf4xFU8,
        m_inImgBuf2xFS8 = inImgBuf2xFS8,
        m_ioImgBuf2xFS8 = ioImgBuf2xFS8,
        m_inImgBuf4xSR8 = inImgBuf4xSR8,
        m_ioImgBuf4xSR8 = ioImgBuf4xSR8
    ));
}

// -----------------------------------------------------------------------------

void KImageTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];
    const Int index = ( g << 5 ) | l;

    const IVec2 imgSize1 = ImageSize ( m_inImgBuf4xF32 );
    const Int width1 = imgSize1 [ X ];
    const Int height1 = imgSize1 [ Y ];
    const Int x1 = index % width1;
    const Int y1 = index / width1;

    If ( y1 < height1 );
    {
        const IVec2 coords = IVec2 ( x1, y1 );

        const Vec4 v1 = ImageLoad ( m_inImgBuf4xF32, coords );
        ImageStore ( m_ioImgBuf4xF32, coords, v1 );

        const Vec4 v2 = ImageLoad ( m_inImgBuf4xFU8, coords );
        ImageStore ( m_ioImgBuf4xFU8, coords, v2 );

        const Vec4 v3 = ImageLoad ( m_inImgBuf2xFS8, coords );
        ImageStore ( m_ioImgBuf2xFS8, coords, -v3 );

        const Vec4 v4 = ImageLoad ( m_inImgBuf4xSR8, coords );
        ImageStore ( m_ioImgBuf4xSR8, coords, v4 );
    }
    Fi();
}

// -----------------------------------------------------------------------------

class KImageTest :
    public vpp::Computation,
    public KImageTestTypes
{
public:
    KImageTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    typedef KImageTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    Buf4xF32 d_sourceBuf4xF32;
    Img4xF32 d_sourceImg4xF32;
    View4xF32 d_sourceView4xF32;
    Buf4xF32 d_targetBuf4xF32;
    Img4xF32 d_targetImg4xF32;
    View4xF32 d_targetView4xF32;

    Buf4xFU8 d_sourceBuf4xFU8;
    Img4xFU8 d_sourceImg4xFU8;
    View4xFU8 d_sourceView4xFU8;
    Buf4xFU8 d_targetBuf4xFU8;
    Img4xFU8 d_targetImg4xFU8;
    View4xFU8 d_targetView4xFU8;

    Buf2xFS8 d_sourceBuf2xFS8;
    Img2xFS8 d_sourceImg2xFS8;
    View2xFS8 d_sourceView2xFS8;
    Buf2xFS8 d_targetBuf2xFS8;
    Img2xFS8 d_targetImg2xFS8;
    View2xFS8 d_targetView2xFS8;

    Buf4xSR8 d_sourceBuf4xSR8;
    Img4xSR8 d_sourceImg4xSR8;
    View4xSR8 d_sourceView4xSR8;
    Buf4xSR8 d_targetBuf4xSR8;
    Img4xSR8 d_targetImg4xSR8;
    View4xSR8 d_targetView4xSR8;
};

// -----------------------------------------------------------------------------

KImageTest :: KImageTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_sourceBuf4xF32 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceImg4xF32 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceView4xF32 ( d_sourceImg4xF32 ),
    d_targetBuf4xF32 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetImg4xF32 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetView4xF32 ( d_targetImg4xF32 ),
    d_sourceBuf4xFU8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceImg4xFU8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceView4xFU8 ( d_sourceImg4xFU8 ),
    d_targetBuf4xFU8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetImg4xFU8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetView4xFU8 ( d_targetImg4xFU8 ),
    d_sourceBuf2xFS8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceImg2xFS8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceView2xFS8 ( d_sourceImg2xFS8 ),
    d_targetBuf2xFS8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetImg2xFS8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetView2xFS8 ( d_targetImg2xFS8 ),
    d_sourceBuf4xSR8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceImg4xSR8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sourceView4xSR8 ( d_sourceImg4xSR8 ),
    d_targetBuf4xSR8 ( LENGTH, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetImg4xSR8 ( { WIDTH, HEIGHT, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_targetView4xSR8 ( d_targetImg4xSR8 )
{
    using namespace vpp;

    d_pipeline.definition().setBuffers (
        d_sourceView4xF32,
        d_targetView4xF32,
        d_sourceView4xFU8,
        d_targetView4xFU8,
        d_sourceView2xFS8,
        d_targetView2xFS8,
        d_sourceView4xSR8,
        d_targetView4xSR8,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        
        d_sourceBuf4xF32.cmdCopyToImage (
            d_sourceImg4xF32,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xF32.cmdCopyToImage (
            d_targetImg4xF32,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_sourceBuf4xFU8.cmdCopyToImage (
            d_sourceImg4xFU8,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xFU8.cmdCopyToImage (
            d_targetImg4xFU8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_sourceBuf2xFS8.cmdCopyToImage (
            d_sourceImg2xFS8,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf2xFS8.cmdCopyToImage (
            d_targetImg2xFS8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_sourceBuf4xSR8.cmdCopyToImage (
            d_sourceImg4xSR8,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xSR8.cmdCopyToImage (
            d_targetImg4xSR8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        cmdDispatch ( ( LENGTH + 31 ) >> 5, 1, 1 );

        d_targetBuf4xF32.cmdCopyFromImage (
            d_targetImg4xF32,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xFU8.cmdCopyFromImage (
            d_targetImg4xFU8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf2xFS8.cmdCopyFromImage (
            d_targetImg2xFS8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xSR8.cmdCopyFromImage (
            d_targetImg4xSR8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );
    };

    prepareVectors();
}

// -----------------------------------------------------------------------------

void KImageTest :: prepareVectors()
{
    d_sourceBuf4xF32.resize ( LENGTH );
    d_targetBuf4xF32.resize ( LENGTH );
    d_sourceBuf4xFU8.resize ( LENGTH );
    d_targetBuf4xFU8.resize ( LENGTH );
    d_sourceBuf2xFS8.resize ( LENGTH );
    d_targetBuf2xFS8.resize ( LENGTH );
    d_sourceBuf4xSR8.resize ( LENGTH );
    d_targetBuf4xSR8.resize ( LENGTH );

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        const float b = float ( k );
        d_sourceBuf4xF32 [ k ] = Data4xF32 ( b, b + 0.25f, b + 0.5f, b + 0.75f );
        d_targetBuf4xF32 [ k ] = Data4xF32 ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_sourceBuf4xFU8 [ k ] = Data4xFU8 ( b, b + 0.25f, b + 0.5f, b + 0.75f );
        d_targetBuf4xFU8 [ k ] = Data4xFU8 ( 0.0f, 0.0f, 0.0f, 0.0f );
        d_sourceBuf2xFS8 [ k ] = Data2xFS8 ( b, b + 0.25f );
        d_targetBuf2xFS8 [ k ] = Data2xFS8 ( 0.0f, 0.0f );
        d_sourceBuf4xSR8 [ k ] = Data4xSR8 ( b, b + 0.25f, b + 0.5f, b + 0.75f );
        d_targetBuf4xSR8 [ k ] = Data4xSR8 ( 0.0f, 0.0f, 0.0f, 0.0f );
    }
}

// -----------------------------------------------------------------------------

void KImageTest :: compareResults()
{
    using namespace vpp;

    for ( unsigned int i = 0; i != HEIGHT; ++i )
        for ( unsigned int j = 0; j != WIDTH; ++j )
    {
        const unsigned int k = i * WIDTH + j;
        const Data4xF32 s1 = d_sourceBuf4xF32 [ k ];
        const Data4xF32 t1 = d_targetBuf4xF32 [ k ];
        check ( s1 == t1 );

        const Data4xFU8 s2 = d_sourceBuf4xFU8 [ k ];
        const Data4xFU8 t2 = d_targetBuf4xFU8 [ k ];
        check ( s2 == t2 );

        const Data2xFS8 s3 = d_sourceBuf2xFS8 [ k ];
        const Data2xFS8 t3 = d_targetBuf2xFS8 [ k ];
        const Data2xFS8 u3 = Data2xFS8 ( -float ( t3.r ), -float ( t3.g ) );
        check ( s3 == u3 );

        const Data4xSR8 s4 = d_sourceBuf4xSR8 [ k ];
        const Data4xSR8 t4 = d_targetBuf4xSR8 [ k ];
        check ( s4 == t4 );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                               Set of all tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KTests : public vpp::ComputationEngine
{
public:
    KTests ( const vpp::Device& hDevice );

    typedef TFloatTest< float > KFloatTest;
    typedef TFloatTest< vpp::vect2 > KVec2Test;
    typedef TFloatTest< vpp::vect3 > KVec3Test;
    typedef TFloatTest< vpp::vect4 > KVec4Test;

    KFloatTest testFloat;
    KVec2Test testVec2;
    KVec3Test testVec3;
    KVec4Test testVec4;
    KVectorTest testVectors;
    KConstructTest testConstructs;
    KIndexingTest testIndexing;
    KPushConstantTest testPushConstants;
    KTexelBufferTest testTexelBuffers;
    KImageTest testImages;
};

// -----------------------------------------------------------------------------

KTests :: KTests ( const vpp::Device& hDevice ) :
    vpp::ComputationEngine ( hDevice, vpp::Q_GRAPHICS ),
    testFloat ( hDevice ),
    testVec2 ( testFloat, hDevice ),
    testVec3 ( testVec2, hDevice ),
    testVec4 ( testVec3, hDevice ),
    testVectors ( testVec4, hDevice ),
    testConstructs ( testVectors, hDevice ),
    testIndexing ( testConstructs, hDevice ),
    testPushConstants ( testIndexing, hDevice ),
    testTexelBuffers ( testPushConstants, hDevice ),
    testImages ( testTexelBuffers, hDevice )
{
    compile();
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
    vpp::StreamDebugReporter m_debugReporter ( validationLog, inst /*, vpp::DebugReporter::SHADERS*/ );

    PhysicalDevices physicalDevices;

    if ( inst.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
         || physicalDevices.empty() )
    {
        return -1;
    }

    PhysicalDevice phd = physicalDevices [ 0 ];
    Device dev ( phd );

    KTests testObject ( dev );
    testObject.testFloat();
    testObject.testVec2();
    testObject.testVec3();
    testObject.testVec4();
    testObject.testVectors();
    testObject.testConstructs();
    testObject.testIndexing();
    testObject.testPushConstants();
    testObject.testTexelBuffers();
    testObject.testImages ( NO_TIMEOUT );

    testObject.testFloat.compareResults();
    testObject.testVec2.compareResults();
    testObject.testVec3.compareResults();
    testObject.testVec4.compareResults();
    testObject.testVectors.compareResults();
    testObject.testConstructs.compareResults();
    testObject.testIndexing.compareResults();
    testObject.testPushConstants.compareResults();
    testObject.testTexelBuffers.compareResults();
    testObject.testImages.compareResults();

    return 0;
}

// -----------------------------------------------------------------------------
