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

#ifndef INC_VPPLANGINTIMAGES_HPP
#define INC_VPPLANGINTIMAGES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGINTBASE_HPP
#include "vppLangIntBase.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/* 
   CPU-side shader data descriptor objects for images, image buffers and samplers.
   Use them to bind CPU data to GPU pipelines.

   Those objects are accessed directly (no accessor declarations required),
   mostly by dedicated functions, e.g. ImageLoad, ImageStore, SubpassLoad,
   TexelFetch, etc.

   The only exception is outAttachment which has assign method and shader-scoped
   operator= for convenient output writing. For other types, operator= is
   used for CPU-side binding operation.

   ioImage
   inSampler
   inConstSampler
   inTexture
   inSampledTexture
   inConstSampledTexture
   inTextureBuffer
   ioImageBuffer
   inAttachment
   outAttachment

*/
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// 14.5. Shader Resource Interface
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// 13.1.1. Storage Image
// -----------------------------------------------------------------------------

template< class ImageViewT, std::uint32_t MEMFLAGS = 0 >
class ioImage :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, ImageViewT >,
    public KShaderScopedVariable
{
public:
    // Types.

    typedef typename ImageViewT::image_tag_type image_tag_type;
    typedef typename ImageViewT::format_type format_type;
    typedef typename shader_type< format_type >::scalar_type scalar_type;
    typedef TRImage< image_tag_type, scalar_type > rvalue_type;
    typedef TImageBinding< ImageViewT > assignment_type;
    typedef typename ImageViewT::image_type image_type;
    typedef typename image_type::attributes_type attributes_type;
    typedef typename image_tag_type::pointer_coord_type pointer_coord_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        ImageViewT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    // Constructors.

    // Standard constructor.
    VPP_INLINE ioImage ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, ImageViewT >( set, binding, 1u )
    {}

    // Arrayed descriptor constructor. Do not use directly.
    VPP_INLINE ioImage ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, ImageViewT >( set, binding, count )
    {}

    // Binding operator.

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        return assigner_type ( this->id(), this->d_set, this->d_binding, value );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedImage< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        pTranslator->addMemoryFlags ( varInfo.d_pointerId, MEMFLAGS );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedImage< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        pTranslator->addMemoryFlags ( varInfo.d_pointerId, MEMFLAGS );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }

    // Pointer creation.

    VPP_INLINE Pointer< scalar_type > GetPointer (
        const pointer_coord_type& coords,
        const Int& nSample )
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( ! varInfo )
        {
            typedef TTypedImage< image_tag_type, scalar_type, format_type::spvCode > typed_image;

            varInfo.d_typeId = typed_image::getType();

            varInfo.d_pointerId = KId ( pTranslator->createVariable (
                spv::StorageClassUniformConstant, varInfo.d_typeId ) );

            pTranslator->addDecoration (
                varInfo.d_pointerId, spv::DecorationDescriptorSet,
                static_cast< int >( this->d_set ) );

            pTranslator->addDecoration (
                varInfo.d_pointerId, spv::DecorationBinding,
                static_cast< int >( this->d_binding ) );

            pTranslator->addMemoryFlags ( varInfo.d_pointerId, MEMFLAGS );

            varInfo.d_variableId = KId (
                KShaderTranslator::get()->createLoad ( varInfo.d_pointerId ) );
        }

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = varInfo.d_pointerId;
        operands [ 1 ] = coords.id();
        operands [ 2 ] = ( image_tag_type::MULTISAMPLED ? nSample.id() : pTranslator->getArrayIndex ( 0u ) );

        const KId typeId = KId ( pTranslator->makePointer (
            spv::StorageClassImage, scalar_type::getType() ) );

        return Pointer< scalar_type >( KId ( pTranslator->createOp (
            spv::OpImageTexelPointer, typeId, operands ) ), spv::StorageClassImage );
    }

    VPP_INLINE Pointer< scalar_type > GetPointer (
        const pointer_coord_type& coords )
    {
        return GetPointer ( coords, KShaderTranslator::get()->getArrayIndex ( 0u ) );
    }

    // Requirements checking.

    static_assert (
        ( attributes_type::usage & VK_IMAGE_USAGE_STORAGE_BIT ),
        "Storage image resource requires 'storage' bit set in the image" );
};

// -----------------------------------------------------------------------------
// 13.1.2. Sampler
// -----------------------------------------------------------------------------

