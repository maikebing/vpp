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

#ifndef INC_VPPPHYSICALDEVICE_HPP
#define INC_VPPPHYSICALDEVICE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct SExtensionsAsFeatures
{
    VkBool32 extBlendOperationAdvanced;
    VkBool32 extConditionalRendering;
    VkBool32 extConservativeRasterization;
    VkBool32 extDepthRangeUnrestricted;
    VkBool32 extDescriptorIndexing;
    VkBool32 extDiscardRectangles;
    VkBool32 extExternalMemoryHost;
    VkBool32 extHdrMetadata;
    VkBool32 extInlineUniformBlock;
    VkBool32 extPostDepthCoverage;
    VkBool32 extSampleLocations;
    VkBool32 extSamplerFilterMinmax;
    VkBool32 extScalarBlockLayout;
    VkBool32 extShaderSubgroupBallot;
    VkBool32 extShaderSubgroupVote;
    VkBool32 extShaderViewportIndexLayer;
    VkBool32 extTransformFeedback;
    VkBool32 extVertexAttributeDivisor;
    VkBool32 extStorage16bit;
    VkBool32 extStorage8bit;
    VkBool32 extBindMemory2;
    VkBool32 extCreateRenderpass2;
    VkBool32 extDedicatedAllocation;
    VkBool32 extDepthStencilResolve;
    VkBool32 extDescriptorUpdateTemplate;
    VkBool32 extDeviceGroup;
    VkBool32 extDrawIndirectCount;
    VkBool32 extDriverProperties;
    VkBool32 extExternalFence;
    VkBool32 extExternalFenceWin32;
    VkBool32 extExternalMemory;
    VkBool32 extExternalMemoryWin32;
    VkBool32 extExternalSemaphore;
    VkBool32 extExternalSemaphoreWin32;
    VkBool32 extGetMemoryRequirements2;
    VkBool32 extImageFormatList;
    VkBool32 extMultiview;
    VkBool32 extPushDescriptor;
    VkBool32 extRelaxedBlockLayout;
    VkBool32 extSamplerMirrorClampToEdge;
    VkBool32 extSamplerYcbcrConversion;
    VkBool32 extShaderAtomicInt64;
    VkBool32 extShaderDrawParameters;
    VkBool32 extShaderFloat16Int8;
    VkBool32 extShaderFloatControls;
    VkBool32 extStorageBufferStorageClass;
    VkBool32 extSwapchain;
    VkBool32 extSwapchainMutableFormat;
    VkBool32 extVariablePointers;
    VkBool32 extVulkanMemoryModel;
    VkBool32 extWin32KeyedMutex;
    VkBool32 extFragmentDensityMap;
    VkBool32 extAstcDecodeMode;
    VkBool32 extDepthClipEnable;
    VkBool32 extMemoryPriority;
    VkBool32 extBufferDeviceAddress;
};

// -----------------------------------------------------------------------------

