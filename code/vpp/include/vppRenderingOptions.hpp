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

#ifndef INC_VPPRENDERINGOPTIONS_HPP
#define INC_VPPRENDERINGOPTIONS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPVIEWPORT_HPP
#include "vppViewport.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class RenderingOptionsImpl;

// -----------------------------------------------------------------------------
// pseudoshader dla depth/stencil opow
// -----------------------------------------------------------------------------

void Discard();

// -----------------------------------------------------------------------------

class PixelDepth
{
};

// -----------------------------------------------------------------------------

class Pixel
{
public:
    VPP_INLINE PixelDepth depth() const
    {
        return PixelDepth();
    }

    bool isFrontFacing() const;
};

// -----------------------------------------------------------------------------

class DepthBuffer
{
public:
    const DepthBuffer& operator= ( PixelDepth );
    bool isNotWithinRange ( float lowerBound, float upperBound ) const;
};

bool operator< ( DepthBuffer, PixelDepth );
bool operator> ( DepthBuffer, PixelDepth );
bool operator<= ( DepthBuffer, PixelDepth );
bool operator>= ( DepthBuffer, PixelDepth );
bool operator== ( DepthBuffer, PixelDepth );
bool operator!= ( DepthBuffer, PixelDepth );
bool operator< ( PixelDepth, DepthBuffer );
bool operator> ( PixelDepth, DepthBuffer );
bool operator<= ( PixelDepth, DepthBuffer );
bool operator>= ( PixelDepth, DepthBuffer );
bool operator== ( PixelDepth, DepthBuffer );
bool operator!= ( PixelDepth, DepthBuffer );

// -----------------------------------------------------------------------------

class MaskedStencilBuffer
{
public:
    VPP_INLINE MaskedStencilBuffer ( std::uint32_t mask ) :
        d_mask ( mask )
    {}

    VPP_INLINE std::uint32_t getMask() const
    {
        return d_mask;
    }

    const MaskedStencilBuffer& operator= ( unsigned int );

    void invertBits();
    void operator++();
    void inc();
    void operator--();
    void dec();
    void incSaturate();
    void decSaturate();

private:
    std::uint32_t d_mask;
};

bool operator< ( MaskedStencilBuffer, unsigned int );
bool operator> ( MaskedStencilBuffer, unsigned int );
bool operator<= ( MaskedStencilBuffer, unsigned int );
bool operator>= ( MaskedStencilBuffer, unsigned int );
bool operator== ( MaskedStencilBuffer, unsigned int );
bool operator!= ( MaskedStencilBuffer, unsigned int );
bool operator< ( unsigned int, MaskedStencilBuffer );
bool operator> ( unsigned int, MaskedStencilBuffer );
bool operator<= ( unsigned int, MaskedStencilBuffer );
bool operator>= ( unsigned int, MaskedStencilBuffer );
bool operator== ( unsigned int, MaskedStencilBuffer );
bool operator!= ( unsigned int, MaskedStencilBuffer );

// -----------------------------------------------------------------------------

class StencilBuffer
{
public:
    const StencilBuffer& operator= ( unsigned int );

    void invertBits();
    void operator++();
    void inc();
    void operator--();
    void dec();
    void incSaturate();
    void decSaturate();
    MaskedStencilBuffer operator& ( unsigned int );
};