template< class SamplerT >
class inSampler :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >,
    public KShaderScopedVariable
{
public:
    typedef TRSampler< SamplerT::isUnnormalized > rvalue_type;
    typedef SamplerT assignment_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_SAMPLER,
        SamplerT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inSampler ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >( set, binding, 1u )
    {}

    VPP_INLINE inSampler ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >( set, binding, count )
    {}

    VPP_INLINE inSampler ( const Sampler&, std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >( set, binding, 1u )
    {
        static_assert ( TFalse< SamplerT >::value, "inSampler does not take sampler in the constructor" );
    }

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        return assigner_type ( this->id(), this->d_set, this->d_binding, value );
    }

    // FIXME - copying descriptors?

    // Sampler access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );
        return rvalue_type ( pTranslator->registerSamplerVariable ( this, this->d_set, this->d_binding ) );
    }

    // Sampler access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );
        return rvalue_type ( pTranslator->registerSamplerVariable ( this, indexId, this->d_set, this->d_binding, this->d_count ) );
    }
};

// -----------------------------------------------------------------------------
// Immutable sampler (13.2.1)
// -----------------------------------------------------------------------------

template< class SamplerT >
class inConstSampler :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >,
    public KShaderScopedVariable
{
public:
    typedef TRSampler< SamplerT::isUnnormalized > rvalue_type;
    typedef std::vector< SamplerT > samplers_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inConstSampler ( const samplers_type& samplers, std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >( samplers, set, binding, count )
    {}

    VPP_INLINE inConstSampler ( const SamplerT& sampler, std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >(
            samplers_type ( 1, sampler ), set, binding, 1u )
    {}

    VPP_INLINE inConstSampler ( const SamplerT& sampler, std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLER, SamplerT >(
            samplers_type ( 1, sampler ), set, binding, count )
    {}

    // FIXME - copying descriptors?

    // Sampler access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );
        return rvalue_type ( pTranslator->registerSamplerVariable ( this, this->d_set, this->d_binding ) );
    }

    // Sampler access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );
        return rvalue_type ( pTranslator->registerSamplerVariable ( this, indexId, this->d_set, this->d_binding, this->d_count ) );
    }
};

// -----------------------------------------------------------------------------
// 13.1.3. Sampled Image
// -----------------------------------------------------------------------------

template< class ImageViewT >
class inTexture :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, ImageViewT >,
    public KShaderScopedVariable
{
public:
    typedef typename ImageViewT::format_type format_type;
    typedef typename shader_type< format_type >::scalar_type scalar_type;
    typedef typename ImageViewT::image_tag_type image_tag_type;
    typedef TRTexture< image_tag_type, scalar_type > rvalue_type;
    typedef TImageBinding< ImageViewT > assignment_type;
    typedef typename ImageViewT::image_type image_type;
    typedef typename image_type::attributes_type attributes_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        ImageViewT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inTexture ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, ImageViewT >( set, binding, 1u )
    {}

    VPP_INLINE inTexture ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, ImageViewT >( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        return assigner_type ( this->id(), this->d_set, this->d_binding, value );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }

    static_assert (
        ( attributes_type::usage & VK_IMAGE_USAGE_SAMPLED_BIT ),
        "Sampled image resource requires 'sampled' bit set in the image" );
};

// -----------------------------------------------------------------------------
// 13.1.4. Combined Image Sampler
// -----------------------------------------------------------------------------

template< class ImageViewT >
class inSampledTexture :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >,
    public KShaderScopedVariable
{
public:
    typedef typename ImageViewT::format_type format_type;
    typedef typename shader_type< format_type >::scalar_type scalar_type;
    typedef typename ImageViewT::image_tag_type image_tag_type;
    typedef TRSampledTexture< image_tag_type, scalar_type > rvalue_type;
    typedef TImageSamplerBinding< ImageViewT > assignment_type;
    typedef typename ImageViewT::image_type image_type;
    typedef typename image_type::attributes_type attributes_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        ImageViewT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inSampledTexture ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >( set, binding, 1u )
    {}

    VPP_INLINE inSampledTexture ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >( set, binding, count )
    {}

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        return assigner_type ( this->id(), this->d_set, this->d_binding, value );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedSampledTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedSampledTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }

    static_assert (
        ( attributes_type::usage & VK_IMAGE_USAGE_SAMPLED_BIT ),
        "Sampled image resource requires 'sampled' bit set in the image" );
};

