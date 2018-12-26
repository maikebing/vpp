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

#ifndef INC_VPPIMAGE_HPP
#define INC_VPPIMAGE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICEMEMORY_HPP
#include "vppDeviceMemory.hpp"
#endif

#ifndef INC_VPPIMAGEINFO_HPP
#include "vppImageInfo.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class ImageImpl;

// -----------------------------------------------------------------------------

class Img : public vpp::TSharedReference< ImageImpl >
{
public:
    enum EUsageFlags
    {
        SOURCE = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        TARGET = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        SAMPLED = VK_IMAGE_USAGE_SAMPLED_BIT,
        STORAGE = VK_IMAGE_USAGE_STORAGE_BIT,
        COLOR = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        DEPTH = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        TRANSIENT = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
        INPUT = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
    };

    Img();

    Img (
        const ImageInfo& imageInfo,
        const MemProfile& memProfile,
        const Device& hDevice,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< std::uint32_t >& queueFamilyIndices = std::vector< std::uint32_t >() );

    Img (
        const ImageInfo& imageInfo,
        const Device& hDevice,
        VkImage hImage );

    ~Img();

    Img ( const Img& rhs );
    const Img& operator= ( const Img& rhs );

    VkImage handle() const;
    bool valid() const;
    const Device& device() const;
    const ImageInfo& info() const;
    const VkExtent3D& extent() const;
    VkFormat format() const;

public:
    void setMemory ( const DeviceMemory& hMemory ) const;
    const DeviceMemory& getMemory() const;

    template< class MemoryT >
    MemoryT bindMemory ( const MemProfile& memProfile ) const;
};

// -----------------------------------------------------------------------------

class ImageImpl : public vpp::TSharedObject< ImageImpl >
{
public:
    VPP_DLLAPI ImageImpl (
        const ImageInfo& imageInfo,
        VkImageLayout initialLayout,
        const Device& hDevice,
        const std::vector< std::uint32_t >& queueFamilyIndices,
        VkImage hImage );

    VPP_DLLAPI ~ImageImpl();

private:
    friend class Img;
    Device d_hDevice;
    VkImage d_handle;
    VkResult d_result;
    DeviceMemory d_memory;
    ImageInfo d_imageInfo;
};

// -----------------------------------------------------------------------------

VPP_INLINE Img :: Img()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Img :: Img (
    const ImageInfo& imageInfo,
    const MemProfile& memProfile,
    const Device& hDevice,
    VkImageLayout initialLayout,
    const std::vector< std::uint32_t >& queueFamilyIndices ) :
        TSharedReference< ImageImpl >( new ImageImpl (
            imageInfo, initialLayout, hDevice, queueFamilyIndices, VK_NULL_HANDLE ) )
{
    if ( get()->d_handle != 0 && get()->d_result == VK_SUCCESS )
        bindMemory< DeviceMemory >( memProfile );
}

// -----------------------------------------------------------------------------