bool operator< ( StencilBuffer, unsigned int );
bool operator> ( StencilBuffer, unsigned int );
bool operator<= ( StencilBuffer, unsigned int );
bool operator>= ( StencilBuffer, unsigned int );
bool operator== ( StencilBuffer, unsigned int );
bool operator!= ( StencilBuffer, unsigned int );
bool operator< ( unsigned int, StencilBuffer );
bool operator> ( unsigned int, StencilBuffer );
bool operator<= ( unsigned int, StencilBuffer );
bool operator>= ( unsigned int, StencilBuffer );
bool operator== ( unsigned int, StencilBuffer );
bool operator!= ( unsigned int, StencilBuffer );

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class RenderingOptions : public TCloneableSharedReference< RenderingOptionsImpl >
{
public:
    RenderingOptions();
    ~RenderingOptions();

public:
    VkPolygonMode getPolygonMode() const;
    VkCullModeFlagBits getCullMode() const;
    VkFrontFace getFrontFace() const;
    VkSampleCountFlagBits getRasterizationSamples() const;

    float getLineWidth() const;
    float getDepthBiasConstantFactor() const;
    float getDepthBiasClamp() const;
    float getDepthBiasSlopeFactor() const;
    float getSampleShadingAmount() const;
    float getMinDepthBounds() const;
    float getMaxDepthBounds() const;
    
    bool getEnableDepthClamp() const;
    bool getEnableRasterizerDiscard() const;
    bool getEnableDepthBias() const;
    bool getEnableSampleShading() const;
    bool getEnableAlphaToCoverage() const;
    bool getEnableResetAlphaToOne() const;
    bool getEnableDepthTest() const;
    bool getEnableDepthWrite() const;
    bool getEnableDepthBoundsTest() const;
    bool getEnableStencilTest() const;

    bool getModifyCoverageMask() const;
    std::uint32_t getModifiedCoverageMask ( std::uint32_t index ) const;

    VkCompareOp getDepthCompareOperator() const;
    const VkStencilOpState& getFrontFacingStencilCfg() const;
    const VkStencilOpState& getBackFacingStencilCfg() const;
    
    void setPolygonMode ( VkPolygonMode v );
    void setCullMode ( VkCullModeFlagBits v );
    void setFrontFace ( VkFrontFace v );
    void setRasterizationSamples ( VkSampleCountFlagBits v );

    void setLineWidth ( float v );
    void setDepthBiasConstantFactor ( float v );
    void setDepthBiasClamp ( float v );
    void setDepthBiasSlopeFactor ( float v );
    void setSampleShadingAmount ( float v );
    void setMinDepthBounds ( float v );
    void setMaxDepthBounds (float  v );
    
    void setEnableDepthClamp ( bool v );
    void setEnableRasterizerDiscard ( bool v );
    void setEnableDepthBias ( bool v );
    void setEnableSampleShading ( bool v );
    void setEnableAlphaToCoverage ( bool v );
    void setEnableResetAlphaToOne ( bool v );
    void setEnableDepthTest ( bool v );
    void setEnableDepthWrite ( bool v );
    void setEnableDepthBoundsTest ( bool v );
    void setEnableStencilTest ( bool v );

    void setModifyCoverageMask ( bool v );
    void setModifiedCoverageMask ( std::uint32_t index, std::uint32_t v );

    void setDepthCompareOperator ( VkCompareOp v );
    void setFrontFacingStencilCfg ( VkStencilOpState v );
    void setBackFacingStencilCfg ( VkStencilOpState v );

    VPP_DLLAPI void addViewport ( const Viewport& vp );

public:
    typedef std::function< void () > FRasterShader;
    void operator<< ( FRasterShader&& fRasterShader );

public:
    void initViewportInfo (
        VkPipelineViewportStateCreateInfo* pDest ) const;

    void initRasterizationInfo (
        VkPipelineRasterizationStateCreateInfo* pDest ) const;

    void initMultisampleInfo (
        VkPipelineMultisampleStateCreateInfo* pDest ) const;

    void initDepthStencilInfo (
        VkPipelineDepthStencilStateCreateInfo* pDest ) const;

private:
    VPP_DLLAPI void copyOnWrite();
};

// -----------------------------------------------------------------------------

class RenderingOptionsImpl : public TSharedObject< RenderingOptionsImpl >
{
public:
    VPP_DLLAPI RenderingOptionsImpl();
    VPP_DLLAPI RenderingOptionsImpl ( const RenderingOptionsImpl& rhs );

private:
    friend class RenderingOptions;

    VPP_DLLAPI void initViewportInfo (
        VkPipelineViewportStateCreateInfo* pDest ) const;

    VPP_DLLAPI void initRasterizationInfo (
        VkPipelineRasterizationStateCreateInfo* pDest ) const;

    VPP_DLLAPI void initMultisampleInfo (
        VkPipelineMultisampleStateCreateInfo* pDest ) const;

    VPP_DLLAPI void initDepthStencilInfo (
        VkPipelineDepthStencilStateCreateInfo* pDest ) const;

private:
    VkPolygonMode d_polygonMode;
    VkCullModeFlagBits d_cullMode;
    VkFrontFace d_frontFace;
    VkSampleCountFlagBits d_rasterizationSamples;

    float d_lineWidth;
    float d_depthBiasConstantFactor;
    float d_depthBiasClamp;
    float d_depthBiasSlopeFactor;
    float d_sampleShadingAmount;
    float d_minDepthBounds;
    float d_maxDepthBounds;
    
    bool d_bEnableDepthClamp;
    bool d_bEnableRasterizerDiscard;
    bool d_bEnableDepthBias;
    bool d_bEnableSampleShading;
    bool d_bEnableAlphaToCoverage;
    bool d_bEnableResetAlphaToOne;
    bool d_bEnableDepthTest;
    bool d_bEnableDepthWrite;
    bool d_bEnableDepthBoundsTest;
    bool d_bEnableStencilTest;

    bool d_bModifyCoverageMask;
    std::uint32_t d_modifiedCoverageMask [ 2 ];

    VkCompareOp d_depthCompareOperator;
    VkStencilOpState d_frontFacingStencilCfg;
    VkStencilOpState d_backFacingStencilCfg;

