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

#ifndef INC_VPPLANGINTBASE_HPP
#define INC_VPPLANGINTBASE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGACCESSOR_HPP
#include "vppLangAccessor.hpp"
#endif

#ifndef INC_VPPLANGBUILTINS_HPP
#include "vppLangBuiltins.hpp"
#endif

#ifndef INC_VPPLANGIMAGES_HPP
#include "vppLangImages.hpp"
#endif

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

#ifndef INC_VPPRENDERGRAPHNODES_HPP
#include "vppRenderGraphNodes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< typename C1, typename C2, typename C3, typename C4, typename C5 >
struct StructMemberTraits< format< C1, C2, C3, C4, C5 > >
{
    typedef typename format< C1, C2, C3, C4, C5 >::data_type data_type;

    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( data_type );
    static const unsigned int row_count = format< C1, C2, C3, C4, C5 >::length;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< C1, C2, C3, C4, C5 >::code;
    typedef typename format< C1, C2, C3, C4, C5 >::scalar_type scalar_type;
    typedef TRValue< scalar_type > gscalar_type;
    typedef TRVector< gscalar_type, row_count > rvalue_type;
    typedef TLVector< gscalar_type, row_count > lvalue_type;
};

// -----------------------------------------------------------------------------
// Arrays of resources
// -----------------------------------------------------------------------------

template< class SingleT >
class arrayOf
{
public:
    static const bool isDescriptorArrayAllowed = false;
    
    static_assert (
        SingleT::isDescriptorArrayAllowed,
        "This binding point is incompatible with arrayOf." );

    typedef typename SingleT::assigner_type single_assigner_type;
    typedef typename single_assigner_type::view_type single_view_type;
    typedef typename single_assigner_type::value_type single_value_type;
    typedef TUpdateMultipleDescriptors< single_value_type > multi_value_type;
    typedef typename SingleT::rvalue_type return_type;

    typedef detail::TDescriptorAssigner<
        single_assigner_type::type_code,
        single_view_type,
        multi_value_type
    > assigner_type;

    VPP_INLINE arrayOf (
        std::uint32_t count,
        std::uint32_t set = 0,
        int binding = -1 ) :
            d_resource ( set, binding, count ),
            d_count ( count )
    {}

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

    VPP_INLINE assigner_type operator= ( const multi_value_type& value )
    {
        return assigner_type (
            d_resource.id(), d_resource.set(), d_resource.binding(), value );
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
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGINTBASE_HPP