typedef VkBool32 VkPhysicalDeviceFeatures::* EFeature;
typedef VkBool32 VkPhysicalDeviceMultiviewFeatures::* EFeature1;
typedef VkBool32 VkPhysicalDeviceVariablePointerFeatures::* EFeature2;
typedef VkBool32 VkPhysicalDevice8BitStorageFeaturesKHR::* EFeature3;
typedef VkBool32 VkPhysicalDeviceShaderAtomicInt64FeaturesKHR::* EFeature4;
typedef VkBool32 VkPhysicalDevice16BitStorageFeatures::* EFeature5;
typedef VkBool32 VkPhysicalDeviceFloat16Int8FeaturesKHR::* EFeature6;
typedef VkBool32 VkPhysicalDeviceSamplerYcbcrConversionFeatures::* EFeature7;
typedef VkBool32 VkPhysicalDeviceProtectedMemoryFeatures::* EFeature8;
typedef VkBool32 VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT::* EFeature9;
typedef VkBool32 VkPhysicalDeviceConditionalRenderingFeaturesEXT::* EFeature10;
typedef VkBool32 VkPhysicalDeviceShaderDrawParameterFeatures::* EFeature11;
typedef VkBool32 VkPhysicalDeviceDescriptorIndexingFeaturesEXT::* EFeature12;
typedef VkBool32 VkPhysicalDeviceASTCDecodeFeaturesEXT::* EFeature13;
typedef VkBool32 VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT::* EFeature14;
typedef VkBool32 VkPhysicalDeviceTransformFeedbackFeaturesEXT::* EFeature15;
typedef VkBool32 VkPhysicalDeviceVulkanMemoryModelFeaturesKHR::* EFeature16;
typedef VkBool32 VkPhysicalDeviceInlineUniformBlockFeaturesEXT::* EFeature17;
typedef VkBool32 VkPhysicalDeviceFragmentDensityMapFeaturesEXT::* EFeature18;
typedef VkBool32 VkPhysicalDeviceScalarBlockLayoutFeaturesEXT::* EFeature19;
typedef VkBool32 VkPhysicalDeviceDepthClipEnableFeaturesEXT::* EFeature20;
typedef VkBool32 VkPhysicalDeviceMemoryPriorityFeaturesEXT::* EFeature21;
typedef VkBool32 VkPhysicalDeviceBufferAddressFeaturesEXT::* EFeature22;

typedef VkBool32 SExtensionsAsFeatures::* EFeatureX;

VPP_DLLAPI extern const EFeature fRobustBufferAccess;
VPP_DLLAPI extern const EFeature fFullDrawIndexUint32;
VPP_DLLAPI extern const EFeature fImageCubeArray;
VPP_DLLAPI extern const EFeature fIndependentBlend;
VPP_DLLAPI extern const EFeature fGeometryShader;
VPP_DLLAPI extern const EFeature fTessellationShader;
VPP_DLLAPI extern const EFeature fSampleRateShading;
VPP_DLLAPI extern const EFeature fDualSrcBlend;
VPP_DLLAPI extern const EFeature fLogicOp;
VPP_DLLAPI extern const EFeature fMultiDrawIndirect;
VPP_DLLAPI extern const EFeature fDrawIndirectFirstInstance;
VPP_DLLAPI extern const EFeature fDepthClamp;
VPP_DLLAPI extern const EFeature fDepthBiasClamp;
VPP_DLLAPI extern const EFeature fFillModeNonSolid;
VPP_DLLAPI extern const EFeature fDepthBounds;
VPP_DLLAPI extern const EFeature fWideLines;
VPP_DLLAPI extern const EFeature fLargePoints;
VPP_DLLAPI extern const EFeature fAlphaToOne;
VPP_DLLAPI extern const EFeature fMultiViewport;
VPP_DLLAPI extern const EFeature fSamplerAnisotropy;
VPP_DLLAPI extern const EFeature fTextureCompressionETC2;
VPP_DLLAPI extern const EFeature fTextureCompressionASTC_LDR;
VPP_DLLAPI extern const EFeature fTextureCompressionBC;
VPP_DLLAPI extern const EFeature fOcclusionQueryPrecise;
VPP_DLLAPI extern const EFeature fPipelineStatisticsQuery;
VPP_DLLAPI extern const EFeature fVertexPipelineStoresAndAtomics;
VPP_DLLAPI extern const EFeature fFragmentStoresAndAtomics;
VPP_DLLAPI extern const EFeature fShaderTessellationAndGeometryPointSize;
VPP_DLLAPI extern const EFeature fShaderImageGatherExtended;
VPP_DLLAPI extern const EFeature fShaderStorageImageExtendedFormats;
VPP_DLLAPI extern const EFeature fShaderStorageImageMultisample;
VPP_DLLAPI extern const EFeature fShaderStorageImageReadWithoutFormat;
VPP_DLLAPI extern const EFeature fShaderStorageImageWriteWithoutFormat;
VPP_DLLAPI extern const EFeature fShaderUniformBufferArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature fShaderSampledImageArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature fShaderStorageBufferArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature fShaderStorageImageArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature fShaderClipDistance;
VPP_DLLAPI extern const EFeature fShaderCullDistance;
VPP_DLLAPI extern const EFeature fShaderFloat64;
VPP_DLLAPI extern const EFeature fShaderInt64;
VPP_DLLAPI extern const EFeature fShaderInt16;
VPP_DLLAPI extern const EFeature fShaderResourceResidency;
VPP_DLLAPI extern const EFeature fShaderResourceMinLod;
VPP_DLLAPI extern const EFeature fSparseBinding;
VPP_DLLAPI extern const EFeature fSparseResidencyBuffer;
VPP_DLLAPI extern const EFeature fSparseResidencyImage2D;
VPP_DLLAPI extern const EFeature fSparseResidencyImage3D;
VPP_DLLAPI extern const EFeature fSparseResidency2Samples;
VPP_DLLAPI extern const EFeature fSparseResidency4Samples;
VPP_DLLAPI extern const EFeature fSparseResidency8Samples;
VPP_DLLAPI extern const EFeature fSparseResidency16Samples;
VPP_DLLAPI extern const EFeature fSparseResidencyAliased;
VPP_DLLAPI extern const EFeature fVariableMultisampleRate;
VPP_DLLAPI extern const EFeature fInheritedQueries;

