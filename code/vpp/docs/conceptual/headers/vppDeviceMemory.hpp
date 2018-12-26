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
    \brief Abstraction of GPU-interoperable memory types.

    Allocation of memory is somewhat complex in core Vulkan, due to large number
    of memory properties and types. This class hides that complexity and
    provides simple classification into four major types.
*/

class MemProfile
{
public:
    /**
        \brief Enumeration specifying the characteristic of requested memory.
    */
    enum ECharacteristic
    {
        DEVICE_STATIC,    /**< \brief Memory resides physically on GPU and is hidden from CPU (but fast). */
        HOST_STATIC,      /**< \brief Memory resides physically on CPU side and is accessible to GPU, likely slower than GPU-local memory. */
        DEVICE_DYNAMIC,   /**< \brief Memory resides physically on GPU and is visible to CPU. If that scenario is not supported by the hardware, falls back to HOST_STATIC. */
        DEVICE_FEEDBACK,  /**< \brief Like HOST_STATIC, but always uncached. */
        DEVICE_ONLY       /**< \brief In this context, same as DEVICE_STATIC. */
    };

    /**
        \brief The constructor - not explicit, you can use the enumeration directly.
    */
    MemProfile ( ECharacteristic eChar );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/**
    \brief
*/

class DeviceMemory
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
    
    VkDeviceMemory handle() const;
    bool valid() const;
    std::uint32_t properties() const;
    VkDeviceSize size() const;

    bool isHostVisible() const;
    bool isHostCoherent() const;

    static size_t availableMemory (
        const MemProfile& memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------
/**
    \brief
*/

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
/**
    \brief A compound object containing references to a buffer and memory bound to it.

    This object is reference-counted and may be passed by value.
*/

template< class ResourceT, class MemoryT >
class MemoryBinding
{
public:
    /** \brief Constructs null reference. */
    MemoryBinding();

    /** \brief Binds memory to a buffer and constructs MemoryBinding object. */
    MemoryBinding (
        const ResourceT& res,
        const MemProfile& memProfile );

    /** \brief Retrieves the buffer. */
    const ResourceT& resource() const;

    /** \brief Retrieves the memory object. */
    MemoryT& memory();
};

// -----------------------------------------------------------------------------
/**
    \brief Binds specified buffer to a DeviceMemory memory, and returns MemoryBinding object.

    The buffer (vpp::Buf or vpp::Buffer subclass) is specified a the \c res parameter.

    It is a convenient practice to use C++ \c auto type inference to declare the result
    for this function.
*/

template< class ResourceT >
MemoryBinding< ResourceT, DeviceMemory > bindDeviceMemory (
    const ResourceT& res, const MemProfile& memProfile );

// -----------------------------------------------------------------------------
/**
    \brief Binds specified buffer to a MappableDeviceMemory memory, and returns MemoryBinding object.

    The buffer (vpp::Buf or vpp::Buffer subclass) is specified a the \c res parameter.

    It is a convenient practice to use C++ \c auto type inference to declare the result
    for this function.
*/

template< class ResourceT >
MemoryBinding< ResourceT, MappableDeviceMemory > bindMappableMemory (
    const ResourceT& res, const MemProfile& memProfile );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
