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

#ifndef INC_VPPSAMPLER_HPP
#define INC_VPPSAMPLER_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct SNormalizedSampler
{
    VPP_DLLAPI SNormalizedSampler ( float maxLod = 1.0f );
    VPP_DLLAPI bool operator< ( const SNormalizedSampler& rhs ) const;

    unsigned int addressModeU : 3;
    unsigned int addressModeV : 3;
    unsigned int addressModeW : 3;
    unsigned int borderColor : 3;
    unsigned int compareOp : 3;
    unsigned int compare : 1;

    unsigned int magFilterMode : 2;
    unsigned int minFilterMode : 2;
    unsigned int mipMapMode : 2;
    unsigned int anisotropy : 1;

    float mipLodBias;
    float maxAnisotropy;
    float minLod;
    float maxLod;
};

// -----------------------------------------------------------------------------

struct SUnnormalizedSampler
{
    /*
        minFilter and magFilter must be equal.
        mipmapMode must be VK_SAMPLER_MIPMAP_MODE_NEAREST.
        minLod and maxLod must be zero.
        addressModeU and addressModeV must each be either VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE or VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER.
        anisotropyEnable must be VK_FALSE.
        compareEnable must be VK_FALSE.
    */

    VPP_DLLAPI SUnnormalizedSampler();
    VPP_DLLAPI bool operator< ( const SUnnormalizedSampler& rhs ) const;

    unsigned int clampToBorderU : 1;
    unsigned int clampToBorderV : 1;
    unsigned int filterMode : 2;
    unsigned int borderColor : 3;

    float mipLodBias;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class SamplerImpl;

// -----------------------------------------------------------------------------

class Sampler : public TSharedReference< SamplerImpl >
{
public:
    Sampler();
    Sampler ( const Device& hDevice, const SNormalizedSampler& samplerInfo );
    Sampler ( const Device& hDevice, const SUnnormalizedSampler& samplerInfo );
    ~Sampler();

    static const bool isUnnormalized = false;

    VkSampler handle() const;
};

// -----------------------------------------------------------------------------

class SamplerImpl : public TSharedObject< SamplerImpl >
{
public:
    VPP_DLLAPI SamplerImpl ( const Device& hDevice, const SNormalizedSampler& samplerInfo );
    VPP_DLLAPI SamplerImpl ( const Device& hDevice, const SUnnormalizedSampler& samplerInfo );
    VPP_DLLAPI ~SamplerImpl();

    bool compareObjects ( const SamplerImpl* pRHS ) const;

private:
    friend class Sampler;
    Device d_hDevice;
    VkSampler d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE bool SamplerImpl :: compareObjects ( const SamplerImpl* pRHS ) const
{
    return this < pRHS;
}

// -----------------------------------------------------------------------------

VPP_INLINE Sampler :: Sampler()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Sampler :: Sampler ( const Device& hDevice, const SNormalizedSampler& samplerInfo ) :
    TSharedReference< SamplerImpl > ( new SamplerImpl ( hDevice, samplerInfo ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Sampler :: Sampler ( const Device& hDevice, const SUnnormalizedSampler& samplerInfo ) :
    TSharedReference< SamplerImpl > ( new SamplerImpl ( hDevice, samplerInfo ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Sampler :: ~Sampler()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkSampler Sampler :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class NormalizedSampler : public Sampler
{
public:
    VPP_INLINE NormalizedSampler()
    {
    }

    VPP_INLINE NormalizedSampler (
        const Device& hDevice, const SNormalizedSampler& samplerInfo = SNormalizedSampler() ) :
            Sampler ( hDevice, samplerInfo )
    {}

    static const bool isUnnormalized = false;
};

// -----------------------------------------------------------------------------

class UnnormalizedSampler : public Sampler
{
public:
    VPP_INLINE UnnormalizedSampler()
    {
    }

    VPP_INLINE UnnormalizedSampler (
        const Device& hDevice, const SUnnormalizedSampler& samplerInfo = SUnnormalizedSampler() ) :
            Sampler ( hDevice, samplerInfo )
    {}

    static const bool isUnnormalized = true;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSAMPLER_HPP
