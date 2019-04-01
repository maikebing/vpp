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
#include "../include/vppDevice.hpp"
#include "../include/vppCommandPool.hpp"
#include "../include/vppPipelineCache.hpp"
#include "../include/vppInstance.hpp"

#include <iterator>

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

/*
    Known device extensions:                                   Dependency:
        VK_EXT_blend_operation_advanced                             -
        VK_EXT_conditional_rendering                                -
        VK_EXT_conservative_rasterization                           -
        VK_EXT_depth_range_unrestricted                             -
        VK_EXT_descriptor_indexing                                  -
        VK_EXT_discard_rectangles                                   -
        VK_EXT_external_memory_host                      VK_KHR_external_memory     
        VK_EXT_hdr_metadata                                 VK_KHR_swapchain
        VK_EXT_inline_uniform_block                                 -       
        VK_EXT_post_depth_coverage                                  -
        VK_EXT_sample_locations                                     -
        VK_EXT_sampler_filter_minmax                                -
        VK_EXT_scalar_block_layout                                  -    
        VK_EXT_shader_subgroup_ballot                               -
        VK_EXT_shader_subgroup_vote                                 -   
        VK_EXT_shader_viewport_index_layer                          -
        VK_EXT_transform_feedback                                   -     
        VK_EXT_vertex_attribute_divisor                             -

        VK_KHR_16bit_storage(p)                    VK_KHR_storage_buffer_storage_class
        VK_KHR_8bit_storage                        VK_KHR_storage_buffer_storage_class
        VK_KHR_bind_memory2(p)                                      -
        VK_KHR_create_renderpass2                          VK_KHR_multiview
        VK_KHR_dedicated_allocation(p)                 VK_KHR_get_memory_requirements2
        VK_KHR_depth_stencil_resolve                       VK_KHR_create_renderpass2
        VK_KHR_descriptor_update_template(p)                           -
        VK_KHR_device_group(p)                           VK_KHR_device_group_creation
        VK_KHR_draw_indirect_count                                    -     
        VK_KHR_driver_properties                                      -     
        VK_KHR_external_fence(p)                      VK_KHR_external_fence_capabilities           
        VK_KHR_external_fence_win32                         VK_KHR_external_fence     
        VK_KHR_external_memory(p)                    VK_KHR_external_memory_capabilities
        VK_KHR_external_memory_win32                        VK_KHR_external_memory     
        VK_KHR_external_semaphore(p)               VK_KHR_external_semaphore_capabilities    
        VK_KHR_external_semaphore_win32                     VK_KHR_external_semaphore
        VK_KHR_get_memory_requirements2(p)                            -
        VK_KHR_image_format_list                                      -
        VK_KHR_maintenance1(p)                                        -
        VK_KHR_maintenance2(p)                                        -
        VK_KHR_maintenance3(p)                                        -           
        VK_KHR_multiview(p)                                           -  
        VK_KHR_push_descriptor                                        -
        VK_KHR_relaxed_block_layout(p)                                -
        VK_KHR_sampler_mirror_clamp_to_edge                           -
        VK_KHR_sampler_ycbcr_conversion(p)    VK_KHR_bind_memory2,VK_KHR_get_memory_requirements2   
        VK_KHR_shader_atomic_int64                                    -
        VK_KHR_shader_draw_parameters(p)                              -
        VK_KHR_shader_float16_int8                                    -
        VK_KHR_shader_float_controls                                  -
        VK_KHR_storage_buffer_storage_class(p)                        -
        VK_KHR_swapchain                                        VK_KHR_surface     
        VK_KHR_swapchain_mutable_format        VK_KHR_swapchain, VK_KHR_image_format_list
        VK_KHR_variable_pointers(p)                  VK_KHR_storage_buffer_storage_class      
        VK_KHR_vulkan_memory_model                                    -
        VK_KHR_win32_keyed_mutex                        VK_KHR_external_memory_win32


    V1.1 promoted extensions:
        VK_KHR_16bit_storage
        VK_KHR_bind_memory2
        VK_KHR_dedicated_allocation
        VK_KHR_descriptor_update_template
        VK_KHR_device_group
        VK_KHR_external_fence
        VK_KHR_external_memory
        VK_KHR_external_semaphore
        VK_KHR_get_memory_requirements2
        VK_KHR_maintenance1
        VK_KHR_maintenance2
        VK_KHR_maintenance3
        VK_KHR_multiview
        VK_KHR_relaxed_block_layout
        VK_KHR_sampler_ycbcr_conversion
        VK_KHR_shader_draw_parameters
        VK_KHR_storage_buffer_storage_class
        VK_KHR_variable_pointers
*/

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