VPP_DLLAPI extern const EFeature1 fMultiview;
VPP_DLLAPI extern const EFeature1 fMultiviewGeometryShader;
VPP_DLLAPI extern const EFeature1 fMultiviewTessellationShader;

VPP_DLLAPI extern const EFeature2 fVariablePointersStorageBuffer;
VPP_DLLAPI extern const EFeature2 fVariablePointers;

VPP_DLLAPI extern const EFeature3 fStorageBuffer8BitAccess;
VPP_DLLAPI extern const EFeature3 fUniformAndStorageBuffer8BitAccess;
VPP_DLLAPI extern const EFeature3 fStoragePushConstant8;

VPP_DLLAPI extern const EFeature4 fShaderBufferInt64Atomics;
VPP_DLLAPI extern const EFeature4 fShaderSharedInt64Atomics;

VPP_DLLAPI extern const EFeature5 fStorageBuffer16BitAccess;
VPP_DLLAPI extern const EFeature5 fUniformAndStorageBuffer16BitAccess;
VPP_DLLAPI extern const EFeature5 fStoragePushConstant16;
VPP_DLLAPI extern const EFeature5 fStorageInputOutput16;

VPP_DLLAPI extern const EFeature6 fShaderFloat16;
VPP_DLLAPI extern const EFeature6 fShaderInt8;

VPP_DLLAPI extern const EFeature7 fSamplerYcbcrConversion;
VPP_DLLAPI extern const EFeature8 fProtectedMemory;
VPP_DLLAPI extern const EFeature9 fAdvancedBlendCoherentOperations;

VPP_DLLAPI extern const EFeature10 fConditionalRendering;
VPP_DLLAPI extern const EFeature10 fInheritedConditionalRendering;

VPP_DLLAPI extern const EFeature11 fShaderDrawParameters;

VPP_DLLAPI extern const EFeature12 fShaderInputAttachmentArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature12 fShaderUniformTexelBufferArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature12 fShaderStorageTexelBufferArrayDynamicIndexing;
VPP_DLLAPI extern const EFeature12 fShaderUniformBufferArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderSampledImageArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderStorageBufferArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderStorageImageArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderInputAttachmentArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderUniformTexelBufferArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fShaderStorageTexelBufferArrayNonUniformIndexing;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingUniformBufferUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingSampledImageUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingStorageImageUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingStorageBufferUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingUniformTexelBufferUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingStorageTexelBufferUpdateAfterBind;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingUpdateUnusedWhilePending;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingPartiallyBound;
VPP_DLLAPI extern const EFeature12 fDescriptorBindingVariableDescriptorCount;
VPP_DLLAPI extern const EFeature12 fRuntimeDescriptorArray;

