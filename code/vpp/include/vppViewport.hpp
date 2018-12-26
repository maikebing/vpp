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

#ifndef INC_VPPVIEWPORT_HPP
#define INC_VPPVIEWPORT_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct Viewport : public VkViewport
{
    Viewport ( std::uint32_t _width, std::uint32_t _height );
    
    Viewport (
        std::uint32_t _width, std::uint32_t _height,
        float _minDepth, float _maxDepth );

    VkRect2D d_scissor;
};

// -----------------------------------------------------------------------------

VPP_INLINE Viewport :: Viewport ( std::uint32_t _width, std::uint32_t _height )
{
    x = 0;
    y = 0;
    width = static_cast< float >( _width );
    height = static_cast< float >( _height );
    minDepth = 0.0f;
    maxDepth = 1.0f;
    d_scissor.offset.x = 0;
    d_scissor.offset.y = 0;
    d_scissor.extent.width = _width;
    d_scissor.extent.height = _height;
}

// -----------------------------------------------------------------------------

VPP_INLINE Viewport :: Viewport (
    std::uint32_t _width, std::uint32_t _height,
    float _minDepth, float _maxDepth )
{
    x = 0;
    y = 0;
    width = static_cast< float >( _width );
    height = static_cast< float >( _height );
    minDepth = _minDepth;
    maxDepth = _maxDepth;
    d_scissor.offset.x = 0;
    d_scissor.offset.y = 0;
    d_scissor.extent.width = _width;
    d_scissor.extent.height = _height;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPVIEWPORT_HPP
