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

#ifndef INC_VPPPHYSICALDEVICE_HPP
#define INC_VPPPHYSICALDEVICE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

typedef VkBool32 VkPhysicalDeviceFeatures::* EFeature;

static const EFeature fRobustBufferAccess = & VkPhysicalDeviceFeatures::robustBufferAccess;
static const EFeature fFullDrawIndexUint32 = & VkPhysicalDeviceFeatures::fullDrawIndexUint32;
static const EFeature fImageCubeArray = & VkPhysicalDeviceFeatures::imageCubeArray;
static const EFeature fIndependentBlend = & VkPhysicalDeviceFeatures::independentBlend;
static const EFeature fGeometryShader = & VkPhysicalDeviceFeatures::geometryShader;
static const EFeature fTessellationShader = & VkPhysicalDeviceFeatures::tessellationShader;
static const EFeature fSampleRateShading = & VkPhysicalDeviceFeatures::sampleRateShading;
static const EFeature fDualSrcBlend = & VkPhysicalDeviceFeatures::dualSrcBlend;
static const EFeature fLogicOp = & VkPhysicalDeviceFeatures::logicOp;
static const EFeature fMultiDrawIndirect = & VkPhysicalDeviceFeatures::multiDrawIndirect;
static const EFeature fDrawIndirectFirstInstance = & VkPhysicalDeviceFeatures::drawIndirectFirstInstance;
static const EFeature fDepthClamp = & VkPhysicalDeviceFeatures::depthClamp;
static const EFeature fDepthBiasClamp = & VkPhysicalDeviceFeatures::depthBiasClamp;
static const EFeature fFillModeNonSolid = & VkPhysicalDeviceFeatures::fillModeNonSolid;
static const EFeature fDepthBounds = & VkPhysicalDeviceFeatures::depthBounds;
static const EFeature fWideLines = & VkPhysicalDeviceFeatures::wideLines;
static const EFeature fLargePoints = & VkPhysicalDeviceFeatures::largePoints;
static const EFeature fAlphaToOne = & VkPhysicalDeviceFeatures::alphaToOne;
static const EFeature fMultiViewport = & VkPhysicalDeviceFeatures::multiViewport;
static const EFeature fSamplerAnisotropy = & VkPhysicalDeviceFeatures::samplerAnisotropy;
static const EFeature fTextureCompressionETC2 = & VkPhysicalDeviceFeatures::textureCompressionETC2;
static const EFeature fTextureCompressionASTC_LDR = & VkPhysicalDeviceFeatures::textureCompressionASTC_LDR;
static const EFeature fTextureCompressionBC = & VkPhysicalDeviceFeatures::textureCompressionBC;
static const EFeature fOcclusionQueryPrecise = & VkPhysicalDeviceFeatures::occlusionQueryPrecise;
static const EFeature fPipelineStatisticsQuery = & VkPhysicalDeviceFeatures::pipelineStatisticsQuery;
static const EFeature fVertexPipelineStoresAndAtomics = & VkPhysicalDeviceFeatures::vertexPipelineStoresAndAtomics;
static const EFeature fFragmentStoresAndAtomics = & VkPhysicalDeviceFeatures::fragmentStoresAndAtomics;
static const EFeature fShaderTessellationAndGeometryPointSize = & VkPhysicalDeviceFeatures::shaderTessellationAndGeometryPointSize;
static const EFeature fShaderImageGatherExtended = & VkPhysicalDeviceFeatures::shaderImageGatherExtended;
static const EFeature fShaderStorageImageExtendedFormats = & VkPhysicalDeviceFeatures::shaderStorageImageExtendedFormats;
static const EFeature fShaderStorageImageMultisample = & VkPhysicalDeviceFeatures::shaderStorageImageMultisample;
static const EFeature fShaderStorageImageReadWithoutFormat = & VkPhysicalDeviceFeatures::shaderStorageImageReadWithoutFormat;
static const EFeature fShaderStorageImageWriteWithoutFormat = & VkPhysicalDeviceFeatures::shaderStorageImageWriteWithoutFormat;
static const EFeature fShaderUniformBufferArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderUniformBufferArrayDynamicIndexing;
static const EFeature fShaderSampledImageArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderSampledImageArrayDynamicIndexing;
static const EFeature fShaderStorageBufferArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderStorageBufferArrayDynamicIndexing;
static const EFeature fShaderStorageImageArrayDynamicIndexing = & VkPhysicalDeviceFeatures::shaderStorageImageArrayDynamicIndexing;
static const EFeature fShaderClipDistance = & VkPhysicalDeviceFeatures::shaderClipDistance;
static const EFeature fShaderCullDistance = & VkPhysicalDeviceFeatures::shaderCullDistance;
static const EFeature fShaderFloat64 = & VkPhysicalDeviceFeatures::shaderFloat64;
static const EFeature fShaderInt64 = & VkPhysicalDeviceFeatures::shaderInt64;
static const EFeature fShaderInt16 = & VkPhysicalDeviceFeatures::shaderInt16;
static const EFeature fShaderResourceResidency = & VkPhysicalDeviceFeatures::shaderResourceResidency;
static const EFeature fShaderResourceMinLod = & VkPhysicalDeviceFeatures::shaderResourceMinLod;
static const EFeature fSparseBinding = & VkPhysicalDeviceFeatures::sparseBinding;
static const EFeature fSparseResidencyBuffer = & VkPhysicalDeviceFeatures::sparseResidencyBuffer;
static const EFeature fSparseResidencyImage2D = & VkPhysicalDeviceFeatures::sparseResidencyImage2D;
static const EFeature fSparseResidencyImage3D = & VkPhysicalDeviceFeatures::sparseResidencyImage3D;
static const EFeature fSparseResidency2Samples = & VkPhysicalDeviceFeatures::sparseResidency2Samples;
static const EFeature fSparseResidency4Samples = & VkPhysicalDeviceFeatures::sparseResidency4Samples;
static const EFeature fSparseResidency8Samples = & VkPhysicalDeviceFeatures::sparseResidency8Samples;
static const EFeature fSparseResidency16Samples = & VkPhysicalDeviceFeatures::sparseResidency16Samples;
static const EFeature fSparseResidencyAliased = & VkPhysicalDeviceFeatures::sparseResidencyAliased;
static const EFeature fVariableMultisampleRate = & VkPhysicalDeviceFeatures::variableMultisampleRate;
static const EFeature fInheritedQueries = & VkPhysicalDeviceFeatures::inheritedQueries;

