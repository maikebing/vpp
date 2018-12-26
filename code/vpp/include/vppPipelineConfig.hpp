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

#ifndef INC_VPPPIPELINECONFIG_HPP
#define INC_VPPPIPELINECONFIG_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

#ifndef INC_VPPBUFFERVIEW_HPP
#include "vppBufferView.hpp"
#endif

#ifndef INC_VPPIMAGEVIEW_HPP
#include "vppImageView.hpp"
#endif

#ifndef INC_VPPSAMPLER_HPP
#include "vppSampler.hpp"
#endif

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

#ifndef INC_VPPVIEWPORT_HPP
#include "vppViewport.hpp"
#endif

#ifndef INC_VPPRENDERGRAPH_HPP
#include "vppRenderGraph.hpp"
#endif

#ifndef INC_VPPBLENDOPERATORS_HPP
#include "vppBlendOperators.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< class ValueT >
class TUpdateMultipleDescriptors
{
public:
    VPP_INLINE TUpdateMultipleDescriptors (
        std::uint32_t start, const ValueT* b, const ValueT* e ) :
            d_startIndex ( start ),
            d_begin ( b ),
            d_end ( e )
    {}

    VPP_INLINE std::uint32_t startIndex() const { return d_startIndex; }
    VPP_INLINE const ValueT* begin() const { return d_begin; }
    VPP_INLINE const ValueT* end() const { return d_end; }
    VPP_INLINE size_t size() const { return d_end - d_begin; }
    VPP_INLINE const ValueT& operator[]( size_t i ) const { return d_begin [ i ]; }

private:
    std::uint32_t d_startIndex;
    const ValueT* d_begin;
    const ValueT* d_end;
};

// -----------------------------------------------------------------------------

template< class ValueT >
VPP_INLINE TUpdateMultipleDescriptors< ValueT > multi (
    const ValueT& value, std::uint32_t startIdx = 0 )
{
    return TUpdateMultipleDescriptors< ValueT >( startIdx, & value, ( & value ) + 1 );
}

// -----------------------------------------------------------------------------

template< class ValueT >
VPP_INLINE TUpdateMultipleDescriptors< ValueT > multi (
    const std::vector< ValueT >& values, std::uint32_t startIdx = 0 )
{
    return TUpdateMultipleDescriptors< ValueT >(
        startIdx, & values [ 0 ], & values [ 0 ] + values.size() );
}

// -----------------------------------------------------------------------------

