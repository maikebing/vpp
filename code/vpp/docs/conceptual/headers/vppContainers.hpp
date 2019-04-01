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

/** \brief Generic STL-style vector allocating memory on the GPU.

    \ref gvector is general purpose container for GPU data. Depending on the
    USAGE template parameter, it can be used for various data buffers used
    in graphics and compute shaders. Single \ref gvector instance may have multiple
    uses, hence the parameter is bitwise OR of following values:

    - Buf::VERTEX - vertex attributes array.
    - Buf::INDIRECT - buffer holding indirect draw ranges.
    - Buf::INDEX - vertex index array for indexed primitives.
    - Buf::UNIFORM - read-only common data (uniform buffer).
    - Buf::STORAGE - read & write array (storage buffer).
    - Buf::UNITEX - read-only formatted data (uniform texel buffer).
    - Buf::STORTEX - read & write formatted data (storage texel buffer).
    - Buf::SOURCE - source buffer for transfer operations.
    - Buf::TARGET - target buffer for transfer operations.

    The vector usually allocates memory on GPU side, but it is also accessible
    on CPU side in a way depending on the value of memProfile parameter
    of the constructor. This parameter can have the following values:

    - MemProfile::DEVICE_STATIC - most frequently used, also known as staging buffer
      mode. There are two physical memory blocks allocated: one on the device
      side and another one on host size. The data is synchronized on demand.
      The access to the memory on both sides is fastest among available modes,
      but on-demand synchronization takes additional time.

    - MemProfile::DEVICE_DYNAMIC - single memory block shared between a device and host.
      Prefferably allocated on device side, or on host side as a fallback.
      Automatically synchronized by hardware. Efficient on architectures where
      CPU and GPU memory is common.

    - MemProfile::DEVICE_FEEDBACK - similar to DEVICE_DYNAMIC, but always allocated on host
      and visible to the device.

    - MemProfile::HOST_STATIC - like DEVICE_FEEDBACK, allocated on host, may be also cached
      on host. Used internally by DEVICE_STATIC for host-side part of the buffer.

    Typically just use DEVICE_STATIC.

    Object of this class is reference-counted and may be passed by value. Generally,
    functions accepting a buffer, bound buffer and certain kinds of views, do accept
    a gvector instance as well. Therefore you can fill a \ref gvector with e.g. vertex,
    index, indirect or uniform data and bind it directly. Texel buffers require
    creating a view explicitly but this view also accepts gvector as data source.
*/