DeviceImpl :: DeviceImpl (
    const PhysicalDevice& hPhysicalDevice,
    const std::vector< float >& queuePriorities,
    const DeviceFeatures* pFeatures ) :
        d_hPhysicalDevice ( hPhysicalDevice ),
        d_handle(),
        d_result(),
        d_graphicsQueueFamily ( 0 ),
        d_graphicsQueueCount ( 0 ),
        d_queuePriorities ( queuePriorities ),
        d_transferQueueFamily ( 0 ),
        d_transferQueueCount ( 0 ),
        d_pDefaultGraphicsCmdPool ( 0 ),
        d_pDefaultTransferCmdPool ( 0 ),
        d_pDefaultPipelineCache ( 0 )
{
    d_enabledExtensions.emplace ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
    d_enabledExtensions.emplace ( VK_KHR_MAINTENANCE1_EXTENSION_NAME );
    d_enabledExtensions.emplace ( VK_KHR_MAINTENANCE2_EXTENSION_NAME );
    d_enabledExtensions.emplace ( VK_KHR_MAINTENANCE3_EXTENSION_NAME );

    if ( pFeatures )
    {
        d_enabledFeatures = *pFeatures;
        d_enabledFeatures.resolveDependencies();
        d_enabledFeatures.getEnabledExtensions ( & d_enabledExtensions );
        d_enabledFeatures.getSourceExtensions ( & d_sourceExtensions );
    }

    typedef std::vector< VkDeviceQueueCreateInfo > DeviceQueueCreateInfos;
    DeviceQueueCreateInfos deviceQueueCreateInfos;

    const std::uint32_t nQueues = static_cast< std::uint32_t >( d_queuePriorities.size() );

    std::uint32_t familyIndex = 0;
    const std::uint32_t nFamilies = static_cast< std::uint32_t >( d_hPhysicalDevice.queueFamilyCount() );

    for ( std::uint32_t iFamily = 0; iFamily != nFamilies; ++iFamily )
    {
        const auto& family = d_hPhysicalDevice.getQueueFamilyProperties ( iFamily );

        if ( ( family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) && family.queueCount > 0 )
        {
            if ( ! d_graphicsQueueCount )
            {
                d_graphicsQueueFamily = familyIndex;
                d_graphicsQueueCount = std::min ( family.queueCount, nQueues );

                deviceQueueCreateInfos.emplace_back ( VkDeviceQueueCreateInfo {
                    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    0, 0, familyIndex, d_graphicsQueueCount, & d_queuePriorities [ 0 ]
                } );
            }
        }
        else if ( ( family.queueFlags & VK_QUEUE_TRANSFER_BIT ) && family.queueCount > 0 )
        {
            if ( ! d_transferQueueCount )
            {
                d_transferQueueFamily = familyIndex;
                d_transferQueueCount = 1u;
                float priority = 1;

                deviceQueueCreateInfos.emplace_back ( VkDeviceQueueCreateInfo {
                    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    0, 0, familyIndex, d_transferQueueCount, & priority
                } );
            }
        }

        ++familyIndex;
    }

    if ( ! d_transferQueueCount )
    {
        d_transferQueueFamily = d_graphicsQueueFamily;
        d_transferQueueCount = d_graphicsQueueCount;
    }

    const SVulkanVersion& deviceVersion =
        SVulkanVersion::fromId ( hPhysicalDevice.properties().apiVersion );

    const SVulkanVersion& instanceVersion = Instance::supportedVersion();

    d_supportedVersion = std::min ( deviceVersion, instanceVersion );

    removePromotedExtensionNames ( d_supportedVersion );
    removeUnsupportedExtensionNames();

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = static_cast< std::uint32_t >( deviceQueueCreateInfos.size() );
    createInfo.pQueueCreateInfos = & deviceQueueCreateInfos [ 0 ];

    std::vector< const char* > enabledExtensions ( d_enabledExtensions.size(), 0 );

    std::transform (
        d_enabledExtensions.begin(), d_enabledExtensions.end(), enabledExtensions.begin(),
        []( const std::string& extName ) { return extName.c_str(); }
    );

    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = 0;
    createInfo.enabledExtensionCount = static_cast< std::uint32_t >( enabledExtensions.size() );
    createInfo.ppEnabledExtensionNames = & enabledExtensions [ 0 ];

    if ( pFeatures )
    {
        if ( ! ( d_supportedVersion < SVulkanVersion { 1, 1, 0 } ) )
        {
            createInfo.pNext = static_cast< VkPhysicalDeviceFeatures2* >( & d_enabledFeatures );
            createInfo.pEnabledFeatures = 0;
        }
        else
        {
            createInfo.pNext = 0;
            createInfo.pEnabledFeatures =
                & static_cast< VkPhysicalDeviceFeatures2* >( & d_enabledFeatures )->features;
        }
    }
    else
    {
        createInfo.pNext = 0;
        createInfo.pEnabledFeatures = 0;
    }

    d_result = ::vkCreateDevice ( hPhysicalDevice.handle(), & createInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

DeviceImpl :: DeviceImpl (
    const PhysicalDevice& hPhysicalDevice,
    const DeviceFeatures* pFeatures ) :
        DeviceImpl ( hPhysicalDevice, getDefaultQueuePriorities(), pFeatures )
{
}

// -----------------------------------------------------------------------------

DeviceImpl :: ~DeviceImpl()
{
    VPP_EXTSYNC_MTX_SLOCK ( this );

    delete d_pDefaultGraphicsCmdPool;
    delete d_pDefaultTransferCmdPool;

    if ( d_result == VK_SUCCESS )
    {
        ::vkDestroyDevice ( d_handle, 0 );
        d_result = VK_NOT_READY;
    }
}

// -----------------------------------------------------------------------------

const std::vector< float >& DeviceImpl :: getDefaultQueuePriorities()
{
    static std::vector< float > s_result ( 1, 1.0f );
    return s_result;
}

// -----------------------------------------------------------------------------

void DeviceImpl :: removePromotedExtensionNames ( const SVulkanVersion& currentVersion )
{
    static const SVulkanVersion s_vk11 { 1, 1, 0 };

    static const std::vector< const char* > s_vk11promoted =
    {
        "VK_KHR_16bit_storage",
        "VK_KHR_bind_memory2",
        "VK_KHR_dedicated_allocation",
        "VK_KHR_descriptor_update_template",
        "VK_KHR_device_group",
        "VK_KHR_external_fence",
        "VK_KHR_external_memory",
        "VK_KHR_external_semaphore",
        "VK_KHR_get_memory_requirements2",
        "VK_KHR_maintenance1",
        "VK_KHR_maintenance2",
        "VK_KHR_maintenance3",
        "VK_KHR_multiview",
        "VK_KHR_relaxed_block_layout",
        "VK_KHR_sampler_ycbcr_conversion",
        "VK_KHR_shader_draw_parameters",
        "VK_KHR_storage_buffer_storage_class",
        "VK_KHR_variable_pointers"
    };

    if ( ! ( currentVersion < s_vk11 ) )
        for ( const char* pExtName : s_vk11promoted )
            d_enabledExtensions.erase ( std::string ( pExtName ) );
}

// -----------------------------------------------------------------------------

void DeviceImpl :: removeUnsupportedExtensionNames()
{
    ExtensionProperties extProps;
    d_hPhysicalDevice.enumerateExtensions ( & extProps );

    std::set< std::string > supportedExtensions;

    for ( const auto& iExt : extProps )
        supportedExtensions.emplace ( iExt.extensionName );
    
    std::set< std::string > validExtensions;

    std::set_intersection (
        d_enabledExtensions.begin(), d_enabledExtensions.end(), 
        supportedExtensions.begin(), supportedExtensions.end(), 
        std::inserter ( validExtensions, validExtensions.end() ) );

    d_enabledExtensions.swap ( validExtensions );
}

// -----------------------------------------------------------------------------

float Device :: queuePriority ( std::uint32_t iQueue ) const
{
    return get()->d_queuePriorities [ iQueue ];
}

// -----------------------------------------------------------------------------

CommandPool& Device :: defaultCmdPool ( EQueueType queueType ) const
{
    DeviceImpl* pImpl = get();

    if ( queueType == Q_TRANSFER
         && pImpl->d_graphicsQueueFamily != pImpl->d_transferQueueFamily )
    {
        if ( ! pImpl->d_pDefaultTransferCmdPool )
            pImpl->d_pDefaultTransferCmdPool = new CommandPool (
                *this, Q_TRANSFER, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );

        return *pImpl->d_pDefaultTransferCmdPool;
    }
    else
    {
        if ( ! pImpl->d_pDefaultGraphicsCmdPool )
            pImpl->d_pDefaultGraphicsCmdPool = new CommandPool (
                *this, Q_GRAPHICS, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );

        return *pImpl->d_pDefaultGraphicsCmdPool;
    }
}

// -----------------------------------------------------------------------------

PipelineCache& Device :: defaultPipelineCache() const
{
    DeviceImpl* pImpl = get();

    if ( ! pImpl->d_pDefaultPipelineCache )
        pImpl->d_pDefaultPipelineCache = new PipelineCache ( *this );

    return *pImpl->d_pDefaultPipelineCache;
}

// -----------------------------------------------------------------------------

bool Device :: supportsVersion ( const SVulkanVersion& ver ) const
{
    return ! ( get()->d_supportedVersion < ver );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
