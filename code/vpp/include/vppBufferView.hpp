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

#ifndef INC_VPPBUFFERVIEW_HPP
#define INC_VPPBUFFERVIEW_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPBUFFER_HPP
#include "vppBuffer.hpp"
#endif

#ifndef INC_VPPFORMATS_HPP
#include "vppFormats.hpp"
#endif

#ifndef INC_VPPLANGSCALARTYPES_HPP
#include "vppLangScalarTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< class FormatT >
class TexelBufferViewImpl;

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
class TexelBufferView :
    public vpp::TSharedReference< TexelBufferViewImpl< FormatT > >
{
public:
    static const spv::Dim DIM = spv::DimBuffer;
    static const bool ARRAYED = false;
    static const bool MULTISAMPLED = false;
    static const bool UNNORM = false;
    static const bool DEPTH = false;
    typedef Int size_type;
    typedef Float coord_type;
    typedef Float bcoord_type;
    typedef Int offset_type;
    typedef Float sample_coord_type;
    typedef Float grad_type;
    typedef Int pointer_coord_type;

    TexelBufferView();

    template< class BufferT, class MemoryT >
    explicit TexelBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding );

    template< class BufferT, class MemoryT >
    TexelBufferView (
        VkDeviceSize offset, 
        VkDeviceSize size, 
        const MemoryBinding< BufferT, MemoryT >& binding );

    template< unsigned int US >
    explicit TexelBufferView (
        const TexelBufferView< FormatT, US >& otherView );

    operator VkBufferView() const;
    const Buf& buffer() const;

    static_assert (
        ( USAGE & ( Buf::UNITEX | Buf::STORTEX ) ) != 0,
        "This buffer view must have texel buffer usage enabled" );
};

// -----------------------------------------------------------------------------

