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
#include "../include/vppImage.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

ImageImpl :: ImageImpl (
    const ImageInfo& imageInfo,
    VkImageLayout initialLayout,
    const Device& hDevice,
    const std::vector< std::uint32_t >& queueFamilyIndices,
    VkImage hImage ) :
        d_hDevice ( hDevice ),
        d_handle ( hImage ),
        d_imageInfo ( imageInfo )
{
    if ( d_handle == VK_NULL_HANDLE )
    {
        VkImageCreateInfo imageCreateInfo;
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = 0;
        imageCreateInfo.flags = imageInfo.flags;
        imageCreateInfo.format = imageInfo.format;
        imageCreateInfo.extent = imageInfo.extent;
        imageCreateInfo.mipLevels = imageInfo.mipLevels;
        imageCreateInfo.arrayLayers = imageInfo.arrayLayers;
        imageCreateInfo.samples = imageInfo.multisampling;
        imageCreateInfo.tiling = static_cast< VkImageTiling >( imageInfo.tiling );
        imageCreateInfo.initialLayout = initialLayout;
        imageCreateInfo.usage = imageInfo.usage;
        imageCreateInfo.imageType = imageInfo.getImageType();

        if ( ! queueFamilyIndices.empty() )
        {
            imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            imageCreateInfo.queueFamilyIndexCount = static_cast< std::uint32_t >( queueFamilyIndices.size() );
            imageCreateInfo.pQueueFamilyIndices = & queueFamilyIndices [ 0 ];
        }
        else
        {
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.queueFamilyIndexCount = 0;
            imageCreateInfo.pQueueFamilyIndices = 0;
        }

        d_result = ::vkCreateImage ( hDevice.handle(), & imageCreateInfo, 0, & d_handle );
    }
}

// -----------------------------------------------------------------------------

ImageImpl :: ~ImageImpl()
{
    if ( d_imageInfo.purpose != SWAPCHAIN )
        ::vkDestroyImage ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