VPP_INLINE Img :: Img (
    const ImageInfo& imageInfo,
    const Device& hDevice,
    VkImage hImage ) :
        TSharedReference< ImageImpl >( new ImageImpl (
            imageInfo, VK_IMAGE_LAYOUT_UNDEFINED, hDevice,
            std::vector< std::uint32_t >(), hImage ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Img :: ~Img()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Img :: Img ( const Img& rhs ) :
    TSharedReference< ImageImpl >( rhs )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE const Img& Img :: operator= ( const Img& rhs )
{
    static_cast< TSharedReference< ImageImpl >& >( *this ) = rhs;
    return *this;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkImage Img :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool Img :: valid() const
{
    return get() && get()->d_handle != 0 && get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Img :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE const ImageInfo& Img :: info() const
{
    return get()->d_imageInfo;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Img :: setMemory ( const DeviceMemory& hMemory ) const
{
    get()->d_memory = hMemory;
}

// -----------------------------------------------------------------------------

VPP_INLINE const DeviceMemory& Img :: getMemory() const
{
    return get()->d_memory;
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkExtent3D& Img :: extent() const
{
    return info().extent;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkFormat Img :: format() const
{
    return info().format;
}

// -----------------------------------------------------------------------------

template< class MemoryT >
MemoryT Img :: bindMemory ( const MemProfile& memProfile ) const
{
    if ( ! getMemory() )
    {
        VkMemoryRequirements memoryRequirements;

        const Device hDevice = device();

        ::vkGetImageMemoryRequirements (
            hDevice.handle(), handle(), & memoryRequirements );

        MemoryT memory = MemoryT (
            memoryRequirements.size,
            memoryRequirements.memoryTypeBits,
            memProfile,
            hDevice );

        setMemory ( memory );

        ::vkBindImageMemory (
            hDevice.handle(), handle(), memory.handle(), 0 );

        return memory;
    }
    else
        return MemoryT ( getMemory() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

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
    typedef FormatT format_type;
    static const VkFormat format_code = FormatT::code;
    static const spv::ImageFormat spv_code = FormatT::spvCode;

    static const EImagePurpose purpose = PURPOSE;
    static const EImageType type = TYPE;
    static const VkImageTiling tiling = TILING;
    static const unsigned int usage = USAGE;
    static const VkSampleCountFlagBits multisampling = MULTISAMPLING;

    static const bool isMipMapped = MIPMAPPED;
    static const bool isArrayed = ARRAYED;

    static const bool isSparseResidency =
        ( FLAGS & VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT ) != 0;

    static const bool isSparseAliased =
        ( FLAGS & VK_IMAGE_CREATE_SPARSE_ALIASED_BIT ) != 0;

    static const unsigned int flags = 
        FLAGS
        | ( type == IMG_TYPE_CUBE_2D ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0 )
        | ( ( isSparseResidency || isSparseAliased ) ? VK_IMAGE_CREATE_SPARSE_BINDING_BIT : 0 );

    static const VkImageType imageDim =
        ( type == IMG_TYPE_1D ? VK_IMAGE_TYPE_1D :
            ( type == IMG_TYPE_3D ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D ) );

    static const unsigned int textureUsage =
        VK_IMAGE_USAGE_SAMPLED_BIT
        | VK_IMAGE_USAGE_STORAGE_BIT;

    static const unsigned int transferUsage =
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT
        | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    static const unsigned int attachmentUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
        | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
        | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;

    static const bool isData = ( purpose == RENDER || purpose == RENDER_DEBUG );
    static const bool isDisplay = ( purpose == DISPLAY );
    static const bool isSwapchain = ( purpose == SWAPCHAIN );

    static const bool isMultisampled = ( multisampling != VK_SAMPLE_COUNT_1_BIT );

    static const bool isFormatColor = FormatT::isColor;
    static const bool isFormatDepth = FormatT::isDepth;
    static const bool isFormatStencil = FormatT::isStencil;
    static const bool isFormatDepthStencil = isFormatDepth && isFormatStencil;
    static const bool isFormatDepthOrStencil = isFormatDepth || isFormatStencil;

    static const bool isUsageTransferSrc =
        ( usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) != 0;

    static const bool isUsageTransferDst =
        ( usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) != 0;

    static const bool isUsageSampled =
        ( usage & VK_IMAGE_USAGE_SAMPLED_BIT ) != 0;

    static const bool isUsageStorage =
        ( usage & VK_IMAGE_USAGE_STORAGE_BIT ) != 0;

    static const bool isUsageColorAtt =
        ( usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) != 0;

    static const bool isUsageDepthStencilAtt =
        ( usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) != 0;

    static const bool isUsageTransientAtt =
        ( usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ) != 0;

    static const bool isUsageInputAtt =
        ( usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ) != 0;

    static const bool isSparseBinding =
        ( flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT ) != 0;

    static const bool isMutableFormat =
        ( flags & VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT ) != 0;

    static_assert (
        usage != 0,
        "You must specify at least one usage bit" );

    static_assert (
        ( ! isUsageColorAtt || isFormatColor ),
        "Color attachment requires color image format" );

    static_assert (
        ( ! isUsageDepthStencilAtt || isFormatDepthOrStencil ),
        "Depth/stencil attachment requires depth/stencil image format" );

    static_assert (
        ( ! isDisplay || isUsageColorAtt ),
        "Displayed image must be a color attachment" );

    static_assert (
        ( ! isMultisampled || type == IMG_TYPE_2D ),
        "Only 2D images can be multisampled" );

    static_assert (
        ( ! isMultisampled || tiling == VK_IMAGE_TILING_OPTIMAL ),
        "Multisampled images must have optimal tiling" );

    static_assert (
        ( ! isMultisampled || ! isMipMapped ),
        "Multisampled images may have only one mip level" );
    
    static_assert (
        ! isUsageTransientAtt || ( ( usage & attachmentUsage ) == usage ),
        "Transient memory binding is allowed only for attachment images" );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class AttributesT >
class Image : public Img
{
public:
    typedef AttributesT attributes_type;
    typedef typename AttributesT::format_type format_type;

    VPP_INLINE Image()
    {
    }

    VPP_INLINE Image (
        const VkExtent3D& extent,
        const MemProfile& memProfile,
        const Device& hDevice,
        std::uint32_t mipLevels = 1u,
        std::uint32_t arrayLevels = 1u,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< std::uint32_t >& queueFamilyIndices = std::vector< std::uint32_t >() ) :
            Img (
                ImageInfo (
                    AttributesT::purpose, AttributesT::type,
                    AttributesT::format_code, extent,
                    AttributesT::isMipMapped ? mipLevels : 1u,
                    AttributesT::isArrayed ? arrayLevels : 1u,
                    AttributesT::multisampling, AttributesT::tiling,
                    AttributesT::usage, AttributesT::flags
                ),
                memProfile, hDevice, initialLayout, queueFamilyIndices
            )
    {
        static_assert (
            AttributesT::format_code != VK_FORMAT_UNDEFINED,
            "Mo format specified. Provide VkFormat if you use vpp::texture_format." 
        );
    }

    VPP_INLINE Image (
        VkFormat format,
        const VkExtent3D& extent,
        const MemProfile& memProfile,
        const Device& hDevice,
        std::uint32_t mipLevels = 1u,
        std::uint32_t arrayLevels = 1u,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const std::vector< std::uint32_t >& queueFamilyIndices = std::vector< std::uint32_t >() ) :
            Img (
                ImageInfo (
                    AttributesT::purpose, AttributesT::type,
                    format, extent,
                    AttributesT::isMipMapped ? mipLevels : 1u,
                    AttributesT::isArrayed ? arrayLevels : 1u,
                    AttributesT::multisampling, AttributesT::tiling,
                    AttributesT::usage, AttributesT::flags
                ),
                memProfile, hDevice, initialLayout, queueFamilyIndices
            )
    {
    }
    
    VPP_INLINE Image (
        const VkExtent3D& extent,
        const Device& hDevice,
        VkImage hImage,
        std::uint32_t mipLevels = 1u,
        std::uint32_t arrayLevels = 1u ) :
            Img (
                ImageInfo (
                    AttributesT::purpose, AttributesT::type,
                    AttributesT::format_code, extent,
                    AttributesT::isMipMapped ? mipLevels : 1u,
                    AttributesT::isArrayed ? arrayLevels : 1u,
                    AttributesT::multisampling, AttributesT::tiling,
                    AttributesT::usage, AttributesT::flags
                ),
                hDevice, hImage
            )
    {
    }

    VPP_INLINE Image (
        VkFormat format,
        const VkExtent3D& extent,
        const Device& hDevice,
        VkImage hImage,
        std::uint32_t mipLevels = 1u,
        std::uint32_t arrayLevels = 1u ) :
            Img (
                ImageInfo (
                    AttributesT::purpose, AttributesT::type,
                    format, extent,
                    AttributesT::isMipMapped ? mipLevels : 1u,
                    AttributesT::isArrayed ? arrayLevels : 1u,
                    AttributesT::multisampling, AttributesT::tiling,
                    AttributesT::usage, AttributesT::flags
                ),
                hDevice, hImage
            )
    {
    }

    explicit VPP_INLINE Image ( const Img& rawImage ) :
        Img ( rawImage )
    {
    }
};

// -----------------------------------------------------------------------------
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

#endif // INC_VPPIMAGE_HPP
