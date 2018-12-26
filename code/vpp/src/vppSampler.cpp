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
#include "../include/vppSampler.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

SamplerImpl :: SamplerImpl (
    const Device& hDevice, const SNormalizedSampler& samplerInfo ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result()
{
    VkSamplerCreateInfo samplerCreateInfo;

    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = 0;
    samplerCreateInfo.flags = 0;
    samplerCreateInfo.magFilter = static_cast< VkFilter >( samplerInfo.magFilterMode );
    samplerCreateInfo.minFilter = static_cast< VkFilter >( samplerInfo.minFilterMode );
    samplerCreateInfo.mipmapMode = static_cast< VkSamplerMipmapMode >( samplerInfo.mipMapMode );
    samplerCreateInfo.addressModeU = static_cast< VkSamplerAddressMode >( samplerInfo.addressModeU );
    samplerCreateInfo.addressModeV = static_cast< VkSamplerAddressMode >( samplerInfo.addressModeV );
    samplerCreateInfo.addressModeW = static_cast< VkSamplerAddressMode >( samplerInfo.addressModeW );
    samplerCreateInfo.mipLodBias = samplerInfo.mipLodBias;
    samplerCreateInfo.anisotropyEnable = samplerInfo.anisotropy;
    samplerCreateInfo.maxAnisotropy = (
        samplerCreateInfo.anisotropyEnable ? samplerInfo.maxAnisotropy : 1.0f );
    samplerCreateInfo.compareEnable = samplerInfo.compare;
    samplerCreateInfo.compareOp = static_cast< VkCompareOp >( samplerInfo.compareOp );
    samplerCreateInfo.minLod = samplerInfo.minLod;
    samplerCreateInfo.maxLod = samplerInfo.maxLod;
    samplerCreateInfo.borderColor = static_cast< VkBorderColor >( samplerInfo.borderColor );
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    d_result = ::vkCreateSampler (
        d_hDevice.handle(), & samplerCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

SamplerImpl :: SamplerImpl (
    const Device& hDevice, const SUnnormalizedSampler& samplerInfo ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result()
{
    VkSamplerCreateInfo samplerCreateInfo;

    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = 0;
    samplerCreateInfo.flags = 0;
    samplerCreateInfo.magFilter = static_cast< VkFilter >( samplerInfo.filterMode );
    samplerCreateInfo.minFilter = static_cast< VkFilter >( samplerInfo.filterMode );
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    samplerCreateInfo.addressModeU = ( samplerInfo.clampToBorderU ?
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE );

    samplerCreateInfo.addressModeV = ( samplerInfo.clampToBorderV ?
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE );

    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    samplerCreateInfo.mipLodBias = samplerInfo.mipLodBias;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1.0f;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0;
    samplerCreateInfo.maxLod = 0;
    samplerCreateInfo.borderColor = static_cast< VkBorderColor >( samplerInfo.borderColor );
    samplerCreateInfo.unnormalizedCoordinates = VK_TRUE;

    d_result = ::vkCreateSampler (
        d_hDevice.handle(), & samplerCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

SamplerImpl :: ~SamplerImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroySampler ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

SNormalizedSampler :: SNormalizedSampler ( float maxLod )
{
    addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    compareOp = VK_COMPARE_OP_NEVER;
    compare = false;
    magFilterMode = VK_FILTER_LINEAR;
    minFilterMode = VK_FILTER_LINEAR;
    mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    anisotropy = false;
    mipLodBias = 0;
    maxAnisotropy = 0;
    minLod = 0;
    maxLod = maxLod;
}

// -----------------------------------------------------------------------------

SUnnormalizedSampler :: SUnnormalizedSampler()
{
    clampToBorderU = false;
    clampToBorderV = false;
    filterMode = VK_FILTER_LINEAR;
    borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    mipLodBias = 0;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