VPP_DLLAPI extern const EFeature13 fDecodeModeSharedExponent;

VPP_DLLAPI extern const EFeature14 fVertexAttributeInstanceRateDivisor;
VPP_DLLAPI extern const EFeature14 fVertexAttributeInstanceRateZeroDivisor;

VPP_DLLAPI extern const EFeature15 fTransformFeedback;
VPP_DLLAPI extern const EFeature15 fGeometryStreams;

VPP_DLLAPI extern const EFeature16 fVulkanMemoryModel;
VPP_DLLAPI extern const EFeature16 fVulkanMemoryModelDeviceScope;
VPP_DLLAPI extern const EFeature16 fVulkanMemoryModelAvailabilityVisibilityChains;

VPP_DLLAPI extern const EFeature17 fInlineUniformBlock;
VPP_DLLAPI extern const EFeature17 fDescriptorBindingInlineUniformBlockUpdateAfterBind;

VPP_DLLAPI extern const EFeature18 fFragmentDensityMap;
VPP_DLLAPI extern const EFeature18 fFragmentDensityMapDynamic;
VPP_DLLAPI extern const EFeature18 fFragmentDensityMapNonSubsampledImages;

VPP_DLLAPI extern const EFeature19 fScalarBlockLayout;
VPP_DLLAPI extern const EFeature20 fDepthClipEnable;
VPP_DLLAPI extern const EFeature21 fMemoryPriority;

VPP_DLLAPI extern const EFeature22 fBufferDeviceAddress;
VPP_DLLAPI extern const EFeature22 fBufferDeviceAddressCaptureReplay;
VPP_DLLAPI extern const EFeature22 fBufferDeviceAddressMultiDevice;

