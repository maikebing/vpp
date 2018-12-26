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

#ifndef INC_VPPBUFFER_HPP
#define INC_VPPBUFFER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICEMEMORY_HPP
#include "vppDeviceMemory.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class BufferImpl;

// -----------------------------------------------------------------------------

class Buf : public vpp::TSharedReference< BufferImpl >
{
public:
    enum EUsageFlags
    {
        SOURCE = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        TARGET = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        UNITEX = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
        STORTEX = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
        UNIFORM = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        STORAGE = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        INDEX = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VERTEX = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        INDIRECT = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
    };

    enum ECreationFlags
    {
        SPARSE_BINDING = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
        SPARSE_RESIDENCY = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
        SPARSE_ALIASED = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT
    };

    Buf();

    Buf (
        VkDeviceSize size,
        std::uint32_t usageMask,
        const Device& hDevice,
        const std::vector< std::uint32_t >& queueFamilyIndices,
        std::uint32_t flags = 0 );

    Buf (
        VkDeviceSize size,
        std::uint32_t usageMask,
        const Device& hDevice,
        std::uint32_t flags = 0 );

    ~Buf();

    VkBuffer handle() const;
    const Device& device() const;
    VkDeviceSize size() const;
    std::uint32_t getUsage() const;

    VPP_DLLAPI std::uint32_t barrierDestStageHint() const;
    VPP_DLLAPI std::uint32_t barrierDestAccessHint() const;

    template< class MemoryT >
    MemoryT bindMemory ( const MemProfile& memProfile ) const;
};

// -----------------------------------------------------------------------------

class BufferImpl : public vpp::TSharedObject< BufferImpl >
{
public:
    VPP_DLLAPI BufferImpl (
        VkDeviceSize size,
        std::uint32_t usageMask,
        const Device& hDevice,
        const std::vector< std::uint32_t >& queueFamilyIndices,
        std::uint32_t flags = 0 );

    VPP_DLLAPI BufferImpl (
        VkDeviceSize size,
        std::uint32_t usageMask,
        const Device& hDevice,
        std::uint32_t flags = 0 );

    VPP_DLLAPI ~BufferImpl();

    VPP_INLINE bool compareObjects ( const BufferImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class Buf;
    Device d_hDevice;
    VkBuffer d_handle;
    DeviceMemory d_memory;
    VkDeviceSize d_size;
    std::uint32_t d_usage;
};

// -----------------------------------------------------------------------------

VPP_INLINE Buf :: Buf()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Buf :: Buf (
    VkDeviceSize size,
    std::uint32_t usageMask,
    const Device& hDevice,
    const std::vector< std::uint32_t >& queueFamilyIndices,
    std::uint32_t flags ) :
        TSharedReference< BufferImpl >( new BufferImpl (
            size, usageMask, hDevice, queueFamilyIndices, flags ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Buf :: Buf (
    VkDeviceSize size,
    std::uint32_t usageMask,
    const Device& hDevice,
    std::uint32_t flags ) :
        TSharedReference< BufferImpl >( new BufferImpl (
            size, usageMask, hDevice, flags ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Buf :: ~Buf()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkBuffer Buf :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& Buf :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize Buf :: size() const
{
    return get()->d_size;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Buf :: getUsage() const
{
    return get()->d_usage;
}

// -----------------------------------------------------------------------------

template< class MemoryT >
MemoryT Buf :: bindMemory ( const MemProfile& memProfile ) const
{
    BufferImpl* pImpl = get();

    if ( ! pImpl->d_memory )
    {
        VkMemoryRequirements memoryRequirements;

        ::vkGetBufferMemoryRequirements (
            pImpl->d_hDevice.handle(), pImpl->d_handle, & memoryRequirements );

        MemoryT memory = MemoryT (
            memoryRequirements.size,
            memoryRequirements.memoryTypeBits,
            memProfile,
            pImpl->d_hDevice );

        pImpl->d_memory = memory;

        if ( memory )
            ::vkBindBufferMemory (
                pImpl->d_hDevice.handle(), pImpl->d_handle, memory.handle(), 0 );

        return memory;
    }
    else
        return MemoryT ( pImpl->d_memory );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< unsigned int USAGE >
class Buffer : public Buf
{
public:
    static const unsigned int usage = USAGE;

    VPP_INLINE Buffer()
    {
    }

    VPP_INLINE Buffer (
        VkDeviceSize size,
        const Device& hDevice,
        const std::vector< std::uint32_t >& queueFamilyIndices,
        std::uint32_t flags = 0,
        std::uint32_t addUsage = 0 ) :
            Buf ( size, USAGE | addUsage, hDevice, queueFamilyIndices, flags )
    {}

    VPP_INLINE Buffer (
        VkDeviceSize size,
        const Device& hDevice,
        std::uint32_t flags = 0,
        std::uint32_t addUsage = 0 ) :
            Buf ( size, USAGE | addUsage, hDevice, flags )
    {}

    template< unsigned int USAGE2 >
    VPP_INLINE Buffer ( const Buffer< USAGE2 >& other ) :
        Buf ( other )
    {
        static_assert ( 
            ( USAGE & USAGE2 ) == USAGE,
            "Source buffer must have all usage bits required by target buffer"
        );
    }
};

// -----------------------------------------------------------------------------

typedef Buffer< Buf::INDEX > IndexBuffer;
typedef Buffer< Buf::VERTEX > VertexBuffer;
typedef Buffer< Buf::INDIRECT > IndirectBuffer;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPBUFFER_HPP