template< class FormatT >
class TexelBufferViewImpl :
    public vpp::TSharedObject< TexelBufferViewImpl< FormatT > >
{
public:
    TexelBufferViewImpl (
        VkDeviceSize offset, 
        VkDeviceSize size, 
        const Buf& hBuffer );

    ~TexelBufferViewImpl();

private:
    template< class FmtT, unsigned int USAGE >
    friend class TexelBufferView;

    Buf d_hBuffer;
    VkBufferView d_handle;
};

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE TexelBufferViewImpl< FormatT > :: TexelBufferViewImpl (
    VkDeviceSize offset, 
    VkDeviceSize size, 
    const Buf& hBuffer ) :
        d_hBuffer ( hBuffer ),
        d_handle()
{
    VkBufferViewCreateInfo bufferViewCreateInfo;
    bufferViewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    bufferViewCreateInfo.pNext = 0;
    bufferViewCreateInfo.flags = 0;
    bufferViewCreateInfo.buffer = hBuffer.handle();
    bufferViewCreateInfo.format = FormatT::code;
    bufferViewCreateInfo.offset = offset;
    bufferViewCreateInfo.range = size;

    ::vkCreateBufferView ( hBuffer.device().handle(), & bufferViewCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE TexelBufferViewImpl< FormatT > :: ~TexelBufferViewImpl()
{
    ::vkDestroyBufferView ( d_hBuffer.device().handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
VPP_INLINE TexelBufferView< FormatT, USAGE > :: TexelBufferView()
{
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
template< class BufferT, class MemoryT >
VPP_INLINE TexelBufferView< FormatT, USAGE > :: TexelBufferView (
    const MemoryBinding< BufferT, MemoryT >& binding ) :
        TSharedReference< TexelBufferViewImpl< FormatT > > (
            new TexelBufferViewImpl< FormatT >(
                0, VK_WHOLE_SIZE, binding.d_resource ) )
{
    static_assert (
        USAGE == BufferT::usage,
        "Incompatible texel buffer usage for this view."
    );
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
template< class BufferT, class MemoryT >
VPP_INLINE TexelBufferView< FormatT, USAGE > :: TexelBufferView (
    VkDeviceSize offset, 
    VkDeviceSize size, 
    const MemoryBinding< BufferT, MemoryT >& binding ) :
        TSharedReference< TexelBufferViewImpl< FormatT > > (
            new TexelBufferViewImpl< FormatT > ( offset, size, binding.d_resource ) )
{
    static_assert (
        USAGE == BufferT::usage,
        "Incompatible texel buffer usage for this view."
    );
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
template< unsigned int US >
VPP_INLINE TexelBufferView< FormatT, USAGE > :: TexelBufferView (
    const TexelBufferView< FormatT, US >& otherView ) :
        TSharedReference< TexelBufferViewImpl< FormatT > > ( otherView )
{
    static_assert (
        ( ! ( USAGE & Buf::UNITEX ) ) || ( US & Buf::UNITEX ),
        "Source buffer view must have uniform texel usage enabled."
    );

    static_assert (
        ( ! ( USAGE & Buf::STORTEX ) ) || ( US & Buf::STORTEX ),
        "Source buffer view must have storage texel usage enabled."
    );
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
VPP_INLINE TexelBufferView< FormatT, USAGE > :: operator VkBufferView() const
{
    return this->get()->d_handle;
}

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
VPP_INLINE const Buf& TexelBufferView< FormatT, USAGE > :: buffer() const
{
    return this->get()->d_hBuffer;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class VertexBufferView
{
public:
    VertexBufferView();

    template< class BufferT, class MemoryT >
    VertexBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0 );

    const Buf& buffer() const;
    VkDeviceSize offset() const;

    bool operator== ( const VertexBufferView& rhs ) const;
    bool operator!= ( const VertexBufferView& rhs ) const;
    bool operator< ( const VertexBufferView& rhs ) const;

private:
    Buf d_hBuffer;
    VkDeviceSize d_offset;
};

// -----------------------------------------------------------------------------

VPP_INLINE VertexBufferView :: VertexBufferView() :
    d_offset ( 0 )
{
}

// -----------------------------------------------------------------------------

template< class BufferT, class MemoryT >
VPP_INLINE VertexBufferView :: VertexBufferView (
    const MemoryBinding< BufferT, MemoryT >& binding,
    VkDeviceSize offset ) :
        d_hBuffer ( binding.resource() ),
        d_offset ( offset )
{
    static const unsigned int usage = BufferT::usage;

    static_assert (
        ( usage & Buf::VERTEX ) != 0,
        "This buffer must have vertex usage enabled" );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Buf& VertexBufferView :: buffer() const
{
    return d_hBuffer;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize VertexBufferView :: offset() const
{
    return d_offset;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexBufferView :: operator== ( const VertexBufferView& rhs ) const
{
    return d_hBuffer == rhs.d_hBuffer && d_offset == rhs.d_offset;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexBufferView :: operator!= ( const VertexBufferView& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexBufferView :: operator< ( const VertexBufferView& rhs ) const
{
    if ( d_hBuffer != rhs.d_hBuffer )
        return d_hBuffer < rhs.d_hBuffer;
    return d_offset < rhs.d_offset;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class VertexIndexBufferView
{
public:
    VertexIndexBufferView();

    template< class BufferT, class MemoryT >
    VertexIndexBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0,
        VkIndexType indexType = VK_INDEX_TYPE_UINT32 );

    const Buf& buffer() const;
    VkDeviceSize offset() const;
    VkIndexType type() const;

    bool operator== ( const VertexIndexBufferView& rhs ) const;
    bool operator!= ( const VertexIndexBufferView& rhs ) const;
    bool operator< ( const VertexIndexBufferView& rhs ) const;

private:
    Buf d_hBuffer;
    VkDeviceSize d_offset;
    VkIndexType d_indexType;
};

// -----------------------------------------------------------------------------

VPP_INLINE VertexIndexBufferView :: VertexIndexBufferView() :
    d_offset ( 0 ),
    d_indexType ( VK_INDEX_TYPE_UINT32 )
{
}

// -----------------------------------------------------------------------------

template< class BufferT, class MemoryT >
VPP_INLINE VertexIndexBufferView :: VertexIndexBufferView (
    const MemoryBinding< BufferT, MemoryT >& binding,
    VkDeviceSize offset,
    VkIndexType indexType ) :
        d_hBuffer ( binding.resource() ),
        d_offset ( offset ),
        d_indexType ( indexType )
{
    static const unsigned int usage = BufferT::usage;

    static_assert (
        ( usage & Buf::INDEX ) != 0,
        "This buffer must have index usage enabled" );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Buf& VertexIndexBufferView :: buffer() const
{
    return d_hBuffer;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize VertexIndexBufferView :: offset() const
{
    return d_offset;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkIndexType VertexIndexBufferView :: type() const
{
    return d_indexType;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexIndexBufferView :: operator== ( const VertexIndexBufferView& rhs ) const
{
    return
        d_hBuffer == rhs.d_hBuffer
        && d_offset == rhs.d_offset
        && d_indexType == rhs.d_indexType;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexIndexBufferView :: operator!= ( const VertexIndexBufferView& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool VertexIndexBufferView :: operator< ( const VertexIndexBufferView& rhs ) const
{
    if ( d_hBuffer != rhs.d_hBuffer )
        return d_hBuffer < rhs.d_hBuffer;
    if ( d_offset != rhs.d_offset )
        return d_offset < rhs.d_offset;
    return d_indexType < rhs.d_indexType;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class UniformBufferView
{
public:
    UniformBufferView();

    template< class BufferT, class MemoryT >
    UniformBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding,
        VkDeviceSize offset = 0,
        VkDeviceSize length = 0 );

    const Buf& buffer() const;
    VkDeviceSize offset() const;
    VkDeviceSize size() const;

    bool operator== ( const UniformBufferView& rhs ) const;
    bool operator!= ( const UniformBufferView& rhs ) const;
    bool operator< ( const UniformBufferView& rhs ) const;

private:
    Buf d_hBuffer;
    VkDeviceSize d_offset;
    VkDeviceSize d_size;
};

// -----------------------------------------------------------------------------

VPP_INLINE UniformBufferView :: UniformBufferView() :
    d_offset ( 0 ),
    d_size ( 0 )
{
}

// -----------------------------------------------------------------------------

template< class BufferT, class MemoryT >
VPP_INLINE UniformBufferView :: UniformBufferView (
    const MemoryBinding< BufferT, MemoryT >& binding,
    VkDeviceSize offset,
    VkDeviceSize length ) :
        d_hBuffer ( binding.resource() ),
        d_offset ( offset ),
        d_size ( length ? length : d_hBuffer.size() )
{
    static const unsigned int usage = BufferT::usage;

    static_assert (
        ( ( usage & ( Buf::UNIFORM | Buf::STORAGE ) ) != 0 ),
        "This buffer must have uniform or storage usage enabled"
    );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Buf& UniformBufferView :: buffer() const
{
    return d_hBuffer;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize UniformBufferView :: offset() const
{
    return d_offset;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDeviceSize UniformBufferView :: size() const
{
    return d_size;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool UniformBufferView :: operator== ( const UniformBufferView& rhs ) const
{
    return
        d_hBuffer == rhs.d_hBuffer
        && d_offset == rhs.d_offset
        && d_size == rhs.d_size;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool UniformBufferView :: operator!= ( const UniformBufferView& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool UniformBufferView :: operator< ( const UniformBufferView& rhs ) const
{
    if ( d_hBuffer != rhs.d_hBuffer )
        return d_hBuffer < rhs.d_hBuffer;
    if ( d_offset != rhs.d_offset )
        return d_offset < rhs.d_offset;
    return d_size < rhs.d_size;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class IndirectBufferView
{
public:
    IndirectBufferView();

    template< class BufferT, class MemoryT >
    IndirectBufferView (
        const MemoryBinding< BufferT, MemoryT >& binding );

    const Buf& buffer() const;

    bool operator== ( const IndirectBufferView& rhs ) const;
    bool operator!= ( const IndirectBufferView& rhs ) const;
    bool operator< ( const IndirectBufferView& rhs ) const;

private:
    Buf d_hBuffer;
};

// -----------------------------------------------------------------------------

VPP_INLINE IndirectBufferView :: IndirectBufferView()
{
}

// -----------------------------------------------------------------------------

template< class BufferT, class MemoryT >
VPP_INLINE IndirectBufferView :: IndirectBufferView (
    const MemoryBinding< BufferT, MemoryT >& binding ) :
        d_hBuffer ( binding.resource() )
{
    static const unsigned int usage = BufferT::usage;

    static_assert (
        ( usage & Buf::INDIRECT ) != 0,
        "This buffer must have indirect usage enabled"
    );
}

// -----------------------------------------------------------------------------

VPP_INLINE const Buf& IndirectBufferView :: buffer() const
{
    return d_hBuffer;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool IndirectBufferView :: operator== ( const IndirectBufferView& rhs ) const
{
    return d_hBuffer == rhs.d_hBuffer;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool IndirectBufferView :: operator!= ( const IndirectBufferView& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool IndirectBufferView :: operator< ( const IndirectBufferView& rhs ) const
{
    return d_hBuffer < rhs.d_hBuffer;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPBUFFERVIEW_HPP
