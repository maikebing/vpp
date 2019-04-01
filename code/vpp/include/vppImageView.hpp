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

#ifndef INC_VPPIMAGEVIEW_HPP
#define INC_VPPIMAGEVIEW_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPIMAGE_HPP
#include "vppImage.hpp"
#endif

#ifndef INC_VPPFORMATS_HPP
#include "vppFormats.hpp"
#endif

#ifndef INC_VPPLANGVECTORTYPES_HPP
#include "vppLangVectorTypes.hpp"
#endif

#ifndef INC_VPPSAMPLER_HPP
#include "vppSampler.hpp"
#endif

#ifndef INC_VPPFRAMEIMAGEVIEW_HPP
#include "vppFrameImageView.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< class AttributesT >
class TImageViewImpl;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum ESamplingMode
{
    SAMPLER_NORMALIZED,
    SAMPLER_UNNORMALIZED
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< unsigned int DIM, typename NativeScalarT >
struct TDefineShaderVector
{
    typedef TRVector<
        typename TDefineShaderVector< 1u, NativeScalarT >::type,
        DIM
    > type;
};

template< typename NativeScalarT >
struct TDefineShaderVector< 1u, NativeScalarT >
{
    typedef TRValue< NativeScalarT > type;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template<
    class ImageT,
    ESamplingMode SAMPLING = SAMPLER_NORMALIZED,
    unsigned int ASPECTS = ImageT::format_type::aspects,
    class FormatT = typename ImageT::format_type >
struct ImageViewAttributes
{
    typedef ImageT image_type;
    typedef FormatT format_type;
    typedef typename ImageT::attributes_type image_attr_type;
    static const VkFormat format_code = FormatT::code;
    static const spv::ImageFormat spv_code = FormatT::spvCode;

    static const unsigned int aspects = ASPECTS;
    static const ESamplingMode samplingMode = SAMPLING;

    static const EImageType imgType = image_attr_type::type;
    static const bool isImgArrayed = image_attr_type::isArrayed;

    static const bool isUnnormSampling = ( samplingMode == SAMPLER_UNNORMALIZED );

    static const VkImageViewType type = (
        imgType == IMG_TYPE_3D ?
            VK_IMAGE_VIEW_TYPE_3D
            : ( imgType == IMG_TYPE_1D ?
                ( isImgArrayed ? VK_IMAGE_VIEW_TYPE_1D_ARRAY : VK_IMAGE_VIEW_TYPE_1D )
                : ( imgType == IMG_TYPE_CUBE_2D ? 
                    ( isImgArrayed ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE )
                    : ( isImgArrayed ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D ) ) )
    );

    static const bool isMultisampled = image_attr_type::isMultisampled;
    
    static const bool isArrayed = 
        ( type == VK_IMAGE_VIEW_TYPE_1D_ARRAY )
        || ( type == VK_IMAGE_VIEW_TYPE_2D_ARRAY )
        || ( type == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );

    static const bool isDepth =
        ( aspects & VK_IMAGE_ASPECT_DEPTH_BIT ) != 0;

    static const bool isMultisamplingAllowed =
        ( type == VK_IMAGE_VIEW_TYPE_2D )
        || ( type == VK_IMAGE_VIEW_TYPE_2D_ARRAY );

    static const bool isCube =
        ( type == VK_IMAGE_VIEW_TYPE_CUBE )
        || ( type == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );

    static const spv::Dim spvDim =
        ( imgType == IMG_TYPE_1D ? spv::Dim1D
            : ( imgType == IMG_TYPE_3D ? spv::Dim3D
                : ( isCube ? spv::DimCube : spv::Dim2D ) ) );

    static_assert (
        ! isMultisampled || isMultisamplingAllowed,
        "Multisampling is not allowed for this type of view" );

    static_assert (
        ! isUnnormSampling || ( type == VK_IMAGE_VIEW_TYPE_1D || type == VK_IMAGE_VIEW_TYPE_2D ),
        "Unnormalized sampler requires simple 1D or 2D image view" );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< spv::Dim _DIM, bool _ARRAYED, bool _MULTISAMPLED >
struct TValidImageViewTag { static const bool isValid = false; };

template<> struct TValidImageViewTag< spv::Dim1D, false, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim1D, true, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim2D, false, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim2D, false, true >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim2D, true, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim2D, true, true >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::DimCube, false, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::DimCube, true, false >
{ static const bool isValid = true; };

template<> struct TValidImageViewTag< spv::Dim3D, false, false >
{ static const bool isValid = true; };

// -----------------------------------------------------------------------------

template< spv::Dim _DIM, bool _ARRAYED, bool _MULTISAMPLED, bool _DEPTH, bool _UNNORM >
struct TImageViewTag
{
    static const bool isValid =
        TValidImageViewTag< _DIM, _ARRAYED, _MULTISAMPLED >::isValid;

