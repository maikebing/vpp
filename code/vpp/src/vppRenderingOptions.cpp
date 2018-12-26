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

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppRenderingOptions.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

enum EContextFlags
{
    CFLAG_NotWithinRange = 1,
    CFLAG_FrontFacing = 2,
    CFLAG_StencilConditionTrue = 4,
    CFLAG_DepthConditionTrue = 8,

    CMASK_NotWithinRange = 0b1010101010101010,
    CMASK_FrontFacing = 0b1100110011001100,
    CMASK_BackFacing = 0b0011001100110011,
    CMASK_StencilConditionTrue = 0b1111000011110000,
    CMASK_DepthConditionTrue = 0b1111111100000000,
    CMASK_StencilConditionFalse = 0b0000111100001111,
    CMASK_DepthConditionFalse = 0b0000000011111111,
    CMASK_All = 0b1111111111111111
};

// -----------------------------------------------------------------------------

struct SRasterizationShaderOutcome
{
    SRasterizationShaderOutcome();
    VkCompareOp negatedDepthCondition() const;
    VkCompareOp negatedStencilCondition() const;

    static VkCompareOp negate ( VkCompareOp op );

    bool d_bDiscard;
    bool d_bDepthWrite;
    bool d_bDepthBoundsCheck;
    VkCompareOp d_depthCondition;
    VkCompareOp d_stencilCondition;
    VkStencilOp d_stencilOp;
    float d_lowerBound;
    float d_upperBound;
    unsigned int d_stencilRef;
    unsigned int d_stencilCompMask;
    unsigned int d_stencilWriteMask;
};

// -----------------------------------------------------------------------------

