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

#ifndef INC_VPPBARRIERS_HPP
#define INC_VPPBARRIERS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class Barriers
{
public:
    Barriers();

    void setBarriers ( const std::vector< VkMemoryBarrier >& bar );
    void setBarriers ( const std::vector< VkBufferMemoryBarrier >& bar );
    void setBarriers ( const std::vector< VkImageMemoryBarrier >& bar );

    void setBarriers ( const VkMemoryBarrier& bar );
    void setBarriers ( const VkBufferMemoryBarrier& bar );
    void setBarriers ( const VkImageMemoryBarrier bar );

    template< typename BarriersA >
    Barriers ( const BarriersA& ba );

    template< typename BarriersA, typename BarriersB >
    Barriers ( const BarriersA& ba, const BarriersB& bb );

    template< typename BarriersA, typename BarriersB, typename BarriersC >
    Barriers ( const BarriersA& ba, const BarriersB& bb, const BarriersC& bc );

public:
    std::uint32_t d_memoryBarrierCount;
    const VkMemoryBarrier* d_pMemoryBarriers;
    std::uint32_t d_bufferMemoryBarrierCount;
    const VkBufferMemoryBarrier* d_pBufferMemoryBarriers;
    std::uint32_t d_imageMemoryBarrierCount;
    const VkImageMemoryBarrier* d_pImageMemoryBarriers;
};

// -----------------------------------------------------------------------------

VPP_INLINE Barriers :: Barriers() :
    d_memoryBarrierCount ( 0 ),
    d_pMemoryBarriers ( 0 ),
    d_bufferMemoryBarrierCount ( 0 ),
    d_pBufferMemoryBarriers ( 0 ),
    d_imageMemoryBarrierCount ( 0 ),
    d_pImageMemoryBarriers ( 0 )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const std::vector< VkMemoryBarrier >& bar )
{
    d_pMemoryBarriers = & bar [ 0 ];
    d_memoryBarrierCount = static_cast< std::uint32_t >( bar.size() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const std::vector< VkBufferMemoryBarrier >& bar )
{
    d_pBufferMemoryBarriers = & bar [ 0 ];
    d_bufferMemoryBarrierCount = static_cast< std::uint32_t >( bar.size() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const std::vector< VkImageMemoryBarrier >& bar )
{
    d_pImageMemoryBarriers = & bar [ 0 ];
    d_imageMemoryBarrierCount = static_cast< std::uint32_t >( bar.size() );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const VkMemoryBarrier& bar )
{
    d_pMemoryBarriers = & bar;
    d_memoryBarrierCount = 1;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const VkBufferMemoryBarrier& bar )
{
    d_pBufferMemoryBarriers = & bar;
    d_bufferMemoryBarrierCount = 1;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Barriers :: setBarriers ( const VkImageMemoryBarrier bar )
{
    d_pImageMemoryBarriers = & bar;
    d_imageMemoryBarrierCount = 1;
}

// -----------------------------------------------------------------------------

template< typename BarriersA >
VPP_INLINE Barriers :: Barriers (
    const BarriersA& ba ) :
        Barriers()
{
    setBarriers ( ba );
}

// -----------------------------------------------------------------------------

template< typename BarriersA, typename BarriersB >
VPP_INLINE Barriers :: Barriers (
    const BarriersA& ba,
    const BarriersB& bb ) :
        Barriers()
{
    setBarriers ( ba );
    setBarriers ( bb );
}

// -----------------------------------------------------------------------------

template< typename BarriersA, typename BarriersB, typename BarriersC >
VPP_INLINE Barriers :: Barriers (
    const BarriersA& ba,
    const BarriersB& bb,
    const BarriersC& bc ) :
        Barriers()
{
    setBarriers ( ba );
    setBarriers ( bb );
    setBarriers ( bc );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class MemoryBarrier :
    public VkMemoryBarrier,
    public Barriers
{
    MemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class BufferMemoryBarrier :
    public VkBufferMemoryBarrier,
    public Barriers
{
    BufferMemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        const Buf& buffer,
        VkDeviceSize offset = 0,
        VkDeviceSize size = VK_WHOLE_SIZE,
        uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ImageMemoryBarrier :
    public VkImageMemoryBarrier,
    public Barriers
{
    ImageMemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        const Img& image,
        std::uint32_t baseArrayLayer = 0,
        std::uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS,
        std::uint32_t baseMipLevel = 0,
        std::uint32_t levelCount = VK_REMAINING_MIP_LEVELS,
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPBARRIERS_HPP
