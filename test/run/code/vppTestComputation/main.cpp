// -----------------------------------------------------------------------------

#pragma warning (disable: 4503)
#include <vppAll.hpp>

// -----------------------------------------------------------------------------

#include <numeric>

// -----------------------------------------------------------------------------
namespace vpptest {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace {
// -----------------------------------------------------------------------------

unsigned int s_passedChecks = 0;
unsigned int s_failedChecks = 0;

// -----------------------------------------------------------------------------

void check ( bool bCondition )
{
    if ( ! bCondition )
    {
        ++s_failedChecks;
        std::cerr << "Check failed" << std::endl;
    }
    else
        ++s_passedChecks;
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

template<>
struct TTestTypeTraits< int >
{
    typedef vpp::Int shader_type;

    inline int create ( int v ) const
    {
        return v;
    }

    inline vpp::Int create ( const vpp::Int& v ) const
    {
        return v;
    }

    inline bool compare ( int a, int b ) const
    {
        return a == b;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::ivect2 >
{
    typedef vpp::IVec2 shader_type;

    inline vpp::ivect2 create ( int v ) const
    {
        vpp::ivect2 r;
        r.set ( v, v );
        return r;
    }

    inline vpp::IVec2 create ( const vpp::Int& v ) const
    {
        return vpp::IVec2 ( v, v );
    }

    inline bool compare ( const vpp::ivect2& a, const vpp::ivect2& b ) const
    {
        return a.r == b.r && a.g == b.g;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::ivect3 >
{
    typedef vpp::IVec3 shader_type;

    inline vpp::ivect3 create ( int v ) const
    {
        vpp::ivect3 r;
        r.set ( v, v, v );
        return r;
    }

    inline vpp::IVec3 create ( const vpp::Int& v ) const
    {
        return vpp::IVec3 ( v, v, v );
    }

    inline bool compare ( const vpp::ivect3& a, const vpp::ivect3& b ) const
    {
        return a.r == b.r && a.g == b.g && a.b == b.b;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::ivect4 >
{
    typedef vpp::IVec4 shader_type;

    inline vpp::ivect4 create ( int v ) const
    {
        vpp::ivect4 r;
        r.set ( v, v, v, v );
        return r;
    }

    inline vpp::IVec4 create ( const vpp::Int& v ) const
    {
        return vpp::IVec4 ( v, v, v, v );
    }

    inline bool compare ( const vpp::ivect4& a, const vpp::ivect4& b ) const
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< unsigned int >
{
    typedef vpp::UInt shader_type;

    inline unsigned int create ( unsigned int v ) const
    {
        return v;
    }

    inline vpp::UInt create ( const vpp::UInt& v ) const
    {
        return v;
    }

    inline bool compare ( unsigned int a, unsigned int b ) const
    {
        return a == b;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::uvect2 >
{
    typedef vpp::UVec2 shader_type;

    inline vpp::uvect2 create ( unsigned int v ) const
    {
        vpp::uvect2 r;
        r.set ( v, v );
        return r;
    }

    inline vpp::UVec2 create ( const vpp::UInt& v ) const
    {
        return vpp::UVec2 ( v, v );
    }

    inline bool compare ( const vpp::uvect2& a, const vpp::uvect2& b ) const
    {
        return a.r == b.r && a.g == b.g;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::uvect3 >
{
    typedef vpp::UVec3 shader_type;

    inline vpp::uvect3 create ( unsigned int v ) const
    {
        vpp::uvect3 r;
        r.set ( v, v, v );
        return r;
    }

    inline vpp::UVec3 create ( const vpp::UInt& v ) const
    {
        return vpp::UVec3 ( v, v, v );
    }

    inline bool compare ( const vpp::uvect3& a, const vpp::uvect3& b ) const
    {
        return a.r == b.r && a.g == b.g && a.b == b.b;
    }
};

// -----------------------------------------------------------------------------

template<>
struct TTestTypeTraits< vpp::uvect4 >
{
    typedef vpp::UVec4 shader_type;

    inline vpp::uvect4 create ( unsigned int v ) const
    {
        vpp::uvect4 r;
        r.set ( v, v, v, v );
        return r;
    }

    inline vpp::UVec4 create ( const vpp::UInt& v ) const
    {
        return vpp::UVec4 ( v, v, v, v );
    }

    inline bool compare ( const vpp::uvect4& a, const vpp::uvect4& b ) const
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
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
        const vpp::StorageBufferView& vs1,
        const vpp::StorageBufferView& vs2,
        const vpp::StorageBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_source1;
    vpp::ioBuffer d_source2;
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
    const vpp::StorageBufferView& vs1,
    const vpp::StorageBufferView& vs2,
    const vpp::StorageBufferView& vt,
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

template< typename HostT >
void TFloatTestPipeline< HostT > :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    //pShader->DebugCodeDump();

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

    typedef vpp::gvector< HostT, vpp::Buf::UNIFORM | vpp::Buf::STORAGE | vpp::Buf::SOURCE > FloatVector;

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

//                Test scalar (or by-component) integer functions

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum EIntegerTests
{
    IT_ADD, IT_SUB, IT_MUL, IT_DIV, IT_REM, IT_MOD,

    IT_EQU1, IT_NEQ1, IT_LSS1, IT_GTR1, IT_LEQ1, IT_GEQ1,
    IT_EQU0, IT_NEQ0, IT_LSS0, IT_GTR0, IT_LEQ0, IT_GEQ0,

    IT_ABS, IT_SIGN, IT_MIN, IT_MAX,
    IT_CLAMP0, IT_CLAMP1, IT_CLAMP2,

    IT_FINDLSB, IT_FINDMSB, IT_BITCOUNT, IT_BITREVERSE,

    IT_count
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename HostT >
class TIntegerTestPipeline : public vpp::ComputePipelineConfig
{
public:
    TIntegerTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::StorageBufferView& vs1,
        const vpp::StorageBufferView& vs2,
        const vpp::StorageBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_source1;
    vpp::ioBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

template< typename HostT >
TIntegerTestPipeline< HostT > :: TIntegerTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & TIntegerTestPipeline< HostT >::fComputeShader )
{
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TIntegerTestPipeline< HostT > :: setData (
    const vpp::StorageBufferView& vs1,
    const vpp::StorageBufferView& vs2,
    const vpp::StorageBufferView& vt,
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

template< typename HostT >
void TIntegerTestPipeline< HostT > :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    //pShader->DebugCodeDump();

    typedef typename TTestTypeTraits< HostT >::shader_type shader_type;
    TTestTypeTraits< HostT > tt;

    const IVec3 workgroupId = pShader->inWorkgroupId;
    const IVec3 localId = pShader->inLocalInvocationId;
    const Int g = workgroupId [ X ];
    const Int l = localId [ X ];

    UniformSimpleArray< HostT, decltype ( d_source1 ) > inSource1 ( d_source1 );
    UniformSimpleArray< HostT, decltype ( d_source2 ) > inSource2 ( d_source2 );
    UniformSimpleArray< HostT, decltype ( d_target ) > outTarget ( d_target );

    const shader_type v0 = tt.create ( Int ( 0 ) );
    const shader_type v1 = tt.create ( Int ( 1 ) );
    const shader_type v2 = tt.create ( Int ( 2 ) );

    If ( g == 0 && l == 0 );
        outTarget [ IT_ADD ] = inSource1 [ IT_ADD ] + inSource2 [ IT_ADD ];
        outTarget [ IT_SUB ] = inSource1 [ IT_SUB ] - inSource2 [ IT_SUB ];
        outTarget [ IT_MUL ] = inSource1 [ IT_MUL ] * inSource2 [ IT_MUL ];
        outTarget [ IT_DIV ] = inSource1 [ IT_DIV ] / inSource2 [ IT_DIV ];
        outTarget [ IT_REM ] = inSource1 [ IT_REM ] % inSource2 [ IT_REM ];
        outTarget [ IT_MOD ] = Mod ( inSource1 [ IT_MOD ], inSource2 [ IT_MOD ] );

        outTarget [ IT_EQU1 ] = Select ( inSource1 [ IT_EQU1 ] == inSource2 [ IT_EQU1 ], v1, v0 );
        outTarget [ IT_NEQ1 ] = Select ( inSource1 [ IT_NEQ1 ] != inSource2 [ IT_NEQ1 ], v1, v0 );
        outTarget [ IT_LSS1 ] = Select ( inSource1 [ IT_LSS1 ] <  inSource2 [ IT_LSS1 ], v1, v0 );
        outTarget [ IT_GTR1 ] = Select ( inSource1 [ IT_GTR1 ] >  inSource2 [ IT_GTR1 ], v1, v0 );
        outTarget [ IT_LEQ1 ] = Select ( inSource1 [ IT_LEQ1 ] <= inSource2 [ IT_LEQ1 ], v1, v0 );
        outTarget [ IT_GEQ1 ] = Select ( inSource1 [ IT_GEQ1 ] >= inSource2 [ IT_GEQ1 ], v1, v0 );
        outTarget [ IT_EQU0 ] = Select ( inSource1 [ IT_EQU0 ] == inSource2 [ IT_EQU0 ], v1, v0 );
        outTarget [ IT_NEQ0 ] = Select ( inSource1 [ IT_NEQ0 ] != inSource2 [ IT_NEQ0 ], v1, v0 );
        outTarget [ IT_LSS0 ] = Select ( inSource1 [ IT_LSS0 ] <  inSource2 [ IT_LSS0 ], v1, v0 );
        outTarget [ IT_GTR0 ] = Select ( inSource1 [ IT_GTR0 ] >  inSource2 [ IT_GTR0 ], v1, v0 );
        outTarget [ IT_LEQ0 ] = Select ( inSource1 [ IT_LEQ0 ] <= inSource2 [ IT_LEQ0 ], v1, v0 );
        outTarget [ IT_GEQ0 ] = Select ( inSource1 [ IT_GEQ0 ] >= inSource2 [ IT_GEQ0 ], v1, v0 );

        outTarget [ IT_ABS ] = Abs ( inSource1 [ IT_ABS ] );
        outTarget [ IT_SIGN ] = Sign ( inSource1 [ IT_SIGN ] );
        outTarget [ IT_MIN ] = Min ( inSource1 [ IT_MIN ], inSource2 [ IT_MIN ] );
        outTarget [ IT_MAX ] = Max ( inSource1 [ IT_MAX ], inSource2 [ IT_MAX ] );
        outTarget [ IT_CLAMP0 ] = Clamp ( v0, inSource1 [ IT_CLAMP0 ], inSource2 [ IT_CLAMP0 ] );
        outTarget [ IT_CLAMP1 ] = Clamp ( v1, inSource1 [ IT_CLAMP1 ], inSource2 [ IT_CLAMP1 ] );
        outTarget [ IT_CLAMP2 ] = Clamp ( v2, inSource1 [ IT_CLAMP2 ], inSource2 [ IT_CLAMP2 ] );

        outTarget [ IT_FINDLSB ] = FindLsb ( inSource1 [ IT_FINDLSB ] );
        outTarget [ IT_FINDMSB ] = FindMsb ( inSource1 [ IT_FINDMSB ] );
        outTarget [ IT_BITCOUNT ] = BitCount ( inSource1 [ IT_BITCOUNT ] );
        outTarget [ IT_BITREVERSE ] = BitReverse ( inSource1 [ IT_BITREVERSE ] );
    Fi();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename HostT >
class TIntegerTest : public vpp::Computation
{
public:
    TIntegerTest ( const vpp::Device& hDevice );
    TIntegerTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareVectors();
    void compareResults();

private:
    void prepareTest ( EIntegerTests testId, int s1, int s2, int exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef TIntegerTestPipeline< HostT > TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< HostT, vpp::Buf::STORAGE > InIntegerVector;
    typedef vpp::gvector< HostT, vpp::Buf::STORAGE | vpp::Buf::SOURCE > OutIntegerVector;

    InIntegerVector d_source1;
    InIntegerVector d_source2;
    OutIntegerVector d_target;

    std::vector< int > d_expected;
};

// -----------------------------------------------------------------------------

template< typename HostT >
TIntegerTest< HostT > :: TIntegerTest ( const vpp::Device& hDevice ) :
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

template< typename HostT >
TIntegerTest< HostT > :: TIntegerTest (
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

template< typename HostT >
void TIntegerTest< HostT > :: prepareTest (
    EIntegerTests testId, int s1, int s2, int exp )
{
    TTestTypeTraits< HostT > tt;

    d_source1 [ testId ] = tt.create ( s1 );
    d_source2 [ testId ] = tt.create ( s2 );
    d_target [ testId ] = tt.create ( 0 );
    d_expected [ testId ] = exp;
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TIntegerTest< HostT > :: prepareVectors()
{
    const unsigned int nItems = IT_count;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    prepareTest ( IT_ADD, 17, 2, 19 );
    prepareTest ( IT_SUB, 17, 2, 15 );
    prepareTest ( IT_MUL, 17, -2, -34 );
    prepareTest ( IT_DIV, 17, 2, 8 );
    prepareTest ( IT_REM, 3737375, 10, 5 );
    prepareTest ( IT_MOD, 17, 10, 7 );

    prepareTest ( IT_EQU1, 17, 17, 1 );
    prepareTest ( IT_NEQ1, 17, 16, 1 );
    prepareTest ( IT_LSS1, 17, 18, 1 );
    prepareTest ( IT_GTR1, 17, 15, 1 );
    prepareTest ( IT_LEQ1, 17, 17, 1 );
    prepareTest ( IT_GEQ1, 17, 17, 1 );

    prepareTest ( IT_EQU0, 17, 18, 0 );
    prepareTest ( IT_NEQ0, 17, 17, 0 );
    prepareTest ( IT_LSS0, 18, 17, 0 );
    prepareTest ( IT_GTR0, 15, 15, 0 );
    prepareTest ( IT_LEQ0, 18, 17, 0 );
    prepareTest ( IT_GEQ0, 17, 18, 0 );

    prepareTest ( IT_ABS, -23, 0, 23 );
    prepareTest ( IT_SIGN, -23, 0, -1 );
    
    prepareTest ( IT_MIN, 16, 5, 5 );
    prepareTest ( IT_MAX, 16, 5, 16 );

    prepareTest ( IT_CLAMP0, 5, 15, 5 );
    prepareTest ( IT_CLAMP1, 0, 10, 1 );
    prepareTest ( IT_CLAMP2, -5, 1, 1 );

    prepareTest ( IT_FINDLSB, 11392, 0, 7 );
    prepareTest ( IT_FINDMSB, 11392, 0, 13 );
    prepareTest ( IT_BITCOUNT, 11392, 0, 4 );
    prepareTest ( IT_BITREVERSE, 11392, 0, 20185088 );
}

// -----------------------------------------------------------------------------

template< typename HostT >
void TIntegerTest< HostT > :: compareResults()
{
    TTestTypeTraits< HostT > tt;

    for ( size_t i = 0; i != IT_count; ++i )
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
        const vpp::StorageBufferView& vs1,
        const vpp::StorageBufferView& vs2,
        const vpp::StorageBufferView& vt,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_source1;
    vpp::ioBuffer d_source2;
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
    const vpp::StorageBufferView& vs1,
    const vpp::StorageBufferView& vs2,
    const vpp::StorageBufferView& vt,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_source1 = vs1,
        d_source2 = vs2,
        d_target = vt
    ));
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

    typedef vpp::gvector< vpp::vect4, vpp::Buf::STORAGE > InFloatVector;
    typedef vpp::gvector< vpp::vect4, vpp::Buf::STORAGE | vpp::Buf::SOURCE > OutFloatVector;

    InFloatVector d_source1;
    InFloatVector d_source2;
    OutFloatVector d_target;

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
    d_pipeline.definition().setData ( d_source1, d_source2, d_target, & d_dataBlock );
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

KVectorTest :: KVectorTest (
    vpp::Computation& pred, const vpp::Device& hDevice ) :
        vpp::Computation ( pred ),
        d_pipeline ( hDevice ),
        d_dataBlock ( d_pipeline ),
        d_source1 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_source2 ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
        d_target ( VECTOR_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    d_pipeline.definition().setData ( d_source1, d_source2, d_target, & d_dataBlock );
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
    {
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
    }
    End();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KConstructTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KConstructTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::StorageBufferView& vs1,
        const vpp::StorageBufferView& vs2,
        const vpp::StorageBufferView& vt,
        int size,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_source1;
    vpp::ioBuffer d_source2;
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
    const vpp::StorageBufferView& vs1,
    const vpp::StorageBufferView& vs2,
    const vpp::StorageBufferView& vt,
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

    typedef vpp::gvector< int, vpp::Buf::STORAGE > InIntVector;
    typedef vpp::gvector< int, vpp::Buf::STORAGE | vpp::Buf::SOURCE > OutIntVector;

    InIntVector d_source1;
    InIntVector d_source2;
    OutIntVector d_target;

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
    XT_LOCALARR1, XT_LOCALARR2, XT_LOCALARR3, XT_LOCALARR4, XT_LOCALARR5,
    XT_SHAREDARR1, XT_SHAREDARR2, XT_SHAREDARR3,

    XT_VECTORS1, XT_VECTORS2, XT_VECTORS3, XT_VECTORS4, XT_VECTORS5, XT_VECTORS6,
    XT_VECTORS7, XT_VECTORS8, XT_VECTORS9, XT_VECTORS10, XT_VECTORS11, XT_VECTORS12,
    XT_VECTORS13, XT_VECTORS14, XT_VECTORS15, XT_VECTORS16, XT_VECTORS17, XT_VECTORS18,

    XT_VECSWIZZLE1_begin,
    XT_VECSWIZZLE1_end = XT_VECSWIZZLE1_begin + 2 + 3 + 4,

    XT_VVECSWIZZLE1_begin = XT_VECSWIZZLE1_end,
    XT_VVECSWIZZLE1_end = XT_VVECSWIZZLE1_begin + 2 + 3 + 4,

    XT_VECSWIZZLE2_begin = XT_VVECSWIZZLE1_end,
    XT_VECSWIZZLE2_end = XT_VECSWIZZLE2_begin + 29,

    XT_VVECSWIZZLE2_begin = XT_VECSWIZZLE2_end,
    XT_VVECSWIZZLE2_end = XT_VVECSWIZZLE2_begin + 3,

    XT_VECSWIZZLE3_begin = XT_VVECSWIZZLE2_end,
    XT_VECSWIZZLE3_end = XT_VECSWIZZLE3_begin + 64,

    XT_VVECSWIZZLE3_begin = XT_VECSWIZZLE3_end,
    XT_VVECSWIZZLE3_end = XT_VVECSWIZZLE3_begin + 4,

    XT_VECSWIZZLE4_begin = XT_VVECSWIZZLE3_end,
    XT_VECSWIZZLE4_end = XT_VECSWIZZLE4_begin + 8,

    XT_VVECSWIZZLE4_begin = XT_VECSWIZZLE4_end,
    XT_VVECSWIZZLE4_end = XT_VVECSWIZZLE4_begin + 4,

    XT_WRVECSWIZZLE4_begin = XT_VVECSWIZZLE4_end,
    XT_WRVECSWIZZLE4_end = XT_WRVECSWIZZLE4_begin + 7,

    XT_SWIZZLEIND_begin = XT_WRVECSWIZZLE4_end,
    XT_SWIZZLEIND_end = XT_SWIZZLEIND_begin + 4,

    XT_MAT_2x2_0 = XT_SWIZZLEIND_end,
    XT_MAT_2x2_1, XT_MAT_2x2_2, XT_MAT_2x2_3,

    XT_MAT_2x2_D0, XT_MAT_2x2_D1, XT_MAT_2x2_D2, XT_MAT_2x2_D3,
    XT_MAT_2x2_C0, XT_MAT_2x2_C1, XT_MAT_2x2_C2, XT_MAT_2x2_C3,
    XT_MAT_2x2_V0, XT_MAT_2x2_V1, XT_MAT_2x2_V2, XT_MAT_2x2_V3,
    XT_MAT_2x2_V10, XT_MAT_2x2_V11, XT_MAT_2x2_V12, XT_MAT_2x2_V13,
    XT_MAT_2x2_V20, XT_MAT_2x2_V21, XT_MAT_2x2_V22, XT_MAT_2x2_V23,
    
    XT_count
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
typedef vpp::gvector< CTestStruct, vpp::Buf::UNIFORM | vpp::Buf::STORAGE | vpp::Buf::SOURCE > CTestStructBuffer;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KIndexingTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KIndexingTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const vpp::StorageBufferView& vs1,
        const vpp::StorageBufferView& vs2,
        const vpp::StorageBufferView& vt,
        int size,
        const vpp::StorageBufferView& tb1,
        const vpp::StorageBufferView& tb2,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_source1;
    vpp::ioBuffer d_source2;
    vpp::ioBuffer d_target;

    vpp::ioBuffer d_uniTestBuf1;
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
    const vpp::StorageBufferView& vs1,
    const vpp::StorageBufferView& vs2,
    const vpp::StorageBufferView& vt,
    int size,
    const vpp::StorageBufferView& tb1,
    const vpp::StorageBufferView& tb2,
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

    VArray< Int > intArray ( 5 );
    intArray [ 0 ] = 0;
    intArray [ 1 ] = 1;
    intArray [ 2 ] = 4;
    intArray [ 3 ] = 9;
    intArray [ 4 ] = 16;

    WArray< Int > sharedIntArray ( 5 );
    sharedIntArray [ 0 ] = 0;
    sharedIntArray [ 1 ] = 0;
    sharedIntArray [ 2 ] = 0;
    sharedIntArray [ 3 ] = 0;
    sharedIntArray [ 4 ] = 0;

    VInt vidx;

    If ( g == 0 && l == 0 );
    {
        const Int idx1 = inSource1 [ XT_LOCALARR1 ];
        outTarget [ XT_LOCALARR1 ] = intArray [ idx1 ];

        const UInt idx2 = StaticCast< UInt >( inSource1 [ XT_LOCALARR2 ] );
        outTarget [ XT_LOCALARR2 ] = intArray [ idx2 ];

        outTarget [ XT_LOCALARR3 ] = intArray [ inSource1 [ XT_LOCALARR3 ] ];

        intArray [ inSource1 [ XT_LOCALARR4 ] ] = intArray [ inSource2 [ XT_LOCALARR4 ] ];
        outTarget [ XT_LOCALARR4 ] = intArray [ inSource1 [ XT_LOCALARR4 ] ];

        outTarget [ XT_LOCALARR5 ] =
            intArray [ inSource1 [ XT_LOCALARR5 ] ]
            + intArray [ inSource2 [ XT_LOCALARR5 ] ];

        sharedIntArray [ 0 ] = 1;
        sharedIntArray [ 1 ] = 2;
        sharedIntArray [ 2 ] = 3;
        sharedIntArray [ 3 ] = 5;
        sharedIntArray [ 4 ] = 7;
    }
    Fi();

    If ( g == 0 && l == 1 );
    {
        outTarget [ XT_SHAREDARR1 ] = sharedIntArray [ inSource1 [ XT_SHAREDARR1 ] ];
        outTarget [ XT_SHAREDARR2 ] = sharedIntArray [ inSource1 [ XT_SHAREDARR2 ] ];
        outTarget [ XT_SHAREDARR3 ] = sharedIntArray [ inSource1 [ XT_SHAREDARR3 ] ];
    }
    Fi();

    If ( g == 0 && l == 0 );
    {
        // normal

        IVec2 ivec2 { 11, 13 };
        outTarget [ XT_VECTORS1 ] = ivec2 [ 1 ];
        const Int idx2 = inSource1 [ XT_VECTORS2 ];
        outTarget [ XT_VECTORS2 ] = ivec2 [ idx2 ];
        outTarget [ XT_VECTORS3 ] = ivec2 [ inSource1 [ XT_VECTORS3 ] ];

        IVec3 ivec3 { 17, 19, 23 };
        outTarget [ XT_VECTORS4 ] = ivec3 [ 1 ];
        const Int idx3 = inSource1 [ XT_VECTORS5 ];
        outTarget [ XT_VECTORS5 ] = ivec3 [ idx3 ];
        outTarget [ XT_VECTORS6 ] = ivec3 [ inSource1 [ XT_VECTORS6 ] ];

        IVec4 ivec4 { 29, 31, 37, 41 };
        outTarget [ XT_VECTORS7 ] = ivec4 [ 1 ];
        const Int idx4 = inSource1 [ XT_VECTORS8 ];
        outTarget [ XT_VECTORS8 ] = ivec4 [ idx4 ];
        outTarget [ XT_VECTORS9 ] = ivec4 [ inSource1 [ XT_VECTORS9 ] ];

        VIVec2 vivec2 = ivec2;
        outTarget [ XT_VECTORS10 ] = vivec2 [ 1 ];
        const Int idx5 = inSource1 [ XT_VECTORS11 ];
        outTarget [ XT_VECTORS11 ] = vivec2 [ idx5 ];
        outTarget [ XT_VECTORS12 ] = vivec2 [ inSource1 [ XT_VECTORS12 ] ];

        VIVec3 vivec3;
        vivec3 = ivec3;
        outTarget [ XT_VECTORS13 ] = vivec3 [ 1 ];
        const Int idx6 = inSource1 [ XT_VECTORS14 ];
        outTarget [ XT_VECTORS14 ] = vivec3 [ idx6 ];
        outTarget [ XT_VECTORS15 ] = vivec3 [ inSource1 [ XT_VECTORS15 ] ];

        VIVec4 vivec4 { 1, 1, 1, 1 };

        For ( vidx, 0, 4 );
            vivec4 [ vidx ] = ivec4 [ vidx ];
        Rof();

        outTarget [ XT_VECTORS16 ] = vivec4 [ 1 ];
        const Int idx7 = inSource1 [ XT_VECTORS17 ];
        outTarget [ XT_VECTORS17 ] = vivec4 [ idx7 ];
        outTarget [ XT_VECTORS18 ] = vivec4 [ inSource1 [ XT_VECTORS18 ] ];

        // swizzled reads

        outTarget [ XT_VECSWIZZLE1_begin + 0 ] = ivec2 [ X ];
        outTarget [ XT_VECSWIZZLE1_begin + 1 ] = ivec2 [ Y ];
        outTarget [ XT_VECSWIZZLE1_begin + 2 ] = ivec3 [ X ];
        outTarget [ XT_VECSWIZZLE1_begin + 3 ] = ivec3 [ Y ];
        outTarget [ XT_VECSWIZZLE1_begin + 4 ] = ivec3 [ Z ];
        outTarget [ XT_VECSWIZZLE1_begin + 5 ] = ivec4 [ X ];
        outTarget [ XT_VECSWIZZLE1_begin + 6 ] = ivec4 [ Y ];
        outTarget [ XT_VECSWIZZLE1_begin + 7 ] = ivec4 [ Z ];
        outTarget [ XT_VECSWIZZLE1_begin + 8 ] = ivec4 [ W ];
        outTarget [ XT_VVECSWIZZLE1_begin + 0 ] = vivec2 [ X ];
        outTarget [ XT_VVECSWIZZLE1_begin + 1 ] = vivec2 [ Y ];
        outTarget [ XT_VVECSWIZZLE1_begin + 2 ] = vivec3 [ X ];
        outTarget [ XT_VVECSWIZZLE1_begin + 3 ] = vivec3 [ Y ];
        outTarget [ XT_VVECSWIZZLE1_begin + 4 ] = vivec3 [ Z ];
        outTarget [ XT_VVECSWIZZLE1_begin + 5 ] = vivec4 [ X ];
        outTarget [ XT_VVECSWIZZLE1_begin + 6 ] = vivec4 [ Y ];
        outTarget [ XT_VVECSWIZZLE1_begin + 7 ] = vivec4 [ Z ];
        outTarget [ XT_VVECSWIZZLE1_begin + 8 ] = vivec4 [ W ];

        IVec2 swtest2 { 0, 1 };
        IVec3 swtest3 { 0, 1, 2 };
        IVec4 swtest4 { 0, 1, 2, 3 };

        outTarget [ XT_VECSWIZZLE2_begin +  0 ] = testSwizzle ( swtest2, XX );
        outTarget [ XT_VECSWIZZLE2_begin +  1 ] = testSwizzle ( swtest2, YX );
        outTarget [ XT_VECSWIZZLE2_begin +  2 ] = testSwizzle ( swtest2, XY );
        outTarget [ XT_VECSWIZZLE2_begin +  3 ] = testSwizzle ( swtest2, YY );
        outTarget [ XT_VECSWIZZLE2_begin +  4 ] = testSwizzle ( swtest3, XX );
        outTarget [ XT_VECSWIZZLE2_begin +  5 ] = testSwizzle ( swtest3, YX );
        outTarget [ XT_VECSWIZZLE2_begin +  6 ] = testSwizzle ( swtest3, ZX );
        outTarget [ XT_VECSWIZZLE2_begin +  7 ] = testSwizzle ( swtest3, XY );
        outTarget [ XT_VECSWIZZLE2_begin +  8 ] = testSwizzle ( swtest3, YY );
        outTarget [ XT_VECSWIZZLE2_begin +  9 ] = testSwizzle ( swtest3, ZY );
        outTarget [ XT_VECSWIZZLE2_begin + 10 ] = testSwizzle ( swtest3, XZ );
        outTarget [ XT_VECSWIZZLE2_begin + 11 ] = testSwizzle ( swtest3, YZ );
        outTarget [ XT_VECSWIZZLE2_begin + 12 ] = testSwizzle ( swtest3, ZZ );
        outTarget [ XT_VECSWIZZLE2_begin + 13 ] = testSwizzle ( swtest4, XX );
        outTarget [ XT_VECSWIZZLE2_begin + 14 ] = testSwizzle ( swtest4, YX );
        outTarget [ XT_VECSWIZZLE2_begin + 15 ] = testSwizzle ( swtest4, ZX );
        outTarget [ XT_VECSWIZZLE2_begin + 16 ] = testSwizzle ( swtest4, WX );
        outTarget [ XT_VECSWIZZLE2_begin + 17 ] = testSwizzle ( swtest4, XY );
        outTarget [ XT_VECSWIZZLE2_begin + 18 ] = testSwizzle ( swtest4, YY );
        outTarget [ XT_VECSWIZZLE2_begin + 19 ] = testSwizzle ( swtest4, ZY );
        outTarget [ XT_VECSWIZZLE2_begin + 20 ] = testSwizzle ( swtest4, WY );
        outTarget [ XT_VECSWIZZLE2_begin + 21 ] = testSwizzle ( swtest4, XZ );
        outTarget [ XT_VECSWIZZLE2_begin + 22 ] = testSwizzle ( swtest4, YZ );
        outTarget [ XT_VECSWIZZLE2_begin + 23 ] = testSwizzle ( swtest4, ZZ );
        outTarget [ XT_VECSWIZZLE2_begin + 24 ] = testSwizzle ( swtest4, WZ );
        outTarget [ XT_VECSWIZZLE2_begin + 25 ] = testSwizzle ( swtest4, XW );
        outTarget [ XT_VECSWIZZLE2_begin + 26 ] = testSwizzle ( swtest4, YW );
        outTarget [ XT_VECSWIZZLE2_begin + 27 ] = testSwizzle ( swtest4, ZW );
        outTarget [ XT_VECSWIZZLE2_begin + 28 ] = testSwizzle ( swtest4, WW );

        vivec2 = swtest2;
        vivec3 = swtest3;
        vivec4 = swtest4;

        outTarget [ XT_VVECSWIZZLE2_begin + 0 ] = testSwizzle ( vivec2, YY );
        outTarget [ XT_VVECSWIZZLE2_begin + 1 ] = testSwizzle ( vivec3, ZZ );
        outTarget [ XT_VVECSWIZZLE2_begin + 2 ] = testSwizzle ( vivec4, WW );

        outTarget [ XT_VECSWIZZLE3_begin +  0 ] = testSwizzle ( swtest4, XXX );
        outTarget [ XT_VECSWIZZLE3_begin +  1 ] = testSwizzle ( swtest4, YXX );
        outTarget [ XT_VECSWIZZLE3_begin +  1 ] = testSwizzle ( swtest4, ZXX );
        outTarget [ XT_VECSWIZZLE3_begin +  3 ] = testSwizzle ( swtest4, WXX );
        outTarget [ XT_VECSWIZZLE3_begin +  4 ] = testSwizzle ( swtest4, XYX );
        outTarget [ XT_VECSWIZZLE3_begin +  5 ] = testSwizzle ( swtest4, YYX );
        outTarget [ XT_VECSWIZZLE3_begin +  6 ] = testSwizzle ( swtest4, ZYX );
        outTarget [ XT_VECSWIZZLE3_begin +  7 ] = testSwizzle ( swtest4, WYX );
        outTarget [ XT_VECSWIZZLE3_begin +  8 ] = testSwizzle ( swtest4, XZX );
        outTarget [ XT_VECSWIZZLE3_begin +  9 ] = testSwizzle ( swtest4, YZX );
        outTarget [ XT_VECSWIZZLE3_begin + 10 ] = testSwizzle ( swtest4, ZZX );
        outTarget [ XT_VECSWIZZLE3_begin + 11 ] = testSwizzle ( swtest4, WZX );
        outTarget [ XT_VECSWIZZLE3_begin + 11 ] = testSwizzle ( swtest4, XWX );
        outTarget [ XT_VECSWIZZLE3_begin + 13 ] = testSwizzle ( swtest4, YWX );
        outTarget [ XT_VECSWIZZLE3_begin + 14 ] = testSwizzle ( swtest4, ZWX );
        outTarget [ XT_VECSWIZZLE3_begin + 15 ] = testSwizzle ( swtest4, WWX );
        outTarget [ XT_VECSWIZZLE3_begin + 16 ] = testSwizzle ( swtest4, XXY );
        outTarget [ XT_VECSWIZZLE3_begin + 17 ] = testSwizzle ( swtest4, YXY );
        outTarget [ XT_VECSWIZZLE3_begin + 18 ] = testSwizzle ( swtest4, ZXY );
        outTarget [ XT_VECSWIZZLE3_begin + 19 ] = testSwizzle ( swtest4, WXY );
        outTarget [ XT_VECSWIZZLE3_begin + 20 ] = testSwizzle ( swtest4, XYY );
        outTarget [ XT_VECSWIZZLE3_begin + 21 ] = testSwizzle ( swtest4, YYY );
        outTarget [ XT_VECSWIZZLE3_begin + 21 ] = testSwizzle ( swtest4, ZYY );
        outTarget [ XT_VECSWIZZLE3_begin + 23 ] = testSwizzle ( swtest4, WYY );
        outTarget [ XT_VECSWIZZLE3_begin + 24 ] = testSwizzle ( swtest4, XZY );
        outTarget [ XT_VECSWIZZLE3_begin + 25 ] = testSwizzle ( swtest4, YZY );
        outTarget [ XT_VECSWIZZLE3_begin + 26 ] = testSwizzle ( swtest4, ZZY );
        outTarget [ XT_VECSWIZZLE3_begin + 27 ] = testSwizzle ( swtest4, WZY );
        outTarget [ XT_VECSWIZZLE3_begin + 28 ] = testSwizzle ( swtest4, XWY );
        outTarget [ XT_VECSWIZZLE3_begin + 29 ] = testSwizzle ( swtest4, YWY );
        outTarget [ XT_VECSWIZZLE3_begin + 30 ] = testSwizzle ( swtest4, ZWY );
        outTarget [ XT_VECSWIZZLE3_begin + 31 ] = testSwizzle ( swtest4, WWY );
        outTarget [ XT_VECSWIZZLE3_begin + 31 ] = testSwizzle ( swtest4, XXZ );
        outTarget [ XT_VECSWIZZLE3_begin + 33 ] = testSwizzle ( swtest4, YXZ );
        outTarget [ XT_VECSWIZZLE3_begin + 34 ] = testSwizzle ( swtest4, ZXZ );
        outTarget [ XT_VECSWIZZLE3_begin + 35 ] = testSwizzle ( swtest4, WXZ );
        outTarget [ XT_VECSWIZZLE3_begin + 36 ] = testSwizzle ( swtest4, XYZ );
        outTarget [ XT_VECSWIZZLE3_begin + 37 ] = testSwizzle ( swtest4, YYZ );
        outTarget [ XT_VECSWIZZLE3_begin + 38 ] = testSwizzle ( swtest4, ZYZ );
        outTarget [ XT_VECSWIZZLE3_begin + 39 ] = testSwizzle ( swtest4, WYZ );
        outTarget [ XT_VECSWIZZLE3_begin + 40 ] = testSwizzle ( swtest4, XZZ );
        outTarget [ XT_VECSWIZZLE3_begin + 41 ] = testSwizzle ( swtest4, YZZ );
        outTarget [ XT_VECSWIZZLE3_begin + 41 ] = testSwizzle ( swtest4, ZZZ );
        outTarget [ XT_VECSWIZZLE3_begin + 43 ] = testSwizzle ( swtest4, WZZ );
        outTarget [ XT_VECSWIZZLE3_begin + 44 ] = testSwizzle ( swtest4, XWZ );
        outTarget [ XT_VECSWIZZLE3_begin + 45 ] = testSwizzle ( swtest4, YWZ );
        outTarget [ XT_VECSWIZZLE3_begin + 46 ] = testSwizzle ( swtest4, ZWZ );
        outTarget [ XT_VECSWIZZLE3_begin + 47 ] = testSwizzle ( swtest4, WWZ );
        outTarget [ XT_VECSWIZZLE3_begin + 48 ] = testSwizzle ( swtest4, XXW );
        outTarget [ XT_VECSWIZZLE3_begin + 49 ] = testSwizzle ( swtest4, YXW );
        outTarget [ XT_VECSWIZZLE3_begin + 50 ] = testSwizzle ( swtest4, ZXW );
        outTarget [ XT_VECSWIZZLE3_begin + 51 ] = testSwizzle ( swtest4, WXW );
        outTarget [ XT_VECSWIZZLE3_begin + 51 ] = testSwizzle ( swtest4, XYW );
        outTarget [ XT_VECSWIZZLE3_begin + 53 ] = testSwizzle ( swtest4, YYW );
        outTarget [ XT_VECSWIZZLE3_begin + 54 ] = testSwizzle ( swtest4, ZYW );
        outTarget [ XT_VECSWIZZLE3_begin + 55 ] = testSwizzle ( swtest4, WYW );
        outTarget [ XT_VECSWIZZLE3_begin + 56 ] = testSwizzle ( swtest4, XZW );
        outTarget [ XT_VECSWIZZLE3_begin + 57 ] = testSwizzle ( swtest4, YZW );
        outTarget [ XT_VECSWIZZLE3_begin + 58 ] = testSwizzle ( swtest4, ZZW );
        outTarget [ XT_VECSWIZZLE3_begin + 59 ] = testSwizzle ( swtest4, WZW );
        outTarget [ XT_VECSWIZZLE3_begin + 60 ] = testSwizzle ( swtest4, XWW );
        outTarget [ XT_VECSWIZZLE3_begin + 61 ] = testSwizzle ( swtest4, YWW );
        outTarget [ XT_VECSWIZZLE3_begin + 62 ] = testSwizzle ( swtest4, ZWW );
        outTarget [ XT_VECSWIZZLE3_begin + 63 ] = testSwizzle ( swtest4, WWW );

        outTarget [ XT_VVECSWIZZLE3_begin + 0 ] = testSwizzle ( vivec4, YYY );
        outTarget [ XT_VVECSWIZZLE3_begin + 1 ] = testSwizzle ( vivec4, WYX );
        outTarget [ XT_VVECSWIZZLE3_begin + 2 ] = testSwizzle ( vivec4, ZYW );
        outTarget [ XT_VVECSWIZZLE3_begin + 3 ] = testSwizzle ( vivec4, WWW );

        outTarget [ XT_VECSWIZZLE4_begin + 0 ] = testSwizzle ( swtest4, XYZW );
        outTarget [ XT_VECSWIZZLE4_begin + 1 ] = testSwizzle ( swtest4, WZYX );
        outTarget [ XT_VECSWIZZLE4_begin + 2 ] = testSwizzle ( swtest4, YXWZ );
        outTarget [ XT_VECSWIZZLE4_begin + 3 ] = testSwizzle ( swtest4, ZWXY );
        outTarget [ XT_VECSWIZZLE4_begin + 4 ] = testSwizzle ( swtest4, XXXX );
        outTarget [ XT_VECSWIZZLE4_begin + 5 ] = testSwizzle ( swtest4, YYYY );
        outTarget [ XT_VECSWIZZLE4_begin + 6 ] = testSwizzle ( swtest4, ZZZZ );
        outTarget [ XT_VECSWIZZLE4_begin + 7 ] = testSwizzle ( swtest4, WWWW );

        outTarget [ XT_VVECSWIZZLE4_begin + 0 ] = testSwizzle ( vivec4, XYZW );
        outTarget [ XT_VVECSWIZZLE4_begin + 1 ] = testSwizzle ( vivec4, WZYX );
        outTarget [ XT_VVECSWIZZLE4_begin + 2 ] = testSwizzle ( vivec4, YXWZ );
        outTarget [ XT_VVECSWIZZLE4_begin + 3 ] = testSwizzle ( vivec4, ZWXY );

        // swizzled writes
        vivec4 [ X ] = 3;
        vivec4 [ Y ] = 2;
        vivec4 [ Z ] = 1;
        vivec4 [ W ] = 0;

        outTarget [ XT_WRVECSWIZZLE4_begin ] = testSwizzle ( vivec4, XYZW );

        vivec4 [ WZYX ] = vivec4;
        outTarget [ XT_WRVECSWIZZLE4_begin + 1 ] = testSwizzle ( vivec4, XYZW );

        vivec4 [ ZX ] = swtest4 [ WW ];
        vivec4 [ WY ] = swtest4 [ YY ];
        outTarget [ XT_WRVECSWIZZLE4_begin + 2 ] = testSwizzle ( vivec4, XYZW );

        vivec3 [ X ] = 3;
        vivec3 [ Y ] = 2;
        vivec3 [ Z ] = 1;

        outTarget [ XT_WRVECSWIZZLE4_begin + 3 ] = testSwizzle ( vivec3, XYZ );

        vivec3 [ ZYX ] = vivec3;
        outTarget [ XT_WRVECSWIZZLE4_begin + 4 ] = testSwizzle ( vivec3, XYZ );

        vivec2 [ X ] = 2;
        vivec2 [ Y ] = 1;

        outTarget [ XT_WRVECSWIZZLE4_begin + 5 ] = testSwizzle ( vivec2, XY );

        vivec2 [ YX ] = vivec2;
        outTarget [ XT_WRVECSWIZZLE4_begin + 6 ] = testSwizzle ( vivec2, XY );

        // Swizzles combined with normal indexing
        outTarget [ XT_SWIZZLEIND_begin + 0 ] = swtest4 [ WZYX ][ 0 ];
        outTarget [ XT_SWIZZLEIND_begin + 1 ] = swtest4 [ WZYX ][ 1 ];
        outTarget [ XT_SWIZZLEIND_begin + 2 ] = swtest4 [ WZYX ][ 2 ];
        outTarget [ XT_SWIZZLEIND_begin + 3 ] = swtest4 [ WZYX ][ 3 ];

        // 2x2 matrices
        Mat2 mat2 { 11, 12, 21, 22 };

        outTarget [ XT_MAT_2x2_0 ] = ReinterpretCast< Int >( mat2 [ 0 ][ 0 ] );
        outTarget [ XT_MAT_2x2_1 ] = ReinterpretCast< Int >( mat2 [ 1 ][ 0 ] );
        outTarget [ XT_MAT_2x2_2 ] = ReinterpretCast< Int >( mat2 [ 0 ][ 1 ] );
        outTarget [ XT_MAT_2x2_3 ] = ReinterpretCast< Int >( mat2 [ 1 ][ 1 ] );

        outTarget [ XT_MAT_2x2_D0 ] = ReinterpretCast< Int >( mat2 [ inSource1 [ XT_MAT_2x2_D0 ] ][ inSource2 [ XT_MAT_2x2_D0 ] ] );
        outTarget [ XT_MAT_2x2_D1 ] = ReinterpretCast< Int >( mat2 [ inSource1 [ XT_MAT_2x2_D1 ] ][ inSource2 [ XT_MAT_2x2_D1 ] ] );
        outTarget [ XT_MAT_2x2_D2 ] = ReinterpretCast< Int >( mat2 [ inSource1 [ XT_MAT_2x2_D2 ] ][ inSource2 [ XT_MAT_2x2_D2 ] ] );
        outTarget [ XT_MAT_2x2_D3 ] = ReinterpretCast< Int >( mat2 [ inSource1 [ XT_MAT_2x2_D3 ] ][ inSource2 [ XT_MAT_2x2_D3 ] ] );

        Vec2 mat20 = mat2 [ 0 ];
        Vec2 mat21 = mat2 [ 1 ];

        outTarget [ XT_MAT_2x2_C0 ] = ReinterpretCast< Int >( mat20 [ 0 ] );
        outTarget [ XT_MAT_2x2_C1 ] = ReinterpretCast< Int >( mat21 [ 0 ] );
        outTarget [ XT_MAT_2x2_C2 ] = ReinterpretCast< Int >( mat20 [ 1 ] );
        outTarget [ XT_MAT_2x2_C3 ] = ReinterpretCast< Int >( mat21 [ 1 ] );

        VMat2 vmat2 = mat2;
        outTarget [ XT_MAT_2x2_V0 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V1 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V2 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 1 ] );
        outTarget [ XT_MAT_2x2_V3 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 1 ] );

        vmat2 [ 0 ][ 0 ] = 111.0f;
        vmat2 [ 1 ][ 0 ] = 112.0f;
        vmat2 [ 0 ][ 1 ] = 121.0f;
        vmat2 [ 1 ][ 1 ] = 122.0f;
        outTarget [ XT_MAT_2x2_V10 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V11 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V12 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 1 ] );
        outTarget [ XT_MAT_2x2_V13 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 1 ] );

        vmat2 [ 0 ] = mat2 [ 0 ];
        vmat2 [ 1 ] = mat2 [ 1 ];
        outTarget [ XT_MAT_2x2_V20 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V21 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 0 ] );
        outTarget [ XT_MAT_2x2_V22 ] = ReinterpretCast< Int >( vmat2 [ 0 ][ 1 ] );
        outTarget [ XT_MAT_2x2_V23 ] = ReinterpretCast< Int >( vmat2 [ 1 ][ 1 ] );

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
    void prepareTest ( int testId, int s1, int s2, float exp );

private:
    static const size_t VECTOR_SIZE = 1024;

    typedef KIndexingTestPipeline TestPipeline;

    vpp::ComputePipelineLayout< TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    typedef vpp::gvector< int, vpp::Buf::UNIFORM | vpp::Buf::STORAGE | vpp::Buf::SOURCE > IntVector;

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

void KIndexingTest :: prepareTest (
    int testId, int s1, int s2, float exp )
{
    union { int i; float f; };

    d_source1 [ testId ] = s1;
    d_source2 [ testId ] = s2;
    d_target [ testId ] = 0;
    f = exp;
    d_expected [ testId ] = i;
}

// -----------------------------------------------------------------------------

void KIndexingTest :: prepareVectors()
{
    using namespace vpp;

    const unsigned int nItems = XT_count;

    d_source1.resize ( nItems );
    d_source2.resize ( nItems );
    d_target.resize ( nItems );
    d_expected.resize ( nItems );

    d_uniTestBuf1.resize ( 1 );
    d_storTestBuf1.resize ( 1 );

    prepareTest ( XT_LOCALARR1, 3, 0, 9 );
    prepareTest ( XT_LOCALARR2, 4, 0, 16 );
    prepareTest ( XT_LOCALARR3, 2, 0, 4 );
    prepareTest ( XT_LOCALARR4, 0, 4, 16 );
    prepareTest ( XT_LOCALARR5, 3, 4, 25 );
    prepareTest ( XT_SHAREDARR1, 0, 0, 1 );
    prepareTest ( XT_SHAREDARR2, 2, 0, 3 );
    prepareTest ( XT_SHAREDARR3, 4, 0, 7 );
    prepareTest ( XT_VECTORS1, 0, 0, 13 );
    prepareTest ( XT_VECTORS2, 1, 0, 13 );
    prepareTest ( XT_VECTORS3, 1, 0, 13 );
    prepareTest ( XT_VECTORS4, 0, 0, 19 );
    prepareTest ( XT_VECTORS5, 1, 0, 19 );
    prepareTest ( XT_VECTORS6, 2, 0, 23 );
    prepareTest ( XT_VECTORS7, 0, 0, 31 );
    prepareTest ( XT_VECTORS8, 2, 0, 37 );
    prepareTest ( XT_VECTORS9, 3, 0, 41 );
    prepareTest ( XT_VECTORS10, 0, 0, 13 );
    prepareTest ( XT_VECTORS11, 1, 0, 13 );
    prepareTest ( XT_VECTORS12, 1, 0, 13 );
    prepareTest ( XT_VECTORS13, 0, 0, 19 );
    prepareTest ( XT_VECTORS14, 1, 0, 19 );
    prepareTest ( XT_VECTORS15, 2, 0, 23 );
    prepareTest ( XT_VECTORS16, 0, 0, 31 );
    prepareTest ( XT_VECTORS17, 2, 0, 37 );
    prepareTest ( XT_VECTORS18, 3, 0, 41 );

    prepareTest ( XT_VECSWIZZLE1_begin + 0, 0, 0, 11 );
    prepareTest ( XT_VECSWIZZLE1_begin + 1, 0, 0, 13 );
    prepareTest ( XT_VECSWIZZLE1_begin + 2, 0, 0, 17 );
    prepareTest ( XT_VECSWIZZLE1_begin + 3, 0, 0, 19 );
    prepareTest ( XT_VECSWIZZLE1_begin + 4, 0, 0, 23 );
    prepareTest ( XT_VECSWIZZLE1_begin + 5, 0, 0, 29 );
    prepareTest ( XT_VECSWIZZLE1_begin + 6, 0, 0, 31 );
    prepareTest ( XT_VECSWIZZLE1_begin + 7, 0, 0, 37 );
    prepareTest ( XT_VECSWIZZLE1_begin + 8, 0, 0, 41 );

    prepareTest ( XT_VVECSWIZZLE1_begin + 0, 0, 0, 11 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 1, 0, 0, 13 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 2, 0, 0, 17 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 3, 0, 0, 19 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 4, 0, 0, 23 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 5, 0, 0, 29 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 6, 0, 0, 31 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 7, 0, 0, 37 );
    prepareTest ( XT_VVECSWIZZLE1_begin + 8, 0, 0, 41 );

    prepareTest ( XT_VECSWIZZLE2_begin + 0, 0, 0, 0b0000 );
    prepareTest ( XT_VECSWIZZLE2_begin + 1, 0, 0, 0b0001 );
    prepareTest ( XT_VECSWIZZLE2_begin + 2, 0, 0, 0b0100 );
    prepareTest ( XT_VECSWIZZLE2_begin + 3, 0, 0, 0b0101 );
    prepareTest ( XT_VECSWIZZLE2_begin +  4, 0, 0, 0b0000 );
    prepareTest ( XT_VECSWIZZLE2_begin +  5, 0, 0, 0b0001 );
    prepareTest ( XT_VECSWIZZLE2_begin +  6, 0, 0, 0b0010 );
    prepareTest ( XT_VECSWIZZLE2_begin +  7, 0, 0, 0b0100 );
    prepareTest ( XT_VECSWIZZLE2_begin +  8, 0, 0, 0b0101 );
    prepareTest ( XT_VECSWIZZLE2_begin +  9, 0, 0, 0b0110 );
    prepareTest ( XT_VECSWIZZLE2_begin + 10, 0, 0, 0b1000 );
    prepareTest ( XT_VECSWIZZLE2_begin + 11, 0, 0, 0b1001 );
    prepareTest ( XT_VECSWIZZLE2_begin + 12, 0, 0, 0b1010 );
    prepareTest ( XT_VECSWIZZLE2_begin + 13, 0, 0, 0b0000 );
    prepareTest ( XT_VECSWIZZLE2_begin + 14, 0, 0, 0b0001 );
    prepareTest ( XT_VECSWIZZLE2_begin + 15, 0, 0, 0b0010 );
    prepareTest ( XT_VECSWIZZLE2_begin + 16, 0, 0, 0b0011 );
    prepareTest ( XT_VECSWIZZLE2_begin + 17, 0, 0, 0b0100 );
    prepareTest ( XT_VECSWIZZLE2_begin + 18, 0, 0, 0b0101 );
    prepareTest ( XT_VECSWIZZLE2_begin + 19, 0, 0, 0b0110 );
    prepareTest ( XT_VECSWIZZLE2_begin + 20, 0, 0, 0b0111 );
    prepareTest ( XT_VECSWIZZLE2_begin + 21, 0, 0, 0b1000 );
    prepareTest ( XT_VECSWIZZLE2_begin + 22, 0, 0, 0b1001 );
    prepareTest ( XT_VECSWIZZLE2_begin + 23, 0, 0, 0b1010 );
    prepareTest ( XT_VECSWIZZLE2_begin + 24, 0, 0, 0b1011 );
    prepareTest ( XT_VECSWIZZLE2_begin + 25, 0, 0, 0b1100 );
    prepareTest ( XT_VECSWIZZLE2_begin + 26, 0, 0, 0b1101 );
    prepareTest ( XT_VECSWIZZLE2_begin + 27, 0, 0, 0b1110 );
    prepareTest ( XT_VECSWIZZLE2_begin + 28, 0, 0, 0b1111 );

    prepareTest ( XT_VVECSWIZZLE2_begin + 0, 0, 0, 0b0101 );
    prepareTest ( XT_VVECSWIZZLE2_begin + 1, 0, 0, 0b1010 );
    prepareTest ( XT_VVECSWIZZLE2_begin + 2, 0, 0, 0b1111 );

    prepareTest ( XT_VECSWIZZLE3_begin +  0, 0, 0, 0b000000 );
    prepareTest ( XT_VECSWIZZLE3_begin +  1, 0, 0, 0b000001 );
    prepareTest ( XT_VECSWIZZLE3_begin +  1, 0, 0, 0b000010 );
    prepareTest ( XT_VECSWIZZLE3_begin +  3, 0, 0, 0b000011 );
    prepareTest ( XT_VECSWIZZLE3_begin +  4, 0, 0, 0b000100 );
    prepareTest ( XT_VECSWIZZLE3_begin +  5, 0, 0, 0b000101 );
    prepareTest ( XT_VECSWIZZLE3_begin +  6, 0, 0, 0b000110 );
    prepareTest ( XT_VECSWIZZLE3_begin +  7, 0, 0, 0b000111 );
    prepareTest ( XT_VECSWIZZLE3_begin +  8, 0, 0, 0b001000 );
    prepareTest ( XT_VECSWIZZLE3_begin +  9, 0, 0, 0b001001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 10, 0, 0, 0b001010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 11, 0, 0, 0b001011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 11, 0, 0, 0b001100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 13, 0, 0, 0b001101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 14, 0, 0, 0b001110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 15, 0, 0, 0b001111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 16, 0, 0, 0b010000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 17, 0, 0, 0b010001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 18, 0, 0, 0b010010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 19, 0, 0, 0b010011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 20, 0, 0, 0b010100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 21, 0, 0, 0b010101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 21, 0, 0, 0b010110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 23, 0, 0, 0b010111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 24, 0, 0, 0b011000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 25, 0, 0, 0b011001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 26, 0, 0, 0b011010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 27, 0, 0, 0b011011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 28, 0, 0, 0b011100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 29, 0, 0, 0b011101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 30, 0, 0, 0b011110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 31, 0, 0, 0b011111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 31, 0, 0, 0b100000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 33, 0, 0, 0b100001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 34, 0, 0, 0b100010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 35, 0, 0, 0b100011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 36, 0, 0, 0b100100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 37, 0, 0, 0b100101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 38, 0, 0, 0b100110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 39, 0, 0, 0b100111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 40, 0, 0, 0b101000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 41, 0, 0, 0b101001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 41, 0, 0, 0b101010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 43, 0, 0, 0b101011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 44, 0, 0, 0b101100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 45, 0, 0, 0b101101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 46, 0, 0, 0b101110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 47, 0, 0, 0b101111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 48, 0, 0, 0b110000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 49, 0, 0, 0b110001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 50, 0, 0, 0b110010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 51, 0, 0, 0b110011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 51, 0, 0, 0b110100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 53, 0, 0, 0b110101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 54, 0, 0, 0b110110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 55, 0, 0, 0b110111 );
    prepareTest ( XT_VECSWIZZLE3_begin + 56, 0, 0, 0b111000 );
    prepareTest ( XT_VECSWIZZLE3_begin + 57, 0, 0, 0b111001 );
    prepareTest ( XT_VECSWIZZLE3_begin + 58, 0, 0, 0b111010 );
    prepareTest ( XT_VECSWIZZLE3_begin + 59, 0, 0, 0b111011 );
    prepareTest ( XT_VECSWIZZLE3_begin + 60, 0, 0, 0b111100 );
    prepareTest ( XT_VECSWIZZLE3_begin + 61, 0, 0, 0b111101 );
    prepareTest ( XT_VECSWIZZLE3_begin + 62, 0, 0, 0b111110 );
    prepareTest ( XT_VECSWIZZLE3_begin + 63, 0, 0, 0b111111 );

    prepareTest ( XT_VVECSWIZZLE3_begin + 0, 0, 0, 0b010101 );
    prepareTest ( XT_VVECSWIZZLE3_begin + 1, 0, 0, 0b000111 );
    prepareTest ( XT_VVECSWIZZLE3_begin + 2, 0, 0, 0b110110 );
    prepareTest ( XT_VVECSWIZZLE3_begin + 3, 0, 0, 0b111111 );

    prepareTest ( XT_VECSWIZZLE4_begin + 0, 0, 0, 0b11100100 );
    prepareTest ( XT_VECSWIZZLE4_begin + 1, 0, 0, 0b00011011 );
    prepareTest ( XT_VECSWIZZLE4_begin + 2, 0, 0, 0b10110001 );
    prepareTest ( XT_VECSWIZZLE4_begin + 3, 0, 0, 0b01001110 );
    prepareTest ( XT_VECSWIZZLE4_begin + 4, 0, 0, 0b00000000 );
    prepareTest ( XT_VECSWIZZLE4_begin + 5, 0, 0, 0b01010101 );
    prepareTest ( XT_VECSWIZZLE4_begin + 6, 0, 0, 0b10101010 );
    prepareTest ( XT_VECSWIZZLE4_begin + 7, 0, 0, 0b11111111 );

    prepareTest ( XT_VVECSWIZZLE4_begin + 0, 0, 0, 0b11100100 );
    prepareTest ( XT_VVECSWIZZLE4_begin + 1, 0, 0, 0b00011011 );
    prepareTest ( XT_VVECSWIZZLE4_begin + 2, 0, 0, 0b10110001 );
    prepareTest ( XT_VVECSWIZZLE4_begin + 3, 0, 0, 0b01001110 );

    prepareTest ( XT_WRVECSWIZZLE4_begin + 0, 0, 0, 0b00011011 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 1, 0, 0, 0b11100100 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 2, 0, 0, 0b01110111 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 3, 0, 0, 0b011011 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 4, 0, 0, 0b111001 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 5, 0, 0, 0b0110 );
    prepareTest ( XT_WRVECSWIZZLE4_begin + 6, 0, 0, 0b1001 );

    prepareTest ( XT_SWIZZLEIND_begin + 0, 0, 0, 3 );
    prepareTest ( XT_SWIZZLEIND_begin + 1, 0, 0, 2 );
    prepareTest ( XT_SWIZZLEIND_begin + 2, 0, 0, 1 );
    prepareTest ( XT_SWIZZLEIND_begin + 3, 0, 0, 0 );

    prepareTest ( XT_MAT_2x2_0, 0, 0, 11.0f );
    prepareTest ( XT_MAT_2x2_1, 0, 0, 12.0f );
    prepareTest ( XT_MAT_2x2_2, 0, 0, 21.0f );
    prepareTest ( XT_MAT_2x2_3, 0, 0, 22.0f );

    prepareTest ( XT_MAT_2x2_D0, 0, 0, 11.0f );
    prepareTest ( XT_MAT_2x2_D1, 1, 0, 12.0f );
    prepareTest ( XT_MAT_2x2_D2, 0, 1, 21.0f );
    prepareTest ( XT_MAT_2x2_D3, 1, 1, 22.0f );

    prepareTest ( XT_MAT_2x2_C0, 0, 0, 11.0f );
    prepareTest ( XT_MAT_2x2_C1, 0, 0, 12.0f );
    prepareTest ( XT_MAT_2x2_C2, 0, 0, 21.0f );
    prepareTest ( XT_MAT_2x2_C3, 0, 0, 22.0f );

    prepareTest ( XT_MAT_2x2_V0, 0, 0, 11.0f );
    prepareTest ( XT_MAT_2x2_V1, 0, 0, 12.0f );
    prepareTest ( XT_MAT_2x2_V2, 0, 0, 21.0f );
    prepareTest ( XT_MAT_2x2_V3, 0, 0, 22.0f );

    prepareTest ( XT_MAT_2x2_V10, 0, 0, 111.0f );
    prepareTest ( XT_MAT_2x2_V11, 0, 0, 112.0f );
    prepareTest ( XT_MAT_2x2_V12, 0, 0, 121.0f );
    prepareTest ( XT_MAT_2x2_V13, 0, 0, 122.0f );

    prepareTest ( XT_MAT_2x2_V20, 0, 0, 11.0f );
    prepareTest ( XT_MAT_2x2_V21, 0, 0, 12.0f );
    prepareTest ( XT_MAT_2x2_V22, 0, 0, 21.0f );
    prepareTest ( XT_MAT_2x2_V23, 0, 0, 22.0f );

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

    for ( size_t i = 0; i != XT_count; ++i )
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
        const vpp::StorageBufferView& rb,
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
    const vpp::StorageBufferView& rb,
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

    typedef vpp::gvector< vpp::matr2, vpp::Buf::STORAGE | vpp::Buf::SOURCE > MatrixVector;
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
    typedef vpp::TexelBufferView< Fmt4xF32, vpp::Buf::UNITEX | vpp::Buf::STORTEX | vpp::Buf::SOURCE > View4xF32;
    typedef Fmt4xF32::data_type Data4xF32;
    typedef vpp::gvector< Data4xF32, vpp::Buf::UNITEX | vpp::Buf::STORTEX | vpp::Buf::SOURCE > Vec4xF32;
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
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioImage< View4xF32 > m_inImgBuf4xF32;
    vpp::ioImage< View4xF32 > m_ioImgBuf4xF32;
    vpp::ioImage< View4xFU8 > m_inImgBuf4xFU8;
    vpp::ioImage< View4xFU8 > m_ioImgBuf4xFU8;
    vpp::ioImage< View2xFS8 > m_inImgBuf2xFS8;
    vpp::ioImage< View2xFS8 > m_ioImgBuf2xFS8;

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
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        m_inImgBuf4xF32 = inImgBuf4xF32,
        m_ioImgBuf4xF32 = ioImgBuf4xF32,
        m_inImgBuf4xFU8 = inImgBuf4xFU8,
        m_ioImgBuf4xFU8 = ioImgBuf4xFU8,
        m_inImgBuf2xFS8 = inImgBuf2xFS8,
        m_ioImgBuf2xFS8 = ioImgBuf2xFS8
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
    d_targetView2xFS8 ( d_targetImg2xFS8 )
{
    using namespace vpp;

    d_pipeline.definition().setBuffers (
        d_sourceView4xF32,
        d_targetView4xF32,
        d_sourceView4xFU8,
        d_targetView4xFU8,
        d_sourceView2xFS8,
        d_targetView2xFS8,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        
        d_sourceBuf4xF32.cmdCopyToImage (
            d_sourceImg4xF32,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xF32.cmdCopyToImage (
            d_targetImg4xF32,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_sourceBuf4xFU8.cmdCopyToImage (
            d_sourceImg4xFU8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf4xFU8.cmdCopyToImage (
            d_targetImg4xFU8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_sourceBuf2xFS8.cmdCopyToImage (
            d_sourceImg2xFS8,
            VK_IMAGE_LAYOUT_GENERAL,
            0, 0, { 0, 0, 0 }, { WIDTH, HEIGHT, 1 }, 0, WIDTH, HEIGHT );

        d_targetBuf2xFS8.cmdCopyToImage (
            d_targetImg2xFS8,
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
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Synchronization tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KGlobalSyncTestTypes
{
    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::SOURCE | vpp::Buf::TARGET > DataBuffer;
    typedef vpp::format< unsigned int > FormatU32;

    typedef vpp::ImageAttributes<
        FormatU32, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::STORAGE | vpp::Img::TARGET,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > TestImageAttr;

    typedef vpp::Image< TestImageAttr > TestImage;
    typedef vpp::ImageViewAttributes< TestImage > TestImageViewAttr;
    typedef vpp::ImageView< TestImageViewAttr > TestImageView;
};

// -----------------------------------------------------------------------------

class KGlobalSyncTestPipeline :
    public vpp::ComputePipelineConfig,
    public KGlobalSyncTestTypes
{
public:
    KGlobalSyncTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const DataBuffer& resultView,
        const TestImageView& testImageView,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_sourceBuffer;
    vpp::ioBuffer d_resultBuffer;
    vpp::ioImage< TestImageView, vpp::MEM_COHERENT | vpp::MEM_VOLATILE > d_testImage;
    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KGlobalSyncTestPipeline :: KGlobalSyncTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KGlobalSyncTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KGlobalSyncTestPipeline :: setData (
    const DataBuffer& resultView,
    const TestImageView& testImageView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_resultBuffer = resultView,
        d_testImage = testImageView
    ));
}

// -----------------------------------------------------------------------------

void KGlobalSyncTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    // pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    UseImage ( d_testImage );

    If ( g == 0 && l == 0 );
        ImageStore ( d_testImage, IVec2 { 0, 0 }, 77u );
    Fi();

    //DeviceBarrier ( spv::MemorySemanticsImageMemoryMask | spv::MemorySemanticsAcquireReleaseMask );

    const UInt v = ImageLoad ( d_testImage, IVec2 { 0, 0 } )[ X ];

    UniformSimpleArray< unsigned int, decltype ( d_resultBuffer ) > outResultBuffer ( d_resultBuffer );
    outResultBuffer [ ( g << 5 ) | l ] = v;
}

// -----------------------------------------------------------------------------

class KGlobalSyncTest :
    public vpp::Computation,
    public KGlobalSyncTestTypes
{
public:
    KGlobalSyncTest ( vpp::Computation& pred,  const vpp::Device& hDevice );

    void compareResults();

private:
    vpp::ComputePipelineLayout< KGlobalSyncTestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    DataBuffer d_resultBuffer;
    TestImage d_testImage;
    TestImageView d_testImageView;
};

// -----------------------------------------------------------------------------

KGlobalSyncTest :: KGlobalSyncTest ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_resultBuffer ( 2048, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testImage ( { 256, 1, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testImageView ( d_testImage )
{
    using namespace vpp;

    d_pipeline.definition().setData ( d_resultBuffer, d_testImageView, & d_dataBlock );
    addPipeline ( d_pipeline );

    d_resultBuffer.resize ( 2048 );

    ( *this ) << [ this ]()
    {
        cmdFillBuffer ( d_resultBuffer, 0, 2048, 0 );

        cmdBufferPipelineBarrier (
            d_resultBuffer, 
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_ACCESS_SHADER_READ_BIT );

        cmdClearColorImage ( d_testImage, clearColor ( 0, 0, 0 ) );

        cmdPipelineBarrier ( barriers ( Bar::TRANSFER, Bar::COMPUTE, d_testImage ) );

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdPipelineBarrier ( barriers ( Bar::COMPUTE, Bar::TRANSFER, d_resultBuffer ) );

        d_resultBuffer.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KGlobalSyncTest :: compareResults()
{
    std::vector< unsigned int > values ( d_resultBuffer.begin(), d_resultBuffer.end() );

    // FIXME - this test fails anyway because DeviceBarrier does not work (at least
    // on GTX 960)
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Local arrays tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KLocalArraysTestTypes
{
    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferI;
    typedef vpp::gvector< float, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferF;

    typedef vpp::format< unsigned int > FormatU32;
    typedef vpp::format< float > FormatF32;

    static void computeTestOffsets ( std::vector< unsigned int >* pOffsets );
    static unsigned int totalTestVectorLength();

    static const unsigned int s_testSizes[];
    static const unsigned int s_testCount;
};

// -----------------------------------------------------------------------------

const unsigned int KLocalArraysTestTypes :: s_testSizes[] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 256
};

// -----------------------------------------------------------------------------

const unsigned int KLocalArraysTestTypes :: s_testCount =
    sizeof ( s_testSizes ) / sizeof ( unsigned int );

// -----------------------------------------------------------------------------

void KLocalArraysTestTypes :: computeTestOffsets ( std::vector< unsigned int >* pOffsets )
{
    pOffsets->resize ( s_testCount + 1 );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i != s_testCount; ++i )
    {
        ( *pOffsets )[ i ] = offset;
        offset += s_testSizes [ i ];
    }

    ( *pOffsets )[ s_testCount ] = offset;
}

// -----------------------------------------------------------------------------

unsigned int KLocalArraysTestTypes :: totalTestVectorLength()
{
    return std::accumulate ( s_testSizes + 0, s_testSizes + s_testCount, 0 );
}

// -----------------------------------------------------------------------------

class KLocalArraysTestPipeline :
    public vpp::ComputePipelineConfig,
    public KLocalArraysTestTypes
{
public:
    KLocalArraysTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const DataBufferI& generateResultViewI,
        const DataBufferI& transformResultViewI,
        const DataBufferI& reverseResultViewI,
        const DataBufferF& generateResultViewF,
        const DataBufferF& transformResultViewF,
        const DataBufferF& reverseResultViewF,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_generateResultBufferI;
    vpp::ioBuffer d_transformResultBufferI;
    vpp::ioBuffer d_reverseResultBufferI;
    vpp::ioBuffer d_generateResultBufferF;
    vpp::ioBuffer d_transformResultBufferF;
    vpp::ioBuffer d_reverseResultBufferF;
    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KLocalArraysTestPipeline :: KLocalArraysTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KLocalArraysTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KLocalArraysTestPipeline :: setData (
    const DataBufferI& generateResultViewI,
    const DataBufferI& transformResultViewI,
    const DataBufferI& reverseResultViewI,
    const DataBufferF& generateResultViewF,
    const DataBufferF& transformResultViewF,
    const DataBufferF& reverseResultViewF,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_generateResultBufferI = generateResultViewI,
        d_transformResultBufferI = transformResultViewI,
        d_reverseResultBufferI = reverseResultViewI,
        d_generateResultBufferF = generateResultViewF,
        d_transformResultBufferF = transformResultViewF,
        d_reverseResultBufferF = reverseResultViewF
    ));
}

// -----------------------------------------------------------------------------

void KLocalArraysTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;
    using namespace vpp::ct;

    group::GroupInvocation inv ( pShader );

    //pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    UniformSimpleArray< int, decltype ( d_generateResultBufferI ) > outGenerateBufferI ( d_generateResultBufferI );
    UniformSimpleArray< int, decltype ( d_transformResultBufferI ) > outTransformBufferI ( d_transformResultBufferI );
    UniformSimpleArray< int, decltype ( d_reverseResultBufferI ) > outReverseBufferI ( d_reverseResultBufferI );
    UniformSimpleArray< float, decltype ( d_generateResultBufferF ) > outGenerateBufferF ( d_generateResultBufferF );
    UniformSimpleArray< float, decltype ( d_transformResultBufferF ) > outTransformBufferF ( d_transformResultBufferF );
    UniformSimpleArray< float, decltype ( d_reverseResultBufferF ) > outReverseBufferF ( d_reverseResultBufferF );

    VInt tmpi;

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );

    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const Int testDataOffset = static_cast< int >( offsets [ iTest ] );

        VArray< Int > testArrI ( testSize );
        VArray< Float > testArrF ( testSize );

        VInt ti;
        VFloat tf;

        If ( inv.LocalId() == 0 );

            // integer

            For ( tmpi, 0, testSize );
                testArrI [ tmpi ] = tmpi;
            Rof();

            For ( tmpi, 0, testSize );
                outGenerateBufferI [ testDataOffset + tmpi ] = testArrI [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                testArrI [ tmpi ] = 2*testArrI [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                outTransformBufferI [ testDataOffset + tmpi ] = testArrI [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                testArrI [ tmpi ] = tmpi;
            Rof();

            For ( tmpi, 0, testSize / 2 );
                ti = testArrI [ tmpi ];
                testArrI [ tmpi ] = testArrI [ testArrI.Size() - tmpi - 1 ];
                testArrI [ testSize - tmpi - 1 ] = ti;
            Rof();

            For ( tmpi, 0, testSize );
                outReverseBufferI [ testDataOffset + tmpi ] = testArrI [ tmpi ];
            Rof();

            // float

            For ( tmpi, 0, testSize );
                testArrF [ tmpi ] = StaticCast< Float >( tmpi );
            Rof();

            For ( tmpi, 0, testSize );
                outGenerateBufferF [ testDataOffset + tmpi ] = testArrF [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                testArrF [ tmpi ] = 2*testArrF [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                outTransformBufferF [ testDataOffset + tmpi ] = testArrF [ tmpi ];
            Rof();

            For ( tmpi, 0, testSize );
                testArrF [ tmpi ] = StaticCast< Float >( tmpi );
            Rof();

            For ( tmpi, 0, testSize / 2 );
                tf = testArrF [ tmpi ];
                testArrF [ tmpi ] = testArrF [ testArrF.Size() - tmpi - 1 ];
                testArrF [ testSize - tmpi - 1 ] = tf;
            Rof();

            For ( tmpi, 0, testSize );
                outReverseBufferF [ testDataOffset + tmpi ] = testArrF [ tmpi ];
            Rof();

        Fi();
    }
}

// -----------------------------------------------------------------------------

class KLocalArraysTest :
    public vpp::Computation,
    public KLocalArraysTestTypes
{
public:
    KLocalArraysTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareTestVectors();
    void compareResults();

private:
    vpp::ComputePipelineLayout< KLocalArraysTestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    const unsigned int d_testBuffersLength;

    DataBufferI d_generateResultBufferI;
    DataBufferI d_transformResultBufferI;
    DataBufferI d_reverseResultBufferI;
    DataBufferF d_generateResultBufferF;
    DataBufferF d_transformResultBufferF;
    DataBufferF d_reverseResultBufferF;
};

// -----------------------------------------------------------------------------

KLocalArraysTest :: KLocalArraysTest ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_testBuffersLength ( totalTestVectorLength() ),
    d_generateResultBufferI ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_transformResultBufferI ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_reverseResultBufferI ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_generateResultBufferF ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_transformResultBufferF ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_reverseResultBufferF ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setData (
        d_generateResultBufferI,
        d_transformResultBufferI,
        d_reverseResultBufferI,
        d_generateResultBufferF,
        d_transformResultBufferF,
        d_reverseResultBufferF,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 1, 1, 1 );

        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER,
            d_generateResultBufferI, d_transformResultBufferI, d_reverseResultBufferI,
            d_generateResultBufferF, d_transformResultBufferF, d_reverseResultBufferF
        ) );

        d_generateResultBufferI.cmdLoadAll();
        d_transformResultBufferI.cmdLoadAll();
        d_reverseResultBufferI.cmdLoadAll();
        d_generateResultBufferF.cmdLoadAll();
        d_transformResultBufferF.cmdLoadAll();
        d_reverseResultBufferF.cmdLoadAll();
    };

    prepareTestVectors();
}

// -----------------------------------------------------------------------------

void KLocalArraysTest :: prepareTestVectors()
{
    d_generateResultBufferI.resize ( d_testBuffersLength );
    d_transformResultBufferI.resize ( d_testBuffersLength );
    d_reverseResultBufferI.resize ( d_testBuffersLength );
    d_generateResultBufferF.resize ( d_testBuffersLength );
    d_transformResultBufferF.resize ( d_testBuffersLength );
    d_reverseResultBufferF.resize ( d_testBuffersLength );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const unsigned int testDataOffset = offsets [ iTest ];

        for ( unsigned int j = 0; j != testSize; ++j )
        {
            d_generateResultBufferI [ testDataOffset + j ] = 0;
            d_transformResultBufferI [ testDataOffset + j ] = 0;
            d_reverseResultBufferI [ testDataOffset + j ] = 0;
            d_generateResultBufferF [ testDataOffset + j ] = 0.0f;
            d_transformResultBufferF [ testDataOffset + j ] = 0.0f;
            d_reverseResultBufferF [ testDataOffset + j ] = 0.0f;
        }
    }

    d_generateResultBufferI.commitAndWait();
    d_transformResultBufferI.commitAndWait();
    d_reverseResultBufferI.commitAndWait();
    d_generateResultBufferF.commitAndWait();
    d_transformResultBufferF.commitAndWait();
    d_reverseResultBufferF.commitAndWait();
}

// -----------------------------------------------------------------------------

void KLocalArraysTest :: compareResults()
{
    std::vector< unsigned int > generatedValuesI ( d_generateResultBufferI.begin(), d_generateResultBufferI.end() );
    std::vector< unsigned int > transformedValuesI ( d_transformResultBufferI.begin(), d_transformResultBufferI.end() );
    std::vector< unsigned int > reversedValuesI ( d_reverseResultBufferI.begin(), d_reverseResultBufferI.end() );
    std::vector< float > generatedValuesF ( d_generateResultBufferF.begin(), d_generateResultBufferF.end() );
    std::vector< float > transformedValuesF ( d_transformResultBufferF.begin(), d_transformResultBufferF.end() );
    std::vector< float > reversedValuesF ( d_reverseResultBufferF.begin(), d_reverseResultBufferF.end() );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const unsigned int testDataOffset = offsets [ iTest ];

        // integer tests

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = generatedValuesI [ testDataOffset + i ];
            check ( v == i );
        }

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = transformedValuesI [ testDataOffset + i ];
            check ( v == 2*i );
        }

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = reversedValuesI [ testDataOffset + i ];
            check ( v == testSize - i - 1 );
        }

        // float tests

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const float v = generatedValuesF [ testDataOffset + i ];
            check ( v == float(i) );
        }

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const float v = transformedValuesF [ testDataOffset + i ];
            check ( v == 2.0f*i );
        }

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const float v = reversedValuesF [ testDataOffset + i ];
            check ( v == float ( testSize - i - 1 ) );
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Group algorithms tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KGroupAlgorithmsTestTypes
{
    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBuffer;
    typedef vpp::format< unsigned int > FormatU32;

    static void computeTestOffsets ( std::vector< unsigned int >* pOffsets );
    static unsigned int totalTestVectorLength();

    static const unsigned int s_testSizes[];
    static const unsigned int s_testCount;
};

// -----------------------------------------------------------------------------

const unsigned int KGroupAlgorithmsTestTypes :: s_testSizes[] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 17, 31, 32, 33, 63, 64, 65,
    127, 128, 129, 255, 256, 257, 511, 512, 513, 999,
    2047, 6000
};

// -----------------------------------------------------------------------------

const unsigned int KGroupAlgorithmsTestTypes :: s_testCount =
    sizeof ( s_testSizes ) / sizeof ( unsigned int );

// -----------------------------------------------------------------------------

void KGroupAlgorithmsTestTypes :: computeTestOffsets ( std::vector< unsigned int >* pOffsets )
{
    pOffsets->resize ( s_testCount + 1 );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i != s_testCount; ++i )
    {
        ( *pOffsets )[ i ] = offset;
        offset += s_testSizes [ i ];
    }

    ( *pOffsets )[ s_testCount ] = offset;
}

// -----------------------------------------------------------------------------

unsigned int KGroupAlgorithmsTestTypes :: totalTestVectorLength()
{
    return std::accumulate ( s_testSizes + 0, s_testSizes + s_testCount, 0 );
}

// -----------------------------------------------------------------------------

class KGroupAlgorithmsTestPipeline :
    public vpp::ComputePipelineConfig,
    public KGroupAlgorithmsTestTypes
{
public:
    KGroupAlgorithmsTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const DataBuffer& sourceView,
        const DataBuffer& generateResultView,
        const DataBuffer& sortResultView,
        const DataBuffer& reduceResultView,
        const DataBuffer& iscanResultView,
        const DataBuffer& escanResultView,
        const DataBuffer& lboundResultView,
        const DataBuffer& uboundResultView,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_sourceBuffer;
    vpp::ioBuffer d_generateResultBuffer;
    vpp::ioBuffer d_sortResultBuffer;
    vpp::ioBuffer d_reduceResultBuffer;
    vpp::ioBuffer d_iscanResultBuffer;
    vpp::ioBuffer d_escanResultBuffer;
    vpp::ioBuffer d_lboundResultBuffer;
    vpp::ioBuffer d_uboundResultBuffer;
    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KGroupAlgorithmsTestPipeline :: KGroupAlgorithmsTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KGroupAlgorithmsTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KGroupAlgorithmsTestPipeline :: setData (
    const DataBuffer& sourceView,
    const DataBuffer& generateResultView,
    const DataBuffer& sortResultView,
    const DataBuffer& reduceResultView,
    const DataBuffer& iscanResultView,
    const DataBuffer& escanResultView,
    const DataBuffer& lboundResultView,
    const DataBuffer& uboundResultView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_sourceBuffer = sourceView,
        d_generateResultBuffer = generateResultView,
        d_sortResultBuffer = sortResultView,
        d_reduceResultBuffer = reduceResultView,
        d_iscanResultBuffer = iscanResultView,
        d_escanResultBuffer = escanResultView,
        d_lboundResultBuffer = lboundResultView,
        d_uboundResultBuffer = uboundResultView
    ));
}

// -----------------------------------------------------------------------------

void KGroupAlgorithmsTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;
    using namespace vpp::ct;

    group::GroupInvocation inv ( pShader );

    //pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    UniformSimpleArray< unsigned int, decltype ( d_sourceBuffer ) > inSourceBuffer ( d_sourceBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_generateResultBuffer ) > outGenerateBuffer ( d_generateResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_sortResultBuffer ) > outSortBuffer ( d_sortResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_reduceResultBuffer ) > outReduceBuffer ( d_reduceResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_iscanResultBuffer ) > outIscanBuffer ( d_iscanResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_escanResultBuffer ) > outEscanBuffer ( d_escanResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_lboundResultBuffer ) > outLboundBuffer ( d_lboundResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_uboundResultBuffer ) > outUboundBuffer ( d_uboundResultBuffer );

    VUInt tmpv;
    WArray< UInt > temporaryArr ( inv.localCount() );
    WArray< Int > workArr ( 2 * inv.localCount() + 6 );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );

    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const Int testDataOffset = static_cast< int >( offsets [ iTest ] );

        WArray< UInt > testArr ( testSize );

        // Test Generate.
        group::Generate ( testArr, []( const Int& idx ){ return StaticCast< UInt >( idx + 1 ); }, inv );
        group::Store ( testArr, outGenerateBuffer, testDataOffset, inv );

        // Test Reduce and Fill
        const UInt red = group::Reduce (
            testArr, []( const UInt& v1, const UInt& v2 ){ return v1 + v2; },
            tmpv, temporaryArr, inv );

        group::Fill ( testArr, red, inv );
        group::Store ( testArr, outReduceBuffer, testDataOffset, inv );

        // Test InclusiveScan
        group::Generate ( testArr, []( const Int& idx ){ return StaticCast< UInt >( idx + 1 ); }, inv );
        
        group::InclusiveScan (
            testArr, []( const UInt& v1, const UInt& v2 ){ return v1 + v2; },
            tmpv, temporaryArr, inv );

        group::Store ( testArr, outIscanBuffer, testDataOffset, inv );

        // Test ExclusiveScan
        group::Generate ( testArr, []( const Int& idx ){ return StaticCast< UInt >( idx + 1 ); }, inv );
        
        group::ExclusiveScan (
            testArr, []( const UInt& v1, const UInt& v2 ){ return v1 + v2; },
            tmpv, temporaryArr, inv );

        group::Store ( testArr, outEscanBuffer, testDataOffset, inv );

        // Test Sort.

        group::Load ( inSourceBuffer, testDataOffset, testArr, inv );

        group::Sort (
            testArr, []( const UInt& i1, const UInt& i2 ){ return i1 < i2; },
            inv );

        group::Store ( testArr, outSortBuffer, testDataOffset, inv );

        // Test LowerBound
        group::Generate ( testArr, []( const Int& idx ){ return StaticCast< UInt >( idx ); }, inv );

        For ( tmpv, 0u, StaticCast< UInt >( testArr.Size() ) );
        {
            const Int idx = group::LowerBound (
                testArr, UInt ( tmpv ),
                []( const UInt& l, const UInt& r ){ return l < r; },
                workArr, inv );

            If ( l == 0 );
                outLboundBuffer [ StaticCast< UInt >( testDataOffset ) + tmpv ] = StaticCast< UInt >( idx );
            Fi();
        }
        Rof();

        // Test UpperBound
        group::Generate ( testArr, []( const Int& idx ){ return StaticCast< UInt >( idx ); }, inv );

        For ( tmpv, 0u, StaticCast< UInt >( testArr.Size() ) );
        {
            const Int idx = group::UpperBound (
                testArr, UInt ( tmpv ),
                []( const UInt& l, const UInt& r ){ return l < r; },
                workArr, inv );

            If ( l == 0 );
                outUboundBuffer [ StaticCast< UInt >( testDataOffset ) + tmpv ] = StaticCast< UInt >( idx );
            Fi();
        }
        Rof();
    }

    const unsigned int totalMemory = pShader->getTotalWorkgroupMemory();
    const unsigned int freeMemory = pShader->getFreeWorkgroupMemory();
}

// -----------------------------------------------------------------------------

class KGroupAlgorithmsTest :
    public vpp::Computation,
    public KGroupAlgorithmsTestTypes
{
public:
    KGroupAlgorithmsTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareTestVectors();
    void compareResults();

private:
    vpp::ComputePipelineLayout< KGroupAlgorithmsTestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    const unsigned int d_testBuffersLength;
    DataBuffer d_sortSourceBuffer;
    DataBuffer d_sortResultBuffer;
    DataBuffer d_generateResultBuffer;
    DataBuffer d_reduceResultBuffer;
    DataBuffer d_iscanResultBuffer;
    DataBuffer d_escanResultBuffer;
    DataBuffer d_lboundResultBuffer;
    DataBuffer d_uboundResultBuffer;
};

// -----------------------------------------------------------------------------

KGroupAlgorithmsTest :: KGroupAlgorithmsTest ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_testBuffersLength ( totalTestVectorLength() ),
    d_sortSourceBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sortResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_generateResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_reduceResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_iscanResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_escanResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_lboundResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_uboundResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setData (
        d_sortSourceBuffer,
        d_generateResultBuffer,
        d_sortResultBuffer,
        d_reduceResultBuffer,
        d_iscanResultBuffer,
        d_escanResultBuffer,
        d_lboundResultBuffer,
        d_uboundResultBuffer,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 1, 1, 1 );

        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER,
            d_sortResultBuffer, d_generateResultBuffer, d_reduceResultBuffer,
            d_iscanResultBuffer, d_escanResultBuffer, d_lboundResultBuffer, d_uboundResultBuffer
        ) );

        d_sortResultBuffer.cmdLoadAll();
        d_generateResultBuffer.cmdLoadAll();
        d_reduceResultBuffer.cmdLoadAll();
        d_iscanResultBuffer.cmdLoadAll();
        d_escanResultBuffer.cmdLoadAll();
        d_lboundResultBuffer.cmdLoadAll();
        d_uboundResultBuffer.cmdLoadAll();
    };

    prepareTestVectors();
}

// -----------------------------------------------------------------------------

void KGroupAlgorithmsTest :: prepareTestVectors()
{
    d_sortSourceBuffer.resize ( d_testBuffersLength );
    d_generateResultBuffer.resize ( d_testBuffersLength );
    d_sortResultBuffer.resize ( d_testBuffersLength );
    d_reduceResultBuffer.resize ( d_testBuffersLength );
    d_iscanResultBuffer.resize ( d_testBuffersLength );
    d_escanResultBuffer.resize ( d_testBuffersLength );
    d_lboundResultBuffer.resize ( d_testBuffersLength );
    d_uboundResultBuffer.resize ( d_testBuffersLength );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    unsigned int r = 17;

    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const unsigned int testDataOffset = offsets [ iTest ];

        for ( unsigned int j = 0; j != testSize; ++j )
        {
            d_sortSourceBuffer [ testDataOffset + j ] = r;
            r = 69069 * r + 1;
            d_sortResultBuffer [ testDataOffset + j ] = 0;
            d_generateResultBuffer [ testDataOffset + j ] = 0;
            d_reduceResultBuffer [ testDataOffset + j ] = 0;
            d_iscanResultBuffer [ testDataOffset + j ] = 0;
            d_escanResultBuffer [ testDataOffset + j ] = 0;
            d_lboundResultBuffer [ testDataOffset + j ] = 0;
            d_uboundResultBuffer [ testDataOffset + j ] = 0;
        }
    }

    d_sortSourceBuffer.commitAndWait();
    d_generateResultBuffer.commitAndWait();
    d_sortResultBuffer.commitAndWait();
    d_reduceResultBuffer.commitAndWait();
    d_iscanResultBuffer.commitAndWait();
    d_escanResultBuffer.commitAndWait();
    d_lboundResultBuffer.commitAndWait();
    d_uboundResultBuffer.commitAndWait();
}

// -----------------------------------------------------------------------------

void KGroupAlgorithmsTest :: compareResults()
{
    std::vector< unsigned int > sources ( d_sortSourceBuffer.begin(), d_sortSourceBuffer.end() );
    std::vector< unsigned int > generatedValues ( d_generateResultBuffer.begin(), d_generateResultBuffer.end() );
    std::vector< unsigned int > sortedValues ( d_sortResultBuffer.begin(), d_sortResultBuffer.end() );
    std::vector< unsigned int > reducedValues ( d_reduceResultBuffer.begin(), d_reduceResultBuffer.end() );
    std::vector< unsigned int > iscanValues ( d_iscanResultBuffer.begin(), d_iscanResultBuffer.end() );
    std::vector< unsigned int > escanValues ( d_escanResultBuffer.begin(), d_escanResultBuffer.end() );
    std::vector< unsigned int > lboundValues ( d_lboundResultBuffer.begin(), d_lboundResultBuffer.end() );
    std::vector< unsigned int > uboundValues ( d_uboundResultBuffer.begin(), d_uboundResultBuffer.end() );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testSize = s_testSizes [ iTest ];
        const unsigned int testDataOffset = offsets [ iTest ];

        // check Generate results

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = generatedValues [ testDataOffset + i ];
            check ( v == i + 1 );
        }

        // check Sort results
        std::sort (
            sources.begin() + testDataOffset, 
            sources.begin() + testDataOffset + testSize );

        const bool bSorted = std::equal (
            sortedValues.begin() + testDataOffset, 
            sortedValues.begin() + testDataOffset + testSize,
            sources.begin() + testDataOffset );

        check ( bSorted );

        // check Reduce result

        const unsigned int reducedValue = ( testSize * ( testSize + 1 ) ) / 2;

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = reducedValues [ testDataOffset + i ];
            check ( v == reducedValue );
        }

        // check IScan result
        unsigned int r = 0;

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = iscanValues [ testDataOffset + i ];
            r += ( i + 1 );
            check ( v == r );
        }

        // check EScan result
        r = 0;

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = escanValues [ testDataOffset + i ];
            check ( v == r );
            r += ( i + 1 );
        }

        // check LBound results

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = lboundValues [ testDataOffset + i ];
            check ( v == i );
        }

        // check UBound results

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = uboundValues [ testDataOffset + i ];
            check ( v == i + 1 );
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Group algorithms tests 2

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KGroupAlgorithms2TestTypes
{
    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBuffer;
    typedef vpp::format< unsigned int > FormatU32;

    static void computeTestOffsets ( std::vector< unsigned int >* pOffsets );
    static unsigned int totalTestVectorLength();

    static const unsigned int s_testSizesCols[];
    static const unsigned int s_testSizesRows[];
    static const unsigned int s_testCount;
};

// -----------------------------------------------------------------------------

const unsigned int KGroupAlgorithms2TestTypes :: s_testSizesCols[] =
{
    1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 16, 16, 17, 17, 31, 31, 
    32, 32, 33, 33, 41
};

// -----------------------------------------------------------------------------

const unsigned int KGroupAlgorithms2TestTypes :: s_testSizesRows[] =
{
    1, 1, 2, 3, 2, 4, 5, 5, 4, 6, 5, 7, 8, 8, 7, 9, 6, 16, 15, 17, 18, 31, 30, 
    32, 11, 33, 34, 41
};

// -----------------------------------------------------------------------------

static_assert (
    sizeof ( KGroupAlgorithms2TestTypes::s_testSizesCols ) == sizeof ( KGroupAlgorithms2TestTypes::s_testSizesRows ),
    "sizeof ( s_testSizesCols )" );

// -----------------------------------------------------------------------------

const unsigned int KGroupAlgorithms2TestTypes :: s_testCount =
    sizeof ( s_testSizesCols ) / sizeof ( unsigned int );

// -----------------------------------------------------------------------------

void KGroupAlgorithms2TestTypes :: computeTestOffsets ( std::vector< unsigned int >* pOffsets )
{
    pOffsets->resize ( s_testCount + 1 );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i != s_testCount; ++i )
    {
        ( *pOffsets )[ i ] = offset;
        offset += s_testSizesCols [ i ] * s_testSizesRows [ i ];
    }

    ( *pOffsets )[ s_testCount ] = offset;
}

// -----------------------------------------------------------------------------

unsigned int KGroupAlgorithms2TestTypes :: totalTestVectorLength()
{
    unsigned int offset = 0;

    for ( unsigned int i = 0; i != s_testCount; ++i )
        offset += s_testSizesCols [ i ] * s_testSizesRows [ i ];

    return offset;
}

// -----------------------------------------------------------------------------

class KGroupAlgorithms2TestPipeline :
    public vpp::ComputePipelineConfig,
    public KGroupAlgorithms2TestTypes
{
public:
    KGroupAlgorithms2TestPipeline ( const vpp::Device& hDevice );

    void setData (
        const DataBuffer& fillResultView,
        const DataBuffer& generateResultView,
        const DataBuffer& generate2ResultView,
        const DataBuffer& sortResultView,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_fillResultBuffer;
    vpp::ioBuffer d_generateResultBuffer;
    vpp::ioBuffer d_generate2ResultBuffer;
    vpp::ioBuffer d_sortResultBuffer;
    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KGroupAlgorithms2TestPipeline :: KGroupAlgorithms2TestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KGroupAlgorithms2TestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KGroupAlgorithms2TestPipeline :: setData (
    const DataBuffer& fillResultView,
    const DataBuffer& generateResultView,
    const DataBuffer& generate2ResultView,
    const DataBuffer& sortResultView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_fillResultBuffer = fillResultView,
        d_generateResultBuffer = generateResultView,
        d_generate2ResultBuffer = generate2ResultView,
        d_sortResultBuffer = sortResultView
    ));
}

// -----------------------------------------------------------------------------

void KGroupAlgorithms2TestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;
    using namespace vpp::ct;

    group::GroupInvocation inv ( pShader );

    //pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    UniformSimpleArray< unsigned int, decltype ( d_fillResultBuffer ) > outFillBuffer ( d_fillResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_generateResultBuffer ) > outGenerateBuffer ( d_generateResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_generate2ResultBuffer ) > outGenerate2Buffer ( d_generate2ResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_sortResultBuffer ) > outSortBuffer ( d_sortResultBuffer );

    VUInt tmpv;

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );

    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testRows = s_testSizesRows [ iTest ];
        const unsigned int testCols = s_testSizesCols [ iTest ];
        const unsigned int testSize = testRows * testCols;
        const Int testDataOffset = static_cast< int >( offsets [ iTest ] );

