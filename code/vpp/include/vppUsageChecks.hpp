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

#ifndef INC_VPPUSAGECHECKS_HPP
#define INC_VPPUSAGECHECKS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class TSBuf
{
public:
    VPP_INLINE TSBuf ( const Buf& buf ) :
        d_buf ( buf )
    {}

    template< unsigned int USAGE >
    VPP_INLINE TSBuf ( const Buffer< USAGE >& buf ) :
        d_buf ( buf )
    {
        static const bool bSourceUsageBit = (
            ( USAGE & VK_BUFFER_USAGE_TRANSFER_SRC_BIT ) != 0 );

        static_assert ( bSourceUsageBit, "Source buffer must have Buf::SOURCE usage bit set." );
    }

    VPP_INLINE const Buf& bufferRef() const
    {
        return d_buf;
    }

private:
    const Buf& d_buf;
};

// -----------------------------------------------------------------------------

class TDBuf
{
public:
    VPP_INLINE TDBuf ( const Buf& buf ) :
        d_buf ( buf )
    {}

    template< unsigned int USAGE >
    VPP_INLINE TDBuf ( const Buffer< USAGE >& buf ) :
        d_buf ( buf )
    {
        static const bool bTargetUsageBit = (
            ( USAGE & VK_BUFFER_USAGE_TRANSFER_DST_BIT ) != 0 );

        static_assert ( bTargetUsageBit, "Destination buffer must have Buf::TARGET usage bit set." );
    }

    VPP_INLINE const Buf& bufferRef() const
    {
        return d_buf;
    }

private:
    const Buf& d_buf;
};

// -----------------------------------------------------------------------------

class TSImg
{
public:
    VPP_INLINE TSImg ( const Img& img ) :
        d_img ( img )
    {}

    template< class AttributesT >
    VPP_INLINE TSImg ( const Image< AttributesT >& img ) :
        d_img ( img )
    {
        typedef typename Image< AttributesT >::attributes_type attributes_type;

        static const bool bSourceUsageBit = (
            ( attributes_type::usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) != 0 );

        static_assert ( bSourceUsageBit, "Source image must have Img::SOURCE usage bit set." );
    }

    VPP_INLINE const Img& imageRef() const
    {
        return d_img;
    }

private:
    const Img& d_img;
};

// -----------------------------------------------------------------------------

class TDImg
{
public:
    VPP_INLINE TDImg ( const Img& img ) :
        d_img ( img )
    {}

    template< class AttributesT >
    VPP_INLINE TDImg ( const Image< AttributesT >& img ) :
        d_img ( img )
    {
        typedef typename Image< AttributesT >::attributes_type attributes_type;

        static const bool bDestUsageBit = (
            ( attributes_type::usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) != 0 );

        static_assert ( bDestUsageBit, "Destination image must have Img::TARGET usage bit set." );
    }

    VPP_INLINE const Img& imageRef() const
    {
        return d_img;
    }

private:
    const Img& d_img;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPUSAGECHECKS_HPP
