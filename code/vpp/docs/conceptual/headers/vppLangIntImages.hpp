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
    \brief Storage (read-write) image binding point.

    Place inside your custom PipelineConfig (or ComputePipelineConfig) derived class
    to define a binding point for storage image, which can be read or written to.

    The binding point template is parameterized by the view type. In order to construct
    the proper type to be used as an argument here, you need to do two things.

    The first thing is to define an image format. The recommended way is to make a
    typedef of vpp::format< ... > template. For example:

    \code
        typedef vpp::format<
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t > MyImageFormat;
    \endcode

    Having defined the format type, declare view type as follows:

    \code
        typedef vpp::ImageAttributes<
            MyImageFormat,               // your defined format
            RENDER,                      // always use RENDER for textures
            IMG_TYPE_2D,                 // number of dimensions

            // for storage images, always include STORAGE, other flags are optional
            vpp::Img::STORAGE | vpp::Img::TARGET,

            VK_IMAGE_TILING_OPTIMAL,     // most of the time use OPTIMAL
            VK_SAMPLE_COUNT_1_BIT,       // usually textures have 1 sample per pixel
            false,                       // whether mip-mapped
            false                        // whether arrayed
        > MyImageAttr;

        typedef vpp::Image< MyImageAttr > MyStorageImage;
        typedef vpp::ImageViewAttributes< MyStorageImage > MyImageViewAttr;
        typedef vpp::ImageView< MyImageViewAttr > MyImageView;
    \endcode

    Now you can use these types as follows:
    - \c MyStorageImage as runtime image class,
    - \c MyImageView as runtime image view class, to bind views to binding points,
    - also \c MyImageView as the parameter to the binding point.

    \code
        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::ioImage< MyImageView > m_myImage;
            // ...
        };
    \endcode

    This binding point can be used in the following image functions:
    ImageLoad(), ImageStore(), ImageSize(), ImageQuerySamples(), ImageQueryLevels().
*/

template< class ImageViewT, unsigned int MEMFLAGS = 0 >
class ioImage
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    ioImage ( unsigned int set = 0, int binding = -1 );

    /** \brief Binds an image view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        This variant assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.
    */
    auto operator= ( const ImageViewT& view );

    /** \brief Binds an image view to the binding point.

        Same as the other assignment operator, but allows to explicitly specify 
        the layout the image is in. For storage images however, Vulkan always
        requires VK_IMAGE_LAYOUT_GENERAL layout, hence using this overload is
        deprecated.
    */
    auto operator= ( bind ( const ImageViewT& view, VkImageLayout layout ) );
    
    /**
        \brief The type of the atomic variable which can be stored in the image.
        
        Caution: only scalar (single component) formats may support atomic variables.
        In fact the only formats that are required by Vulkan to support atomic variables
        are: \c vpp::format<int> (\c VK_FORMAT_R32_SINT) and <tt>vpp::format<unsigned int></tt>
        (\c VK_FORMAT_R32_UINT).
    */
    typedef <implementation_defined> scalar_type;

    /**
        \brief Type of coordinates within the image used to locate texels
        for making pointers.

        This can be \c Int, \c IVec2, \c IVec3 or \IVec4 depending on number
        of image dimensions. Image arrays add one extra dimension. Also cube
        map images add one extra dimension.
    */
    typedef <implementation_defined> pointer_coord_type;

    /**
        \brief Creates a pointer to specified texel within the image.

        This method allows to place atomic variables inside an image.

        Caution: atomic variables can be quite slow.
    */
    Pointer< scalar_type > GetPointer (
        const pointer_coord_type& coords );

    /**
        \brief Creates a pointer to specified texel sample within the image.

        This method allows to place atomic variables inside an image. The
        overload allows to specify sample index, and this way to access individual
        samples within multisampled images.

        Caution: atomic variables can be quite slow.
    */
    Pointer< scalar_type > GetPointer (
        const pointer_coord_type& coords,
        const Int& nSample );
};

// -----------------------------------------------------------------------------
/**
    \brief Sampler binding point.

    Place inside your custom PipelineConfig (or ComputePipelineConfig) derived class
    to define a binding point for texture sampler. This sampler can be asociated with
    a texture by means of MakeSampledTexture() function.

    In this variant, the sampler is bound dynamically at runtime to a shader data block.
    Use the assignment operator for that. Provide a vpp::NormalizedSampler or
    vpp::UnnormalizedSampler object. The sampler can be changed later by binding
    a different sampler.
*/

