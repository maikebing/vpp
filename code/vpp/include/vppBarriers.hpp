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

#ifndef INC_VPPBARRIERS_HPP
#define INC_VPPBARRIERS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class Bar
{
public:
    enum EStage
    {
        NONE = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        INDIRECT = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
        VTXIN = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        VSHADER = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        VERTEX = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        TCSHADER = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
        TESHADER = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
        GSHADER = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
        GEOMETRY = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
        FSHADER = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        FRAGMENT = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        EDEPTH = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        LDEPTH = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        CLROUT = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        CSHADER = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        COMPUTE = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        TRANSFER = VK_PIPELINE_STAGE_TRANSFER_BIT,
        HOST = VK_PIPELINE_STAGE_HOST_BIT
    };
};

// -----------------------------------------------------------------------------

class Barriers : public Bar
{
public:
    Barriers();
    Barriers ( const Barriers& rhs );

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

VPP_INLINE Barriers :: Barriers ( const Barriers& rhs ) :
    d_memoryBarrierCount ( rhs.d_memoryBarrierCount ),
    d_pMemoryBarriers ( rhs.d_pMemoryBarriers ),
    d_bufferMemoryBarrierCount ( rhs.d_bufferMemoryBarrierCount ),
    d_pBufferMemoryBarriers ( rhs.d_pBufferMemoryBarriers ),
    d_imageMemoryBarrierCount ( rhs.d_imageMemoryBarrierCount ),
    d_pImageMemoryBarriers ( rhs.d_pImageMemoryBarriers )
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
// -----------------------------------------------------------------------------

class BarrierList : public Barriers
{
public:
    BarrierList ( VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage );
    BarrierList ( BarrierList&& rhs );

    VPP_DLLAPI void addBarrier ( const Img& hImg );
    VPP_DLLAPI void addBarrier ( const Buf& hBuf );

    VPP_DLLAPI void createBarriers (
        const Img& hImg,
        const std::vector< VkImageSubresourceRange >& regions );

    template< typename SingleT >
    void collectBarriers ( SingleT&& single )
    {
        addBarrier ( single );
    }

    template< typename FirstT, typename... ArgTypes >
    void collectBarriers ( FirstT&& first, ArgTypes&& ... args )
    {
        collectBarriers ( first );
        collectBarriers ( args... );
    }

private:
    static VkAccessFlags getBarrierAccessMaskHintForSource ( VkFlags stageBit, const Img& resImg );
    static VkAccessFlags getBarrierAccessMaskHintForSource ( VkFlags stageBit, const Buf& resBuf );
    static VkAccessFlags getBarrierAccessMaskHintForTarget ( VkFlags stageBit, const Img& resImg );
    static VkAccessFlags getBarrierAccessMaskHintForTarget ( VkFlags stageBit, const Buf& resBuf );

    static VkAccessFlags errorMissingUsage ( const Img& resImg, unsigned int u );
    static VkAccessFlags errorMissingUsage ( const Buf& resBuf, unsigned int u );
    static VkAccessFlags errorInvalidStageMask ( const Img& resImg, unsigned int m );
    static VkAccessFlags errorInvalidStageMask ( const Buf& resBuf, unsigned int m );

    static VkImageLayout getBarrierLayoutHintForSource ( VkFlags stageBit, const Img& resImg );
    static VkImageLayout getBarrierLayoutHintForTarget ( VkFlags stageBit, const Img& resImg );

public:
    VkPipelineStageFlags d_sourceStage;
    VkPipelineStageFlags d_targetStage;

    std::vector< VkBufferMemoryBarrier > d_bufferBarriers;
    std::vector< VkImageMemoryBarrier > d_imageBarriers;
    std::vector< VkMemoryBarrier > d_memoryBarriers;
};

// -----------------------------------------------------------------------------

VPP_INLINE BarrierList :: BarrierList (
    VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage ) :
        d_sourceStage ( sourceStage ),
        d_targetStage ( targetStage )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE BarrierList :: BarrierList ( BarrierList&& rhs ) :
    Barriers ( static_cast< const Barriers& >( rhs ) )
{
    d_sourceStage = rhs.d_sourceStage;
    d_targetStage = rhs.d_targetStage;

    d_bufferBarriers.swap ( rhs.d_bufferBarriers );
    d_imageBarriers.swap ( rhs.d_imageBarriers );
    d_memoryBarriers.swap ( rhs.d_memoryBarriers );
}

// -----------------------------------------------------------------------------

template< typename ... Args >
VPP_INLINE BarrierList barriers (
    VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage,
    Args&& ... args )
{
    BarrierList result ( sourceStage, targetStage );
    
    result.collectBarriers ( args... );

    if ( ! result.d_bufferBarriers.empty() )
        result.setBarriers ( result.d_bufferBarriers );

    if ( ! result.d_imageBarriers.empty() )
        result.setBarriers ( result.d_imageBarriers );

    return result;
}

// -----------------------------------------------------------------------------

VPP_INLINE BarrierList barriers (
    const Img& hImage,
    const std::vector< VkImageSubresourceRange >& regions,
    VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage )
{
    BarrierList result ( sourceStage, targetStage );
    
    result.createBarriers ( hImage, regions );

    if ( ! result.d_bufferBarriers.empty() )
        result.setBarriers ( result.d_bufferBarriers );

    if ( ! result.d_imageBarriers.empty() )
        result.setBarriers ( result.d_imageBarriers );

    return result;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPBARRIERS_HPP