        WArray2< UInt > testArr ( s_testSizesRows [ iTest ], s_testSizesCols [ iTest ] );

        // Test Fill.
        group::Fill ( testArr, UInt(testSize), inv );
        group::Store ( testArr, outFillBuffer, false, testDataOffset, inv );

        // Test Generate.
        group::Generate (
            testArr, []( const Int& row, const Int& col )
            {
                return StaticCast< UInt >( ( row << 16 ) | col ); 
            }, inv
        );

        group::Store ( testArr, outGenerateBuffer, false, testDataOffset, inv );
        group::Store ( testArr, outGenerate2Buffer, true, testDataOffset, inv );

        // Test Sort
        group::Generate (
            testArr, [ testCols ]( const Int& row, const Int& col )
            {
                return StaticCast< UInt >( testCols - col ); 
            }, inv
        );

        For ( tmpv, 0, testRows );
        {
            WArray< UInt > row = testArr [ tmpv ];

            group::Sort (
                row, []( const UInt& lhs, const UInt& rhs ){ return lhs < rhs; },
                inv );
        }
        Rof();

        group::Store ( testArr, outSortBuffer, false, testDataOffset, inv );
    }
}

// -----------------------------------------------------------------------------

class KGroupAlgorithms2Test :
    public vpp::Computation,
    public KGroupAlgorithms2TestTypes
{
public:
    KGroupAlgorithms2Test ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareTestVectors();
    void compareResults();

private:
    vpp::ComputePipelineLayout< KGroupAlgorithms2TestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    const unsigned int d_testBuffersLength;
    DataBuffer d_fillResultBuffer;
    DataBuffer d_generateResultBuffer;
    DataBuffer d_generate2ResultBuffer;
    DataBuffer d_sortResultBuffer;
};

