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
#include "../include/vppDebugProbe.hpp"
#include "../include/vppShaderDataBlock.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

IDebugProbe :: ~IDebugProbe()
{
}

// -----------------------------------------------------------------------------

void IDebugProbe :: bind (
    VkImageView hView,
    std::uint32_t set, std::uint32_t binding, 
    const ShaderDataBlock& hDataBlock )
{
    VkDescriptorImageInfo imageInfo;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    imageInfo.sampler = 0;
    imageInfo.imageView = hView;

    VkWriteDescriptorSet singleSet;
    singleSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    singleSet.pNext = 0;
    singleSet.dstSet = hDataBlock.getDescriptorSet ( set );
    singleSet.dstBinding = binding;
    singleSet.dstArrayElement = 0;
    singleSet.descriptorCount = 1;
    singleSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    singleSet.pImageInfo = & imageInfo;
    singleSet.pBufferInfo = 0;
    singleSet.pTexelBufferView = 0;

    ::vkUpdateDescriptorSets (
        hDataBlock.device().handle(),
        1, & singleSet,
        0, 0 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