VPP_DLLAPI extern const EFeatureX fExtBlendOperationAdvanced;
VPP_DLLAPI extern const EFeatureX fExtConditionalRendering;
VPP_DLLAPI extern const EFeatureX fExtConservativeRasterization;
VPP_DLLAPI extern const EFeatureX fExtDepthRangeUnrestricted;
VPP_DLLAPI extern const EFeatureX fExtDescriptorIndexing;
VPP_DLLAPI extern const EFeatureX fExtDiscardRectangles;
VPP_DLLAPI extern const EFeatureX fExtExternalMemoryHost;
VPP_DLLAPI extern const EFeatureX fExtHdrMetadata;
VPP_DLLAPI extern const EFeatureX fExtInlineUniformBlock;
VPP_DLLAPI extern const EFeatureX fExtPostDepthCoverage;
VPP_DLLAPI extern const EFeatureX fExtSampleLocations;
VPP_DLLAPI extern const EFeatureX fExtSamplerFilterMinmax;
VPP_DLLAPI extern const EFeatureX fExtScalarBlockLayout;
VPP_DLLAPI extern const EFeatureX fExtShaderSubgroupBallot;
VPP_DLLAPI extern const EFeatureX fExtShaderSubgroupVote;
VPP_DLLAPI extern const EFeatureX fExtShaderViewportIndexLayer;
VPP_DLLAPI extern const EFeatureX fExtTransformFeedback;
VPP_DLLAPI extern const EFeatureX fExtVertexAttributeDivisor;
VPP_DLLAPI extern const EFeatureX fExtStorage16bit;
VPP_DLLAPI extern const EFeatureX fExtStorage8bit;
VPP_DLLAPI extern const EFeatureX fExtBindMemory2;
VPP_DLLAPI extern const EFeatureX fExtCreateRenderpass2;
VPP_DLLAPI extern const EFeatureX fExtDedicatedAllocation;
VPP_DLLAPI extern const EFeatureX fExtDepthStencilResolve;
VPP_DLLAPI extern const EFeatureX fExtDescriptorUpdateTemplate;
VPP_DLLAPI extern const EFeatureX fExtDeviceGroup;
VPP_DLLAPI extern const EFeatureX fExtDrawIndirectCount;
VPP_DLLAPI extern const EFeatureX fExtDriverProperties;
VPP_DLLAPI extern const EFeatureX fExtExternalFence;
VPP_DLLAPI extern const EFeatureX fExtExternalFenceWin32;
VPP_DLLAPI extern const EFeatureX fExtExternalMemory;
VPP_DLLAPI extern const EFeatureX fExtExternalMemoryWin32;
VPP_DLLAPI extern const EFeatureX fExtExternalSemaphore;
VPP_DLLAPI extern const EFeatureX fExtExternalSemaphoreWin32;
VPP_DLLAPI extern const EFeatureX fExtGetMemoryRequirements2;
VPP_DLLAPI extern const EFeatureX fExtImageFormatList;
VPP_DLLAPI extern const EFeatureX fExtMultiview;
VPP_DLLAPI extern const EFeatureX fExtPushDescriptor;
VPP_DLLAPI extern const EFeatureX fExtRelaxedBlockLayout;
VPP_DLLAPI extern const EFeatureX fExtSamplerMirrorClampToEdge;
VPP_DLLAPI extern const EFeatureX fExtSamplerYcbcrConversion;
VPP_DLLAPI extern const EFeatureX fExtShaderAtomicInt64;
VPP_DLLAPI extern const EFeatureX fExtShaderDrawParameters;
VPP_DLLAPI extern const EFeatureX fExtShaderFloat16Int8;
VPP_DLLAPI extern const EFeatureX fExtShaderFloatControls;
VPP_DLLAPI extern const EFeatureX fExtStorageBufferStorageClass;
VPP_DLLAPI extern const EFeatureX fExtSwapchain;
VPP_DLLAPI extern const EFeatureX fExtSwapchainMutableFormat;
VPP_DLLAPI extern const EFeatureX fExtVariablePointers;
VPP_DLLAPI extern const EFeatureX fExtVulkanMemoryModel;
VPP_DLLAPI extern const EFeatureX fExtWin32KeyedMutex;
VPP_DLLAPI extern const EFeatureX fExtFragmentDensityMap;
VPP_DLLAPI extern const EFeatureX fExtAstcDecodeMode;
VPP_DLLAPI extern const EFeatureX fExtDepthClipEnable;
VPP_DLLAPI extern const EFeatureX fExtMemoryPriority;
VPP_DLLAPI extern const EFeatureX fExtBufferDeviceAddress;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class DeviceFeatures :
    public VkPhysicalDeviceFeatures2,
    public VkPhysicalDeviceMultiviewFeatures,
    public VkPhysicalDeviceVariablePointerFeatures,
    public VkPhysicalDevice8BitStorageFeaturesKHR,
    public VkPhysicalDeviceShaderAtomicInt64FeaturesKHR,
    public VkPhysicalDevice16BitStorageFeatures,
    public VkPhysicalDeviceFloat16Int8FeaturesKHR,
    public VkPhysicalDeviceSamplerYcbcrConversionFeatures,
    public VkPhysicalDeviceProtectedMemoryFeatures,
    public VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT,
    public VkPhysicalDeviceConditionalRenderingFeaturesEXT,
    public VkPhysicalDeviceShaderDrawParameterFeatures,
    public VkPhysicalDeviceDescriptorIndexingFeaturesEXT,
    public VkPhysicalDeviceASTCDecodeFeaturesEXT,
    public VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT,
    public VkPhysicalDeviceTransformFeedbackFeaturesEXT,
    public VkPhysicalDeviceVulkanMemoryModelFeaturesKHR,
    public VkPhysicalDeviceInlineUniformBlockFeaturesEXT,
    public VkPhysicalDeviceFragmentDensityMapFeaturesEXT,
    public VkPhysicalDeviceScalarBlockLayoutFeaturesEXT,
    public VkPhysicalDeviceDepthClipEnableFeaturesEXT,
    public VkPhysicalDeviceMemoryPriorityFeaturesEXT,
    public VkPhysicalDeviceBufferAddressFeaturesEXT,
    public SExtensionsAsFeatures
{
public:
    VPP_DLLAPI DeviceFeatures();
    VPP_DLLAPI DeviceFeatures ( const DeviceFeatures& rhs );

    VPP_DLLAPI const DeviceFeatures& operator= ( const DeviceFeatures& rhs );

    VPP_DLLAPI void readSupported ( VkPhysicalDevice hDevice );
    VPP_DLLAPI void readSupported ( const PhysicalDevice& hDevice );

    VPP_DLLAPI void resolveDependencies();
    VPP_DLLAPI void getEnabledExtensions ( std::set< std::string >* pExtNames ) const;
    VPP_DLLAPI void getSourceExtensions ( std::set< std::string >* pExtNames ) const;

    bool operator[] ( VkBool32 VkPhysicalDeviceFeatures::* feature ) const;
    
    template< class FeatureStructT >
    bool operator[] ( VkBool32 FeatureStructT::* feature ) const;

    bool enableIfSupported ( VkBool32 VkPhysicalDeviceFeatures::* feature, const PhysicalDevice& hDevice );

    template< class FeatureStructT >
    bool enableIfSupported ( VkBool32 FeatureStructT::* feature, const PhysicalDevice& hDevice );

    template< class FeatureStructT >
    static const char* getFeatureName ( VkBool32 FeatureStructT::* feature );

private:
    void initStructures();
    void addExtensionIfEnabled ( EFeatureX eExt, std::set< std::string >* pExtNames ) const;
};

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class FeatureStructT >
class TFeatureNames 
{
public:
    typedef std::pair< VkBool32 FeatureStructT::*, const char* > Feature2Name;
    typedef std::vector< Feature2Name > Features2Names;
    typedef typename Features2Names::const_iterator const_iterator;

