/*
    Copyright 2016-2019 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
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

#ifndef INC_VPPLANGCONVERSIONS_HPP
#define INC_VPPLANGCONVERSIONS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGMATRIXTYPES_HPP
#include "vppLangMatrixTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

VPP_INLINE spv::Id extendIntegerTypeTo64 ( spv::Id type, bool bSigned )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    const spv::Id componentType = pTranslator->makeIntegerType ( 64, bSigned );

    if ( pTranslator->isVectorType ( type ) )
    {
        const int nComponents = pTranslator->getNumTypeComponents ( type );
        return pTranslator->makeVectorType ( componentType, nComponents );
    }
    else
        return componentType;
}

// -----------------------------------------------------------------------------

VPP_INLINE spv::Id reduceIntegerTypeTo32 ( spv::Id type, bool bSigned )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    const spv::Id componentType = pTranslator->makeIntegerType ( 32, bSigned );

    if ( pTranslator->isVectorType ( type ) )
    {
        const int nComponents = pTranslator->getNumTypeComponents ( type );
        return pTranslator->makeVectorType ( componentType, nComponents );
    }
    else
        return componentType;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class TargetT, class SourceT, typename TargetScalarT, typename SourceScalarT >
struct TConvertBaseTypes {};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, unsigned int, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertFToU, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, unsigned int, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertFToU, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, unsigned int, int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, int, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertFToS, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, int, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertFToS, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, int, unsigned int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float, unsigned int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertUToF, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, double, unsigned int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertUToF, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float, int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertSToF, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, double, int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpConvertSToF, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, double, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpFConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpFConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float16_t, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpFConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float, float16_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpFConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint64_t, std::uint32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpUConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint32_t, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpUConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int64_t, std::int32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpSConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int32_t, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpSConvert, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int64_t, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint64_t, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint64_t, std::int32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        const spv::Id idTypeConvertedToInt64 = extendIntegerTypeTo64 ( SourceT::getType(), true );

        const spv::Id idConvertedToInt64 = KShaderTranslator::get()->createUnaryOp (
            spv::OpSConvert, idTypeConvertedToInt64, source.id() );

        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), idConvertedToInt64 ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int64_t, std::uint32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        const spv::Id idTypeConvertedToInt64 = extendIntegerTypeTo64 ( SourceT::getType(), false );

        const spv::Id idConvertedToInt64 = KShaderTranslator::get()->createUnaryOp (
            spv::OpUConvert, idTypeConvertedToInt64, source.id() );

        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), idConvertedToInt64 ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint32_t, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        const spv::Id idTypeConvertedToInt32 = reduceIntegerTypeTo32 ( SourceT::getType(), true );

        const spv::Id idConvertedToInt32 = KShaderTranslator::get()->createUnaryOp (
            spv::OpSConvert, idTypeConvertedToInt32, source.id() );

        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), idConvertedToInt32 ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int32_t, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        const spv::Id idTypeConvertedToInt32 = reduceIntegerTypeTo32 ( SourceT::getType(), false );

        const spv::Id idConvertedToInt32 = KShaderTranslator::get()->createUnaryOp (
            spv::OpUConvert, idTypeConvertedToInt32, source.id() );

        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), idConvertedToInt32 ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0.0f );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpFOrdNotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0.0 );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpFOrdNotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, std::int32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0 );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpINotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0ll );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpINotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, std::uint32_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0u );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpINotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, bool, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const SourceT zero = SourceT ( 0ull );

        return TargetT ( KId ( pTranslator->createBinOp (
            spv::OpINotEqual, TargetT::getType(), source.id(), zero.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, float, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0.0f );
        const TargetT one = TargetT ( 1.0f );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, double, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0.0 );
        const TargetT one = TargetT ( 1.0 );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int32_t, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0 );
        const TargetT one = TargetT ( 1 );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::int64_t, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0ll );
        const TargetT one = TargetT ( 1ll );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint32_t, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0u );
        const TargetT one = TargetT ( 1u );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TConvertBaseTypes< TargetT, SourceT, std::uint64_t, bool >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        const TargetT zero = TargetT ( 0ull );
        const TargetT one = TargetT ( 1ull );

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = source.id();
        operands [ 1 ] = one.id();
        operands [ 2 ] = zero.id();

        return TargetT ( KId ( pTranslator->createOp (
            spv::OpSelect, TargetT::getType(), operands ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class IdentityT, class IdentityScalarT >
struct TConvertBaseTypes< IdentityT, IdentityT, IdentityScalarT, IdentityScalarT >
{
    VPP_INLINE IdentityT operator()( const IdentityT& source ) const
    {
        return source;
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class TargetT, class SourceT, typename TargetScalarT, typename SourceScalarT >
struct TCastBaseTypes {};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, unsigned int, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, float, unsigned int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, int, float >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, float, int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, int, unsigned int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, unsigned int, int >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, std::uint64_t, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, std::int64_t, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, double, std::int64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, std::int64_t, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, double, std::uint64_t >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------

template< class TargetT, class SourceT >
struct TCastBaseTypes< TargetT, SourceT, std::uint64_t, double >
{
    VPP_INLINE TargetT operator()( const SourceT& source ) const
    {
        return TargetT ( KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpBitcast, TargetT::getType(), source.id() ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename TargetTypeT, typename SourceTypeT >
struct TConvert
{
    static const bool isValid = false;
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT, typename SourceScalarT >
struct TConvert< TRValue< TargetScalarT >, TRValue< SourceScalarT > >
{
    static const bool isValid = true;

    VPP_INLINE TRValue< TargetScalarT > operator()( const TRValue< SourceScalarT >& source ) const
    {
        return TConvertBaseTypes<
            TRValue< TargetScalarT >, TRValue< SourceScalarT >,
            TargetScalarT, SourceScalarT >()( source );
    }
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT >
struct TConvert< TRValue< TargetScalarT >, Bool >
{
    static const bool isValid = true;

    VPP_INLINE TRValue< TargetScalarT > operator()( const Bool& source ) const
    {
        return TConvertBaseTypes<
            TRValue< TargetScalarT >, Bool, TargetScalarT, bool >()( source );
    }
};

// -----------------------------------------------------------------------------

template< typename SourceScalarT >
struct TConvert< Bool, TRValue< SourceScalarT > >
{
    static const bool isValid = true;

    VPP_INLINE Bool operator()( const TRValue< SourceScalarT >& source ) const
    {
        return TConvertBaseTypes<
            Bool, TRValue< SourceScalarT >, bool, SourceScalarT >()( source );
    }
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT, typename SourceScalarT, size_t SIZE >
struct TConvert< TRVector< TargetScalarT, SIZE >, TRVector< SourceScalarT, SIZE > >
{
    static const bool isValid = true;

    VPP_INLINE TRVector< TargetScalarT, SIZE > operator()( const TRVector< SourceScalarT, SIZE >& source ) const
    {
        typedef typename SourceScalarT::scalar_type source_scalar_type;
        typedef typename TargetScalarT::scalar_type target_scalar_type;

        return TConvertBaseTypes<
            TRVector< TargetScalarT, SIZE >, TRVector< SourceScalarT, SIZE >,
            target_scalar_type, source_scalar_type >()( source );
    }
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT, typename SourceScalarT, size_t COLS, size_t ROWS >
struct TConvert< TRMatrix< TargetScalarT, COLS, ROWS >, TRMatrix< SourceScalarT, COLS, ROWS > >
{
    static const bool isValid = true;

    VPP_INLINE TRMatrix< TargetScalarT, COLS, ROWS > operator()( const TRMatrix< SourceScalarT, COLS, ROWS >& source ) const
    {
        return source.componentWiseOperation (
            []( const TRVector< SourceScalarT, ROWS >& v )
            {
                return TConvert<
                    TRVector< TargetScalarT, ROWS >,
                    TRVector< SourceScalarT, ROWS > >()( v );
            }
        );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename TargetTypeT, typename SourceTypeT >
struct TCast
{
    static const bool isValid = false;
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT, typename SourceScalarT >
struct TCast< TRValue< TargetScalarT >, TRValue< SourceScalarT > >
{
    static const bool isValid = true;

    VPP_INLINE TRValue< TargetScalarT > operator()( const TRValue< SourceScalarT >& source ) const
    {
        return TCastBaseTypes<
            TRValue< TargetScalarT >, TRValue< SourceScalarT >,
            TargetScalarT, SourceScalarT >()( source );
    }
};

// -----------------------------------------------------------------------------

template< typename TargetScalarT, typename SourceScalarT, size_t SIZE >
struct TCast< TRVector< TargetScalarT, SIZE >, TRVector< SourceScalarT, SIZE > >
{
    static const bool isValid = true;

    VPP_INLINE TRVector< TargetScalarT, SIZE > operator()( const TRVector< SourceScalarT, SIZE >& source ) const
    {
        typedef typename SourceScalarT::scalar_type source_scalar_type;
        typedef typename TargetScalarT::scalar_type target_scalar_type;

        return TCastBaseTypes<
            TRVector< TargetScalarT, SIZE >, TRVector< SourceScalarT, SIZE >,
            target_scalar_type, source_scalar_type >()( source );
    }
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< typename TargetT, typename SourceT >
VPP_INLINE TargetT StaticCast ( const SourceT& source )
{
    typedef typename detail::TGetRV< SourceT >::type SourceRVT;
    typedef detail::TConvert< TargetT, SourceRVT > Converter;

    static_assert (
        Converter::isValid,
        "StaticCast between these types is not supported" );

    const SourceRVT rSource = source;
    return Converter()( rSource );
}

// -----------------------------------------------------------------------------

template< typename TargetT, typename SourceT >
VPP_INLINE TargetT ReinterpretCast ( const SourceT& source )
{
    typedef typename detail::TGetRV< SourceT >::type SourceRVT;
    typedef detail::TCast< TargetT, SourceRVT > Converter;

    static_assert (
        Converter::isValid,
        "ReinterpretCast between these types is not supported" );

    const SourceRVT rSource = source;
    return Converter()( rSource );
}

// -----------------------------------------------------------------------------

template< typename ScalarT >
template< typename OtherScalarT >
VPP_INLINE TRValue< ScalarT > :: TRValue ( const TRValue< OtherScalarT >& rhs ) :
    TRValue< ScalarT >( StaticCast< TRValue< ScalarT > >( rhs ) )
{
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGCONVERSIONS_HPP
