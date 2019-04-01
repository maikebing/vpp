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

#ifndef INC_VPPSUPPORTGLM_HPP
#define INC_VPPSUPPORTGLM_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMON_HPP
#include "vppCommon.hpp"
#endif

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tvec2< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( glm::tvec2< T, P > );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRVector< gscalar_type, 2 > rvalue_type;
    typedef TLVector< gscalar_type, 2, spv::StorageClassFunction > lvalue_type;
    typedef glm::tvec2< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tvec3< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( glm::tvec3< T, P > );
    static const unsigned int row_count = 3u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRVector< gscalar_type, 3 > rvalue_type;
    typedef TLVector< gscalar_type, 3, spv::StorageClassFunction > lvalue_type;
    typedef glm::tvec3< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tvec4< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( glm::tvec4< T, P > );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< T, T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRVector< gscalar_type, 4 > rvalue_type;
    typedef TLVector< gscalar_type, 4, spv::StorageClassFunction > lvalue_type;
    typedef glm::tvec4< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat2x2< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 2 * sizeof ( T );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 2u;
    static const unsigned int attrib_count = 2u;
    static const VkFormat format_code = format< T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 2, 2 > rvalue_type;
    typedef TLMatrix< gscalar_type, 2, 2, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat2x2< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat3x3< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 3 * sizeof ( T );
    static const unsigned int row_count = 3u;
    static const unsigned int column_count = 3u;
    static const unsigned int attrib_count = 3u;
    static const VkFormat format_code = format< T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 3, 3 > rvalue_type;
    typedef TLMatrix< gscalar_type, 3, 3, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat3x3< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat4x4< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 4 * sizeof ( T );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 4u;
    static const unsigned int attrib_count = 4u;
    static const VkFormat format_code = format< T, T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 4, 4 > rvalue_type;
    typedef TLMatrix< gscalar_type, 4, 4, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat4x4< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat2x3< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 3 * sizeof ( T );
    static const unsigned int row_count = 3u;
    static const unsigned int column_count = 2u;
    static const unsigned int attrib_count = 2u;
    static const VkFormat format_code = format< T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 2, 3 > rvalue_type;
    typedef TLMatrix< gscalar_type, 2, 3, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat2x3< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat2x4< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 4 * sizeof ( T );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 2u;
    static const unsigned int attrib_count = 2u;
    static const VkFormat format_code = format< T, T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 2, 4 > rvalue_type;
    typedef TLMatrix< gscalar_type, 2, 4, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat2x4< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat3x2< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 2 * sizeof ( T );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 3u;
    static const unsigned int attrib_count = 3u;
    static const VkFormat format_code = format< T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 3, 2 > rvalue_type;
    typedef TLMatrix< gscalar_type, 3, 2, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat3x2< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat3x4< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 4 * sizeof ( T );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 3u;
    static const unsigned int attrib_count = 3u;
    static const VkFormat format_code = format< T, T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 3, 4 > rvalue_type;
    typedef TLMatrix< gscalar_type, 3, 4, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat3x4< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat4x2< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 2 * sizeof ( T );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 4u;
    static const unsigned int attrib_count = 4u;
    static const VkFormat format_code = format< T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 4, 2 > rvalue_type;
    typedef TLMatrix< gscalar_type, 4, 2, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat4x2< T, P > data_type;
};

// -----------------------------------------------------------------------------

template< typename T, glm::precision P >
struct StructMemberTraits< glm::tmat4x3< T, P > >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 3 * sizeof ( T );
    static const unsigned int row_count = 3u;
    static const unsigned int column_count = 4u;
    static const unsigned int attrib_count = 4u;
    static const VkFormat format_code = format< T, T, T >::code;
    typedef T scalar_type;
    typedef TRValue< T > gscalar_type;
    typedef TRMatrix< gscalar_type, 4, 3 > rvalue_type;
    typedef TLMatrix< gscalar_type, 4, 3, spv::StorageClassFunction > lvalue_type;
    typedef glm::tmat4x3< T, P > data_type;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSUPPORTGLM_HPP
