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

#ifndef INC_VPPRENDERGRAPHNODES_HPP
#define INC_VPPRENDERGRAPHNODES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPIMAGEINFO_HPP
#include "vppImageInfo.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class BaseAttachment
{
public:
    VPP_DLLAPI BaseAttachment (
        const VkExtent2D& imageSize,
        EImagePurpose imagePurpose,
        VkFormat format );

    VPP_DLLAPI BaseAttachment (
        RenderGraph* pGraph,
        const VkExtent2D& imageSize,
        EImagePurpose imagePurpose,
        VkFormat format );

    VPP_DLLAPI BaseAttachment (
        const ImageInfo& imageInfo );

    VPP_DLLAPI BaseAttachment (
        RenderGraph* pGraph,
        const ImageInfo& imageInfo );

    VPP_DLLAPI BaseAttachment (
        const FrameImageView& imageView );

    VPP_DLLAPI BaseAttachment (
        RenderGraph* pGraph,
        const FrameImageView& imageView );

    VPP_DLLAPI ~BaseAttachment();

    RenderGraph* graph() const;

    std::uint32_t index() const;
    std::uint32_t width() const;
    std::uint32_t height() const;

    VPP_DLLAPI const BaseAttachment& aliasesMemory ( const BaseAttachment& rhs );

    VPP_DLLAPI void setFormat ( VkFormat fmt );

protected:
    VPP_DLLAPI static VkFormat getSurfaceFormat ( const Surface& hSurface );

private:
    RenderGraph* d_pInstance;
    std::uint32_t d_index;
    std::uint32_t d_width;
    std::uint32_t d_height;
};

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t BaseAttachment :: index() const
{
    return d_index;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t BaseAttachment :: width() const
{
    return d_width;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t BaseAttachment :: height() const
{
    return d_height;
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph* BaseAttachment :: graph() const
{
    return d_pInstance;
}

// -----------------------------------------------------------------------------

template< class FormatT >
class Attachment : public BaseAttachment
{
public:
    VPP_INLINE Attachment (
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose = RENDER ) :
            BaseAttachment ( VkExtent2D { width, height }, imagePurpose, FormatT::code )
    {}

    VPP_INLINE Attachment (
        RenderGraph* pGraph,
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose = RENDER ) :
            BaseAttachment ( pGraph, VkExtent2D { width, height }, imagePurpose, FormatT::code )
    {}

    template< class ViewT >
    VPP_INLINE Attachment (
        const ViewT& imageView ) :
            BaseAttachment ( imageView.frameView() )
    {}

    template< class ViewT >
    VPP_INLINE Attachment (
        RenderGraph* pGraph,
        const ViewT& imageView ) :
            BaseAttachment ( pGraph, imageView.frameView() )
    {}
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Display : public BaseAttachment
{
public:
    VPP_DLLAPI Display ( const Surface& hSurface );
    VPP_DLLAPI Display ( RenderGraph* pGraph, const Surface& hSurface );
    VPP_DLLAPI ~Display();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Process
{
public:
    VPP_DLLAPI Process();
    VPP_DLLAPI Process ( RenderGraph* pGraph );
    VPP_DLLAPI Process ( RenderGraph* pGraph, std::uint32_t index );
    VPP_DLLAPI ~Process();

    std::uint32_t index() const;
    RenderGraph* graph() const;

    template< class FormatT >
    void addInput (
        const Attachment< FormatT >& d,
        VkImageLayout nodeImageLayout = VK_IMAGE_LAYOUT_GENERAL,
        bool bAddDependency = true );

    // Adds color output node to be produced from scratch. Any previous
    // content is destroyed.
    template< class FormatT >
    void addColorOutput (
        const Attachment< FormatT >& d,
        const typename FormatT::init_type& initValue,
        VkImageLayout destLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    // Adds color output node which was already filled by previous render passes,
    // to be modified by this render pass.
    template< class FormatT >
    void addReusedColorOutput (
        const Attachment< FormatT >& d,
        VkImageLayout destLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    // Adds color output node for display image.
    VPP_DLLAPI void addColorOutput ( const Display& d );
    VPP_DLLAPI void addColorOutput ( const Display& d, const VkClearColorValue& color );

    // Sets depth node to be produced from scratch. Any previous
    // content is destroyed.
    template< class FormatT >
    void setDepthOutput (
        const Attachment< FormatT >& d,
        const typename FormatT::init_type& initValue,
        VkImageLayout preserveInLayout = VK_IMAGE_LAYOUT_UNDEFINED );

    // Sets depth node which was already filled by previous render passes,
    // to be modified by this render pass.
    template< class FormatT >
    void setReusedDepthOutput (
        const Attachment< FormatT >& d,
        VkImageLayout preserveInLayout = VK_IMAGE_LAYOUT_UNDEFINED );

    // Sets depth node which was already filled by previous render passes,
    // to be read by this render pass. The data is read-only.
    template< class FormatT >
    void setDepthInput ( const Attachment< FormatT >& d );

    template< class FormatT >
    void addColorAndResolveOutput (
        const Attachment< FormatT >& dc,
        const Attachment< FormatT >& dr,
        const typename FormatT::init_type& initValue );

    VPP_DLLAPI void addPreservedOutput ( const BaseAttachment& d );

    VPP_DLLAPI void addDependency (
        const Process& targetProcess,
        const VkSubpassDependency& dependency );

private:
    RenderGraph* d_pInstance;
    std::uint32_t d_index;
};

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Process :: index() const
{
    return d_index;
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph* Process :: graph() const
{
    return d_pInstance;
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: addInput (
    const Attachment< FormatT >& d,
    VkImageLayout nodeImageLayout,
    bool bAddDependency )
{
    d_pInstance->addProcessInputNode (
        d_index, d.index(), nodeImageLayout, bAddDependency );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: addColorOutput (
    const Attachment< FormatT >& d,
    const typename FormatT::init_type& initValue,
    VkImageLayout destLayout )
{
    VkClearValue clearValue;
    initValue.convert ( & clearValue );
    d_pInstance->addProcessOutputColorNode ( d_index, d.index(), clearValue, destLayout );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: addReusedColorOutput (
    const Attachment< FormatT >& d,
    VkImageLayout destLayout )
{
    d_pInstance->addProcessOutputColorNode ( d_index, d.index(), destLayout );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: setDepthOutput (
    const Attachment< FormatT >& d,
    const typename FormatT::init_type& initValue,
    VkImageLayout preserveInLayout )
{
    VkClearValue clearValue;
    initValue.convert ( & clearValue );

    d_pInstance->addProcessOutputDepthNode (
        d_index, d.index(), & clearValue, preserveInLayout );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: setReusedDepthOutput (
    const Attachment< FormatT >& d,
    VkImageLayout preserveInLayout )
{
    d_pInstance->addProcessOutputDepthNode (
        d_index, d.index(), 0, preserveInLayout );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: setDepthInput (
    const Attachment< FormatT >& d )
{
    d_pInstance->addProcessInputDepthNode ( d_index, d.index() );
}

// -----------------------------------------------------------------------------

template< class FormatT >
VPP_INLINE void Process :: addColorAndResolveOutput (
    const Attachment< FormatT >& dc,
    const Attachment< FormatT >& dr,
    const typename FormatT::init_type& initValue )
{
    VkClearValue clearValue;
    initValue.convert ( & clearValue );
    d_pInstance->addProcessOutputColorAndResolveNode ( d_index, dc.index(), dr.index(), clearValue );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Preprocess
{
public:
    VPP_DLLAPI Preprocess();
    VPP_DLLAPI Preprocess ( RenderGraph* pGraph );
    VPP_DLLAPI ~Preprocess();

    std::uint32_t index() const;
    RenderGraph* graph() const;

private:
    RenderGraph* d_pInstance;
    std::uint32_t d_index;
};

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Preprocess :: index() const
{
    return d_index;
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph* Preprocess :: graph() const
{
    return d_pInstance;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Postprocess
{
public:
    VPP_DLLAPI Postprocess();
    VPP_DLLAPI Postprocess ( RenderGraph* pGraph );
    VPP_DLLAPI ~Postprocess();

    std::uint32_t index() const;
    RenderGraph* graph() const;

private:
    RenderGraph* d_pInstance;
    std::uint32_t d_index;
};

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Postprocess :: index() const
{
    return d_index;
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph* Postprocess :: graph() const
{
    return d_pInstance;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_DLLAPI void operator<< ( const Process& target, const std::function< void () >& cmds );
VPP_DLLAPI void operator<< ( const Preprocess& target, const std::function< void () >& cmds );
VPP_DLLAPI void operator<< ( const Postprocess& target, const std::function< void () >& cmds );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPRENDERGRAPHNODES_HPP
