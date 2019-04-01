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

#ifndef INC_VPPEXCEPTIONS_HPP
#define INC_VPPEXCEPTIONS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class XRuntimeError : public std::runtime_error
{
public:
    XRuntimeError ( const char* pMessage );
};

// -----------------------------------------------------------------------------

VPP_INLINE XRuntimeError :: XRuntimeError ( const char* pMessage ) :
    std::runtime_error ( pMessage )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XUsageError : public std::runtime_error
{
public:
    XUsageError ( const char* pMessage );
};

// -----------------------------------------------------------------------------

VPP_INLINE XUsageError :: XUsageError ( const char* pMessage ) :
    std::runtime_error ( pMessage )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XSurfaceNotSupportedByDevice : public XRuntimeError
{
public:
    XSurfaceNotSupportedByDevice();
};

// -----------------------------------------------------------------------------

VPP_INLINE XSurfaceNotSupportedByDevice :: XSurfaceNotSupportedByDevice() :
    XRuntimeError ( "Surface provided for the swapchain is not supported by rendering device" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XMemoryAllocationError : public XRuntimeError
{
public:
    XMemoryAllocationError();
};

// -----------------------------------------------------------------------------

VPP_INLINE XMemoryAllocationError :: XMemoryAllocationError() :
    XRuntimeError ( "Memory allocation error" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XDeviceMemoryTypeError : public XRuntimeError
{
public:
    XDeviceMemoryTypeError();
};

// -----------------------------------------------------------------------------

VPP_INLINE XDeviceMemoryTypeError :: XDeviceMemoryTypeError() :
    XRuntimeError ( "Device memory type error" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XVectorOverflow : public XRuntimeError
{
public:
    XVectorOverflow();
};

// -----------------------------------------------------------------------------

VPP_INLINE XVectorOverflow :: XVectorOverflow() :
    XRuntimeError ( "Device vector capacity overflow" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XPushConstantTooBig : public XRuntimeError
{
public:
    XPushConstantTooBig();
};

// -----------------------------------------------------------------------------

VPP_INLINE XPushConstantTooBig :: XPushConstantTooBig() :
    XRuntimeError ( "Push constant size exceeds maximum size supported by the device" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XSharedMemoryLimitExceeded : public XRuntimeError
{
public:
    XSharedMemoryLimitExceeded ( unsigned int r, unsigned int l );

    unsigned int requested() const;
    unsigned int limit() const;

private:
    unsigned int d_requested;
    unsigned int d_limit;
};

// -----------------------------------------------------------------------------

VPP_INLINE XSharedMemoryLimitExceeded :: XSharedMemoryLimitExceeded (
    unsigned int r, unsigned int l ) :
        XRuntimeError (
            "Requested workgroup-scoped variables and arrays exceed maximum size supported by the device" ),
        d_requested ( r ),
        d_limit ( l )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int XSharedMemoryLimitExceeded :: requested() const
{
    return d_requested;
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int XSharedMemoryLimitExceeded :: limit() const
{
    return d_limit;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XImageNotBound : public XRuntimeError
{
public:
    XImageNotBound();
};

// -----------------------------------------------------------------------------

VPP_INLINE XImageNotBound :: XImageNotBound() :
    XRuntimeError ( "Image must be bound to device memory before creating a view" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XInvalidImage : public XRuntimeError
{
public:
    XInvalidImage();
};

// -----------------------------------------------------------------------------

VPP_INLINE XInvalidImage :: XInvalidImage() :
    XRuntimeError ( "Invalid image specified for a view" )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XInvalidBarrier : public XRuntimeError
{
public:
    XInvalidBarrier ( const std::string& msg );
};

// -----------------------------------------------------------------------------

VPP_INLINE XInvalidBarrier :: XInvalidBarrier ( const std::string& msg ) :
    XRuntimeError ( msg.c_str() )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class XMissingFeature : public XRuntimeError
{
public:
    XMissingFeature ( const std::string& featureName );

private:
    static std::string getMessageText ( const std::string& featureName );
};

// -----------------------------------------------------------------------------

VPP_INLINE XMissingFeature :: XMissingFeature ( const std::string& featureName ) :
    XRuntimeError ( getMessageText ( featureName ).c_str() )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE std::string XMissingFeature :: getMessageText ( const std::string& featureName )
{
    static const std::string s_msgpart1 ( "Feature '" );
    static const std::string s_msgpart2 ( "' is required but has not been enabled on the device." );
    return s_msgpart1 + featureName + s_msgpart2;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KExceptionThrower
{
public:
    VPP_INLINE void raiseVectorOverflow() const
    {
        throw XVectorOverflow();
    }

    VPP_INLINE void raisePushConstantTooBig() const
    {
        throw XPushConstantTooBig();
    }

    VPP_INLINE void raiseSharedMemoryLimitExceeded ( unsigned int r, unsigned int l ) const
    {
        throw XSharedMemoryLimitExceeded ( r, l );
    }

    VPP_INLINE void raiseImageNotBound() const
    {
        throw XImageNotBound();
    }

    VPP_INLINE void raiseInvalidImage() const
    {
        throw XInvalidImage();
    }

    VPP_INLINE void raiseMemoryAllocationError() const
    {
        throw XMemoryAllocationError();
    }

    VPP_INLINE void raiseUsageError ( const char* pMessage ) const
    {
        throw XUsageError ( pMessage );
    }
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPEXCEPTIONS_HPP
