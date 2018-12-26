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

struct SGatherOffsets
{
    typedef const SGatherOffsets& rvalue_type;
    int offsets [ 4 ][ 2 ];
};

// -----------------------------------------------------------------------------

/*
    FIXME wywalic

    ktore co zwracaja

    ioImage                       TRImage
    inSampler                     TRSampler
    inConstSampler                TRSampler
    inTexture                     TRTexture
    inSampledTexture              TRSampledTexture
    inConstSampledTexture         TRSampledTexture
    inTextureBuffer               TRSampledTexture
    ioImageBuffer                 TRImage
    inAttachment                  TRInput

*/

// -----------------------------------------------------------------------------
/**
    \brief Converts texture and sampler pair to combined sampled texture.

    As the first argument, provide inTexture object.

    As the second argument you may use inSampler or inConstSampler object.

    This function creates and returns combined texture-sampler object,
    which is equivalent to inSampledTexture.
*/

template< class TextureT, class SamplerT >
auto MakeSampledTexture ( const TextureT& texture, const SamplerT& sampler );

// -----------------------------------------------------------------------------
/**
    \brief Extracts the texture part from combined sampled texture.

    As the argument, provide inSampledTexture, inConstSampledTexture 
    or inTextureBuffer object.

    Returns an equivalent of inTexture object.
*/

template< class TextureT >
auto ExtractSampledTexture ( const TextureT& texture );

// -----------------------------------------------------------------------------
/**
    \brief Retrieves the extent of specified image.

    As the argument, you may provide an object of the following types:
    ioImage, inTexture, inSampledTexture, inConstSampledTexture,
    inTextureBuffer, ioImageBuffer.

    This function returns either Int or integer vector of size appropriate
    for the number of dimensions in the image (e.g. IVec2 for 2D image).
*/

template< class ImageT >
auto ImageSize ( const ImageT& image );

// -----------------------------------------------------------------------------
/**
    \brief Retrieves the extent of specified MIP image level.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, specify the MIP level index.

    This function returns either Int or integer vector of size appropriate
    for the number of dimensions in the image (e.g. IVec2 for 2D image).
*/

template< class ImageT, class LodT >
auto TextureSize ( const ImageT& image, const Int& lod );

// -----------------------------------------------------------------------------
/**
    \brief Retrieves the number of samples per pixel in the image.

    As the argument, you may provide an object of the following types:
    ioImage, inTexture, inSampledTexture, inConstSampledTexture. It must be
    a 2D image.
*/

template< class ImageT >
Int ImageQuerySamples ( const ImageT& image );

// -----------------------------------------------------------------------------
/**
    \brief Retrieves the number of MIP levels in the image.

    As the argument, you may provide an object of the following types:
    ioImage, inTexture, inSampledTexture, inConstSampledTexture.
*/

template< class ImageT >
Int ImageQueryLevels ( const ImageT& image );

// -----------------------------------------------------------------------------
/**
    \brief Retrieves the mipmap level and the level of detail for a hypothetical
           sampling of image at coordinate using an implicit level of detail.

    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.

    As the second argument, specify a vector of floating point values of
    the size appropriate for image dimensionality (e.g. Vec2 for 2D image).

    This function may be called only in fragment shaders.
*/

template< class ImageT, class CoordsT >
Vec2 TextureQueryLod ( const ImageT& image, const CoordsT& coords );

// -----------------------------------------------------------------------------
/**
    \brief Reads a pixel from input attachment.

    Call only inside fragment shaders.

    As the argument, provide an object of inAttachment type.

    This function returns 4-element vector of type compatible with image
    format. For example, Vec4 or IVec4. For images where the format uses less
    than 4 components, unused components will be equal to 0.

    The function reads a pixel from currently processed location. It is implicitly
    maintained by the fragment shader.
*/

template< class ImageT >
auto SubpassLoad ( const ImageT& image );

