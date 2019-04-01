// -----------------------------------------------------------------------------

#include <vppAll.hpp>

// -----------------------------------------------------------------------------
namespace vpptest {
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

void testBasicTypes()
{
    using namespace vpp;

    {
        usint2_t v20, v21, v22, v23, v24;
        v20 = 0u; v21 = 1u; v22 = 2u; v23 = 3u; v24 = 4u;
        check ( v20 == 0u ); check ( v21 == 1u ); check ( v22 == 2u );
        check ( v23 == 3u ); check ( v24 == 0u );

        usint8_t v80, v81, v82, v8255, v8256;
        v80 = 0u; v81 = 1u; v82 = 2u; v8255 = 255u; v8256 = 256u;
        check ( v80 == 0u ); check ( v81 == 1u ); check ( v82 == 2u );
        check ( v8255 == 255u ); check ( v8256 == 0u );
    }
    {
        sint2_t v20, v21, v2m1, v2m2, v24;
        v20 = 0; v21 = 1; v2m1 = -1; v2m2 = -2; v24 = 4;
        check ( v20 == 0 ); check ( v21 == 1 ); check ( v2m1 == -1 );
        check ( v2m2 == -2 ); check ( v24 == 0 );

        sint8_t v80, v81, v8m1, v8256;
        v80 = 0; v81 = 1; v8m1 = -1; v8256 = 256;
        check ( v80 == 0 ); check ( v81 == 1 ); check ( v8m1 == -1 );
        check ( v8256 == 0 );
    }
    {
        unorm8_t a0, a1, a2;
        a0 = 0.0f; check ( float(a0) == 0.0f );
        a1 = 1.0f; check ( float(a1) == 1.0f );
        a2 = 7.0f/255.0f; check ( float(a2) == 7.0f/255.0f );

        a0 = 0.0; check ( double(a0) == 0.0 );
        a1 = 1.0; check ( double(a1) == 1.0 );
        a2 = 7.0/255.0; check ( double(a2) == 7.0/255.0 );
    }
    {
        snorm8_t a0, a1, a2, a3, a4;
        a0 = 0.0f; check ( float(a0) == 0.0f );
        a1 = 1.0f; check ( float(a1) == 1.0f );
        a2 = -1.0f; check ( float(a2) == -1.0f );
        a3 = 7.0f/127.0f; check ( float(a3) == 7.0f/127.0f );
        a4 = -7.0f/127.0f; check ( float(a4) == -7.0f/127.0f );

        a0 = 0.0; check ( double(a0) == 0.0 );
        a1 = 1.0; check ( double(a1) == 1.0 );
        a2 = -1.0; check ( double(a2) == -1.0 );
        a3 = 7.0/127.0; check ( double(a3) == 7.0/127.0 );
        a4 = -7.0/127.0; check ( double(a4) == -7.0/127.0 );
    }

    // uscaled_t
    // sscaled_t
    // srgb_t
    // float16_t
}

// -----------------------------------------------------------------------------

void testPackedFormats()
{
    using namespace vpp;

    {
        typedef format< unorm4_t, unorm4_t > FmtR4G4;
        typedef FmtR4G4::data_type DatR4G4;
        check ( FmtR4G4::code == VK_FORMAT_R4G4_UNORM_PACK8 );
        check ( sizeof ( DatR4G4 ) == 1 );

        DatR4G4 a00, a01, a10, a11;
        a00.set ( 0.0f, 0.0f ); check ( float(a00.r) == 0.0f && float(a00.g) == 0.0f );
        a01.set ( 0.0f, 1.0f ); check ( float(a01.r) == 0.0f && float(a01.g) == 1.0f );
        a10.set ( 1.0f, 0.0f ); check ( float(a10.r) == 1.0f && float(a10.g) == 0.0f );
        a11.set ( 1.0f, 1.0f ); check ( float(a11.r) == 1.0f && float(a11.g) == 1.0f );
    }
    {
        typedef format< unorm4_t, unorm4_t, unorm4_t, unorm4_t > FmtR4G4B4A4;
        typedef FmtR4G4B4A4::data_type DatR4G4B4A4;
        check ( FmtR4G4B4A4::code == VK_FORMAT_R4G4B4A4_UNORM_PACK16 );
        check ( sizeof ( DatR4G4B4A4 ) == 2 );

        DatR4G4B4A4 a0000, a0001, a0010, a0100, a1000, a1111;
        a0000.set ( 0.0f, 0.0f, 0.0f, 0.0f ); check ( float(a0000.r) == 0.0f && float(a0000.g) == 0.0f && float(a0000.b) == 0.0f && float(a0000.a) == 0.0f );
        a0001.set ( 0.0f, 0.0f, 0.0f, 1.0f ); check ( float(a0001.r) == 0.0f && float(a0001.g) == 0.0f && float(a0001.b) == 0.0f && float(a0001.a) == 1.0f );
        a0010.set ( 0.0f, 0.0f, 1.0f, 0.0f ); check ( float(a0010.r) == 0.0f && float(a0010.g) == 0.0f && float(a0010.b) == 1.0f && float(a0010.a) == 0.0f );
        a0100.set ( 0.0f, 1.0f, 0.0f, 0.0f ); check ( float(a0100.r) == 0.0f && float(a0100.g) == 1.0f && float(a0100.b) == 0.0f && float(a0100.a) == 0.0f );
        a1000.set ( 1.0f, 0.0f, 0.0f, 0.0f ); check ( float(a1000.r) == 1.0f && float(a1000.g) == 0.0f && float(a1000.b) == 0.0f && float(a1000.a) == 0.0f );
        a1111.set ( 1.0f, 1.0f, 1.0f, 1.0f ); check ( float(a1111.r) == 1.0f && float(a1111.g) == 1.0f && float(a1111.b) == 1.0f && float(a1111.a) == 1.0f );
    }
    {
        typedef format< unorm4_t, unorm4_t, unorm4_t, unorm4_t, BGRA > FmtB4G4R4A4;
        typedef FmtB4G4R4A4::data_type DatB4G4R4A4;
        check ( FmtB4G4R4A4::code == VK_FORMAT_B4G4R4A4_UNORM_PACK16 );
        check ( sizeof ( DatB4G4R4A4 ) == 2 );

        DatB4G4R4A4 a0000, a0001, a0010, a0100, a1000, a1111;
        a0000.set ( 0.0f, 0.0f, 0.0f, 0.0f ); check ( float(a0000.r) == 0.0f && float(a0000.g) == 0.0f && float(a0000.b) == 0.0f && float(a0000.a) == 0.0f );
        a0001.set ( 0.0f, 0.0f, 0.0f, 1.0f ); check ( float(a0001.r) == 0.0f && float(a0001.g) == 0.0f && float(a0001.b) == 0.0f && float(a0001.a) == 1.0f );
        a0010.set ( 0.0f, 0.0f, 1.0f, 0.0f ); check ( float(a0010.r) == 0.0f && float(a0010.g) == 0.0f && float(a0010.b) == 1.0f && float(a0010.a) == 0.0f );
        a0100.set ( 0.0f, 1.0f, 0.0f, 0.0f ); check ( float(a0100.r) == 0.0f && float(a0100.g) == 1.0f && float(a0100.b) == 0.0f && float(a0100.a) == 0.0f );
        a1000.set ( 1.0f, 0.0f, 0.0f, 0.0f ); check ( float(a1000.r) == 1.0f && float(a1000.g) == 0.0f && float(a1000.b) == 0.0f && float(a1000.a) == 0.0f );
        a1111.set ( 1.0f, 1.0f, 1.0f, 1.0f ); check ( float(a1111.r) == 1.0f && float(a1111.g) == 1.0f && float(a1111.b) == 1.0f && float(a1111.a) == 1.0f );
    }
    {
        
        typedef format< unorm5_t, unorm6_t, unorm5_t > FmtR5G6B5;
        typedef FmtR5G6B5::data_type DatR5G6B5;
        check ( FmtR5G6B5::code == VK_FORMAT_R5G6B5_UNORM_PACK16 );
        check ( sizeof ( DatR5G6B5 ) == 2 );

        DatR5G6B5 a000, a001, a010, a100, a111;
        a000.set ( 0.0f, 0.0f, 0.0f ); check ( float(a000.r) == 0.0f && float(a000.g) == 0.0f && float(a000.b) == 0.0f );
        a001.set ( 0.0f, 0.0f, 1.0f ); check ( float(a001.r) == 0.0f && float(a001.g) == 0.0f && float(a001.b) == 1.0f );
        a010.set ( 0.0f, 1.0f, 0.0f ); check ( float(a010.r) == 0.0f && float(a010.g) == 1.0f && float(a010.b) == 0.0f );
        a100.set ( 1.0f, 0.0f, 0.0f ); check ( float(a100.r) == 1.0f && float(a100.g) == 0.0f && float(a100.b) == 0.0f );
        a111.set ( 1.0f, 1.0f, 1.0f ); check ( float(a111.r) == 1.0f && float(a111.g) == 1.0f && float(a111.b) == 1.0f );
    }
}

// -----------------------------------------------------------------------------

void printResults()
{
    std::cout << "VPP Formats test results:" << std::endl;

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
    testBasicTypes();
    testPackedFormats();

    printResults();

    return 0;
}

// -----------------------------------------------------------------------------