// -----------------------------------------------------------------------------
// 13.1.4. Image combined with immutable sampler (13.2.1)
// -----------------------------------------------------------------------------

template< class ImageViewT >
class inConstSampledTexture :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >,
    public KShaderScopedVariable
{
public:
    typedef typename ImageViewT::format_type format_type;
    typedef typename shader_type< format_type >::scalar_type scalar_type;
    typedef typename ImageViewT::image_tag_type image_tag_type;
    typedef TRSampledTexture< image_tag_type, scalar_type > rvalue_type;
    typedef TImageBinding< ImageViewT > assignment_type;
    typedef typename ImageViewT::image_type image_type;
    typedef typename image_type::attributes_type attributes_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        ImageViewT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inConstSampledTexture (
        const std::vector< NormalizedSampler >& samplers,
        std::uint32_t set, int binding, std::uint32_t count ) :
            detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >( samplers, set, binding, count )
    {}

    VPP_INLINE inConstSampledTexture (
        const std::vector< UnnormalizedSampler >& samplers,
        std::uint32_t set, int binding, std::uint32_t count ) :
            detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >( samplers, set, binding, count )
    {
        static_assert (
            ImageViewT::isUnnormSampling,
            "Unnormalized sampler can't be bound to normalized-only sampling view" );
    }

    VPP_INLINE inConstSampledTexture ( const NormalizedSampler& sampler, std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >(
            std::vector< NormalizedSampler >( 1, sampler ), set, binding, 1u )
    {}

    VPP_INLINE inConstSampledTexture ( const NormalizedSampler& sampler, std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >(
            std::vector< NormalizedSampler >( 1, sampler ), set, binding, count )
    {}

    VPP_INLINE inConstSampledTexture ( const UnnormalizedSampler& sampler, std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >(
            std::vector< UnnormalizedSampler >( 1, sampler ), set, binding, 1u )
    {
        static_assert (
            ImageViewT::isUnnormSampling,
            "Unnormalized sampler can't be bound to normalized-only sampling view" );
    }

    VPP_INLINE inConstSampledTexture ( const UnnormalizedSampler& sampler, std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ImageViewT >(
            std::vector< UnnormalizedSampler >( 1, sampler ), set, binding, count )
    {
        static_assert (
            ImageViewT::isUnnormSampling,
            "Unnormalized sampler can't be bound to normalized-only sampling view" );
    }

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        return assigner_type ( this->id(), this->d_set, this->d_binding, value );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedSampledTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedSampledTexture< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }

    static_assert (
        ( attributes_type::usage & VK_IMAGE_USAGE_SAMPLED_BIT ),
        "Sampled image resource requires 'sampled' bit set in the image" );
};

// -----------------------------------------------------------------------------
// 13.1.5. Uniform Texel Buffer
// -----------------------------------------------------------------------------

template< class FormatT >
class inTextureBuffer :
    public detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        TexelBufferView< FormatT, Buf::UNITEX > >,
    public KShaderScopedVariable
{
public:
    typedef typename shader_type< FormatT >::scalar_type scalar_type;
    typedef typename shader_type< FormatT >::rvalue_type texel_type;
    typedef TexelBufferView< FormatT, Buf::UNITEX > view_type;
    typedef TRTexture< view_type, scalar_type > rvalue_type;
    typedef view_type assignment_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        view_type, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE inTextureBuffer ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType<
            VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, view_type >( set, binding, 1u )
    {}

    VPP_INLINE inTextureBuffer ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType<
            VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, view_type >( set, binding, count )
    {}

    template< unsigned int USAGE >
    VPP_INLINE assigner_type operator= ( const TexelBufferView< FormatT, USAGE >& value )
    {
        const view_type view = view_type ( value );
        return assigner_type ( this->id(), this->d_set, this->d_binding, view );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedTexture<
            view_type, scalar_type, FormatT::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedTexture<
            view_type, scalar_type, FormatT::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }
};

// -----------------------------------------------------------------------------
// 13.1.6. Storage Texel Buffer
// -----------------------------------------------------------------------------

template< class FormatT >
class ioImageBuffer :
    public detail::TDescriptorType<
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        TexelBufferView< FormatT, Buf::STORTEX > >,
    public KShaderScopedVariable
{
public:
    typedef typename shader_type< FormatT >::scalar_type scalar_type;
    typedef typename shader_type< FormatT >::rvalue_type texel_type;
    typedef TexelBufferView< FormatT, Buf::STORTEX > view_type;
    typedef TRImage< view_type, scalar_type > rvalue_type;
    typedef view_type assignment_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        view_type, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = true;

    VPP_INLINE ioImageBuffer ( std::uint32_t set = 0, int binding = -1 ) :
        detail::TDescriptorType<
            VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, view_type >( set, binding, 1u )
    {}

    VPP_INLINE ioImageBuffer ( std::uint32_t set, int binding, std::uint32_t count ) :
        detail::TDescriptorType<
            VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, view_type >( set, binding, count )
    {}

    template< unsigned int USAGE >
    VPP_INLINE assigner_type operator= ( const TexelBufferView< FormatT, USAGE >& value )
    {
        const view_type view = view_type ( value );
        return assigner_type ( this->id(), this->d_set, this->d_binding, view );
    }

    // FIXME - copying descriptors?

    // Image access operator for single descriptor.

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedImage< view_type, scalar_type, FormatT::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        varInfo.d_variableId = KId ( pTranslator->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    // Image access operator for arrayed descriptor. Do not use directly.

    VPP_INLINE rvalue_type getArrayedDescriptor ( const KId& indexId ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( pTranslator->loadDescriptorFromArray (
                varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );

        typedef TTypedImage< view_type, scalar_type, FormatT::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createArrayedDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding, this->d_count );

        return rvalue_type ( pTranslator->loadDescriptorFromArray (
            varInfo.d_pointerId, indexId, varInfo.d_typeId ), varInfo.d_typeId );
    }
};

// -----------------------------------------------------------------------------
// 13.1.11. Input Attachment; 14.4. Fragment Input Attachment Interface
// -----------------------------------------------------------------------------

template< class ImageViewT >
class inAttachment :
    public detail::TDescriptorType< VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, ImageViewT >,
    public KShaderScopedVariable
{
public:
    typedef typename ImageViewT::format_type format_type;
    typedef typename shader_type< format_type >::scalar_type scalar_type;
    typedef typename ImageViewT::image_tag_type image_tag_type;
    typedef TRInput< image_tag_type, scalar_type > rvalue_type;
    typedef TImageBinding< ImageViewT > assignment_type;
    typedef typename ImageViewT::image_type image_type;
    typedef typename image_type::attributes_type attributes_type;

    typedef detail::TDescriptorAssigner<
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
        ImageViewT, assignment_type > assigner_type;

    static const bool isDescriptorArrayAllowed = false;

    VPP_INLINE inAttachment (
        const Attachment< format_type >& dataNode,
        std::uint32_t set,
        int binding, 
        std::uint32_t count ) :
            detail::TDescriptorType< VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, ImageViewT >( set, binding, count ),
            d_attachmentIndex ( 0 ),
            d_layout ( VK_IMAGE_LAYOUT_GENERAL )
    {
        const PipelineConfig* pConfig = PipelineConfig::getInstance();
        const RenderGraph& renderGraph = pConfig->getRenderGraph();
        const std::uint32_t attachmentIndex = dataNode.index();
        const std::uint32_t processIndex = pConfig->getProcessIndex();

        const SubpassDescriptions& spd = renderGraph.getSubpassDescriptions();
        const unsigned int nInputAttachments = spd [ processIndex ].inputAttachmentCount;

        for ( unsigned int i = 0; i != nInputAttachments; ++i )
            if ( spd [ processIndex ].pInputAttachments [ i ].attachment == attachmentIndex )
            {
                d_layout = spd [ processIndex ].pInputAttachments [ i ].layout;
                break;
            }        

        const std::uint32_t location =
            renderGraph.findInputLocation ( processIndex, attachmentIndex );

        if ( location != 0xFFFFFFFFu )
            d_attachmentIndex = location;
    }

    VPP_INLINE inAttachment (
        const Attachment< format_type >& dataNode,
        std::uint32_t set = 0,
        int binding = -1 ) :
            inAttachment ( dataNode, set, binding, 1u )
    {
    }

    VPP_INLINE assigner_type operator= ( const assignment_type& value )
    {
        const assignment_type newAssignment ( value.d_view, d_layout );
        return assigner_type ( this->id(), this->d_set, this->d_binding, newAssignment );
    }

    // FIXME - copying descriptors?

    VPP_INLINE operator rvalue_type() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        this->addStageFlag ( pTranslator->getStage() );

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( varInfo )
            return rvalue_type ( varInfo.d_variableId, varInfo.d_typeId );

        typedef TTypedInput< image_tag_type, scalar_type, format_type::spvCode > typed_image;

        varInfo.d_typeId = typed_image::getType();

        varInfo.d_pointerId = pTranslator->createDescriptor (
            varInfo.d_typeId, this->d_set, this->d_binding );

        pTranslator->addDecoration (
            varInfo.d_pointerId, spv::DecorationInputAttachmentIndex,
            d_attachmentIndex );

        varInfo.d_variableId = KId (
            KShaderTranslator::get()->createLoad ( varInfo.d_pointerId ) );

        return typed_image ( varInfo.d_variableId );
    }

    static_assert (
        ( attributes_type::usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ),
        "Input image resource requires 'input attachment' bit set in the image" );

private:
    int d_attachmentIndex;
    VkImageLayout d_layout;
};

// -----------------------------------------------------------------------------
// 14.3. Fragment Output Interface
// -----------------------------------------------------------------------------

template< class FormatT >
struct outAttachment : public KShaderScopedVariable
{
    typedef typename shader_type< FormatT >::rvalue_type rvalue_type;

    static const bool isDescriptorArrayAllowed = false;

    VPP_INLINE outAttachment ( const Attachment< FormatT >& attachmentNode ) :
        d_attachmentLocation ( 0 )
    {
        const PipelineConfig* pConfig = PipelineConfig::getInstance();
        const RenderGraph& renderGraph = pConfig->getRenderGraph();
        const std::uint32_t attachmentIndex = attachmentNode.index();
        const std::uint32_t processIndex = pConfig->getProcessIndex();

        const std::uint32_t location =
            renderGraph.findOutputLocation ( processIndex, attachmentIndex );

        if ( location != 0xFFFFFFFFu )
            d_attachmentLocation = location;
    }

    VPP_INLINE outAttachment ( const Display& attachmentNode ) :
        d_attachmentLocation ( 0 )
    {
        const PipelineConfig* pConfig = PipelineConfig::getInstance();
        const RenderGraph& renderGraph = pConfig->getRenderGraph();
        const std::uint32_t attachmentIndex = attachmentNode.index();
        const std::uint32_t processIndex = pConfig->getProcessIndex();

        const std::uint32_t location =
            renderGraph.findOutputLocation ( processIndex, attachmentIndex );

        if ( location != 0xFFFFFFFFu )
            d_attachmentLocation = location;
    }

    VPP_INLINE void assign ( const rvalue_type& value, int index = 0 )
    {
        // FIXME: here Component declarations are not supported, as there
        // is no clear explanation how to implement this, e.g. what type
        // should be the variable? Vector type or component type?
        // Can't be a component type, because spec says "variable without
        // Component dec. is assumed to have Component 0 dec." that means
        // in either case (component or not) the variable must have the same
        // type -- vector type. But now we must assign entire vector,
        // how to do it when the target is component? Is it just masked?
        // No, because assumed "Component 0" would write only red channel...
        // So how is this crap supposed to work?

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        KShaderTranslator::SVariableInfo& varInfo =
            pTranslator->getShaderScopedVariable ( this );

        if ( ! varInfo )
        {
            varInfo.d_typeId = rvalue_type::getType();

            varInfo.d_variableId = KId ( pTranslator->createVariable (
                spv::StorageClassOutput, varInfo.d_typeId ) );

            pTranslator->addDecoration (
                varInfo.d_variableId, spv::DecorationLocation,
                d_attachmentLocation );

            if ( index )
                pTranslator->addDecoration (
                    varInfo.d_variableId, spv::DecorationIndex,
                    index );

            pTranslator->registerInputOutputVariable ( varInfo.d_variableId );
        }

        KShaderTranslator::get()->createStore ( value.id(), varInfo.d_variableId );
    }

    VPP_INLINE void operator= ( const rvalue_type& value )
    {
        assign ( value );
    }

private:
    int d_attachmentLocation;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class SamplerT >
class arrayOf< inConstSampler< SamplerT > >
{
public:
    typedef inConstSampler< SamplerT > SingleT;
    typedef typename SingleT::rvalue_type return_type;

    VPP_INLINE arrayOf (
        const std::vector< NormalizedSampler >& samplers,
        std::uint32_t count,
        std::uint32_t set = 0,
        int binding = -1 ) :
            d_resource ( samplers, set, binding, count ),
            d_count ( count )
    {}

    VPP_INLINE arrayOf (
        const std::vector< UnnormalizedSampler >& samplers,
        std::uint32_t count,
        std::uint32_t set = 0,
        int binding = -1 ) :
            d_resource ( samplers, set, binding, count ),
            d_count ( count )
    {}

    VPP_INLINE SingleT& resource()
    {
        return d_resource;
    }

    VPP_INLINE const SingleT& resource() const
    {
        return d_resource;
    }

    VPP_INLINE std::uint32_t count() const
    {
        return d_count;
    }

    template< typename IndexT >
    VPP_INLINE return_type getItem ( const IndexT& index ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();
        return d_resource.getArrayedDescriptor ( pTranslator->getArrayIndex ( index ) );
    }

    VPP_INLINE return_type operator[]( const Int& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( const UInt& index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( int index ) const { return getItem ( index ); }
    VPP_INLINE return_type operator[]( unsigned int index ) const { return getItem ( index ); }

private:
    SingleT d_resource;
    std::uint32_t d_count;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class BufferT >
struct TexelAccessor
{
};

// -----------------------------------------------------------------------------

template< class FormatT >
struct TexelAccessor< inTextureBuffer< FormatT > >
{
public:
    typedef inTextureBuffer< FormatT > buffer_type;
    typedef typename buffer_type::texel_type rvalue_type;

    VPP_INLINE TexelAccessor ( const buffer_type& bpoint, const Int& index ) :
        d_buffer ( bpoint ),
        d_index ( index )
    {}

    VPP_INLINE operator rvalue_type() const
    {
        const rvalue_type rv = TexelFetch ( d_buffer, d_index );
        return rv;
    }

    template< typename SourceT >
    VPP_INLINE void operator= ( const SourceT& rhs )
    {
        static_assert ( TFalse< SourceT >::value, "Attempt to write to read-only buffer." );
    }

private:
    const buffer_type& d_buffer;
    Int d_index;
};

// -----------------------------------------------------------------------------

template< class FormatT >
struct TexelAccessor< ioImageBuffer< FormatT > >
{
public:
    typedef ioImageBuffer< FormatT > buffer_type;
    typedef typename buffer_type::texel_type rvalue_type;

    VPP_INLINE TexelAccessor ( const buffer_type& bpoint, const Int& index ) :
        d_buffer ( bpoint ),
        d_index ( index )
    {}

    VPP_INLINE operator rvalue_type() const
    {
        const rvalue_type rv = ImageLoad ( d_buffer, d_index );
        return rv;
    }

    template< typename SourceT >
    VPP_INLINE void operator= ( const SourceT& rhs )
    {
        const rvalue_type rv = rhs;
        ImageStore ( d_buffer, d_index, rv );
    }

private:
    const buffer_type& d_buffer;
    Int d_index;
};

// -----------------------------------------------------------------------------

template< class BufferT >
struct TexelArray
{
};

template< class FormatT >
struct TexelArray< inTextureBuffer< FormatT > >
{
public:
    typedef inTextureBuffer< FormatT > buffer_type;

    VPP_INLINE TexelArray ( const buffer_type& bpoint ) :
        d_buffer ( bpoint )
    {}

    VPP_INLINE auto operator[] ( const Int& idx )
    {
        return TexelAccessor< buffer_type >( d_buffer, idx );
    }

    VPP_INLINE Int Size() const
    {
        return ImageSize ( d_buffer );
    }

private:
    const buffer_type& d_buffer;
};

template< class FormatT >
struct TexelArray< ioImageBuffer< FormatT > >
{
public:
    typedef ioImageBuffer< FormatT > buffer_type;

    VPP_INLINE TexelArray ( const buffer_type& bpoint ) :
        d_buffer ( bpoint )
    {}

    VPP_INLINE auto operator[] ( const Int& idx )
    {
        return TexelAccessor< buffer_type >( d_buffer, idx );
    }

    VPP_INLINE Int Size() const
    {
        return ImageSize ( d_buffer );
    }

private:
    const buffer_type& d_buffer;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGINTIMAGES_HPP
