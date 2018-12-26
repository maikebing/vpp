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

#ifndef INC_VPPGEOMETRY_HPP
#define INC_VPPGEOMETRY_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

VPP_INLINE VkOffset2D operator+ ( const VkOffset2D& lhs, const VkOffset2D& rhs )
{
    return VkOffset2D { lhs.x + rhs.x, lhs.y + rhs.y };
}

VPP_INLINE VkOffset2D operator+ ( const VkOffset2D& lhs, const VkExtent2D& rhs )
{
    return VkOffset2D {
        static_cast< std::int32_t >( lhs.x + rhs.width ),
        static_cast< std::int32_t >( lhs.y + rhs.height ) };
}

VPP_INLINE VkOffset2D operator+ ( const VkExtent2D& lhs, const VkOffset2D& rhs )
{
    return VkOffset2D {
        static_cast< std::int32_t >( lhs.width + rhs.x ),
        static_cast< std::int32_t >( lhs.height + rhs.y ) };
}

VPP_INLINE VkOffset2D operator- ( const VkOffset2D& lhs, const VkOffset2D& rhs )
{
    return VkOffset2D { lhs.x - rhs.x, lhs.y - rhs.y };
}

VPP_INLINE VkExtent2D min ( const VkExtent2D& lhs, const VkExtent2D& rhs )
{
    return VkExtent2D {
        std::min ( lhs.width, rhs.width ), 
        std::min ( lhs.height, rhs.height ) }; 
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE VkOffset3D operator+ ( const VkOffset3D& lhs, const VkOffset3D& rhs )
{
    return VkOffset3D { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

VPP_INLINE VkOffset3D operator+ ( const VkOffset3D& lhs, const VkExtent3D& rhs )
{
    return VkOffset3D {
        static_cast< std::int32_t >( lhs.x + rhs.width ),
        static_cast< std::int32_t >( lhs.y + rhs.height ),
        static_cast< std::int32_t >( lhs.z + rhs.depth ) };
}

VPP_INLINE VkOffset3D operator+ ( const VkExtent3D& lhs, const VkOffset3D& rhs )
{
    return VkOffset3D {
        static_cast< std::int32_t >( lhs.width + rhs.x ),
        static_cast< std::int32_t >( lhs.height + rhs.y ),
        static_cast< std::int32_t >( lhs.depth + rhs.z ) };
}

VPP_INLINE VkOffset3D operator- ( const VkOffset3D& lhs, const VkOffset3D& rhs )
{
    return VkOffset3D { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

VPP_INLINE VkExtent3D min ( const VkExtent3D& lhs, const VkExtent3D& rhs )
{
    return VkExtent3D {
        std::min ( lhs.width, rhs.width ), 
        std::min ( lhs.height, rhs.height ), 
        std::min ( lhs.depth, rhs.depth ) }; 
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPGEOMETRY_HPP