// -----------------------------------------------------------------------------
/**
    \brief Reads a pixel from input attachment, with offset.

    Call only inside fragment shaders.

    As the first argument, provide an object of inAttachment type.

    As the second argument, provide a vector of coordinates which will be
    added to the currently processed location, in order to compute source
    pixel location.

    This function returns 4-element vector of type compatible with image
    format. For example, Vec4 or IVec4. For images where the format uses less
    than 4 components, unused components will be equal to 0.
*/

template< class ImageT >
auto SubpassLoad ( const ImageT& image, const IVec2& coords );

// -----------------------------------------------------------------------------
/**
    \brief Reads a sample from multisampled input attachment.

    Call only inside fragment shaders.

    As the first argument, provide an object of inAttachment type.

    As the second argument, provide an index of the sample.

    This function returns 4-element vector of type compatible with image
    format. For example, Vec4 or IVec4. For images where the format uses less
    than 4 components, unused components will be equal to 0.
*/

template< class ImageT >
auto SubpassLoad ( const ImageT& image, const Int& nSample );

// -----------------------------------------------------------------------------
/**
    \brief Reads a sample from multisampled input attachment, with offset.

    Call only inside fragment shaders.

    As the first argument, provide an object of inAttachment type.

    As the second argument, provide a vector of coordinates which will be
    added to the currently processed location, in order to compute source
    pixel location.

    As the third argument, provide an index of the sample.

    This function returns 4-element vector of type compatible with image
    format. For example, Vec4 or IVec4. For images where the format uses less
    than 4 components, unused components will be equal to 0.
*/

template< class ImageT >
auto SubpassLoad ( const ImageT& image, const IVec2& coords, const Int& nSample );

// -----------------------------------------------------------------------------
/**
    \brief Reads a pixel from a storage image.

    As the first argument, provide an object of ioImage or ioImageBuffer type.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.
*/

template< class ImageT, class CoordsT >
auto ImageLoad ( const ImageT& image, const CoordsT& coords );

// -----------------------------------------------------------------------------
/**
    \brief Reads a sample from multisampled storage image.

    As the first argument, provide an object of ioImage or ioImageBuffer type.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, specify the sample index.
*/

template< class ImageT, class CoordsT >
auto ImageLoad ( const ImageT& image, const CoordsT& coords, const Int& nSample );

// -----------------------------------------------------------------------------
/**
    \brief Writes a pixel to a storage image.

    As the first argument, provide an object of ioImage or ioImageBuffer type.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, provide a scalar or vector compatible with image format
    (e.g. Int for I32 format, Vec4 for normalized RGBA formats, etc.).
*/

template< class ImageT, class CoordsT, class ValueT >
void ImageStore ( const ImageT& image, const CoordsT& coords, const ValueT& value );

// -----------------------------------------------------------------------------
/**
    \brief Writes a sample to multisampled storage image.

    As the first argument, provide an object of ioImage or ioImageBuffer type.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, provide a scalar or vector compatible with image format
    (e.g. Int for I32 format, Vec4 for normalized RGBA formats, etc.).

    As the fourth argument, specify the destination sample index.
*/

template< class ImageT, class CoordsT, class ValueT >
void ImageStore ( const ImageT& image, const CoordsT& coords, const ValueT& value, const Int& nSample );

// -----------------------------------------------------------------------------
/**
    \brief Reads a texel from a texture without sampling.

    This function allows to read raw texel, without any sampling or filtering.
    It is a counterpart of ImageLoad(), but working on textures instead of
    storage images.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The source image must not be a cube map.
*/

template< class TextureT, class CoordsT >
auto TexelFetch ( const TextureT& texture, const CoordsT& coords );

// -----------------------------------------------------------------------------
/**
    \brief Reads a texel from multisampled texture without sampling.

    This function allows to read raw texel, without any sampling or filtering.
    It is a counterpart of ImageLoad(), but working on textures instead of
    storage images.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, specify the sample index.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The source image must not be a cube map.
*/

