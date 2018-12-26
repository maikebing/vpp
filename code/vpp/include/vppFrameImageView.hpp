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

#ifndef INC_VPPFRAMEIMAGEVIEW_HPP
#define INC_VPPFRAMEIMAGEVIEW_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPIMAGE_HPP
#include "vppImage.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class FrameImageViewImpl;

// -----------------------------------------------------------------------------

class FrameImageView : public TSharedReference< FrameImageViewImpl >
{
public:
    FrameImageView();
    explicit FrameImageView ( const Img& hImage, std::uint32_t layer = 0 );
    FrameImageView ( const Img& hImage, const VkImageSubresourceRange& sr );
    FrameImageView ( const ImageInfo& imageInfo, const Device& hDevice );
    FrameImageView ( const FrameImageView& rhs );

    template< class MemoryT, class ResourceT >
    FrameImageView (
        const MemoryBinding< ResourceT, MemoryT >& binding,
        std::uint32_t layer = 0 );

    ~FrameImageView();

    const FrameImageView& operator= ( const FrameImageView& rhs );

    operator VkImageView() const;
    const Img& image() const;
    const Device& device() const;
    const FrameImageView& frameView() const;
};

// -----------------------------------------------------------------------------

class FrameImageViewImpl : public TSharedObject< FrameImageViewImpl >
{
public:
    VPP_DLLAPI FrameImageViewImpl ( const Img& hImage, std::uint32_t layer );
    VPP_DLLAPI FrameImageViewImpl ( const Img& hImage, const VkImageSubresourceRange& range );
    VPP_DLLAPI ~FrameImageViewImpl();

private:
    friend class FrameImageView;
    Img d_hImage;
    Device d_hDevice;
    VkImageView d_handle;
};

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: FrameImageView()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: FrameImageView ( const Img& hImage, std::uint32_t layer ) :
    TSharedReference< FrameImageViewImpl >( new FrameImageViewImpl ( hImage, layer ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: FrameImageView ( const Img& hImage, const VkImageSubresourceRange& sr ) :
    TSharedReference< FrameImageViewImpl >( new FrameImageViewImpl ( hImage, sr ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: FrameImageView (
    const ImageInfo& imageInfo, const Device& hDevice ) :
        FrameImageView (
            Img ( imageInfo, MemProfile::DEVICE_STATIC, hDevice ) )
{
}

// -----------------------------------------------------------------------------

template< class MemoryT, class ResourceT >
VPP_INLINE FrameImageView :: FrameImageView (
    const MemoryBinding< ResourceT, MemoryT >& binding,
    std::uint32_t layer ) :
        TSharedReference< FrameImageViewImpl >( new FrameImageViewImpl ( binding.resource(), layer ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: FrameImageView ( const FrameImageView& rhs ) :
    TSharedReference< FrameImageViewImpl >( rhs )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: ~FrameImageView()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE const FrameImageView& FrameImageView :: operator= ( const FrameImageView& rhs )
{
    TSharedReference< FrameImageViewImpl >::operator= ( rhs );
    return *this;
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView :: operator VkImageView() const
{
    if ( *this )
        return get()->d_handle;
    else
        return VK_NULL_HANDLE;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Img& FrameImageView :: image() const
{
    return get()->d_hImage;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& FrameImageView :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE const FrameImageView& FrameImageView :: frameView() const
{
    return *this;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPFRAMEIMAGEVIEW_HPP
