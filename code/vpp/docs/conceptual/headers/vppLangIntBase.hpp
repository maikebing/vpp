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
    \brief Template allowing to construct arrays of binding points.

    This template is used as a wrapper over binding point classes to create
    arrays of binding points. The purpose of such arrays is to allow dynamic
    selection of binding points in shader, based on GPU-level index. Arrays
    of binding points are static, so their size must be known in advance.

    The arrayOf template is applicable for the following binding point
    classes: inUniformBuffer, inUniformBufferDyn, ioBuffer, ioBufferDyn,
    ioImageBuffer, inTextureBuffer, ioImage, inSampler, inConstSampler,
    inTexture, inSampledTexture, inConstSampledTexture.

    Arrays of data buffers (inUniformBuffer, inUniformBufferDyn, ioBuffer,
    ioBufferDyn) are being accessed slightly differently than arrays of
    images and texel buffers (ioImageBuffer, inTextureBuffer, ioImage, inSampler, inConstSampler,
    inTexture, inSampledTexture, inConstSampledTexture). These differences will
    be explained further below.

    An array of binding points in VPP corresponds to array of descriptors
    in Vulkan. That is, it defines proper \c count values in descriptor defining
    structures and generates array types on SPIR-V level.

    In order to declare an array of binding points, just wrap the base type
    inside arrayOf. Examples:

    \code
        // defines array of uniform buffers
        vpp::arrayOf< vpp::inUniformBuffer > m_inUniformBufferArr;

        // defines array of storage buffers.
        vpp::arrayOf< vpp::ioBuffer > m_ioBufferArr;

        // defines arrays of texel buffers
        vpp::arrayOf< vpp::ioImageBuffer< MyFormatUInt32 > > m_ioImageBufferArr;
        vpp::arrayOf< vpp::inTextureBuffer< MyFormatUInt32 > > m_inTextureBufferArr;

        // defines arrays of various image resources
        vpp::arrayOf< vpp::ioImage< MyImageView > > m_ioImageArr;
        vpp::arrayOf< vpp::inTexture< MyImageView > > m_inTextureArr;
        vpp::arrayOf< vpp::inSampledTexture< MyTextureView > > m_inSampledTextureArr;
        vpp::arrayOf< vpp::inConstSampledTexture< MyTextureView > > m_inConstSampledTextureArr;

        // defines arrays of samplers
        vpp::arrayOf< vpp::inSampler< vpp::NormalizedSampler > > m_inSamplerArr;
        vpp::arrayOf< vpp::inConstSampler< vpp::UnnormalizedSampler > > m_inConstSamplerArr;
    \endcode

    In order to construct arrayOf object, you must provide array size to the
    constructor. See the docs for individual constructors for more detail.

    In order to access an arrayed binding point in your shader code, use one of
    two methods depending on what kind of binding point is is.

    For buffer binding points, i.e. inUniformBuffer, inUniformBufferDyn, ioBuffer,
    ioBufferDyn, use UniformVar accessor. It is specialized for arrays an provides
    an \c operator[] for additional level of indirection.

    Examples:

    \code
        template< ETag TAG >
        struct TFramePar : public UniformStruct< TAG, TFramePar >
        {
            UniformFld< TAG, glm::mat4 > m_world2projected;
            UniformFld< TAG, glm::mat4 > m_world2view;
            // ...
        };

        typedef TFramePar< vpp::CPU > CFramePar;
        typedef TFramePar< vpp::GPU > GFramePar;

        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::arrayOf< vpp::inUniformBuffer > m_inUniformBufferArr;

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                UniformVar< TFramePar, decltype ( m_inUniformBufferArr ) >
                    varUniformBufferArr ( m_inUniformBufferArr );
                    
                Int arrayIndex = ...; // compute array index

                Mat4 w2p = varUniformBufferArr [ arrayIndex ][ & GFramePar::m_world2projected ];

                // ...
            }
        };
    \endcode

    For image binding points, i.e. ioImageBuffer, inTextureBuffer, ioImage, inSampler,
    inConstSampler, inTexture, inSampledTexture, inConstSampledTexture, use the
    indexing operator directly on arrayOf object when supplying an argument to
    the image manipulation function.

    Examples:

    \code
        class MyPipelineConfig : public vpp::PipelineConfig
        {
            vpp::inSampler< vpp::NormalizedSampler > m_inSampler;
            vpp::arrayOf< vpp::ioImageBuffer< MyFormatUInt32 > > m_ioImageBufferArr;
            vpp::arrayOf< vpp::inTextureBuffer< MyFormatUInt32 > > m_inTextureBufferArr;
            vpp::arrayOf< vpp::ioImage< MyImageView > > m_ioImageArr;
            vpp::arrayOf< vpp::inTexture< MyImageView > > m_inTextureArr;
            vpp::arrayOf< vpp::inSampledTexture< MyTextureView > > m_inSampledTextureArr;
            vpp::arrayOf< vpp::inConstSampledTexture< MyTextureView > > m_inConstSampledTextureArr;
            vpp::arrayOf< vpp::inSampler< vpp::NormalizedSampler > > m_inSamplerArr;
            vpp::arrayOf< vpp::inConstSampler< vpp::UnnormalizedSampler > > m_inConstSamplerArr;

            void fFragmentShader ( vpp::FragmentShader* pShader )
            {
                using namespace vpp;

                Int arrIdx = ...; // compute array index

                // dummy coordinates only to demonstrate function call syntax
                Int n = 0;
                Vec2 c = { 0.5f, 0.5f };
                IVec3 ci = { 7, 8, 0 };
                Float b = 0.0f;
                Vec3 d = { 0.5f, 0.5f, 0.0f };
                IVec2 i = { 7, 8 };

                IVec3 s1 = ImageSize ( m_ioImageArr [ arrIdx ] );
                IVec3 s2 = TextureSize ( m_inTextureArr [ arrIdx ], n );
                Int s3 = ImageQuerySamples ( m_inSampledTextureArr [ arrIdx ] );
                Int s4 = ImageQueryLevels ( m_inConstSampledTextureArr [ arrIdx ] );
                Vec2 s5 = TextureQueryLod ( m_inSampledTextureArr [ arrIdx ], c );
                UVec4 s6 = ImageLoad ( m_ioImageArr [ arrIdx ], ci );
                ImageStore ( m_ioImageArr [ arrIdx ], ci, s6 [ X ] );
                UVec4 s7 = TexelFetch ( m_inTextureBufferArr [ arrIdx ], n );
                UVec4 s8 = TexelFetchLod ( m_inTextureArr [ arrIdx ], ci, n );
                Vec4 s9 = TexelFetchOffset ( m_inConstSampledTextureArr [ arrIdx ], i, n, i );
                Vec4 s10 = Texture ( m_inSampledTextureArr [ arrIdx ], c );
                Vec4 s11 = TextureLod ( m_inConstSampledTextureArr [ arrIdx ], c, b );
                Vec4 s12 = TextureProj ( m_inSampledTextureArr [ arrIdx ], d, b );
                Vec4 s13 = TextureLodProj ( m_inConstSampledTextureArr [ arrIdx ], d, b );
                Vec4 s14 = TextureGather ( m_inSampledTextureArr [ arrIdx ], c, n );

                const auto st1 = MakeSampledTexture (
                    m_inTextureArr [ arrIdx ], m_inSamplerArr [ arrIdx ] );
                const auto st2 = MakeSampledTexture (
                    m_inTextureArr [ arrIdx ], m_inSampler );
            }
        };

    \endcode