template< class SamplerT >
class inSampler
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inSampler ( unsigned int set = 0, int binding = -1 );

    /** \brief Binds a sampler object to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        Provide a vpp::NormalizedSampler or vpp::UnnormalizedSampler object
        as the argument. The sampler can be changed later by binding
        a different sampler.
    */
    auto operator= ( const SamplerT& value );
};

// -----------------------------------------------------------------------------
/**
    \brief Static sampler binding point.

    Place inside your custom PipelineConfig (or ComputePipelineConfig) derived class
    to define a binding point for texture sampler. This sampler can be asociated with
    a texture by means of MakeSampledTexture() function.

    In this variant, the sampler is bound statically to the binding point.
    Provide a vpp::NormalizedSampler or vpp::UnnormalizedSampler object to the
    constructor. The sampler cannot be changed.
*/

template< class SamplerT >
class inConstSampler
{
public:
    /**
        \brief Creates the binding point.

        Provide a vpp::NormalizedSampler or vpp::UnnormalizedSampler object to the
        constructor. The sampler cannot be changed.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inConstSampler ( const SamplerT& sampler, unsigned int set = 0, int binding = -1 );
};

// -----------------------------------------------------------------------------
/**
    \brief Texture (read-only) image binding point.

    This variant of texture binding point is meant to be sampled, but is not
    associated with a sampler. You must manually associate it with a sampler
    in the shader code, by means of MakeSampledTexture() function. It will
    convert the pure texture to texture-sampler pair. The pair can be provided
    to sampling functions, e.g. Texture(), TextureLod(), etc.

    Otherwise the raw texture may only be used for functions: MakeSampledTexture(),
    ImageSize(), TextureSize(), ImageQuerySamples(), ImageQueryLevels(), TexelFetch(),
    TexelFetchLod(), TexelFetchOffset().

    The binding point template is parameterized by the view type. In order to construct
    the proper type to be used as an argument here, you need to do two things.

    The first thing is to define an image format. The recommended way is to make a
    typedef of vpp::format< ... > template. For example:

    \code
        typedef vpp::format<
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t > MyImageFormat;
    \endcode

    However, in case of textures the format is often not known at compile time
    (especially when the texture is in compressed format). In such case, use the
    special vpp::texture_format tag:

    \code
        typedef vpp::format< vpp::texture_format > MyImageFormat;
    \endcode

    The difference is that you should provide format code at runtime to the
    image constructor. Otherwise VPP will create an image of unknown format,
    which may (or may not) be supported by the device.

    Having defined the format type, declare view type as follows:

    \code
        typedef vpp::ImageAttributes<
            MyImageFormat,               // your defined format
            RENDER,                      // always use RENDER for textures
            IMG_TYPE_2D,                 // number of dimensions

            // for textures, always include SAMPLED, other flags are optional
            vpp::Img::SAMPLED | vpp::Img::TARGET,

            VK_IMAGE_TILING_OPTIMAL,     // most of the time use OPTIMAL
            VK_SAMPLE_COUNT_1_BIT,       // usually textures have 1 sample per pixel
            true,                        // whether mip-mapped
            false                        // whether arrayed
        > MyTextureAttr;

        typedef vpp::Image< MyTextureAttr > MyTextureImage;
        typedef vpp::ImageViewAttributes< MyTextureImage > MyTextureViewAttr;
        typedef vpp::ImageView< MyTextureViewAttr > MyTextureView;
    \endcode

    Now you can use these types as follows:
    - MyTextureImage as runtime image class,
    - MyTextureView as runtime image view class, to bind views to binding points
    - also MyTextureView as the parameter to the binding point.

    \code
        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::inTexture< MyTextureView > m_myTextureInput;
            // ...
        };
    \endcode
*/

template< class ImageViewT >
class inTexture
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inTexture ( unsigned int set = 0, int binding = -1 );

    /**
        \brief Binds an image view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        This overload assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.
    */
    auto operator= ( const ImageViewT& value );

    /**
        \brief Binds an image view to the binding point.

        This overload allows to specify the layout the image is in when
        accessing the binding point. For that purpose, use \c vpp::bind
        construct.
    */
    auto operator= ( vpp::bind ( const ImageViewT& view, VkImageLayout layout ) );
};

// -----------------------------------------------------------------------------
/**
    \brief A binding point for read-only texture with associated sampler.

    Note that for this type of binding point you must use bind() function in order
    to bind an image and sampler simultaneously. An example:

    \code
        void MyPipeline :: setTextureInput (
            vpp::ShaderDataBlock* pDataBlock,
            const MyTextureView& texture,
            const vpp::NormalizedSampler& sampler )
        {
            pDataBlock->update ( (
                m_myTextureInput = vpp::bind ( sampler, texture )
            ) );
        }
    \endcode

    The binding point template is parameterized by the view type. In order to construct
    the proper type to be used as an argument here, you need to do two things.

    The first thing is to define an image format. The recommended way is to make a
    typedef of vpp::format< ... > template. For example:

    \code
        typedef vpp::format<
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t > MyImageFormat;
    \endcode

    However, in case of textures the format is often not known at compile time
    (especially when the texture is in compressed format). In such case, use the
    special vpp::texture_format tag:

    \code
        typedef vpp::format< vpp::texture_format > MyImageFormat;
    \endcode

    The difference is that you should provide format code at runtime to the
    image constructor. Otherwise VPP will create an image of unknown format,
    which may (or may not) be supported by the device.

    Having defined the format type, declare view type as follows:

    \code
        typedef vpp::ImageAttributes<
            MyImageFormat,               // your defined format
            RENDER,                      // always use RENDER for textures
            IMG_TYPE_2D,                 // number of dimensions

            // for textures, always include SAMPLED, other flags are optional
            vpp::Img::SAMPLED | vpp::Img::TARGET,

            VK_IMAGE_TILING_OPTIMAL,     // most of the time use OPTIMAL
            VK_SAMPLE_COUNT_1_BIT,       // usually textures have 1 sample per pixel
            true,                        // whether mip-mapped
            false                        // whether arrayed
        > MyTextureAttr;

        typedef vpp::Image< MyTextureAttr > MyTextureImage;
        typedef vpp::ImageViewAttributes< MyTextureImage > MyTextureViewAttr;
        typedef vpp::ImageView< MyTextureViewAttr > MyTextureView;
    \endcode

    Now you can use these types as follows:
    - MyTextureImage as runtime image class,
    - MyTextureView as runtime image view class, to bind views to binding points
    - also MyTextureView as the parameter to the binding point.

    \code
        class MyPipelineConfig : public vpp::PipelineConfig
        {
            // ...
            vpp::inSampledTexture< MyTextureView > m_myTextureInput;
            // ...
        };
    \endcode

    This binding point can be used in the following image functions:
    ExtractSampledTexture(), ImageSize(), TextureSize(), ImageQuerySamples(),
    ImageQueryLevels(), TextureQueryLod(), TexelFetch(), TexelFetchLod(),
    TexelFetchOffset(), Texture(), TextureLod(), TextureProj(), TextureLodProj(),
    TextureDref(), TextureLodDref(), TextureProjDref(), TextureLodProjDref(),
    TextureGather(), TextureGatherDref().
*/

template< class ImageViewT >
class inSampledTexture
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inSampledTexture ( unsigned int set = 0, int binding = -1 );

    /**
        \brief Binds an image view and normalized sampler to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        This overload assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.

        Use \c vpp::bind construct to provide both image and sampler.
    */
    auto operator= ( vpp::bind ( const ImageViewT& view, const NormalizedSampler& sampler ) );

    /**
        \brief Binds an image view and unnormalized sampler to the binding point.

        This overload assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.

        Use \c vpp::bind construct to provide both image and sampler.
    */
    auto operator= ( vpp::bind ( const ImageViewT& view, const UnnormalizedSampler& sampler ) );

    /**
        \brief Binds an image view and normalized sampler to the binding point.

        This overload allows to specify the layout the image is in when
        accessing the binding point.

        Use \c vpp::bind construct to provide image, sampler and layout.
    */
    auto operator= ( vpp::bind ( const ImageViewT& view, const NormalizedSampler& sampler, VkImageLayout layout ) );

    /**
        \brief Binds an image view and unnormalized sampler to the binding point.

        This overload allows to specify the layout the image is in when
        accessing the binding point.
        
        Use \c vpp::bind construct to provide image, sampler and layout.
    */
    auto operator= ( vpp::bind ( const ImageViewT& view, const UnnormalizedSampler& sampler, VkImageLayout layout ) );
};

// -----------------------------------------------------------------------------
/**
    \brief A binding point for read-only texture with predefined (constant) sampler.

    For this type of binding point, you specify the sampler directly in the constructor
    of the binding point object.

    This type of binding point has a chance to be more efficient than variants with
    dynamically assigned samplers.

    The binding point template is parameterized by the view type. In order to construct
    the proper type to be used as an argument here, you need to do two things.

    The first thing is to define an image format. The recommended way is to make a
    typedef of vpp::format< ... > template. For example:

    \code
        typedef vpp::format<
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t,
            vpp::unorm8_t > MyImageFormat;
    \endcode

    However, in case of textures the format is often not known at compile time
    (especially when the texture is in compressed format). In such case, use the
    special vpp::texture_format tag:

    \code
        typedef vpp::format< vpp::texture_format > MyImageFormat;
    \endcode

    The difference is that you should provide format code at runtime to the
    image constructor. Otherwise VPP will create an image of unknown format,
    which may (or may not) be supported by the device.

    Having defined the format type, declare view type as follows:

    \code
        typedef vpp::ImageAttributes<
            MyImageFormat,               // your defined format
            RENDER,                      // always use RENDER for textures
            IMG_TYPE_2D,                 // number of dimensions

            // for textures, always include SAMPLED, other flags are optional
            vpp::Img::SAMPLED | vpp::Img::TARGET,

            VK_IMAGE_TILING_OPTIMAL,     // most of the time use OPTIMAL
            VK_SAMPLE_COUNT_1_BIT,       // usually textures have 1 sample per pixel
            true,                        // whether mip-mapped
            false                        // whether arrayed
        > MyTextureAttr;

        typedef vpp::Image< MyTextureAttr > MyTextureImage;
        typedef vpp::ImageViewAttributes< MyTextureImage > MyTextureViewAttr;
        typedef vpp::ImageView< MyTextureViewAttr > MyTextureView;
    \endcode

    Now you can use these types as follows:
    - MyTextureImage as runtime image class,
    - MyTextureView as runtime image view class, to bind views to binding points
    - also MyTextureView as the parameter to the binding point.

    \code
        vpp::inConstSampledTexture< MyTextureView > m_myTextureInput;
    \endcode

    This binding point can be used in the following image functions:
    ExtractSampledTexture(), ImageSize(), TextureSize(), ImageQuerySamples(),
    ImageQueryLevels(), TextureQueryLod(), TexelFetch(), TexelFetchLod(),
    TexelFetchOffset(), Texture(), TextureLod(), TextureProj(), TextureLodProj(),
    TextureDref(), TextureLodDref(), TextureProjDref(), TextureLodProjDref(),
    TextureGather(), TextureGatherDref().
*/

template< class ImageViewT >
class inConstSampledTexture
{
public:
    /**
        \brief Creates the binding point.

        You must provide valid sampler object. This constructor works with
        normalized samplers. The sampler is associated with this binding point
        permanently. 

        Typically you do not need to specify other arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inConstSampledTexture (
        const NormalizedSampler& sampler,
        unsigned int set = 0, int binding = -1 );

    /**
        \brief Creates the binding point.

        You must provide valid sampler object. This constructor works with
        unnormalized samplers. The sampler is associated with this binding point
        permanently. 

        Typically you do not need to specify other arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inConstSampledTexture (
        const UnnormalizedSampler& sampler,
        unsigned int set = 0, int binding = -1 );

    /**
        \brief Binds an image view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        This overload assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.
    */

    auto operator= ( const ImageViewT& value );

    /**
        \brief Binds an image view to the binding point.

        This overload allows to specify the layout the image is in when
        accessing the binding point. For that purpose, use \c vpp::bind
        construct.
    */
    auto operator= ( bind ( const ImageViewT& view, VkImageLayout layout ) );
};

