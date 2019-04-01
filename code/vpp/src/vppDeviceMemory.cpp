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
#include "../include/vppDeviceMemory.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

const std::uint32_t MemProfile :: s_deviceStatic[] =
{
    DEVICE_HOST_SEPARATE
};

// -----------------------------------------------------------------------------

const std::uint32_t MemProfile :: s_deviceDynamic[] =
{
    DEVICE_HOST_COHERENT_CACHED,
    DEVICE_HOST_COHERENT,
    HOST_DEVICE_COHERENT_CACHED,
    HOST_DEVICE_COHERENT
};

// -----------------------------------------------------------------------------

const std::uint32_t MemProfile :: s_deviceFeedback[] =
{
    HOST_DEVICE_COHERENT
};

// -----------------------------------------------------------------------------

const std::uint32_t MemProfile :: s_deviceOnly[] =
{
    DEVICE_HOST_SEPARATE
};

// -----------------------------------------------------------------------------

const std::uint32_t MemProfile :: s_hostStatic[] =
{
    HOST_DEVICE_COHERENT_CACHED,
    HOST_DEVICE_COHERENT
};

// -----------------------------------------------------------------------------
// aligned to MemProfile::ECharacteristic

const std::uint32_t* MemProfile :: s_wantedProperties [ ECharacteristic_count ] =
{
    s_deviceStatic,
    s_deviceDynamic,
    s_deviceFeedback,
    s_deviceOnly,
    s_hostStatic
};

// -----------------------------------------------------------------------------
// aligned to MemProfile::ECharacteristic

const size_t MemProfile :: s_wantedPropertiesCount [ ECharacteristic_count ] =
{
    sizeof ( s_deviceStatic ) / sizeof ( std::uint32_t ),
    sizeof ( s_deviceDynamic ) / sizeof ( std::uint32_t ),
    sizeof ( s_deviceFeedback ) / sizeof ( std::uint32_t ),
    sizeof ( s_deviceOnly ) / sizeof ( std::uint32_t ),
    sizeof ( s_hostStatic ) / sizeof ( std::uint32_t )
};

// -----------------------------------------------------------------------------

MemProfile :: MemProfile (
    const std::uint32_t* pWantedProperties,
    size_t nWantedProperties,
    bool bForceHostVisible ) :
        d_pWantedProperties ( pWantedProperties ),
        d_nWantedProperties ( nWantedProperties ),
        d_bForceHostVisible ( bForceHostVisible )
{
}

// -----------------------------------------------------------------------------

MemProfile :: MemProfile (
    const MemProfile& rhs,
    bool bForceHostVisible ) :
        d_pWantedProperties ( rhs.d_pWantedProperties ),
        d_nWantedProperties ( rhs.d_nWantedProperties ),
        d_bForceHostVisible ( bForceHostVisible )
{
}

// -----------------------------------------------------------------------------

