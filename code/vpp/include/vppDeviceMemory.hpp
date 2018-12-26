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

#ifndef INC_VPPDEVICEMEMORY_HPP
#define INC_VPPDEVICEMEMORY_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class DeviceMemoryImpl;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class MemProfile
{
public:
    VPP_DLLAPI MemProfile (
        const std::uint32_t* pWantedProperties,
        size_t nWantedProperties,
        bool bForceHostVisible = false );

    VPP_DLLAPI MemProfile (
        const MemProfile& rhs,
        bool bForceHostVisible );

    enum ECharacteristic
    {
        DEVICE_STATIC,
        DEVICE_DYNAMIC,
        DEVICE_FEEDBACK,
        DEVICE_ONLY,
        HOST_STATIC,

        ECharacteristic_count
    };

    enum EAllowedProperties
    {
        DEVICE_HOST_COHERENT_CACHED =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,

        DEVICE_HOST_COHERENT =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,

        DEVICE_HOST_INCOHERENT =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_CACHED_BIT,

        DEVICE_HOST_SEPARATE =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,

        HOST_DEVICE_COHERENT_CACHED =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        
        HOST_DEVICE_COHERENT =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,

        HOST_DEVICE_INCOHERENT =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
    };

    VPP_DLLAPI MemProfile ( ECharacteristic eChar );

    typedef const std::uint32_t* iterator;

    VPP_INLINE iterator begin() const { return d_pWantedProperties; }
    VPP_INLINE iterator end() const { return d_pWantedProperties + d_nWantedProperties; }
    VPP_INLINE bool isForceHostVisible() const { return d_bForceHostVisible; }

private:
    const std::uint32_t* d_pWantedProperties;
    const size_t d_nWantedProperties;
    bool d_bForceHostVisible;

private:
    static const std::uint32_t* s_wantedProperties [ ECharacteristic_count ];
    static const size_t s_wantedPropertiesCount [ ECharacteristic_count ];

    static const std::uint32_t s_deviceStatic[];
    static const std::uint32_t s_deviceDynamic[];
    static const std::uint32_t s_deviceFeedback[];
    static const std::uint32_t s_hostStatic[];
    static const std::uint32_t s_deviceOnly[];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class DeviceMemory : public TSharedReference< DeviceMemoryImpl >
{
public:
    DeviceMemory();

    enum EProperties
    {
        DEVICE_LOCAL = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        HOST_VISIBLE = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        HOST_COHERENT = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        HOST_CACHED = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
        LAZILY_ALLOCATED = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
    };

    DeviceMemory (
        VkDeviceSize size,
        std::uint32_t typeMask,
        const MemProfile& memProfile,
        Device hDevice );

    ~DeviceMemory();
    
    VkDeviceMemory handle() const;
    bool valid() const;
    std::uint32_t properties() const;
    VkDeviceSize size() const;

    bool isHostVisible() const;
    bool isHostCoherent() const;

    VPP_DLLAPI static VkDeviceSize availableMemory (
        const MemProfile& memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class DeviceMemoryImpl : public TSharedObject< DeviceMemoryImpl >
{
public:
    VPP_DLLAPI DeviceMemoryImpl (
        VkDeviceSize size,
        std::uint32_t typeMask,
        const MemProfile& memProfile,
        Device hDevice );

    VPP_DLLAPI ~DeviceMemoryImpl();

    VPP_EXTSYNC_METHODS_DECLARE ( this );

private:
    static std::uint32_t findMemoryTypeIndex (
        std::uint32_t typeMask,
        std::uint32_t properties,
        const VkPhysicalDeviceMemoryProperties& devMemProperties );

private:
    friend class DeviceMemory;
    friend class MappableDeviceMemory;

    Device d_hDevice;
    VkDeviceMemory d_handle;
    VkResult d_result;
    std::uint32_t d_properties;

    unsigned char* d_pMappedBegin;
    unsigned char* d_pMappedEnd;
    VkDeviceSize d_size;

    VPP_EXTSYNC_MTX_DECLARE;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE DeviceMemory :: DeviceMemory()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE DeviceMemory :: DeviceMemory (
    VkDeviceSize s,
    std::uint32_t typeMask,
    const MemProfile& memProfile,
    Device hDevice ) :
        TSharedReference< DeviceMemoryImpl > ( new DeviceMemoryImpl (
            s, typeMask, memProfile, hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE DeviceMemory :: ~DeviceMemory()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceMemory DeviceMemory :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceMemory :: valid() const
{
    return get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t DeviceMemory :: properties() const
{
    return get()->d_properties;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize DeviceMemory :: size() const
{
    return get()->d_size;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceMemory :: isHostVisible() const
{
    return ( properties() & HOST_VISIBLE ) != 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool DeviceMemory :: isHostCoherent() const
{
    return ( properties() & HOST_COHERENT ) != 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class MappableDeviceMemory : public DeviceMemory
{
public:
    MappableDeviceMemory();

    MappableDeviceMemory (
        VkDeviceSize size,
        std::uint32_t typeMask,
        const MemProfile& memProfile,
        Device hDevice );

    MappableDeviceMemory ( const DeviceMemory& mem );

    unsigned char* beginMapped() const;
    unsigned char* endMapped() const;

    VkResult map ( VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE );
    void unmap();

    void syncFromDevice();
    void syncToDevice();

    void load ( const void* pBegin, size_t size ); 
};

// -----------------------------------------------------------------------------

VPP_INLINE MappableDeviceMemory :: MappableDeviceMemory()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE MappableDeviceMemory :: MappableDeviceMemory (
    VkDeviceSize size,
    std::uint32_t typeMask,
    const MemProfile& memProfile,
    Device hDevice ) :
        DeviceMemory (
            size, typeMask,
            MemProfile ( memProfile, true ),
            hDevice )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned char* MappableDeviceMemory :: beginMapped() const
{
    return get()->d_pMappedBegin;
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned char* MappableDeviceMemory :: endMapped() const
{
    return get()->d_pMappedEnd;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult MappableDeviceMemory :: map ( VkDeviceSize offset, VkDeviceSize size )
{
    VPP_EXTSYNC_MTX_SLOCK ( get() );

    VkResult result = ::vkMapMemory (
        get()->d_hDevice.handle(), get()->d_handle, offset, size, 0,
        reinterpret_cast< void** > ( & get()->d_pMappedBegin ) );

    VkDeviceSize actualSize = std::min ( size, get()->d_size );

    if ( result == VK_SUCCESS )
        get()->d_pMappedEnd = get()->d_pMappedBegin + actualSize;
    else
        get()->d_pMappedEnd = get()->d_pMappedBegin = 0;

    return result;
}

// -----------------------------------------------------------------------------

VPP_INLINE void MappableDeviceMemory :: unmap()
{
    VPP_EXTSYNC_MTX_SLOCK ( get() );

    if ( get()->d_pMappedBegin )
        ::vkUnmapMemory ( get()->d_hDevice.handle(), get()->d_handle );

    get()->d_pMappedEnd = get()->d_pMappedBegin = 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE void MappableDeviceMemory :: syncFromDevice()
{
    if ( ! isHostCoherent() )
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = 0;
        memoryRange.memory = handle();
        memoryRange.offset = 0;
        memoryRange.size = size();

        ::vkInvalidateMappedMemoryRanges ( get()->d_hDevice.handle(), 1, & memoryRange );
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE void MappableDeviceMemory :: syncToDevice()
{
    if ( ! isHostCoherent() )
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = 0;
        memoryRange.memory = handle();
        memoryRange.offset = 0;
        memoryRange.size = size();

        ::vkFlushMappedMemoryRanges ( get()->d_hDevice.handle(), 1, & memoryRange );
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE void MappableDeviceMemory :: load ( const void* pBegin, size_t size )
{
    map ( 0, size );
    std::memcpy ( beginMapped(), pBegin, size );
    unmap();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ResourceT, class MemoryT >
class MemoryBinding
{
public:
    MemoryBinding();

    MemoryBinding (
        const ResourceT& res,
        const MemProfile& memProfile );

    const ResourceT& resource() const;
    MemoryT& memory();

public:
    ResourceT d_resource;
    MemoryT d_memory;
};

// -----------------------------------------------------------------------------

template< class ResourceT, class MemoryT >
VPP_INLINE MemoryBinding< ResourceT, MemoryT > :: MemoryBinding()
{
}

// -----------------------------------------------------------------------------

template< class ResourceT, class MemoryT >
VPP_INLINE MemoryBinding< ResourceT, MemoryT > :: MemoryBinding (
    const ResourceT& res,
    const MemProfile& memProfile ) :
        d_resource ( res ),
        d_memory ( res.template bindMemory< MemoryT >( memProfile ) )
{
}

// -----------------------------------------------------------------------------

template< class ResourceT, class MemoryT >
VPP_INLINE const ResourceT& MemoryBinding< ResourceT, MemoryT > :: resource() const
{
    return d_resource;
}

// -----------------------------------------------------------------------------

template< class ResourceT, class MemoryT >
VPP_INLINE MemoryT& MemoryBinding< ResourceT, MemoryT > :: memory()
{
    return d_memory;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ResourceT >
VPP_INLINE MemoryBinding< ResourceT, DeviceMemory > bindDeviceMemory (
    const ResourceT& res,
    const MemProfile& memProfile )
{
    return MemoryBinding< ResourceT, DeviceMemory >( res, memProfile ); 
}

// -----------------------------------------------------------------------------

template< class ResourceT >
VPP_INLINE MemoryBinding< ResourceT, MappableDeviceMemory > bindMappableMemory (
    const ResourceT& res,
    const MemProfile& memProfile )
{
    return MemoryBinding< ResourceT, MappableDeviceMemory >( res, memProfile ); 
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPDEVICEMEMORY_HPP
