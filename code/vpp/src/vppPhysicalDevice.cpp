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
#include "../include/vppPhysicalDevice.hpp"
#include "../include/vppSurface.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

PhysicalDeviceImpl :: PhysicalDeviceImpl ( VkPhysicalDevice hDevice ) :
    d_handle ( hDevice )
{
    std::uint32_t nFamilies = 0;
    ::vkGetPhysicalDeviceQueueFamilyProperties ( d_handle, & nFamilies, 0 );

    d_queueFamilyProperties.resize ( nFamilies );
    ::vkGetPhysicalDeviceQueueFamilyProperties ( d_handle, & nFamilies, & d_queueFamilyProperties [ 0 ] );

    ::vkGetPhysicalDeviceFeatures ( d_handle, & d_supportedFeatures );
}

// -----------------------------------------------------------------------------

PhysicalDeviceImpl :: ~PhysicalDeviceImpl()
{
}

// -----------------------------------------------------------------------------

size_t PhysicalDevice :: queueFamilyCount() const
{
    return get()->d_queueFamilyProperties.size();
}

// -----------------------------------------------------------------------------

const VkQueueFamilyProperties& PhysicalDevice :: getQueueFamilyProperties ( size_t iFamily ) const
{
    return get()->d_queueFamilyProperties [ iFamily ];
}

// -----------------------------------------------------------------------------

bool PhysicalDevice :: supportsSurface ( const Surface& surface, size_t iFamily ) const
{
    VkBool32 bSupported = VK_FALSE;

    const VkResult result = ::vkGetPhysicalDeviceSurfaceSupportKHR (
        get()->d_handle,
        static_cast< std::uint32_t >( iFamily ),
        surface.handle(), & bSupported );

    return result == VK_SUCCESS && bSupported;
}

// -----------------------------------------------------------------------------