SRasterizationShaderOutcome :: SRasterizationShaderOutcome() :
    d_bDiscard ( false ),
    d_bDepthWrite ( false ),
    d_bDepthBoundsCheck ( false ),
    d_depthCondition ( VK_COMPARE_OP_ALWAYS ),
    d_stencilCondition ( VK_COMPARE_OP_ALWAYS ),
    d_stencilOp ( VK_STENCIL_OP_KEEP ),
    d_lowerBound ( 0.0f ),
    d_upperBound ( 1.0f ),
    d_stencilRef ( 0 ),
    d_stencilCompMask ( 0xFFFFFFFFu ),
    d_stencilWriteMask ( 0xFFFFFFFFu )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCompareOp SRasterizationShaderOutcome :: negate ( VkCompareOp op )
{
    switch ( op )
    {
        case VK_COMPARE_OP_NEVER: return VK_COMPARE_OP_ALWAYS;
        case VK_COMPARE_OP_LESS: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case VK_COMPARE_OP_EQUAL: return VK_COMPARE_OP_NOT_EQUAL;
        case VK_COMPARE_OP_LESS_OR_EQUAL: return VK_COMPARE_OP_GREATER;
        case VK_COMPARE_OP_GREATER: return VK_COMPARE_OP_LESS_OR_EQUAL;
        case VK_COMPARE_OP_NOT_EQUAL: return VK_COMPARE_OP_EQUAL;
        case VK_COMPARE_OP_GREATER_OR_EQUAL: return VK_COMPARE_OP_LESS;
        case VK_COMPARE_OP_ALWAYS: return VK_COMPARE_OP_NEVER;
        default: return VK_COMPARE_OP_ALWAYS;
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCompareOp SRasterizationShaderOutcome :: negatedDepthCondition() const
{
    return negate ( d_depthCondition );
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCompareOp SRasterizationShaderOutcome :: negatedStencilCondition() const
{
    return negate ( d_stencilCondition );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KRasterizationShaderTranslator
{
public:
    KRasterizationShaderTranslator ( size_t contextFlags );
    ~KRasterizationShaderTranslator();

    bool isNotWithinRange() const;
    bool isFrontFacing() const;
    bool isStencilConditionTrue() const;
    bool isDepthConditionTrue() const;

    void discard();
    void setDepthCondition ( VkCompareOp cond );
    void setStencilCondition ( VkCompareOp cond, unsigned int ref );
    void setStencilCompareMask ( unsigned int mask );

    const SRasterizationShaderOutcome& getOutcome() const;

public:
    static KRasterizationShaderTranslator* get();

private:
    static thread_local KRasterizationShaderTranslator* s_pThis;

private:
    friend class DepthBuffer;
    friend class StencilBuffer;
    friend class MaskedStencilBuffer;

    size_t d_contextFlags;
    SRasterizationShaderOutcome d_outcome;
};

// -----------------------------------------------------------------------------

thread_local KRasterizationShaderTranslator* KRasterizationShaderTranslator :: s_pThis = 0;

// -----------------------------------------------------------------------------

VPP_INLINE KRasterizationShaderTranslator ::
    KRasterizationShaderTranslator ( size_t contextFlags ) :
        d_contextFlags ( contextFlags )
{
    s_pThis = this;
}

// -----------------------------------------------------------------------------

VPP_INLINE KRasterizationShaderTranslator :: ~KRasterizationShaderTranslator()
{
    s_pThis = 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE KRasterizationShaderTranslator* KRasterizationShaderTranslator :: get()
{
    return s_pThis;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KRasterizationShaderTranslator :: isNotWithinRange() const
{
    return ( d_contextFlags & CFLAG_NotWithinRange ) != 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KRasterizationShaderTranslator :: isFrontFacing() const
{
    return ( d_contextFlags & CFLAG_FrontFacing ) != 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KRasterizationShaderTranslator :: isStencilConditionTrue() const
{
    return ( d_contextFlags & CFLAG_StencilConditionTrue ) != 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KRasterizationShaderTranslator :: isDepthConditionTrue() const
{
    return ( d_contextFlags & CFLAG_DepthConditionTrue ) != 0;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KRasterizationShaderTranslator :: discard()
{
    d_outcome.d_bDiscard = true;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KRasterizationShaderTranslator :: setDepthCondition ( VkCompareOp cond )
{
    if ( d_outcome.d_depthCondition != VK_COMPARE_OP_ALWAYS
         && cond != d_outcome.d_depthCondition )
    {
        throw XRuntimeError (
            "All depth conditions within single shader must be the same." );
    }

    d_outcome.d_depthCondition = cond;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KRasterizationShaderTranslator :: setStencilCondition (
    VkCompareOp cond, unsigned int ref )
{
    if ( d_outcome.d_stencilCondition != VK_COMPARE_OP_ALWAYS )
    {
        if ( cond != d_outcome.d_stencilCondition )
            throw XRuntimeError (
                "All stencil conditions within front/back branch of the shader must be the same." );
        if ( ref != d_outcome.d_stencilRef )
            throw XRuntimeError (
                "All stencil reference values within front/back branch of the shader must be the same." );
    }

    d_outcome.d_stencilCondition = cond;
    d_outcome.d_stencilRef = ref;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KRasterizationShaderTranslator :: setStencilCompareMask ( unsigned int mask )
{
    if ( d_outcome.d_stencilCompMask != 0xFFFFFFFFu
         && mask != d_outcome.d_stencilCompMask )
    {
        throw XRuntimeError (
            "All stencil compare masks within front/back branch of the shader must be the same." );
    }

    d_outcome.d_stencilCompMask = mask;
}

// -----------------------------------------------------------------------------

VPP_INLINE const SRasterizationShaderOutcome& KRasterizationShaderTranslator :: getOutcome() const
{
    return d_outcome;
}

// -----------------------------------------------------------------------------

void Discard()
{
    KRasterizationShaderTranslator::get()->discard();
}

// -----------------------------------------------------------------------------

bool Pixel :: isFrontFacing() const
{
    return KRasterizationShaderTranslator::get()->isFrontFacing();
}

// -----------------------------------------------------------------------------

const DepthBuffer& DepthBuffer :: operator= ( PixelDepth )
{
    KRasterizationShaderTranslator::get()->d_outcome.d_bDepthWrite = true;
    return *this;
}

// -----------------------------------------------------------------------------

bool DepthBuffer :: isNotWithinRange ( float lowerBound, float upperBound ) const
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();

    if ( pTranslator->d_outcome.d_bDepthBoundsCheck )
        throw XRuntimeError (
            "Depth bounds test can be used only once within single shader." );

    pTranslator->d_outcome.d_lowerBound = lowerBound;
    pTranslator->d_outcome.d_upperBound = upperBound;
    pTranslator->d_outcome.d_bDepthBoundsCheck = true;
    return pTranslator->isNotWithinRange();
}

// -----------------------------------------------------------------------------

bool operator< ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_LESS );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator> ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_GREATER );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator<= ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_LESS_OR_EQUAL );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator>= ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_GREATER_OR_EQUAL );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator== ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_EQUAL );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator!= ( PixelDepth, DepthBuffer )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setDepthCondition ( VK_COMPARE_OP_NOT_EQUAL );
    return pTranslator->isDepthConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator< ( DepthBuffer b, PixelDepth d )
{
    return operator> ( d, b );
}

// -----------------------------------------------------------------------------

bool operator> ( DepthBuffer b, PixelDepth d )
{
    return operator< ( d, b );
}

// -----------------------------------------------------------------------------

bool operator<= ( DepthBuffer b, PixelDepth d )
{
    return operator>= ( d, b );
}

// -----------------------------------------------------------------------------

bool operator>= ( DepthBuffer b, PixelDepth d )
{
    return operator<= ( d, b );
}

// -----------------------------------------------------------------------------

bool operator== ( DepthBuffer b , PixelDepth d )
{
    return operator== ( d, b );
}

// -----------------------------------------------------------------------------

bool operator!= ( DepthBuffer b, PixelDepth d )
{
    return operator!= ( d, b );
}

// -----------------------------------------------------------------------------

const StencilBuffer& StencilBuffer :: operator= ( unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();

    if ( value == 0 )
        pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_ZERO;
    else
    {
        if ( value != pTranslator->d_outcome.d_stencilRef
             && pTranslator->d_outcome.d_stencilCondition != VK_COMPARE_OP_NEVER
             && pTranslator->d_outcome.d_stencilCondition != VK_COMPARE_OP_ALWAYS )
        {
            // raise error
        }

        pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_REPLACE;
        pTranslator->d_outcome.d_stencilRef = value;
    }

    return *this;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: invertBits()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INVERT;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: operator++()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: inc()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: operator--()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: dec()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: incSaturate()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
}

// -----------------------------------------------------------------------------

void StencilBuffer :: decSaturate()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
}

// -----------------------------------------------------------------------------

MaskedStencilBuffer StencilBuffer :: operator& ( unsigned int mask )
{
    return MaskedStencilBuffer ( mask );
}

// -----------------------------------------------------------------------------

bool operator< ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_LESS, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator> ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_GREATER, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator<= ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_LESS_OR_EQUAL, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator>= ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_GREATER_OR_EQUAL, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator== ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_EQUAL, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator!= ( StencilBuffer, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_NOT_EQUAL, value );
    pTranslator->setStencilCompareMask ( 0xFFFFFFFFu );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator< ( unsigned int value, StencilBuffer b )
{
    return operator> ( b, value );
}

// -----------------------------------------------------------------------------

bool operator> ( unsigned int value, StencilBuffer b )
{
    return operator< ( b, value );
}

// -----------------------------------------------------------------------------

bool operator<= ( unsigned int value, StencilBuffer b )
{
    return operator>= ( b, value );
}

// -----------------------------------------------------------------------------

bool operator>= ( unsigned int value, StencilBuffer b )
{
    return operator<= ( b, value );
}

// -----------------------------------------------------------------------------

bool operator== ( unsigned int value, StencilBuffer b )
{
    return operator== ( b, value );
}

// -----------------------------------------------------------------------------

bool operator!= ( unsigned int value, StencilBuffer b )
{
    return operator!= ( b, value );
}

// -----------------------------------------------------------------------------

const MaskedStencilBuffer& MaskedStencilBuffer :: operator= ( unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();

    if ( value == 0 )
    {
        pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_ZERO;
        pTranslator->d_outcome.d_stencilWriteMask = d_mask;
    }
    else
    {
        if ( value != pTranslator->d_outcome.d_stencilRef
             && pTranslator->d_outcome.d_stencilCondition != VK_COMPARE_OP_NEVER
             && pTranslator->d_outcome.d_stencilCondition != VK_COMPARE_OP_ALWAYS )
        {
            // raise error
        }

        pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_REPLACE;
        pTranslator->d_outcome.d_stencilWriteMask = d_mask;
    }

    return *this;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: invertBits()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INVERT;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: operator++()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: inc()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: operator--()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: dec()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: incSaturate()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

void MaskedStencilBuffer :: decSaturate()
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->d_outcome.d_stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    pTranslator->d_outcome.d_stencilWriteMask = d_mask;
}

// -----------------------------------------------------------------------------

bool operator< ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_LESS, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator> ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_GREATER, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator<= ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_LESS_OR_EQUAL, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator>= ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_GREATER_OR_EQUAL, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator== ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_EQUAL, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator!= ( MaskedStencilBuffer b, unsigned int value )
{
    KRasterizationShaderTranslator* pTranslator = KRasterizationShaderTranslator::get();
    pTranslator->setStencilCondition ( VK_COMPARE_OP_NOT_EQUAL, value );
    pTranslator->setStencilCompareMask ( b.getMask() );
    return pTranslator->isStencilConditionTrue();
}

// -----------------------------------------------------------------------------

bool operator< ( unsigned int value, MaskedStencilBuffer b )
{
    return operator> ( b, value );
}

// -----------------------------------------------------------------------------

bool operator> ( unsigned int value, MaskedStencilBuffer b )
{
    return operator< ( b, value );
}

// -----------------------------------------------------------------------------

bool operator<= ( unsigned int value, MaskedStencilBuffer b )
{
    return operator>= ( b, value );
}

// -----------------------------------------------------------------------------

bool operator>= ( unsigned int value, MaskedStencilBuffer b )
{
    return operator<= ( b, value );
}

// -----------------------------------------------------------------------------

bool operator== ( unsigned int value, MaskedStencilBuffer b )
{
    return operator== ( b, value );
}

// -----------------------------------------------------------------------------

bool operator!= ( unsigned int value, MaskedStencilBuffer b )
{
    return operator!= ( b, value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

RenderingOptionsImpl :: RenderingOptionsImpl() :
    d_polygonMode ( VK_POLYGON_MODE_FILL ),
    d_cullMode ( VK_CULL_MODE_BACK_BIT ),
    d_frontFace ( VK_FRONT_FACE_COUNTER_CLOCKWISE ),
    d_rasterizationSamples ( VK_SAMPLE_COUNT_1_BIT ),
    d_lineWidth ( 1.0f ),
    d_depthBiasConstantFactor ( 0 ),
    d_depthBiasClamp ( 0 ),
    d_depthBiasSlopeFactor ( 0 ),
    d_sampleShadingAmount ( 1.0f ),
    d_minDepthBounds ( 0.0f ),
    d_maxDepthBounds ( 1.0f ),
    d_bEnableDepthClamp ( false ),
    d_bEnableRasterizerDiscard ( false ),
    d_bEnableDepthBias ( false ),
    d_bEnableSampleShading ( false ),
    d_bEnableAlphaToCoverage ( false ),
    d_bEnableResetAlphaToOne ( false ),
    d_bEnableDepthTest ( false ),
    d_bEnableDepthWrite ( true ),
    d_bEnableDepthBoundsTest ( false ),
    d_bEnableStencilTest ( false ),
    d_bModifyCoverageMask ( false ),
    d_depthCompareOperator ( VK_COMPARE_OP_LESS_OR_EQUAL )
{
    d_modifiedCoverageMask [ 0 ] = 0xFFFFFFFFu;
    d_modifiedCoverageMask [ 1 ] = 0xFFFFFFFFu;

    d_frontFacingStencilCfg.failOp = VK_STENCIL_OP_KEEP;
    d_frontFacingStencilCfg.passOp = VK_STENCIL_OP_KEEP;
    d_frontFacingStencilCfg.depthFailOp = VK_STENCIL_OP_KEEP;
    d_frontFacingStencilCfg.compareOp = VK_COMPARE_OP_NEVER;
    d_frontFacingStencilCfg.compareMask = 0;
    d_frontFacingStencilCfg.writeMask = 0;
    d_frontFacingStencilCfg.reference = 0;

    d_backFacingStencilCfg = d_frontFacingStencilCfg;
}

// -----------------------------------------------------------------------------

RenderingOptionsImpl :: RenderingOptionsImpl ( const RenderingOptionsImpl& rhs ) :
    d_polygonMode ( rhs.d_polygonMode ),
    d_cullMode ( rhs.d_cullMode ),
    d_frontFace ( rhs.d_frontFace ),
    d_rasterizationSamples ( rhs.d_rasterizationSamples ),
    d_lineWidth ( rhs.d_lineWidth ),
    d_depthBiasConstantFactor ( rhs.d_depthBiasConstantFactor ),
    d_depthBiasClamp ( rhs.d_depthBiasClamp ),
    d_depthBiasSlopeFactor ( rhs.d_depthBiasSlopeFactor ),
    d_sampleShadingAmount ( rhs.d_sampleShadingAmount ),
    d_minDepthBounds ( rhs.d_minDepthBounds ),
    d_maxDepthBounds ( rhs.d_maxDepthBounds ),
    d_bEnableDepthClamp ( rhs.d_bEnableDepthClamp ),
    d_bEnableRasterizerDiscard ( rhs.d_bEnableRasterizerDiscard ),
    d_bEnableDepthBias ( rhs.d_bEnableDepthBias ),
    d_bEnableSampleShading ( rhs.d_bEnableSampleShading ),
    d_bEnableAlphaToCoverage ( rhs.d_bEnableAlphaToCoverage ),
    d_bEnableResetAlphaToOne ( rhs.d_bEnableResetAlphaToOne ),
    d_bEnableDepthTest ( rhs.d_bEnableDepthTest ),
    d_bEnableDepthWrite ( rhs.d_bEnableDepthWrite ),
    d_bEnableDepthBoundsTest ( rhs.d_bEnableDepthBoundsTest ),
    d_bEnableStencilTest ( rhs.d_bEnableStencilTest ),
    d_bModifyCoverageMask ( rhs.d_bModifyCoverageMask ),
    d_depthCompareOperator ( rhs.d_depthCompareOperator ),
    d_frontFacingStencilCfg ( rhs.d_frontFacingStencilCfg ),
    d_backFacingStencilCfg ( rhs.d_backFacingStencilCfg ),
    d_viewports ( rhs.d_viewports ),
    d_scissors ( rhs.d_scissors )
{
    d_modifiedCoverageMask [ 0 ] = rhs.d_modifiedCoverageMask [ 0 ];
    d_modifiedCoverageMask [ 1 ] = rhs.d_modifiedCoverageMask [ 1 ];
}

// -----------------------------------------------------------------------------

void RenderingOptionsImpl :: initViewportInfo (
    VkPipelineViewportStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->viewportCount = static_cast< std::uint32_t >( d_viewports.size() );
    pDest->scissorCount = static_cast< std::uint32_t >( d_scissors.size() );
    pDest->pViewports = & d_viewports [ 0 ];
    pDest->pScissors = & d_scissors [ 0 ];
}

// -----------------------------------------------------------------------------

void RenderingOptionsImpl :: initRasterizationInfo (
    VkPipelineRasterizationStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->depthClampEnable = d_bEnableDepthClamp;
    pDest->rasterizerDiscardEnable = d_bEnableRasterizerDiscard;
    pDest->polygonMode = d_polygonMode;
    pDest->cullMode = d_cullMode;
    pDest->frontFace = d_frontFace;
    pDest->depthBiasEnable = d_bEnableDepthBias;
    pDest->depthBiasConstantFactor = d_depthBiasConstantFactor;
    pDest->depthBiasClamp = d_depthBiasClamp;
    pDest->depthBiasSlopeFactor = d_depthBiasSlopeFactor;
    pDest->lineWidth = d_lineWidth;
}

// -----------------------------------------------------------------------------

void RenderingOptionsImpl :: initMultisampleInfo (
    VkPipelineMultisampleStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->rasterizationSamples = d_rasterizationSamples;
    pDest->sampleShadingEnable = d_bEnableSampleShading;
    pDest->minSampleShading = d_sampleShadingAmount;

    if ( d_bModifyCoverageMask )
        pDest->pSampleMask = d_modifiedCoverageMask;
    else
        pDest->pSampleMask = 0;

    pDest->alphaToCoverageEnable = d_bEnableAlphaToCoverage;
    pDest->alphaToOneEnable = d_bEnableResetAlphaToOne;
}

// -----------------------------------------------------------------------------

void RenderingOptionsImpl :: initDepthStencilInfo (
    VkPipelineDepthStencilStateCreateInfo* pDest ) const
{
    pDest->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pDest->pNext = 0;
    pDest->flags = 0;
    pDest->depthTestEnable = d_bEnableDepthTest;
    pDest->depthWriteEnable = d_bEnableDepthWrite;
    pDest->depthCompareOp = d_depthCompareOperator;
    pDest->depthBoundsTestEnable = d_bEnableDepthBoundsTest;
    pDest->stencilTestEnable = d_bEnableStencilTest;
    pDest->front = d_frontFacingStencilCfg;
    pDest->back = d_backFacingStencilCfg;
    pDest->minDepthBounds = d_minDepthBounds;
    pDest->maxDepthBounds = d_maxDepthBounds;
}

// -----------------------------------------------------------------------------

void RenderingOptions :: addViewport ( const Viewport& vp )
{
    copyOnWrite();
    get()->d_viewports.push_back ( vp );
    get()->d_scissors.push_back ( vp.d_scissor );
}

// -----------------------------------------------------------------------------

void RenderingOptions :: operator<< ( FRasterShader&& fRasterShader )
{
    SRasterizationShaderOutcome outcomes [ 16 ];

    RenderingOptionsImpl* impl = get();

    size_t discardMask = 0; 
    size_t depthWriteMask = 0; 
    size_t stencilWriteMask = 0;
    size_t depthBoundsCheckMask = 0;

    for ( size_t iCfg = 0; iCfg != 16; ++iCfg )
    {
        KRasterizationShaderTranslator translator ( iCfg );
        fRasterShader();
        outcomes [ iCfg ] = translator.getOutcome();

        const size_t m = 1ull << iCfg;

        if ( outcomes [ iCfg ].d_bDiscard )
            discardMask |= m;
        if ( outcomes [ iCfg ].d_bDepthWrite )
            depthWriteMask |= m;
        if ( outcomes [ iCfg ].d_bDepthBoundsCheck )
            depthBoundsCheckMask |= m;
        if ( outcomes [ iCfg ].d_stencilOp != VK_STENCIL_OP_KEEP )
            stencilWriteMask |= m;
    }

    if ( discardMask & depthWriteMask )
        throw XRuntimeError (
            "Can't write to depth buffer and discard at the same time." );

    VkCompareOp commonCmpOp = VK_COMPARE_OP_ALWAYS;

    for ( size_t iCfg = 0; iCfg != 16; ++iCfg )
    {
        if ( outcomes [ iCfg ].d_depthCondition != VK_COMPARE_OP_ALWAYS )
        {
            if ( commonCmpOp == VK_COMPARE_OP_ALWAYS )
                commonCmpOp = outcomes [ iCfg ].d_depthCondition;
            else if ( commonCmpOp != outcomes [ iCfg ].d_depthCondition )
                throw XRuntimeError (
                    "All depth conditions within single shader must be the same." );
        }
    }

    bool bFrontCulling = false;
    bool bBackCulling = false;
    bool bDepthTestEnable = false;
    bool bNegatedDepthTestEnable = false;

    if ( ( ~stencilWriteMask & discardMask & CMASK_FrontFacing ) == CMASK_FrontFacing )
        bFrontCulling = true;

    if ( ( ~stencilWriteMask & discardMask & CMASK_BackFacing ) == CMASK_BackFacing )
        bBackCulling = true;

    const bool bDiscardAll = ( bFrontCulling && bBackCulling );

    impl->d_bEnableDepthBoundsTest = ( depthBoundsCheckMask != 0 );

    if ( impl->d_bEnableDepthBoundsTest )
    {
        if ( ( discardMask & CMASK_NotWithinRange ) != CMASK_NotWithinRange )
            throw XRuntimeError (
                "Pixels with depth buffer values outside bounds must be discarded."
            );

        if ( stencilWriteMask & CMASK_NotWithinRange )
            throw XRuntimeError (
                "Pixels with depth buffer values outside bounds can't be stencil-written."
            );

        const SRasterizationShaderOutcome* pLast = 0;

        for ( size_t iCfg = 0; iCfg != 16; ++iCfg )
        {
            const SRasterizationShaderOutcome& outcome = outcomes [ iCfg ];

            if ( ! pLast )
                pLast = & outcome;
            else if ( outcome.d_lowerBound != pLast->d_lowerBound
                        || outcome.d_upperBound != pLast->d_upperBound )
            {
                throw XRuntimeError (
                    "Multiple values of depth bounds within single shader are not allowed."
                );
            }
        }

        impl->d_minDepthBounds = outcomes [ 1 ].d_lowerBound;
        impl->d_maxDepthBounds = outcomes [ 1 ].d_upperBound;
    }

    if ( ( discardMask & CMASK_DepthConditionFalse ) == CMASK_DepthConditionFalse )
        bDepthTestEnable = true;
    else if ( ( discardMask & CMASK_DepthConditionTrue ) == CMASK_DepthConditionTrue )
        bNegatedDepthTestEnable = true;

    if ( bDiscardAll )
    {
        impl->d_bEnableDepthBoundsTest = false;
        bDepthTestEnable = false;
        bNegatedDepthTestEnable = false;
    }

    impl->d_bEnableDepthTest = ( bDepthTestEnable || bNegatedDepthTestEnable );

    if ( bDepthTestEnable )
        impl->d_depthCompareOperator = outcomes [ 0 ].d_depthCondition;

    if ( bNegatedDepthTestEnable )
        impl->d_depthCompareOperator = outcomes [ 0 ].negatedDepthCondition();

    if ( bFrontCulling )
    {
        if ( bBackCulling )
            impl->d_cullMode = VK_CULL_MODE_FRONT_AND_BACK;
        else
            impl->d_cullMode = VK_CULL_MODE_FRONT_BIT;
    }
    else
    {
        if ( bBackCulling )
            impl->d_cullMode = VK_CULL_MODE_BACK_BIT;
        else
            impl->d_cullMode = VK_CULL_MODE_NONE;
    }

    if ( ( discardMask ^ ( discardMask >> 4 ) ) & CMASK_StencilConditionFalse )
        impl->d_bEnableStencilTest = true;

    // case when condition=true is really fail
    bool bNegateStencilTestFront = false;
    bool bNegateStencilTestBack = false;

    if ( impl->d_bEnableStencilTest )
    {
        if ( ( discardMask & CMASK_StencilConditionTrue & CMASK_FrontFacing )
                == ( CMASK_StencilConditionTrue & CMASK_FrontFacing ) )
        {
            bNegateStencilTestFront = true;
        }

        if ( ( discardMask & CMASK_StencilConditionTrue & CMASK_BackFacing )
                == ( CMASK_StencilConditionTrue & CMASK_BackFacing ) )
        {
            bNegateStencilTestBack = true;
        }
    }

    bool bFakeStencilTest = false;

    if ( ! impl->d_bEnableStencilTest && stencilWriteMask )
    {
        impl->d_bEnableStencilTest = true;
        bFakeStencilTest = true;
        impl->d_frontFacingStencilCfg.compareOp = VK_COMPARE_OP_ALWAYS;
        impl->d_backFacingStencilCfg.compareOp = VK_COMPARE_OP_ALWAYS;
    }

    impl->d_frontFacingStencilCfg.passOp = VK_STENCIL_OP_KEEP;
    impl->d_frontFacingStencilCfg.failOp = VK_STENCIL_OP_KEEP;
    impl->d_frontFacingStencilCfg.depthFailOp = VK_STENCIL_OP_KEEP;
    impl->d_backFacingStencilCfg.passOp = VK_STENCIL_OP_KEEP;
    impl->d_backFacingStencilCfg.failOp = VK_STENCIL_OP_KEEP;
    impl->d_backFacingStencilCfg.depthFailOp = VK_STENCIL_OP_KEEP;

    if ( impl->d_bEnableStencilTest )
    {
        if ( outcomes [ 4 ].d_stencilRef != outcomes [ 0 ].d_stencilRef
             || outcomes [ 8 ].d_stencilRef != outcomes [ 0 ].d_stencilRef
             || outcomes [ 12 ].d_stencilRef != outcomes [ 0 ].d_stencilRef
             || outcomes [ 6 ].d_stencilRef != outcomes [ 2 ].d_stencilRef
             || outcomes [ 10 ].d_stencilRef != outcomes [ 2 ].d_stencilRef
             || outcomes [ 14 ].d_stencilRef != outcomes [ 2 ].d_stencilRef )
        {
            throw XRuntimeError (
                "Reference value for stencil test must be the same in entire front/back branch."
            );
        }

        if ( outcomes [ 4 ].d_stencilCompMask != outcomes [ 0 ].d_stencilCompMask
             || outcomes [ 8 ].d_stencilCompMask != outcomes [ 0 ].d_stencilCompMask
             || outcomes [ 12 ].d_stencilCompMask != outcomes [ 0 ].d_stencilCompMask
             || outcomes [ 6 ].d_stencilCompMask != outcomes [ 2 ].d_stencilCompMask
             || outcomes [ 10 ].d_stencilCompMask != outcomes [ 2 ].d_stencilCompMask
             || outcomes [ 14 ].d_stencilCompMask != outcomes [ 2 ].d_stencilCompMask )
        {
            throw XRuntimeError (
                "Compare mask for stencil test must be the same in entire front/back branch."
            );
        }

        for ( size_t iCfg = 0; iCfg != 16; ++iCfg )
        { 
            const SRasterizationShaderOutcome& outcome = outcomes [ iCfg ];

            if ( iCfg & CFLAG_NotWithinRange )
                continue;

            VkStencilOpState* pDestStencilState = 0;
            bool bNegateStencilTest = false;

            if ( iCfg & CFLAG_FrontFacing )
            {
                pDestStencilState = & impl->d_frontFacingStencilCfg;
                bNegateStencilTest = bNegateStencilTestFront;
            }
            else
            {
                pDestStencilState = & impl->d_backFacingStencilCfg;
                bNegateStencilTest = bNegateStencilTestBack;
            }

            if ( ! bFakeStencilTest )
                pDestStencilState->compareOp = bNegateStencilTest ?
                    outcome.negatedStencilCondition()
                    : outcome.d_stencilCondition;
            else
                pDestStencilState->compareOp = VK_COMPARE_OP_ALWAYS;

            pDestStencilState->reference = outcome.d_stencilRef;
            pDestStencilState->compareMask = outcome.d_stencilCompMask;

            if ( outcome.d_stencilOp != VK_STENCIL_OP_KEEP )
                pDestStencilState->writeMask = outcome.d_stencilWriteMask;

            bool bDepthTestFail = impl->d_bEnableDepthTest && (
                ( ( iCfg & CFLAG_DepthConditionTrue ) && bNegatedDepthTestEnable )
                || ( ! ( iCfg & CFLAG_DepthConditionTrue ) && bDepthTestEnable )
            );

            if ( iCfg & CFLAG_StencilConditionTrue )
            {
                if ( bNegateStencilTest )
                    pDestStencilState->failOp = outcome.d_stencilOp;
                else if ( ! bDepthTestFail ) 
                    pDestStencilState->passOp = outcome.d_stencilOp;
                else
                    pDestStencilState->depthFailOp = outcome.d_stencilOp;
            }
            else if ( ! bFakeStencilTest )
            {
                if ( bNegateStencilTest )
                {
                    if ( ! bDepthTestFail ) 
                        pDestStencilState->passOp = outcome.d_stencilOp;
                    else
                        pDestStencilState->depthFailOp = outcome.d_stencilOp;
                }
                else 
                    pDestStencilState->failOp = outcome.d_stencilOp;
            }
        }
    }

    impl->d_bEnableDepthWrite =
        ( impl->d_bEnableDepthTest
          && ( ~discardMask & depthWriteMask ) == ( ~discardMask & CMASK_All ) );
    
    if ( ! impl->d_bEnableDepthTest && depthWriteMask )
    {
        // depth write without depth test
        // make depth test which always passes
        impl->d_bEnableDepthTest = true;
        impl->d_depthCompareOperator = VK_COMPARE_OP_ALWAYS;
        impl->d_bEnableDepthWrite = true;
    }
}

// -----------------------------------------------------------------------------

void RenderingOptions :: copyOnWrite()
{
    TCloneableSharedReference< RenderingOptionsImpl >::copyOnWrite();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