*/

template< class SingleT >
class arrayOf
{
public:
    /**
        \brief Type of accessed image resource.

        This type is used only for the following binding point types:
        ioImage, inSampler, inConstSampler, inTexture, inSampledTexture,
        inConstSampledTexture, inTextureBuffer, ioImageBuffer.

        This is the type of object that goes to image manipulation
        function inside the shader. The object is obtained by indexing
        the arrayOf object (by means of \c operator[]).
    */

    typedef <implementation_defined> return_type;

    /**
        \brief Constructs array of binding points.

        This constructor is meant to be used with all binding point types
        except inConstSampler and inConstSampledTexture.

        Size of the array is mandatory argument.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */

    arrayOf (
        unsigned int count,
        unsigned int set = 0,
        int binding = -1 );

    /**
        \brief Constructs array of binding points.

        This constructor is meant to be used with inConstSampler and
        inConstSampledTexture binding points and normalized samplers.

        You must specify a list of samplers to be statically bound to the image.

        Size of the array is also mandatory argument. If the size is larger than
        provided vector of samplers, the last sampler on the list will be duplicated
        to fill remaining positions.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */

    arrayOf (
        const std::vector< NormalizedSampler >& samplers,
        unsigned int count,
        unsigned int set = 0,
        int binding = -1 );

    /**
        \brief Constructs array of binding points.

        This constructor is meant to be used with inConstSampler and
        inConstSampledTexture binding points and unnormalized samplers.

        You must specify a list of samplers to be statically bound to the image.

        Size of the array is also mandatory argument. If the size is larger than
        provided vector of samplers, the last sampler on the list will be duplicated
        to fill remaining positions.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */

    arrayOf (
        const std::vector< UnnormalizedSampler >& samplers,
        unsigned int count,
        unsigned int set = 0,
        int binding = -1 );

    auto operator= ( const multi_value_type& value );

    /**
        \brief Indexing operator for accessing individual binding points in the array.

        This operator is used only for the following binding point types:
        ioImage, inSampler, inConstSampler, inTexture, inSampledTexture,
        inConstSampledTexture, inTextureBuffer, ioImageBuffer.

        Use it inside shaders to extract the binding point used as an argument
        for image or texture manipulation functions.
    */

    return_type operator[]( const Int& index ) const;

    /** \overload */
    return_type operator[]( const UInt& index ) const;

    /** \overload */
    return_type operator[]( int index ) const;

    /** \overload */
    return_type operator[]( unsigned int index ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
