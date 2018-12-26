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
    \brief Structure describing normalized sampler parameters.

    Normalized sampler is a sampler using the coordinates in range [ 0, 1 ].

    Use SNormalizedSampler structure to define parameters of the sampler
    and NormalizedSampler class to create the sampler object.
*/

struct SNormalizedSampler
{
    /**
        \brief Constructor setting default parameters.

        The default values are given in individual parameter descriptions.
    */
    SNormalizedSampler ( float maxLod = 1.0f );

    /**
        \brief Ordering operator. Useful for storing sampler descriptions
        in sets or maps.
    */
    bool operator< ( const SNormalizedSampler& rhs ) const;

    /**
        \brief Texel addressing mode for the U coordinate.

        Controls the wraparound of computed integer coordinate of the texel.

        Let \c SIZE be the image size in corresponding direction and \c i the
        integer coordinate value (computed from floating-point value). Then
        the meaning of values is as follows:

        - \c VK_SAMPLER_ADDRESS_MODE_REPEAT: <tt>i mod SIZE </tt>
        - \c VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: <tt>(SIZE-1) - mirror ( ( i mod ( 2*SIZE ) ) - SIZE )</tt>
        - \c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: <tt>clamp ( i, 0, SIZE-1 )</tt>
        - \c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: <tt>clamp ( i, -1, SIZE )</tt>
        - \c VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: <tt>clamp ( mirror(i), 0, SIZE-1 )</tt>

        The functions are defined as:
        - <tt>mirror ( n ) = ( n >= 0 ? n : -(1+n) )</tt>
        - <tt>clamp ( n, a, b ) = ( n < a ? a : ( n > b ? b : n ) )</tt>

        See section 15.9.1 of the Vulkan specs for more detail.

        Default value: \c VK_SAMPLER_ADDRESS_MODE_REPEAT.
    */
    unsigned int addressModeU;

    /**
        \brief Texel addressing mode for the V coordinate.

        The meaning is the same as for \c addressModeU.
        See section 15.9.1 of the Vulkan specs for more detail.

        Default value: \c VK_SAMPLER_ADDRESS_MODE_REPEAT.
    */
    unsigned int addressModeV;

    /**
        \brief Texel addressing mode for the W coordinate.

        The meaning is the same as for \c addressModeU.
        See section 15.9.1 of the Vulkan specs for more detail.

        Default value: \c VK_SAMPLER_ADDRESS_MODE_REPEAT.
    */
    unsigned int addressModeW;

    /**
        \brief Defines the color for border texels.

        A border texel is a texel outside valid range of coordinates. This can happen
        e.g. if \c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER mdoe is set. The substitute
        color is chosen according to the following:

        - VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK specifies a transparent, floating-point format, black color.
        - VK_BORDER_COLOR_INT_TRANSPARENT_BLACK specifies a transparent, integer format, black color.
        - VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK specifies an opaque, floating-point format, black color.
        - VK_BORDER_COLOR_INT_OPAQUE_BLACK specifies an opaque, integer format, black color.
        - VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE specifies an opaque, floating-point format, white color.
        - VK_BORDER_COLOR_INT_OPAQUE_WHITE specifies an opaque, integer format, white color.

        See section 15.3.1 of Vulkan docs for more details.

        Default value: VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK.
    */

    unsigned int borderColor;

    /**
        \brief Specifies the comparison function to apply to fetched data
        before the depth compare operation.

        One of the following values:
        - VK_COMPARE_OP_NEVER,
        - VK_COMPARE_OP_LESS,
        - VK_COMPARE_OP_EQUAL,
        - VK_COMPARE_OP_LESS_OR_EQUAL,
        - VK_COMPARE_OP_GREATER,
        - VK_COMPARE_OP_NOT_EQUAL,
        - VK_COMPARE_OP_GREATER_OR_EQUAL,
        - VK_COMPARE_OP_ALWAYS.

        Depth comparisons are performed when reading the image with depth
        comparing functions, namely: TextureDref, TextureLodDref, TextureProjDref,
        TextureLodProjDref, TextureGatherDref. These functions operate on images
        containing single floating point values, determining some kind of distance
        (e.g. from the light source). Depth comparison functions do not return
        this distance, but rather compares it with specified value and yield
        either \c 0.0 or \c 1.0 value depending on the selected comparison
        operator. These values are then subject to filtering (interpolation).
                
        This is typically used in shadow map algorithms to create smooth shadows.

        See section 15.3.4 of Vulkan docs for more details.

        Default value: VK_COMPARE_OP_NEVER.
    */

    unsigned int compareOp;

    /**
        \brief Enables depth comparison operation for this sampler.

        This field must be set to \c true if this sampler will be used to
        sample a depth image with TextureDref, TextureLodDref, TextureProjDref,
        TextureLodProjDref, TextureGatherDref functions. In such case, also
        comparison operator must be selected.

        Otherwise it must be set to \c false.

        Default value: false.
    */

    unsigned int compare;

    /**
        \brief Specifies the method of interpolation when upscaling the texture. 

        It has to be one of the values:
        - VK_FILTER_NEAREST specifies nearest filtering (no interpolation).
        - VK_FILTER_LINEAR specifies linear filtering (linear interpolation with neighbours).

        See section 15.9.3 of the Vulkan specification for more details.

        Default value: VK_FILTER_LINEAR.
    */

    unsigned int magFilterMode;

    /**
        \brief Specifies the method of interpolation when downscaling the texture. 

        It has to be one of the values:
        - VK_FILTER_NEAREST specifies nearest filtering (no interpolation).
        - VK_FILTER_LINEAR specifies linear filtering (linear interpolation with neighbours).

        See section 15.9.3 of the Vulkan specification for more details.

        Default value: VK_FILTER_LINEAR.
    */

    unsigned int minFilterMode;

    /**
        \brief Specifies the method of interpolation between mip levels.

        - VK_SAMPLER_MIPMAP_MODE_NEAREST specifies nearest filtering (no interpolation).
        - VK_SAMPLER_MIPMAP_MODE_LINEAR specifies linear filtering (linear interpolation with neighbours).

        See section 15.9.3 of the Vulkan specification for more detail.

        Default value: VK_SAMPLER_MIPMAP_MODE_LINEAR.
    */

    unsigned int mipMapMode;

    /**
        \brief Enables anisotropic filtering. 

        See section 15.9.4 of the Vulkan specification for more details.

        Default value: false.
    */

    unsigned int anisotropy;

    /**
        \brief The bias to be added to mipmap LOD (level-of-detail) calculation.

        See section 15.6.7 of the Vulkan specification for more details.

        Default value: 0.
    */
    float mipLodBias;

    /**
        \brief The anisotropy value clamp. Ignored when anisotropy is disabled.

        Default value: 0.
    */
    float maxAnisotropy;

    /**
        \brief The minimum value used to clamp the computed LOD value.

        See section 15.6.7 of the Vulkan specification for more details.

        Default value: 0.
    */
    float minLod;

    /**
        \brief The maximum value used to clamp the computed LOD value.

        See section 15.6.7 of the Vulkan specification for more details.

        Default value: 1 or specified in the constructor.
    */
    float maxLod;
};

// -----------------------------------------------------------------------------
/**
    \brief Structure describing unnormalized sampler parameters.

    Unnormalized sampler is a sampler using the coordinates in range [ 0, SIZE ].

    Use SUnnormalizedSampler structure to define parameters of the sampler
    and UnnormalizedSampler class to create the sampler object.
*/

struct SUnnormalizedSampler
{
    /**
        \brief Constructor setting default parameters.

        The default values are given in individual parameter descriptions.
    */
    SUnnormalizedSampler();

    /**
        \brief Ordering operator. Useful for storing sampler descriptions
        in sets or maps.
    */
    bool operator< ( const SUnnormalizedSampler& rhs ) const;

    /**
        \brief Texel addressing mode for the U coordinate.

        Controls the wraparound of computed integer coordinate of the texel.

        Let \c SIZE be the image size in corresponding direction and \c i the
        integer coordinate value (computed from floating-point value). Then
        the meaning of values is as follows:

        - \c false (\c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE): <tt>clamp ( i, 0, SIZE-1 )</tt>
        - \c true (\c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER): <tt>clamp ( i, -1, SIZE )</tt>

        The functions are defined as:
        - <tt>clamp ( n, a, b ) = ( n < a ? a : ( n > b ? b : n ) )</tt>

        See section 15.9.1 of the Vulkan specs for more detail.

        Default value: \c false.
    */
    unsigned int clampToBorderU;

