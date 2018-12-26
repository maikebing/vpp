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

#ifndef INC_VPPDEBUGPROBE_HPP
#define INC_VPPDEBUGPROBE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGINTIMAGES_HPP
#include "vppLangIntImages.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class ValueT > struct TDebugImageFormatForValue {};

template<> struct TDebugImageFormatForValue< Int >
{ typedef format< int > format_type; };

template<> struct TDebugImageFormatForValue< UInt >
{ typedef format< unsigned int > format_type; };

template<> struct TDebugImageFormatForValue< Float >
{ typedef format< float > format_type; };

template<> struct TDebugImageFormatForValue< IVec4 >
{ typedef format< int, int, int, int > format_type; };

template<> struct TDebugImageFormatForValue< UVec4 >
{ typedef format< unsigned int, unsigned int, unsigned int, unsigned int > format_type; };

template<> struct TDebugImageFormatForValue< Vec4 >
{ typedef format< float, float, float, float > format_type; };

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ValueT >
class TDebugProbe : public IDebugProbe
{
public:
    typedef VPP_RVTYPE(ValueT) rvalue_type;
    typedef typename detail::TDebugImageFormatForValue< rvalue_type >::format_type format_type;

    typedef ImageAttributes<
        format_type, RENDER_DEBUG, IMG_TYPE_2D,
        VK_IMAGE_USAGE_STORAGE_BIT,
        VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
        false, false > DebugImageAttr;

    typedef Image< DebugImageAttr > DebugImage;
    typedef ImageViewAttributes< DebugImage > DebugImageViewAttr;
    typedef ImageView< DebugImageViewAttr > DebugImageView;

    VPP_INLINE TDebugProbe (
        const VkExtent3D& extent,
        const Device& hDevice ) :
            d_image ( extent, MemProfile::DEVICE_STATIC, hDevice ),
            d_view ( d_image )
    {}

    virtual void bind ( const ShaderDataBlock& hDataBlock )
    {
        IDebugProbe::bind (
            d_view,
            d_bindingPoint.set(), d_bindingPoint.binding(),
            hDataBlock );
    }

    VPP_INLINE void write ( const IVec2& coords, const ValueT& value )
    {
        const rvalue_type rv = value;
        ImageStore ( d_bindingPoint, coords, rv );
    }

    VPP_INLINE VkFormat format() const
    {
        return format_type::code;
    }

private:
    DebugImage d_image;
    DebugImageView d_view;
    ioImage< DebugImageView > d_bindingPoint;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPDEBUGPROBE_HPP
