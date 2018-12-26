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

/**
    \brief Class representing generic (untyped) Vulkan image.

    This class represents an image without encoding image attributes
    (e.g. format) in the C++ type. This makes image handling easier.

    Many VPP operations require only such untyped reference to an image.
    On the other hand, there are contexts (like binding points in shaders)
    that require \b typed image references, i.e. instances of Image
    template.

    You can always cast from typed image reference (Image) to untyped (Img),
    as Img is the base class of all Image template instances.

    As for deciding whether to create/pass Img or Image reference,
    choose the variant appropriate for the usage of the reference. If the
    image has to be bound to a shader binding point, Image will be
    required. If the image will be only supplied to a command (e.g. cmdCopyImage()),
    Img will suffice.

    This object is reference-counted and may be passed by value.
*/

class Img
{
public:
    /**
        \brief Enumeration specifying how an image may be used.

        Every image has a parameter called \b usage which is a bitwise combination
        of values taken from EUsageFlags enumeration. It restricts possible usage
        of the image, but also allows performance optimization of it.

        These values are identical to corresponding low-level Vulkan flags
        and may be used interchangeably.
    */
    enum EUsageFlags
    {
        SOURCE = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,              /**< \brief Allows to use an image as a source of transfer or blit operation. */
        TARGET = VK_IMAGE_USAGE_TRANSFER_DST_BIT,              /**< \brief Allows to use an image as a destination of transfer or blit operation. */
        SAMPLED = VK_IMAGE_USAGE_SAMPLED_BIT,                  /**< \brief Allows to use an image as a read-only texture. */
        STORAGE = VK_IMAGE_USAGE_STORAGE_BIT,                  /**< \brief Allows to use an image as a mutable image (writable in the shader). */
        COLOR = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,           /**< \brief Allows to use an image as an attachment, to be used in a rendering process. */
        DEPTH = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,   /**< \brief Allows to use an image as an attachment, to be used in a rendering process as depth buffer. */
        TRANSIENT = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,   /**< \brief Allows to optimize memory allocation for an attachment image. */
        INPUT = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT            /**< \brief Allows to use an image as an input attachment, to provide data for a rendering process. */
    };

    /** \brief Constructs null reference. */
    Img();

    /** \brief Constructs an image with attributes defined by an ImageInfo structure.
    
        The \c memProfile argument specifies what kind of physical memory is
        needed for the image. Typically use MemProfile::DEVICE_STATIC
        value to allocate the image memory on GPU side.

        An example:

        \code
            vpp::Device hDevice = ...;

            vpp::ImageInfo myImgInfo (
                vpp::RENDER,
                vpp::IMG_TYPE_2D,
                VK_FORMAT_R8G8B8A8_UINT,
                { 1024, 768, 1 },
                1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
                vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
            );

            vpp::Img myImg = vpp::Img (
                myImgInfo, MemProfile::DEVICE_STATIC, hDevice );

            vpp::Img myImgWithLayout = vpp::Img (
                myImgInfo, MemProfile::DEVICE_STATIC, hDevice,
                VK_IMAGE_LAYOUT_GENERAL );

        \endcode
    */
    Img (
        const ImageInfo& imageInfo,
        const MemProfile& memProfile,
        const Device& hDevice,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< unsigned int >& queueFamilyIndices = std::vector< unsigned int >() );

    /** \brief Constructs an image from already existing Vulkan image handle.

        You must still provide image metadata, as Vulkan does not have queries for it.

        An example:

        \code
            VkImage hVulkanImg = ...;

            vpp::Device hDevice = ...;

            vpp::ImageInfo myImgInfo (
                vpp::RENDER,
                vpp::IMG_TYPE_2D,
                VK_FORMAT_R8G8B8A8_UINT,
                { 1024, 768, 1 },
                1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
                vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
            );

            vpp::Img myImg = vpp::Img ( myImgInfo, hDevice, hVulkanImg );

        \endcode
    */
    Img (
        const ImageInfo& imageInfo,
        const Device& hDevice,
        VkImage hImage );

    /** \brief Retrieves the Vulkan handle. */
    VkImage handle() const;

    /** \brief Checks whether this is a valid image. */
    bool valid() const;