void PhysicalDevice :: getLimitValuesAsText ( std::ostream& sst ) const
{
    const VkPhysicalDeviceProperties dp = getPhysicalDeviceProperties();
    const VkPhysicalDeviceLimits& dl = dp.limits;

    sst << "Device limits: " << '\n';
    sst << '\n';
    sst << "maxImageDimension1D: " << dl.maxImageDimension1D << '\n';
    sst << "maxImageDimension2D: " << dl.maxImageDimension2D << '\n';
    sst << "maxImageDimension3D: " << dl.maxImageDimension3D << '\n';
    sst << "maxImageDimensionCube: " << dl.maxImageDimensionCube << '\n';
    sst << "maxImageArrayLayers: " << dl.maxImageArrayLayers << '\n';
    sst << "maxTexelBufferElements: " << dl.maxTexelBufferElements << '\n';
    sst << "maxUniformBufferRange: " << dl.maxUniformBufferRange << '\n';
    sst << "maxStorageBufferRange: " << dl.maxStorageBufferRange << '\n';
    sst << "maxPushConstantsSize: " << dl.maxPushConstantsSize << '\n';
    sst << "maxMemoryAllocationCount: " << dl.maxMemoryAllocationCount << '\n';
    sst << "maxSamplerAllocationCount: " << dl.maxSamplerAllocationCount << '\n';
    sst << "bufferImageGranularity: " << dl.bufferImageGranularity << '\n';
    sst << "sparseAddressSpaceSize: " << dl.sparseAddressSpaceSize << '\n';
    sst << "maxBoundDescriptorSets: " << dl.maxBoundDescriptorSets << '\n';
    sst << "maxPerStageDescriptorSamplers: " << dl.maxPerStageDescriptorSamplers << '\n';
    sst << "maxPerStageDescriptorUniformBuffers: " << dl.maxPerStageDescriptorUniformBuffers << '\n';
    sst << "maxPerStageDescriptorStorageBuffers: " << dl.maxPerStageDescriptorStorageBuffers << '\n';
    sst << "maxPerStageDescriptorSampledImages: " << dl.maxPerStageDescriptorSampledImages << '\n';
    sst << "maxPerStageDescriptorStorageImages: " << dl.maxPerStageDescriptorStorageImages << '\n';
    sst << "maxPerStageDescriptorInputAttachments: " << dl.maxPerStageDescriptorInputAttachments << '\n';
    sst << "maxPerStageResources: " << dl.maxPerStageResources << '\n';
    sst << "maxDescriptorSetSamplers: " << dl.maxDescriptorSetSamplers << '\n';
    sst << "maxDescriptorSetUniformBuffers: " << dl.maxDescriptorSetUniformBuffers << '\n';
    sst << "maxDescriptorSetUniformBuffersDynamic: " << dl.maxDescriptorSetUniformBuffersDynamic << '\n';
    sst << "maxDescriptorSetStorageBuffers: " << dl.maxDescriptorSetStorageBuffers << '\n';
    sst << "maxDescriptorSetStorageBuffersDynamic: " << dl.maxDescriptorSetStorageBuffersDynamic << '\n';
    sst << "maxDescriptorSetSampledImages: " << dl.maxDescriptorSetSampledImages << '\n';
    sst << "maxDescriptorSetStorageImages: " << dl.maxDescriptorSetStorageImages << '\n';
    sst << "maxDescriptorSetInputAttachments: " << dl.maxDescriptorSetInputAttachments << '\n';
    sst << "maxVertexInputAttributes: " << dl.maxVertexInputAttributes << '\n';
    sst << "maxVertexInputBindings: " << dl.maxVertexInputBindings << '\n';
    sst << "maxVertexInputAttributeOffset: " << dl.maxVertexInputAttributeOffset << '\n';
    sst << "maxVertexInputBindingStride: " << dl.maxVertexInputBindingStride << '\n';
    sst << "maxVertexOutputComponents: " << dl.maxVertexOutputComponents << '\n';
    sst << "maxTessellationGenerationLevel: " << dl.maxTessellationGenerationLevel << '\n';
    sst << "maxTessellationPatchSize: " << dl.maxTessellationPatchSize << '\n';
    sst << "maxTessellationControlPerVertexInputComponents: " << dl.maxTessellationControlPerVertexInputComponents << '\n';
    sst << "maxTessellationControlPerVertexOutputComponents: " << dl.maxTessellationControlPerVertexOutputComponents << '\n';
    sst << "maxTessellationControlPerPatchOutputComponents: " << dl.maxTessellationControlPerPatchOutputComponents << '\n';
    sst << "maxTessellationControlTotalOutputComponents: " << dl.maxTessellationControlTotalOutputComponents << '\n';
    sst << "maxTessellationEvaluationInputComponents: " << dl.maxTessellationEvaluationInputComponents << '\n';
    sst << "maxTessellationEvaluationOutputComponents: " << dl.maxTessellationEvaluationOutputComponents << '\n';
    sst << "maxGeometryShaderInvocations: " << dl.maxGeometryShaderInvocations << '\n';
    sst << "maxGeometryInputComponents: " << dl.maxGeometryInputComponents << '\n';
    sst << "maxGeometryOutputComponents: " << dl.maxGeometryOutputComponents << '\n';
    sst << "maxGeometryOutputVertices: " << dl.maxGeometryOutputVertices << '\n';
    sst << "maxGeometryTotalOutputComponents: " << dl.maxGeometryTotalOutputComponents << '\n';
    sst << "maxFragmentInputComponents: " << dl.maxFragmentInputComponents << '\n';
    sst << "maxFragmentOutputAttachments: " << dl.maxFragmentOutputAttachments << '\n';
    sst << "maxFragmentDualSrcAttachments: " << dl.maxFragmentDualSrcAttachments << '\n';
    sst << "maxFragmentCombinedOutputResources: " << dl.maxFragmentCombinedOutputResources << '\n';
    sst << "maxComputeSharedMemorySize: " << dl.maxComputeSharedMemorySize << '\n';
    sst << "maxComputeWorkGroupCount[0]: " << dl.maxComputeWorkGroupCount[0] << '\n';
    sst << "maxComputeWorkGroupCount[1]: " << dl.maxComputeWorkGroupCount[1] << '\n';
    sst << "maxComputeWorkGroupCount[2]: " << dl.maxComputeWorkGroupCount[2] << '\n';
    sst << "maxComputeWorkGroupInvocations: " << dl.maxComputeWorkGroupInvocations << '\n';
    sst << "maxComputeWorkGroupSize[0]: " << dl.maxComputeWorkGroupSize[0] << '\n';
    sst << "maxComputeWorkGroupSize[1]: " << dl.maxComputeWorkGroupSize[1] << '\n';
    sst << "maxComputeWorkGroupSize[2]: " << dl.maxComputeWorkGroupSize[2] << '\n';
    sst << "subPixelPrecisionBits: " << dl.subPixelPrecisionBits << '\n';
    sst << "subTexelPrecisionBits: " << dl.subTexelPrecisionBits << '\n';
    sst << "mipmapPrecisionBits: " << dl.mipmapPrecisionBits << '\n';
    sst << "maxDrawIndexedIndexValue: " << dl.maxDrawIndexedIndexValue << '\n';
    sst << "maxDrawIndirectCount: " << dl.maxDrawIndirectCount << '\n';
    sst << "maxSamplerLodBias: " << dl.maxSamplerLodBias << '\n';
    sst << "maxSamplerAnisotropy: " << dl.maxSamplerAnisotropy << '\n';
    sst << "maxViewports: " << dl.maxViewports << '\n';
    sst << "maxViewportDimensions[0]: " << dl.maxViewportDimensions[0] << '\n';
    sst << "maxViewportDimensions[1]: " << dl.maxViewportDimensions[1] << '\n';
    sst << "viewportBoundsRange[0]: " << dl.viewportBoundsRange[0] << '\n';
    sst << "viewportBoundsRange[1]: " << dl.viewportBoundsRange[1] << '\n';
    sst << "viewportSubPixelBits: " << dl.viewportSubPixelBits << '\n';
    sst << "minMemoryMapAlignment: " << dl.minMemoryMapAlignment << '\n';
    sst << "minTexelBufferOffsetAlignment: " << dl.minTexelBufferOffsetAlignment << '\n';
    sst << "minUniformBufferOffsetAlignment: " << dl.minUniformBufferOffsetAlignment << '\n';
    sst << "minStorageBufferOffsetAlignment: " << dl.minStorageBufferOffsetAlignment << '\n';
    sst << "minTexelOffset: " << dl.minTexelOffset << '\n';
    sst << "maxTexelOffset: " << dl.maxTexelOffset << '\n';
    sst << "minTexelGatherOffset: " << dl.minTexelGatherOffset << '\n';
    sst << "maxTexelGatherOffset: " << dl.maxTexelGatherOffset << '\n';
    sst << "minInterpolationOffset: " << dl.minInterpolationOffset << '\n';
    sst << "maxInterpolationOffset: " << dl.maxInterpolationOffset << '\n';
    sst << "subPixelInterpolationOffsetBits: " << dl.subPixelInterpolationOffsetBits << '\n';
    sst << "maxFramebufferWidth: " << dl.maxFramebufferWidth << '\n';
    sst << "maxFramebufferHeight: " << dl.maxFramebufferHeight << '\n';
    sst << "maxFramebufferLayers: " << dl.maxFramebufferLayers << '\n';
    sst << "framebufferColorSampleCounts: " << dl.framebufferColorSampleCounts << '\n';
    sst << "framebufferDepthSampleCounts: " << dl.framebufferDepthSampleCounts << '\n';
    sst << "framebufferStencilSampleCounts: " << dl.framebufferStencilSampleCounts << '\n';
    sst << "framebufferNoAttachmentsSampleCounts: " << dl.framebufferNoAttachmentsSampleCounts << '\n';
    sst << "maxColorAttachments: " << dl.maxColorAttachments << '\n';
    sst << "sampledImageColorSampleCounts: " << dl.sampledImageColorSampleCounts << '\n';
    sst << "sampledImageIntegerSampleCounts: " << dl.sampledImageIntegerSampleCounts << '\n';
    sst << "sampledImageDepthSampleCounts: " << dl.sampledImageDepthSampleCounts << '\n';
    sst << "sampledImageStencilSampleCounts: " << dl.sampledImageStencilSampleCounts << '\n';
    sst << "storageImageSampleCounts: " << dl.storageImageSampleCounts << '\n';
    sst << "maxSampleMaskWords: " << dl.maxSampleMaskWords << '\n';
    sst << "timestampComputeAndGraphics: " << dl.timestampComputeAndGraphics << '\n';
    sst << "timestampPeriod: " << dl.timestampPeriod << '\n';
    sst << "maxClipDistances: " << dl.maxClipDistances << '\n';
    sst << "maxCullDistances: " << dl.maxCullDistances << '\n';
    sst << "maxCombinedClipAndCullDistances: " << dl.maxCombinedClipAndCullDistances << '\n';
    sst << "discreteQueuePriorities: " << dl.discreteQueuePriorities << '\n';
    sst << "pointSizeRange[0]: " << dl.pointSizeRange[0] << '\n';
    sst << "pointSizeRange[1]: " << dl.pointSizeRange[1] << '\n';
    sst << "lineWidthRange[0]: " << dl.lineWidthRange[0] << '\n';
    sst << "lineWidthRange[1]: " << dl.lineWidthRange[1] << '\n';
    sst << "pointSizeGranularity: " << dl.pointSizeGranularity << '\n';
    sst << "lineWidthGranularity: " << dl.lineWidthGranularity << '\n';
    sst << "strictLines: " << dl.strictLines << '\n';
    sst << "standardSampleLocations: " << dl.standardSampleLocations << '\n';
    sst << "optimalBufferCopyOffsetAlignment: " << dl.optimalBufferCopyOffsetAlignment << '\n';
    sst << "optimalBufferCopyRowPitchAlignment: " << dl.optimalBufferCopyRowPitchAlignment << '\n';
    sst << "nonCoherentAtomSize: " << dl.nonCoherentAtomSize << '\n';
}

// -----------------------------------------------------------------------------

VkFormatFeatureFlags PhysicalDevice :: supportsFormat ( VkFormat fmt, EFormatUsage u ) const
{
    VkFormatProperties formatProperties;

    ::vkGetPhysicalDeviceFormatProperties (
        get()->d_handle, fmt, & formatProperties );

    switch ( u )
    {
        case LINEAR_TILING: return formatProperties.linearTilingFeatures;
        case BUFFER: return formatProperties.bufferFeatures;
        default: return formatProperties.optimalTilingFeatures;
    }
}

// -----------------------------------------------------------------------------

bool PhysicalDevice :: supportsDepthStencilFormat ( VkFormat fmt ) const
{
    return ( supportsFormat ( fmt )
        & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT ) != 0;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
