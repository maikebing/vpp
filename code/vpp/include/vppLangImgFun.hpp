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

#ifndef INC_VPPSHADERLANGUAGEIMGFUN_HPP
#define INC_VPPSHADERLANGUAGEIMGFUN_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGIMAGES_HPP
#include "vppLangImages.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct GatherOffsets
{
    typedef const GatherOffsets& rvalue_type;
    int offsets [ 4 ][ 2 ];
};

// -----------------------------------------------------------------------------
namespace impl {
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, bool UNNORM >
VPP_INLINE TRSampledTexture< ViewT, ScalarT > MakeSampledTexture (
    const TRTexture< ViewT, ScalarT >& hTexture, const TRSampler< UNNORM >& hSampler )
{
    static_assert (
        ! UNNORM || ViewT::isUnnormSampling,
        "Unnormalized sampler can't be bound to normalized-only sampling view" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();
    const KId resultType ( pTranslator->makeSampledImageType ( hTexture.type() ) );

    return TRSampledTexture< ViewT, ScalarT > (
        KId ( pTranslator->createBinOp (
            spv::OpSampledImage, resultType, hTexture.id(), hSampler.id() ) ),
        resultType
    );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRTexture< ViewT, ScalarT > ExtractSampledTexture (
    const TRSampledTexture< ViewT, ScalarT >& hSampledTexture )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    const KId sid = hSampledTexture.id();

    return TRTexture< ViewT, ScalarT >(
        KId ( pTranslator->createUnaryOp (
            spv::OpImage, pTranslator->getImageType ( sid ), sid ) ),
        KId ( pTranslator->getImageType ( sid ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRTexture< ViewT, ScalarT > ExtractSampledTexture (
    const TRTexture< ViewT, ScalarT >& hTexture )
{
    return hTexture;
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRImage< ViewT, ScalarT > ExtractSampledTexture (
    const TRImage< ViewT, ScalarT >& hImage )
{
    return hImage;
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ImageT >
KId TextureQuery (
    const ImageT& img, spv::Op op, spv::Id lod = 0, spv::Id coords = 0  )
{
    // Capability: done by createTextureQueryCall

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = ExtractSampledTexture ( img ).id();
    params.lod = lod;
    params.coords = coords;

    return KId ( pTranslator->createTextureQueryCall ( op, params ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE typename TRImage< ViewT, ScalarT >::size_type ImageSize (
    const TRImage< ViewT, ScalarT >& img )
{
    typedef TRImage< ViewT, ScalarT > image_type;
    typedef typename image_type::size_type result_type;

    return result_type ( detail::TextureQuery ( img, spv::OpImageQuerySize ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE typename TRTexture< ViewT, ScalarT >::size_type ImageSize (
    const TRTexture< ViewT, ScalarT >& img )
{
    typedef TRTexture< ViewT, ScalarT > image_type;
    typedef typename image_type::size_type result_type;

    // 1, 2, 3, cube => MULTISAMPLED
    static_assert (
        ! ( image_type::DIM == spv::Dim1D
            || image_type::DIM == spv::Dim2D
            || image_type::DIM == spv::Dim3D
            || image_type::DIM == spv::DimCube )
        || ( image_type::MULTISAMPLED == true ),
        "ImageSize: you must use TextureSize with 'lod' argument for this image" );

    return result_type ( detail::TextureQuery ( img, spv::OpImageQuerySize ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE typename TRSampledTexture< ViewT, ScalarT >::size_type ImageSize (
    const TRSampledTexture< ViewT, ScalarT >& img )
{
    typedef TRSampledTexture< ViewT, ScalarT > image_type;
    typedef typename image_type::size_type result_type;

    // 1, 2, 3, cube => MULTISAMPLED
    static_assert (
        ! ( image_type::DIM == spv::Dim1D
            || image_type::DIM == spv::Dim2D
            || image_type::DIM == spv::Dim3D
            || image_type::DIM == spv::DimCube )
        || ( image_type::MULTISAMPLED == true ),
        "ImageSize: you must use TextureSize with 'lod' argument for this image" );

    return result_type ( detail::TextureQuery ( img, spv::OpImageQuerySize ) );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE typename ImageT::size_type TextureSize (
    const ImageT& img, const Int& lod )
{
    typedef ImageT image_type;
    typedef typename image_type::size_type result_type;

    static_assert (
        ( image_type::DIM == spv::Dim1D
          || image_type::DIM == spv::Dim2D
          || image_type::DIM == spv::Dim3D
          || image_type::DIM == spv::DimCube ),
        "TextureSize: must be image of dimensionality: 1D, 2D, 3D or Cube (Buffer and Rect are not allowed)" );

    static_assert (
        image_type::MULTISAMPLED == false,
        "TextureSize with 'lod' argument does not support multisampled images" );

    return result_type ( TextureQuery ( img, spv::OpImageQuerySizeLod, lod.id() ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE typename TRTexture< ViewT, ScalarT >::size_type TextureSize (
    const TRTexture< ViewT, ScalarT >& img, const Int& lod )
{
    return detail::TextureSize ( img, lod );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE typename TRSampledTexture< ViewT, ScalarT >::size_type TextureSize (
    const TRSampledTexture< ViewT, ScalarT >& img, const Int& lod )
{
    return detail::TextureSize ( img, lod );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE Int ImageQuerySamples ( const ImageT& img )
{
    typedef ImageT image_type;

    static_assert (
        image_type::DIM == spv::Dim2D,
        "ImageQuerySamples supports only 2D images" );

    if ( ImageT::MULTISAMPLED )
        return Int ( TextureQuery ( img, spv::OpImageQuerySamples ) );
    else
        return Int ( 1 );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQuerySamples ( const TRImage< ViewT, ScalarT >& img )
{
    return detail::ImageQuerySamples ( img );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQuerySamples ( const TRTexture< ViewT, ScalarT >& img )
{
    return detail::ImageQuerySamples ( img );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQuerySamples ( const TRSampledTexture< ViewT, ScalarT >& img )
{
    return detail::ImageQuerySamples ( img );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE Int ImageQueryLevels ( const ImageT& img )
{
    typedef ImageT image_type;

    static_assert (
        image_type::DIM == spv::Dim1D
        || image_type::DIM == spv::Dim2D
        || image_type::DIM == spv::Dim3D
        || image_type::DIM == spv::DimCube,
        "ImageQueryLevels does not support this image type" );

    return Int ( TextureQuery ( img, spv::OpImageQueryLevels ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQueryLevels ( const TRImage< ViewT, ScalarT >& img )
{
    return detail::ImageQueryLevels ( img );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQueryLevels ( const TRTexture< ViewT, ScalarT >& img )
{
    return detail::ImageQueryLevels ( img );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Int ImageQueryLevels ( const TRSampledTexture< ViewT, ScalarT >& img )
{
    return detail::ImageQueryLevels ( img );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE Vec2 TextureQueryLod (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::bcoord_type& coords )
{
    typedef TRSampledTexture< ViewT, ScalarT > image_type;

    static_assert (
        image_type::DIM == spv::Dim1D
        || image_type::DIM == spv::Dim2D
        || image_type::DIM == spv::Dim3D
        || image_type::DIM == spv::DimCube,
        "TextureQueryLod does not support this image type" );

    return Vec2 ( detail::TextureQuery ( img, spv::OpImageQueryLod, 0, coords.id() ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > SubpassLoad (
    const TRInput< ViewT, ScalarT >& img )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    std::vector< spv::Id > operands ( 2 );
    std::vector< spv::Id > comps ( 2 );

    const spv::Id zero = pTranslator->makeIntConstant ( 0 );
    comps [ 0 ] = zero;
    comps [ 1 ] = zero;

    operands [ 0 ] = img.id();

    operands [ 1 ] = pTranslator->makeCompositeConstant (
        pTranslator->makeVectorType ( pTranslator->makeIntType ( 32 ), 2 ),
        comps );

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > SubpassLoad (
    const TRInput< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 2 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();

    // Pitfall: these coords are relative to current pixel position,
    // according to SPIR-V spec. This is different from other image read ops.

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > SubpassLoad (
    const TRInput< ViewT, ScalarT >& img,
    const Int& nSample )
{
    static_assert (
        ViewT::MULTISAMPLED,
        "SubpassLoad: sample index specified, but image is not multisampled" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();
    std::vector< spv::Id > operands ( 4 );
    std::vector< spv::Id > comps ( 2 );

    // Pitfall: these coords are relative to current pixel position,
    // according to SPIR-V spec. Therefore we supply zeros.

    const spv::Id zero = pTranslator->makeIntConstant ( 0 );
    comps [ 0 ] = zero;
    comps [ 1 ] = zero;

    operands [ 0 ] = img.id();

    operands [ 1 ] = pTranslator->makeCompositeConstant (
        pTranslator->makeVectorType ( pTranslator->makeIntType ( 32 ), 2 ),
        comps );

    operands [ 2 ] = spv::ImageOperandsSampleMask;
    operands [ 3 ] = nSample.id();

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > SubpassLoad (
    const TRInput< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& nSample )
{
    static_assert (
        ViewT::MULTISAMPLED,
        "SubpassLoad: sample index specified, but image is not multisampled" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 4 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();
    operands [ 2 ] = spv::ImageOperandsSampleMask;
    operands [ 3 ] = nSample.id();

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > ImageLoad (
    const TRImage< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 2 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();

    if ( pTranslator->getImageTypeFormat ( pTranslator->getImageType ( img.id() ) ) == spv::ImageFormatUnknown )
        pTranslator->useCapability ( spv::CapabilityStorageImageReadWithoutFormat );

    switch ( ViewT::DIM )
    {
        case spv::Dim1D: pTranslator->useCapability ( spv::CapabilityImage1D ); break;
        case spv::DimRect: pTranslator->useCapability ( spv::CapabilityImageRect ); break;
        case spv::DimBuffer: pTranslator->useCapability ( spv::CapabilityImageBuffer ); break;

        case spv::DimCube:
            if ( ViewT::ARRAYED )
                pTranslator->useCapability ( spv::CapabilityImageCubeArray );
            break;

        default:
            if ( ViewT::ARRAYED && ViewT::MULTISAMPLED )
                pTranslator->useCapability ( spv::CapabilityImageMSArray );
            break;
    }

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
TRVector< ScalarT, 4 > ImageLoad (
    const TRImage< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& nSample )
{
    static_assert (
        ViewT::MULTISAMPLED,
        "ImageLoad: sample index specified, but image is not multisampled" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 4 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();
    operands [ 2 ] = spv::ImageOperandsSampleMask;
    operands [ 3 ] = nSample.id();

    if ( pTranslator->getImageTypeFormat ( pTranslator->getImageType ( img.id() ) ) == spv::ImageFormatUnknown )
        pTranslator->useCapability ( spv::CapabilityStorageImageReadWithoutFormat );

    if ( ViewT::ARRAYED )
        pTranslator->useCapability ( spv::CapabilityImageMSArray );

    return TRVector< ScalarT, 4 > ( KId ( pTranslator->createOp (
        spv::OpImageRead, TRVector< ScalarT, 4 >::getType(), operands ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class BaseT >
void ImageStore (
    const TRImage< ViewT, TRValue< BaseT > >& img,
    const typename ViewT::size_type& coords,
    const TRValue< BaseT >& value )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 3 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();
    operands [ 2 ] = value.id();

    if ( pTranslator->getImageTypeFormat ( pTranslator->getImageType ( img.id() ) ) == spv::ImageFormatUnknown )
        pTranslator->useCapability ( spv::CapabilityStorageImageReadWithoutFormat );

    switch ( ViewT::DIM )
    {
        case spv::Dim1D: pTranslator->useCapability ( spv::CapabilityImage1D ); break;
        case spv::DimRect: pTranslator->useCapability ( spv::CapabilityImageRect ); break;
        case spv::DimBuffer: pTranslator->useCapability ( spv::CapabilityImageBuffer ); break;

        case spv::DimCube:
            if ( ViewT::ARRAYED )
                pTranslator->useCapability ( spv::CapabilityImageCubeArray );
            break;

        default:
            if ( ViewT::ARRAYED && ViewT::MULTISAMPLED )
                pTranslator->useCapability ( spv::CapabilityImageMSArray );
            break;
    }

    pTranslator->createNoResultOp ( spv::OpImageWrite, operands );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, size_t SIZE >
void ImageStore (
    const TRImage< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const TRVector< ScalarT, SIZE >& value )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 3 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();
    operands [ 2 ] = value.id();

    if ( pTranslator->getImageTypeFormat ( pTranslator->getImageType ( img.id() ) ) == spv::ImageFormatUnknown )
        pTranslator->useCapability ( spv::CapabilityStorageImageReadWithoutFormat );

    switch ( ViewT::DIM )
    {
        case spv::Dim1D: pTranslator->useCapability ( spv::CapabilityImage1D ); break;
        case spv::DimRect: pTranslator->useCapability ( spv::CapabilityImageRect ); break;
        case spv::DimBuffer: pTranslator->useCapability ( spv::CapabilityImageBuffer ); break;

        case spv::DimCube:
            if ( ViewT::ARRAYED )
                pTranslator->useCapability ( spv::CapabilityImageCubeArray );
            break;

        default:
            if ( ViewT::ARRAYED && ViewT::MULTISAMPLED )
                pTranslator->useCapability ( spv::CapabilityImageMSArray );
            break;
    }

    pTranslator->createNoResultOp ( spv::OpImageWrite, operands );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, size_t SIZE >
void ImageStore (
    const TRImage< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const TRVector< ScalarT, SIZE >& value,
    const Int& nSample )
{
    static_assert (
        ViewT::MULTISAMPLED,
        "ImageStore: sample index specified, but image is not multisampled" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > operands ( 5 );
    operands [ 0 ] = img.id();
    operands [ 1 ] = coords.id();
    operands [ 2 ] = value.id();
    operands [ 3 ] = spv::ImageOperandsSampleMask;
    operands [ 4 ] = nSample.id();

    if ( pTranslator->getImageTypeFormat ( pTranslator->getImageType ( img.id() ) ) == spv::ImageFormatUnknown )
        pTranslator->useCapability ( spv::CapabilityStorageImageReadWithoutFormat );

    if ( ViewT::ARRAYED )
        pTranslator->useCapability ( spv::CapabilityImageMSArray );

    pTranslator->createNoResultOp ( spv::OpImageWrite, operands );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ScalarT, class ImageT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetch (
    const ImageT& img,
    const typename ImageT::size_type& coords,
    spv::Id lod = 0,
    spv::Id sample = 0,
    spv::Id offset = 0 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = ExtractSampledTexture ( img ).id();
    params.coords = coords.id();
    params.lod = lod;
    params.sample = sample;
    params.offset = offset;

    const bool bNoImplicitLod = ( ImageT::DIM != spv::DimBuffer );

    return TRVector< ScalarT, 4 > ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, true, false, false, bNoImplicitLod, params ) ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetch (
    const TRTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    return detail::TexelFetch< ScalarT > ( img, coords );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetch (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    return detail::TexelFetch< ScalarT > ( img, coords );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetch (
    const TRTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& nSample )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED,
        "TexelFetch: sample index specified, but image is not multisampled" );

    return detail::TexelFetch< ScalarT > ( img, coords, 0, nSample.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetch (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& nSample )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED,
        "TexelFetch: sample index specified, but image is not multisampled" );

    return detail::TexelFetch< ScalarT > ( img, coords, 0, nSample.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchLod (
    const TRTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& lod )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "TexelFetchLod does not support multisampled images" );

    return detail::TexelFetch< ScalarT > ( img, coords, lod.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchLod (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& lod )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "TexelFetchLod does not support multisampled images" );

    return detail::TexelFetch< ScalarT > ( img, coords, lod.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchOffset (
    const TRTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const typename ViewT::offset_type& offset )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    return detail::TexelFetch< ScalarT > ( img, coords, 0, 0, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchOffset (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const typename ViewT::offset_type& offset )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    return detail::TexelFetch< ScalarT > ( img, coords, 0, 0, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchLodOffset (
    const TRTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& lod,
    const typename ViewT::offset_type& offset )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "TexelFetchOffset does not support multisampled images" );

    return detail::TexelFetch< ScalarT > ( img, coords, lod.id(), 0, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TexelFetchLodOffset (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename ViewT::size_type& coords,
    const Int& lod,
    const typename ViewT::offset_type& offset )
{
    static_assert (
        ViewT::DIM != spv::DimCube,
        "TexelFetch: cube maps not allowed" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "TexelFetchOffset does not support multisampled images" );

    return detail::TexelFetch< ScalarT > ( img, coords, lod.id(), 0, offset.id() );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureSample (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    spv::Id lod = 0,
    spv::Id bias = 0,
    spv::Id offset = 0,
    spv::Id gx = 0,
    spv::Id gy = 0,
    spv::Id dref = 0 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.lod = lod;
    params.bias = bias;
    params.offset = offset;
    params.gradX = gx;
    params.gradY = gy;
    params.Dref = dref;

    return TRVector< ScalarT, 4 > ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, false, false, false, params ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureSampleProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    spv::Id lod = 0,
    spv::Id bias = 0,
    spv::Id offset = 0,
    spv::Id gx = 0,
    spv::Id gy = 0,
    spv::Id dref = 0 )
{
    static_assert (
        ViewT::MULTISAMPLED == false,
        "TextureProj does not support multisampled images" );

    static_assert (
        ViewT::ARRAYED == false,
        "TextureProj does not support arrayed images" );

    static_assert (
        ViewT::UNNORM == false,
        "TextureProj does not support unnormalized sampled images" );

    static_assert (
        ViewT::DIM == spv::Dim1D
        || ViewT::DIM == spv::Dim2D
        || ViewT::DIM == spv::Dim3D
        || ViewT::DIM == spv::DimRect,
        "TextureProj does not support this image type" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.lod = lod;
    params.bias = bias;
    params.offset = offset;
    params.gradX = gx;
    params.gradY = gy;
    params.Dref = dref;

    return TRVector< ScalarT, 4 > ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, true, false, false, params ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureSampleDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    spv::Id lod = 0,
    spv::Id bias = 0,
    spv::Id offset = 0,
    spv::Id gx = 0,
    spv::Id gy = 0,
    spv::Id dref = 0 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.lod = lod;
    params.bias = bias;
    params.offset = offset;
    params.gradX = gx;
    params.gradY = gy;
    params.Dref = dref;

    return ScalarT ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, false, false, false, params ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureSampleProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    spv::Id lod = 0,
    spv::Id bias = 0,
    spv::Id offset = 0,
    spv::Id gx = 0,
    spv::Id gy = 0,
    spv::Id dref = 0 )
{
    static_assert (
        ViewT::MULTISAMPLED == false,
        "TextureProjDref does not support multisampled images" );

    static_assert (
        ViewT::ARRAYED == false,
        "TextureProjDref does not support arrayed images" );

    static_assert (
        ViewT::UNNORM == false,
        "TextureProjDref does not support unnormalized sampled images" );

    static_assert (
        ViewT::DIM == spv::Dim1D
        || ViewT::DIM == spv::Dim2D
        || ViewT::DIM == spv::Dim3D
        || ViewT::DIM == spv::DimRect,
        "TextureProjDref does not support this image type" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.lod = lod;
    params.bias = bias;
    params.offset = offset;
    params.gradX = gx;
    params.gradY = gy;
    params.Dref = dref;

    return ScalarT ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, true, false, false, params ) ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords )
{
    return detail::TextureSample ( img, coords );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& bias )
{
    static_assert (
        ViewT::MULTISAMPLED == false,
        "Texture with bias does not support multisampled images" );

    return detail::TextureSample ( img, coords, 0, bias.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy )
{
    static_assert (
        ViewT::MULTISAMPLED == false,
        "Texture with gradient does not support multisampled images" );

    return detail::TextureSample ( img, coords, 0, 0, 0, gx.id(), gy.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset )
{
    static_assert (
        ViewT::UNNORM == false,
        "Texture: Can't use offsets with unnormalized sampled images" );

    static_assert (
        ViewT::DIM != spv::DimCube,
        "Texture: Can't use offsets with cube maps" );

    return detail::TextureSample ( img, coords, 0, 0, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& bias )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    return detail::TextureSample ( img, coords, 0, bias.id(), offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > Texture (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "Texture with gradient does not support multisampled images" );

    return detail::TextureSample ( img, coords, 0, 0, offset.id(), gx.id(), gy.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureLod (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& lod )
{
    static_assert (
        ViewT::MULTISAMPLED == false,
        "Texture with gradient does not support multisampled images" );

    return detail::TextureSample ( img, coords, lod.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureLod (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& lod )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    static_assert (
        ViewT::MULTISAMPLED == false,
        "Texture with gradient does not support multisampled images" );

    return detail::TextureSample ( img, coords, lod.id(), 0, offset.id() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords )
{
    return detail::TextureSampleProj ( img, coords );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const Float& bias )
{
    return detail::TextureSampleProj ( img, coords, 0, bias.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy )
{
    return detail::TextureSampleProj ( img, coords, 0, 0, 0, gx.id(), gy.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset )
{
    return detail::TextureSampleProj ( img, coords, 0, 0, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& bias )
{
    return detail::TextureSampleProj ( img, coords, 0, bias.id(), offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy )
{
    return detail::TextureSampleProj ( img, coords, 0, 0, offset.id(), gx.id(), gy.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureLodProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const Float& lod )
{
    return detail::TextureSampleProj ( img, coords, lod.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureLodProj (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& lod )
{
    return detail::TextureSampleProj ( img, coords, lod.id(), 0, offset.id() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, 0, 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& bias,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, bias.id(), 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, 0, 0, gx.id(), gy.id(), dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, 0, offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& bias,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, bias.id(), offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, 0, 0, offset.id(), gx.id(), gy.id(), dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureLodDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& lod,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, lod.id(), 0, 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureLodDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& lod,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleDref ( img, coords, lod.id(), 0, offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, 0, 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const Float& bias,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, bias.id(), 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, 0, 0, gx.id(), gy.id(), dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, 0, offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& bias,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, bias.id(), offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gx,
    const typename TRSampledTexture< ViewT, ScalarT >::grad_type& gy,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, 0, 0, offset.id(), gx.id(), gy.id(), dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureLodProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const Float& lod,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, lod.id(), 0, 0, 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE ScalarT TextureLodProjDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::proj_coord_type& coords,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset,
    const Float& lod,
    const Float& dref )
{
    static_assert (
        ViewT::DEPTH,
        "Dref operations require image with depth information" );

    static_assert (
        ViewT::UNNORM == false,
        "Can't use Dref operations with unnormalized sampled images" );

    return detail::TextureSampleProjDref ( img, coords, lod.id(), 0, offset.id(), 0, 0, dref.id() );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGather (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Int& component,
    spv::Id offset = 0,
    spv::Id offsets = 0 )
{
    static_assert (
        ViewT::DIM == spv::Dim2D
        || ViewT::DIM == spv::DimCube
        || ViewT::DIM == spv::DimRect,
        "TextureGather supports only 2D images and cube maps" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.offset = offset;
    params.offsets = offsets;
    params.component = component.id();

    return TRVector< ScalarT, 4 > ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, false, true, false, params ) ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGatherDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    spv::Id dref,
    spv::Id offset = 0,
    spv::Id offsets = 0 )
{
    static_assert (
        ViewT::DIM == spv::Dim2D
        || ViewT::DIM == spv::DimCube
        || ViewT::DIM == spv::DimRect,
        "TextureGatherDref supports only 2D images and cube maps" );

    KShaderTranslator* pTranslator = KShaderTranslator::get();

    spv::Builder::TextureParameters params;
    std::memset ( & params, 0, sizeof ( params ) );

    params.sampler = img.id();
    params.coords = coords.id();
    params.offset = offset;
    params.offsets = offsets;
    params.Dref = dref;

    return TRVector< ScalarT, 4 > ( KId (
        pTranslator->createTextureCall ( 
            pTranslator->getPrecision(), TRVector< ScalarT, 4 >::getType(),
            false, false, false, true, false, params ) ) );
}

// -----------------------------------------------------------------------------

VPP_DLLAPI spv::Id makeOffsetArray ( const GatherOffsets& offsets );

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGather (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Int& component )
{
    return detail::TextureGather ( img, coords, component );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGather (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Int& component,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    return detail::TextureGather ( img, coords, component, offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGather (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Int& component,
    const GatherOffsets& offsets )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    return detail::TextureGather (
        img, coords, component, 0, detail::makeOffsetArray ( offsets ) );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGatherDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& dref )
{
    return detail::TextureGatherDref ( img, coords, dref.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGatherDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& dref,
    const typename TRSampledTexture< ViewT, ScalarT >::offset_type& offset )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    return detail::TextureGatherDref ( img, coords, dref.id(), offset.id() );
}

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
VPP_INLINE TRVector< ScalarT, 4 > TextureGatherDref (
    const TRSampledTexture< ViewT, ScalarT >& img,
    const typename TRSampledTexture< ViewT, ScalarT >::sample_coord_type& coords,
    const Float& dref,
    const GatherOffsets& offsets )
{
    static_assert (
        ViewT::UNNORM == false,
        "Can't use offsets with unnormalized sampled images" );

    return detail::TextureGatherDref (
        img, coords, dref.id(), 0, detail::makeOffsetArray ( offsets ) );
}

// -----------------------------------------------------------------------------
} // namespace impl
// -----------------------------------------------------------------------------

template< class TextureT, class SamplerT >
VPP_INLINE auto MakeSampledTexture ( const TextureT& texture, const SamplerT& sampler )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(SamplerT) rSampler = sampler;
    return impl::MakeSampledTexture ( rTexture, rSampler );
}

// -----------------------------------------------------------------------------

template< class TextureT >
VPP_INLINE auto ExtractSampledTexture ( const TextureT& texture )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    return impl::ExtractSampledTexture ( rTexture );
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE auto ImageSize ( const ImageT& image )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    return impl::ImageSize ( rImage );
}

// -----------------------------------------------------------------------------

template< class ImageT, class LodT >
VPP_INLINE auto TextureSize ( const ImageT& image, const LodT& lod )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(LodT) rLod = lod;
    return impl::TextureSize ( rImage, rLod );
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE auto ImageQuerySamples ( const ImageT& image )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    return impl::ImageQuerySamples ( rImage );
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE auto ImageQueryLevels ( const ImageT& image )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    return impl::ImageQueryLevels ( rImage );
}

// -----------------------------------------------------------------------------

template< class ImageT, class CoordsT >
VPP_INLINE auto TextureQueryLod ( const ImageT& image, const CoordsT& coords )
{
    const VPP_RVTYPE(ImageT) rImage = image; 
    const VPP_RVTYPE(CoordsT) rCoords = coords; 
    return impl::TextureQueryLod ( rImage, rCoords );
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE auto SubpassLoad ( const ImageT& image )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    return impl::SubpassLoad ( rImage );
}

// -----------------------------------------------------------------------------

template< class ImageT, class ArgT >
VPP_INLINE auto SubpassLoad ( const ImageT& image, const ArgT& arg )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(ArgT) rArg = arg;
    return impl::SubpassLoad ( rImage, rArg );
}

// -----------------------------------------------------------------------------

template< class ImageT, class Arg1T, class Arg2T >
VPP_INLINE auto SubpassLoad ( const ImageT& image, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::SubpassLoad ( rImage, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class ImageT, class ArgT >
VPP_INLINE auto ImageLoad ( const ImageT& image, const ArgT& arg )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(ArgT) rArg = arg;
    return impl::ImageLoad ( rImage, rArg );
}

// -----------------------------------------------------------------------------

template< class ImageT, class Arg1T, class Arg2T >
VPP_INLINE auto ImageLoad ( const ImageT& image, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::ImageLoad ( rImage, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class ImageT, class Arg1T, class Arg2T >
VPP_INLINE void ImageStore ( const ImageT& image, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    impl::ImageStore ( rImage, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class ImageT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE void ImageStore ( const ImageT& image, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(ImageT) rImage = image;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    impl::ImageStore ( rImage, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T >
VPP_INLINE auto TexelFetch ( const TextureT& texture, const Arg1T& arg1 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    return impl::TexelFetch ( rTexture, rArg1 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TexelFetch ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TexelFetch ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TexelFetchLod ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TexelFetchLod ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TexelFetchOffset ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TexelFetchOffset ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TexelFetchLodOffset ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TexelFetchLodOffset ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T >
VPP_INLINE auto Texture ( const TextureT& texture, const Arg1T& arg1 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    return impl::Texture ( rTexture, rArg1 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto Texture ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::Texture ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto Texture ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::Texture ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto Texture ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::Texture ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureLod ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureLod ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureLod ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureLod ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T >
VPP_INLINE auto TextureProj ( const TextureT& texture, const Arg1T& arg1 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    return impl::TextureProj ( rTexture, rArg1 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureProj ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureProj ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureProj ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureProj ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto TextureProj ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::TextureProj ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureLodProj ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureLodProj ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureLodProj ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureLodProj ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureDref ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureDref ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::TextureDref ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T >
VPP_INLINE auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4, const Arg5T& arg5 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    const VPP_RVTYPE(Arg5T) rArg5 = arg5;
    return impl::TextureDref ( rTexture, rArg1, rArg2, rArg3, rArg4, rArg5 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureLodDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureLodDref ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto TextureLodDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::TextureLodDref ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureProjDref ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureProjDref ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::TextureProjDref ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T >
VPP_INLINE auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4, const Arg5T& arg5 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    const VPP_RVTYPE(Arg5T) rArg5 = arg5;
    return impl::TextureProjDref ( rTexture, rArg1, rArg2, rArg3, rArg4, rArg5 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureLodProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureLodProjDref ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
VPP_INLINE auto TextureLodProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    const VPP_RVTYPE(Arg4T) rArg4 = arg4;
    return impl::TextureLodProjDref ( rTexture, rArg1, rArg2, rArg3, rArg4 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureGather ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureGather ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureGather ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureGather ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T >
VPP_INLINE auto TextureGatherDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    return impl::TextureGatherDref ( rTexture, rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto TextureGatherDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const VPP_RVTYPE(TextureT) rTexture = texture;
    const VPP_RVTYPE(Arg1T) rArg1 = arg1;
    const VPP_RVTYPE(Arg2T) rArg2 = arg2;
    const VPP_RVTYPE(Arg3T) rArg3 = arg3;
    return impl::TextureGatherDref ( rTexture, rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHADERLANGUAGEIMGFUN_HPP
