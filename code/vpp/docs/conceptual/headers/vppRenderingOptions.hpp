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
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief A container for various rendering parameters.

    This object is consumed by RenderPass::addPipeline() function.

    This object is reference counted and can be passed by value.
*/

class RenderingOptions
{
public:
    /** \brief Constructs a container with default options. */
    RenderingOptions();

public:

    /**
        \brief Sets the polygon rendering mode.

        There are three polygon modes: VK_POLYGON_MODE_FILL,
        VK_POLYGON_MODE_LINE, and VK_POLYGON_MODE_POINT. See chapter 24.7.2
        of the official Vulkan specification for details.
    */
    void setPolygonMode ( VkPolygonMode v );

    /**
        \brief Retrieves currently set polygon mode.
    */
    VkPolygonMode getPolygonMode() const;

    /**
        \brief Retrieves currently set face culling mode.

        There are four modes: VK_CULL_MODE_NONE, VK_CULL_MODE_FRONT_BIT,
        VK_CULL_MODE_BACK_BIT, VK_CULL_MODE_FRONT_AND_BACK. See chapter 24.7.1
        of the official Vulkan specification for details.
    */
    VkCullModeFlagBits getCullMode() const;

    /**
        \brief Retrieves currently set front face detection mode.

        There are two modes: VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_FRONT_FACE_CLOCKWISE. See chapter 24.7.1
        of the official Vulkan specification for details.
    */
    VkFrontFace getFrontFace() const;

    /**
        \brief Retrieves currently set sample count for multisampling.

        It can be one of these values: VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_2_BIT,
        VK_SAMPLE_COUNT_4_BIT ... VK_SAMPLE_COUNT_16_BIT. See chapter 24
        of the official Vulkan specification for details.
    */
    VkSampleCountFlagBits getRasterizationSamples() const;

    /**
        \brief Retrieves currently set line width for rasterizing lines.

        See chapter 24.6 of the official Vulkan specification for details.
    */
    float getLineWidth() const;

    /**
        \brief Retrieves the scalar factor controlling the constant depth value added to each fragment.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    float getDepthBiasConstantFactor() const;

    /**
        \brief Retrieves the maximum (or minimum) depth bias of a fragment.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    float getDepthBiasClamp() const;

    /**
        \brief Retrieves the scalar factor applied to a fragment’s slope in depth bias calculations.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    float getDepthBiasSlopeFactor() const;

    /**
        \brief Retrieves the minimum fraction of sample shading.

        See chapter 24.4 of the official Vulkan specification for details.
    */
    float getSampleShadingAmount() const;

    /**
        \brief Retrieves the minimum depth for depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    float getMinDepthBounds() const;

    /**
        \brief Retrieves the maximum depth for depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    float getMaxDepthBounds() const;
    
    /**
        \brief Retrieves the status of the depth clamp feature.

        See chapter 23.2 of the official Vulkan specification for details.
    */
    bool getEnableDepthClamp() const;

    /**
        \brief Retrieves the status of the rasterizer discard feature.

        Rasterizer discard mode allows to skip rasterization and fragment
        processing entirely. This can speed up computation if only geometry
        processing is being done in the pipeline, without fragment shader.

        See chapter 24.1 of the official Vulkan specification for details.
    */
    bool getEnableRasterizerDiscard() const;

    /**
        \brief Retrieves the status of the depth bias feature.

        Depth bias allows to add some bias to depth values, to prevent
        artifacts resulting limited floating point precision (e.g. self-shadowing).

        See chapter 24.7.3 of the official Vulkan specification for details.
    */

    bool getEnableDepthBias() const;

    /**
        \brief Retrieves the status of the samples shading feature.

        Sample shading allows to call fragment shader separately for
        each sample in multisampling mode. Otherwise the shader is being
        called only once for each fragment.

        See chapter 24.4 of the official Vulkan specification for details.
    */
    bool getEnableSampleShading() const;

    /**
        \brief Retrieves the status of the alpha to coverage conversion feature.

        See chapter 25.6 of the official Vulkan specification for details.
    */
    bool getEnableAlphaToCoverage() const;

    /**
        \brief Retrieves the status of the alpha reset feature.

        See chapter 25.6 of the official Vulkan specification for details.
    */
    bool getEnableResetAlphaToOne() const;

    /**
        \brief Retrieves the status of the depth test.

        Depth test enables usage of the Z-buffer e.g. to discard fragments
        which are behind already drawn fragments. Z-buffer operation
        is configurable.

        See chapter 25.10 of the official Vulkan specification for details.
    */
    bool getEnableDepthTest() const;

    /**
        \brief Retrieves the status of the depth write mode.

        This enables writing to the Z-buffer. It is usually the desired side
        effect. However, if you have separate D-pass and then subsequent passes
        which only read the Z-buffer, turn writing off.

        See chapter 25.10 of the official Vulkan specification for details.
    */
    bool getEnableDepthWrite() const;

    /**
        \brief Retrieves the status of the depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    bool getEnableDepthBoundsTest() const;

    /**
        \brief Retrieves the status of the stencil test.

        See chapter 25.9 of the official Vulkan specification for details.
    */
    bool getEnableStencilTest() const;

    bool getModifyCoverageMask() const;
    std::uint32_t getModifiedCoverageMask ( std::uint32_t index ) const;

    /**
        \brief Retrieves comparison operator for the depth test.

        See chapter 25.10 of the official Vulkan specification for details.
    */

    VkCompareOp getDepthCompareOperator() const;


    const VkStencilOpState& getFrontFacingStencilCfg() const;
    const VkStencilOpState& getBackFacingStencilCfg() const;
    
    /**
        \brief Sets the face culling mode.

        There are four modes: VK_CULL_MODE_NONE, VK_CULL_MODE_FRONT_BIT,
        VK_CULL_MODE_BACK_BIT, VK_CULL_MODE_FRONT_AND_BACK. See chapter 24.7.1
        of the official Vulkan specification for details.
    */
    void setCullMode ( VkCullModeFlagBits v );

    /**
        \brief Set the front face detection mode.

        There are two modes: VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_FRONT_FACE_CLOCKWISE. See chapter 24.7.1
        of the official Vulkan specification for details.
    */
    void setFrontFace ( VkFrontFace v );

    /**
        \brief Sets the sample count for multisampling.

        It can be one of these values: VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_2_BIT,
        VK_SAMPLE_COUNT_4_BIT ... VK_SAMPLE_COUNT_16_BIT. See chapter 24
        of the official Vulkan specification for details.
    */
    void setRasterizationSamples ( VkSampleCountFlagBits v );

    /**
        \brief Sets the line width for rasterizing lines.

        See chapter 24.6 of the official Vulkan specification for details.
    */
    void setLineWidth ( float v );

    /**
        \brief Sets the scalar factor controlling the constant depth value added to each fragment.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    void setDepthBiasConstantFactor ( float v );

    /**
        \brief Sets the maximum (or minimum) depth bias of a fragment.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    void setDepthBiasClamp ( float v );

    /**
        \brief Sets the scalar factor applied to a fragment’s slope in depth bias calculations.

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    void setDepthBiasSlopeFactor ( float v );

    /**
        \brief Sets the minimum fraction of sample shading.

        See chapter 24.4 of the official Vulkan specification for details.
    */
    void setSampleShadingAmount ( float v );

    /**
        \brief Sets the minimum depth for depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    void setMinDepthBounds ( float v );

    /**
        \brief Sets the maximum depth for depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    void setMaxDepthBounds (float  v );
    
    /**
        \brief Sets the status of the depth clamp feature.

        See chapter 23.2 of the official Vulkan specification for details.
    */
    void setEnableDepthClamp ( bool v );

    /**
        \brief Sets the status of the rasterizer discard feature.

        Rasterizer discard mode allows to skip rasterization and fragment
        processing entirely. This can speed up computation if only geometry
        processing is being done in the pipeline, without fragment shader.

        See chapter 24.1 of the official Vulkan specification for details.
    */
    void setEnableRasterizerDiscard ( bool v );

    /**
        \brief Sets the status of the depth bias feature.

        Depth bias allows to add some bias to depth values, to prevent
        artifacts resulting limited floating point precision (e.g. self-shadowing).

        See chapter 24.7.3 of the official Vulkan specification for details.
    */
    void setEnableDepthBias ( bool v );

    /**
        \brief Sets the status of the samples shading feature.

        Sample shading allows to call fragment shader separately for
        each sample in multisampling mode. Otherwise the shader is being
        called only once for each fragment.

        See chapter 24.4 of the official Vulkan specification for details.
    */
    void setEnableSampleShading ( bool v );

    /**
        \brief Sets the status of the alpha to coverage conversion feature.

        See chapter 25.6 of the official Vulkan specification for details.
    */
    void setEnableAlphaToCoverage ( bool v );

    /**
        \brief Sets the status of the alpha reset feature.

        See chapter 25.6 of the official Vulkan specification for details.
    */
    void setEnableResetAlphaToOne ( bool v );

    /**
        \brief Sets the status of the depth test.

        Depth test enables usage of the Z-buffer e.g. to discard fragments
        which are behind already drawn fragments. Z-buffer operation
        is configurable.

        See chapter 25.10 of the official Vulkan specification for details.
    */
    void setEnableDepthTest ( bool v );

    /**
        \brief Sets the status of the depth write mode.

        This enables writing to the Z-buffer. It is usually the desired side
        effect. However, if you have separate D-pass and then subsequent passes
        which only read the Z-buffer, turn writing off.

        See chapter 25.10 of the official Vulkan specification for details.
    */
    void setEnableDepthWrite ( bool v );

    /**
        \brief Sets the status of the depth bounds test.

        See chapter 25.8 of the official Vulkan specification for details.
    */
    void setEnableDepthBoundsTest ( bool v );

    /**
        \brief Sets the status of the stencil test.

        See chapter 25.9 of the official Vulkan specification for details.
    */
    void setEnableStencilTest ( bool v );

    void setModifyCoverageMask ( bool v );
    void setModifiedCoverageMask ( std::uint32_t index, std::uint32_t v );

    void setDepthCompareOperator ( VkCompareOp v );
    void setFrontFacingStencilCfg ( VkStencilOpState v );
    void setBackFacingStencilCfg ( VkStencilOpState v );

    void addViewport ( const Viewport& vp );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