    /** \brief Retrieves the device. */
    const Device& device() const;

    /** \brief Retrieves all image attributes. */
    const ImageInfo& info() const;

    /** \brief Retrieves image size. */
    const VkExtent3D& extent() const;

    /** \brief Retrieves image format. */
    VkFormat format() const;
};

// -----------------------------------------------------------------------------

/**
    \brief A template containing image attributes inside C++ type.

    Advanced image usage in VPP (e.g. in shader code) requires that
    certain image attributes are known at compile time. This template
    allows to encode these attributes. This is the first template you should
    \c typedef in order to make your custom image class.

    \tparam FormatT %Instance of format< ... > template specifying the format.
    \tparam PURPOSE Purpose of the image. Usually RENDER.
    \tparam TYPE Type of the image. This determines number of dimensions.
    \tparam USAGE Allowed usage of the image. A bitwise sum of Img::EUsageFlags values.
    \tparam TILING %Image tiling. Usually VK_IMAGE_TILING_OPTIMAL.
    \tparam MULTISAMPLING Multisampling mode (MSAA).
    \tparam MIPMAPPED Whether the image may contain multiple mip-maps.
    \tparam ARRAYED Whether the image may contain multiple array levels.

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

    \endcode
*/

template<
    class FormatT, 
    EImagePurpose PURPOSE,
    EImageType TYPE,
    unsigned int USAGE,
    VkImageTiling TILING = VK_IMAGE_TILING_OPTIMAL,
    VkSampleCountFlagBits MULTISAMPLING = VK_SAMPLE_COUNT_1_BIT,
    bool MIPMAPPED = true,
    bool ARRAYED = false,
    unsigned int FLAGS = 0u >
struct ImageAttributes
{
};

// -----------------------------------------------------------------------------
/**
    \brief Class representing typed Vulkan image.

    This image class template accepts an instance of ImageAttributes template,
    so it has compile-time knowledge of various image attributes. This is required
    if the image has to be accessed in a shader.

    This object is reference-counted and may be passed by value.

    An example:

    \code
        typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > MyColorFormat;

        typedef vpp::ImageAttributes<
            MyColorFormat,
            vpp::RENDER,
            vpp::IMG_TYPE_2D,
            vpp::Img::COLOR,
            VK_IMAGE_TILING_OPTIMAL,
            VK_SAMPLE_COUNT_1_BIT,
            false,
            false
        > MyColorImageAttr;

        typedef vpp::Image< MyColorImageAttr > MyImage;

        // ...

        // Creates the image.
        MyImage hMyImage = MyImage (
            { 1280, 960, 1 },
            vpp::MemProfile::DEVICE_STATIC,
            hDevice
        );

        // ...

    \endcode

    Another example:

    \code
        typedef vpp::format< vpp::texture_format > MyTextureFormat;

        typedef vpp::ImageAttributes<
            MyTextureFormat,
            vpp::RENDER,
            vpp::IMG_TYPE_2D,
            vpp::Img::SAMPLED | vpp::Img::TARGET,
            VK_IMAGE_TILING_OPTIMAL,
            VK_SAMPLE_COUNT_1_BIT,
            true,
            true
        > MyTextureArrayAttr;

        typedef vpp::Image< MyTextureArrayAttr > MyTextureArrayImage;

        // ...

        // Creates the image.
        MyTextureArrayImage hMyTextureArray = MyImage (
            VK_FORMAT_BC7_UNORM_BLOCK,
            { 1024, 1024, 1 },
            vpp::MemProfile::DEVICE_STATIC,
            hDevice,
            11,  // mip levels
            8    // array layers
        );

        // ...

    \endcode
*/

template< class AttributesT >
class Image : public Img
{
public:
    /** \brief Image attributes. */
    typedef AttributesT attributes_type;

    /** \brief Image format. */
    typedef typename AttributesT::format_type format_type;

    /** \brief Constructs null reference. */
    Image();

    /**
        \brief Construct a typed image.

        Provide image size and the target device.

        The \c memProfile argument specifies what kind of physical memory is
        needed for the image. Typically use MemProfile::DEVICE_STATIC
        value to allocate the image memory on GPU side.
        
        Optional arguments include number of mip maps and array levels
        (applicable only for mip-mapped and/or arrayed images). Also
        you can specify starting layout of the image. Finally there is
        \c queueFamilyIndices parameter, applicable only for images shared
        between queues (rare usage).
    */

