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
#include "../include/vppPhysicalDevice.hpp"
#include "../include/vppSurface.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

const KFeatureNames s_featureNames;

// -----------------------------------------------------------------------------

KFeatureNames :: KFeatureNames() :
    TFeatureNames< VkPhysicalDeviceFeatures >{
        { fRobustBufferAccess, "RobustBufferAccess" },
        { fFullDrawIndexUint32, "FullDrawIndexUint32" },
        { fImageCubeArray, "ImageCubeArray" },
        { fIndependentBlend, "IndependentBlend" },
        { fGeometryShader, "GeometryShader" },
        { fTessellationShader, "TessellationShader" },
        { fSampleRateShading, "SampleRateShading" },
        { fDualSrcBlend, "DualSrcBlend" },
        { fLogicOp, "LogicOp" },
        { fMultiDrawIndirect, "MultiDrawIndirect" },
        { fDrawIndirectFirstInstance, "DrawIndirectFirstInstance" },
        { fDepthClamp, "DepthClamp" },
        { fDepthBiasClamp, "DepthBiasClamp" },
        { fFillModeNonSolid, "FillModeNonSolid" },
        { fDepthBounds, "DepthBounds" },
        { fWideLines, "WideLines" },
        { fLargePoints, "LargePoints" },
        { fAlphaToOne, "AlphaToOne" },
        { fMultiViewport, "MultiViewport" },
        { fSamplerAnisotropy, "SamplerAnisotropy" },
        { fTextureCompressionETC2, "TextureCompressionETC2" },
        { fTextureCompressionASTC_LDR, "TextureCompressionASTC_LDR" },
        { fTextureCompressionBC, "TextureCompressionBC" },
        { fOcclusionQueryPrecise, "OcclusionQueryPrecise" },
        { fPipelineStatisticsQuery, "PipelineStatisticsQuery" },
        { fVertexPipelineStoresAndAtomics, "VertexPipelineStoresAndAtomics" },
        { fFragmentStoresAndAtomics, "FragmentStoresAndAtomics" },
        { fShaderTessellationAndGeometryPointSize, "ShaderTessellationAndGeometryPointSize" },
        { fShaderImageGatherExtended, "ShaderImageGatherExtended" },
        { fShaderStorageImageExtendedFormats, "ShaderStorageImageExtendedFormats" },
        { fShaderStorageImageMultisample, "ShaderStorageImageMultisample" },
        { fShaderStorageImageReadWithoutFormat, "ShaderStorageImageReadWithoutFormat" },
        { fShaderStorageImageWriteWithoutFormat, "ShaderStorageImageWriteWithoutFormat" },
        { fShaderUniformBufferArrayDynamicIndexing, "ShaderUniformBufferArrayDynamicIndexing" },
        { fShaderSampledImageArrayDynamicIndexing, "ShaderSampledImageArrayDynamicIndexing" },
        { fShaderStorageBufferArrayDynamicIndexing, "ShaderStorageBufferArrayDynamicIndexing" },
        { fShaderStorageImageArrayDynamicIndexing, "ShaderStorageImageArrayDynamicIndexing" },
        { fShaderClipDistance, "ShaderClipDistance" },
        { fShaderCullDistance, "ShaderCullDistance" },
        { fShaderFloat64, "ShaderFloat64" },
        { fShaderInt64, "ShaderInt64" },
        { fShaderInt16, "ShaderInt16" },
        { fShaderResourceResidency, "ShaderResourceResidency" },
        { fShaderResourceMinLod, "ShaderResourceMinLod" },
        { fSparseBinding, "SparseBinding" },
        { fSparseResidencyBuffer, "SparseResidencyBuffer" },
        { fSparseResidencyImage2D, "SparseResidencyImage2D" },
        { fSparseResidencyImage3D, "SparseResidencyImage3D" },
        { fSparseResidency2Samples, "SparseResidency2Samples" },
        { fSparseResidency4Samples, "SparseResidency4Samples" },
        { fSparseResidency8Samples, "SparseResidency8Samples" },
        { fSparseResidency16Samples, "SparseResidency16Samples" },
        { fSparseResidencyAliased, "SparseResidencyAliased" },
        { fVariableMultisampleRate, "VariableMultisampleRate" },
        { fInheritedQueries, "InheritedQueries" }
    },
    TFeatureNames< VkPhysicalDeviceMultiviewFeatures >{
        { fMultiview, "Multiview" },
        { fMultiviewGeometryShader, "MultiviewGeometryShader" },
        { fMultiviewTessellationShader, "MultiviewTessellationShader" }
    },
    TFeatureNames< VkPhysicalDeviceVariablePointerFeatures >{
        { fVariablePointersStorageBuffer, "VariablePointersStorageBuffer" },
        { fVariablePointers, "VariablePointers" }
    },
    TFeatureNames< VkPhysicalDevice8BitStorageFeaturesKHR >{
        { fStorageBuffer8BitAccess, "StorageBuffer8BitAccess" },
        { fUniformAndStorageBuffer8BitAccess, "UniformAndStorageBuffer8BitAccess" },
        { fStoragePushConstant8, "StoragePushConstant8" }
    },
    TFeatureNames< VkPhysicalDeviceShaderAtomicInt64FeaturesKHR >{
        { fShaderBufferInt64Atomics, "ShaderBufferInt64Atomics" },
        { fShaderSharedInt64Atomics, "ShaderSharedInt64Atomics" }
    },
    TFeatureNames< VkPhysicalDevice16BitStorageFeatures >{
        { fStorageBuffer16BitAccess, "StorageBuffer16BitAccess" },
        { fUniformAndStorageBuffer16BitAccess, "UniformAndStorageBuffer16BitAccess" },
        { fStoragePushConstant16, "StoragePushConstant16" },
        { fStorageInputOutput16, "StorageInputOutput16" },
    },
    TFeatureNames< VkPhysicalDeviceFloat16Int8FeaturesKHR >{
        { fShaderFloat16, "ShaderFloat16" },
        { fShaderInt8, "ShaderInt8" }
    },
    TFeatureNames< VkPhysicalDeviceSamplerYcbcrConversionFeatures >{
        { fSamplerYcbcrConversion, "SamplerYcbcrConversion" }
    },
    TFeatureNames< VkPhysicalDeviceProtectedMemoryFeatures >{
        { fProtectedMemory, "ProtectedMemory" }
    },
    TFeatureNames< VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT >{
        { fAdvancedBlendCoherentOperations, "AdvancedBlendCoherentOperations" }
    },
    TFeatureNames< VkPhysicalDeviceConditionalRenderingFeaturesEXT >{
        { fConditionalRendering, "ConditionalRendering" },
        { fInheritedConditionalRendering, "InheritedConditionalRendering" }
    },
    TFeatureNames< VkPhysicalDeviceShaderDrawParameterFeatures >{
        { fShaderDrawParameters, "ShaderDrawParameters" }
    },
    TFeatureNames< VkPhysicalDeviceDescriptorIndexingFeaturesEXT >{
        { fShaderInputAttachmentArrayDynamicIndexing, "ShaderInputAttachmentArrayDynamicIndexing" },
        { fShaderUniformTexelBufferArrayDynamicIndexing, "ShaderUniformTexelBufferArrayDynamicIndexing" },
        { fShaderStorageTexelBufferArrayDynamicIndexing, "ShaderStorageTexelBufferArrayDynamicIndexing" },
        { fShaderUniformBufferArrayNonUniformIndexing, "ShaderUniformBufferArrayNonUniformIndexing" },
        { fShaderSampledImageArrayNonUniformIndexing, "ShaderSampledImageArrayNonUniformIndexing" },
        { fShaderStorageBufferArrayNonUniformIndexing, "ShaderStorageBufferArrayNonUniformIndexing" },
        { fShaderStorageImageArrayNonUniformIndexing, "ShaderStorageImageArrayNonUniformIndexing" },
        { fShaderInputAttachmentArrayNonUniformIndexing, "ShaderInputAttachmentArrayNonUniformIndexing" },
        { fShaderUniformTexelBufferArrayNonUniformIndexing, "ShaderUniformTexelBufferArrayNonUniformIndexing" },
        { fShaderStorageTexelBufferArrayNonUniformIndexing, "ShaderStorageTexelBufferArrayNonUniformIndexing" },
        { fDescriptorBindingUniformBufferUpdateAfterBind, "DescriptorBindingUniformBufferUpdateAfterBind" },
        { fDescriptorBindingSampledImageUpdateAfterBind, "DescriptorBindingSampledImageUpdateAfterBind" },
        { fDescriptorBindingStorageImageUpdateAfterBind, "DescriptorBindingStorageImageUpdateAfterBind" },
        { fDescriptorBindingStorageBufferUpdateAfterBind, "DescriptorBindingStorageBufferUpdateAfterBind" },
        { fDescriptorBindingUniformTexelBufferUpdateAfterBind, "DescriptorBindingUniformTexelBufferUpdateAfterBind" },
        { fDescriptorBindingStorageTexelBufferUpdateAfterBind, "DescriptorBindingStorageTexelBufferUpdateAfterBind" },
        { fDescriptorBindingUpdateUnusedWhilePending, "DescriptorBindingUpdateUnusedWhilePending" },
        { fDescriptorBindingPartiallyBound, "DescriptorBindingPartiallyBound" },
        { fDescriptorBindingVariableDescriptorCount, "DescriptorBindingVariableDescriptorCount" },
        { fRuntimeDescriptorArray, "RuntimeDescriptorArray" }
    },
    TFeatureNames< VkPhysicalDeviceASTCDecodeFeaturesEXT >{
        { fDecodeModeSharedExponent, "DecodeModeSharedExponent" }
    },
    TFeatureNames< VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT >{
        { fVertexAttributeInstanceRateDivisor, "VertexAttributeInstanceRateDivisor" },
        { fVertexAttributeInstanceRateZeroDivisor, "VertexAttributeInstanceRateZeroDivisor" }
    },
    TFeatureNames< VkPhysicalDeviceTransformFeedbackFeaturesEXT >{
        { fTransformFeedback, "TransformFeedback" },
        { fGeometryStreams, "GeometryStreams" }
    },
    TFeatureNames< VkPhysicalDeviceVulkanMemoryModelFeaturesKHR >{
        { fVulkanMemoryModel, "VulkanMemoryModel" },
        { fVulkanMemoryModelDeviceScope, "VulkanMemoryModelDeviceScope" },
        { fVulkanMemoryModelAvailabilityVisibilityChains, "VulkanMemoryModelAvailabilityVisibilityChains" }
    },
    TFeatureNames< VkPhysicalDeviceInlineUniformBlockFeaturesEXT >{
        { fInlineUniformBlock, "InlineUniformBlock" },
        { fDescriptorBindingInlineUniformBlockUpdateAfterBind, "DescriptorBindingInlineUniformBlockUpdateAfterBind" }
    },
    TFeatureNames< VkPhysicalDeviceFragmentDensityMapFeaturesEXT >{
        { fFragmentDensityMap, "FragmentDensityMap" },
        { fFragmentDensityMapDynamic, "FragmentDensityMapDynamic" },
        { fFragmentDensityMapNonSubsampledImages, "FragmentDensityMapNonSubsampledImages" }
    },
    TFeatureNames< VkPhysicalDeviceScalarBlockLayoutFeaturesEXT >{
        { fScalarBlockLayout, "ScalarBlockLayout" }
    },
    TFeatureNames< VkPhysicalDeviceDepthClipEnableFeaturesEXT >{
        { fDepthClipEnable, "DepthClipEnable" }
    },
    TFeatureNames< VkPhysicalDeviceMemoryPriorityFeaturesEXT >{
        { fMemoryPriority, "MemoryPriority" }
    },
    TFeatureNames< VkPhysicalDeviceBufferAddressFeaturesEXT >{
        { fBufferDeviceAddress, "BufferDeviceAddress" },
        { fBufferDeviceAddressCaptureReplay, "BufferDeviceAddressCaptureReplay" },
        { fBufferDeviceAddressMultiDevice, "BufferDeviceAddressMultiDevice" }
    },
    TFeatureNames< SExtensionsAsFeatures >{
        { fExtBlendOperationAdvanced, "VK_EXT_blend_operation_advanced" },
        { fExtConditionalRendering, "VK_EXT_conditional_rendering" },
        { fExtConservativeRasterization, "VK_EXT_conservative_rasterization" },
        { fExtDepthRangeUnrestricted, "VK_EXT_depth_range_unrestricted" },
        { fExtDescriptorIndexing, "VK_EXT_descriptor_indexing" },
        { fExtDiscardRectangles, "VK_EXT_discard_rectangles" },
        { fExtExternalMemoryHost, "VK_EXT_external_memory_host" },
        { fExtHdrMetadata, "VK_EXT_hdr_metadata" },
        { fExtInlineUniformBlock, "VK_EXT_inline_uniform_block" },
        { fExtPostDepthCoverage, "VK_EXT_post_depth_coverage" },
        { fExtSampleLocations, "VK_EXT_sample_locations" },
        { fExtSamplerFilterMinmax, "VK_EXT_sampler_filter_minmax" },
        { fExtScalarBlockLayout, "VK_EXT_scalar_block_layout" },
        { fExtShaderSubgroupBallot, "VK_EXT_shader_subgroup_ballot" },
        { fExtShaderSubgroupVote, "VK_EXT_shader_subgroup_vote" },
        { fExtShaderViewportIndexLayer, "VK_EXT_shader_viewport_index_layer" },
        { fExtTransformFeedback, "VK_EXT_transform_feedback" },
        { fExtVertexAttributeDivisor, "VK_EXT_vertex_attribute_divisor" },
        { fExtStorage16bit, "VK_KHR_16bit_storage" },
        { fExtStorage8bit, "VK_KHR_8bit_storage" },
        { fExtBindMemory2, "VK_KHR_bind_memory2" },
        { fExtCreateRenderpass2, "VK_KHR_create_renderpass2" },
        { fExtDedicatedAllocation, "VK_KHR_dedicated_allocation" },
        { fExtDepthStencilResolve, "VK_KHR_depth_stencil_resolve" },
        { fExtDescriptorUpdateTemplate, "VK_KHR_descriptor_update_template" },
        { fExtDeviceGroup, "VK_KHR_device_group" },
        { fExtDrawIndirectCount, "VK_KHR_draw_indirect_count" },
        { fExtDriverProperties, "VK_KHR_driver_properties" },
        { fExtExternalFence, "VK_KHR_external_fence" },
        { fExtExternalFenceWin32, "VK_KHR_external_fence_win32" },
        { fExtExternalMemory, "VK_KHR_external_memory" },
        { fExtExternalMemoryWin32, "VK_KHR_external_memory_win32" },
        { fExtExternalSemaphore, "VK_KHR_external_semaphore" },
        { fExtExternalSemaphoreWin32, "VK_KHR_external_semaphore_win32" },
        { fExtGetMemoryRequirements2, "VK_KHR_get_memory_requirements2" },
        { fExtImageFormatList, "VK_KHR_image_format_list" },
        { fExtMultiview, "VK_KHR_multiview" },
        { fExtPushDescriptor, "VK_KHR_push_descriptor" },
        { fExtRelaxedBlockLayout, "VK_KHR_relaxed_block_layout" },
        { fExtSamplerMirrorClampToEdge, "VK_KHR_sampler_mirror_clamp_to_edge" },
        { fExtSamplerYcbcrConversion, "VK_KHR_sampler_ycbcr_conversion" },
        { fExtShaderAtomicInt64, "VK_KHR_shader_atomic_int64" },
        { fExtShaderDrawParameters, "VK_KHR_shader_draw_parameters" },
        { fExtShaderFloat16Int8, "VK_KHR_shader_float16_int8" },
        { fExtShaderFloatControls, "VK_KHR_shader_float_controls" },
        { fExtStorageBufferStorageClass, "VK_KHR_storage_buffer_storage_class" },
        { fExtSwapchain, "VK_KHR_swapchain" },
        { fExtSwapchainMutableFormat, "VK_KHR_swapchain_mutable_format" },
        { fExtVariablePointers, "VK_KHR_variable_pointers" },
        { fExtVulkanMemoryModel, "VK_KHR_vulkan_memory_model" },
        { fExtWin32KeyedMutex, "VK_KHR_win32_keyed_mutex" },
        { fExtFragmentDensityMap, "VK_EXT_fragment_density_map" },
        { fExtAstcDecodeMode, "VK_EXT_astc_decode_mode" },
        { fExtDepthClipEnable, "VK_EXT_depth_clip_enable" },
        { fExtMemoryPriority, "VK_EXT_memory_priority" },
        { fExtBufferDeviceAddress, "VK_EXT_buffer_device_address" }
    }
{
    typedef TFeatureNames< SExtensionsAsFeatures > ExtNames;
    const ExtNames* pExtNames = static_cast< const ExtNames* >( this );

    for ( auto iName = pExtNames->begin(); iName != pExtNames->end(); ++iName )
        d_name2extField.emplace ( iName->second, iName->first );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

PhysicalDeviceImpl :: PhysicalDeviceImpl ( VkPhysicalDevice hDevice ) :
    d_handle ( hDevice )
{
    std::uint32_t nFamilies = 0;
    ::vkGetPhysicalDeviceQueueFamilyProperties ( d_handle, & nFamilies, 0 );

    d_queueFamilyProperties.resize ( nFamilies );
    ::vkGetPhysicalDeviceQueueFamilyProperties ( d_handle, & nFamilies, & d_queueFamilyProperties [ 0 ] );

    ::vkGetPhysicalDeviceProperties ( d_handle, & d_properties );

    d_supportedFeatures.readSupported ( d_handle );
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
    const VkPhysicalDeviceProperties& dp = properties();
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

VkResult PhysicalDevice :: enumerateExtensions ( ExtensionProperties* pResult )
{
    std::uint32_t nProperties = 0;

    const VkResult hResult =
        ::vkEnumerateDeviceExtensionProperties ( get()->d_handle, 0, & nProperties, 0 );

    if ( hResult != VK_SUCCESS )
        return hResult;

    pResult->resize ( nProperties );

    return ::vkEnumerateDeviceExtensionProperties (
        get()->d_handle, 0, & nProperties, & ( *pResult )[ 0 ] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

DeviceFeatures :: DeviceFeatures()
{
    std::memset ( this, 0, sizeof ( *this ) );
    initStructures();
}

// -----------------------------------------------------------------------------

DeviceFeatures :: DeviceFeatures ( const DeviceFeatures& rhs )
{
    std::memcpy ( this, & rhs, sizeof ( *this ) );
    initStructures();
}

// -----------------------------------------------------------------------------

const DeviceFeatures& DeviceFeatures :: operator= ( const DeviceFeatures& rhs )
{
    std::memcpy ( this, & rhs, sizeof ( *this ) );
    initStructures();
    return *this;
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: readSupported ( VkPhysicalDevice hDevice )
{
    ::vkGetPhysicalDeviceFeatures2 ( hDevice, this );

    SExtensionsAsFeatures* pFeatures = this;

    ExtensionProperties extProperties;
    std::uint32_t nProperties = 0;

    const VkResult hResult =
        ::vkEnumerateDeviceExtensionProperties ( hDevice, 0, & nProperties, 0 );

    if ( hResult != VK_SUCCESS )
        return;

    extProperties.resize ( nProperties );

    ::vkEnumerateDeviceExtensionProperties (
        hDevice, 0, & nProperties, & extProperties [ 0 ] );

    for ( const auto& iProp : extProperties )
    {
        const std::string extName ( iProp.extensionName );
        const EFeatureX eFeature = detail::s_featureNames.getExtensionFeature ( extName );

        if ( eFeature != EFeatureX() )
            pFeatures->*eFeature = VK_TRUE;
    }
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: readSupported ( const PhysicalDevice& hDevice )
{
    readSupported ( hDevice.handle() );
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: addExtensionIfEnabled ( EFeatureX eExt, std::set< std::string >* pExtNames ) const
{
    const SExtensionsAsFeatures* pFeatures = this;

    if ( pFeatures->*eExt )
        pExtNames->emplace ( detail::s_featureNames.findName ( eExt ) );
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: resolveDependencies()
{
    // feature-feature dependencies

    if ( multiviewGeometryShader || multiviewTessellationShader )
        multiview = VK_TRUE;

    if ( variablePointers )
        variablePointersStorageBuffer = VK_TRUE;

    // feature-extension dependencies

    if ( multiview )
        extMultiview = VK_TRUE;

    if ( variablePointersStorageBuffer )
        extVariablePointers = VK_TRUE;

    if ( storageBuffer8BitAccess || uniformAndStorageBuffer8BitAccess || storagePushConstant8 )
        extStorage8bit = VK_TRUE;

    if ( shaderBufferInt64Atomics || shaderSharedInt64Atomics )
        extShaderAtomicInt64 = VK_TRUE;

    if ( storageBuffer16BitAccess || uniformAndStorageBuffer16BitAccess || storagePushConstant16 || storageInputOutput16 )
        extStorage16bit = VK_TRUE;

    if ( shaderFloat16 || shaderInt8 )
        extShaderFloat16Int8 = VK_TRUE;

    if ( samplerYcbcrConversion )
        extSamplerYcbcrConversion = VK_TRUE;

    if ( advancedBlendCoherentOperations )
        extBlendOperationAdvanced = VK_TRUE;

    if ( conditionalRendering || inheritedConditionalRendering )
        extConditionalRendering = VK_TRUE;

    if ( shaderInputAttachmentArrayDynamicIndexing
         || shaderUniformTexelBufferArrayDynamicIndexing
         || shaderStorageTexelBufferArrayDynamicIndexing
         || shaderUniformBufferArrayNonUniformIndexing
         || shaderSampledImageArrayNonUniformIndexing
         || shaderStorageBufferArrayNonUniformIndexing
         || shaderStorageImageArrayNonUniformIndexing
         || shaderInputAttachmentArrayNonUniformIndexing
         || shaderUniformTexelBufferArrayNonUniformIndexing
         || shaderStorageTexelBufferArrayNonUniformIndexing
         || descriptorBindingUniformBufferUpdateAfterBind
         || descriptorBindingSampledImageUpdateAfterBind
         || descriptorBindingStorageImageUpdateAfterBind
         || descriptorBindingStorageBufferUpdateAfterBind
         || descriptorBindingUniformTexelBufferUpdateAfterBind
         || descriptorBindingStorageTexelBufferUpdateAfterBind
         || descriptorBindingUpdateUnusedWhilePending
         || descriptorBindingPartiallyBound
         || descriptorBindingVariableDescriptorCount
         || runtimeDescriptorArray )
    {
        extDescriptorIndexing = VK_TRUE;
    }

    if ( decodeModeSharedExponent )
        extAstcDecodeMode = VK_TRUE;

    if ( vertexAttributeInstanceRateDivisor || vertexAttributeInstanceRateZeroDivisor )
        extVertexAttributeDivisor = VK_TRUE;

    if ( transformFeedback || geometryStreams )
        extTransformFeedback = VK_TRUE;

    if ( vulkanMemoryModel || vulkanMemoryModelDeviceScope || vulkanMemoryModelAvailabilityVisibilityChains )
        extVulkanMemoryModel = VK_TRUE;

    if ( inlineUniformBlock || descriptorBindingInlineUniformBlockUpdateAfterBind )
        extInlineUniformBlock = VK_TRUE;

    if ( fragmentDensityMap || fragmentDensityMapDynamic || fragmentDensityMapNonSubsampledImages )
        extFragmentDensityMap = VK_TRUE;

    if ( scalarBlockLayout )
        extScalarBlockLayout = VK_TRUE;

    if ( depthClipEnable )
        extDepthClipEnable = VK_TRUE;

    if ( memoryPriority )
        extMemoryPriority = VK_TRUE;

    if ( bufferDeviceAddress || bufferDeviceAddressCaptureReplay || bufferDeviceAddressMultiDevice )
        extBufferDeviceAddress = VK_TRUE;

    // extension-extension dependencies

    if ( extHdrMetadata )
        extSwapchain = VK_TRUE;

    if ( extStorage8bit || extStorage16bit || extVariablePointers )
        extStorageBufferStorageClass = VK_TRUE;

    if ( extCreateRenderpass2 )
        extMultiview = VK_TRUE;

    if ( extDedicatedAllocation )
        extGetMemoryRequirements2 = VK_TRUE;

    if ( extDepthStencilResolve )
        extCreateRenderpass2 = VK_TRUE;

    if ( extExternalFenceWin32 )
        extExternalFence = VK_TRUE;

    if ( extWin32KeyedMutex )
        extExternalMemoryWin32 = VK_TRUE;

    if ( extExternalMemoryWin32 || extExternalMemoryHost )
        extExternalMemory = VK_TRUE;

    if ( extExternalSemaphoreWin32 )
        extExternalSemaphore = VK_TRUE;

    if ( extSamplerYcbcrConversion )
    {
        extBindMemory2 = VK_TRUE;
        extGetMemoryRequirements2 = VK_TRUE;
    }

    if ( extSwapchainMutableFormat )
    {
        extSwapchain = VK_TRUE;
        extImageFormatList = VK_TRUE;
    }
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: getEnabledExtensions ( std::set< std::string >* pExtNames ) const
{
    addExtensionIfEnabled ( fExtBlendOperationAdvanced, pExtNames );
    addExtensionIfEnabled ( fExtConditionalRendering, pExtNames );
    addExtensionIfEnabled ( fExtConservativeRasterization, pExtNames );
    addExtensionIfEnabled ( fExtDepthRangeUnrestricted, pExtNames );
    addExtensionIfEnabled ( fExtDescriptorIndexing, pExtNames );
    addExtensionIfEnabled ( fExtDiscardRectangles, pExtNames );
    addExtensionIfEnabled ( fExtExternalMemoryHost, pExtNames );
    addExtensionIfEnabled ( fExtHdrMetadata, pExtNames );
    addExtensionIfEnabled ( fExtInlineUniformBlock, pExtNames );
    addExtensionIfEnabled ( fExtPostDepthCoverage, pExtNames );
    addExtensionIfEnabled ( fExtSampleLocations, pExtNames );
    addExtensionIfEnabled ( fExtSamplerFilterMinmax, pExtNames );
    addExtensionIfEnabled ( fExtScalarBlockLayout, pExtNames );
    addExtensionIfEnabled ( fExtShaderSubgroupBallot, pExtNames );
    addExtensionIfEnabled ( fExtShaderSubgroupVote, pExtNames );
    addExtensionIfEnabled ( fExtShaderViewportIndexLayer, pExtNames );
    addExtensionIfEnabled ( fExtTransformFeedback, pExtNames );
    addExtensionIfEnabled ( fExtVertexAttributeDivisor, pExtNames );
    addExtensionIfEnabled ( fExtStorage16bit, pExtNames );
    addExtensionIfEnabled ( fExtStorage8bit, pExtNames );
    addExtensionIfEnabled ( fExtBindMemory2, pExtNames );
    addExtensionIfEnabled ( fExtCreateRenderpass2, pExtNames );
    addExtensionIfEnabled ( fExtDedicatedAllocation, pExtNames );
    addExtensionIfEnabled ( fExtDepthStencilResolve, pExtNames );
    addExtensionIfEnabled ( fExtDescriptorUpdateTemplate, pExtNames );
    addExtensionIfEnabled ( fExtDeviceGroup, pExtNames );
    addExtensionIfEnabled ( fExtDrawIndirectCount, pExtNames );
    addExtensionIfEnabled ( fExtDriverProperties, pExtNames );
    addExtensionIfEnabled ( fExtExternalFence, pExtNames );
    addExtensionIfEnabled ( fExtExternalFenceWin32, pExtNames );
    addExtensionIfEnabled ( fExtExternalMemory, pExtNames );
    addExtensionIfEnabled ( fExtExternalMemoryWin32, pExtNames );
    addExtensionIfEnabled ( fExtExternalSemaphore, pExtNames );
    addExtensionIfEnabled ( fExtExternalSemaphoreWin32, pExtNames );
    addExtensionIfEnabled ( fExtGetMemoryRequirements2, pExtNames );
    addExtensionIfEnabled ( fExtImageFormatList, pExtNames );
    addExtensionIfEnabled ( fExtMultiview, pExtNames );
    addExtensionIfEnabled ( fExtPushDescriptor, pExtNames );
    addExtensionIfEnabled ( fExtRelaxedBlockLayout, pExtNames );
    addExtensionIfEnabled ( fExtSamplerMirrorClampToEdge, pExtNames );
    addExtensionIfEnabled ( fExtSamplerYcbcrConversion, pExtNames );
    addExtensionIfEnabled ( fExtShaderAtomicInt64, pExtNames );
    addExtensionIfEnabled ( fExtShaderDrawParameters, pExtNames );
    addExtensionIfEnabled ( fExtShaderFloat16Int8, pExtNames );
    addExtensionIfEnabled ( fExtShaderFloatControls, pExtNames );
    addExtensionIfEnabled ( fExtStorageBufferStorageClass, pExtNames );
    addExtensionIfEnabled ( fExtSwapchain, pExtNames );
    addExtensionIfEnabled ( fExtSwapchainMutableFormat, pExtNames );
    addExtensionIfEnabled ( fExtVariablePointers, pExtNames );
    addExtensionIfEnabled ( fExtVulkanMemoryModel, pExtNames );
    addExtensionIfEnabled ( fExtWin32KeyedMutex, pExtNames );
    addExtensionIfEnabled ( fExtFragmentDensityMap, pExtNames );
    addExtensionIfEnabled ( fExtAstcDecodeMode, pExtNames );
    addExtensionIfEnabled ( fExtDepthClipEnable, pExtNames );
    addExtensionIfEnabled ( fExtMemoryPriority, pExtNames );
    addExtensionIfEnabled ( fExtBufferDeviceAddress, pExtNames );
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: getSourceExtensions ( std::set< std::string >* pExtNames ) const
{
    const SExtensionsAsFeatures* pFeatures = this;

    if ( pFeatures->*fExtShaderAtomicInt64 )
    {
        pExtNames->emplace ( "GL_ARB_gpu_shader_int64" );
        pExtNames->emplace ( "GL_EXT_shader_atomic_int64" );
    }
}

// -----------------------------------------------------------------------------

void DeviceFeatures :: initStructures()
{
    VkPhysicalDeviceFeatures2* pFeatures2 = static_cast< VkPhysicalDeviceFeatures2* >( this );
    VkPhysicalDeviceMultiviewFeatures* pMultiviewFeatures = static_cast< VkPhysicalDeviceMultiviewFeatures* >( this );
    VkPhysicalDeviceVariablePointerFeatures* pVariablePointerFeatures = static_cast< VkPhysicalDeviceVariablePointerFeatures* >( this );
    VkPhysicalDevice8BitStorageFeaturesKHR* p8BitStorageFeatures = static_cast< VkPhysicalDevice8BitStorageFeaturesKHR* >( this );
    VkPhysicalDeviceShaderAtomicInt64FeaturesKHR* pShaderAtomicInt64Features = static_cast< VkPhysicalDeviceShaderAtomicInt64FeaturesKHR* >( this );
    VkPhysicalDevice16BitStorageFeatures* p16BitStorageFeatures = static_cast< VkPhysicalDevice16BitStorageFeatures* >( this );
    VkPhysicalDeviceFloat16Int8FeaturesKHR* pFloat16Int8Features = static_cast< VkPhysicalDeviceFloat16Int8FeaturesKHR* >( this );
    VkPhysicalDeviceSamplerYcbcrConversionFeatures* pSamplerYcbcrConversionFeatures = static_cast< VkPhysicalDeviceSamplerYcbcrConversionFeatures* >( this );
    VkPhysicalDeviceProtectedMemoryFeatures* pProtectedMemoryFeatures = static_cast< VkPhysicalDeviceProtectedMemoryFeatures* >( this );
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT* pBlendOperationAdvancedFeatures = static_cast< VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT* >( this );
    VkPhysicalDeviceConditionalRenderingFeaturesEXT* pConditionalRenderingFeatures = static_cast< VkPhysicalDeviceConditionalRenderingFeaturesEXT* >( this );
    VkPhysicalDeviceShaderDrawParameterFeatures* pShaderDrawParameterFeatures = static_cast< VkPhysicalDeviceShaderDrawParameterFeatures* >( this );
    VkPhysicalDeviceDescriptorIndexingFeaturesEXT* pDescriptorIndexingFeatures = static_cast< VkPhysicalDeviceDescriptorIndexingFeaturesEXT* >( this );
    VkPhysicalDeviceASTCDecodeFeaturesEXT* pASTCDecodeFeatures = static_cast< VkPhysicalDeviceASTCDecodeFeaturesEXT* >( this );
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* pVertexAttributeDivisorFeatures = static_cast< VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* >( this );
    VkPhysicalDeviceTransformFeedbackFeaturesEXT* pTransformFeedbackFeatures = static_cast< VkPhysicalDeviceTransformFeedbackFeaturesEXT* >( this );
    VkPhysicalDeviceVulkanMemoryModelFeaturesKHR* pVulkanMemoryModelFeatures = static_cast< VkPhysicalDeviceVulkanMemoryModelFeaturesKHR* >( this );
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT* pInlineUniformBlockFeatures = static_cast< VkPhysicalDeviceInlineUniformBlockFeaturesEXT* >( this );
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT* pFragmentDensityMapFeatures = static_cast< VkPhysicalDeviceFragmentDensityMapFeaturesEXT* >( this );
    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* pScalarBlockLayoutFeatures = static_cast< VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* >( this );
    VkPhysicalDeviceDepthClipEnableFeaturesEXT* pDepthClipEnableFeatures = static_cast< VkPhysicalDeviceDepthClipEnableFeaturesEXT* >( this );
    VkPhysicalDeviceMemoryPriorityFeaturesEXT* pMemoryPriorityFeatures = static_cast< VkPhysicalDeviceMemoryPriorityFeaturesEXT* >( this );
    VkPhysicalDeviceBufferAddressFeaturesEXT* pBufferAddressFeatures = static_cast< VkPhysicalDeviceBufferAddressFeaturesEXT* >( this );

    pFeatures2->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    pMultiviewFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
    pVariablePointerFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTER_FEATURES;
    p8BitStorageFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR;
    pShaderAtomicInt64Features->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR;
    p16BitStorageFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES;
    pFloat16Int8Features->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR;
    pSamplerYcbcrConversionFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES;
    pProtectedMemoryFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES;
    pBlendOperationAdvancedFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT;
    pConditionalRenderingFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT;
    pShaderDrawParameterFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETER_FEATURES;
    pDescriptorIndexingFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    pASTCDecodeFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT;
    pVertexAttributeDivisorFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT;
    pTransformFeedbackFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT;
    pVulkanMemoryModelFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES_KHR;
    pInlineUniformBlockFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT;
    pFragmentDensityMapFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT;
    pScalarBlockLayoutFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT;
    pDepthClipEnableFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT;
    pMemoryPriorityFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT;
    pBufferAddressFeatures->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_ADDRESS_FEATURES_EXT;

    pFeatures2->pNext = pMultiviewFeatures;
    pMultiviewFeatures->pNext = pVariablePointerFeatures;
    pVariablePointerFeatures->pNext = p8BitStorageFeatures;
    p8BitStorageFeatures->pNext = pShaderAtomicInt64Features;
    pShaderAtomicInt64Features->pNext = p16BitStorageFeatures;
    p16BitStorageFeatures->pNext = pFloat16Int8Features;
    pFloat16Int8Features->pNext = pSamplerYcbcrConversionFeatures;
    pSamplerYcbcrConversionFeatures->pNext = pProtectedMemoryFeatures;
    pProtectedMemoryFeatures->pNext = pBlendOperationAdvancedFeatures;
    pBlendOperationAdvancedFeatures->pNext = pConditionalRenderingFeatures;
    pConditionalRenderingFeatures->pNext = pShaderDrawParameterFeatures;
    pShaderDrawParameterFeatures->pNext = pDescriptorIndexingFeatures;
    pDescriptorIndexingFeatures->pNext = pASTCDecodeFeatures;
    pASTCDecodeFeatures->pNext = pVertexAttributeDivisorFeatures;
    pVertexAttributeDivisorFeatures->pNext = pTransformFeedbackFeatures;
    pTransformFeedbackFeatures->pNext = pVulkanMemoryModelFeatures;
    pVulkanMemoryModelFeatures->pNext = pInlineUniformBlockFeatures;
    pInlineUniformBlockFeatures->pNext = pFragmentDensityMapFeatures;
    pFragmentDensityMapFeatures->pNext = pScalarBlockLayoutFeatures;
    pScalarBlockLayoutFeatures->pNext = pDepthClipEnableFeatures;
    pDepthClipEnableFeatures->pNext = pMemoryPriorityFeatures;
    pMemoryPriorityFeatures->pNext = pBufferAddressFeatures;
    pBufferAddressFeatures->pNext = 0;
}

// -----------------------------------------------------------------------------

const EFeature fRobustBufferAccess = & VkPhysicalDeviceFeatures::robustBufferAccess;
const EFeature fFullDrawIndexUint32 = & VkPhysicalDeviceFeatures::fullDrawIndexUint32;
const EFeature fImageCubeArray = & VkPhysicalDeviceFeatures::imageCubeArray;
const EFeature fIndependentBlend = & VkPhysicalDeviceFeatures::independentBlend;
const EFeature fGeometryShader = & VkPhysicalDeviceFeatures::geometryShader;
const EFeature fTessellationShader = & VkPhysicalDeviceFeatures::tessellationShader;
const EFeature fSampleRateShading = & VkPhysicalDeviceFeatures::sampleRateShading;
const EFeature fDualSrcBlend = & VkPhysicalDeviceFeatures::dualSrcBlend;
const EFeature fLogicOp = & VkPhysicalDeviceFeatures::logicOp;
const EFeature fMultiDrawIndirect = & VkPhysicalDeviceFeatures::multiDrawIndirect;
const EFeature fDrawIndirectFirstInstance = & VkPhysicalDeviceFeatures::drawIndirectFirstInstance;
const EFeature fDepthClamp = & VkPhysicalDeviceFeatures::depthClamp;
const EFeature fDepthBiasClamp = & VkPhysicalDeviceFeatures::depthBiasClamp;
const EFeature fFillModeNonSolid = & VkPhysicalDeviceFeatures::fillModeNonSolid;
const EFeature fDepthBounds = & VkPhysicalDeviceFeatures::depthBounds;
const EFeature fWideLines = & VkPhysicalDeviceFeatures::wideLines;
const EFeature fLargePoints = & VkPhysicalDeviceFeatures::largePoints;
const EFeature fAlphaToOne = & VkPhysicalDeviceFeatures::alphaToOne;
const EFeature fMultiViewport = & VkPhysicalDeviceFeatures::multiViewport;
const EFeature fSamplerAnisotropy = & VkPhysicalDeviceFeatures::samplerAnisotropy;
const EFeature fTextureCompressionETC2 = & VkPhysicalDeviceFeatures::textureCompressionETC2;
const EFeature fTextureCompressionASTC_LDR = & VkPhysicalDeviceFeatures::textureCompressionASTC_LDR;
const EFeature fTextureCompressionBC = & VkPhysicalDeviceFeatures::textureCompressionBC;
const EFeature fOcclusionQueryPrecise = & VkPhysicalDeviceFeatures::occlusionQueryPrecise;
const EFeature fPipelineStatisticsQuery = & VkPhysicalDeviceFeatures::pipelineStatisticsQuery;
const EFeature fVertexPipelineStoresAndAtomics = & VkPhysicalDeviceFeatures::vertexPipelineStoresAndAtomics;
const EFeature fFragmentStoresAndAtomics = & VkPhysicalDeviceFeatures::fragmentStoresAndAtomics;
const EFeature fShaderTessellationAndGeometryPointSize = & VkPhysicalDeviceFeatures::shaderTessellationAndGeometryPointSize;
const EFeature fShaderImageGatherExtended = & VkPhysicalDeviceFeatures::shaderImageGatherExtended;
const EFeature fShaderStorageImageExtendedFormats = & VkPhysicalDeviceFeatures::shaderStorageImageExtendedFormats;
const EFeature fShaderStorageImageMultisample = & VkPhysicalDeviceFeatures::shaderStorageImageMultisample;
const EFeature fShaderStorageImageReadWithoutFormat = & VkPhysicalDeviceFeatures::shaderStorageImageReadWithoutFormat;
const EFeature fShaderStorageImageWriteWithoutFormat = & VkPhysicalDeviceFeatures::shaderStorageImageWriteWithoutFormat;
const EFeature fShaderUniformBufferArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderUniformBufferArrayDynamicIndexing;
const EFeature fShaderSampledImageArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderSampledImageArrayDynamicIndexing;
const EFeature fShaderStorageBufferArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderStorageBufferArrayDynamicIndexing;
const EFeature fShaderStorageImageArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderStorageImageArrayDynamicIndexing;
const EFeature fShaderClipDistance = & VkPhysicalDeviceFeatures::shaderClipDistance;
const EFeature fShaderCullDistance = & VkPhysicalDeviceFeatures::shaderCullDistance;
const EFeature fShaderFloat64 = & VkPhysicalDeviceFeatures::shaderFloat64;
const EFeature fShaderInt64 = & VkPhysicalDeviceFeatures::shaderInt64;
const EFeature fShaderInt16 = & VkPhysicalDeviceFeatures::shaderInt16;
const EFeature fShaderResourceResidency = & VkPhysicalDeviceFeatures::shaderResourceResidency;
const EFeature fShaderResourceMinLod = & VkPhysicalDeviceFeatures::shaderResourceMinLod;
const EFeature fSparseBinding = & VkPhysicalDeviceFeatures::sparseBinding;
const EFeature fSparseResidencyBuffer = & VkPhysicalDeviceFeatures::sparseResidencyBuffer;
const EFeature fSparseResidencyImage2D = & VkPhysicalDeviceFeatures::sparseResidencyImage2D;
const EFeature fSparseResidencyImage3D = & VkPhysicalDeviceFeatures::sparseResidencyImage3D;
const EFeature fSparseResidency2Samples = & VkPhysicalDeviceFeatures::sparseResidency2Samples;
const EFeature fSparseResidency4Samples = & VkPhysicalDeviceFeatures::sparseResidency4Samples;
const EFeature fSparseResidency8Samples = & VkPhysicalDeviceFeatures::sparseResidency8Samples;
const EFeature fSparseResidency16Samples = & VkPhysicalDeviceFeatures::sparseResidency16Samples;
const EFeature fSparseResidencyAliased = & VkPhysicalDeviceFeatures::sparseResidencyAliased;
const EFeature fVariableMultisampleRate = & VkPhysicalDeviceFeatures::variableMultisampleRate;
const EFeature fInheritedQueries = & VkPhysicalDeviceFeatures::inheritedQueries;

const EFeature1 fMultiview = & VkPhysicalDeviceMultiviewFeatures::multiview;
const EFeature1 fMultiviewGeometryShader = & VkPhysicalDeviceMultiviewFeatures::multiviewGeometryShader;
const EFeature1 fMultiviewTessellationShader = & VkPhysicalDeviceMultiviewFeatures::multiviewTessellationShader;

const EFeature2 fVariablePointersStorageBuffer = & VkPhysicalDeviceVariablePointerFeatures::variablePointersStorageBuffer;
const EFeature2 fVariablePointers = & VkPhysicalDeviceVariablePointerFeatures::variablePointers;

const EFeature3 fStorageBuffer8BitAccess = & VkPhysicalDevice8BitStorageFeaturesKHR::storageBuffer8BitAccess;
const EFeature3 fUniformAndStorageBuffer8BitAccess = & VkPhysicalDevice8BitStorageFeaturesKHR::uniformAndStorageBuffer8BitAccess;
const EFeature3 fStoragePushConstant8 = & VkPhysicalDevice8BitStorageFeaturesKHR::storagePushConstant8;

const EFeature4 fShaderBufferInt64Atomics = & VkPhysicalDeviceShaderAtomicInt64FeaturesKHR::shaderBufferInt64Atomics;
const EFeature4 fShaderSharedInt64Atomics = & VkPhysicalDeviceShaderAtomicInt64FeaturesKHR::shaderSharedInt64Atomics;

const EFeature5 fStorageBuffer16BitAccess = & VkPhysicalDevice16BitStorageFeatures::storageBuffer16BitAccess;
const EFeature5 fUniformAndStorageBuffer16BitAccess = & VkPhysicalDevice16BitStorageFeatures::uniformAndStorageBuffer16BitAccess;
const EFeature5 fStoragePushConstant16 = & VkPhysicalDevice16BitStorageFeatures::storagePushConstant16;
const EFeature5 fStorageInputOutput16 = & VkPhysicalDevice16BitStorageFeatures::storageInputOutput16;

const EFeature6 fShaderFloat16 = & VkPhysicalDeviceFloat16Int8FeaturesKHR::shaderFloat16;
const EFeature6 fShaderInt8 = & VkPhysicalDeviceFloat16Int8FeaturesKHR::shaderInt8;

const EFeature7 fSamplerYcbcrConversion = & VkPhysicalDeviceSamplerYcbcrConversionFeatures::samplerYcbcrConversion;
const EFeature8 fProtectedMemory = & VkPhysicalDeviceProtectedMemoryFeatures::protectedMemory;
const EFeature9 fAdvancedBlendCoherentOperations = & VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT::advancedBlendCoherentOperations;

const EFeature10 fConditionalRendering = & VkPhysicalDeviceConditionalRenderingFeaturesEXT::conditionalRendering;
const EFeature10 fInheritedConditionalRendering = & VkPhysicalDeviceConditionalRenderingFeaturesEXT::inheritedConditionalRendering;

const EFeature11 fShaderDrawParameters = & VkPhysicalDeviceShaderDrawParameterFeatures::shaderDrawParameters;

const EFeature12 fShaderInputAttachmentArrayDynamicIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderInputAttachmentArrayDynamicIndexing;
const EFeature12 fShaderUniformTexelBufferArrayDynamicIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderUniformTexelBufferArrayDynamicIndexing;
const EFeature12 fShaderStorageTexelBufferArrayDynamicIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderStorageTexelBufferArrayDynamicIndexing;
const EFeature12 fShaderUniformBufferArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderUniformBufferArrayNonUniformIndexing;
const EFeature12 fShaderSampledImageArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderSampledImageArrayNonUniformIndexing;
const EFeature12 fShaderStorageBufferArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderStorageBufferArrayNonUniformIndexing;
const EFeature12 fShaderStorageImageArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderStorageImageArrayNonUniformIndexing;
const EFeature12 fShaderInputAttachmentArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderInputAttachmentArrayNonUniformIndexing;
const EFeature12 fShaderUniformTexelBufferArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderUniformTexelBufferArrayNonUniformIndexing;
const EFeature12 fShaderStorageTexelBufferArrayNonUniformIndexing = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::shaderStorageTexelBufferArrayNonUniformIndexing;
const EFeature12 fDescriptorBindingUniformBufferUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingUniformBufferUpdateAfterBind;
const EFeature12 fDescriptorBindingSampledImageUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingSampledImageUpdateAfterBind;
const EFeature12 fDescriptorBindingStorageImageUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingStorageImageUpdateAfterBind;
const EFeature12 fDescriptorBindingStorageBufferUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingStorageBufferUpdateAfterBind;
const EFeature12 fDescriptorBindingUniformTexelBufferUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingUniformTexelBufferUpdateAfterBind;
const EFeature12 fDescriptorBindingStorageTexelBufferUpdateAfterBind = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingStorageTexelBufferUpdateAfterBind;
const EFeature12 fDescriptorBindingUpdateUnusedWhilePending = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingUpdateUnusedWhilePending;
const EFeature12 fDescriptorBindingPartiallyBound = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingPartiallyBound;
const EFeature12 fDescriptorBindingVariableDescriptorCount = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::descriptorBindingVariableDescriptorCount;
const EFeature12 fRuntimeDescriptorArray = & VkPhysicalDeviceDescriptorIndexingFeaturesEXT::runtimeDescriptorArray;

const EFeature13 fDecodeModeSharedExponent = & VkPhysicalDeviceASTCDecodeFeaturesEXT::decodeModeSharedExponent;

const EFeature14 fVertexAttributeInstanceRateDivisor = & VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT::vertexAttributeInstanceRateDivisor;
const EFeature14 fVertexAttributeInstanceRateZeroDivisor = & VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT::vertexAttributeInstanceRateZeroDivisor;

const EFeature15 fTransformFeedback = & VkPhysicalDeviceTransformFeedbackFeaturesEXT::transformFeedback;
const EFeature15 fGeometryStreams = & VkPhysicalDeviceTransformFeedbackFeaturesEXT::geometryStreams;

const EFeature16 fVulkanMemoryModel = & VkPhysicalDeviceVulkanMemoryModelFeaturesKHR::vulkanMemoryModel;
const EFeature16 fVulkanMemoryModelDeviceScope = & VkPhysicalDeviceVulkanMemoryModelFeaturesKHR::vulkanMemoryModelDeviceScope;
const EFeature16 fVulkanMemoryModelAvailabilityVisibilityChains = & VkPhysicalDeviceVulkanMemoryModelFeaturesKHR::vulkanMemoryModelAvailabilityVisibilityChains;

const EFeature17 fInlineUniformBlock = & VkPhysicalDeviceInlineUniformBlockFeaturesEXT::inlineUniformBlock;
const EFeature17 fDescriptorBindingInlineUniformBlockUpdateAfterBind = & VkPhysicalDeviceInlineUniformBlockFeaturesEXT::descriptorBindingInlineUniformBlockUpdateAfterBind;

const EFeature18 fFragmentDensityMap = & VkPhysicalDeviceFragmentDensityMapFeaturesEXT::fragmentDensityMap;
const EFeature18 fFragmentDensityMapDynamic = & VkPhysicalDeviceFragmentDensityMapFeaturesEXT::fragmentDensityMapDynamic;
const EFeature18 fFragmentDensityMapNonSubsampledImages = & VkPhysicalDeviceFragmentDensityMapFeaturesEXT::fragmentDensityMapNonSubsampledImages;

const EFeature19 fScalarBlockLayout = & VkPhysicalDeviceScalarBlockLayoutFeaturesEXT::scalarBlockLayout;
const EFeature20 fDepthClipEnable = & VkPhysicalDeviceDepthClipEnableFeaturesEXT::depthClipEnable;
const EFeature21 fMemoryPriority = & VkPhysicalDeviceMemoryPriorityFeaturesEXT::memoryPriority;

const EFeature22 fBufferDeviceAddress = & VkPhysicalDeviceBufferAddressFeaturesEXT::bufferDeviceAddress;
const EFeature22 fBufferDeviceAddressCaptureReplay = & VkPhysicalDeviceBufferAddressFeaturesEXT::bufferDeviceAddressCaptureReplay;
const EFeature22 fBufferDeviceAddressMultiDevice = & VkPhysicalDeviceBufferAddressFeaturesEXT::bufferDeviceAddressMultiDevice;

const EFeatureX fExtBlendOperationAdvanced = & SExtensionsAsFeatures::extBlendOperationAdvanced;
const EFeatureX fExtConditionalRendering = & SExtensionsAsFeatures::extConditionalRendering;
const EFeatureX fExtConservativeRasterization = & SExtensionsAsFeatures::extConservativeRasterization;
const EFeatureX fExtDepthRangeUnrestricted = & SExtensionsAsFeatures::extDepthRangeUnrestricted;
const EFeatureX fExtDescriptorIndexing = & SExtensionsAsFeatures::extDescriptorIndexing;
const EFeatureX fExtDiscardRectangles = & SExtensionsAsFeatures::extDiscardRectangles;
const EFeatureX fExtExternalMemoryHost = & SExtensionsAsFeatures::extExternalMemoryHost;
const EFeatureX fExtHdrMetadata = & SExtensionsAsFeatures::extHdrMetadata;
const EFeatureX fExtInlineUniformBlock = & SExtensionsAsFeatures::extInlineUniformBlock;
const EFeatureX fExtPostDepthCoverage = & SExtensionsAsFeatures::extPostDepthCoverage;
const EFeatureX fExtSampleLocations = & SExtensionsAsFeatures::extSampleLocations;
const EFeatureX fExtSamplerFilterMinmax = & SExtensionsAsFeatures::extSamplerFilterMinmax;
const EFeatureX fExtScalarBlockLayout = & SExtensionsAsFeatures::extScalarBlockLayout;
const EFeatureX fExtShaderSubgroupBallot = & SExtensionsAsFeatures::extShaderSubgroupBallot;
const EFeatureX fExtShaderSubgroupVote = & SExtensionsAsFeatures::extShaderSubgroupVote;
const EFeatureX fExtShaderViewportIndexLayer = & SExtensionsAsFeatures::extShaderViewportIndexLayer;
const EFeatureX fExtTransformFeedback = & SExtensionsAsFeatures::extTransformFeedback;
const EFeatureX fExtVertexAttributeDivisor = & SExtensionsAsFeatures::extVertexAttributeDivisor;
const EFeatureX fExtStorage16bit = & SExtensionsAsFeatures::extStorage16bit;
const EFeatureX fExtStorage8bit = & SExtensionsAsFeatures::extStorage8bit;
const EFeatureX fExtBindMemory2 = & SExtensionsAsFeatures::extBindMemory2;
const EFeatureX fExtCreateRenderpass2 = & SExtensionsAsFeatures::extCreateRenderpass2;
const EFeatureX fExtDedicatedAllocation = & SExtensionsAsFeatures::extDedicatedAllocation;
const EFeatureX fExtDepthStencilResolve = & SExtensionsAsFeatures::extDepthStencilResolve;
const EFeatureX fExtDescriptorUpdateTemplate = & SExtensionsAsFeatures::extDescriptorUpdateTemplate;
const EFeatureX fExtDeviceGroup = & SExtensionsAsFeatures::extDeviceGroup;
const EFeatureX fExtDrawIndirectCount = & SExtensionsAsFeatures::extDrawIndirectCount;
const EFeatureX fExtDriverProperties = & SExtensionsAsFeatures::extDriverProperties;
const EFeatureX fExtExternalFence = & SExtensionsAsFeatures::extExternalFence;
const EFeatureX fExtExternalFenceWin32 = & SExtensionsAsFeatures::extExternalFenceWin32;
const EFeatureX fExtExternalMemory = & SExtensionsAsFeatures::extExternalMemory;
const EFeatureX fExtExternalMemoryWin32 = & SExtensionsAsFeatures::extExternalMemoryWin32;
const EFeatureX fExtExternalSemaphore = & SExtensionsAsFeatures::extExternalSemaphore;
const EFeatureX fExtExternalSemaphoreWin32 = & SExtensionsAsFeatures::extExternalSemaphoreWin32;
const EFeatureX fExtGetMemoryRequirements2 = & SExtensionsAsFeatures::extGetMemoryRequirements2;
const EFeatureX fExtImageFormatList = & SExtensionsAsFeatures::extImageFormatList;
const EFeatureX fExtMultiview = & SExtensionsAsFeatures::extMultiview;
const EFeatureX fExtPushDescriptor = & SExtensionsAsFeatures::extPushDescriptor;
const EFeatureX fExtRelaxedBlockLayout = & SExtensionsAsFeatures::extRelaxedBlockLayout;
const EFeatureX fExtSamplerMirrorClampToEdge = & SExtensionsAsFeatures::extSamplerMirrorClampToEdge;
const EFeatureX fExtSamplerYcbcrConversion = & SExtensionsAsFeatures::extSamplerYcbcrConversion;
const EFeatureX fExtShaderAtomicInt64 = & SExtensionsAsFeatures::extShaderAtomicInt64;
const EFeatureX fExtShaderDrawParameters = & SExtensionsAsFeatures::extShaderDrawParameters;
const EFeatureX fExtShaderFloat16Int8 = & SExtensionsAsFeatures::extShaderFloat16Int8;
const EFeatureX fExtShaderFloatControls = & SExtensionsAsFeatures::extShaderFloatControls;
const EFeatureX fExtStorageBufferStorageClass = & SExtensionsAsFeatures::extStorageBufferStorageClass;
const EFeatureX fExtSwapchain = & SExtensionsAsFeatures::extSwapchain;
const EFeatureX fExtSwapchainMutableFormat = & SExtensionsAsFeatures::extSwapchainMutableFormat;
const EFeatureX fExtVariablePointers = & SExtensionsAsFeatures::extVariablePointers;
const EFeatureX fExtVulkanMemoryModel = & SExtensionsAsFeatures::extVulkanMemoryModel;
const EFeatureX fExtWin32KeyedMutex = & SExtensionsAsFeatures::extWin32KeyedMutex;
const EFeatureX fExtFragmentDensityMap = & SExtensionsAsFeatures::extFragmentDensityMap;
const EFeatureX fExtAstcDecodeMode = & SExtensionsAsFeatures::extAstcDecodeMode;
const EFeatureX fExtDepthClipEnable = & SExtensionsAsFeatures::extDepthClipEnable;
const EFeatureX fExtMemoryPriority = & SExtensionsAsFeatures::extMemoryPriority;
const EFeatureX fExtBufferDeviceAddress = & SExtensionsAsFeatures::extBufferDeviceAddress;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