    /**
        \brief Texel addressing mode for the V coordinate.

        Controls the wraparound of computed integer coordinate of the texel.

        See section 15.9.1 of the Vulkan specs for more detail.

        Default value: \c false.
    */
    unsigned int clampToBorderV;

    /**
        \brief Specifies the method of interpolation when upscaling and downscaling the texture. 

        It has to be one of the values:
        - VK_FILTER_NEAREST specifies nearest filtering (no interpolation).
        - VK_FILTER_LINEAR specifies linear filtering (linear interpolation with neighbours).

        For unnormalized samplers, filters for upscaling and downscaling
        are always the same.

        See section 15.9.3 of the Vulkan specification for more details.

        Default value: VK_FILTER_LINEAR.
    */
    unsigned int filterMode;

    /**
        \brief Defines the color for border texels.

        A border texel is a texel outside valid range of coordinates. This can happen
        e.g. if \c VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER mode is set. The substitute
        color is chosen according to the following:

        - VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK specifies a transparent, floating-point format, black color.
        - VK_BORDER_COLOR_INT_TRANSPARENT_BLACK specifies a transparent, integer format, black color.
        - VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK specifies an opaque, floating-point format, black color.
        - VK_BORDER_COLOR_INT_OPAQUE_BLACK specifies an opaque, integer format, black color.
        - VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE specifies an opaque, floating-point format, white color.
        - VK_BORDER_COLOR_INT_OPAQUE_WHITE specifies an opaque, integer format, white color.

        See section 15.3.1 of Vulkan docs for more details.

        Default value: VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK.
    */
    unsigned int borderColor;

    /**
        \brief The bias to be added to mipmap LOD (level-of-detail) calculation.

        See section 15.6.7 of the Vulkan specification for more details.

        Default value: 0.
    */
    float mipLodBias;
};

// -----------------------------------------------------------------------------
/**
    \brief Texture sampler object (normalized).

    This is the actual sampler object. It is constructed from SNormalizedSampler.

    This object is reference-counted and may be passed by value.

    The NormalizedSampler object can be used in the following places:
    - constructors of inConstSampledTexture binding point,
    - constructors of inConstSampler binding point,
    - constructors of arrays of the above (arrayOf),
    - bind() construct, to bind the sampler and image to 
      inSampledTexture binding points,
    - on the right-hand side of assigment operator when binding
      to inSampler binding points.
*/

class NormalizedSampler
{
public:
    /** \brief Constructs null reference. */
    NormalizedSampler();

    /**
        \brief Constructs a sampler with default parameters on specified device.
    */
    NormalizedSampler ( const Device& hDevice );

    /**
        \brief Constructs a sampler with provided parameters on specified device.
    */
    NormalizedSampler ( const Device& hDevice, const SNormalizedSampler& samplerInfo );

    /**
        \brief Returns Vulkan handle of the sampler object.
    */
    VkSampler handle() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Texture sampler object (normalized).

    This is the actual sampler object. It is constructed from SUnnormalizedSampler.

    This object is reference-counted and may be passed by value.

    The UnnormalizedSampler object can be used in the following places:
    - constructors of inConstSampledTexture binding point,
    - constructors of inConstSampler binding point,
    - constructors of arrays of the above (arrayOf),
    - bind() construct, to bind the sampler and image to 
      inSampledTexture binding points,
    - on the right-hand side of assigment operator when binding
      to inSampler binding points.
*/

class UnnormalizedSampler
{
public:
    /** \brief Constructs null reference. */
    UnnormalizedSampler();

    /**
        \brief Constructs a sampler with default parameters on specified device.
    */
    UnnormalizedSampler ( const Device& hDevice );

    /**
        \brief Constructs a sampler with provided parameters on specified device.
    */
    UnnormalizedSampler ( const Device& hDevice, const SUnnormalizedSampler& samplerInfo );

    /**
        \brief Returns Vulkan handle of the sampler object.
    */
    VkSampler handle() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
