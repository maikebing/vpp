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
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Optional device feature enumeration.

    VPP offers unified interface to deal with both device features and known
    device extensions. Thus, extensions are listed as features in this enumeration
    and are being enabled in the same fashion. VPP automatically handles dependencies
    between features and extensions. For example, if you enable the 
    \c fShaderSharedInt64Atomics feature (e.g. to use Int64 in a shader),
    VPP will automatically enable \c fExtShaderAtomicInt64 extension.

    Extensions correspond to values with \c fExt prefix. Other values are for features.

    See section 30.1 in the official Vulkan docs for complete description of
    base features. For extension-specific features, you can find separate edition
    of Vulkan spec document augmented with a description of each extension.

    Caution: values of this type listed below are intended to be used only
    as arguments to methods accepting EFeature type. Do not declare your
    own EFeature variables nor use the values in other contexts, as you will
    likely encounter C++ errors if you do so.

    Extensions which are listed in the enumeration below are being referred to
    as <b>known extensions</b>. Those are mostly Khronos and multivendor extensions.
    Some of these extensions are also integrated into VPP API, but not all.
    Integration of extensions is a work in progress. Currently the following ones
    have explicit support in the VPP API:
    - fExtShaderAtomicInt64

    Vendor-specific extensions are currently unsupported. Subsequent versions of
    VPP will make possible to enable them and use though mixing of VPP and core
    Vulkan API.
*/