template< class TextureT, class CoordsT >
auto TexelFetch ( const TextureT& texture, const CoordsT& coords, const Int& nSample );

// -----------------------------------------------------------------------------
/**
    \brief Reads a texel from a specified MIP level of texture without sampling.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, specify the MIP level.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The source image must not be a cube map nor a multisampled image.
*/

template< class TextureT, class CoordsT >
auto TexelFetchLod ( const TextureT& texture, const CoordsT& coords, const Int& nLevel );

// -----------------------------------------------------------------------------
/**
    \brief Reads a texel (with coordinate offset) from a texture without sampling.

    This function allows to read raw texel, without any sampling or filtering.
    It is a counterpart of ImageLoad(), but working on textures instead of
    storage images.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, provide additional offset which will be added
    to \c coords coordinate vector. Note that the offset vector does not
    have additional coordinate for the array level.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The source image must not be a cube map.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TexelFetchOffset ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset );

// -----------------------------------------------------------------------------
/**
    \brief Reads a texel from a specified MIP level (with coordinate offset) of texture without sampling.

    This function allows to read raw texel, without any sampling or filtering.
    It is a counterpart of ImageLoad(), but working on textures instead of
    storage images.

    As the first argument, you may provide an object of the following types:
    inTexture, inSampledTexture, inConstSampledTexture, inTextureBuffer.

    As the second argument, provide a vector of integer type and the number
    of components appropriate for the image dimensionality (e.g. IVec2 for
    2D image, Int for a buffer, IVec3 for arrayed 2D image, etc.). Note that
    arrayed images require extra coordinate to specify the array level.

    As the third argument, specify the MIP level.

    As the fourth argument, provide additional offset which will be added
    to \c coords coordinate vector. Note that the offset vector does not
    have additional coordinate for the array level.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The source image must not be a cube map.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TexelFetchLodOffset ( const TextureT& texture, const CoordsT& coords, const Int& nLevel, const OffsetT& offset );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture.

    This function allows to read a texel with sampling and filtering. It is
    a primary method to read from textures.

    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.

    As the second argument, provide a vector of floating point type and the number
    of components appropriate for the image dimensionality, taking into account
    whether the image is arrayed and/or cube map.

    Arrayed image always adds single coordinate at the end. Since entire vector
    is floating point, you must convert an integer array index to floating point.

    Cube maps are very specific case. They are essentially layered 2D textures treated
    like they were 3D textures. A cube map has 6 layers corresponding to different
    faces of a cube. Imagine that we are positioned in the middle of the cube
    (this is an origin of our coordinate system) and we have a 3D vector of
    (s, r, t) coordinates which starts at origin and points away from it in
    some direction. A line along this direction will intersect one of cube faces
    at some point and we take our texel color from this point (with sampling
    and filtering). Therefore for cube maps, you need to specify 3D vector
    to \c Texture function, and for arrayed cube maps - 4D vector.

    The function returns 4-element vector of type determined by the image
    format. It can be either floating point type (Vec4) or integer type
    (IVec4, UVec4).

    The function may be only used within fragment shaders.
*/

template< class TextureT, class CoordsT >
auto Texture ( const TextureT& texture, const CoordsT& coords );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with LOD bias.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify a bias to be added to the parameter
    affecting selection of MIP levels from which the texel will be taken.

    The image must not be multisampled.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT >
auto Texture ( const TextureT& texture, const CoordsT& coords, const Float& bias );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with coordinate offset.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify an offset to be added to the
    coordinates from where the texel will be sampled.

    The image must not be a cube map.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto Texture ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with LOD gradient.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify a gradient which affects selection
    of MIP levels from which the texel will be taken.

    This is more advanced function. See section 15.6.2 of the Vulkan docs for
    more information.

    The image must not be multisampled.

    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT, class GradT >