    static const spv::Dim DIM = _DIM;
    static const bool ARRAYED = _ARRAYED;
    static const bool MULTISAMPLED = _MULTISAMPLED;
    static const bool DEPTH = _DEPTH;
    static const bool CUBE = ( DIM == spv::DimCube );
    static const bool UNNORM = _UNNORM;

    static const bool isUnnormSampling = _UNNORM;

    static const unsigned int baseDim = (
        DIM == spv::Dim3D ? 3u : ( DIM == spv::Dim1D ? 1u : 2u ) );

    static const unsigned int sizeDim = baseDim + ARRAYED;
    static const unsigned int coordDim = sizeDim;
    static const unsigned int offsetDim = baseDim;
    static const unsigned int sampleCoordDim = coordDim + CUBE;
    static const unsigned int projCoordDim = offsetDim + 1u;
    static const unsigned int gradCoordDim = offsetDim + CUBE;

    typedef typename TDefineShaderVector< sizeDim, int >::type size_type;
    typedef typename TDefineShaderVector< coordDim, float >::type coord_type;
    typedef typename TDefineShaderVector< baseDim, float >::type bcoord_type;
    typedef typename TDefineShaderVector< offsetDim, int >::type offset_type;
    typedef typename TDefineShaderVector< sampleCoordDim, float >::type sample_coord_type;
    typedef typename TDefineShaderVector< projCoordDim, float >::type proj_coord_type;
    typedef typename TDefineShaderVector< gradCoordDim, float >::type grad_type;
    typedef typename TDefineShaderVector< sampleCoordDim, int >::type pointer_coord_type;
};

// -----------------------------------------------------------------------------

typedef TImageViewTag< spv::Dim1D, false, false, false, false > KImageViewTag1D;
typedef TImageViewTag< spv::Dim1D, true, false, false, false > KImageViewTagArray1D;
typedef TImageViewTag< spv::Dim2D, false, false, false, false > KImageViewTag2D;
typedef TImageViewTag< spv::Dim2D, true, false, false, false > KImageViewTagArray2D;
typedef TImageViewTag< spv::Dim2D, false, true, false, false > KImageViewTagMultisampled2D;
typedef TImageViewTag< spv::Dim2D, true, true, false, false > KImageViewTagMultisampledArray2D;
typedef TImageViewTag< spv::DimCube, false, false, false, false > KImageViewTagCube2D;
typedef TImageViewTag< spv::DimCube, true, false, false, false > KImageViewTagCubeArray2D;
typedef TImageViewTag< spv::Dim3D, false, false, false, false > KImageViewTag3D;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class AttributesT >
class ImageView : public vpp::TSharedReference< TImageViewImpl< AttributesT > >
{
public:
    typedef typename AttributesT::image_type image_type;
    typedef typename AttributesT::format_type format_type;
    typedef AttributesT attributes_type;

    typedef TImageViewTag<
        AttributesT::spvDim,
        AttributesT::isArrayed,
        AttributesT::isMultisampled,
        AttributesT::isDepth,
        AttributesT::isUnnormSampling > image_tag_type;

    static const bool isUnnormSampling = AttributesT::isUnnormSampling;

    ImageView();
    explicit ImageView ( const image_type& hImage );
    ImageView ( const image_type& hImage, std::uint32_t layer );
    ImageView ( const image_type& hImage, std::uint32_t mipLevel, std::uint32_t layer );
    explicit ImageView ( const FrameImageView& hGenericView );

    ImageView (
        const image_type& hImage,
        std::uint32_t baseMipLevel,
        std::uint32_t levelCount,
        std::uint32_t baseArrayLayer,
        std::uint32_t layerCount,
        const VkComponentMapping& componentMapping = ComponentMapping()
    );

    operator VkImageView() const;
    const image_type& image() const;
    const Device& device() const;

    const VkImageSubresourceRange& subresourceRange() const;
    FrameImageView frameView() const;

    static_assert (
        image_tag_type::isValid,
        "Invalid view attributes combination" );
};

// -----------------------------------------------------------------------------

template< class AttributesT >
class TImageViewImpl : public vpp::TSharedObject< TImageViewImpl< AttributesT > >
{
public:
    typedef typename AttributesT::image_type image_type;

    VPP_INLINE TImageViewImpl (
        image_type hImage,
        const VkComponentMapping& componentMapping,
        std::uint32_t baseMipLevel,
        std::uint32_t levelCount,
        std::uint32_t baseArrayLayer,
        std::uint32_t layerCount ) :
            d_hImage ( hImage ),
            d_hDevice ( hImage.device() ),
            d_handle()
    {
        if ( ! hImage || ! hImage.valid() )
        {
            KExceptionThrower exceptionThrower;
            exceptionThrower.raiseInvalidImage();
        }

        if ( ! hImage.getMemory() || ! hImage.getMemory().valid() )
        {
            KExceptionThrower exceptionThrower;
            exceptionThrower.raiseImageNotBound();
        }

        if ( levelCount == VK_REMAINING_MIP_LEVELS )
            levelCount = hImage.info().mipLevels - baseMipLevel;

        if ( layerCount == VK_REMAINING_ARRAY_LAYERS )
            layerCount = hImage.info().arrayLayers - baseArrayLayer;

        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = 0;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = hImage.handle();
        imageViewCreateInfo.viewType = AttributesT::type;

        if ( AttributesT::format_code == VK_FORMAT_UNDEFINED )
            imageViewCreateInfo.format = hImage.format();
        else
            imageViewCreateInfo.format = AttributesT::format_code;

        imageViewCreateInfo.subresourceRange.aspectMask = AttributesT::aspects;
        imageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
        imageViewCreateInfo.subresourceRange.levelCount = levelCount;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        imageViewCreateInfo.subresourceRange.layerCount = layerCount;
        imageViewCreateInfo.components = componentMapping;

        d_subresourceRange = imageViewCreateInfo.subresourceRange;

        ::vkCreateImageView (
            d_hDevice.handle(), & imageViewCreateInfo, 0, & d_handle );
    }

    VPP_INLINE TImageViewImpl (
        const Img& hImage,
        const VkComponentMapping& componentMapping,
        std::uint32_t baseMipLevel,
        std::uint32_t levelCount,
        std::uint32_t baseArrayLayer,
        std::uint32_t layerCount ) :
            d_hImage ( hImage ),
            d_hDevice ( hImage.device() ),
            d_handle()
    {
        if ( ! hImage || ! hImage.valid() )
        {
            KExceptionThrower exceptionThrower;
            exceptionThrower.raiseInvalidImage();
        }

        if ( ! hImage.getMemory() || ! hImage.getMemory().valid() )
        {
            KExceptionThrower exceptionThrower;
            exceptionThrower.raiseImageNotBound();
        }

        if ( levelCount == VK_REMAINING_MIP_LEVELS )
            levelCount = hImage.info().mipLevels - baseMipLevel;

        if ( layerCount == VK_REMAINING_ARRAY_LAYERS )
            layerCount = hImage.info().arrayLayers - baseArrayLayer;

        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = 0;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = hImage.handle();
        imageViewCreateInfo.viewType = AttributesT::type;

        if ( AttributesT::format_code == VK_FORMAT_UNDEFINED )
            imageViewCreateInfo.format = hImage.format();
        else
            imageViewCreateInfo.format = AttributesT::format_code;

        imageViewCreateInfo.subresourceRange.aspectMask = AttributesT::aspects;
        imageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
        imageViewCreateInfo.subresourceRange.levelCount = levelCount;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        imageViewCreateInfo.subresourceRange.layerCount = layerCount;
        imageViewCreateInfo.components = componentMapping;

        d_subresourceRange = imageViewCreateInfo.subresourceRange;

        ::vkCreateImageView (
            d_hDevice.handle(), & imageViewCreateInfo, 0, & d_handle );
    }

    VPP_INLINE ~TImageViewImpl()
    {
        ::vkDestroyImageView ( d_hDevice.handle(), d_handle, 0 );
    }

private:
    friend class ImageView< AttributesT >;
    image_type d_hImage;
    Device d_hDevice;
    VkImageView d_handle;

