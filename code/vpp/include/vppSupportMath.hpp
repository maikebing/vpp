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

#ifndef INC_VPPSUPPORTMATH_HPP
#define INC_VPPSUPPORTMATH_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMON_HPP
#include "vppCommon.hpp"
#endif

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

#ifndef INC_VPPFORMATS_HPP
#include "vppFormats.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct vect2 : public format< float, float >::data_type
{
    typedef format< float, float >::data_type base_type;
    VPP_INLINE vect2() {}
    VPP_INLINE vect2 ( float x, float y ) : base_type ( x, y ) {}
};

// -----------------------------------------------------------------------------

struct vect3 : public format< float, float, float, float >::data_type
{
    typedef format< float, float, float, float >::data_type base_type;
    VPP_INLINE vect3() {}
    VPP_INLINE vect3 ( float x, float y, float z ) : base_type ( x, y, z, 0.0f ) {}
};

// -----------------------------------------------------------------------------

struct vect4 : public format< float, float, float, float >::data_type
{
    typedef format< float, float, float, float >::data_type base_type;
    VPP_INLINE vect4() {}
    VPP_INLINE vect4 ( float x, float y, float z, float w ) : base_type ( x, y, z, w ) {}
};

// -----------------------------------------------------------------------------

struct matr2
{
    VPP_INLINE matr2() {}
    VPP_INLINE matr2 ( const vect2& c0, const vect2& c1 ) : d_columns { c0, c1 } {}
    vect2 d_columns [ 2 ];
};

// -----------------------------------------------------------------------------

struct matr3
{
    VPP_INLINE matr3() {}
    VPP_INLINE matr3 ( const vect3& c0, const vect3& c1, const vect3& c2 ) :
        d_columns { c0, c1, c2 } {}
    vect3 d_columns [ 3 ];
};

// -----------------------------------------------------------------------------

