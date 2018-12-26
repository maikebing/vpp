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

#ifndef INC_VPPFRAMEBUFFER_HPP
#define INC_VPPFRAMEBUFFER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPFRAMEIMAGEVIEW_HPP
#include "vppFrameImageView.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KFrameBufferImpl;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class FrameBuffer : public TSharedReference< KFrameBufferImpl >
{
public:
    FrameBuffer();

    FrameBuffer (
        const RenderPass& hRenderPass );

    FrameBuffer (
        const std::vector< FrameImageView >& imageViews,
        const RenderPass& hRenderPass );

    ~FrameBuffer();

    VkFramebuffer handle() const;
    VkRect2D area() const;
    FrameImageView imageView ( size_t index ) const;
    size_t imageViewCount() const;
};

// -----------------------------------------------------------------------------

class KFrameBufferImpl : public TSharedObject< KFrameBufferImpl >
{
public:
    VPP_DLLAPI KFrameBufferImpl (
        const std::vector< FrameImageView >& imageViews,
        const RenderPass& hRenderPass );

    VPP_DLLAPI ~KFrameBufferImpl();

    VPP_INLINE bool compareObjects ( const KFrameBufferImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class FrameBuffer;

    Device d_hDevice;
    VkFramebuffer d_handle;
    VkResult d_result;

    std::uint32_t d_width;
    std::uint32_t d_height;
    std::uint32_t d_layers;

    std::vector< FrameImageView > d_imageViews;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE FrameBuffer :: FrameBuffer()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameBuffer :: FrameBuffer (
    const RenderPass& hRenderPass ) :
        TSharedReference< KFrameBufferImpl >( new KFrameBufferImpl (
            std::vector< FrameImageView >(), hRenderPass ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameBuffer :: FrameBuffer (
    const std::vector< FrameImageView >& imageViews,
    const RenderPass& hRenderPass ) :
        TSharedReference< KFrameBufferImpl >( new KFrameBufferImpl (
            imageViews, hRenderPass ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameBuffer :: ~FrameBuffer()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkFramebuffer FrameBuffer :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkRect2D FrameBuffer :: area() const
{
    const VkRect2D result = { { 0, 0 }, { get()->d_width, get()->d_height } };
    return result;
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView FrameBuffer :: imageView ( size_t index ) const
{
    return get()->d_imageViews [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE size_t FrameBuffer :: imageViewCount() const
{
    return get()->d_imageViews.size();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef std::vector< FrameBuffer > FrameBuffers;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPFRAMEBUFFER_HPP