// -----------------------------------------------------------------------------

KGroupAlgorithms2Test :: KGroupAlgorithms2Test ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_testBuffersLength ( totalTestVectorLength() ),
    d_fillResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_generateResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_generate2ResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_sortResultBuffer ( d_testBuffersLength, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setData (
        d_fillResultBuffer,
        d_generateResultBuffer,
        d_generate2ResultBuffer,
        d_sortResultBuffer,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 1, 1, 1 );

        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER,
            d_fillResultBuffer, d_generateResultBuffer, d_generate2ResultBuffer
        ) );

        d_fillResultBuffer.cmdLoadAll();
        d_generateResultBuffer.cmdLoadAll();
        d_generate2ResultBuffer.cmdLoadAll();
        d_sortResultBuffer.cmdLoadAll();
    };

    prepareTestVectors();
}

// -----------------------------------------------------------------------------

void KGroupAlgorithms2Test :: prepareTestVectors()
{
    d_fillResultBuffer.resize ( d_testBuffersLength );
    d_generateResultBuffer.resize ( d_testBuffersLength );
    d_generate2ResultBuffer.resize ( d_testBuffersLength );
    d_sortResultBuffer.resize ( d_testBuffersLength );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testRows = s_testSizesRows [ iTest ];
        const unsigned int testCols = s_testSizesCols [ iTest ];
        const unsigned int testSize = testRows * testCols;
        const unsigned int testDataOffset = offsets [ iTest ];

        for ( unsigned int j = 0; j != testSize; ++j )
        {
            d_fillResultBuffer [ testDataOffset + j ] = 0;
            d_generateResultBuffer [ testDataOffset + j ] = 0;
            d_generate2ResultBuffer [ testDataOffset + j ] = 0;
            d_sortResultBuffer [ testDataOffset + j ] = 0;
        }
    }

    d_fillResultBuffer.commitAndWait();
    d_generateResultBuffer.commitAndWait();
    d_generate2ResultBuffer.commitAndWait();
    d_sortResultBuffer.commitAndWait();
}