    Image (
        const VkExtent3D& extent,
        const MemProfile& memProfile,
        const Device& hDevice,
        unsigned int mipLevels = 1u,
        unsigned int arrayLevels = 1u,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< unsigned int >& queueFamilyIndices = std::vector< unsigned int >() );

    /**
        \brief Construct a typed image using dynamic format.

        This constructor allows to specify a format as an argument, which
        overrides the format encoded in image type. This is meant mainly for
        compressed texture formats, which have general \c texture_format
        format statically encoded, but real format (e.g. \c VK_FORMAT_BC1_RGB_UNORM_BLOCK )
        is not known at compile time and may vary for different textures.

        To make handling such images simpler, you can specify the format
        in the constructor after checking at runtime what the format really is.
        The drawback of such approach is that the image usage is limited
        to be read-only texture image.

        The \c memProfile argument specifies what kind of physical memory is
        needed for the image. Typically use MemProfile::DEVICE_STATIC
        value to allocate the image memory on GPU side.
        
        Also specify image size and the target device.

        Optional arguments include number of mip maps and array levels
        (applicable only for mip-mapped and/or arrayed images). Also
        you can specify starting layout of the image. Finally there is
        \c queueFamilyIndices parameter, applicable only for images shared
        between queues (rare usage).
    */
    Image (
        VkFormat format,
        const VkExtent3D& extent,
        const MemProfile& memProfile,
        const Device& hDevice,
        unsigned int mipLevels = 1u,
        unsigned int arrayLevels = 1u,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< unsigned int >& queueFamilyIndices = std::vector< unsigned int >() );
    
    /**
        \brief Construct a typed image from existing Vulkan image handle.

        This constructor makes an Image object for already existing
        Vulkan image represented by a handle. Also specify image size and
        the target device.

        Optional arguments include number of mip maps and array levels
        (applicable only for mip-mapped and/or arrayed images).
    */
    Image (
        const VkExtent3D& extent,
        const Device& hDevice,
        VkImage hImage,
        unsigned int mipLevels = 1u,
        unsigned int arrayLevels = 1u );

    /**
        \brief Construct a typed image using dynamic format
        from existing Vulkan image handle.

        This constructor makes an Image object for already existing
        Vulkan image represented by a handle. Also specify image size and
        the target device.

        This constructor allows to specify a format as an argument, which
        overrides the format encoded in image type. This is meant mainly for
        compressed texture formats, which have general \c texture_format
        format statically encoded, but real format (e.g. \c VK_FORMAT_BC1_RGB_UNORM_BLOCK )
        is not known at compile time and may vary for different textures.

        To make handling such images simpler, you can specify the format
        in the constructor after checking at runtime what the format really is.
        The drawback of such approach is that the image usage is limited
        to be read-only texture image.

        Optional arguments include number of mip maps and array levels
        (applicable only for mip-mapped and/or arrayed images).
    */
    Image (
        VkFormat format,
        const VkExtent3D& extent,
        const Device& hDevice,
        VkImage hImage,
        unsigned int mipLevels = 1u,
        unsigned int arrayLevels = 1u );

    /**
        \brief Constructs typed image from untyped image.

        This allows to convert untyped Img object to a typed image.
        This operation is potentially unsafe, as there is no checking
        whether attributes actually match. Use only when you are sure that
        the actual image is compatible with the type.
    */

    explicit Image ( const Img& rawImage );
};

// -----------------------------------------------------------------------------

template< class FormatT >
using ImgTexture1D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_1D,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgTexture2D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_2D,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgTexture3D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_3D,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgTextureCube = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_CUBE_2D,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgStorage1D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_1D,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgStorage2D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_2D,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgStorage3D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_3D,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgStorageCube = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_CUBE_2D,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT > >;

template< class FormatT >
using ImgAttachment2D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_2D,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT > >;

template< class FormatT >
using ImgDepthAttachment2D = Image<
    ImageAttributes<
        FormatT, RENDER, IMG_TYPE_2D,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT > >;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