MemProfile :: MemProfile ( ECharacteristic eChar ) :
    d_pWantedProperties ( s_wantedProperties [ eChar ] ),
    d_nWantedProperties ( s_wantedPropertiesCount [ eChar ] ),
    d_bForceHostVisible ( false )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VkDeviceSize DeviceMemory :: availableMemory (
    const MemProfile& memProfile,
    Device hDevice )
{
    const VkPhysicalDeviceMemoryProperties devMemProperties =
        hDevice.physical().getMemoryProperties();

    std::uint32_t memoryTypeIndex = std::numeric_limits< std::uint32_t >::max();

    for ( auto iProperties : memProfile )
    {
        const std::uint32_t properties = (
            iProperties
            | ( memProfile.isForceHostVisible() ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0 ) );

        memoryTypeIndex = DeviceMemoryImpl::findMemoryTypeIndex (
            0xFFFFFFFFu, properties, devMemProperties );

        if ( memoryTypeIndex != std::numeric_limits< std::uint32_t >::max() )
            break;
    }

    if ( memoryTypeIndex == std::numeric_limits< std::uint32_t >::max() )
    {
        // fallback -- according to "10.2. Device Memory", there must be
        // at least the following memory types present:
        // - isolated device-local
        // - shared (host/device) host-local memory with coherency (may be slow)

        std::uint32_t properties = (
            *memProfile.begin()
            | ( memProfile.isForceHostVisible() ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0 ) );

        if ( properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
            properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        else
            properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        memoryTypeIndex = DeviceMemoryImpl::findMemoryTypeIndex ( 0xFFFFFFFFu, properties, devMemProperties );
    }

    const std::uint32_t heapIndex =
        devMemProperties.memoryTypes [ memoryTypeIndex ].heapIndex;

    return devMemProperties.memoryHeaps [ heapIndex ].size;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

DeviceMemoryImpl :: DeviceMemoryImpl (
    VkDeviceSize size,
    std::uint32_t typeMask,
    const MemProfile& memProfile,
    Device hDevice ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result ( VK_ERROR_OUT_OF_DEVICE_MEMORY ),
        d_properties ( 0 ),
        d_pMappedBegin ( 0 ),
        d_pMappedEnd ( 0 ),
        d_size ( size )
{
    const VkPhysicalDeviceMemoryProperties devMemProperties =
        d_hDevice.physical().getMemoryProperties();

    std::uint32_t memoryTypeIndex = std::numeric_limits< std::uint32_t >::max();

    for ( auto iProperties : memProfile )
    {
        const std::uint32_t properties = (
            iProperties
            | ( memProfile.isForceHostVisible() ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0 ) );

        memoryTypeIndex = findMemoryTypeIndex (
            typeMask, properties, devMemProperties );

        if ( memoryTypeIndex != std::numeric_limits< std::uint32_t >::max() )
        {
            d_properties = properties;
            break;
        }
    }

    if ( memoryTypeIndex == std::numeric_limits< std::uint32_t >::max() )
    {
        // fallback -- according to "10.2. Device Memory", there must be
        // at least the following memory types present:
        // - isolated device-local
        // - shared (host/device) host-local memory with coherency (may be slow)

        const std::uint32_t properties = (
            *memProfile.begin()
            | ( memProfile.isForceHostVisible() ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0 ) );

        if ( properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
            d_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        else
            d_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        memoryTypeIndex = findMemoryTypeIndex ( typeMask, d_properties, devMemProperties );
    }

    {
        VkMemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = 0;
        memoryAllocateInfo.allocationSize = size;
        memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

        d_result = ::vkAllocateMemory (
            d_hDevice.handle(), & memoryAllocateInfo, 0, & d_handle );
    }
}

// -----------------------------------------------------------------------------

DeviceMemoryImpl :: ~DeviceMemoryImpl()
{
    VPP_EXTSYNC_MTX_SLOCK ( this );

    if ( d_result == VK_SUCCESS )
    {
        ::vkFreeMemory ( d_hDevice.handle(), d_handle, 0 );
        d_result = VK_NOT_READY;
    }
}

// -----------------------------------------------------------------------------

std::uint32_t DeviceMemoryImpl :: findMemoryTypeIndex (
    std::uint32_t typeMask,
    std::uint32_t properties,
    const VkPhysicalDeviceMemoryProperties& devMemProperties )
{
    for ( std::uint32_t i = 0; i < devMemProperties.memoryTypeCount; ++i )
    {
        if ( ( typeMask & ( 1 << i ) )
               && ( ( devMemProperties.memoryTypes [ i ].propertyFlags & properties ) == properties ) )
        {
            return i;
        }
    }

    return std::numeric_limits< std::uint32_t >::max();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

MappableDeviceMemory :: MappableDeviceMemory ( const DeviceMemory& mem ) :
    DeviceMemory ( mem )
{
    if ( ! ( mem.properties() & DeviceMemory::HOST_VISIBLE ) )
        throw XDeviceMemoryTypeError();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