auto Texture ( const TextureT& texture, const CoordsT& coords, const GradT& gradX, const GradT& gradY );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with coordinate offset and LOD gradient.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify both a coordinate offset and 
    a gradient which affects selection of MIP levels from which the texel will
    be taken.

    This is more advanced function. See section 15.6.2 of the Vulkan docs for
    more information.

    The image must not be multisampled.

    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT, class OffsetT, class GradT >
auto Texture ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset, const GradT& gradX, const GradT& gradY );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with explicit MIP levels selection.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify a parameter which selects the MIP
    levels in more direct way, as described in section 15.6.7 of the official
    Vulkan spec.

    The image must not be multisampled.

    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT >
auto TextureLod ( const TextureT& texture, const CoordsT& coords, const Float& lod );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with coordinate offset and explicit
           MIP levels selection.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that you specify a coordinate offset as well as a parameter
    which selects the MIP levels in more direct way, as described in section
    15.6.7 of the official Vulkan spec.

    The image must not be multisampled.

    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TextureLod ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset, const Float& lod );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that the coordinate vector has one more component
    at the end. Remaining components will be divided by this extra component,
    just as in the prespective division.

    The image must not be multisampled, arrayed nor cube map.

    The image sampler must be a normalized sampler.

    Remaining details are identical as in the base \c Texture function.
*/

template< class TextureT, class CoordsT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division and LOD bias.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies both perspective division and LOD
    bias. See the descriptions of respective \c Texture variants for more detail.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords, const Float& bias );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division and
           coordinate offset.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies both perspective division and
    coordinate offset. The offset is being applied after the division.
    See the descriptions of respective \c Texture variants for more detail.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division,
           coordinate offset and LOD bias.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies all of these operations: perspective
    division, coordinate offset and LOD bias. The offset is being applied after
    the division. See the descriptions of respective \c Texture variants for more
    detail.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset, const Float& bias );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division,
           and LOD gradient.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies both perspective and LOD gradient.
    See the descriptions of respective \c Texture variants for more detail.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT, class GradT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords, const GradT& gradX, const GradT& gradY );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division,
           coordinate offset and LOD gradient.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies all of these operations: perspective
    division, coordinate offset and LOD gradient. The offset is being applied after
    the division. See the descriptions of respective \c Texture variants for more
    detail.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT, class OffsetT, class GradT >
auto TextureProj ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset, const GradT& gradX, const GradT& gradY );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division and explicit MIP
           levels selection.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies perspective division and MIP levels
    selection.
    
    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT >
auto TextureLodProj ( const TextureT& texture, const CoordsT& coords, const Float& lod );

// -----------------------------------------------------------------------------
/**
    \brief Samples a texel from a texture with perspective division,
           coordinate offset and explicit MIP levels selection.

    This function allows to read a texel with sampling and filtering. It differs
    from the base version in that it applies all of these operations: perspective
    division, coordinate offset and LOD level selection. The offset is being applied after
    the division. See the descriptions of respective \c Texture variants for more
    detail.

    The function may be used also in other shaders than fragment.

    Remaining details are identical as in the base \c TextureProj function.
*/

template< class TextureT, class CoordsT, class OffsetT >
auto TextureLodProj ( const TextureT& texture, const CoordsT& coords, const OffsetT& offset, const Float& lod );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T >
auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T >
auto TextureDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4, const Arg5T& arg5 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureLodDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
auto TextureLodDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T >
auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T >
auto TextureProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4, const Arg5T& arg5 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureLodProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T, class Arg4T >
auto TextureLodProjDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T >
auto TextureGather ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureGather ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
/**
    As the first argument, you may provide an object of the following types:
    inSampledTexture, inConstSampledTexture.
*/

template< class TextureT, class Arg1T, class Arg2T, class Arg3T >
auto TextureGatherDref ( const TextureT& texture, const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