    std::vector< VkViewport > d_viewports;
    std::vector< VkRect2D > d_scissors;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE RenderingOptions :: RenderingOptions() :
    TCloneableSharedReference< RenderingOptionsImpl >( new RenderingOptionsImpl() )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE RenderingOptions :: ~RenderingOptions()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPolygonMode RenderingOptions :: getPolygonMode() const
{
    return get()->d_polygonMode;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCullModeFlagBits RenderingOptions :: getCullMode() const
{
    return get()->d_cullMode;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkFrontFace RenderingOptions :: getFrontFace() const
{
    return get()->d_frontFace;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkSampleCountFlagBits RenderingOptions :: getRasterizationSamples() const
{
    return get()->d_rasterizationSamples;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getLineWidth() const
{
    return get()->d_lineWidth;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getDepthBiasConstantFactor() const
{
    return get()->d_depthBiasConstantFactor;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getDepthBiasClamp() const
{
    return get()->d_depthBiasClamp;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getDepthBiasSlopeFactor() const
{
    return get()->d_depthBiasSlopeFactor;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getSampleShadingAmount() const
{
    return get()->d_sampleShadingAmount;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getMinDepthBounds() const
{
    return get()->d_minDepthBounds;
}

// -----------------------------------------------------------------------------

VPP_INLINE float RenderingOptions :: getMaxDepthBounds() const
{
    return get()->d_maxDepthBounds;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableDepthClamp() const
{
    return get()->d_bEnableDepthClamp;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableRasterizerDiscard() const
{
    return get()->d_bEnableRasterizerDiscard;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableDepthBias() const
{
    return get()->d_bEnableDepthBias;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableSampleShading() const
{
    return get()->d_bEnableSampleShading;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableAlphaToCoverage() const
{
    return get()->d_bEnableAlphaToCoverage;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableResetAlphaToOne() const
{
    return get()->d_bEnableResetAlphaToOne;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableDepthTest() const
{
    return get()->d_bEnableDepthTest;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableDepthWrite() const
{
    return get()->d_bEnableDepthWrite;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableDepthBoundsTest() const
{
    return get()->d_bEnableDepthBoundsTest;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getEnableStencilTest() const
{
    return get()->d_bEnableStencilTest;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool RenderingOptions :: getModifyCoverageMask() const
{
    return get()->d_bModifyCoverageMask;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t RenderingOptions :: getModifiedCoverageMask ( std::uint32_t index ) const
{
    return get()->d_modifiedCoverageMask [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE VkCompareOp RenderingOptions :: getDepthCompareOperator() const
{
    return get()->d_depthCompareOperator;
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkStencilOpState& RenderingOptions :: getFrontFacingStencilCfg() const
{
    return get()->d_frontFacingStencilCfg;
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkStencilOpState& RenderingOptions :: getBackFacingStencilCfg() const
{
    return get()->d_backFacingStencilCfg;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setPolygonMode ( VkPolygonMode v )
{
    copyOnWrite();
    get()->d_polygonMode = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setCullMode ( VkCullModeFlagBits v )
{
    copyOnWrite();
    get()->d_cullMode = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setFrontFace ( VkFrontFace v )
{
    copyOnWrite();
    get()->d_frontFace = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setRasterizationSamples ( VkSampleCountFlagBits v )
{
    copyOnWrite();
    get()->d_rasterizationSamples = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setLineWidth ( float v )
{
    copyOnWrite();
    get()->d_lineWidth = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setDepthBiasConstantFactor ( float v )
{
    copyOnWrite();
    get()->d_depthBiasConstantFactor = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setDepthBiasClamp ( float v )
{
    copyOnWrite();
    get()->d_depthBiasClamp = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setDepthBiasSlopeFactor ( float v )
{
    copyOnWrite();
    get()->d_depthBiasSlopeFactor = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setSampleShadingAmount ( float v )
{
    copyOnWrite();
    get()->d_sampleShadingAmount = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setMinDepthBounds ( float v )
{
    copyOnWrite();
    get()->d_minDepthBounds = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setMaxDepthBounds ( float v )
{
    copyOnWrite();
    get()->d_maxDepthBounds = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableDepthClamp ( bool v )
{
    copyOnWrite();
    get()->d_bEnableDepthClamp = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableRasterizerDiscard ( bool v )
{
    copyOnWrite();
    get()->d_bEnableRasterizerDiscard = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableDepthBias ( bool v )
{
    copyOnWrite();
    get()->d_bEnableDepthBias = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableSampleShading ( bool v )
{
    copyOnWrite();
    get()->d_bEnableSampleShading = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableAlphaToCoverage ( bool v )
{
    copyOnWrite();
    get()->d_bEnableAlphaToCoverage = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableResetAlphaToOne ( bool v )
{
    copyOnWrite();
    get()->d_bEnableResetAlphaToOne = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableDepthTest ( bool v )
{
    copyOnWrite();
    get()->d_bEnableDepthTest = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableDepthWrite ( bool v )
{
    copyOnWrite();
    get()->d_bEnableDepthWrite = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableDepthBoundsTest ( bool v )
{
    copyOnWrite();
    get()->d_bEnableDepthBoundsTest = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setEnableStencilTest ( bool v )
{
    copyOnWrite();
    get()->d_bEnableStencilTest = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setModifyCoverageMask ( bool v )
{
    copyOnWrite();
    get()->d_bModifyCoverageMask = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setModifiedCoverageMask ( std::uint32_t index, std::uint32_t v )
{
    copyOnWrite();
    get()->d_modifiedCoverageMask [ index ] = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setDepthCompareOperator ( VkCompareOp v )
{
    copyOnWrite();
    get()->d_depthCompareOperator = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setFrontFacingStencilCfg ( VkStencilOpState v )
{
    copyOnWrite();
    get()->d_frontFacingStencilCfg = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: setBackFacingStencilCfg ( VkStencilOpState v )
{
    copyOnWrite();
    get()->d_backFacingStencilCfg = v;
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: initViewportInfo (
    VkPipelineViewportStateCreateInfo* pDest ) const
{
    get()->initViewportInfo ( pDest );
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: initRasterizationInfo (
    VkPipelineRasterizationStateCreateInfo* pDest ) const
{
    get()->initRasterizationInfo ( pDest );
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: initMultisampleInfo (
    VkPipelineMultisampleStateCreateInfo* pDest ) const
{
    get()->initMultisampleInfo ( pDest );
}

// -----------------------------------------------------------------------------

VPP_INLINE void RenderingOptions :: initDepthStencilInfo (
    VkPipelineDepthStencilStateCreateInfo* pDest ) const
{
    get()->initDepthStencilInfo ( pDest );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPRENDERINGOPTIONS_HPP
