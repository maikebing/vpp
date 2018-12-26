/*
    Copyright 2016-2018 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
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

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum ESamplingMode
{
    SAMPLER_NORMALIZED,
    SAMPLER_UNNORMALIZED
};

// -----------------------------------------------------------------------------
/**
    \brief A template encoding image view attributes inside C++ type.

    Advanced image usage in VPP (e.g. in shader code) requires that
    certain image attributes are known at compile time. This template
    allows to encode these attributes. This is additional template you should
    \c typedef additionally to Image template, in order to define ypour image type.

    \tparam ImageT %Instance of Image< ... > template specifying the image type.
    \tparam SAMPLING Type of sampler to be associated with this image. Usually SAMPLER_NORMALIZED.
    \tparam FormatT Format class to override image format. 
    \tparam ASPECTS Aspect mask.
    \tparam CUBE Whether cube maps are supported by this view.

    If you specify the format (which is optional), it will override declared format
    of the image. This is quite like to \c reinterpret_cast or union in C++. The
    formats should be \b compatible, which means that single pixel should occupy
    same area in memory. The pixel data will be reinterpreted according to the new format.
    See section 30.3.1 in official Vulkan docs (Format Compatibility Classes)
    for complete list of compatible formats.

    An example:

    \code
        typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > MyColorFormat;

        typedef vpp::ImageAttributes<
            MyColorFormat,
            vpp::RENDER,
            vpp::IMG_TYPE_2D,
            vpp::Img::COLOR | vpp::Img::STORAGE,
            VK_IMAGE_TILING_OPTIMAL,
            VK_SAMPLE_COUNT_1_BIT,
            false,
            false
        > MyColorBufferAttr;

        typedef vpp::Image< MyColorImageAttr > MyImage;
        typedef vpp::ImageViewAttributes< MyImage > MyImageViewAttr;

    \endcode
*/

template<
    class ImageT,
    ESamplingMode SAMPLING = SAMPLER_NORMALIZED,
    class FormatT = typename ImageT::format_type,
    unsigned int ASPECTS = FormatT::aspects,
    ECubeMode CUBE = CUBE_ALLOW >
struct ImageViewAttributes
{
};

// -----------------------------------------------------------------------------
/**
    \brief Typed image view.

    An image view is auxiliary object that acts as an interface between
    the image and the binding point in the pipeline. In case of image
    buffers, the view is actual Vulkan object and has two functions:
    - providing information about the format of the image data (it can
      optionally override the format declared for the image itself),
    - optionally, specifying a image slice, i.e. a subset of mipmap levels
      and/or array layers.

    You must hold ImageView references along with your image. This
    is similar to texel buffer views.

    As the template argument, use an instance of ImageViewAttributes template.

    This object is reference-counted and may be passed by value.
*/

template< class AttributesT >
class ImageView
{
public:
    /** \brief %Image type. */
    typedef typename AttributesT::image_type image_type;

    /** \brief Format type (an instance of vpp::format template). */
    typedef typename AttributesT::format_type format_type;

    /** \brief Attributes type (taken from the argument). */
    typedef AttributesT attributes_type;

    /** \brief Constructs null reference. */
    ImageView();

    /** \brief Constructs a view for specified image. */
    explicit ImageView ( const image_type& hImage );

    /** \brief Constructs a view for single array layer selected from specified image. */
    ImageView ( const image_type& hImage, unsigned int layer );

    /** \brief Constructs a typed view for single array layer and mip level selected from specified image. */
    ImageView ( const image_type& hImage, unsigned int mipLevel, unsigned int layer );

    /** \brief Constructs a typed view from untyped view. */
    explicit ImageView ( const FrameImageView& hGenericView );

    /** \brief Constructs a typed view for specified range of array layers and mip levels selected from specified image. */
    ImageView (
        const image_type& hImage,
        unsigned int baseMipLevel,
        unsigned int levelCount,
        unsigned int baseArrayLayer,
        unsigned int layerCount,
        const VkComponentMapping& componentMapping = ComponentMapping()
    );

    /** \brief Retrieves the Vulkan handle. */
    operator VkImageView() const;

    /** \brief Retrieves the image. */
    const image_type& image() const;

    /** \brief Retrieves the device. */
    const Device& device() const;

    /** \brief Retrieves the range of image slice (levels and layers). */
    const VkImageSubresourceRange& subresourceRange() const;

    /** \brief Converts the typed view to untyped view. */
    FrameImageView frameView() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Helper function to bind specified image view to a binding point in a pipeline.

    When you call ShaderDataBlock::update method in order to bind actual resources
    (buffers, images) to shader data blocks, you provide a list of assignments.
    In each assignment, the binding point name comes on the left hand side, and on
    the right hand side usually there is the name of the image or buffer object.
    You can use the bind() function call in this place, to provide additional parameters.

    Currently you can specify the layout in which the resource will be when being
    accessed by the shader. This is applicable only for binding points of the following
    types: inTexture, inSampledTexture, inConstSampledTexture, inAttachment. Allowed
    values are:
        - VK_IMAGE_LAYOUT_GENERAL,
        - VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        - VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL.

    In any case, if you do not use the \ref bind function at all,
    one of the values listed above, appropriate for the binding point, will be assumed.
*/

template< class AttributesT >
auto bind (
    const ImageView< AttributesT >& view,
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED );

// -----------------------------------------------------------------------------
/**
    \brief Helper function to bind specified image view and a sampler to a binding point in a pipeline.

    When you call ShaderDataBlock::update method in order to bind actual resources
    (buffers, images) to shader data blocks, you provide a list of assignments.
    In each assignment, the binding point name comes on the left hand side, and on
    the right hand side usually there is the name of the image or buffer object.
    You can use the bind() function call in this place, to provide additional parameters.

    This variant of the bind() function allows to bind a sampler along with
    an image. You \b must use it for inSampledTexture binding points (and for
    anything else). This is because inSampledTexture represents a texture image
    with predefined sampler, although not statically associated (as with
    inConstSampledTexture).

    You can also specify the layout in which the resource will be when being
    accessed by the shader. This is applicable only for binding points of the following
    types: inTexture, inSampledTexture, inConstSampledTexture, inAttachment. Allowed
    values are:
        - VK_IMAGE_LAYOUT_GENERAL,
        - VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        - VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL.

    If you do not provide this parameter,
    one of the values listed above, appropriate for the binding point, will be assumed.
*/

template< class AttributesT, class SamplerT >
auto bind (
    const SamplerT& sampler,
    const ImageView< AttributesT >& view,
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