template< typename ItemT, unsigned int USAGE >
class gvector :
    public Buffer< USAGE >,
    public MemoryBinding< Buffer< USAGE >, DeviceMemory >
{
public:
    /** \brief Random access iterator. */
    typedef ItemT* iterator;

    /** \brief Random access const iterator. */
    typedef const ItemT* const_iterator;

    /** \brief Constructor.
    
        The vector has fixed capacity, but varying number of valid elements (size).
    */
    gvector (
        size_t maxItemCount,
        MemProfile::ECharacteristic memProfile,
        const Device& hDevice );

    /** \brief Iterator to begin of the vector. */
    iterator begin() { return d_pBegin; }

    /** \brief Iterator to the end of the valid range (but not whole area). */
    iterator end() { return d_pBegin + d_size; }

    /** \brief Const iterator to begin of the vector. */
    const_iterator cbegin() const { return d_pBegin; }

    /** \brief Const iterator to the end of the valid range (but not whole area). */
    const_iterator cend() const { return d_pBegin + d_size; }

    /** \brief Checks whether the valid range is empty. */
    bool empty() const { return d_size == 0; }

    /** \brief Returns the size of the valid range. */
    size_t size() const { return d_size; }

    /** \brief Returns the size of the whole allocated area (maximum number of elements). */
    size_t capacity() const { return d_capacity; }

    /** \brief Adds element to the end of the valid range. */
    void push_back ( const ItemT& item );

    /** \brief Constructs element in place at the end of the valid range. */
    template< typename ... ArgsT >
    void emplace_back ( ArgsT... args );

    /** \brief Allocates space for new item without constructing it. */
    ItemT* allocate_back();

    /** \brief Resizes the vector in proper way (constructing/destructing elements).
    
        This does not allocate/deallocate any memory nor invalidate iterators.
        The capacity of the vector is static. Resizing it only affects the number
        of valid items inside the vector.
    */
    void resize ( size_t newSize, const ItemT& value = ItemT() );

    /** \brief Resizes the vector in dumb way (just setting the size without initialization).
        Use for numeric or vector types only.
    */
    void setSize ( size_t newSize );

    /** \brief Empties the valid range. */
    void clear();

    /** \brief Access to indexed element. */
    ItemT& operator[] ( size_t index )

    /** \brief Access to const indexed element. */
    const ItemT& operator[] ( size_t index ) const;

    /** \brief Generates a command ensuring that valid elements have been synchronized
        from host to device. Optionally can be restricted to a range.
    */
    void cmdCommit (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Generates a command ensuring that entire memory area has been
        synchronized from host to device. Optionally can be restricted to a range.
    */
    void cmdCommitAll (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Generates a command (to implicit context) which ensures valid elements 
        has been synchronized from host to device.
    */
    void cmdCommit (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Generates a command (to implicit context) which ensures that
        entire buffer area has been synchronized from host to device.
    */
    void cmdCommitAll (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Generates a command ensuring that valid elements have been synchronized
        from device to host. Optionally can be restricted to a range.

        Caution: the vector must explicitly list Buf::SOURCE flag in order to be able
        to use this function.
    */
    void cmdLoad (
        CommandBuffer cmdBuffer,
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Generates a command (to implicit context) which ensures valid elements
        have been synchronized from device to host.

        Caution: the vector must explicitly list Buf::SOURCE flag in order to be able
        to use this function.
    */
    void cmdLoad (
        size_t firstItem = 0,
        size_t nItems = std::numeric_limits< size_t >::max() );

    /** \brief Synchronizes entire buffer from host to device.
    
        Submits a command to specified queue. Does not wait for completion,
        uses specified semaphores and fence.
    */
    void commit (
        EQueueType eQueue = Q_GRAPHICS,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore() );

    /** \brief Synchronizes entire buffer from host to device and waits for completion.
    */
    void commitAndWait (
        EQueueType eQueue = Q_GRAPHICS );

    /** \brief Synchronizes entire buffer from device to host.
    
        Submits a command to specified queue. Does not wait for completion,
        uses specified semaphores and fence.

        Caution: the vector must explicitly list Buf::SOURCE flag in order to be able
        to use this function.
    */
    void load (
        EQueueType eQueue = Q_GRAPHICS,
        const Fence& signalFenceOnEnd = Fence(),
        const Semaphore& waitOnBegin = Semaphore(),
        const Semaphore& signalOnEnd = Semaphore() );

    /** \brief Synchronizes entire buffer from device to host and waits for completion.
    
        Caution: the vector must explicitly list Buf::SOURCE flag in order to be able
        to use this function.
    */
    void loadAndWait (
        EQueueType eQueue = Q_GRAPHICS );

    /** \brief Generates a command to copy the buffer contents to specified image.
    
        Caution: may generate other auxiliary commands as well. The vector must explicitly
        list Buf::SOURCE flag in order to be able to use this function.
    */
    void cmdCopyToImage (
        CommandBuffer hCmdBuffer,
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command (to the default context) to copy
        the buffer contents to specified image.

        Caution: may generate other auxiliary commands as well. The vector must explicitly
        list Buf::SOURCE flag in order to be able to use this function.
    */
    void cmdCopyToImage (
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Submits a command to copy the buffer contents to specified image.
    
        Caution: may generate other auxiliary commands as well. The vector must explicitly
        list Buf::SOURCE flag in order to be able to use this function.
    */
    void copyToImage (
        EQueueType eQueue,
        const Img& img,
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

    /** \brief Submits a command to copy the buffer contents to specified image,
        waits for completion.

        Caution: may generate other auxiliary commands as well. The vector must explicitly
        list Buf::SOURCE flag in order to be able to use this function.
    */
    void copyToImageAndWait (
        EQueueType eQueue,
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command to copy the buffer contents from specified image.
    
        Caution: may generate other auxiliary commands as well. Changes the layout of
        the image from \c sourceImageLayout to \c VK_IMAGE_LAYOUT_GENERAL.
    */
    void cmdCopyFromImage (
        CommandBuffer hCmdBuffer,
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command (to the default context) to copy the buffer
         contents from specified image.
         
        Caution: may generate other auxiliary commands as well. Changes the layout of
        the image from \c sourceImageLayout to \c VK_IMAGE_LAYOUT_GENERAL.
    */
    void cmdCopyFromImage (
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Submits a command to copy the buffer contents from specified image.
    
        Caution: may generate other auxiliary commands as well. Changes the layout of
        the image from \c sourceImageLayout to \c VK_IMAGE_LAYOUT_GENERAL.
    */
    void copyFromImage (
        EQueueType eQueue,
        const Img& img,
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

    /** \brief Submits a command to copy the buffer contents from specified image.
        Waits for completion.

        Caution: may generate other auxiliary commands as well. Changes the layout of
        the image from \c sourceImageLayout to \c VK_IMAGE_LAYOUT_GENERAL.
    */
    void copyFromImageAndWait (
        EQueueType eQueue,
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );
};

// -----------------------------------------------------------------------------
/**
    \brief Utility subclass representing a vector of indirect draw ranges.

    This object is reference-counted and may be passed by value.
*/

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
/**
    \brief Utility subclass representing a vector of indexed indirect draw ranges.

    This object is reference-counted and may be passed by value.
*/

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
/**
    \brief Utility subclass representing a vector of indirect dispatch ranges.

    This object is reference-counted and may be passed by value.
*/

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
/**
    \brief Utility subclass representing a vector of indices for indexed draws.

    This object is reference-counted and may be passed by value.
*/

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

/**
    \brief Array allocated entirely on GPU side, without any mapping to the CPU side.

    Use this class for buffers of data which are generated and consumed entirely
    on GPU side, without a need to transfer to/from CPU. Such transfers
    are still possible but require explicit transfer command to be issued. This
    vector class does not offer a staging buffer functionality, therefore has
    less memory overhead than gvector (no buffer copy on CPU side).
    
    Use for best performance during multiple-staged GPU computations.
*/

template< typename ItemT, unsigned int USAGE >
class dgvector :
    public Buffer< USAGE >,
    public MemoryBinding< Buffer< USAGE >, DeviceMemory >
{
public:
    dgvector ( size_t maxItemCount, const Device& hDevice );

    /** \brief Generates a command to copy the buffer contents to specified image. */
    void cmdCopyToImage (
        CommandBuffer hCmdBuffer,
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command (to the implicit context) to copy
        the buffer contents to specified image. */
    void cmdCopyToImage (
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Submits a command to copy the buffer contents to specified image. */
    void copyToImage (
        EQueueType eQueue,
        const Img& img,
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

    /** \brief Submits a command to copy the buffer contents to specified image,
        waits for completion.
    */
    void copyToImageAndWait (
        EQueueType eQueue,
        const Img& img,
        VkImageLayout targetLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command to copy the buffer contents from specified image. */
    void cmdCopyFromImage (
        CommandBuffer hCmdBuffer,
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Generates a command (to the implicit context) to copy the buffer
         contents from specified image. */
    void cmdCopyFromImage (
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );

    /** \brief Submits a command to copy the buffer contents from specified image. */
    void copyFromImage (
        EQueueType eQueue,
        const Img& img,
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

    /** \brief Submits a command to copy the buffer contents from specified image.
        Waits for completion.
    */
    void copyFromImageAndWait (
        EQueueType eQueue,
        const Img& img,
        VkImageLayout sourceImageLayout,
        std::uint32_t mipLevel = 0,
        std::uint32_t layer = 0,
        const VkOffset3D& imageOffset = VkOffset3D { 0, 0, 0 },
        const VkExtent3D& imageExtent = VkExtent3D { 0, 0, 0 },
        VkDeviceSize bufferOffset = 0,
        std::uint32_t bufferRowLength = 0,
        std::uint32_t bufferImageHeight = 0 );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