template< class ValueT >
VPP_INLINE TUpdateMultipleDescriptors< ValueT > multi (
    const std::initializer_list< ValueT >& values, std::uint32_t startIdx = 0 )
{
    std::vector< ValueT > tmpValueList ( values.begin(), values.end() );

    return TUpdateMultipleDescriptors< ValueT >(
        startIdx, & tmpValueList [ 0 ], & tmpValueList [ 0 ] + tmpValueList.size() );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class KResourceSets;

// -----------------------------------------------------------------------------

template< typename ValueT >
struct TGetValueProperties
{
    static VPP_INLINE size_t count ( const ValueT& )
    {
        return 1;
    }

    static VPP_INLINE std::uint32_t startIndex ( const ValueT& )
    {
        return 0;
    }
};

// -----------------------------------------------------------------------------

template< typename ValueT >
struct TGetValueProperties< TUpdateMultipleDescriptors< ValueT > >
{
    static VPP_INLINE size_t count ( const TUpdateMultipleDescriptors< ValueT >& value )
    {
        return value.size();
    }

    static VPP_INLINE std::uint32_t startIndex ( const TUpdateMultipleDescriptors< ValueT >& value )
    {
        return static_cast< std::uint32_t >( value.startIndex() );
    }
};

// -----------------------------------------------------------------------------

template< typename ValueT >
VPP_INLINE size_t getValueCount ( const ValueT& value )
{
    return TGetValueProperties< ValueT >().count ( value );
}

// -----------------------------------------------------------------------------

template< typename ValueT >
VPP_INLINE std::uint32_t getValueStartIndex ( const ValueT& value )
{
    return TGetValueProperties< ValueT >().startIndex ( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SResourceInfo : public VkDescriptorSetLayoutBinding
{
public:
    VPP_INLINE bool operator< ( const SResourceInfo& rhs ) const;
    std::uint32_t d_set;
};

// -----------------------------------------------------------------------------

VPP_INLINE bool SResourceInfo :: operator< ( const SResourceInfo& rhs ) const
{
    return d_set < rhs.d_set;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SVertexSourceInfo
{
    const unsigned char* d_pBaseOffset;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SVertexFieldInfo
{
    SVertexFieldInfo ( const unsigned char* pBaseOffset );

    const unsigned char* d_pBaseOffset;
    KId d_shaderVariableId;
};

// -----------------------------------------------------------------------------

VPP_INLINE SVertexFieldInfo :: SVertexFieldInfo ( const unsigned char* pBaseOffset ) :
    d_pBaseOffset ( pBaseOffset ),
    d_shaderVariableId ( 0 )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SStructLocationInfo
{
    SStructLocationInfo();

    bool d_bDefined;
    int d_occupiedLocations;
};

// -----------------------------------------------------------------------------

VPP_INLINE SStructLocationInfo :: SStructLocationInfo() :
    d_bDefined ( false ),
    d_occupiedLocations ( 0 )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SIOVariablesInfo
{
    SIOVariablesInfo();

    int d_currentLocation;
};

// -----------------------------------------------------------------------------

VPP_INLINE SIOVariablesInfo :: SIOVariablesInfo() :
    d_currentLocation ( 0 )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, class ViewT >
struct TDescriptorTypeTraits
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;
};

// -----------------------------------------------------------------------------

template< class ViewT >
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_SAMPLER, ViewT >
{
    static const bool HAS_IMAGE_INFO = true;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorImageInfo update_dest_type;

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const Sampler& hSampler )
    {
        pDest->sampler = hSampler.handle();
        pDest->imageView = 0;
        pDest->imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< NormalizedSampler >& hSamplers )
    {
        for ( size_t i = 0; i != hSamplers.size(); ++i )
        {
            pDest [ i ].sampler = hSamplers [ i ].handle();
            pDest [ i ].imageView = 0;
            pDest [ i ].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< UnnormalizedSampler >& hSamplers )
    {
        for ( size_t i = 0; i != hSamplers.size(); ++i )
        {
            pDest [ i ].sampler = hSamplers [ i ].handle();
            pDest [ i ].imageView = 0;
            pDest [ i ].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
};

// -----------------------------------------------------------------------------

template< class ViewT >
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ViewT >
{
    static const bool HAS_IMAGE_INFO = true;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorImageInfo update_dest_type;

    template< class BindingT >
    static VPP_INLINE VkImageLayout optimalLayout (
        const BindingT& value )
    {
        const VkImageLayout origLayout = value.layout();

        if ( origLayout != VK_IMAGE_LAYOUT_UNDEFINED )
            return origLayout;
        else
        {
            if ( value.view().image().info().getAspect() & VK_IMAGE_ASPECT_DEPTH_BIT )
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            else
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TImageSamplerBinding< ViewT >& value )
    {
        // sampler, view and layout
        pDest->sampler = value.sampler().handle();
        pDest->imageView = value.view();
        pDest->imageLayout = optimalLayout ( value );
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TImageBinding< ViewT >& value )
    {
        // view and layout (for immutable sampler bound descriptors)
        pDest->sampler = 0;
        pDest->imageView = value.view();
        pDest->imageLayout = optimalLayout ( value );
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< TImageSamplerBinding< ViewT > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].sampler = values [ i ].sampler().handle();
            pDest [ i ].imageView = values [ i ].view();
            pDest [ i ].imageLayout = optimalLayout ( values [ i ] );
        }
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< TImageBinding< ViewT > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].sampler = 0;
            pDest [ i ].imageView = values [ i ].view();
            pDest [ i ].imageLayout = optimalLayout ( values [ i ] );
        }
    }
};

// -----------------------------------------------------------------------------

template< class ViewT >
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, ViewT >
{
    static const bool HAS_IMAGE_INFO = true;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorImageInfo update_dest_type;

    template< class BindingT >
    static VPP_INLINE VkImageLayout optimalLayout (
        const BindingT& value )
    {
        const VkImageLayout origLayout = value.layout();

        if ( origLayout != VK_IMAGE_LAYOUT_UNDEFINED )
            return origLayout;
        else
        {
            if ( value.view().image().info().getAspect() & VK_IMAGE_ASPECT_DEPTH_BIT )
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            else
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TImageBinding< ViewT >& value )
    {
        // view and layout
        pDest->sampler = 0;
        pDest->imageView = value.view();
        pDest->imageLayout = optimalLayout ( value );
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< TImageBinding< ViewT > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].sampler = 0;
            pDest [ i ].imageView = values [ i ].view();
            pDest [ i ].imageLayout = optimalLayout ( values [ i ] );
        }
    }
};

// -----------------------------------------------------------------------------

template< class ViewT >
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, ViewT >
{
    static const bool HAS_IMAGE_INFO = true;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorImageInfo update_dest_type;

    template< class BindingT >
    static VPP_INLINE VkImageLayout optimalLayout (
        const BindingT& value )
    {
        const VkImageLayout origLayout = value.layout();

        if ( origLayout != VK_IMAGE_LAYOUT_UNDEFINED )
            return origLayout;
        else
            return VK_IMAGE_LAYOUT_GENERAL;
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TImageBinding< ViewT >& value )
    {
        // view and layout
        pDest->sampler = 0;
        pDest->imageView = value.view();
        pDest->imageLayout = optimalLayout ( value );
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< TImageBinding< ViewT > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].sampler = 0;
            pDest [ i ].imageView = values [ i ].view();
            pDest [ i ].imageLayout = optimalLayout ( values [ i ] );
        }
    }
};

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
struct TDescriptorTypeTraits<
    VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, TexelBufferView< FormatT, USAGE > >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = true;

    typedef VkBufferView update_dest_type;

    static VPP_INLINE void update (
        VkBufferView* pDest, const TexelBufferView< FormatT, USAGE >& value )
    {
        *pDest = value;
    }

    static VPP_INLINE void update (
        VkBufferView* pDest,
        const TUpdateMultipleDescriptors< TexelBufferView< FormatT, USAGE > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
            pDest [ i ] = values [ i ];
    }
};

// -----------------------------------------------------------------------------

template< class FormatT, unsigned int USAGE >
struct TDescriptorTypeTraits<
    VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, TexelBufferView< FormatT, USAGE > >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = true;

    typedef VkBufferView update_dest_type;

    static VPP_INLINE void update (
        VkBufferView* pDest, const TexelBufferView< FormatT, USAGE >& value )
    {
        *pDest = value;
    }

    static VPP_INLINE void update (
        VkBufferView* pDest,
        const TUpdateMultipleDescriptors< TexelBufferView< FormatT, USAGE > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
            pDest [ i ] = values [ i ];
    }
};

// -----------------------------------------------------------------------------

template<>
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, UniformBufferView >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = true;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorBufferInfo update_dest_type;

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest, const UniformBufferView& value )
    {
        pDest->buffer = value.buffer().handle();
        pDest->offset = value.offset();
        pDest->range = value.size();
    }

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest,
        const TUpdateMultipleDescriptors< UniformBufferView >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].buffer = values [ i ].buffer().handle();
            pDest [ i ].offset = values [ i ].offset();
            pDest [ i ].range = values [ i ].size();
        }
    }
};

// -----------------------------------------------------------------------------

