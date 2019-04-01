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
#include "../include/vppBarriers.hpp"
#include "../include/vppBuffer.hpp"
#include "../include/vppImage.hpp"
#include "../include/vppExceptions.hpp"
#include "../include/vppInternalUtils.hpp"

#include <sstream>

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

MemoryBarrier :: MemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask ) :
        VkMemoryBarrier
        {
            VK_STRUCTURE_TYPE_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask
        },
        Barriers ( static_cast< const VkMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

BufferMemoryBarrier :: BufferMemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    const Buf& buffer,
    VkDeviceSize offset,
    VkDeviceSize size,
    uint32_t srcQueueFamilyIndex,
    uint32_t dstQueueFamilyIndex ) :
        VkBufferMemoryBarrier
        {
            VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask,
            srcQueueFamilyIndex, dstQueueFamilyIndex,
            buffer.handle(), offset, size
        },
        Barriers ( static_cast< const VkBufferMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

ImageMemoryBarrier :: ImageMemoryBarrier ( 
    VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask,
    VkImageLayout oldLayout,
    VkImageLayout newLayout,
    const Img& image,
    std::uint32_t baseArrayLayer,
    std::uint32_t layerCount,
    std::uint32_t baseMipLevel,
    std::uint32_t levelCount,
    VkImageAspectFlags aspectMask,
    uint32_t srcQueueFamilyIndex,
    uint32_t dstQueueFamilyIndex ) :
        VkImageMemoryBarrier
        {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, 0,
            srcAccessMask, dstAccessMask,
            oldLayout, newLayout,
            srcQueueFamilyIndex, dstQueueFamilyIndex,
            image.handle(),
            VkImageSubresourceRange {
                aspectMask, baseMipLevel, levelCount, baseArrayLayer, layerCount }
        },
        Barriers ( static_cast< const VkImageMemoryBarrier& >( *this ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: getBarrierAccessMaskHintForSource ( VkFlags stageBit, const Img& resImg )
{
    const std::uint32_t usage = resImg.info().usage;

    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT:
            return 0;

        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            if ( usage & VK_IMAGE_USAGE_STORAGE_BIT )
                return VK_ACCESS_SHADER_WRITE_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_STORAGE_BIT );

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            if ( usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT );

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            if ( usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
                return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT );

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            if ( usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT )
                return VK_ACCESS_TRANSFER_WRITE_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_TRANSFER_DST_BIT );

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_ACCESS_HOST_WRITE_BIT;

        default:
            return errorInvalidStageMask ( resImg, stageBit );
    }
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: getBarrierAccessMaskHintForSource ( VkFlags stageBit, const Buf& resBuf )
{
    const std::uint32_t usage = resBuf.getUsage();

    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            if ( usage & ( VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) )
                return VK_ACCESS_SHADER_WRITE_BIT;
            else
                return errorMissingUsage (
                    resBuf,
                    ( VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT )
                );

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            return errorInvalidStageMask ( resBuf, stageBit );

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            return errorInvalidStageMask ( resBuf, stageBit );

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            if ( usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT )
                return VK_ACCESS_TRANSFER_WRITE_BIT;
            else
                return errorMissingUsage ( resBuf, VK_BUFFER_USAGE_TRANSFER_DST_BIT );

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_ACCESS_HOST_WRITE_BIT;

        default:
            return errorInvalidStageMask ( resBuf, stageBit );
    }
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: getBarrierAccessMaskHintForTarget ( VkFlags stageBit, const Img& resImg )
{
    const std::uint32_t usage = resImg.info().usage;

    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT:
            return errorInvalidStageMask ( resImg, stageBit );

        case VK_PIPELINE_STAGE_VERTEX_INPUT_BIT:
            return errorInvalidStageMask ( resImg, stageBit );

        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            if ( usage & ( VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ) )
                return VK_ACCESS_SHADER_READ_BIT;
            else
                return errorMissingUsage (
                    resImg,
                    ( VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT )
                );

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            if ( usage & ( VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) )
                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT );

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            if ( usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
                return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT );

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            if ( usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT )
                return VK_ACCESS_TRANSFER_READ_BIT;
            else
                return errorMissingUsage ( resImg, VK_IMAGE_USAGE_TRANSFER_SRC_BIT );

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_ACCESS_HOST_READ_BIT;

        default:
            return errorInvalidStageMask ( resImg, stageBit );
    }
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: getBarrierAccessMaskHintForTarget ( VkFlags stageBit, const Buf& resBuf )
{
    const std::uint32_t usage = resBuf.getUsage();
    VkAccessFlags result = 0;

    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT:
            if ( usage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT )
                return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
            else
                return errorMissingUsage ( resBuf, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT );

        case VK_PIPELINE_STAGE_VERTEX_INPUT_BIT:
            if ( usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT )
                return VK_ACCESS_INDEX_READ_BIT;
            else if ( usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT )
                return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            else
                return errorMissingUsage (
                    resBuf,
                    ( VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT )
                );

        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            if ( usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT )
                result |= VK_ACCESS_UNIFORM_READ_BIT;
            if ( usage & (
                    VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT
                    | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT
                    | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ) )
            {
                result |= VK_ACCESS_SHADER_READ_BIT;
            }

            if ( result )
                return result;
            else
                return errorMissingUsage (
                    resBuf,
                    ( VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT
                      | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
                );

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            return errorInvalidStageMask ( resBuf, stageBit );

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            return errorInvalidStageMask ( resBuf, stageBit );

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            if ( usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT )
                return VK_ACCESS_TRANSFER_READ_BIT;
            else
                return errorMissingUsage ( resBuf, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_ACCESS_HOST_READ_BIT;

        default:
            return errorInvalidStageMask ( resBuf, stageBit );
    }
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: errorMissingUsage ( const Img& resImg, unsigned int u )
{
    std::ostringstream sst;

    sst << "Image " << resImg.handle() << " has no required usage bit set: ";

    const char* pUsage = 0;
    unsigned int nBits = 0;

    for ( unsigned int b = 1; b != 0; b <<= 1 )
        if ( b & u )
    {
        switch ( b )
        {
            case VK_IMAGE_USAGE_TRANSFER_SRC_BIT: pUsage = "VK_IMAGE_USAGE_TRANSFER_SRC_BIT"; break;
            case VK_IMAGE_USAGE_TRANSFER_DST_BIT: pUsage = "VK_IMAGE_USAGE_TRANSFER_DST_BIT"; break;
            case VK_IMAGE_USAGE_SAMPLED_BIT: pUsage = "VK_IMAGE_USAGE_SAMPLED_BIT"; break;
            case VK_IMAGE_USAGE_STORAGE_BIT: pUsage = "VK_IMAGE_USAGE_STORAGE_BIT"; break;
            case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT: pUsage = "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT"; break;
            case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT: pUsage = "VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT"; break;
            case VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT: pUsage = "VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT"; break;
            case VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT: pUsage = "VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT"; break;
            default: throw XRuntimeError ( "Internal error" ); break;
        }

        if ( nBits > 0 )
            sst << " or ";

        sst << pUsage << std::endl;
        ++nBits;
    }

    throw XInvalidBarrier ( sst.str() );

    return 0;
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: errorMissingUsage ( const Buf& resBuf, unsigned int u )
{
    std::ostringstream sst;

    sst << "Buffer " << resBuf.handle() << " has no required usage bit set: ";

    const char* pUsage = 0;
    unsigned int nBits = 0;

    for ( unsigned int b = 1; b != 0; b <<= 1 )
        if ( b & u )
    {
        switch ( u )
        {
            case VK_BUFFER_USAGE_TRANSFER_SRC_BIT: pUsage = "VK_BUFFER_USAGE_TRANSFER_SRC_BIT"; break;
            case VK_BUFFER_USAGE_TRANSFER_DST_BIT: pUsage = "VK_BUFFER_USAGE_TRANSFER_DST_BIT"; break;
            case VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_STORAGE_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_INDEX_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_INDEX_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_VERTEX_BUFFER_BIT"; break;
            case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT: pUsage = "VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT"; break;
            default: throw XRuntimeError ( "Internal error" ); break;
        }

        if ( nBits > 0 )
            sst << " or ";

        sst << pUsage << std::endl;
        ++nBits;
    }

    throw XInvalidBarrier ( sst.str() );

    return 0;
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: errorInvalidStageMask ( const Img& resImg, unsigned int m )
{
    std::ostringstream sst;

    switch ( detail::count1bits ( m ) )
    {
        case 0:
            sst << "Image " << resImg.handle() << " has zero stage mask." << std::endl;
            break;

        case 1:
            sst << "Image " << resImg.handle() << " has invalid stage bit: " << m << std::endl;
            break;

        default:
            sst << "Image " << resImg.handle() << " has multiple stage bits, unsupported by this function: " << m << std::endl;
            break;
    }

    throw XInvalidBarrier ( sst.str() );
    return 0;
}

// -----------------------------------------------------------------------------

VkAccessFlags BarrierList :: errorInvalidStageMask ( const Buf& resBuf, unsigned int m )
{
    std::ostringstream sst;

    switch ( detail::count1bits ( m ) )
    {
        case 0:
            sst << "Buffer " << resBuf.handle() << " has zero stage mask." << std::endl;
            break;

        case 1:
            sst << "Buffer " << resBuf.handle() << " has invalid stage bit: " << m << std::endl;
            break;

        default:
            sst << "Buffer " << resBuf.handle() << " has multiple stage bits, unsupported by this function: " << m << std::endl;
            break;
    }

    throw XInvalidBarrier ( sst.str() );
    return 0;
}

// -----------------------------------------------------------------------------

VkImageLayout BarrierList :: getBarrierLayoutHintForSource ( VkFlags stageBit, const Img& resImg )
{
    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT:
            return VK_IMAGE_LAYOUT_UNDEFINED;

        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            return VK_IMAGE_LAYOUT_GENERAL;

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_IMAGE_LAYOUT_PREINITIALIZED;

        default:
            return VK_IMAGE_LAYOUT_GENERAL;
    }
}

// -----------------------------------------------------------------------------

VkImageLayout BarrierList :: getBarrierLayoutHintForTarget ( VkFlags stageBit, const Img& resImg )
{
    const std::uint32_t usage = resImg.info().usage;

    switch ( stageBit )
    {
        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
            if ( usage & VK_IMAGE_USAGE_STORAGE_BIT )
                return VK_IMAGE_LAYOUT_GENERAL;
            else if ( usage & VK_IMAGE_USAGE_SAMPLED_BIT )
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            else
                return VK_IMAGE_LAYOUT_GENERAL;

        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        case VK_PIPELINE_STAGE_TRANSFER_BIT:
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

        case VK_PIPELINE_STAGE_HOST_BIT:
            return VK_IMAGE_LAYOUT_GENERAL;

        default:
            return VK_IMAGE_LAYOUT_GENERAL;
    }
}

// -----------------------------------------------------------------------------

void BarrierList :: addBarrier ( const Img& hImg )
{
    VkImageMemoryBarrier imgBarrier;
    const ImageInfo& imgInfo = hImg.info();

    imgBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgBarrier.pNext = 0;

    imgBarrier.srcAccessMask = getBarrierAccessMaskHintForSource ( d_sourceStage, hImg );
    imgBarrier.oldLayout = getBarrierLayoutHintForSource ( d_sourceStage, hImg );

    if ( imgBarrier.srcAccessMask )
    {
        imgBarrier.dstAccessMask = getBarrierAccessMaskHintForTarget ( d_targetStage, hImg );
        imgBarrier.newLayout = getBarrierLayoutHintForTarget ( d_targetStage, hImg );
    }
    else
    {
        imgBarrier.dstAccessMask = getBarrierAccessMaskHintForSource ( d_targetStage, hImg );
        imgBarrier.newLayout = getBarrierLayoutHintForSource ( d_targetStage, hImg );
    }

    imgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imgBarrier.image = hImg.handle();
    imgBarrier.subresourceRange.baseMipLevel = 0;
    imgBarrier.subresourceRange.levelCount = imgInfo.mipLevels;
    imgBarrier.subresourceRange.baseArrayLayer = 0;
    imgBarrier.subresourceRange.layerCount = imgInfo.arrayLayers;
    imgBarrier.subresourceRange.aspectMask = imgInfo.getAspect();

    d_imageBarriers.push_back ( imgBarrier );
}

// -----------------------------------------------------------------------------

void BarrierList :: addBarrier ( const Buf& hBuf )
{
    VkBufferMemoryBarrier bufBarrier;

    bufBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufBarrier.pNext = 0;
    bufBarrier.srcAccessMask = getBarrierAccessMaskHintForSource ( d_sourceStage, hBuf );
    bufBarrier.dstAccessMask = getBarrierAccessMaskHintForTarget ( d_targetStage, hBuf );
    bufBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufBarrier.buffer = hBuf.handle();
    bufBarrier.offset = 0;
    bufBarrier.size = VK_WHOLE_SIZE;

    d_bufferBarriers.push_back ( bufBarrier );
}

// -----------------------------------------------------------------------------

void BarrierList :: createBarriers (
    const Img& hImg,
    const std::vector< VkImageSubresourceRange >& regions )
{
    d_bufferBarriers.reserve ( d_bufferBarriers.size() + regions.size() );

    for ( size_t i = 0; i != regions.size(); ++i )
    {
        VkImageMemoryBarrier imgBarrier;
        const ImageInfo& imgInfo = hImg.info();

        imgBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imgBarrier.pNext = 0;
        imgBarrier.srcAccessMask = getBarrierAccessMaskHintForSource ( d_sourceStage, hImg );
        imgBarrier.dstAccessMask = getBarrierAccessMaskHintForTarget ( d_targetStage, hImg );
        imgBarrier.oldLayout = getBarrierLayoutHintForSource ( d_sourceStage, hImg );
        imgBarrier.newLayout = getBarrierLayoutHintForTarget ( d_targetStage, hImg );
        imgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgBarrier.image = hImg.handle();

        imgBarrier.subresourceRange = regions [ i ];

        d_imageBarriers.push_back ( imgBarrier );
    }
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