// -----------------------------------------------------------------------------
/**
    \brief Uniform (read-only) texel buffer binding point.

    This binding point can be used in the following image functions:
    ImageSize(), TextureSize(), ImageQuerySamples(),
    ImageQueryLevels(), TextureQueryLod(), TexelFetch(), TexelFetchLod(),
    TexelFetchOffset().
*/

template< class FormatT >
class inTextureBuffer
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inTextureBuffer ( unsigned int set = 0, int binding = -1 )

    /**
        \brief Binds a buffer view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.
    */

    auto operator= ( const TexelBufferView< FormatT, Buf::UNITEX >& value );
};

// -----------------------------------------------------------------------------
/**
    \brief Storage (read-write) texel buffer binding point.

    This binding point can be used in the following image functions:
    ImageLoad(), ImageStore(), ImageSize(), ImageQuerySamples(), ImageQueryLevels().
*/

template< class FormatT >
class ioImageBuffer
{
public:
    /**
        \brief Creates the binding point.

        Typically you do not need to specify any arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    ioImageBuffer ( unsigned int set = 0, int binding = -1 );

    /**
        \brief Binds a buffer view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.
    */

    auto operator= ( const TexelBufferView< FormatT, Buf::STORTEX >& value );
};

// -----------------------------------------------------------------------------
/**
    \brief Input attachment binding point.

    Input attachment is an intermediate image node in complex render graph.
    The same node is being declared as input and output attachment. One rendering
    process writes to it (seeing it as output attachment) while others can read from it.
    For these reading processes declare the node as input and provide corresponding
    \c inAttachment binding point in each pipeline.

    You must provide a reference to render graph node in the constructor.

    The \c inAttachment must be bound to an image view just as all other input
    binding points (to the shader data block and by means of assignment operator).

    However, as the node is declared both output and input, in order to satisfy
    the output side requirements, it must be also be initialized with the image
    view in the Attachment constructor (or provide the view to FrameBuffer object).
    This must be a view to the same image.

    Note that \c addInput method will automatically add dependency between processes.
    This ensures that the second process will read complete data from the input
    attachment.

    Example:

    \code
        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph (
                const MyImageView& intView,
                const MyImageView& finalView ) :
                    m_intermediate ( intView ),
                    m_final ( finalView )
            {
                m_process1.addColorOutput (
                    m_intermediate, 0.0f, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

                m_process2.addInput (
                    m_intermediate, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

                m_process2.addColorOutput ( m_final, 0.0f );
            }

        public:
            vpp::Process m_process1;
            vpp::Process m_process2;

            vpp::Attachment< MyImageFormat > m_intermediate;
            vpp::Attachment< MyImageFormat > m_final;
        };

        class MyPipeline1 : public vpp::PipelineConfig
        {
        public:
            MyPipeline1 (
                const vpp::Process& hProcess1, const vpp::Device& hDevice,
                const Attachment< MyImageFormat >& intAtt ) :
                    vpp::PipelineConfig ( hProcess1, hDevice ),
                    m_intermediate ( intAtt ),
                    m_fragmentShader ( this, & MyPipeline1::fragmentShader )
            {}

            void fragmentShader ( vpp::FragmentShader* pShader );
            {
                using namespace vpp;
                const Vec4 intValue = ...; // compute intValue
                m_intermediate = intValue;
            }

        private:
            vpp::outAttachment< MyImageFormat > m_intermediate;
            vpp::fragmentShader m_fragmentShader;
        };

        class MyPipeline2 : public vpp::PipelineConfig
        {
        public:
            MyPipeline2 (
                const vpp::Process& hProcess2, const vpp::Device& hDevice,
                const Attachment< MyImageFormat >& intAtt,
                const Attachment< MyImageFormat >& finalAtt ) :
                    vpp::PipelineConfig ( hProcess2, hDevice ),
                    m_intermediate ( intAtt ),
                    m_final ( finalAtt ),
                    m_fragmentShader ( this, & MyPipeline2::fragmentShader )
            {}

            void setInputImage (
                ShaderDataBlock* pDataBlock,
                const MyImageView& intImageView )
            {
                pDataBlock->update ( (
                    m_intermediate = intImageView
                ) );
            }

            void fragmentShader ( vpp::FragmentShader* pShader );
            {
                using namespace vpp;
                const Vec4 inValue = SubpassLoad ( m_intermediate );
                const Vec4 outValue = ...; // compute outValue from inValue
                m_final = outValue;
            }

        private:
            vpp::inAttachment< MyImageView > m_intermediate;
            vpp::outAttachment< MyImageFormat > m_final;
            vpp::fragmentShader m_fragmentShader;
        };

    \endcode

    This binding point can only be used in SubpassLoad() function.
*/