struct matr4
{
    VPP_INLINE matr4() {}
    VPP_INLINE matr4 ( const vect4& c0, const vect4& c1, const vect4& c2, const vect4& c3 ) :
        d_columns { c0, c1, c2, c3 } {}
    vect4 d_columns [ 4 ];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct ivect2 : public format< int, int >::data_type
{
    typedef format< int, int >::data_type base_type;
    VPP_INLINE ivect2() {}
    VPP_INLINE ivect2 ( int x, int y ) : base_type ( x, y ) {}
};

// -----------------------------------------------------------------------------

struct ivect3 : public format< int, int, int, int >::data_type
{
    typedef format< int, int, int, int >::data_type base_type;
    VPP_INLINE ivect3() {}
    VPP_INLINE ivect3 ( int x, int y, int z ) : base_type ( x, y, z, 0 ) {}
};

// -----------------------------------------------------------------------------

struct ivect4 : public format< int, int, int, int >::data_type
{
    typedef format< int, int, int, int >::data_type base_type;
    VPP_INLINE ivect4() {}
    VPP_INLINE ivect4 ( int x, int y, int z, int w ) : base_type ( x, y, z, w ) {}
};

// -----------------------------------------------------------------------------

struct uvect2 : public format< unsigned int, unsigned int >::data_type
{
    typedef format< unsigned int, unsigned int >::data_type base_type;
    VPP_INLINE uvect2() {}
    VPP_INLINE uvect2 ( unsigned int x, unsigned int y ) : base_type ( x, y ) {}
};

// -----------------------------------------------------------------------------

struct uvect3 : public format< unsigned int, unsigned int, unsigned int, unsigned int >::data_type
{
    typedef format< unsigned int, unsigned int, unsigned int, unsigned int >::data_type base_type;
    VPP_INLINE uvect3() {}
    VPP_INLINE uvect3 ( unsigned int x, unsigned int y, unsigned int z ) : base_type ( x, y, z, 0 ) {}
};

// -----------------------------------------------------------------------------

struct uvect4 : public format< unsigned int, unsigned int, unsigned int, unsigned int >::data_type
{
    typedef format< unsigned int, unsigned int, unsigned int, unsigned int >::data_type base_type;
    VPP_INLINE uvect4() {}
    VPP_INLINE uvect4 ( unsigned int x, unsigned int y, unsigned int z, unsigned int w ) : base_type ( x, y, z, w ) {}
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< vect2 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( vect2 );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRVector< gscalar_type, 2 > rvalue_type;
    typedef TLVector< gscalar_type, 2, spv::StorageClassFunction > lvalue_type;
    typedef vect2 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< vect3 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( vect3 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRVector< gscalar_type, 3 > rvalue_type;
    typedef TLVector< gscalar_type, 3, spv::StorageClassFunction > lvalue_type;
    typedef vect3 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< vect4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( vect4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRVector< gscalar_type, 4 > rvalue_type;
    typedef TLVector< gscalar_type, 4, spv::StorageClassFunction > lvalue_type;
    typedef vect4 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< matr2 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = sizeof ( vect2 );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 2u;
    static const unsigned int attrib_count = 2u;
    static const VkFormat format_code = format< float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRMatrix< gscalar_type, 2, 2 > rvalue_type;
    typedef TLMatrix< gscalar_type, 2, 2, spv::StorageClassFunction > lvalue_type;
    typedef matr2 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< matr3 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = sizeof ( vect3 );
    static const unsigned int row_count = 3u;
    static const unsigned int column_count = 3u;
    static const unsigned int attrib_count = 3u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRMatrix< gscalar_type, 3, 3 > rvalue_type;
    typedef TLMatrix< gscalar_type, 3, 3, spv::StorageClassFunction > lvalue_type;
    typedef matr3 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< matr4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = sizeof ( vect4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 4u;
    static const unsigned int attrib_count = 4u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRMatrix< gscalar_type, 4, 4 > rvalue_type;
    typedef TLMatrix< gscalar_type, 4, 4, spv::StorageClassFunction > lvalue_type;
    typedef matr4 data_type;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< ivect2 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( ivect2 );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< int, int >::code;
    typedef int scalar_type;
    typedef TRValue< int > gscalar_type;
    typedef TRVector< gscalar_type, 2 > rvalue_type;
    typedef TLVector< gscalar_type, 2, spv::StorageClassFunction > lvalue_type;
    typedef ivect2 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< ivect3 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( ivect3 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< int, int, int, int >::code;
    typedef int scalar_type;
    typedef TRValue< int > gscalar_type;
    typedef TRVector< gscalar_type, 3 > rvalue_type;
    typedef TLVector< gscalar_type, 3, spv::StorageClassFunction > lvalue_type;
    typedef ivect3 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< ivect4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( ivect4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< int, int, int, int >::code;
    typedef int scalar_type;
    typedef TRValue< int > gscalar_type;
    typedef TRVector< gscalar_type, 4 > rvalue_type;
    typedef TLVector< gscalar_type, 4, spv::StorageClassFunction > lvalue_type;
    typedef ivect4 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< uvect2 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( uvect2 );
    static const unsigned int row_count = 2u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< unsigned int, unsigned int >::code;
    typedef unsigned int scalar_type;
    typedef TRValue< unsigned int > gscalar_type;
    typedef TRVector< gscalar_type, 2 > rvalue_type;
    typedef TLVector< gscalar_type, 2, spv::StorageClassFunction > lvalue_type;
    typedef uvect2 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< uvect3 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( uvect3 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< unsigned int, unsigned int, unsigned int, unsigned int >::code;
    typedef unsigned int scalar_type;
    typedef TRValue< unsigned int > gscalar_type;
    typedef TRVector< gscalar_type, 3 > rvalue_type;
    typedef TLVector< gscalar_type, 3, spv::StorageClassFunction > lvalue_type;
    typedef uvect3 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< uvect4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( uvect4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< unsigned int, unsigned int, unsigned int, unsigned int >::code;
    typedef unsigned int scalar_type;
    typedef TRValue< unsigned int > gscalar_type;
    typedef TRVector< gscalar_type, 4 > rvalue_type;
    typedef TLVector< gscalar_type, 4, spv::StorageClassFunction > lvalue_type;
    typedef uvect4 data_type;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSUPPORTMATH_HPP