enum EFeature
{
    fRobustBufferAccess,
    fFullDrawIndexUint32,
    fImageCubeArray,
    fIndependentBlend,
    fGeometryShader,
    fTessellationShader,
    fSampleRateShading,
    fDualSrcBlend,
    fLogicOp,
    fMultiDrawIndirect,
    fDrawIndirectFirstInstance,
    fDepthClamp,
    fDepthBiasClamp,
    fFillModeNonSolid,
    fDepthBounds,
    fWideLines,
    fLargePoints,
    fAlphaToOne,
    fMultiViewport,
    fSamplerAnisotropy,
    fTextureCompressionETC2,
    fTextureCompressionASTC_LDR,
    fTextureCompressionBC,
    fOcclusionQueryPrecise,
    fPipelineStatisticsQuery,
    fVertexPipelineStoresAndAtomics,
    fFragmentStoresAndAtomics,
    fShaderTessellationAndGeometryPointSize,
    fShaderImageGatherExtended,
    fShaderStorageImageExtendedFormats,
    fShaderStorageImageMultisample,
    fShaderStorageImageReadWithoutFormat,
    fShaderStorageImageWriteWithoutFormat,
    fShaderUniformBufferArrayDynamicIndexing,
    fShaderSampledImageArrayDynamicIndexing,
    fShaderStorageBufferArrayDynamicIndexing,
    fShaderStorageImageArrayDynamicIndexing,
    fShaderClipDistance,
    fShaderCullDistance,
    fShaderFloat64,
    fShaderInt64,
    fShaderInt16,
    fShaderResourceResidency,
    fShaderResourceMinLod,
    fSparseBinding,
    fSparseResidencyBuffer,
    fSparseResidencyImage2D,
    fSparseResidencyImage3D,
    fSparseResidency2Samples,
    fSparseResidency4Samples,
    fSparseResidency8Samples,
    fSparseResidency16Samples,
    fSparseResidencyAliased,
    fVariableMultisampleRate,
    fInheritedQueries,
    fMultiview,
    fMultiviewGeometryShader,
    fMultiviewTessellationShader,
    fVariablePointersStorageBuffer,
    fVariablePointers,
    fStorageBuffer8BitAccess,
    fUniformAndStorageBuffer8BitAccess,
    fStoragePushConstant8,
    fShaderBufferInt64Atomics,
    fShaderSharedInt64Atomics,
    fStorageBuffer16BitAccess,
    fUniformAndStorageBuffer16BitAccess,
    fStoragePushConstant16,
    fStorageInputOutput16,
    fShaderFloat16,
    fShaderInt8,
    fSamplerYcbcrConversion,
    fProtectedMemory,
    fAdvancedBlendCoherentOperations,
    fConditionalRendering,
    fInheritedConditionalRendering,
    fShaderDrawParameters,
    fShaderInputAttachmentArrayDynamicIndexing,
    fShaderUniformTexelBufferArrayDynamicIndexing,
    fShaderStorageTexelBufferArrayDynamicIndexing,
    fShaderUniformBufferArrayNonUniformIndexing,
    fShaderSampledImageArrayNonUniformIndexing,
    fShaderStorageBufferArrayNonUniformIndexing,
    fShaderStorageImageArrayNonUniformIndexing,
    fShaderInputAttachmentArrayNonUniformIndexing,
    fShaderUniformTexelBufferArrayNonUniformIndexing,
    fShaderStorageTexelBufferArrayNonUniformIndexing,
    fDescriptorBindingUniformBufferUpdateAfterBind,
    fDescriptorBindingSampledImageUpdateAfterBind,
    fDescriptorBindingStorageImageUpdateAfterBind,
    fDescriptorBindingStorageBufferUpdateAfterBind,
    fDescriptorBindingUniformTexelBufferUpdateAfterBind,
    fDescriptorBindingStorageTexelBufferUpdateAfterBind,
    fDescriptorBindingUpdateUnusedWhilePending,
    fDescriptorBindingPartiallyBound,
    fDescriptorBindingVariableDescriptorCount,
    fRuntimeDescriptorArray,
    fDecodeModeSharedExponent,
    fVertexAttributeInstanceRateDivisor,
    fVertexAttributeInstanceRateZeroDivisor,
    fTransformFeedback,
    fGeometryStreams,
    fVulkanMemoryModel,
    fVulkanMemoryModelDeviceScope,
    fVulkanMemoryModelAvailabilityVisibilityChains,
    fInlineUniformBlock,
    fDescriptorBindingInlineUniformBlockUpdateAfterBind,
    fFragmentDensityMap,
    fFragmentDensityMapDynamic,
    fFragmentDensityMapNonSubsampledImages,
    fScalarBlockLayout,
    fDepthClipEnable,
    fMemoryPriority,
    fBufferDeviceAddress,
    fBufferDeviceAddressCaptureReplay,
    fBufferDeviceAddressMultiDevice,
    fExtBlendOperationAdvanced,
    fExtConditionalRendering,
    fExtConservativeRasterization,
    fExtDepthRangeUnrestricted,
    fExtDescriptorIndexing,
    fExtDiscardRectangles,
    fExtExternalMemoryHost,
    fExtHdrMetadata,
    fExtInlineUniformBlock,
    fExtPostDepthCoverage,
    fExtSampleLocations,
    fExtSamplerFilterMinmax,
    fExtScalarBlockLayout,
    fExtShaderSubgroupBallot,
    fExtShaderSubgroupVote,
    fExtShaderViewportIndexLayer,
    fExtTransformFeedback,
    fExtVertexAttributeDivisor,
    fExtStorage16bit,
    fExtStorage8bit,
    fExtBindMemory2,
    fExtCreateRenderpass2,
    fExtDedicatedAllocation,
    fExtDepthStencilResolve,
    fExtDescriptorUpdateTemplate,
    fExtDeviceGroup,
    fExtDrawIndirectCount,
    fExtDriverProperties,
    fExtExternalFence,
    fExtExternalFenceWin32,
    fExtExternalMemory,
    fExtExternalMemoryWin32,
    fExtExternalSemaphore,
    fExtExternalSemaphoreWin32,
    fExtGetMemoryRequirements2,
    fExtImageFormatList,
    fExtMultiview,
    fExtPushDescriptor,
    fExtRelaxedBlockLayout,
    fExtSamplerMirrorClampToEdge,
    fExtSamplerYcbcrConversion,
    fExtShaderAtomicInt64,
    fExtShaderDrawParameters,
    fExtShaderFloat16Int8,
    fExtShaderFloatControls,
    fExtStorageBufferStorageClass,
    fExtSwapchain,
    fExtSwapchainMutableFormat,
    fExtVariablePointers,
    fExtVulkanMemoryModel,
    fExtWin32KeyedMutex,
    fExtFragmentDensityMap,
    fExtAstcDecodeMode,
    fExtDepthClipEnable,
    fExtMemoryPriority,
    fExtBufferDeviceAddress

};

// -----------------------------------------------------------------------------

/**
    \brief Represents physical rendering device.

    You obtain it from the Instance object, or by providing already known
    Vulkan physical device handle. In the latter case, PhysicalDevice class
    does not acquire ownership on the handle.

    The PhysicalDevice object has the following purposes:
    - providing information about various device capabilities and properties,
    - by means of DeviceFeatures helper class, enabling optional features,
    - creating a logical device by means of Device class constructor.

    This object is reference-counted and may be passed by value.
*/

class PhysicalDevice
{
public:
    /** \brief Constructs null reference. */
    PhysicalDevice();

    /** \brief Constructs device reference from Vulkan device handle. */
    PhysicalDevice ( VkPhysicalDevice hDevice );

    /** \brief Checks whether this is not a null reference. */
    operator bool() const;

    /** \brief Retrieves Vulkan handle for this device. */
    VkPhysicalDevice handle() const;

