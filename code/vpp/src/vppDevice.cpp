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
#include "../include/vppDevice.hpp"
#include "../include/vppCommandPool.hpp"
#include "../include/vppPipelineCache.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

DeviceImpl :: DeviceImpl (
    const PhysicalDevice& hPhysicalDevice,
    const std::vector< float >& queuePriorities,
    const VkPhysicalDeviceFeatures* pFeatures ) :
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
    std::vector< const char * > enabledExtensions;
    enabledExtensions.push_back ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

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

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = 0;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = static_cast< std::uint32_t >( deviceQueueCreateInfos.size() );
    createInfo.pQueueCreateInfos = & deviceQueueCreateInfos [ 0 ];
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = 0;
    createInfo.enabledExtensionCount = static_cast< std::uint32_t >( enabledExtensions.size() );
    createInfo.ppEnabledExtensionNames = & enabledExtensions [ 0 ];
    createInfo.pEnabledFeatures = pFeatures;

    d_result = ::vkCreateDevice ( hPhysicalDevice.handle(), & createInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

DeviceImpl :: DeviceImpl (
    const PhysicalDevice& hPhysicalDevice,
    const VkPhysicalDeviceFeatures* pFeatures ) :
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
} // namespace vpp
// -----------------------------------------------------------------------------
