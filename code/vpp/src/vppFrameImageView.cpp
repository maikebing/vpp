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
#include "../include/vppFrameImageView.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

FrameImageViewImpl :: FrameImageViewImpl ( const Img& hImage, std::uint32_t layer ) :
    d_hImage ( hImage ),
    d_hDevice ( hImage.device() ),
    d_handle()
{
    const ImageInfo& imgInfo = hImage.info();

    VkImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = 0;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = hImage.handle();
    imageViewCreateInfo.viewType = imgInfo.getViewType();
    imageViewCreateInfo.format = imgInfo.format;
    imageViewCreateInfo.subresourceRange.aspectMask = imgInfo.getAspect();
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = layer;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.components = ComponentMapping();

    ::vkCreateImageView (
        d_hDevice.handle(), & imageViewCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

FrameImageViewImpl :: FrameImageViewImpl (
    const Img& hImage, const VkImageSubresourceRange& range ) :
        d_hImage ( hImage ),
        d_hDevice ( hImage.device() ),
        d_handle()
{
    const ImageInfo& imgInfo = hImage.info();

    VkImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = 0;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = hImage.handle();
    imageViewCreateInfo.viewType = imgInfo.getViewType();
    imageViewCreateInfo.format = imgInfo.format;
    imageViewCreateInfo.subresourceRange = range;
    imageViewCreateInfo.components = ComponentMapping();

    ::vkCreateImageView (
        d_hDevice.handle(), & imageViewCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

FrameImageViewImpl :: ~FrameImageViewImpl()
{
    ::vkDestroyImageView ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