    /** \brief Retrieves device properties. */
    const VkPhysicalDeviceProperties& properties() const;

    /** \brief Retrieves features and extensions supported by the device. */
    const DeviceFeatures& features() const;

    /** \brief Retrieves memory properties for this device. */
    VkPhysicalDeviceMemoryProperties getMemoryProperties() const;

    /** \brief Retrieves number of queue families supported by this device. */
    size_t queueFamilyCount() const;

    /** \brief Retrieves properties for specified queue family. */
    const VkQueueFamilyProperties& getQueueFamilyProperties ( size_t iFamily ) const;

    /** \brief Checks whether a queue family of this device supports presentation to a given surface. */
    bool supportsSurface ( const Surface& surface, size_t iFamily ) const;

    /** \brief Gets textual representation of limits section in device properties. Useful for diagnostic logs.*/
    void getLimitValuesAsText ( std::ostream& sst ) const;

    /**
        \brief Enumeration of possible usages for data format.
    */

    enum EFormatUsage
    {
        OPTIMAL_TILING,  /**< \brief Use the format in optimal tiling images. */
        LINEAR_TILING,   /**< \brief Use the format in linear tiling images. */
        BUFFER           /**< \brief Use the format in texel buffers. */
    };

    /** \brief Checks whether this device supports specified usage of given format.
    
        For typical applications, \c OPTIMAL_TILING is the best choice and largest
        number of formats support it. The \c LINEAR_TILING mode can be useful when
        you generate image contents on CPU. \c BUFFER is required to use with
        texel buffers.

        Examples:
    
        \code
            VkFormatFeatureFlags bFormatFlags1 = hPhysicalDevice.supportsFormat (
                VK_FORMAT_R8G8B8_UNORM );

            VkFormatFeatureFlags bFormatFlags2 = hPhysicalDevice.supportsFormat (
                VK_FORMAT_R8G8B8_UNORM, vpp::PhysicalDevice::LINEAR_TILING );

            if ( bFormatFlags2 & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT )
            {
                // can use linear tiling for storage images in VK_FORMAT_R8G8B8_UNORM format
            }

        \endcode
    */
    VkFormatFeatureFlags supportsFormat ( VkFormat fmt, EFormatUsage u = OPTIMAL_TILING ) const;

    /** \brief Checks whether this device supports specified depth/stencil format. */
    bool supportsDepthStencilFormat ( VkFormat fmt ) const;

    /** \brief Gets a list of available Vulkan extensions for this device. */
    VkResult enumerateExtensions ( ExtensionProperties* pResult );
};

// -----------------------------------------------------------------------------

/**
    \brief Array of physical devices.
*/

typedef std::vector< PhysicalDevice > PhysicalDevices;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Utility class for dealing with device features and known extensions.

    Certain device features and extensions must be explicitly enabled
    in order to make their functionality available. DeviceFeatures class manages
    such features and extensions known by VPP. All features and most extensions
    are disabled by default, and you can enable them selectively calling the
    enableIfSupported() method.

    Next, you should specify the DeviceFeatures object to the Device
    constructor. DeviceFeatures is only a container of flags,
    it is the Device object that will actually enable the features and extensions.
*/

class DeviceFeatures
{
public:
    /** \brief Constructs the empty feature list.
    
        Everything is disabled except certain extensions enabled by default (like
        the swapchain) and extensions incorporated into core Vulkan API.
    */
    DeviceFeatures();

    /** \brief Constructs a copy of specified feature list. */
    DeviceFeatures ( const DeviceFeatures& rhs );

    /** \brief Makes a copy of specified feature list. */
    const DeviceFeatures& operator= ( const DeviceFeatures& rhs );

    /** \brief Reads supported features and known extensions from specified device.
    
        Note: this retrieves supported features and known extensions from Vulkan
        each time. For a cached version, refer to PhysicalDevice::features()
        method.
    */
    void readSupported ( const PhysicalDevice& hDevice );

    /** \overload */
    void readSupported ( VkPhysicalDevice hDevice );

    /** \brief Reads the status of specified feature or known extension.
    
        Value of \c true means that specific feature or extension is marked as enabled
        in this particular feature set.
    */
    bool operator[] ( EFeature feature ) const;
    
    /** \brief Enables specified feature or known extension if supported by the device.
    
        Returns whether enabling was successful.
    */
    bool enableIfSupported ( EFeature feature, const PhysicalDevice& hDevice );

    /** \brief Retrieves the name of specified feature or known extension.
    
        For extensions, this will return the canonical extension name, e.g.
        \c VK_KHR_shader_atomic_int64.
    */
    const char* getFeatureName ( EFeature feature );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------