    VPP_INLINE TFeatureNames ( const std::initializer_list< Feature2Name >& init ) :
        d_names ( init )
    {}

    VPP_INLINE const char* findName ( VkBool32 FeatureStructT::* pFeature ) const
    {
        for ( const auto& iFeature : d_names )
            if ( iFeature.first == pFeature )
                return iFeature.second;

        return "UnknownFeature";
    }

    VPP_INLINE const_iterator begin() const
    {
        return d_names.cbegin();
    }

    VPP_INLINE const_iterator end() const
    {
        return d_names.cend();
    }

private:
    Features2Names d_names;
};

// -----------------------------------------------------------------------------

class KFeatureNames :
    public TFeatureNames< VkPhysicalDeviceFeatures >,
    public TFeatureNames< VkPhysicalDeviceMultiviewFeatures >,
    public TFeatureNames< VkPhysicalDeviceVariablePointerFeatures >,
    public TFeatureNames< VkPhysicalDevice8BitStorageFeaturesKHR >,
    public TFeatureNames< VkPhysicalDeviceShaderAtomicInt64FeaturesKHR >,
    public TFeatureNames< VkPhysicalDevice16BitStorageFeatures >,
    public TFeatureNames< VkPhysicalDeviceFloat16Int8FeaturesKHR >,
    public TFeatureNames< VkPhysicalDeviceSamplerYcbcrConversionFeatures >,
    public TFeatureNames< VkPhysicalDeviceProtectedMemoryFeatures >,
    public TFeatureNames< VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceConditionalRenderingFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceShaderDrawParameterFeatures >,
    public TFeatureNames< VkPhysicalDeviceDescriptorIndexingFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceASTCDecodeFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceTransformFeedbackFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceVulkanMemoryModelFeaturesKHR >,
    public TFeatureNames< VkPhysicalDeviceInlineUniformBlockFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceFragmentDensityMapFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceScalarBlockLayoutFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceDepthClipEnableFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceMemoryPriorityFeaturesEXT >,
    public TFeatureNames< VkPhysicalDeviceBufferAddressFeaturesEXT >,
    public TFeatureNames< SExtensionsAsFeatures >
{
public:
    KFeatureNames();

    template< typename FeatureStructT >
    VPP_INLINE const char* findName ( VkBool32 FeatureStructT::* pFeature ) const
    {
        typedef TFeatureNames< FeatureStructT > FeatureSet;
        const FeatureSet* thisFeatureSet = static_cast< const FeatureSet* >( this );
        return thisFeatureSet->findName ( pFeature );
    }

    template< typename FeatureStructT >
    VPP_INLINE auto begin() const
    {
        typedef TFeatureNames< FeatureStructT > FeatureSet;
        const FeatureSet* thisFeatureSet = static_cast< const FeatureSet* >( this );
        return thisFeatureSet->begin();
    }

    template< typename FeatureStructT >
    VPP_INLINE auto end() const
    {
        typedef TFeatureNames< FeatureStructT > FeatureSet;
        const FeatureSet* thisFeatureSet = static_cast< const FeatureSet* >( this );
        return thisFeatureSet->end();
    }

    VPP_INLINE EFeatureX getExtensionFeature ( const std::string& extName ) const
    {
        const Name2ExtField::const_iterator iName = d_name2extField.find ( extName );
        return iName != d_name2extField.end() ? iName->second : EFeatureX();
    }

private:
    typedef std::map< std::string, EFeatureX > Name2ExtField;
    Name2ExtField d_name2extField;
};

