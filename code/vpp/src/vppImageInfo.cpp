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

#include "ph.hpp"
#include "../include/vppImageInfo.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

ImageInfo :: ImageInfo (
    EImagePurpose _purpose,
    EImageType _type,
    VkFormat _format,
    const VkExtent3D& _extent,
    unsigned int _mipLevels,
    unsigned int _arrayLayers,
    VkSampleCountFlagBits _multisampling,
    VkImageTiling _tiling,
    unsigned int _usage,
    unsigned int _flags ) :
        purpose ( _purpose ),
        type ( _type ),
        format ( _format ),
        extent ( _extent ),
        mipLevels ( _mipLevels ),
        arrayLayers ( _arrayLayers ),
        multisampling ( _multisampling ),
        tiling ( _tiling ),
        usage ( _usage ),
        flags ( _flags )
{
}

// -----------------------------------------------------------------------------

ImageInfo :: ImageInfo (
    unsigned int width,
    unsigned int height,
    EImagePurpose _purpose,
    unsigned int usage,
    VkFormat fmt ) :
        ImageInfo (
            _purpose,
            IMG_TYPE_2D,
            fmt,
            VkExtent3D { width, height, 1 },
            1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
            usage, 0 )
{
}

// -----------------------------------------------------------------------------

std::uint32_t ImageInfo :: getAspect() const
{
    switch ( format )
    {
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
        case VK_FORMAT_D32_SFLOAT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;

        case VK_FORMAT_S8_UINT:
            return VK_IMAGE_ASPECT_STENCIL_BIT;

        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

        default:
            break;
    }

    return VK_IMAGE_ASPECT_COLOR_BIT;
}

// -----------------------------------------------------------------------------

VkImageViewType ImageInfo :: getViewType() const
{
    switch ( type )
    {
        case IMG_TYPE_1D: return VK_IMAGE_VIEW_TYPE_1D;
        case IMG_TYPE_2D: return VK_IMAGE_VIEW_TYPE_2D;
        case IMG_TYPE_3D: return VK_IMAGE_VIEW_TYPE_3D;
        case IMG_TYPE_CUBE_2D: return VK_IMAGE_VIEW_TYPE_CUBE;
        case IMG_TYPE_RECT_2D: return VK_IMAGE_VIEW_TYPE_2D;
        default: return VK_IMAGE_VIEW_TYPE_2D;
    }
}

// -----------------------------------------------------------------------------

VkImageType ImageInfo :: getImageType() const
{
    switch ( type )
    {
        case IMG_TYPE_1D: return VK_IMAGE_TYPE_1D;
        case IMG_TYPE_3D: return VK_IMAGE_TYPE_3D;
        default: return VK_IMAGE_TYPE_2D;
    }
}

// -----------------------------------------------------------------------------

std::uint32_t ImageInfo :: getDimensions() const
{
    switch ( type )
    {
        case IMG_TYPE_1D:
            return 1;

        case IMG_TYPE_3D:
            return 3;

        default:
            return 2;
    }
}

// -----------------------------------------------------------------------------

bool ImageInfo :: operator< ( const ImageInfo& rhs ) const
{
    if ( purpose != rhs.purpose ) return purpose < rhs.purpose;
    if ( type != rhs.type ) return type < rhs.type;
    if ( format != rhs.format ) return format < rhs.format;
    if ( extent.width != rhs.extent.width ) return extent.width < rhs.extent.width;
    if ( extent.height != rhs.extent.height ) return extent.height < rhs.extent.height;
    if ( extent.depth != rhs.extent.depth ) return extent.depth < rhs.extent.depth;
    if ( mipLevels != rhs.mipLevels ) return mipLevels < rhs.mipLevels;
    if ( arrayLayers != rhs.arrayLayers ) return arrayLayers < rhs.arrayLayers;
    if ( multisampling != rhs.multisampling ) return multisampling < rhs.multisampling;
    if ( tiling != rhs.tiling ) return tiling < rhs.tiling;
    return usage < rhs.usage;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

bool SAttachmentDescription :: operator< ( const SAttachmentDescription& rhs ) const
{
    if ( flags != rhs.flags ) return flags < rhs.flags;
    if ( format != rhs.format ) return format < rhs.format;
    if ( samples != rhs.samples ) return samples < rhs.samples;
    if ( loadOp != rhs.loadOp ) return loadOp < rhs.loadOp;
    if ( storeOp != rhs.storeOp ) return storeOp < rhs.storeOp;
    if ( stencilLoadOp != rhs.stencilLoadOp ) return stencilLoadOp < rhs.stencilLoadOp;
    if ( stencilStoreOp != rhs.stencilStoreOp ) return stencilStoreOp < rhs.stencilStoreOp;
    if ( initialLayout != rhs.initialLayout ) return initialLayout < rhs.initialLayout;
    return finalLayout < rhs.finalLayout;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
