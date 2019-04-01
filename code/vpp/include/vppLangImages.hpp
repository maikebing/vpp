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

#ifndef INC_VPPLANGIMAGES_HPP
#define INC_VPPLANGIMAGES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGVECTORTYPES_HPP
#include "vppLangVectorTypes.hpp"
#endif

#ifndef INC_VPPIMAGEVIEW_HPP
#include "vppImageView.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum EImageTypeFlags
{
    IMG_DEPTH = 1
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
class TRImage : public KValue
{
public:
    static const spv::Dim DIM = ViewT::DIM;
    static const bool ARRAYED = ViewT::ARRAYED;
    static const bool MULTISAMPLED = ViewT::MULTISAMPLED;
    static const bool DEPTH = ViewT::DEPTH;
    typedef typename ViewT::size_type size_type;
    typedef typename ViewT::coord_type coord_type;
    typedef typename ViewT::bcoord_type bcoord_type;
    typedef ScalarT scalar_type;
    typedef TRImage< ViewT, ScalarT > rvalue_type;

    VPP_INLINE TRImage() :
        KValue ( KId ( 0 ) ),
        d_type ( 0 )
    {
    }

    VPP_INLINE TRImage (
        const KId& valueId,
        const KId& typeId ) :
            KValue ( valueId ),
            d_type ( typeId )
    {
    }

    VPP_INLINE KId type() const
    {
        return d_type;
    }

private:
    KId d_type;
};

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, spv::ImageFormat FORMAT >
class TTypedImage : public TRImage< ViewT, ScalarT >
{
public:
    typedef ScalarT scalar_type;
    typedef TRImage< ViewT, ScalarT > image_type;

    VPP_INLINE TTypedImage ( const KId& id ) :
        TRImage< ViewT, ScalarT >( id, getType() )
    {}

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId ( pTranslator->makeImageType (
            scalar_type::getType(),
            ViewT::DIM,
            ViewT::DEPTH,
            ViewT::ARRAYED,
            ViewT::MULTISAMPLED,
            2,
            pTranslator->validateImageFormat ( FORMAT ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRImage< KImageViewTag1D, Float > RImage1D;
typedef TRImage< KImageViewTagArray1D, Float > RImageArray1D;
typedef TRImage< KImageViewTag2D, Float > RImage2D;
typedef TRImage< KImageViewTagArray2D, Float > RImageArray2D;
typedef TRImage< KImageViewTagMultisampled2D, Float > RImageMultisampled2D;
typedef TRImage< KImageViewTagMultisampledArray2D, Float > RImageMultisampledArray2D;
typedef TRImage< KImageViewTagCube2D, Float > RImageCube2D;
typedef TRImage< KImageViewTagCubeArray2D, Float > RImageCubeArray2D;
typedef TRImage< KImageViewTag3D, Float > RImage3D;

typedef TRImage< KImageViewTag1D, Int > RIImage1D;
typedef TRImage< KImageViewTagArray1D, Int > RIImageArray1D;
typedef TRImage< KImageViewTag2D, Int > RIImage2D;
typedef TRImage< KImageViewTagArray2D, Int > RIImageArray2D;
typedef TRImage< KImageViewTagMultisampled2D, Int > RIImageMultisampled2D;
typedef TRImage< KImageViewTagMultisampledArray2D, Int > RIImageMultisampledArray2D;
typedef TRImage< KImageViewTagCube2D, Int > RIImageCube2D;
typedef TRImage< KImageViewTagCubeArray2D, Int > RIImageCubeArray2D;
typedef TRImage< KImageViewTag3D, Int > RIImage3D;

typedef TRImage< KImageViewTag1D, UInt > RUImage1D;
typedef TRImage< KImageViewTagArray1D, UInt > RUImageArray1D;
typedef TRImage< KImageViewTag2D, UInt > RUImage2D;
typedef TRImage< KImageViewTagArray2D, UInt > RUImageArray2D;
typedef TRImage< KImageViewTagMultisampled2D, UInt > RUImageMultisampled2D;
typedef TRImage< KImageViewTagMultisampledArray2D, UInt > RUImageMultisampledArray2D;
typedef TRImage< KImageViewTagCube2D, UInt > RUImageCube2D;
typedef TRImage< KImageViewTagCubeArray2D, UInt > RUImageCubeArray2D;
typedef TRImage< KImageViewTag3D, UInt > RUImage3D;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
class TRTexture : public KValue
{
public:
    static const spv::Dim DIM = ViewT::DIM;
    static const bool ARRAYED = ViewT::ARRAYED;
    static const bool MULTISAMPLED = ViewT::MULTISAMPLED;
    static const bool DEPTH = ViewT::DEPTH;
    typedef typename ViewT::size_type size_type;
    typedef typename ViewT::coord_type coord_type;
    typedef typename ViewT::bcoord_type bcoord_type;
    typedef ScalarT scalar_type;
    typedef TRTexture< ViewT, ScalarT > rvalue_type;

    VPP_INLINE TRTexture() :
        KValue ( KId ( 0 ) ),
        d_type ( 0 )
    {
    }

    VPP_INLINE TRTexture (
        const KId& valueId,
        const KId& typeId ) :
            KValue ( valueId ),
            d_type ( typeId )
    {
    }

    VPP_INLINE KId type() const
    {
        return d_type;
    }

private:
    KId d_type;
};

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, spv::ImageFormat FORMAT >
class TTypedTexture : public TRTexture< ViewT, ScalarT >
{
public:
    typedef ScalarT scalar_type;
    typedef TRTexture< ViewT, ScalarT > image_type;

    VPP_INLINE TTypedTexture ( const KId& id ) :
        TRTexture< ViewT, ScalarT >( id, getType() )
    {}

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        // FIXME - here false is being forced into depth parameter because
        // crappy nvidia driver barfs when trying to sample "depth" marked
        // image with ImageSample or ImageFetch - probably they work only
        // for Dref ? This is to be figured out. For now, just pretend these
        // are plain textures, not "depth".

        return KId ( pTranslator->makeImageType (
            scalar_type::getType(),
            ViewT::DIM,
            /*ViewT::DEPTH,*/ false,
            ViewT::ARRAYED,
            ViewT::MULTISAMPLED,
            1,
            pTranslator->validateImageFormat ( FORMAT ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRTexture< KImageViewTag1D, Float > RTexture1D;
typedef TRTexture< KImageViewTagArray1D, Float > RTextureArray1D;
typedef TRTexture< KImageViewTag2D, Float > RTexture2D;
typedef TRTexture< KImageViewTagArray2D, Float > RTextureArray2D;
typedef TRTexture< KImageViewTagMultisampled2D, Float > RTextureMultisampled2D;
typedef TRTexture< KImageViewTagMultisampledArray2D, Float > RTextureMultisampledArray2D;
typedef TRTexture< KImageViewTagCube2D, Float > RTextureCube2D;
typedef TRTexture< KImageViewTagCubeArray2D, Float > RTextureCubeArray2D;
typedef TRTexture< KImageViewTag3D, Float > RTexture3D;

typedef TRTexture< KImageViewTag1D, Int > RITexture1D;
typedef TRTexture< KImageViewTagArray1D, Int > RITextureArray1D;
typedef TRTexture< KImageViewTag2D, Int > RITexture2D;
typedef TRTexture< KImageViewTagArray2D, Int > RITextureArray2D;
typedef TRTexture< KImageViewTagMultisampled2D, Int > RITextureMultisampled2D;
typedef TRTexture< KImageViewTagMultisampledArray2D, Int > RITextureMultisampledArray2D;
typedef TRTexture< KImageViewTagCube2D, Int > RITextureCube2D;
typedef TRTexture< KImageViewTagCubeArray2D, Int > RITextureCubeArray2D;
typedef TRTexture< KImageViewTag3D, Int > RITexture3D;

typedef TRTexture< KImageViewTag1D, UInt > RUTexture1D;
typedef TRTexture< KImageViewTagArray1D, UInt > RUTextureArray1D;
typedef TRTexture< KImageViewTag2D, UInt > RUTexture2D;
typedef TRTexture< KImageViewTagArray2D, UInt > RUTextureArray2D;
typedef TRTexture< KImageViewTagMultisampled2D, UInt > RUTextureMultisampled2D;
typedef TRTexture< KImageViewTagMultisampledArray2D, UInt > RUTextureMultisampledArray2D;
typedef TRTexture< KImageViewTagCube2D, UInt > RUTextureCube2D;
typedef TRTexture< KImageViewTagCubeArray2D, UInt > RUTextureCubeArray2D;
typedef TRTexture< KImageViewTag3D, UInt > RUTexture3D;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
class TRSampledTexture : public KValue
{
public:
    static const spv::Dim DIM = ViewT::DIM;
    static const bool ARRAYED = ViewT::ARRAYED;
    static const bool MULTISAMPLED = ViewT::MULTISAMPLED;
    static const bool DEPTH = ViewT::DEPTH;
    typedef typename ViewT::size_type size_type;
    typedef typename ViewT::coord_type coord_type;
    typedef typename ViewT::bcoord_type bcoord_type;
    typedef typename ViewT::offset_type offset_type;
    typedef typename ViewT::proj_coord_type proj_coord_type;
    typedef typename ViewT::sample_coord_type sample_coord_type;
    typedef typename ViewT::grad_type grad_type;
    typedef ScalarT scalar_type;
    typedef TRSampledTexture< ViewT, ScalarT > rvalue_type;

    VPP_INLINE TRSampledTexture() :
        KValue ( KId ( 0 ) ),
        d_type ( 0 )
    {
    }

    VPP_INLINE TRSampledTexture (
        const KId& valueId,
        const KId& typeId ) :
            KValue ( valueId ),
            d_type ( typeId )
    {
    }

    VPP_INLINE KId type() const
    {
        return d_type;
    }

private:
    KId d_type;
};

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, spv::ImageFormat FORMAT >
class TTypedSampledTexture : public TRSampledTexture< ViewT, ScalarT >
{
public:
    typedef ScalarT scalar_type;
    typedef TRSampledTexture< ViewT, ScalarT > image_type;

    VPP_INLINE TTypedSampledTexture ( const KId& id ) :
        TRSampledTexture< ViewT, ScalarT >( id, getType() )
    {}

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId ( pTranslator->makeSampledImageType ( 
            pTranslator->makeImageType (
                scalar_type::getType(),
                ViewT::DIM,
                ViewT::DEPTH,
                ViewT::ARRAYED,
                ViewT::MULTISAMPLED,
                1,
                pTranslator->validateImageFormat ( FORMAT )
        ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRSampledTexture< KImageViewTag1D, Float > RSampledTexture1D;
typedef TRSampledTexture< KImageViewTagArray1D, Float > RSampledTextureArray1D;
typedef TRSampledTexture< KImageViewTag2D, Float > RSampledTexture2D;
typedef TRSampledTexture< KImageViewTagArray2D, Float > RSampledTextureArray2D;
typedef TRSampledTexture< KImageViewTagMultisampled2D, Float > RSampledTextureMultisampled2D;
typedef TRSampledTexture< KImageViewTagMultisampledArray2D, Float > RSampledTextureMultisampledArray2D;
typedef TRSampledTexture< KImageViewTagCube2D, Float > RSampledTextureCube2D;
typedef TRSampledTexture< KImageViewTagCubeArray2D, Float > RSampledTextureCubeArray2D;
typedef TRSampledTexture< KImageViewTag3D, Float > RSampledTexture3D;

typedef TRSampledTexture< KImageViewTag1D, Int > RISampledTexture1D;
typedef TRSampledTexture< KImageViewTagArray1D, Int > RISampledTextureArray1D;
typedef TRSampledTexture< KImageViewTag2D, Int > RISampledTexture2D;
typedef TRSampledTexture< KImageViewTagArray2D, Int > RISampledTextureArray2D;
typedef TRSampledTexture< KImageViewTagMultisampled2D, Int > RISampledTextureMultisampled2D;
typedef TRSampledTexture< KImageViewTagMultisampledArray2D, Int > RISampledTextureMultisampledArray2D;
typedef TRSampledTexture< KImageViewTagCube2D, Int > RISampledTextureCube2D;
typedef TRSampledTexture< KImageViewTagCubeArray2D, Int > RISampledTextureCubeArray2D;
typedef TRSampledTexture< KImageViewTag3D, Int > RISampledTexture3D;

typedef TRSampledTexture< KImageViewTag1D, UInt > RUSampledTexture1D;
typedef TRSampledTexture< KImageViewTagArray1D, UInt > RUSampledTextureArray1D;
typedef TRSampledTexture< KImageViewTag2D, UInt > RUSampledTexture2D;
typedef TRSampledTexture< KImageViewTagArray2D, UInt > RUSampledTextureArray2D;
typedef TRSampledTexture< KImageViewTagMultisampled2D, UInt > RUSampledTextureMultisampled2D;
typedef TRSampledTexture< KImageViewTagMultisampledArray2D, UInt > RUSampledTextureMultisampledArray2D;
typedef TRSampledTexture< KImageViewTagCube2D, UInt > RUSampledTextureCube2D;
typedef TRSampledTexture< KImageViewTagCubeArray2D, UInt > RUSampledTextureCubeArray2D;
typedef TRSampledTexture< KImageViewTag3D, UInt > RUSampledTexture3D;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT >
class TRInput : public KValue
{
public:
    static const spv::Dim DIM = spv::DimSubpassData;
    static const bool ARRAYED = ViewT::ARRAYED;
    static const bool MULTISAMPLED = ViewT::MULTISAMPLED;
    static const bool DEPTH = ViewT::DEPTH;
    typedef typename ViewT::size_type size_type;
    typedef typename ViewT::coord_type coord_type;
    typedef typename ViewT::bcoord_type bcoord_type;
    typedef ScalarT scalar_type;
    typedef TRInput< ViewT, ScalarT > rvalue_type;

    VPP_INLINE TRInput() :
        KValue ( KId ( 0 ) ),
        d_type ( 0 )
    {
    }

    VPP_INLINE TRInput (
        const KId& valueId,
        const KId& typeId ) :
            KValue ( valueId ),
            d_type ( typeId )
    {
    }

    VPP_INLINE KId type() const
    {
        return d_type;
    }

private:
    KId d_type;
};

// -----------------------------------------------------------------------------

template< class ViewT, class ScalarT, spv::ImageFormat FORMAT >
class TTypedInput : public TRInput< ViewT, ScalarT >
{
public:
    typedef ScalarT scalar_type;
    typedef TRInput< ViewT, ScalarT > image_type;

    VPP_INLINE TTypedInput ( const KId& id ) :
        TRInput< ViewT, ScalarT >( id, getType() )
    {}

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        return KId ( pTranslator->makeImageType (
            scalar_type::getType(),
            spv::DimSubpassData,
            ViewT::DEPTH,
            ViewT::ARRAYED,
            ViewT::MULTISAMPLED,
            2,
            spv::ImageFormatUnknown ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRInput< KImageViewTag2D, Float > RInput2D;
typedef TRInput< KImageViewTagArray2D, Float > RInputArray2D;
typedef TRInput< KImageViewTagMultisampled2D, Float > RInputMultisampled2D;
typedef TRInput< KImageViewTagMultisampledArray2D, Float > RInputMultisampledArray2D;

typedef TRInput< KImageViewTag2D, Int > RIInput2D;
typedef TRInput< KImageViewTagArray2D, Int > RIInputArray2D;
typedef TRInput< KImageViewTagMultisampled2D, Int > RIInputMultisampled2D;
typedef TRInput< KImageViewTagMultisampledArray2D, Int > RIInputMultisampledArray2D;

typedef TRInput< KImageViewTag2D, UInt > RUInput2D;
typedef TRInput< KImageViewTagArray2D, UInt > RUInputArray2D;
typedef TRInput< KImageViewTagMultisampled2D, UInt > RUInputMultisampled2D;
typedef TRInput< KImageViewTagMultisampledArray2D, UInt > RUInputMultisampledArray2D;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< bool UNNORM >
class TRSampler : public KValue
{
public:
    typedef TRSampler< UNNORM > rvalue_type;

    VPP_INLINE TRSampler() :
        KValue ( KId ( 0 ) )
    {
    }

    VPP_INLINE TRSampler ( const KId& valueId ) :
        KValue ( valueId )
    {
    }

    static VPP_INLINE KId getType()
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        return KId ( pTranslator->makeSamplerType() );
    }
};

// -----------------------------------------------------------------------------

typedef TRSampler< false > RNormSampler;
typedef TRSampler< true > RUnnormSampler;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGIMAGES_HPP