template< class ImageViewT, unsigned int COUNT = 1 >
class inAttachment
{
public:
    typedef typename ImageViewT::format_type format_type;
    typedef TImageBinding< ImageViewT > assignment_type;

    /**
        \brief Creates the binding point.

        Specify the attachment node (from render graph) as the first argument.

        Typically you do not need to specify remaining arguments for the constructor.

        Optionally you can force the set and binding index. This feature may
        be useful if you need to interface VPP binding point with externally
        supplied shader (written in GLSL and compiled externally to SPIR-V blob).
    */
    inAttachment (
        const Attachment< format_type >& attachmentNode,
        unsigned int set = 0,
        int binding = -1 );

    /**
        \brief Binds an image view to the binding point.

        This operator returns a value that must be passed to ShaderDataBlock::update()
        method. You can also make a list of more assignments, joining them with comma
        operator. The update method accepts such a list.

        The binding is stored inside ShaderDataBlock instance, immediately
        when \c update is called. You select active ShaderDataBlock
        in your drawing command sequence by calling ShaderDataBlock::cmdBind().
        Thus, actual resource binding (to the pipeline) occurs at command execution
        time, simultaneously for all bindings in that ShaderDataBlock instance.

        This overload assumes the image is in VK_IMAGE_LAYOUT_GENERAL layout.
    */

    auto operator= ( const ImageViewT& view );

    /**
        \brief Binds an image view to the binding point.

        This overload allows to specify the layout the image is in when
        accessing the binding point.

        Use \c vpp::bind construct to provide image, sampler and layout.
    */
    auto operator= ( bind ( const ImageViewT& view, VkImageLayout layout ) )
};

// -----------------------------------------------------------------------------
/**
    \brief Output attachment binding point.

    This is the primary means to generate images in fragment shaders.
    
    Each \c outAttachment binding point is associated with corresponding node
    in render graph. You must provide a reference to that node in the constructor.
    Both Display and Attachment nodes are accepted.

    You also do not bind an image view to the output attachment (as with other
    binding points). Instead, images are bound to Attachment and Display nodes in
    the render graph. This binding occurs either directly (providing surface or
    image view reference to the attachment constructor), or indirectly, through
    explicit FrameBuffer object. The simplest method is to use the constructor.

    The template argument should be a typedef to \c vpp::format template instance.
    It should be exactly the same format as used for the Attachment node. For display
    nodes use \c FormatF32x4 format as defined in the example below (assume RGBA
    order).

    An example:

    \code
        typedef vpp::format< float, float, float, float > FormatF32x4;

        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph ( const vpp::Surface& hSurface ) :
                m_display ( hSurface )
            {
                m_render.addColorOutput ( m_display );
            }

        public:
            vpp::Process m_render;
            vpp::Display m_display;
        };

        class MyRenderPipeline : public vpp::PipelineConfig
        {
        public:
            MyRenderPipeline (
                const vpp::Process& pr,
                const vpp::Device& dev,
                const vpp::Display& outImage ) :
                    vpp::PipelineConfig ( pr ),
                    m_outColor ( outImage ),
                    // ...
                    m_fragmentShader ( this, & MyRenderPipeline::fFragmentShader )
            {}

            // ...

            void fFragmentShader ( vpp::FragmentShader* pShader )
            {
                using namespace vpp;
                const Vec4 color = ... ; // compute color value
                m_outColor = color;
            }

        private:
            // ...
            vpp::outAttachment< FormatF32x4 > m_outColor;
            vpp::fragmentShader m_fragmentShader;
        };

    \endcode

    This binding point can not be used in any image functions. Use assignment operator
    to write the pixel value to it.
*/