template<>
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, UniformBufferView >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = true;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorBufferInfo update_dest_type;

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest, const UniformBufferView& value )
    {
        pDest->buffer = value.buffer().handle();
        pDest->offset = value.offset();
        pDest->range = value.size();
    }

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest,
        const TUpdateMultipleDescriptors< UniformBufferView >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].buffer = values [ i ].buffer().handle();
            pDest [ i ].offset = values [ i ].offset();
            pDest [ i ].range = values [ i ].size();
        }
    }
};

// -----------------------------------------------------------------------------

template<>
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, UniformBufferView >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = true;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorBufferInfo update_dest_type;

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest, const UniformBufferView& value )
    {
        pDest->buffer = value.buffer().handle();
        pDest->offset = value.offset();
        pDest->range = value.size();
    }

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest,
        const TUpdateMultipleDescriptors< UniformBufferView >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].buffer = values [ i ].buffer().handle();
            pDest [ i ].offset = values [ i ].offset();
            pDest [ i ].range = values [ i ].size();
        }
    }
};

// -----------------------------------------------------------------------------

template<>
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, UniformBufferView >
{
    static const bool HAS_IMAGE_INFO = false;
    static const bool HAS_BUFFER_INFO = true;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorBufferInfo update_dest_type;

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest, const UniformBufferView& value )
    {
        pDest->buffer = value.buffer().handle();
        pDest->offset = value.offset();
        pDest->range = value.size();
    }

    static VPP_INLINE void update (
        VkDescriptorBufferInfo* pDest,
        const TUpdateMultipleDescriptors< UniformBufferView >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].buffer = values [ i ].buffer().handle();
            pDest [ i ].offset = values [ i ].offset();
            pDest [ i ].range = values [ i ].size();
        }
    }
};

// -----------------------------------------------------------------------------

template< class ViewT >
struct TDescriptorTypeTraits< VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, ViewT >
{
    static const bool HAS_IMAGE_INFO = true;
    static const bool HAS_BUFFER_INFO = false;
    static const bool HAS_TEXEL_BUFFER_VIEW = false;

    typedef VkDescriptorImageInfo update_dest_type;