// -----------------------------------------------------------------------------

extern const KFeatureNames s_featureNames;

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

class PhysicalDeviceImpl;

// -----------------------------------------------------------------------------

class PhysicalDevice : public TSharedReference< PhysicalDeviceImpl >
{
public:
    PhysicalDevice();
    PhysicalDevice ( VkPhysicalDevice hDevice );
    ~PhysicalDevice();

    const VkPhysicalDeviceProperties& properties() const;
    const DeviceFeatures& features() const;

    VPP_DLLAPI size_t queueFamilyCount() const;
    VPP_DLLAPI const VkQueueFamilyProperties& getQueueFamilyProperties ( size_t iFamily ) const;

    VPP_DLLAPI bool supportsSurface ( const Surface& surface, size_t iFamily ) const;
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

    VPP_DLLAPI VkResult enumerateExtensions ( ExtensionProperties* pResult );
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
    VkPhysicalDeviceProperties d_properties;

    DeviceFeatures d_supportedFeatures;
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

VPP_INLINE const VkPhysicalDeviceProperties& PhysicalDevice :: properties() const
{
    return get()->d_properties;
}

// -----------------------------------------------------------------------------

VPP_INLINE const DeviceFeatures& PhysicalDevice :: features() const
{
    return get()->d_supportedFeatures;
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

VPP_INLINE bool DeviceFeatures :: operator[] ( VkBool32 VkPhysicalDeviceFeatures::* feature ) const
{
    return features.*feature != VK_FALSE;
}

// -----------------------------------------------------------------------------

template< class FeatureStructT >
VPP_INLINE bool DeviceFeatures :: operator[] ( VkBool32 FeatureStructT::* feature ) const
{
    return static_cast< const FeatureStructT* >( this )->*feature != VK_FALSE;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceFeatures :: enableIfSupported (
    VkBool32 VkPhysicalDeviceFeatures::* feature, const PhysicalDevice& hDevice )
{
    if ( hDevice.features()[ feature ] )
    {
        features.*feature = VK_TRUE;
        return true;
    }
    else
        return false;
}

// -----------------------------------------------------------------------------

template< class FeatureStructT >
VPP_INLINE bool DeviceFeatures :: enableIfSupported (
    VkBool32 FeatureStructT::* feature, const PhysicalDevice& hDevice )
{
    if ( hDevice.features()[ feature ] )
    {
        static_cast< FeatureStructT* >( this )->*feature = VK_TRUE;
        return true;
    }
    else
        return false;
}

// -----------------------------------------------------------------------------

template< class FeatureStructT >
VPP_INLINE const char* DeviceFeatures :: getFeatureName ( VkBool32 FeatureStructT::* feature )
{
    return detail::s_featureNames.findName ( feature );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPHYSICALDEVICE_HPP


