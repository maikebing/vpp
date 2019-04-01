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

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppImageOperations.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

void FillImageSolid :: init()
{
    execute << [ this ]()
    {
        cmdClearColorImage ( d_targetImage, d_color );

        cmdChangeImageLayout (
            d_targetImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            d_finalLayout
        );
    };

    compile();
}

// -----------------------------------------------------------------------------

void CopyImageToDevice :: init()
{
    execute << [ this ]()
    {
        cmdChangeImageLayout (
            d_targetImage,
            VK_IMAGE_LAYOUT_UNDEFINED, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            static_cast< unsigned int >( d_regions.size() )
        );

        cmdCopyBufferToImage (
            d_sourceBuffer,
            d_targetImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            d_regions
        );

        cmdChangeImageLayout (
            d_targetImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            d_finalLayout,
            static_cast< unsigned int >( d_regions.size() )
        );
    };

    compile();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

SLayoutInitializer :: SLayoutInitializer ( const Img& img ) :
    image ( img ),
    oldImageLayout ( VK_IMAGE_LAYOUT_UNDEFINED ),
    newImageLayout ( usageToLayout ( img.info().usage ) ),
    levelCount ( img.info().mipLevels ),
    layerCount ( img.info().arrayLayers ),
    baseMipLevel ( 0 ),
    baseArrayLayer ( 0 ),
    aspectMask ( img.info().getAspect() )
{
}

// -----------------------------------------------------------------------------

SLayoutInitializer :: SLayoutInitializer ( const Img& img, VkImageLayout layout ) :
    image ( img ),
    oldImageLayout ( VK_IMAGE_LAYOUT_UNDEFINED ),
    newImageLayout ( layout ),
    levelCount ( img.info().mipLevels ),
    layerCount ( img.info().arrayLayers ),
    baseMipLevel ( 0 ),
    baseArrayLayer ( 0 ),
    aspectMask ( img.info().getAspect() )
{
}

// -----------------------------------------------------------------------------

VkImageLayout SLayoutInitializer :: usageToLayout ( unsigned int usage )
{
    if ( usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    else if ( usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    else if ( usage & VK_IMAGE_USAGE_SAMPLED_BIT )
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    else if ( usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT )
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    else if ( usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT )
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    else
        return VK_IMAGE_LAYOUT_GENERAL;
}

// -----------------------------------------------------------------------------

void KInitializeLayouts :: init()
{
    execute << [ this ]()
    {
        for ( const auto& iInitializer : d_initializers )
        {
            cmdChangeImageLayout (
                iInitializer.image,
                iInitializer.oldImageLayout,
                iInitializer.newImageLayout,
                iInitializer.levelCount,
                iInitializer.layerCount,
                iInitializer.baseMipLevel,
                iInitializer.baseArrayLayer,
                iInitializer.aspectMask );
        }
    };

    compile();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
