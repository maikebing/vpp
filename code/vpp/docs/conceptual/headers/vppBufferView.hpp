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
    \brief Buffer view for texel buffers.

    A buffer view is auxiliary object that acts as an interface between
    the buffer and the binding point in the pipeline. In case of texel
    buffers, the view is actual Vulkan object and has two functions:
    - providing information about the format of the data inside the buffer,
    - optionally, specifying a buffer slice.

    You must hold the TexelBufferView object along with your buffer. This
    is similar to image buffer views.

    As the first template argument, use one of VPP format classes. The second
    argument is bitwise OR of one or both of the following values:
    - Buf::UNITEX - for a view to be used with a uniform texel buffer,
    - Buf::STORTEX - for a view to be used with a storage texel buffer.

    This object is reference-counted and may be passed by value.
*/

template< class FormatT, unsigned int USAGE >
class TexelBufferView
{
public:
    /** \brief Constructs null reference. */
    TexelBufferView();

    /** \brief Constructs view object for entire buffer (bound to memory). */
    template< class BufferT, class MemoryT >
    explicit TexelBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding );

    /** \brief Constructs view object for a slice of given buffer (bound to memory). */
    template< class BufferT, class MemoryT >
    TexelBufferView (
        VkDeviceSize offset, 
        VkDeviceSize size, 
        const MemoryBinding< BufferT, MemoryT >& binding );

    /** \brief Converts texel view to different view with compatible usage. */
    template< unsigned int US >
    explicit TexelBufferView (
        const TexelBufferView< FormatT, US >& otherView );

    /** \brief Retrieves Vulkan handle of the view. */
    operator VkBufferView() const;

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Buffer view for vertex buffers.

    A buffer view is auxiliary object that acts as an interface between
    the buffer and the binding point in the pipeline. In case of vertex
    buffers, the view is constructed on VPP level only, there is no
    corresponding Vulkan object.

    The view can also define a slice of the buffer. In such case you specify
    only start index, as the size is determined by the draw command.

    VertexBufferView may be deleted after binding.

    This object is reference-counted and may be passed by value.
*/

class VertexBufferView
{
public:
    /** \brief Constructs null reference. */
    VertexBufferView();

    /** \brief Constructs view object for provided buffer (bound to memory). */
    template< class BufferT, class MemoryT >
    VertexBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0 );

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;

    /** \brief Retrieves the starting offset of the slice. */
    VkDeviceSize offset() const;
};

// -----------------------------------------------------------------------------

class VertexIndexBufferView
{
public:
    /** \brief Constructs null reference. */
    VertexIndexBufferView();

    template< class BufferT, class MemoryT >
    VertexIndexBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0,
        VkIndexType indexType = VK_INDEX_TYPE_UINT32 );

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;

    VkDeviceSize offset() const;
    VkIndexType type() const;
};

// -----------------------------------------------------------------------------

class UniformBufferView
{
public:
    /** \brief Constructs null reference. */
    UniformBufferView();

    template< class BufferT, class MemoryT >
    UniformBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0,
        VkDeviceSize length = 0 );

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;

    VkDeviceSize offset() const;
    VkDeviceSize size() const;
};

// -----------------------------------------------------------------------------

class StorageBufferView
{
public:
    /** \brief Constructs null reference. */
    StorageBufferView();

    template< class BufferT, class MemoryT >
    StorageBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0,
        VkDeviceSize length = 0 );

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;

    VkDeviceSize offset() const;
    VkDeviceSize size() const;
};

// -----------------------------------------------------------------------------

class IndirectBufferView
{
public:
    /** \brief Constructs null reference. */
    IndirectBufferView();

    template< class BufferT, class MemoryT >
    IndirectBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding );

    /** \brief Retrieves a reference to the buffer. */
    const Buf& buffer() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
