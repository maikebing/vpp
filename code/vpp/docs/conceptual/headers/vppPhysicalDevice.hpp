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

    See section 30.1 in the official Vulkan docs for complete description of
    these features.
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
    fInheritedQueries
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
    VkPhysicalDeviceProperties getPhysicalDeviceProperties() const;

    /** \brief Retrieves memory properties for this device. */
    VkPhysicalDeviceMemoryProperties getMemoryProperties() const;

    /** \brief Retrieves number of queue families supported by this device. */
    size_t queueFamilyCount() const;

    /** \brief Retrieves properties for specified queue family. */
    const VkQueueFamilyProperties& getQueueFamilyProperties ( size_t iFamily ) const;

    /** \brief Checks whether a queue family of this device supports presentation to a given surface. */
    bool supportsSurface ( const Surface& surface, size_t iFamily ) const;

    /** \brief Checks whether this device supports given feature. */
    bool supportsFeature ( EFeature feature ) const;

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
};

// -----------------------------------------------------------------------------

/**
    \brief Array of physical devices.
*/

typedef std::vector< PhysicalDevice > PhysicalDevices;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Utility class for dealing with device feature lists.

    Certain device features must be explicitly enabled to be used.
    DeviceFeatures class manages such features. All features are
    disabled by default, and you can enable them selectively using
    the enableIfSupported() method.

    Next, you should specify the DeviceFeatures object to the Device
    constructor. DeviceFeatures is only a container of flags,
    it is the Device object that will actually enable the features.
*/

class DeviceFeatures : public VkPhysicalDeviceFeatures
{
public:
    /** \brief Constructs the feature list for specified device. */
    DeviceFeatures ( const PhysicalDevice& hPhysDevice );

    /** \brief Enables specified feature, if supported by the device.
    
        Returns whether feaure is supported and has been enabled.
    */
    bool enableIfSupported ( EFeature feature );

    /** \brief Checks whether the device supports given feature. */
    bool isSupported ( EFeature feature ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------