template< class FormatT >
struct outAttachment
{
    /**
        \brief The type of the pixel color value.
        
        This is a typedef indicating the type of the value this attachment
        expects as output value in the shader. It depends on the image format
        provided as the template argument. It can be vector or scalar.

        The number of vector components corresponds to the format component count.
        Individual component type can be \c Float, \c Double, \c Int or \c UInt
        depending also on the format. Most formats use \c Float type (e.g.
        \c float, \c vpp::float16_t, \c vpp::unorm_t, \c vpp::snorm_t).
        Formats defined from standard integer or unsigned integer types yield
        \c Int or \c UInt shader types. In case of vectors
        we have e.g. \c Vec4 for RGBA formats, \c Vec3 for RGB, etc. (and respectively
        \c IVec4, \c IVec3, \c UVec4, \c UVec3).
    */

    typedef <implementation_defined> rvalue_type;

    /**
        \brief Creates the binding point.

        This constructor applies to regular (memory output) attachments.

        Specify the attachment node (from render graph) as the only argument.
    */
    outAttachment ( const Attachment< FormatT >& attachmentNode );

    /**
        \brief Creates the binding point.

        This constructor applies to display attachments.

        Specify the display node (from render graph) as the only argument.
    */
    outAttachment ( const Display& attachmentNode );

    /**
        \brief Writes output color value to the attachment.

        Use the operator inside fragment shader code to set resulting color
        for currently processed pixel.

        Note that the meaning of the assignment operator is different here than
        for other binding points.
    */
    void operator= ( const rvalue_type& value );

    /**
        \brief Writes output color value to the attachment.

        Use this function inside fragment shader code to set resulting color
        for currently processed pixel. It is equivalent to the assignment
        operator, however it accepts one additional argument.

        The \c index argument can have value \c 0 or \c 1 and specifies which
        input of the blending unit the color value will be directed to. See
        chapter 14.3 of the official Vulkan specification for more detail.
    */
    void assign ( const rvalue_type& value, int index = 0 );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/**
    \brief Provides shader code access to a binding point for a buffer holding
           array of formatted texels. These data objects are either simple scalars
           or vectors, defined by vpp::format object.

    Place TexelArray inside your shader code to access the data in uniform
    or storage texel buffer. This is a simpler alternative to use image
    access functions directly.

    Texel buffers are a hybrid of images and buffers. They are one-dimensional,
    can hold only arrays of simple data and are accessed via image functions
    (e.g. \c ImageStore or \c TexelFetch). VPP provides the TexelArray
    accessor to allow using these buffers like regular buffers, with indexing
    operator instead of function calls.
    
    The template argument should be the decltype of accessed binding point. 
    It accepts the following binding points: inTextureBuffer, ioImageBuffer.
    Also specify the binding point in the constructor of TexelArray. The binding point
    should be an object inside PipelineConfig subclass and the shader should be
    a method in the same subclass - or some code called from it.

    Texel buffers are somewhat similar to uniform buffers accessed via UniformSimpleArray.
    There are however the following differences:
    - Texel buffers are more limited regarding the item type. They require a type 
      covered by vpp::format. For that reason, matrices are not supported directly
      (although you can encode them over ranges of vectors).
    - Texel buffers have less size restrictions. Usually the hardware limits maximum
      size of both uniform buffers and texel buffers, however limits for the latter
      are much higher.
    - Texel buffers allow some tight data packaging schemes (according to vpp::format used).
      Caution: you must check whether the hardware actually supports such a format.
      Use PhysicalDevice::supportsFormat() for that, with PhysicalDevice::BUFFER
      usage argument.

    The GPU-side type (returned by indexing operators) is automatically inferred
    from the vpp::format type of the buffer binding point.
*/

template< class BufferT >
struct TexelArray
{
    /** \brief Constructs accessor for given texel buffer binding point.
    
        The constructor of the accessor does not create ny objects in the
        SPIR-V code. This is a VPP level wrapper over 1D image functions
        which operate on the buffer.

        As the argument, provide reference to the binding point.
    */
    TexelArray ( const BufferT& buf );

    /** \brief Provides read and write access to specified item.
    
        Returns a type inferred from the host data type, allowing to read or
        write the array (in case of uniform texel buffers, only read).
        
        Uniform buffers are read-only. It is an error to attempt to write them.
        Storage texel buffers support both reading and writing.
    */
    auto operator[]( const Int& index ) const;

    /** \brief Returns GPU-side value equal to the size of the array.
    
        The size determined by this function is dynamic. It is equal to actual
        texel buffer size.
    */
    Int Size() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