// -----------------------------------------------------------------------------

void KGroupAlgorithms2Test :: compareResults()
{
    std::vector< unsigned int > filledValues ( d_fillResultBuffer.begin(), d_fillResultBuffer.end() );
    std::vector< unsigned int > generatedValues ( d_generateResultBuffer.begin(), d_generateResultBuffer.end() );
    std::vector< unsigned int > generated2Values ( d_generate2ResultBuffer.begin(), d_generate2ResultBuffer.end() );
    std::vector< unsigned int > sortedValues ( d_sortResultBuffer.begin(), d_sortResultBuffer.end() );

    std::vector< unsigned int > offsets;
    computeTestOffsets ( & offsets );
    
    for ( unsigned int iTest = 0; iTest != s_testCount; ++iTest )
    {
        const unsigned int testRows = s_testSizesRows [ iTest ];
        const unsigned int testCols = s_testSizesCols [ iTest ];
        const unsigned int testSize = testRows * testCols;
        const unsigned int testDataOffset = offsets [ iTest ];

        // check Fill results

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = filledValues [ testDataOffset + i ];
            check ( v == testSize );
        }

        // check Generate results

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = generatedValues [ testDataOffset + i ];
            const unsigned int row = i / testCols;
            const unsigned int col = i % testCols;
            const unsigned int expected = ( row << 16 ) | col;
            check ( v == expected );
        }

        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = generated2Values [ testDataOffset + i ];
            const unsigned int row = i % testRows;
            const unsigned int col = i / testRows;
            const unsigned int expected = ( row << 16 ) | col;
            check ( v == expected );
        }

        // check Sort results
        for ( unsigned int i = 0; i != testSize; ++i )
        {
            const unsigned int v = sortedValues [ testDataOffset + i ];
            const unsigned int row = i / testCols;
            const unsigned int col = i % testCols;
            const unsigned int expected = col + 1;
            check ( v == expected );
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                Group variables tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KGroupVariablesTestTypes
{
    typedef vpp::gvector< int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferI;
    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferU;
    typedef vpp::gvector< float, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferF;
    typedef vpp::gvector< double, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBufferD;
};

// -----------------------------------------------------------------------------

class KGroupVariablesTestPipeline :
    public vpp::ComputePipelineConfig,
    public KGroupVariablesTestTypes
{
public:
    KGroupVariablesTestPipeline ( const vpp::Device& hDevice );

    void setData (
        const DataBufferI& intResultView,
        const DataBufferU& uintResultView,
        const DataBufferF& floatResultView,
        const DataBufferD& doubleResultView,
        const DataBufferI& groupResultView,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

private:
    void setVariable ( vpp::WBool& var, int value );

    template< typename ScalarT >
    void setVariable ( vpp::TLValue< ScalarT, spv::StorageClassWorkgroup >& var, int value );

    template< size_t SIZE >
    void setVariableB ( vpp::TLVector< vpp::Bool, SIZE, spv::StorageClassWorkgroup >& var, int value );

    template< typename BaseT >
    void setVariable ( vpp::TLVector< BaseT, 2, spv::StorageClassWorkgroup >& var, int value );

    template< typename BaseT >
    void setVariable ( vpp::TLVector< BaseT, 3, spv::StorageClassWorkgroup >& var, int value );

    template< typename BaseT >
    void setVariable ( vpp::TLVector< BaseT, 4, spv::StorageClassWorkgroup >& var, int value );

    template< typename BaseT, size_t COLS, size_t ROWS  >
    void setVariable ( const vpp::TLMatrix< BaseT, COLS, ROWS, spv::StorageClassWorkgroup >& var, int value );

    void storeVariable ( const vpp::Bool& v );
    void storeVariable ( const vpp::Int& v );
    void storeVariable ( const vpp::UInt& v );
    void storeVariable ( const vpp::Float& v );
    void storeVariable ( const vpp::Double& v );

    template< typename BaseT, size_t SIZE >
    void storeVariable ( const vpp::TLVector< BaseT, SIZE, spv::StorageClassWorkgroup >& var );

    template< typename BaseT, size_t COLS, size_t ROWS >
    void storeVariable ( const vpp::TLMatrix< BaseT, COLS, ROWS, spv::StorageClassWorkgroup >& var );

public:
    vpp::ioBuffer d_intResultBuffer;
    vpp::ioBuffer d_uintResultBuffer;
    vpp::ioBuffer d_floatResultBuffer;
    vpp::ioBuffer d_doubleResultBuffer;
    vpp::ioBuffer d_groupResultBuffer;
    vpp::computeShader d_shader;

    unsigned int d_intResultBufferOffset;
    unsigned int d_uintResultBufferOffset;
    unsigned int d_floatResultBufferOffset;
    unsigned int d_doubleResultBufferOffset;

    typedef vpp::UniformSimpleArray< int, decltype ( d_intResultBuffer ) > OutIntBuffer;
    typedef vpp::UniformSimpleArray< unsigned int, decltype ( d_uintResultBuffer ) > OutUIntBuffer;
    typedef vpp::UniformSimpleArray< float, decltype ( d_floatResultBuffer ) > OutFloatBuffer;
    typedef vpp::UniformSimpleArray< double, decltype ( d_doubleResultBuffer ) > OutDoubleBuffer;

    OutIntBuffer* d_pOutIntBuffer;
    OutUIntBuffer* d_pOutUIntBuffer;
    OutFloatBuffer* d_pOutFloatBuffer;
    OutDoubleBuffer* d_pOutDoubleBuffer;
};

// -----------------------------------------------------------------------------

KGroupVariablesTestPipeline :: KGroupVariablesTestPipeline ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KGroupVariablesTestPipeline::fComputeShader ),
    d_intResultBufferOffset ( 0 ),
    d_uintResultBufferOffset ( 0 ),
    d_floatResultBufferOffset ( 0 ),
    d_doubleResultBufferOffset ( 0 ),
    d_pOutIntBuffer ( 0 ),
    d_pOutUIntBuffer ( 0 ),
    d_pOutFloatBuffer ( 0 ),
    d_pOutDoubleBuffer ( 0 )
{
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: setData (
    const DataBufferI& intResultView,
    const DataBufferU& uintResultView,
    const DataBufferF& floatResultView,
    const DataBufferD& doubleResultView,
    const DataBufferI& groupResultView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_intResultBuffer = intResultView,
        d_uintResultBuffer = uintResultView,
        d_floatResultBuffer = floatResultView,
        d_doubleResultBuffer = doubleResultView,
        d_groupResultBuffer = groupResultView
    ));
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: setVariable (
    vpp::WBool& var, int value )
{
    var = ( value != 0 );
}

// -----------------------------------------------------------------------------

template< typename ScalarT >
void KGroupVariablesTestPipeline :: setVariable (
    vpp::TLValue< ScalarT, spv::StorageClassWorkgroup >& var, int value )
{
    var = static_cast< ScalarT >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
void KGroupVariablesTestPipeline :: setVariableB (
    vpp::TLVector< vpp::Bool, SIZE, spv::StorageClassWorkgroup >& var, int value )
{
    using namespace vpp;

    for ( size_t i = 0; i != SIZE; ++i )
    {
        const UInt idx = static_cast< unsigned int >( i );
        var [ idx ] = ( value != 0 );
    }
}

// -----------------------------------------------------------------------------

template< typename BaseT >
void KGroupVariablesTestPipeline :: setVariable (
    vpp::TLVector< BaseT, 2, spv::StorageClassWorkgroup >& var, int value )
{
    using namespace vpp;
    typedef typename BaseT::scalar_type ScalarT;
    var [ X ] = static_cast< ScalarT >( value );
    var [ Y ] = static_cast< ScalarT >( value );
}

// -----------------------------------------------------------------------------

template< typename BaseT >
void KGroupVariablesTestPipeline :: setVariable (
    vpp::TLVector< BaseT, 3, spv::StorageClassWorkgroup >& var, int value )
{
    using namespace vpp;
    typedef typename BaseT::scalar_type ScalarT;
    var [ X ] = static_cast< ScalarT >( value );
    var [ Y ] = static_cast< ScalarT >( value );
    var [ Z ] = static_cast< ScalarT >( value );
}

// -----------------------------------------------------------------------------

template< typename BaseT >
void KGroupVariablesTestPipeline :: setVariable (
    vpp::TLVector< BaseT, 4, spv::StorageClassWorkgroup >& var, int value )
{
    using namespace vpp;
    typedef typename BaseT::scalar_type ScalarT;
    var [ X ] = static_cast< ScalarT >( value );
    var [ Y ] = static_cast< ScalarT >( value );
    var [ Z ] = static_cast< ScalarT >( value );
    var [ W ] = static_cast< ScalarT >( value );
}

// -----------------------------------------------------------------------------

template< typename BaseT, size_t COLS, size_t ROWS  >
void KGroupVariablesTestPipeline :: setVariable (
    const vpp::TLMatrix< BaseT, COLS, ROWS, spv::StorageClassWorkgroup >& var, int value )
{
    using namespace vpp;
    typedef typename BaseT::scalar_type ScalarT;

    for ( size_t r = 0; r != ROWS; ++r )
        for ( size_t c = 0; c != COLS; ++c )
    {
        const UInt irow = static_cast< unsigned int >( r );
        const UInt icol = static_cast< unsigned int >( c );
        var [ icol ][ irow ] = static_cast< ScalarT >( value );
    }
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: storeVariable ( const vpp::Bool& v )
{
    ( *d_pOutIntBuffer ) [ d_intResultBufferOffset++ ] = Select ( v, 1, 0 );
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: storeVariable ( const vpp::Int& v )
{
    ( *d_pOutIntBuffer ) [ d_intResultBufferOffset++ ] = v;
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: storeVariable ( const vpp::UInt& v )
{
    ( *d_pOutUIntBuffer ) [ d_uintResultBufferOffset++ ] = v;
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: storeVariable ( const vpp::Float& v )
{
    ( *d_pOutFloatBuffer ) [ d_floatResultBufferOffset++ ] = v;
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: storeVariable ( const vpp::Double& v )
{
    ( *d_pOutDoubleBuffer ) [ d_doubleResultBufferOffset++ ] = v;
}

// -----------------------------------------------------------------------------

template< typename BaseT, size_t SIZE >
void KGroupVariablesTestPipeline :: storeVariable (
    const vpp::TLVector< BaseT, SIZE, spv::StorageClassWorkgroup >& var )
{
    for ( unsigned int i = 0; i != SIZE; ++i )
    {
        const BaseT scalarValue = var [ i ];
        storeVariable ( scalarValue );
    }
}

// -----------------------------------------------------------------------------

template< typename BaseT, size_t COLS, size_t ROWS  >
void KGroupVariablesTestPipeline :: storeVariable (
    const vpp::TLMatrix< BaseT, COLS, ROWS, spv::StorageClassWorkgroup >& var )
{
    for ( unsigned int r = 0; r != ROWS; ++r )
        for ( unsigned int c = 0; c != COLS; ++c )
    {
        const BaseT scalarValue = var [ c ][ r ];
        storeVariable ( scalarValue );
    }
}

// -----------------------------------------------------------------------------

void KGroupVariablesTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;
    using namespace vpp::ct;

    group::GroupInvocation inv ( pShader );

    //pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    UniformSimpleArray< int, decltype ( d_intResultBuffer ) > outIntBuffer ( d_intResultBuffer );
    UniformSimpleArray< unsigned int, decltype ( d_uintResultBuffer ) > outUIntBuffer ( d_uintResultBuffer );
    UniformSimpleArray< float, decltype ( d_floatResultBuffer ) > outFloatBuffer ( d_floatResultBuffer );
    UniformSimpleArray< double, decltype ( d_doubleResultBuffer ) > outDoubleBuffer ( d_doubleResultBuffer );
    UniformSimpleArray< int, decltype ( d_groupResultBuffer ) > outGroupBuffer ( d_groupResultBuffer );

    d_pOutIntBuffer = & outIntBuffer;
    d_pOutUIntBuffer = & outUIntBuffer;
    d_pOutFloatBuffer = & outFloatBuffer;
    d_pOutDoubleBuffer = & outDoubleBuffer;

    WInt wint;
    WUInt wuint;
    WFloat wfloat;
    WDouble wdouble;
    WBool wbool;
    WVec2 wvec2;
    WVec3 wvec3;
    WVec4 wvec4;
    WDVec2 wdvec2;
    WDVec3 wdvec3;
    WDVec4 wdvec4;
    WIVec2 wivec2;
    WIVec3 wivec3;
    WIVec4 wivec4;
    WUVec2 wuvec2;
    WUVec3 wuvec3;
    WUVec4 wuvec4;
    WBVec2 wbvec2;
    WBVec3 wbvec3;
    WBVec4 wbvec4;
    WMat2 wmat2;
    WMat3 wmat3;
    WMat4 wmat4;
    WMat2x3 wmat2x3;
    WMat2x4 wmat2x4;
    WMat3x2 wmat3x2;
    WMat3x4 wmat3x4;
    WMat4x2 wmat4x2;
    WMat4x3 wmat4x3;
    WDMat2 wdmat2;
    WDMat3 wdmat3;
    WDMat4 wdmat4;
    WDMat2x3 wdmat2x3;
    WDMat2x4 wdmat2x4;
    WDMat3x2 wdmat3x2;
    WDMat3x4 wdmat3x4;
    WDMat4x2 wdmat4x2;
    WDMat4x3 wdmat4x3;

    WArray< Vec2 > wav2 ( 2 );
    WArray2< Mat2 > wa2m2 ( 2, 2 );
    WArray3< Int > wa3i ( 2, 2, 2 );

    If ( inv.LocalId() == 0 );
        setVariable ( wint, 0 );
        setVariable ( wuint, 0 );
        setVariable ( wfloat, 0 );
        setVariable ( wdouble, 0 );
        setVariable ( wbool, 0 );
        setVariable ( wvec2, 0 );
        setVariable ( wvec3, 0 );
        setVariable ( wvec4, 0 );
        setVariable ( wdvec2, 0 );
        setVariable ( wdvec3, 0 );
        setVariable ( wdvec4, 0 );
        setVariable ( wivec2, 0 );
        setVariable ( wivec3, 0 );
        setVariable ( wivec4, 0 );
        setVariable ( wuvec2, 0 );
        setVariable ( wuvec3, 0 );
        setVariable ( wuvec4, 0 );
        setVariableB ( wbvec2, 0 );
        setVariableB ( wbvec3, 0 );
        setVariableB ( wbvec4, 0 );
        setVariable ( wmat2, 0 );
        setVariable ( wmat3, 0 );
        setVariable ( wmat4, 0 );
        setVariable ( wmat2x3, 0 );
        setVariable ( wmat2x4, 0 );
        setVariable ( wmat3x2, 0 );
        setVariable ( wmat3x4, 0 );
        setVariable ( wmat4x2, 0 );
        setVariable ( wmat4x3, 0 );
        setVariable ( wdmat2, 0 );
        setVariable ( wdmat3, 0 );
        setVariable ( wdmat4, 0 );
        setVariable ( wdmat2x3, 0 );
        setVariable ( wdmat2x4, 0 );
        setVariable ( wdmat3x2, 0 );
        setVariable ( wdmat3x4, 0 );
        setVariable ( wdmat4x2, 0 );
        setVariable ( wdmat4x3, 0 );

        wav2 [ 0 ][ 0 ] = 0.0f;
        wav2 [ 0 ][ 1 ] = 0.0f;
        wav2 [ 1 ][ 0 ] = 0.0f;
        wav2 [ 1 ][ 1 ] = 0.0f;

        wa2m2 [ 0 ][ 0 ][ 0 ][ 0 ] = 0.0f;
        wa2m2 [ 0 ][ 0 ][ 0 ][ 1 ] = 0.0f;
        wa2m2 [ 0 ][ 0 ][ 1 ][ 0 ] = 0.0f;
        wa2m2 [ 0 ][ 0 ][ 1 ][ 1 ] = 0.0f;
        wa2m2 [ 0 ][ 1 ][ 0 ][ 0 ] = 0.0f;
        wa2m2 [ 0 ][ 1 ][ 0 ][ 1 ] = 0.0f;
        wa2m2 [ 0 ][ 1 ][ 1 ][ 0 ] = 0.0f;
        wa2m2 [ 0 ][ 1 ][ 1 ][ 1 ] = 0.0f;
        wa2m2 [ 1 ][ 0 ][ 0 ][ 0 ] = 0.0f;
        wa2m2 [ 1 ][ 0 ][ 0 ][ 1 ] = 0.0f;
        wa2m2 [ 1 ][ 0 ][ 1 ][ 0 ] = 0.0f;
        wa2m2 [ 1 ][ 0 ][ 1 ][ 1 ] = 0.0f;
        wa2m2 [ 1 ][ 1 ][ 0 ][ 0 ] = 0.0f;
        wa2m2 [ 1 ][ 1 ][ 0 ][ 1 ] = 0.0f;
        wa2m2 [ 1 ][ 1 ][ 1 ][ 0 ] = 0.0f;
        wa2m2 [ 1 ][ 1 ][ 1 ][ 1 ] = 0.0f;

        wa3i [ 0 ][ 0 ][ 0 ] = 0;
        wa3i [ 0 ][ 0 ][ 1 ] = 0;
        wa3i [ 0 ][ 1 ][ 0 ] = 0;
        wa3i [ 0 ][ 1 ][ 1 ] = 0;
        wa3i [ 1 ][ 0 ][ 0 ] = 0;
        wa3i [ 1 ][ 0 ][ 1 ] = 0;
        wa3i [ 1 ][ 1 ][ 0 ] = 0;
        wa3i [ 1 ][ 1 ][ 1 ] = 0;

    Fi();

    WorkgroupBarrier();

    If ( inv.LocalId() == 1 );
        setVariable ( wint, 1 );
        setVariable ( wuint, 1 );
        setVariable ( wfloat, 1 );
        setVariable ( wdouble, 1 );
        setVariable ( wbool, 1 );
        setVariable ( wvec2, 1 );
        setVariable ( wvec3, 1 );
        setVariable ( wvec4, 1 );
        setVariable ( wdvec2, 1 );
        setVariable ( wdvec3, 1 );
        setVariable ( wdvec4, 1 );
        setVariable ( wivec2, 1 );
        setVariable ( wivec3, 1 );
        setVariable ( wivec4, 1 );
        setVariable ( wuvec2, 1 );
        setVariable ( wuvec3, 1 );
        setVariable ( wuvec4, 1 );
        setVariableB ( wbvec2, 1 );
        setVariableB ( wbvec3, 1 );
        setVariableB ( wbvec4, 1 );
        setVariable ( wmat2, 1 );
        setVariable ( wmat3, 1 );
        setVariable ( wmat4, 1 );
        setVariable ( wmat2x3, 1 );
        setVariable ( wmat2x4, 1 );
        setVariable ( wmat3x2, 1 );
        setVariable ( wmat3x4, 1 );
        setVariable ( wmat4x2, 1 );
        setVariable ( wmat4x3, 1 );
        setVariable ( wdmat2, 1 );
        setVariable ( wdmat3, 1 );
        setVariable ( wdmat4, 1 );
        setVariable ( wdmat2x3, 1 );
        setVariable ( wdmat2x4, 1 );
        setVariable ( wdmat3x2, 1 );
        setVariable ( wdmat3x4, 1 );
        setVariable ( wdmat4x2, 1 );
        setVariable ( wdmat4x3, 1 );

        wav2 [ 0 ][ 0 ] = 1.0f;
        wav2 [ 0 ][ 1 ] = 1.0f;
        wav2 [ 1 ][ 0 ] = 1.0f;
        wav2 [ 1 ][ 1 ] = 1.0f;

        wa2m2 [ 0 ][ 0 ][ 0 ][ 0 ] = 1.0f;
        wa2m2 [ 0 ][ 0 ][ 0 ][ 1 ] = 1.0f;
        wa2m2 [ 0 ][ 0 ][ 1 ][ 0 ] = 1.0f;
        wa2m2 [ 0 ][ 0 ][ 1 ][ 1 ] = 1.0f;
        wa2m2 [ 0 ][ 1 ][ 0 ][ 0 ] = 1.0f;
        wa2m2 [ 0 ][ 1 ][ 0 ][ 1 ] = 1.0f;
        wa2m2 [ 0 ][ 1 ][ 1 ][ 0 ] = 1.0f;
        wa2m2 [ 0 ][ 1 ][ 1 ][ 1 ] = 1.0f;
        wa2m2 [ 1 ][ 0 ][ 0 ][ 0 ] = 1.0f;
        wa2m2 [ 1 ][ 0 ][ 0 ][ 1 ] = 1.0f;
        wa2m2 [ 1 ][ 0 ][ 1 ][ 0 ] = 1.0f;
        wa2m2 [ 1 ][ 0 ][ 1 ][ 1 ] = 1.0f;
        wa2m2 [ 1 ][ 1 ][ 0 ][ 0 ] = 1.0f;
        wa2m2 [ 1 ][ 1 ][ 0 ][ 1 ] = 1.0f;
        wa2m2 [ 1 ][ 1 ][ 1 ][ 0 ] = 1.0f;
        wa2m2 [ 1 ][ 1 ][ 1 ][ 1 ] = 1.0f;

        wa3i [ 0 ][ 0 ][ 0 ] = 1;
        wa3i [ 0 ][ 0 ][ 1 ] = 1;
        wa3i [ 0 ][ 1 ][ 0 ] = 1;
        wa3i [ 0 ][ 1 ][ 1 ] = 1;
        wa3i [ 1 ][ 0 ][ 0 ] = 1;
        wa3i [ 1 ][ 0 ][ 1 ] = 1;
        wa3i [ 1 ][ 1 ][ 0 ] = 1;
        wa3i [ 1 ][ 1 ][ 1 ] = 1;
    Fi();

    WorkgroupBarrier();

    If ( inv.LocalId() == 0 );
        storeVariable ( wint );
        storeVariable ( wuint );
        storeVariable ( wfloat );
        storeVariable ( wdouble );
        storeVariable ( wbool );
        storeVariable ( wvec2 );
        storeVariable ( wvec3 );
        storeVariable ( wvec4 );
        storeVariable ( wdvec2 );
        storeVariable ( wdvec3 );
        storeVariable ( wdvec4 );
        storeVariable ( wivec2 );
        storeVariable ( wivec3 );
        storeVariable ( wivec4 );
        storeVariable ( wuvec2 );
        storeVariable ( wuvec3 );
        storeVariable ( wuvec4 );
        storeVariable ( wbvec2 );
        storeVariable ( wbvec3 );
        storeVariable ( wbvec4 );
        storeVariable ( wmat2 );
        storeVariable ( wmat3 );
        storeVariable ( wmat4 );
        storeVariable ( wmat2x3 );
        storeVariable ( wmat2x4 );
        storeVariable ( wmat3x2 );
        storeVariable ( wmat3x4 );
        storeVariable ( wmat4x2 );
        storeVariable ( wmat4x3 );
        storeVariable ( wdmat2 );
        storeVariable ( wdmat3 );
        storeVariable ( wdmat4 );
        storeVariable ( wdmat2x3 );
        storeVariable ( wdmat2x4 );
        storeVariable ( wdmat3x2 );
        storeVariable ( wdmat3x4 );
        storeVariable ( wdmat4x2 );
        storeVariable ( wdmat4x3 );

        storeVariable ( wav2 [ 0 ][ 0 ] );
        storeVariable ( wav2 [ 0 ][ 1 ] );
        storeVariable ( wav2 [ 1 ][ 0 ] );
        storeVariable ( wav2 [ 1 ][ 1 ] );

        storeVariable ( wa2m2 [ 0 ][ 0 ][ 0 ][ 0 ] );
        storeVariable ( wa2m2 [ 0 ][ 0 ][ 0 ][ 1 ] );
        storeVariable ( wa2m2 [ 0 ][ 0 ][ 1 ][ 0 ] );
        storeVariable ( wa2m2 [ 0 ][ 0 ][ 1 ][ 1 ] );
        storeVariable ( wa2m2 [ 0 ][ 1 ][ 0 ][ 0 ] );
        storeVariable ( wa2m2 [ 0 ][ 1 ][ 0 ][ 1 ] );
        storeVariable ( wa2m2 [ 0 ][ 1 ][ 1 ][ 0 ] );
        storeVariable ( wa2m2 [ 0 ][ 1 ][ 1 ][ 1 ] );
        storeVariable ( wa2m2 [ 1 ][ 0 ][ 0 ][ 0 ] );
        storeVariable ( wa2m2 [ 1 ][ 0 ][ 0 ][ 1 ] );
        storeVariable ( wa2m2 [ 1 ][ 0 ][ 1 ][ 0 ] );
        storeVariable ( wa2m2 [ 1 ][ 0 ][ 1 ][ 1 ] );
        storeVariable ( wa2m2 [ 1 ][ 1 ][ 0 ][ 0 ] );
        storeVariable ( wa2m2 [ 1 ][ 1 ][ 0 ][ 1 ] );
        storeVariable ( wa2m2 [ 1 ][ 1 ][ 1 ][ 0 ] );
        storeVariable ( wa2m2 [ 1 ][ 1 ][ 1 ][ 1 ] );

        storeVariable ( wa3i [ 0 ][ 0 ][ 0 ] );
        storeVariable ( wa3i [ 0 ][ 0 ][ 1 ] );
        storeVariable ( wa3i [ 0 ][ 1 ][ 0 ] );
        storeVariable ( wa3i [ 0 ][ 1 ][ 1 ] );
        storeVariable ( wa3i [ 1 ][ 0 ][ 0 ] );
        storeVariable ( wa3i [ 1 ][ 0 ][ 1 ] );
        storeVariable ( wa3i [ 1 ][ 1 ][ 0 ] );
        storeVariable ( wa3i [ 1 ][ 1 ][ 1 ] );

    Fi();

    WArray2< Int > wa2i ( 6, 10 );
    WArray< Int > tmp ( 2 * inv.localCount() + 2 );
    
    group::Fill ( wa2i [ 0 ], Int ( 17 ), inv );
    group::Copy ( wa2i [ 0 ], wa2i [ 1 ], inv );
    group::Generate ( wa2i [ 2 ], []( const Int& idx ){ return 100-idx; }, inv );
    group::Transform ( wa2i [ 2 ], wa2i [ 3 ], []( const Int& v ){ return 2*v; }, inv );
    group::Sort ( wa2i [ 3 ], []( const Int& v1, const Int& v2 ){ return v1 < v2; }, inv );

    const Int lb = group::LowerBound (
        wa2i [ 3 ], 184,
        []( const Int& v1, const Int& v2 ){ return v1 < v2; },
        tmp, inv );

    const Int ub = group::UpperBound (
        wa2i [ 3 ], 184,
        []( const Int& v1, const Int& v2 ){ return v1 < v2; },
        tmp, inv );

    group::Fill ( wa2i [ 4 ], lb, inv );
    group::Fill ( wa2i [ 5 ], ub, inv );

    group::Store ( wa2i, outGroupBuffer, 0, inv );
}

// -----------------------------------------------------------------------------

class KGroupVariablesTest :
    public vpp::Computation,
    public KGroupVariablesTestTypes
{
public:
    KGroupVariablesTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void prepareTestVectors();
    void compareResults();

private:
    vpp::ComputePipelineLayout< KGroupVariablesTestPipeline > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    static const unsigned int s_bufferSize = 1024;

    DataBufferI d_intResultBuffer;
    DataBufferU d_uintResultBuffer;
    DataBufferF d_floatResultBuffer;
    DataBufferD d_doubleResultBuffer;
    DataBufferI d_groupResultBuffer;
};

// -----------------------------------------------------------------------------

KGroupVariablesTest :: KGroupVariablesTest ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_intResultBuffer ( s_bufferSize, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_uintResultBuffer ( s_bufferSize, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_floatResultBuffer ( s_bufferSize, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_doubleResultBuffer ( s_bufferSize, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_groupResultBuffer ( s_bufferSize, vpp::MemProfile::DEVICE_STATIC, hDevice )
{
    using namespace vpp;

    d_pipeline.definition().setData (
        d_intResultBuffer,
        d_uintResultBuffer,
        d_floatResultBuffer,
        d_doubleResultBuffer,
        d_groupResultBuffer,
        & d_dataBlock );

    addPipeline ( d_pipeline );

    ( *this ) << [ this ]()
    {
        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 1, 1, 1 );

        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER,
            d_intResultBuffer, d_uintResultBuffer, d_floatResultBuffer, d_doubleResultBuffer,
            d_groupResultBuffer
        ) );

        d_intResultBuffer.cmdLoadAll();
        d_uintResultBuffer.cmdLoadAll();
        d_floatResultBuffer.cmdLoadAll();
        d_doubleResultBuffer.cmdLoadAll();
        d_groupResultBuffer.cmdLoadAll();
    };

    prepareTestVectors();
}

// -----------------------------------------------------------------------------

void KGroupVariablesTest :: prepareTestVectors()
{
    d_intResultBuffer.resize ( s_bufferSize );
    d_uintResultBuffer.resize ( s_bufferSize );
    d_floatResultBuffer.resize ( s_bufferSize );
    d_doubleResultBuffer.resize ( s_bufferSize );
    d_groupResultBuffer.resize ( s_bufferSize );

    for ( unsigned int j = 0; j != s_bufferSize; ++j )
    {
        d_intResultBuffer [ j ] = 0;
        d_uintResultBuffer [ j ] = 0;
        d_floatResultBuffer [ j ] = 0;
        d_doubleResultBuffer [ j ] = 0;
        d_groupResultBuffer [ j ] = 0;
    }

    d_intResultBuffer.commitAndWait();
    d_uintResultBuffer.commitAndWait();
    d_floatResultBuffer.commitAndWait();
    d_doubleResultBuffer.commitAndWait();
    d_groupResultBuffer.commitAndWait();
}

// -----------------------------------------------------------------------------

void KGroupVariablesTest :: compareResults()
{
    std::vector< int > intValues ( d_intResultBuffer.begin(), d_intResultBuffer.end() );
    std::vector< unsigned int > uintValues ( d_uintResultBuffer.begin(), d_uintResultBuffer.end() );
    std::vector< float > floatValues ( d_floatResultBuffer.begin(), d_floatResultBuffer.end() );
    std::vector< double > doubleValues ( d_doubleResultBuffer.begin(), d_doubleResultBuffer.end() );
    std::vector< int > groupValues ( d_groupResultBuffer.begin(), d_groupResultBuffer.end() );

    const unsigned int intSize = d_pipeline.definition().d_intResultBufferOffset;
    const unsigned int uintSize = d_pipeline.definition().d_uintResultBufferOffset;
    const unsigned int floatSize = d_pipeline.definition().d_floatResultBufferOffset;
    const unsigned int doubleSize = d_pipeline.definition().d_doubleResultBufferOffset;

    for ( unsigned int i = 0; i != intSize; ++i )
    {
        const int v = intValues [ i ];
        check ( v == 1 );
    }

    for ( unsigned int i = 0; i != uintSize; ++i )
    {
        const unsigned int v = uintValues [ i ];
        check ( v == 1u );
    }

    for ( unsigned int i = 0; i != floatSize; ++i )
    {
        const float v = floatValues [ i ];
        check ( v == 1.0f );
    }

    for ( unsigned int i = 0; i != doubleSize; ++i )
    {
        const double v = doubleValues [ i ];
        check ( v == 1.0 );
    }

    for ( unsigned int i = 0; i != 20; ++i )
    {
        const double v = groupValues [ i ];
        check ( v == 17 );
    }

    for ( unsigned int i = 0; i != 10; ++i )
    {
        const double v = groupValues [ 20 + i ];
        check ( v == (100-i) );
    }

    for ( unsigned int i = 0; i != 10; ++i )
    {
        const double v = groupValues [ 30 + i ];
        check ( v == 182 + 2*i );
    }

    for ( unsigned int i = 0; i != 10; ++i )
    {
        const double v = groupValues [ 40 + i ];
        check ( v == 1 );
    }

    for ( unsigned int i = 0; i != 10; ++i )
    {
        const double v = groupValues [ 50 + i ];
        check ( v == 2 );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

//                                 Atomics tests

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KAtomicsTestTypes
{
    static const int TEST_COUNT = 7;
    static const int BLOCK_SIZE = 32768;

    typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBuffer;
    typedef vpp::gvector< std::uint64_t, vpp::Buf::STORAGE | vpp::Buf::TARGET | vpp::Buf::SOURCE > DataBuffer64;

    typedef vpp::format< unsigned int > FormatU32;
    typedef vpp::format< std::uint64_t > FormatU64;

    typedef vpp::ImageAttributes<
        FormatU32, vpp::RENDER, vpp::IMG_TYPE_2D,
        vpp::Img::STORAGE | vpp::Img::TARGET,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > TestImageAttr;

    typedef vpp::Image< TestImageAttr > TestImage;
    typedef vpp::ImageViewAttributes< TestImage > TestImageViewAttr;
    typedef vpp::ImageView< TestImageViewAttr > TestImageView;
};

// -----------------------------------------------------------------------------

class KAtomicsTestPipeline1 :
    public vpp::ComputePipelineConfig,
    public KAtomicsTestTypes
{
public:
    KAtomicsTestPipeline1 ( const vpp::Device& hDevice );

    void setData (
        const DataBuffer& resultView,
        const DataBuffer& testView,
        const DataBuffer64& testView64,
        const TestImageView& testImageView,
        vpp::ShaderDataBlock* pDataBlock );

    void fComputeShader ( vpp::ComputeShader* pShader );

public:
    vpp::ioBuffer d_resultBuffer;
    vpp::ioBuffer d_testBuffer;
    vpp::ioBuffer d_testBuffer64;
    vpp::ioImage< TestImageView > d_testImage;
    vpp::computeShader d_shader;
};

// -----------------------------------------------------------------------------

KAtomicsTestPipeline1 :: KAtomicsTestPipeline1 ( const vpp::Device& hDevice ) :
    d_shader ( this, { 32, 1, 1 }, & KAtomicsTestPipeline1::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KAtomicsTestPipeline1 :: setData (
    const DataBuffer& resultView,
    const DataBuffer& testView,
    const DataBuffer64& testView64,
    const TestImageView& testImageView,
    vpp::ShaderDataBlock* pDataBlock )
{
    pDataBlock->update ((
        d_resultBuffer = resultView,
        d_testBuffer = testView,
        d_testBuffer64 = testView64,
        d_testImage = testImageView
    ));
}

// -----------------------------------------------------------------------------

void KAtomicsTestPipeline1 :: fComputeShader ( vpp::ComputeShader* pShader )
{
    using namespace vpp;

    //pShader->DebugCodeDump();

    const Int ng = pShader->inNumWorkgroups [ X ];
    const Int nl = pShader->inWorkgroupSize [ X ];
    const Int np = ng * nl;
    const Int l = pShader->inLocalInvocationId [ X ];
    const Int g = pShader->inWorkgroupId [ X ];

    const Int nIterations = 10;
    const Int nBlockSize = BLOCK_SIZE;

    UniformSimpleArray< int, decltype ( d_resultBuffer ) > outBuffer ( d_resultBuffer );
    UniformSimpleArray< int, decltype ( d_testBuffer ) > testBuffer ( d_testBuffer );
    UniformSimpleArray< std::uint64_t, decltype ( d_testBuffer64 ) > testBuffer64 ( d_testBuffer64 );
    UseImage ( d_testImage );

    WInt atomicInt = 0;
    WUInt atomicUInt = 0;
    WInt64 atomicInt64 = 0;
    WUInt64 atomicUInt64 = 0;

    VInt i;

    For ( i, 0, nIterations );
    {
        const Int localIdx = ( & atomicInt ).Increment();
        const Int globalIdx = g * nl * nIterations + localIdx;
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const UInt localIdx = ( & atomicUInt ).Increment();
        const Int globalIdx = nBlockSize + g * nl * nIterations + Int(localIdx);
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const Int64 localIdx = ( & atomicInt64 ).Add ( 1 );
        const Int globalIdx = 2*nBlockSize + g * nl * nIterations + Int(localIdx);
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const UInt64 localIdx = ( & atomicUInt64 ).Increment();
        const Int globalIdx = 3*nBlockSize + g * nl * nIterations + Int(localIdx);
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const Int localIdx = ( & testBuffer [ 0 ] ).Increment();
        const Int globalIdx = 4*nBlockSize + localIdx;
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const UInt localIdx = d_testImage.GetPointer ( IVec2 { 0, 0 } ).Increment();
        const Int globalIdx = 5*nBlockSize + Int(localIdx);
        outBuffer [ globalIdx ] = 1;
    }
    Rof();

    WorkgroupBarrier();

    For ( i, 0, nIterations );
    {
        const Pointer< UInt64 > p = & testBuffer64 [ 0 ];
        const UInt64 localIdx = p.Increment();
        const Int globalIdx = 6*nBlockSize + Int(localIdx);
        outBuffer [ globalIdx ] = 1;
    }
    Rof();
}

// -----------------------------------------------------------------------------

class KAtomicsTest :
    public vpp::Computation,
    public KAtomicsTestTypes
{
public:
    KAtomicsTest ( vpp::Computation& pred, const vpp::Device& hDevice );

    void compareResults();

private:
    vpp::ComputePipelineLayout< KAtomicsTestPipeline1 > d_pipeline;
    vpp::ShaderDataBlock d_dataBlock;

    DataBuffer d_resultBuffer;
    DataBuffer d_testBuffer;
    DataBuffer64 d_testBuffer64;
    TestImage d_testImage;
    TestImageView d_testImageView;
};

// -----------------------------------------------------------------------------

KAtomicsTest :: KAtomicsTest ( vpp::Computation& pred, const vpp::Device& hDevice ) :
    vpp::Computation ( pred ),
    d_pipeline ( hDevice ),
    d_dataBlock ( d_pipeline ),
    d_resultBuffer ( TEST_COUNT*BLOCK_SIZE, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testBuffer ( 16, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testBuffer64 ( 16, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testImage ( { 256, 1, 1 }, vpp::MemProfile::DEVICE_STATIC, hDevice ),
    d_testImageView ( d_testImage )
{
    using namespace vpp;

    d_pipeline.definition().setData (
        d_resultBuffer, d_testBuffer, d_testBuffer64,
        d_testImageView, & d_dataBlock );

    addPipeline ( d_pipeline );

    d_resultBuffer.resize ( TEST_COUNT*BLOCK_SIZE );

    ( *this ) << [ this ]()
    {
        cmdFillBuffer ( d_resultBuffer, 0, TEST_COUNT*BLOCK_SIZE*sizeof ( unsigned int ), 0 );
        cmdFillBuffer ( d_testBuffer, 0, 16*sizeof ( unsigned int ), 0 );
        cmdFillBuffer ( d_testBuffer64, 0, 16*sizeof ( std::uint64_t ), 0 );
        cmdClearColorImage ( d_testImage, clearColor ( 0, 0, 0 ) );

        cmdPipelineBarrier ( barriers (
            Bar::TRANSFER, Bar::COMPUTE,
            d_resultBuffer, d_testBuffer, d_testBuffer64, d_testImage ) );

        d_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER, 
            d_resultBuffer ) );

        d_resultBuffer.cmdLoadAll();
    };
}

// -----------------------------------------------------------------------------

void KAtomicsTest :: compareResults()
{
    std::vector< unsigned int > values ( d_resultBuffer.begin(), d_resultBuffer.end() );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + 2*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + 3*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + 4*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + 5*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
        check ( values [ i + 6*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );

    //for ( unsigned int i = 0; i != BLOCK_SIZE; ++i )
    //    check ( values [ i + 7*BLOCK_SIZE ] == static_cast< unsigned int >( i < 20480 ) );
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

    typedef TIntegerTest< int > KIntTest;
    typedef TIntegerTest< vpp::ivect2 > KIVec2Test;
    typedef TIntegerTest< vpp::ivect3 > KIVec3Test;
    typedef TIntegerTest< vpp::ivect4 > KIVec4Test;

    KFloatTest testFloat;
    KVec2Test testVec2;
    KVec3Test testVec3;
    KVec4Test testVec4;

    KIntTest testInt;
    KIVec2Test testIVec2;
    KIVec3Test testIVec3;
    KIVec4Test testIVec4;

    KVectorTest testVectors;
    KConstructTest testConstructs;
    KIndexingTest testIndexing;
    KPushConstantTest testPushConstants;
    KTexelBufferTest testTexelBuffers;
    KImageTest testImages;
    KGlobalSyncTest testGlobalSync;
    KLocalArraysTest testLocalArrays;
    KGroupAlgorithmsTest testGroupAlgorithms;
    KGroupAlgorithms2Test testGroupAlgorithms2;
    KGroupVariablesTest testGroupVariables;
    KAtomicsTest testAtomics;
};

// -----------------------------------------------------------------------------

KTests :: KTests ( const vpp::Device& hDevice ) :
    vpp::ComputationEngine ( hDevice, vpp::Q_GRAPHICS ),
    testFloat ( hDevice ),
    testVec2 ( testFloat, hDevice ),
    testVec3 ( testVec2, hDevice ),
    testVec4 ( testVec3, hDevice ),
    testInt ( testVec4, hDevice ),
    testIVec2 ( testInt, hDevice ),
    testIVec3 ( testIVec2, hDevice ),
    testIVec4 ( testIVec3, hDevice ),
    testVectors ( testIVec4, hDevice ),
    testConstructs ( testVectors, hDevice ),
    testIndexing ( testConstructs, hDevice ),
    testPushConstants ( testIndexing, hDevice ),
    testTexelBuffers ( testPushConstants, hDevice ),
    testImages ( testTexelBuffers, hDevice ),
    testGlobalSync ( testImages, hDevice ),
    testLocalArrays ( testGlobalSync, hDevice ),
    testGroupAlgorithms ( testLocalArrays, hDevice ),
    testGroupAlgorithms2 ( testGroupAlgorithms, hDevice ),
    testGroupVariables ( testGroupAlgorithms2, hDevice ),
    testAtomics ( testGroupVariables, hDevice )
{
    compile();
}

// -----------------------------------------------------------------------------

void printResults()
{
    std::cout << "VPP Computation test results:" << std::endl;

    if ( s_failedChecks == 0 )
        std::cout << "All tests passed !" << std::endl;

    std::cout << "Passed checks: " << s_passedChecks << std::endl;
    std::cout << "Failed checks: " << s_failedChecks << std::endl;
}

// -----------------------------------------------------------------------------
} // namespace vpptest
// -----------------------------------------------------------------------------

int main()
{
    using namespace vpptest;
    using namespace vpp;

    std::ostringstream validationLog;

    SVulkanVersion vkver = Instance::getLatestAvailableVulkanVersion();

    ExtensionProperties exts;
    Instance::enumerateExtensions ( & exts );

    Instance inst = createInstance()
        .vulkan ( { 1, 1, 0 } )
        .validation ( true );

    const auto& ee = inst.enabledExtensions();

    vpp::StreamDebugReporter m_debugReporter ( validationLog, inst );
    //vpp::StreamDebugReporter m_debugReporter ( validationLog, inst, vpp::DebugReporter::SHADERS );

    PhysicalDevices physicalDevices;

    if ( inst.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
         || physicalDevices.empty() )
    {
        return -1;
    }

    PhysicalDevice phd = physicalDevices [ 0 ];

    const auto& props = phd.properties();
    const auto& api = SVulkanVersion::fromId ( props.apiVersion );

    DeviceFeatures feat;
    feat.enableIfSupported ( fShaderInt64, phd );
    feat.enableIfSupported ( fShaderStorageImageExtendedFormats, phd );
    feat.enableIfSupported ( fShaderFloat64, phd );

    feat.enableIfSupported ( fShaderBufferInt64Atomics, phd );
    feat.enableIfSupported ( fShaderSharedInt64Atomics, phd );

    size_t sf = sizeof ( feat );

    Device dev ( phd, feat );

    bool bSupVer = dev.supportsVersion ( { 1, 1, 0 } );

    KTests testObject ( dev );
    testObject.testFloat();
    testObject.testVec2();
    testObject.testVec3();
    testObject.testVec4();
    testObject.testInt();
    testObject.testIVec2();
    testObject.testIVec3();
    testObject.testIVec4();
    testObject.testVectors();
    testObject.testConstructs();
    testObject.testIndexing();
    testObject.testPushConstants();
    testObject.testTexelBuffers();
    testObject.testImages();
    testObject.testGlobalSync();
    testObject.testLocalArrays();
    testObject.testGroupAlgorithms();
    testObject.testGroupAlgorithms2();
    testObject.testGroupVariables();
    testObject.testAtomics ( NO_TIMEOUT );

    testObject.testFloat.compareResults();
    testObject.testVec2.compareResults();
    testObject.testVec3.compareResults();
    testObject.testVec4.compareResults();
    testObject.testInt.compareResults();
    testObject.testIVec2.compareResults();
    testObject.testIVec3.compareResults();
    testObject.testIVec4.compareResults();
    testObject.testVectors.compareResults();
    testObject.testConstructs.compareResults();
    testObject.testIndexing.compareResults();
    testObject.testPushConstants.compareResults();
    testObject.testTexelBuffers.compareResults();
    testObject.testImages.compareResults();
    testObject.testGlobalSync.compareResults();
    testObject.testLocalArrays.compareResults();
    testObject.testGroupAlgorithms.compareResults();
    testObject.testGroupAlgorithms2.compareResults();
    testObject.testGroupVariables.compareResults();
    testObject.testAtomics.compareResults();

    std::string vl = validationLog.str();

    printResults();

    return 0;
}

// -----------------------------------------------------------------------------