    template< class BindingT >
    static VPP_INLINE VkImageLayout optimalLayout (
        const BindingT& value )
    {
        const VkImageLayout origLayout = value.layout();

        if ( origLayout != VK_IMAGE_LAYOUT_UNDEFINED )
            return origLayout;
        else
        {
            if ( value.view().image().info().getAspect() & VK_IMAGE_ASPECT_DEPTH_BIT )
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            else
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TImageBinding< ViewT >& value )
    {
        // view and layout
        pDest->sampler = 0;
        pDest->imageView = value.view();
        pDest->imageLayout = optimalLayout ( value );
    }

    static VPP_INLINE void update (
        VkDescriptorImageInfo* pDest,
        const TUpdateMultipleDescriptors< TImageBinding< ViewT > >& values )
    {
        for ( size_t i = 0; i != values.size(); ++i )
        {
            pDest [ i ].sampler = 0;
            pDest [ i ].imageView = values [ i ].view();
            pDest [ i ].imageLayout = optimalLayout ( values [ i ] );
        }
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KDescriptor
{
public:
    VPP_DLLAPI KDescriptor (
        VkDescriptorType type,
        std::uint32_t set,
        int binding,
        std::uint32_t count );

    VPP_DLLAPI ~KDescriptor();

    template< class SamplerT >
    KDescriptor (
        VkDescriptorType type,
        std::uint32_t set,
        int binding,
        std::uint32_t count,
        const std::vector< SamplerT >& samplers );

    VPP_INLINE std::uint32_t id() const
    {
        return d_id;
    }

    VPP_INLINE std::uint32_t set() const
    {
        return d_set;
    }

    VPP_INLINE std::uint32_t binding() const
    {
        return d_binding;
    }

    VPP_INLINE std::uint32_t count() const
    {
        return d_count;
    }

    VPP_INLINE bool isAutomaticBinding() const
    {
        return d_bAutomaticBinding;
    }

    VPP_DLLAPI void rebind ( std::uint32_t newBinding );
    
protected:
    VkDescriptorType d_type;
    std::uint32_t d_id;
    std::uint32_t d_set;
    std::uint32_t d_binding;
    std::uint32_t d_count;
    bool d_bAutomaticBinding;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KDescriptorItemHolder
{
public:
    VPP_INLINE KDescriptorItemHolder()
    {
    }

    VPP_INLINE KDescriptorItemHolder ( const Sampler& item ) :
        d_sampler ( item )
    {}

    template< class ViewT >
    VPP_INLINE KDescriptorItemHolder ( const TImageSamplerBinding< ViewT >& value ) :
        d_image ( value.view().image() ),
        d_sampler ( value.sampler() )
    {}

    template< class ViewT >
    VPP_INLINE KDescriptorItemHolder ( const TImageBinding< ViewT >& value ) :
        d_image ( value.view().image() )
    {}

    template< class FormatT, unsigned int USAGE >
    VPP_INLINE KDescriptorItemHolder ( const TexelBufferView< FormatT, USAGE >& value ) :
        d_buffer ( value.buffer() )
    {}

    VPP_INLINE KDescriptorItemHolder ( const UniformBufferView& value ) :
        d_buffer ( value.buffer() )
    {}

private:
    Buf d_buffer;
    Img d_image;
    Sampler d_sampler;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

class IDebugProbe
{
public:
    VPP_DLLAPI virtual ~IDebugProbe();
    virtual void bind ( const ShaderDataBlock& hDataBlock ) = 0;

protected:
    VPP_DLLAPI void bind (
        VkImageView hView,
        std::uint32_t set, std::uint32_t binding, 
        const ShaderDataBlock& hDataBlock );
};

// -----------------------------------------------------------------------------

typedef std::shared_ptr< IDebugProbe > HDebugProbe;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class PipelineConfigImpl;

// -----------------------------------------------------------------------------

class PipelineConfig : public TSharedReference< PipelineConfigImpl >
{
public:
    VPP_DLLAPI VkPrimitiveTopology getPrimitiveTopology() const;
    VPP_DLLAPI bool getEnablePrimitiveRestart() const;
    VPP_DLLAPI std::uint32_t getTessPatchControlPoints() const;

    VPP_DLLAPI static PipelineConfig* getInstance();

public:
    static const VkPrimitiveTopology POINT_LIST = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    static const VkPrimitiveTopology LINE_LIST = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    static const VkPrimitiveTopology LINE_STRIP = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    static const VkPrimitiveTopology TRIANGLE_LIST = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    static const VkPrimitiveTopology TRIANGLE_STRIP = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    static const VkPrimitiveTopology TRIANGLE_FAN = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    static const VkPrimitiveTopology LINE_LIST_ADJ = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
    static const VkPrimitiveTopology LINE_STRIP_ADJ = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
    static const VkPrimitiveTopology TRIANGLE_LIST_ADJ = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
    static const VkPrimitiveTopology TRIANGLE_STRIP_ADJ = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
    static const VkPrimitiveTopology PATCH_LIST = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

protected:
    VPP_DLLAPI PipelineConfig ( const Process& boundProcess );
    VPP_DLLAPI ~PipelineConfig();

    VPP_DLLAPI void setBlendingMode (
        const BaseAttachment& dataNode,
        const VkPipelineColorBlendAttachmentState& blendConfig );

    VPP_DLLAPI void enableLogicOperation ( bool bEnable, VkLogicOp logicOp );

    VPP_DLLAPI void setPrimitiveTopology ( VkPrimitiveTopology v );
    VPP_DLLAPI void setEnablePrimitiveRestart ( bool v );
    VPP_DLLAPI void setTessPatchControlPoints ( std::uint32_t v );

public:
    typedef std::vector< detail::SResourceInfo > Id2ResourceDefinition;
    typedef std::vector< VkPushConstantRange > Id2Constant;
    typedef std::vector< detail::SVertexSourceInfo > Id2VertexSource;
    typedef std::vector< VkVertexInputBindingDescription > Id2VertexBinding;
    typedef std::vector< detail::SVertexFieldInfo > Id2VertexField;
    typedef std::vector< VkVertexInputAttributeDescription > Id2VertexFieldDesc;
    typedef std::vector< VkPipelineColorBlendAttachmentState > BlendModeTable;
    typedef std::vector< VkPipelineShaderStageCreateInfo > ShaderTable;
    typedef std::vector< VkDynamicState > DynamicStates;
    typedef std::set< std::vector< VkSampler > > ImmutableSamplerHandles;
    typedef std::set< Sampler > ImmutableSamplers;
    typedef std::set< KShaderModule > ShaderModules;
    typedef std::map< std::pair< EShader, EShader >, detail::SIOVariablesInfo > ShaderIOVariables;
    typedef std::map< std::type_index, detail::SStructLocationInfo > Struct2LocationInfo;
    typedef std::list< detail::SStructLocationInfo* > StructTypeStack;
    typedef std::pair< detail::SVertexFieldInfo*, VkVertexInputAttributeDescription* > VertexFieldInfo;
    typedef std::vector< HDebugProbe > DebugProbes;
    
    RenderGraph& getRenderGraph() const;
    std::uint32_t getProcessIndex() const;

    std::uint32_t createNewResource();
    detail::SResourceInfo& getResource ( std::uint32_t id );
    const Id2ResourceDefinition& getResources() const;

    std::uint32_t allocateBinding ( detail::KDescriptor* pDescriptor );
    void setBinding ( detail::KDescriptor* pDescriptor, std::uint32_t binding );

    VPP_DLLAPI void pushStructType ( const std::type_index& index );
    VPP_DLLAPI detail::SStructLocationInfo& currentStructType();
    VPP_DLLAPI void popStructType();
    const detail::SStructLocationInfo* findStructType ( const std::type_index& index );

    VPP_DLLAPI std::uint32_t createNewConstant();
    VkPushConstantRange& getConstant ( std::uint32_t id );
    const VkPushConstantRange& getConstant ( std::uint32_t id ) const;

    const Id2Constant& getConstants() const;

    VPP_DLLAPI const VkSampler* addImmutableSamplers (
        const std::vector< NormalizedSampler >& samplers, std::uint32_t count );

    VPP_DLLAPI const VkSampler* addImmutableSamplers (
        const std::vector< UnnormalizedSampler >& samplers, std::uint32_t count );

    VPP_DLLAPI void createVertexSource ( const void* pBase, std::uint32_t stride, bool bInstanceScope );
    const detail::SVertexSourceInfo* findVertexSource ( const void* pBase ) const;
    VPP_DLLAPI const VkVertexInputBindingDescription* findVertexBinding ( const void* pBase ) const;

    VPP_DLLAPI void createVertexDataField ( const void* pOffset, VkFormat format );
    VPP_DLLAPI VertexFieldInfo findVertexDataField ( const void* pOffset );

    template< class SourceShader, class DestShader >
    detail::SIOVariablesInfo* findIOVariablesInfo() const;
    
    VPP_DLLAPI void registerDebugProbe ( const HDebugProbe& hDebugProbe );
    VPP_DLLAPI void bindDebugProbes ( const ShaderDataBlock& hDataBlock ) const;

    void initVertexInputCreateInfo (
        VkPipelineVertexInputStateCreateInfo* pDest ) const;

    void initInputAssemblyStateCreateInfo (
        VkPipelineInputAssemblyStateCreateInfo* pDest ) const;

    bool initTessellationStateCreateInfo (
        VkPipelineTessellationStateCreateInfo* pDest ) const;

    void initColorBlendStateInfo (
        VkPipelineColorBlendStateCreateInfo* pDest ) const;

    bool initDynamicStateInfo (
        VkPipelineDynamicStateCreateInfo* pDest ) const;
    
public:
    void setVertexShader ( vertexShader* pShader );
    void setTessControlShader ( tessControlShader* pShader );
    void setTessEvalShader ( tessEvalShader* pShader );
    void setGeometryShader ( geometryShader* pShader );
    void setFragmentShader ( fragmentShader* pShader );
    void setComputeShader ( computeShader* pShader );

    VPP_DLLAPI void fillShaderTable ( ShaderTable* pShaderTable, Device hDevice ) const;

    virtual VkPipelineLayout getCompiledHandle() const = 0;

    template< class AssignmentListT >
    void cmdBindVertexInput (
        const AssignmentListT& list,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI void cmdBindIndexInput (
        const VertexIndexBufferView& hVertexIndexBufferView,
        CommandBuffer hCmdBuffer = CommandBuffer() );

protected:
    // For compute pipelines.
    enum EComputePipelineTag { COMPUTE_PIPELINE_TAG };
    VPP_DLLAPI PipelineConfig ( EComputePipelineTag );

private:
    void addShader (
        ShaderTable* pShaderTable,
        KShader* pShader,
        Device hDevice,
        SDynamicParameters* pDynamicParameters ) const;

    PipelineConfig ( const PipelineConfig& ) = delete;
    PipelineConfig ( PipelineConfig&& ) = delete;
    const PipelineConfig& operator= ( const PipelineConfig& ) = delete;

private:
    static thread_local PipelineConfig* d_pThis;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ComputePipelineConfig : public PipelineConfig
{
public:
    ComputePipelineConfig();
};

// -----------------------------------------------------------------------------

VPP_INLINE ComputePipelineConfig :: ComputePipelineConfig() :
    PipelineConfig ( PipelineConfig::COMPUTE_PIPELINE_TAG )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class PipelineConfigImpl : public TSharedObject< PipelineConfigImpl >
{
public:
    VPP_DLLAPI PipelineConfigImpl();
    VPP_DLLAPI PipelineConfigImpl ( const Process& boundProcess );
    VPP_DLLAPI ~PipelineConfigImpl();

private:
    std::uint32_t findFirstFreeBinding ( std::uint32_t set ) const;

private:
    friend class PipelineConfig;

    RenderGraph d_renderGraph;
    std::uint32_t d_processIndex;

    PipelineConfig::Id2ResourceDefinition d_id2resourceDefinition;
    PipelineConfig::Id2Constant d_id2constant;
    PipelineConfig::Id2VertexSource d_id2vertexSource;
    PipelineConfig::Id2VertexBinding d_id2vertexBinding;
    PipelineConfig::Id2VertexField d_id2vertexField;
    PipelineConfig::Id2VertexFieldDesc d_id2vertexFieldDesc;
    PipelineConfig::ImmutableSamplerHandles d_immutableSamplerHandles;
    PipelineConfig::ImmutableSamplers d_immutableSamplers;
    PipelineConfig::ShaderModules d_shaderModules;
    PipelineConfig::ShaderIOVariables d_shaderIOVariables;
    PipelineConfig::DynamicStates d_dynamicStates;
    PipelineConfig::BlendModeTable d_blendModeTable;
    PipelineConfig::Struct2LocationInfo d_struct2LocationInfo;
    PipelineConfig::StructTypeStack d_structTypeStack;
    PipelineConfig::DebugProbes d_debugProbes;

    typedef std::pair< std::uint32_t, std::uint32_t > SetBinding;
    typedef std::map< SetBinding, detail::KDescriptor* > SetBinding2Descriptor;
    SetBinding2Descriptor d_setBinding2descriptor;

    vertexShader* d_pVertexShader;
    tessControlShader* d_pTessControlShader;
    tessEvalShader* d_pTessEvalShader;
    geometryShader* d_pGeometryShader;
    fragmentShader* d_pFragmentShader;
    computeShader* d_pComputeShader;

    VkPrimitiveTopology d_topology;
    std::uint32_t d_tessPatchControlPoints;
    bool d_bEnablePrimitiveRestart;
    VkBool32 d_logicOpEnable;
    VkLogicOp d_logicOp;

    std::uint32_t d_vertexInputLocationCounter;
};

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph& PipelineConfig :: getRenderGraph() const
{
    return get()->d_renderGraph;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t PipelineConfig :: getProcessIndex() const
{
    return get()->d_processIndex;
}

// -----------------------------------------------------------------------------

VPP_INLINE detail::SResourceInfo& PipelineConfig :: getResource ( std::uint32_t id )
{
    return get()->d_id2resourceDefinition [ id ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineConfig::Id2ResourceDefinition& PipelineConfig :: getResources() const
{
    return get()->d_id2resourceDefinition;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPushConstantRange& PipelineConfig :: getConstant ( std::uint32_t id )
{
    return get()->d_id2constant [ id ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkPushConstantRange& PipelineConfig :: getConstant ( std::uint32_t id ) const
{
    return get()->d_id2constant [ id ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineConfig::Id2Constant& PipelineConfig :: getConstants() const
{
    return get()->d_id2constant;
}

// -----------------------------------------------------------------------------

template< class SourceShader, class DestShader >
VPP_INLINE detail::SIOVariablesInfo* PipelineConfig :: findIOVariablesInfo() const
{
    const EShader src = SourceShader::shader_type;
    const EShader dst = DestShader::shader_type;
    return & get()->d_shaderIOVariables [ std::make_pair ( src, dst ) ];
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setVertexShader ( vertexShader* pShader )
{
    get()->d_pVertexShader = pShader;
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setTessControlShader ( tessControlShader* pShader )
{
    get()->d_pTessControlShader = pShader;
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setTessEvalShader ( tessEvalShader* pShader )
{
    get()->d_pTessEvalShader = pShader;
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setGeometryShader ( geometryShader* pShader )
{
    get()->d_pGeometryShader = pShader;
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setFragmentShader ( fragmentShader* pShader )
{
    get()->d_pFragmentShader = pShader;
}

// -----------------------------------------------------------------------------

VPP_INLINE void PipelineConfig :: setComputeShader ( computeShader* pShader )
{
    get()->d_pComputeShader = pShader;
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class SamplerT >
VPP_INLINE KDescriptor :: KDescriptor (
    VkDescriptorType type,
    std::uint32_t set,
    int binding,
    std::uint32_t count,
    const std::vector< SamplerT >& samplers ) :
        KDescriptor ( type, set, binding, count )
{
    PipelineConfig* pConfig = PipelineConfig::getInstance();
    SResourceInfo& resInfo = pConfig->getResource ( d_id );
    resInfo.pImmutableSamplers = pConfig->addImmutableSamplers ( samplers, count );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class LeftT, class RightT >
class TDescriptorAssignerList
{
public:
    VPP_INLINE TDescriptorAssignerList ( const LeftT& left, const RightT& right ) :
        d_left ( left ),
        d_right ( right )
    {}

    template< class FunctorT >
    VPP_INLINE void for_each ( FunctorT& fOperation ) const
    {
        d_left.for_each ( fOperation );
        d_right.for_each ( fOperation );
    }

    VPP_INLINE size_t getImageInfoCount() const
    {
        return d_left.getImageInfoCount() + d_right.getImageInfoCount();
    }

    VPP_INLINE size_t getBufferInfoCount() const
    {
        return d_left.getBufferInfoCount() + d_right.getBufferInfoCount();
    }

    VPP_INLINE size_t getTexelBufferViewCount() const
    {
        return d_left.getTexelBufferViewCount() + d_right.getTexelBufferViewCount();
    }

    static const size_t ASSIGNER_COUNT = LeftT::ASSIGNER_COUNT + RightT::ASSIGNER_COUNT;
    static const size_t COPIER_COUNT = LeftT::COPIER_COUNT + RightT::COPIER_COUNT;

    const LeftT& d_left;
    const RightT& d_right;
};

// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, class ViewT, typename ValueT >
class TDescriptorAssigner
{
public:
    static const VkDescriptorType type_code = TYPE;
    typedef ViewT view_type;
    typedef ValueT value_type;
    typedef TDescriptorTypeTraits< TYPE, ViewT > type_traits;

    VPP_INLINE TDescriptorAssigner (
        std::uint32_t id, std::uint32_t set, std::uint32_t binding,
        const ValueT& value ) :
            d_id ( id ),
            d_set ( set ),
            d_binding ( binding ),
            d_value ( value )
    {
    }

    template< class FunctorT >
    VPP_INLINE void for_each ( FunctorT& fOperation ) const
    {
        fOperation. template write< TYPE, ViewT, ValueT >( d_id, d_set, d_binding, d_value );
    }

    VPP_INLINE size_t getImageInfoCount() const
    {
        return type_traits::HAS_IMAGE_INFO * detail::getValueCount ( d_value );
    }

    VPP_INLINE size_t getBufferInfoCount() const
    {
        return type_traits::HAS_BUFFER_INFO * detail::getValueCount ( d_value );
    }

    VPP_INLINE size_t getTexelBufferViewCount() const
    {
        return type_traits::HAS_TEXEL_BUFFER_VIEW * detail::getValueCount ( d_value );
    }

    static const size_t ASSIGNER_COUNT = 1;
    static const size_t COPIER_COUNT = 0;

private:
    std::uint32_t d_id;
    std::uint32_t d_set;
    std::uint32_t d_binding;
    ValueT d_value;
};

// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, std::uint32_t COUNT >
class TDescriptorCopier
{
public:
    VPP_INLINE TDescriptorCopier (
        std::uint32_t sid, std::uint32_t sset, std::uint32_t sbinding, std::uint32_t soffset,
        std::uint32_t did, std::uint32_t dset, std::uint32_t dbinding, std::uint32_t doffset ) :
            d_sid ( d_sid ), d_sset  ( d_sset ), d_sbinding ( d_sbinding ), d_soffset ( d_soffset ),
            d_did ( d_did ), d_dset ( d_dset ), d_dbinding ( d_dbinding ), d_doffset ( d_doffset )
    {}

    template< class FunctorT >
    VPP_INLINE void for_each ( FunctorT& fOperation ) const
    {
        fOperation.copy (
            d_sset, d_sbinding, d_soffset, d_dset, d_dbinding, d_doffset, COUNT );
    }

    static const size_t ASSIGNER_COUNT = 0;
    static const size_t COPIER_COUNT = 1;

private:
    std::uint32_t d_sid;
    std::uint32_t d_sset;
    std::uint32_t d_sbinding;
    std::uint32_t d_soffset;

    std::uint32_t d_did;
    std::uint32_t d_dset;
    std::uint32_t d_dbinding;
    std::uint32_t d_doffset;
};

// -----------------------------------------------------------------------------

template< class LeftT, class MidT, class RightT >
VPP_INLINE TDescriptorAssignerList< TDescriptorAssignerList< LeftT, MidT >, RightT >
    operator, (
        const TDescriptorAssignerList< LeftT, MidT >& left,
        const RightT& right )
{
    return TDescriptorAssignerList< TDescriptorAssignerList< LeftT, MidT >, RightT >(
        left, right );
}

// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, class ViewT, typename ValueT, class RightT >
VPP_INLINE TDescriptorAssignerList<
    TDescriptorAssigner< TYPE, ViewT, ValueT >, RightT >
    operator, (
        const TDescriptorAssigner< TYPE, ViewT, ValueT >& left,
        const RightT& right )
{
    return TDescriptorAssignerList< TDescriptorAssigner< TYPE, ViewT, ValueT >, RightT >(
        left, right );
}

// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, std::uint32_t COUNT, class RightT >
VPP_INLINE TDescriptorAssignerList< TDescriptorCopier< TYPE, COUNT >, RightT >
    operator, ( const TDescriptorCopier< TYPE, COUNT >& left, const RightT& right )
{
    return TDescriptorAssignerList< TDescriptorCopier< TYPE, COUNT >, RightT >( left, right );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< VkDescriptorType TYPE, class ViewT >
class TDescriptorType : public detail::KDescriptor
{
public:
    static const VkDescriptorType descriptor_type = TYPE;

    VPP_INLINE TDescriptorType (
        std::uint32_t set, int binding, std::uint32_t count ) :
            KDescriptor (
                TYPE,
                set, binding, count )
    {
    }

    template< typename SamplerT >
    VPP_INLINE TDescriptorType (
        const std::vector< SamplerT >& samplers,
        std::uint32_t set, int binding, std::uint32_t count ) :
            KDescriptor (
                TYPE,
                set, binding, count,
                samplers )
    {
    }

    VPP_INLINE void addStageFlag ( std::uint32_t f ) const
    {
        PipelineConfig::getInstance()->getResource ( d_id ).stageFlags |= f;
    }

    //VPP_INLINE TDescriptorCopier< TYPE, 1 >
    //    operator= ( const TDescriptorType< TYPE, ViewT >& src )
    //{
    //    return TDescriptorCopier< TYPE, 1 >(
    //        src.id(), src.set(), src.binding(), 0, d_id, d_set, d_binding, 0 );
    //}

    // FIXME - zrobic tutaj jeszcze wersje dla kopiowania zakresow arrays
    // (w ktorej nie ma zer na poz. element)
    // jakis wrapper na TDescriptorType, np. zwracany przez metode range< begin, size >

    //template< std::uint32_t OFFSET, std::uint32_t LENGTH >
    //VPP_INLINE TRange< ??? > range()
    //{
    //}

    //template< std::uint32_t SSET, std::uint32_t SBINDING, std::uint32_t SOFFSET, std::uint32_t DOFFSET >
    //VPP_INLINE TDescriptorCopier< TYPE, COUNT, SSET, SBINDING, SOFFSET, SET, BINDING, DOFFSET >
    //    operator= ( const TRange< TDescriptorType< TYPE, SSET, SBINDING, COUNT >& src )
    //{
    //    return TDescriptorCopier< TYPE, COUNT, SSET, SBINDING, SOFFSET, SET, BINDING, DOFFSET >(
    //        src.id(), d_id );
    //}
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KResourceSets
{
public:
    KResourceSets ( const PipelineConfig& res );

    size_t getSetCount() const;
    const VkDescriptorSetLayoutCreateInfo* getSetInfo ( size_t iSet ) const;

private:
    std::vector< VkDescriptorSetLayoutBinding > d_resourceInfos;
    std::vector< VkDescriptorSetLayoutCreateInfo > d_setCreateInfos;
};

// -----------------------------------------------------------------------------

VPP_INLINE KResourceSets :: KResourceSets ( const PipelineConfig& res )
{
    PipelineConfig::Id2ResourceDefinition resourceDefs = res.getResources();

    if ( resourceDefs.empty() )
        return;

    std::sort ( resourceDefs.begin(), resourceDefs.end() );

    d_resourceInfos.resize ( resourceDefs.size() );

    const size_t maxSetId = resourceDefs.back().d_set;
    const size_t setCount = maxSetId + 1;

    std::vector< size_t > setSizes ( setCount, 0 );
    std::vector< size_t > setStarts ( setCount, 0 );

    for ( const auto& iResource : resourceDefs )
        ++setSizes [ iResource.d_set ];

    size_t currentSetStart = 0;

    for ( size_t iSet = 0; iSet != setCount; ++iSet )
    {
        setStarts [ iSet ] = currentSetStart;
        currentSetStart += setSizes [ iSet ];
    }

    std::copy (
        resourceDefs.begin(),
        resourceDefs.end(),
        d_resourceInfos.begin() );

    d_setCreateInfos.resize ( setCount );

    for ( size_t iSet = 0; iSet != setCount; ++iSet )
    {
        VkDescriptorSetLayoutCreateInfo& currentSet = d_setCreateInfos [ iSet ];
        currentSet.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        currentSet.pNext = 0;
        currentSet.flags = 0;
        currentSet.bindingCount = static_cast< std::uint32_t >( setSizes [ iSet ] );
        currentSet.pBindings = & d_resourceInfos [ setStarts [ iSet ] ];
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE size_t KResourceSets :: getSetCount() const
{
    return d_setCreateInfos.size();
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkDescriptorSetLayoutCreateInfo* KResourceSets :: getSetInfo ( size_t iSet ) const
{
    return & d_setCreateInfos [ iSet ];
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KDescriptorSetLayoutImpl;

// -----------------------------------------------------------------------------

class KDescriptorSetLayout : public TSharedReference< KDescriptorSetLayoutImpl >
{
public:
    KDescriptorSetLayout();

    KDescriptorSetLayout (
        const detail::KResourceSets& resourceSets,
        size_t iSet,
        Device hDevice );

    VkDescriptorSetLayout handle() const;
    Device device() const;
};

// -----------------------------------------------------------------------------

class KDescriptorSetLayoutImpl : public TSharedObject< KDescriptorSetLayoutImpl >
{
public:
    KDescriptorSetLayoutImpl (
        const detail::KResourceSets& resourceSets,
        size_t iSet,
        Device hDevice );

    ~KDescriptorSetLayoutImpl();

private:
    friend class KDescriptorSetLayout;
    Device d_hDevice;
    VkDescriptorSetLayout d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE KDescriptorSetLayoutImpl :: KDescriptorSetLayoutImpl (
    const detail::KResourceSets& resourceSets,
    size_t iSet,
    Device hDevice ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result()
{
    d_result = ::vkCreateDescriptorSetLayout (
        hDevice.handle(), resourceSets.getSetInfo ( iSet ), 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KDescriptorSetLayoutImpl :: ~KDescriptorSetLayoutImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyDescriptorSetLayout ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE KDescriptorSetLayout :: KDescriptorSetLayout()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KDescriptorSetLayout :: KDescriptorSetLayout (
    const detail::KResourceSets& resourceSets,
    size_t iSet,
    Device hDevice ) :
        TSharedReference< KDescriptorSetLayoutImpl > (
            new KDescriptorSetLayoutImpl ( resourceSets, iSet, hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDescriptorSetLayout KDescriptorSetLayout :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE Device KDescriptorSetLayout :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void vertex ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_VERTEX_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void vertex ( FirstT first, Args... args )
{
    vertex ( first );
    vertex ( args... );
}

// -----------------------------------------------------------------------------

//VPP_INLINE void tessellation (
//    tessControlShader& tcsh,
//    tessEvalShader& tesh,
//    std::uint32_t patchControlPoints )
//{
//    PipelineConfig* pConfig = PipelineConfig::getInstance();
//    pConfig->setTessControlShader ( & tcsh );
//    pConfig->setTessEvalShader ( & tesh );
//    pConfig->setTessPatchControlPoints ( patchControlPoints );
//    pConfig->setPrimitiveTopology ( VK_PRIMITIVE_TOPOLOGY_PATCH_LIST );
//}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void tessControl ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void tessControl ( FirstT first, Args... args )
{
    tessControl ( first );
    tessControl ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void tessEval ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void tessEval ( FirstT first, Args... args )
{
    tessEval ( first );
    tessEval ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void geometry ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_GEOMETRY_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void geometry ( FirstT first, Args... args )
{
    geometry ( first );
    geometry ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void fragment ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_FRAGMENT_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void fragment ( FirstT first, Args... args )
{
    fragment ( first );
    fragment ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void compute ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_COMPUTE_BIT );
}

template< typename FirstT, typename... Args >
VPP_INLINE void compute ( FirstT first, Args... args )
{
    compute ( first );
    compute ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT  >
VPP_INLINE void graphics ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_ALL_GRAPHICS );
}

template< typename FirstT, typename... Args >
VPP_INLINE void graphics ( FirstT first, Args... args )
{
    graphics ( first );
    graphics ( args... );
}

// -----------------------------------------------------------------------------

template< typename FirstT >
VPP_INLINE void all ( FirstT first )
{
    first.addStageFlag ( VK_SHADER_STAGE_ALL );
}

template< typename FirstT, typename... Args >
VPP_INLINE void all ( FirstT first, Args... args )
{
    all ( first );
    all ( args... );
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class AssignmentListT >
class TVertexInputBinder
{
public:
    VPP_INLINE TVertexInputBinder (
        const CommandBuffer& hCmdBuffer,
        const PipelineConfig& hDataSpec ) :
            d_commandBuffer ( hCmdBuffer ),
            d_dataSpec ( hDataSpec ),
            d_currentIndex ( 0 )
    {}

    struct SBindingInfo
    {
        VPP_INLINE bool operator< ( const SBindingInfo& rhs ) const
        {
            return d_binding < rhs.d_binding;
        }

        std::uint32_t d_binding;
        VkBuffer d_buffer;
        VkDeviceSize d_offset;
    };
    
    VPP_INLINE void bindVertexInput (
        const void* pBase, const VertexBufferView& hVertexBufferView )
    {
        const VkVertexInputBindingDescription* pBindingInfo = d_dataSpec.findVertexBinding ( pBase );
        
        SBindingInfo& info = d_bindingInfos [ d_currentIndex++ ];
        info.d_binding = pBindingInfo->binding;
        info.d_buffer = hVertexBufferView.buffer().handle();
        info.d_offset = hVertexBufferView.offset();
    }

    VPP_INLINE void bind ( const AssignmentListT& list )
    {
        list.for_each ( *this );

        if ( d_currentIndex > 1 )
            std::sort ( d_bindingInfos + 0, d_bindingInfos + d_currentIndex );

        for ( size_t i = 0; i != d_currentIndex; ++i )
        {
            d_bindings [ i ] = d_bindingInfos [ i ].d_binding;
            d_buffers [ i ] = d_bindingInfos [ i ].d_buffer;
            d_offsets [ i ] = d_bindingInfos [ i ].d_offset;
        }

        size_t startBindingIndex = 0;
        std::uint32_t bindingCount = 1;

        for ( size_t i = 1; i < d_currentIndex + 1; ++i )
        {
            if ( i < d_currentIndex && d_bindings [ i ] == d_bindings [ i - 1 ] + 1 )
                ++bindingCount;
            else
            {
                ::vkCmdBindVertexBuffers (
                    d_commandBuffer.handle(),
                    d_bindings [ startBindingIndex ], bindingCount,
                    d_buffers + startBindingIndex, d_offsets + startBindingIndex );

                startBindingIndex = i;
                bindingCount = 1;
            }
        }
    }

private:
    CommandBuffer d_commandBuffer;
    const PipelineConfig& d_dataSpec;

    size_t d_currentIndex;

    SBindingInfo d_bindingInfos [ AssignmentListT::BINDER_COUNT ];
    std::uint32_t d_bindings [ AssignmentListT::BINDER_COUNT ];
    VkBuffer d_buffers [ AssignmentListT::BINDER_COUNT ];
    VkDeviceSize d_offsets [ AssignmentListT::BINDER_COUNT ];
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class AssignmentListT >
VPP_INLINE void PipelineConfig :: cmdBindVertexInput (
    const AssignmentListT& list,
    CommandBuffer hCommandBuffer )
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    detail::TVertexInputBinder< AssignmentListT > binder ( CommandBuffer ( hCmdBuffer ), *this );
    binder.bind ( list );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPIPELINECONFIG_HPP
