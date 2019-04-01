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

#ifndef INC_VPPIMAGEINFO_HPP
#define INC_VPPIMAGEINFO_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum EImagePurpose
{
    RENDER, DISPLAY, SWAPCHAIN, RENDER_DEBUG
};

// -----------------------------------------------------------------------------

enum EImageType
{
    IMG_TYPE_1D,
    IMG_TYPE_2D,
    IMG_TYPE_3D,
    IMG_TYPE_CUBE_2D,
    IMG_TYPE_RECT_2D
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ComponentMapping : public VkComponentMapping
{
public:
    ComponentMapping();
};

// -----------------------------------------------------------------------------

VPP_INLINE ComponentMapping :: ComponentMapping()
{
    r = VK_COMPONENT_SWIZZLE_R;
    g = VK_COMPONENT_SWIZZLE_G;
    b = VK_COMPONENT_SWIZZLE_B;
    a = VK_COMPONENT_SWIZZLE_A;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct ImageInfo
{
    VPP_DLLAPI ImageInfo (
        EImagePurpose _purpose,
        EImageType _type,
        VkFormat _format,
        const VkExtent3D& _extent,
        unsigned int _mipLevels,
        unsigned int _arrayLayers,
        VkSampleCountFlagBits _multisampling,
        VkImageTiling _tiling,
        unsigned int _usage,
        unsigned int _flags );
    
    VPP_DLLAPI ImageInfo (
        unsigned int width,
        unsigned int height,
        EImagePurpose _purpose,
        unsigned int usage,
        VkFormat fmt );
    
    VPP_DLLAPI std::uint32_t getAspect() const;
    VPP_DLLAPI VkImageViewType getViewType() const;
    VPP_DLLAPI VkImageType getImageType() const;
    VPP_DLLAPI std::uint32_t getDimensions() const;

    VPP_DLLAPI bool operator< ( const ImageInfo& rhs ) const;
    bool operator== ( const ImageInfo& rhs ) const;
    bool operator!= ( const ImageInfo& rhs ) const;

    EImagePurpose purpose;
    EImageType type;
    VkFormat format;
    VkExtent3D extent;
    unsigned int mipLevels;
    unsigned int arrayLayers;
    VkSampleCountFlagBits multisampling;
    VkImageTiling tiling;
    unsigned int usage;
    unsigned int flags;
};

// -----------------------------------------------------------------------------

VPP_INLINE bool ImageInfo :: operator== ( const ImageInfo& rhs ) const
{
    return
        purpose == rhs.purpose
        && type == rhs.type
        && format == rhs.format
        && extent.width == rhs.extent.width
        && extent.height == rhs.extent.height
        && extent.depth == rhs.extent.depth
        && mipLevels == rhs.mipLevels
        && arrayLayers == rhs.arrayLayers
        && multisampling == rhs.multisampling
        && tiling == rhs.tiling
        && usage == rhs.usage;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool ImageInfo :: operator!= ( const ImageInfo& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SAttachmentDescription : public VkAttachmentDescription
{
    VPP_DLLAPI bool operator< ( const SAttachmentDescription& rhs ) const;
    bool operator== ( const SAttachmentDescription& rhs ) const;
    bool operator!= ( const SAttachmentDescription& rhs ) const;
};

// -----------------------------------------------------------------------------

VPP_INLINE bool SAttachmentDescription :: operator== ( const SAttachmentDescription& rhs ) const
{
    return
        flags == rhs.flags
        && format == rhs.format
        && samples == rhs.samples
        && loadOp == rhs.loadOp
        && storeOp == rhs.storeOp
        && stencilLoadOp == rhs.stencilLoadOp
        && stencilStoreOp == rhs.stencilStoreOp
        && initialLayout == rhs.initialLayout
        && finalLayout == rhs.finalLayout;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool SAttachmentDescription :: operator!= ( const SAttachmentDescription& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

static_assert (
    sizeof ( SAttachmentDescription ) == sizeof ( VkAttachmentDescription ),
    "Attachment description size mismatch (compiler bug)." );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPIMAGEINFO_HPP