    VkImageSubresourceRange d_subresourceRange;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView()
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView (
    const typename AttributesT::image_type& hImage ) :
        TSharedReference< TImageViewImpl< AttributesT > > (
            new TImageViewImpl< AttributesT >(
                hImage,
                ComponentMapping(),
                0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS ) )
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView (
    const typename AttributesT::image_type& hImage, std::uint32_t layer ) :
        TSharedReference< TImageViewImpl< AttributesT > > (
            new TImageViewImpl< AttributesT >(
                hImage,
                ComponentMapping(),
                0, VK_REMAINING_MIP_LEVELS, layer, 1 ) )
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView (
    const typename AttributesT::image_type& hImage,
    std::uint32_t mipLevel, std::uint32_t layer ) :
        TSharedReference< TImageViewImpl< AttributesT > > (
            new TImageViewImpl< AttributesT >(
                hImage,
                ComponentMapping(),
                mipLevel, 1, layer, 1 ) )
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView ( const FrameImageView& hGenericView ) :
    TSharedReference< TImageViewImpl< AttributesT > > (
        new TImageViewImpl< AttributesT >(
            hGenericView.image(),
            ComponentMapping(),
            0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS ) )
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: ImageView (
    const image_type& hImage,
    std::uint32_t baseMipLevel,
    std::uint32_t levelCount,
    std::uint32_t baseArrayLayer,
    std::uint32_t layerCount,
    const VkComponentMapping& componentMapping ) :
        TSharedReference< TImageViewImpl< AttributesT > > (
            new TImageViewImpl< AttributesT >(
                hImage,
                componentMapping,
                baseMipLevel,
                levelCount,
                baseArrayLayer,
                layerCount ) )
{
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE ImageView< AttributesT > :: operator VkImageView() const
{
    return this->get()->d_handle;
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE const typename AttributesT::image_type& ImageView< AttributesT > :: image() const
{
    return this->get()->d_hImage;
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE const Device& ImageView< AttributesT > :: device() const
{
    return this->get()->d_hDevice;
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE const VkImageSubresourceRange& ImageView< AttributesT > :: subresourceRange() const
{
    return this->get()->d_subresourceRange;
}

// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE FrameImageView ImageView< AttributesT > :: frameView() const
{
    return FrameImageView ( image(), subresourceRange() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ImageViewT >
class TImageBinding
{
public:
    VPP_INLINE TImageBinding (
        const ImageViewT& view,
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED ) :
            d_view ( view ),
            d_layout ( layout )
    {}

    VPP_INLINE const ImageViewT& view() const
    {
        return d_view;
    }

    VPP_INLINE VkImageLayout layout() const
    {
        return d_layout;
    }

    ImageViewT d_view;
    VkImageLayout d_layout;
};

// -----------------------------------------------------------------------------

template< class ImageViewT >
class TImageSamplerBinding
{
public:
    VPP_INLINE TImageSamplerBinding (
        const ImageViewT& view,
        const NormalizedSampler& sampler,
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED ) :
            d_view ( view ),
            d_sampler ( sampler ),
            d_layout ( layout )
    {
    }

    VPP_INLINE TImageSamplerBinding (
        const ImageViewT& view,
        const UnnormalizedSampler& sampler,
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED ) :
            d_view ( view ),
            d_sampler ( sampler ),
            d_layout ( layout )
    {
        static_assert (
            ImageViewT::isUnnormSampling,
            "Unnormalized sampler can't be bound to normalized-only sampling view" );
    }

    VPP_INLINE const ImageViewT& view() const
    {
        return d_view;
    }

    VPP_INLINE const Sampler& sampler() const
    {
        return d_sampler;
    }

    VPP_INLINE VkImageLayout layout() const
    {
        return d_layout;
    }

    ImageViewT d_view;
    Sampler d_sampler;
    VkImageLayout d_layout;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class AttributesT >
VPP_INLINE TImageBinding< ImageView< AttributesT > > bind (
    const ImageView< AttributesT >& view,
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED )
{
    // Default undefined layout will be later coerced into proper layout.
    return TImageBinding< ImageView< AttributesT > >( view, layout );
}

// -----------------------------------------------------------------------------

template< class AttributesT, class SamplerT >
VPP_INLINE TImageSamplerBinding< ImageView< AttributesT > > bind (
    const SamplerT& sampler,
    const ImageView< AttributesT >& view,
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED )
{
    // Default undefined layout will be later coerced into proper layout.
    return TImageSamplerBinding< ImageView< AttributesT > >( view, sampler, layout );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPIMAGEVIEW_HPP
