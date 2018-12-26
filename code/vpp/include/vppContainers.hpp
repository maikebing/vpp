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

#ifndef INC_VPPCONTAINERS_HPP
#define INC_VPPCONTAINERS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPBUFFER_HPP
#include "vppBuffer.hpp"
#endif

#ifndef INC_VPPQUEUE_HPP
#include "vppQueue.hpp"
#endif

#ifndef INC_VPPCOMMANDPOOL_HPP
#include "vppCommandPool.hpp"
#endif

#ifndef INC_VPPEXCEPTIONS_HPP
#include "vppExceptions.hpp"
#endif

#ifndef INC_VPPUSAGECHECKS_HPP
#include "vppUsageChecks.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class VectorBase : public KExceptionThrower
{
public:
    // Synchronizes entire buffer from host to device. Submits a command
    // to specified queue. Does not wait for completion, uses specified
    // semaphores and fence.

    VPP_DLLAPI void commit (
        EQueueType eQueue = Q_GRAPHICS,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore() );

    // Synchronizes entire buffer from host to device and waits for completion.

    VPP_DLLAPI void commitAndWait (
        EQueueType eQueue = Q_GRAPHICS );

    // Synchronizes entire buffer from device to host. Submits a command
    // to specified queue. Does not wait for completion, uses specified
    // semaphores and fence.

    VPP_DLLAPI void load (
        EQueueType eQueue = Q_GRAPHICS,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore() );

    // Synchronizes entire buffer from device to host and waits for completion.

    VPP_DLLAPI void loadAndWait (
        EQueueType eQueue = Q_GRAPHICS );

    // Generates a command to copy the buffer contents to specified image.

    VPP_DLLAPI void cmdCopyToImage (
        CommandBuffer hCmdBuffer,
        const TDImg& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    void cmdCopyToImage (
        const TDImg& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    // Submits a command to copy the buffer contents to specified image.

    VPP_DLLAPI void copyToImage (
        EQueueType eQueue,
        const TDImg& img,
        VkImageLayout targetLayout,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    // Submits a command to copy the buffer contents to specified image,
    // waits for completion.

    VPP_DLLAPI void copyToImageAndWait (
        EQueueType eQueue,
        const TDImg& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    // Generates a command to copy the buffer contents from specified image.

    VPP_DLLAPI void cmdCopyFromImage (
        CommandBuffer hCmdBuffer,
        const TSImg& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    void cmdCopyFromImage (
        const TSImg& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    // Submits a command to copy the buffer contents from specified image.

    VPP_DLLAPI void copyFromImage (
        EQueueType eQueue,
        const TSImg& img,
        VkImageLayout sourceImageLayout,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore(),
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    // Submits a command to copy the buffer contents from specified image.
    // Waits for completion.

    VPP_DLLAPI void copyFromImageAndWait (
        EQueueType eQueue,
        const TSImg& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

protected:
    VPP_DLLAPI VectorBase (
        size_t bufferSize,
        std::uint32_t usage,
        MemProfile::ECharacteristic memProfile,
        Buf* pBuffer,
        DeviceMemory* pMemory,
        const Device& hDevice );

    VPP_DLLAPI void flushHostToDevice (
        VkCommandBuffer hCmdBuffer, const DeviceMemory& mem );

    VPP_DLLAPI void flushDeviceToHost (
        VkCommandBuffer hCmdBuffer, const DeviceMemory& mem );

    VPP_DLLAPI void syncHostToDevice (
        VkCommandBuffer hCmdBuffer, VkDeviceSize offset, VkDeviceSize size ); 

    VPP_DLLAPI void syncDeviceToHost (
        VkCommandBuffer hCmdBuffer, VkDeviceSize offset, VkDeviceSize size );

    VPP_DLLAPI static std::uint32_t getAdditionalUsage (
        MemProfile::ECharacteristic memProfile );

    static size_t fixCapacity ( size_t nMaxItemCount );

protected:
    size_t d_memorySize;
    MemProfile::ECharacteristic d_memProfile;

    Buf* d_pBuffer;
    DeviceMemory* d_pMemory;
    Device d_device;

    typedef MemoryBinding< Buf, MappableDeviceMemory > LocalMemoryBinding;
    Buf d_localBuffer;
    LocalMemoryBinding d_localMemoryBinding;

    CommandBuffer d_commitCmdBuffer [ Q_count ];
    CommandBuffer d_loadCmdBuffer [ Q_count ];
    CommandBuffer d_copyCmdBuffer [ Q_count ];
};

// -----------------------------------------------------------------------------

VPP_INLINE void VectorBase :: cmdCopyToImage (
    const TDImg& img,
    VkImageLayout targetLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    const VkCommandBuffer hCmdBuffer =
        RenderingCommandContext::getCommandBufferHandle();

    cmdCopyToImage (
        hCmdBuffer,
        img, targetLayout, mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );
}

// -----------------------------------------------------------------------------

VPP_INLINE void VectorBase :: cmdCopyFromImage (
    const TSImg& img,
    VkImageLayout sourceImageLayout,
    std::uint32_t mipLevel,
    std::uint32_t layer,
    const VkOffset3D& imageOffset,
    const VkExtent3D& imageExtent,
    VkDeviceSize bufferOffset,
    std::uint32_t bufferRowLength,
    std::uint32_t bufferImageHeight )
{
    const VkCommandBuffer hCmdBuffer =
        RenderingCommandContext::getCommandBufferHandle();

    cmdCopyFromImage (
        hCmdBuffer,
        img, sourceImageLayout, mipLevel, layer, imageOffset, imageExtent,
        bufferOffset, bufferRowLength, bufferImageHeight );
}

// -----------------------------------------------------------------------------

VPP_INLINE size_t VectorBase :: fixCapacity ( size_t nMaxItemCount )
{
    return nMaxItemCount > 0 ? nMaxItemCount : 1;
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
class gvector :
    public Buffer< USAGE >,
    public MemoryBinding< Buffer< USAGE >, DeviceMemory >,
    public detail::VectorBase
{
public:
    // Types.

    typedef ItemT* iterator;
    typedef const ItemT* const_iterator;

    // Constructor.
    // THe vector has fixed capacity, but varying number of valid elements (size).

    gvector (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        const Device& hDevice );

    // Standard container operations.

    VPP_INLINE iterator begin() { return d_pBegin; }
    VPP_INLINE iterator end() { return d_pBegin + d_size; }

    VPP_INLINE const_iterator cbegin() const { return d_pBegin; }
    VPP_INLINE const_iterator cend() const { return d_pBegin + d_size; }

    VPP_INLINE bool empty() const { return d_size == 0; }
    VPP_INLINE size_t size() const { return d_size; }
    VPP_INLINE size_t capacity() const { return d_capacity; }

    VPP_INLINE void push_back ( const ItemT& item )
    {
        if ( d_size == d_capacity )
            raiseVectorOverflow();

        new ( d_pBegin + d_size++ ) ItemT ( item );
    }

    template< typename ... ArgsT >
    VPP_INLINE void emplace_back ( ArgsT... args )
    {
        if ( d_size == d_capacity )
            raiseVectorOverflow();

        new ( d_pBegin + d_size++ ) ItemT ( args... );
    }

    // Allocates space for new item without constructing it.

    VPP_INLINE ItemT* allocate_back()
    {
        if ( d_size == d_capacity )
            raiseVectorOverflow();

        return reinterpret_cast< ItemT* >( d_pBegin + d_size++ );
    }

    // Resizes the vector in proper way (constructing/destructing elements).

    VPP_INLINE void resize ( size_t newSize, const ItemT& value = ItemT() )
    {
        if ( newSize > d_capacity )
            raiseVectorOverflow();

        if ( newSize < d_size )
        {
            for ( size_t i = d_size - newSize; i != d_size; ++i )
                d_pBegin [ i ].~ItemT();
            d_size = newSize;
        }
        else if ( newSize > d_size )
        {
            for ( size_t i = d_size; i != newSize; ++i )
                new ( d_pBegin + i ) ItemT ( value );
            d_size = newSize;
        }
    }

    // Resizes the vector in dumb way (just setting the size without initialization).
    // Use for numeric or vector types only.

    VPP_INLINE void setSize ( size_t newSize )
    {
        if ( newSize > d_capacity )
            raiseVectorOverflow();

        d_size = newSize;
    }

    VPP_INLINE void clear()
    {
        resize ( 0 );
    }

    VPP_INLINE ItemT& operator[] ( size_t index )
    {
        return d_pBegin [ index ];
    }

    VPP_INLINE const ItemT& operator[] ( size_t index ) const
    {
        return d_pBegin [ index ];
    }

public:
    // Transfer from host to device. All make sense only for DEVICE_STATIC
    // memory profile, which requires manual synchronization.

    // Generates a command ensuring that valid elements have been synchronized
    // from host to device. Optionally can be restricted to a range.

    VPP_INLINE void cmdCommit (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        if ( firstItem + nItems > d_size )
            nItems = d_size - firstItem;

        const VkDeviceSize offset = 
            static_cast< VkDeviceSize >( firstItem * sizeof ( ItemT ) );

        const VkDeviceSize length = 
            static_cast< VkDeviceSize >( nItems * sizeof ( ItemT ) );

        syncHostToDevice ( cmdBuffer.handle(), offset, length );
    }

    // Generates a command ensuring that entire memory area has been synchronized
    // from host to device. Optionally can be restricted to a range.

    VPP_INLINE void cmdCommitAll (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        if ( firstItem + nItems > d_capacity )
            nItems = d_capacity - firstItem;

        const VkDeviceSize offset = 
            static_cast< VkDeviceSize >( firstItem * sizeof ( ItemT ) );

        const VkDeviceSize length = 
            static_cast< VkDeviceSize >( nItems * sizeof ( ItemT ) );

        syncHostToDevice ( cmdBuffer.handle(), offset, length );
    }

    // Generates a command (to implicit context) which ensures valid elements 
    // or entire buffer area has been synchronized from host to device.

    VPP_INLINE void cmdCommit (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        const VkCommandBuffer hCmdBuffer =
            RenderingCommandContext::getCommandBufferHandle();

        cmdCommit ( hCmdBuffer, firstItem, nItems );
    }

    VPP_INLINE void cmdCommitAll (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        const VkCommandBuffer hCmdBuffer =
            RenderingCommandContext::getCommandBufferHandle();

        cmdCommitAll ( hCmdBuffer, firstItem, nItems );
    }

    // Transfer from device to host.

    // Generates a command ensuring that valid elements have been synchronized
    // from device to host. Optionally can be restricted to a range.

    VPP_INLINE void cmdLoad (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        if ( firstItem + nItems > d_size )
            nItems = d_size - firstItem;

        const VkDeviceSize offset = 
            static_cast< VkDeviceSize >( firstItem * sizeof ( ItemT ) );

        const VkDeviceSize length = 
            static_cast< VkDeviceSize >( nItems * sizeof ( ItemT ) );

        syncDeviceToHost ( cmdBuffer.handle(), offset, length );
    }

    VPP_INLINE void cmdLoadAll (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        if ( firstItem + nItems > d_capacity )
            nItems = d_capacity - firstItem;

        const VkDeviceSize offset = 
            static_cast< VkDeviceSize >( firstItem * sizeof ( ItemT ) );

        const VkDeviceSize length = 
            static_cast< VkDeviceSize >( nItems * sizeof ( ItemT ) );

        syncDeviceToHost ( cmdBuffer.handle(), offset, length );
    }

    // Generates a command (to implicit context) which ensures valid elements
    // have been synchronized from device to host.

    VPP_INLINE void cmdLoad (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        const VkCommandBuffer hCmdBuffer =
            RenderingCommandContext::getCommandBufferHandle();

        cmdLoad ( hCmdBuffer, firstItem, nItems );
    }

    VPP_INLINE void cmdLoadAll (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() )
    {
        const VkCommandBuffer hCmdBuffer =
            RenderingCommandContext::getCommandBufferHandle();

        cmdLoadAll ( hCmdBuffer, firstItem, nItems );
    }

private:
    // Internal routines.

    void map();
    void unmap();

private:
    ItemT* d_pBegin;
    ItemT* d_pEnd;
    size_t d_size;
    size_t d_capacity;
};

// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
gvector< ItemT, USAGE > :: gvector (
    size_t maxItemCount,
    MemProfile::ECharacteristic memProfile,
    const Device& hDevice ) :
        Buffer< USAGE >(
            fixCapacity ( maxItemCount ) * sizeof ( ItemT ),
            hDevice, 0, getAdditionalUsage ( memProfile ) ),
        MemoryBinding< Buffer< USAGE >, DeviceMemory >(
            static_cast< const Buffer< USAGE >& >( *this ),
            MemProfile ( memProfile )
        ),
        detail::VectorBase (
            fixCapacity ( maxItemCount ) * sizeof ( ItemT ), USAGE, memProfile,
            static_cast< Buf* >( this ), & this->memory(), hDevice ),
        d_pBegin ( 0 ),
        d_pEnd ( 0 ),
        d_size ( 0 ),
        d_capacity ( fixCapacity ( maxItemCount ) )
{
    map();
}

// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
void gvector< ItemT, USAGE > :: map()
{
    if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        d_localMemoryBinding.memory().map();

        d_pBegin = reinterpret_cast< ItemT* >(
            d_localMemoryBinding.memory().beginMapped() );

        d_pEnd = reinterpret_cast< ItemT* >(
            d_localMemoryBinding.memory().endMapped() );
    }
    else
    {
        VPP_EXTSYNC_MTX_LOCK ( this->memory().get() );

        VkResult result = ::vkMapMemory (
            this->device().handle(), this->memory().handle(), 0, d_capacity * sizeof ( ItemT ),
            0, reinterpret_cast< void** > ( & d_pBegin ) );

        VPP_EXTSYNC_MTX_UNLOCK ( this->memory().get() );

        if ( result == VK_SUCCESS )
            d_pEnd = d_pBegin + d_capacity;
        else
            raiseMemoryAllocationError();
    }
}

// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
void gvector< ItemT, USAGE > :: unmap()
{
    if ( d_memProfile == MemProfile::DEVICE_STATIC )
    {
        d_localMemoryBinding.memory().unmap();
    }
    else
    {
        VPP_EXTSYNC_MTX_LOCK ( this->memory().get() );
        ::vkUnmapMemory ( this->device().handle(), this->memory().handle() );
        VPP_EXTSYNC_MTX_UNLOCK ( this->memory().get() );

        d_pEnd = d_pBegin = 0;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class IndirectCommands :
    public gvector< VkDrawIndirectCommand, Buf::INDIRECT >
{
public:
    IndirectCommands (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------

VPP_INLINE IndirectCommands :: IndirectCommands (
    size_t maxItemCount,
    MemProfile::ECharacteristic memProfile,
    Device hDevice ) :
        gvector< VkDrawIndirectCommand, Buf::INDIRECT >(
            maxItemCount,
            memProfile,
            hDevice
        )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class IndexedIndirectCommands :
    public gvector< VkDrawIndexedIndirectCommand, Buf::INDIRECT >
{
public:
    IndexedIndirectCommands (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------

VPP_INLINE IndexedIndirectCommands :: IndexedIndirectCommands (
    size_t maxItemCount,
    MemProfile::ECharacteristic memProfile,
    Device hDevice ) :
        gvector< VkDrawIndexedIndirectCommand, Buf::INDIRECT >(
            maxItemCount,
            memProfile,
            hDevice
        )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class DispatchIndirectCommands :
    public gvector< VkDispatchIndirectCommand, Buf::INDIRECT >
{
public:
    DispatchIndirectCommands (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------

VPP_INLINE DispatchIndirectCommands :: DispatchIndirectCommands (
    size_t maxItemCount,
    MemProfile::ECharacteristic memProfile,
    Device hDevice ) :
        gvector< VkDispatchIndirectCommand, Buf::INDIRECT >(
            maxItemCount,
            memProfile,
            hDevice
        )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Indices :
    public gvector< std::uint32_t, Buf::INDEX >
{
public:
    Indices (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        Device hDevice );
};

// -----------------------------------------------------------------------------

VPP_INLINE Indices :: Indices (
    size_t maxItemCount,
    MemProfile::ECharacteristic memProfile,
    Device hDevice ) :
        gvector< std::uint32_t, Buf::INDEX >(
            maxItemCount,
            memProfile,
            hDevice
        )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
class dgvector :
    public Buffer< USAGE >,
    public MemoryBinding< Buffer< USAGE >, DeviceMemory >,
    public detail::VectorBase
{
public:
    dgvector ( size_t maxItemCount, const Device& hDevice );
};

// -----------------------------------------------------------------------------

template< typename ItemT, unsigned int USAGE >
dgvector< ItemT, USAGE > :: dgvector ( size_t maxItemCount, const Device& hDevice ) :
    Buffer< USAGE >(
        maxItemCount * sizeof ( ItemT ),
        hDevice, 0, getAdditionalUsage ( MemProfile::DEVICE_ONLY ) ),
    MemoryBinding< Buffer< USAGE >, DeviceMemory >(
        static_cast< const Buffer< USAGE >& >( *this ),
        MemProfile::DEVICE_ONLY
    ),
    detail::VectorBase (
        maxItemCount * sizeof ( ItemT ), USAGE,
        MemProfile::DEVICE_ONLY,
        static_cast< Buf* >( this ), & this->memory(), hDevice )
{
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCONTAINERS_HPP