// -----------------------------------------------------------------------------

class PhysicalDeviceImpl;

// -----------------------------------------------------------------------------

class PhysicalDevice : public TSharedReference< PhysicalDeviceImpl >
{
public:
    PhysicalDevice();
    PhysicalDevice ( VkPhysicalDevice hDevice );
    ~PhysicalDevice();

    VkPhysicalDeviceProperties getPhysicalDeviceProperties() const;

    VPP_DLLAPI size_t queueFamilyCount() const;
    VPP_DLLAPI const VkQueueFamilyProperties& getQueueFamilyProperties ( size_t iFamily ) const;

    VPP_DLLAPI bool supportsSurface ( const Surface& surface, size_t iFamily ) const;
    bool supportsFeature ( EFeature feature ) const;
    VPP_DLLAPI void getLimitValuesAsText ( std::ostream& sst ) const;

    enum EFormatUsage
    {
        OPTIMAL_TILING,
        LINEAR_TILING,
        BUFFER
    };

    VPP_DLLAPI VkFormatFeatureFlags supportsFormat ( VkFormat fmt, EFormatUsage u = OPTIMAL_TILING ) const;
    VPP_DLLAPI bool supportsDepthStencilFormat ( VkFormat fmt ) const;

    VkPhysicalDevice handle() const;
    VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
};

// -----------------------------------------------------------------------------

typedef std::vector< PhysicalDevice > PhysicalDevices;

// -----------------------------------------------------------------------------

class PhysicalDeviceImpl : public TSharedObject< PhysicalDeviceImpl >
{
public:
    VPP_DLLAPI PhysicalDeviceImpl ( VkPhysicalDevice hDevice );
    VPP_DLLAPI ~PhysicalDeviceImpl();

    VPP_INLINE bool compareObjects ( const PhysicalDeviceImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class PhysicalDevice;

    spinlock_mutex d_mutex;
    VkPhysicalDevice d_handle;

    QueueFamilyProperties d_queueFamilyProperties;
    VkPhysicalDeviceFeatures d_supportedFeatures;
};

// -----------------------------------------------------------------------------

VPP_INLINE PhysicalDevice :: PhysicalDevice()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE PhysicalDevice :: PhysicalDevice ( VkPhysicalDevice hDevice ) :
    TSharedReference< PhysicalDeviceImpl > ( new PhysicalDeviceImpl ( hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE PhysicalDevice :: ~PhysicalDevice()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPhysicalDeviceProperties PhysicalDevice :: getPhysicalDeviceProperties() const
{
    VkPhysicalDeviceProperties properties;
    ::vkGetPhysicalDeviceProperties ( get()->d_handle, & properties );
    return properties;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool PhysicalDevice :: supportsFeature ( EFeature feature ) const
{
    return get()->d_supportedFeatures.*feature != VK_FALSE;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPhysicalDevice PhysicalDevice :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPhysicalDeviceMemoryProperties PhysicalDevice :: getMemoryProperties() const
{
    VkPhysicalDeviceMemoryProperties properties;
    ::vkGetPhysicalDeviceMemoryProperties ( get()->d_handle, & properties );
    return properties;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class DeviceFeatures : public VkPhysicalDeviceFeatures
{
public:
    DeviceFeatures ( const PhysicalDevice& hPhysDevice );
    bool enableIfSupported ( EFeature feature );
    bool isSupported ( EFeature feature ) const;

private:
    PhysicalDevice d_hPhysDevice;
};

// -----------------------------------------------------------------------------

VPP_INLINE DeviceFeatures :: DeviceFeatures ( const PhysicalDevice& hPhysDevice ) :
    d_hPhysDevice ( hPhysDevice )
{
    std::memset (
        static_cast< VkPhysicalDeviceFeatures* >( this ),
        0, sizeof ( VkPhysicalDeviceFeatures ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceFeatures :: enableIfSupported ( EFeature feature )
{
    if ( d_hPhysDevice.supportsFeature ( feature ) )
    {
        static_cast< VkPhysicalDeviceFeatures* >( this )->*feature = VK_TRUE;
        return true;
    }
    else
        return false;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceFeatures :: isSupported ( EFeature feature ) const
{
    return d_hPhysDevice.supportsFeature ( feature );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPHYSICALDEVICE_HPP


