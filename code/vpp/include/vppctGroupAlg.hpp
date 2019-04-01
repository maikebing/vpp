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

#ifndef INC_VPPCTGROUPALG_HPP
#define INC_VPPCTGROUPALG_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGAGGREGATES_HPP
#include "vppLangAggregates.hpp"
#endif

#ifndef INC_VPPLANGINTERFACE_HPP
#include "vppLangInterface.hpp"
#endif

#ifndef INC_VPPINTERNALUTILS_HPP
#include "vppInternalUtils.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace ct {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace group {
// -----------------------------------------------------------------------------

class GroupInvocation
{
public:
    GroupInvocation ( vpp::ComputeShader* pShader );

    VPP_INLINE vpp::ComputeShader* shader() const { return d_pShader; }
    VPP_INLINE const vpp::Int& LocalId() const { return d_localId; }
    VPP_INLINE const vpp::Int& LocalCount() const { return d_localCount; }
    VPP_INLINE int localCount() const { return d_staticLocalCount; }

private:
    vpp::ComputeShader* d_pShader;
    const vpp::Int d_localId;
    const vpp::Int d_localCount;
    const int d_staticLocalCount;
};

// -----------------------------------------------------------------------------

VPP_INLINE GroupInvocation :: GroupInvocation ( vpp::ComputeShader* pShader ) :
    d_pShader ( pShader ),
    d_localId ( pShader->inLocalInvocationId [ vpp::X ] ),
    d_localCount ( pShader->inWorkgroupSize [ vpp::X ] ),
    d_staticLocalCount ( pShader->localGroupSize().x )
{
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

VPP_INLINE Int FastDivision ( const Int& i, int div, int maxDividend )
{
    // Ranges: lhs: 0 .. 32767, rhs: 1 .. 255

    if ( maxDividend < 32768 && div < 256 )
    {
        const unsigned int udiv = static_cast< unsigned int >( div );
        const unsigned int pos = vpp::detail::lowestBit ( udiv );

        if ( vpp::detail::count1bits ( udiv ) == 1 )
            return i >> static_cast< int >( pos );
        else
        {
            const unsigned int rd = udiv >> pos;
            const unsigned int m = ( rd - 1 ) >> 1;

            return
                ( i * static_cast< int >( vpp::detail::s_fastDivisionMultipliers [ m ] ) )
                    >> static_cast< int >( vpp::detail::s_fastDivisionShifts [ m ] + pos );
        }
    }
    else
        return i / div;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t DIM, class ArrayT >
struct TIndexGenerator
{
};

// -----------------------------------------------------------------------------

template< class ArrayT >
struct TIndexGenerator< 1, ArrayT >
{
    template< class FunctorT >
    void Apply ( FunctorT&& fFunctor, int size, const GroupInvocation& inv );

    template< typename ValueT >
    void Fill ( const ArrayT& arr, const ValueT& value, const GroupInvocation& inv );

    template< class FunctorT >
    void Generate ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv );

    template< class FunctorT >
    void Transform ( const ArrayT& inArr, const ArrayT& outArr, FunctorT&& fFunctor, const GroupInvocation& inv );

    void Copy ( const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv );

    template< class SourceArrayT >
    void Load (
        const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
        const ArrayT& outArr, const GroupInvocation& inv );

    template< class TargetArrayT >
    void Store (
        const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor,
        const Int& destStartIndex, const GroupInvocation& inv );
};

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class FunctorT >
void TIndexGenerator< 1, ArrayT > :: Apply (
    FunctorT&& fFunctor, int size, const GroupInvocation& inv )
{
    const int nLocalThreads = inv.localCount();
    const Int nThisThread = inv.LocalId();
    const Int totalSize = size;

    if ( size <= 3*nLocalThreads )
    {
        int i = 0;

        for ( ; i + nLocalThreads <= size ; i += nLocalThreads )
            fFunctor ( nThisThread + i );

        if ( i < size )
        {
            If ( nThisThread + i < totalSize );
                fFunctor ( nThisThread + i );
            Fi();
        }
    }
    else
    {
        VInt i;

        For ( i, 0, totalSize - (nLocalThreads-1), nLocalThreads );
            fFunctor ( nThisThread + i );
        Rof();

        If ( nThisThread + i < totalSize );
            fFunctor ( nThisThread + i );
        Fi();
    }

    WorkgroupBarrier();
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< typename ValueT >
void TIndexGenerator< 1, ArrayT > :: Fill (
    const ArrayT& arr, const ValueT& val, const GroupInvocation& inv )
{
    Apply ( [ & arr, & val ]( const Int& i ) { arr [ i ] = val; }, arr.size(), inv );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class FunctorT >
void TIndexGenerator< 1, ArrayT > :: Generate (
    const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    Apply ( [ & arr, & fFunctor ]( const Int& i ) { arr [ i ] = fFunctor ( i ); }, arr.size(), inv );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class FunctorT >
void TIndexGenerator< 1, ArrayT > :: Transform (
    const ArrayT& inArr, const ArrayT& outArr,
    FunctorT&& fFunctor, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & fFunctor ]( const Int& i ) { outArr [ i ] = fFunctor ( inArr [ i ] ); },
        std::min ( inArr.size(), outArr.size() ), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
void TIndexGenerator< 1, ArrayT > :: Copy (
    const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr ]( const Int& i ) { outArr [ i ] = inArr [ i ]; },
        std::min ( inArr.size(), outArr.size() ), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class SourceArrayT >
void TIndexGenerator< 1, ArrayT > :: Load (
    const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
    const ArrayT& outArr, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & sourceStartIndex ]( const Int& i )
        {
            outArr [ i ] = inArr [ i + sourceStartIndex ];
        },
        outArr.size(), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class TargetArrayT >
void TIndexGenerator< 1, ArrayT > :: Store (
    const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor,
    const Int& destStartIndex, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & destStartIndex ]( const Int& i )
        {
            outArr [ i + destStartIndex ] = inArr [ i ];
        },
        inArr.size(), inv
    );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ArrayT >
struct TIndexGenerator< 2, ArrayT >
{
    template< typename FunctorT >
    void Apply ( FunctorT&& fFunctor, int nRows, int nCols, const GroupInvocation& inv );

    template< typename ValueT >
    void Fill ( const ArrayT& arr, const ValueT& value, const GroupInvocation& inv );

    template< class FunctorT >
    void Generate ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv );

    template< class FunctorT >
    void Transform ( const ArrayT& inArr, const ArrayT& outArr, FunctorT&& fFunctor, const GroupInvocation& inv );

    void Copy ( const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv );

    template< class SourceArrayT >
    void Load (
        const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
        const ArrayT& outArr, const GroupInvocation& inv );

    template< class TargetArrayT >
    void Store (
        const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor, 
        const Int& destStartIndex, const GroupInvocation& inv );
};

// -----------------------------------------------------------------------------

template< class ArrayT >
template< typename FunctorT >
void TIndexGenerator< 2, ArrayT > :: Apply (
    FunctorT&& fFunctor, int nRows, int nCols, const GroupInvocation& inv )
{
    const int nLocalThreads = inv.localCount();
    const int size = nRows * nCols;
    const Int nThisThread = inv.LocalId();
    const Int totalSize = size;

    if ( size <= 3*nLocalThreads )
    {
        int i = 0;

        for ( ; i + nLocalThreads <= size ; i += nLocalThreads )
        {
            const Int linearIdx = nThisThread + i;
            const Int row = FastDivision ( linearIdx, nCols, size );
            const Int col = linearIdx - row * nCols;
            fFunctor ( row, col );
        }

        if ( i < size )
        {
            If ( nThisThread + i < totalSize );
            {
                const Int linearIdx = nThisThread + i;
                const Int row = FastDivision ( linearIdx, nCols, size );
                const Int col = linearIdx - row * nCols;
                fFunctor ( row, col );
            }
            Fi();
        }
    }
    else
    {
        VInt i;

        For ( i, 0, totalSize - (nLocalThreads-1), nLocalThreads );
        {
            const Int linearIdx = nThisThread + i;
            const Int row = FastDivision ( linearIdx, nCols, size );
            const Int col = linearIdx - row * nCols;
            fFunctor ( row, col );
        }
        Rof();

        If ( nThisThread + i < totalSize );
        {
            const Int linearIdx = nThisThread + i;
            const Int row = FastDivision ( linearIdx, nCols, size );
            const Int col = linearIdx - row * nCols;
            fFunctor ( row, col );
        }
        Fi();
    }

    WorkgroupBarrier();
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< typename ValueT >
void TIndexGenerator< 2, ArrayT > :: Fill (
    const ArrayT& arr, const ValueT& val, const GroupInvocation& inv )
{
    Apply (
        [ & arr, & val ]( const Int& row, const Int& col ) { arr ( row, col ) = val; },
        arr.rows(), arr.cols(), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class FunctorT >
void TIndexGenerator< 2, ArrayT > :: Generate (
    const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    Apply (
        [ & arr, & fFunctor ]( const Int& row, const Int& col ) { arr ( row, col ) = fFunctor ( row, col ); },
        arr.rows(), arr.cols(), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class FunctorT >
void TIndexGenerator< 2, ArrayT > :: Transform (
    const ArrayT& inArr, const ArrayT& outArr,
    FunctorT&& fFunctor, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & fFunctor ]( const Int& row, const Int& col ) {
            outArr ( row, col ) = fFunctor ( inArr ( row, col ) ); },
        std::min ( inArr.rows(), outArr.rows() ),
        std::min ( inArr.cols(), outArr.cols() ),
        inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
void TIndexGenerator< 2, ArrayT > :: Copy (
    const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr ]( const Int& row, const Int& col ) { outArr ( row, col ) = inArr ( row, col ); },
        std::min ( inArr.rows(), outArr.rows() ),
        std::min ( inArr.cols(), outArr.cols() ),
        inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class SourceArrayT >
void TIndexGenerator< 2, ArrayT > :: Load (
    const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
    const ArrayT& outArr, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & sourceStartIndex, bColumnMajor ]( const Int& iRow, const Int& iCol )
        {
            if ( bColumnMajor )
                outArr ( iRow, iCol ) = inArr [ iCol * outArr.Rows() + iRow + sourceStartIndex ];
            else
                outArr ( iRow, iCol ) = inArr [ iRow * outArr.Cols() + iCol + sourceStartIndex ];
        },
        outArr.rows(), outArr.cols(), inv
    );
}

// -----------------------------------------------------------------------------

template< class ArrayT >
template< class TargetArrayT >
void TIndexGenerator< 2, ArrayT > :: Store (
    const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor,
    const Int& destStartIndex, const GroupInvocation& inv )
{
    Apply (
        [ & inArr, & outArr, & destStartIndex, bColumnMajor ]( const Int& iRow, const Int& iCol )
        {
            if ( bColumnMajor )
                outArr [ iCol * inArr.Rows() + iRow + destStartIndex ] = inArr ( iRow, iCol );
            else
                outArr [ iRow * inArr.Cols() + iCol + destStartIndex ] = inArr ( iRow, iCol );
        },
        inArr.rows(), inArr.cols(), inv
    );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ValueT, class FunctorT >
bool InclusiveScanSmall (
    WArray< ValueT >& smallArray, int length, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    const int nLocalThreads = inv.localCount();

    if ( length <= nLocalThreads )
    {
        const Int nThisThread = inv.LocalId();
        const Int nSize = length;

        for ( int stride = 1; stride < length; stride <<= 1 )
        {
            const Int secondIdx = nThisThread + stride;
            
            WorkgroupBarrier();

            If ( secondIdx < nSize );
            {
                const ValueT t1 = smallArray [ nThisThread ];
                const ValueT t2 = smallArray [ secondIdx ];
                const ValueT t3 = fFunctor ( t1, t2 );
                smallArray [ secondIdx ] = t3;
            }
            Fi();
        }

        WorkgroupBarrier();

        return true;
    }
    else
        return false;
}

// -----------------------------------------------------------------------------

template< typename ValueT, class FunctorT >
bool ExclusiveScanSmall (
    WArray< ValueT >& smallArray, int length, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    if ( ! InclusiveScanSmall ( smallArray, length, fFunctor, inv ) )
        return false;

    const Int nThisThread = inv.LocalId();
    const Int nSize = length;
    const ValueT zero;

    If ( nThisThread < nSize );
        const ValueT cv = Select ( nThisThread == 0, zero, smallArray [ nThisThread - 1 ] );
        smallArray [ nThisThread ] = cv;
    Fi();

    WorkgroupBarrier();
    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void SortLocations ( const ArrayT& arr, const UInt& i1, const UInt& i2, FunctorT&& fFunctor )
{
    typedef typename ArrayT::base_type ValueT;
    const ValueT v1 = arr [ i1 ];
    const ValueT v2 = arr [ i2 ];
    const Bool bSorted = fFunctor ( v1, v2 );

    If ( ! bSorted );
        arr [ i1 ] = v2;
        arr [ i2 ] = v1;
    Fi();
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void SortLocations ( const ArrayT& arr, const Int& i1, const Int& i2, FunctorT&& fFunctor )
{
    typedef typename ArrayT::base_type ValueT;
    const ValueT v1 = arr [ i1 ];
    const ValueT v2 = arr [ i2 ];
    const Bool bSorted = fFunctor ( v1, v2 );

    If ( ! bSorted );
        arr [ i1 ] = v2;
        arr [ i2 ] = v1;
    Fi();
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void SortBatcherBitonicSmall (
    const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    const unsigned int ci = static_cast< unsigned int >( arr.size() );
    const unsigned int d = vpp::detail::highestBit ( ci - 1 ) + 1;

    const UInt nLocalThreads = StaticCast< UInt >( inv.LocalCount() );
    const UInt nThisThread = StaticCast< UInt >( inv.LocalId() );

    for ( unsigned int l = 1; l <= d; ++l )
    {
        const unsigned int l1 = l - 1;

        {
            const unsigned int h = 1u << l1;
            const unsigned int h1 = h - 1;
            const unsigned int ns = 1 + ( ( ci - h - 1 ) >> l );
            const UInt nt = ( ns << l1 );
            const UInt t = nThisThread;
            const UInt k = t & h1;
            const UInt i = t >> l1;
            const UInt a = ( i << l ) + k;
            const UInt b = ( ( i + 1 ) << l ) - 1 - k;

            If ( b < ci && t < nt );
                SortLocations ( arr, a, b, fFunctor );
            Fi();

            WorkgroupBarrier();
        }

        for ( unsigned int p = 2; p <= l; ++p )
        {
            const unsigned int lp = ( l - p );
            const unsigned int lp1 = lp + 1;
            const unsigned int h = 1u << lp;
            const unsigned int h1 = h - 1;
            const unsigned int ns = 1 + ( ( ( ( ci-1 ) >> lp ) - 1 ) >> 1 );
            const UInt nt = ( ns << lp );
            const UInt t = nThisThread;
            const UInt k = t & h1;
            const UInt i = t >> lp;
            const UInt a = ( i << lp1 ) + k;
            const UInt b = a + h;

            If ( b < ci && t < nt );
                SortLocations ( arr, a, b, fFunctor );
            Fi();

            WorkgroupBarrier();
        }
    }
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void SortBatcherBitonicLarge (
    const ArrayT& arr, FunctorT&& fFunctor, 
    const GroupInvocation& inv )
{
    const int ci = arr.size();
    const int d = static_cast< int >( vpp::detail::highestBit ( static_cast< unsigned int >( ci ) - 1 ) + 1 );
    const Int nLocalThreads = inv.LocalCount();
    const Int nThisThread = inv.LocalId();

    VInt tvar1, tvar2, tvar3;

    For ( tvar3, 1, d + 1 );
    {
        const Int l = tvar3;
        const Int l1 = l - 1;

        {
            const Int h = 1 << l1;
            const Int h1 = h - 1;
            const Int ns = 1 + ( ( ci - h - 1 ) >> l );
            const Int nt = ( ns << l1 );

            For ( tvar2, 0, nt, nLocalThreads );
            {
                const Int t = tvar2 + nThisThread;
                const Int k = t & h1;
                const Int i = t >> l1;
                const Int a = ( i << l ) + k;
                const Int b = ( ( i + 1 ) << l ) - 1 - k;

                If ( b < ci && t < nt );
                    SortLocations ( arr, a, b, fFunctor );
                Fi();
            }
            Rof();

            WorkgroupBarrier();
        }

        For ( tvar1, 2, l + 1 );
        {
            const Int p = tvar1;
            const Int lp = ( l - p );
            const Int lp1 = lp + 1;
            const Int h = 1 << lp;
            const Int h1 = h - 1;
            const Int ns = 1 + ( ( ( ( ci-1 ) >> lp ) - 1 ) >> 1 );
            const Int nt = ( ns << lp );

            For ( tvar2, 0, nt, nLocalThreads );
            {
                const Int t = tvar2 + nThisThread;
                const Int k = t & h1;
                const Int i = t >> lp;
                const Int a = ( i << lp1 ) + k;
                const Int b = a + h;

                If ( b < ci && t < nt );
                    SortLocations ( arr, a, b, fFunctor );
                Fi();
            }
            Rof();

            WorkgroupBarrier();
        }
        Rof();
    }
    Rof();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ValueT >
class BinarySearchImpl
{
public:
    BinarySearchImpl (
        const WArray< ValueT >& arr,
        const ValueT& value,
        WArray< Int >& tmpArr,
        const GroupInvocation& inv,
        bool bUpperBound );

    template< typename FunctorT >
    Int operator() ( FunctorT&& fFunctor );

    template< typename FunctorT >
    Int Stage ( FunctorT&& fFunctor, const Int& first, const Int& last, const Int& result, int iter );

private:
    const WArray< ValueT >& d_array;
    const ValueT& d_value;
    WArray< Int >& d_tmpArray;
    const GroupInvocation& d_invocation;
    bool d_bUpperBound;
    int d_nIterations;

    const Int d_nThisThread;
    const Float d_fnThreads;
    const Float d_fnThisThread;
    const Int d_iPrevCmp;
    const Int d_iCmp;
    const Int d_nPrevThread;
};

// -----------------------------------------------------------------------------

template< typename ValueT >
BinarySearchImpl< ValueT > :: BinarySearchImpl (
    const WArray< ValueT >& arr,
    const ValueT& value,
    WArray< Int >& tmpArr,
    const GroupInvocation& inv,
    bool bUpperBound ) :
        d_array ( arr ),
        d_value ( value ),
        d_tmpArray ( tmpArr ),
        d_invocation ( inv ),
        d_bUpperBound ( bUpperBound ),
        d_nIterations ( 0 ),
        d_nThisThread ( inv.LocalId() ),
        d_fnThreads ( static_cast< float >( inv.localCount() ) ),
        d_fnThisThread ( StaticCast< Float >( d_nThisThread ) ),
        d_iPrevCmp ( d_nThisThread + inv.localCount() ),
        d_iCmp ( d_nThisThread + inv.localCount() + 1 ),
        d_nPrevThread ( d_nThisThread - 1 )
{
    const int nThreads = inv.localCount();
    const int length = arr.size();

    d_nIterations = 1 + static_cast< int >(
        std::log ( 1.0 + ( (nThreads-2.0)/(nThreads-1.0) )*( length - 1.0 ) ) / std::log ( nThreads-1.0 ) );
}

// -----------------------------------------------------------------------------

template< typename ValueT >
template< typename FunctorT >
Int BinarySearchImpl< ValueT > :: Stage (
    FunctorT&& fFunctor, const Int& first, const Int& last, const Int& result, int iter )
{
    const int nThreads = d_invocation.localCount();
    const float q1 = static_cast< float >( 1.0 / ( nThreads - 1.0 ) );

    const auto& interval = d_tmpArray [ 2*nThreads + 1 ];

    If ( result == -1 );
    {
        const Float ffirst = StaticCast< Float >( first );
        const Float flast = StaticCast< Float >( last );
        const Float a = ( flast - ffirst ) * q1;
        const Int idx = StaticCast< Int >( Floor ( a * d_fnThisThread + ffirst + 0.5f ) );
        d_tmpArray [ d_nThisThread ] = idx;
        const ValueT item = d_array [ idx ];
        const ValueT arg1 = ( d_bUpperBound ? d_value : item );
        const ValueT arg2 = ( d_bUpperBound ? item : d_value );
        const Int c = Select ( fFunctor ( arg1, arg2 ), 1, 0 );
        d_tmpArray [ d_iCmp ] = c;
    }
    Fi();

    If ( d_nThisThread == 0 );
        interval = nThreads - 1;
    Fi();

    WorkgroupBarrier();

    if ( d_bUpperBound )
        If ( d_tmpArray [ d_iPrevCmp ] != 1 && d_tmpArray [ d_iCmp ] == 1 );
    else
        If ( d_tmpArray [ d_iPrevCmp ] == 1 && d_tmpArray [ d_iCmp ] != 1 );

        interval = d_nPrevThread;
    Fi();

    WorkgroupBarrier();

    const Int foundInterval = interval;

    if ( iter == d_nIterations - 1 )
    {
        const Int f = d_tmpArray [ foundInterval ] + 1;
        const Int p = d_tmpArray [ foundInterval + 1 ];
        const Bool b1 = ( foundInterval == nThreads-1 );
        const Bool b2 = ( foundInterval == -1 );
        const Int s2 = Select ( b2, first, p );
        const Int s1 = Select ( b1, last + 1, s2 );
        return Select ( result == -1, s1, result );
    }
    else
    {
        const Int f = d_tmpArray [ foundInterval ] + 1;
        const Int p = d_tmpArray [ foundInterval + 1 ];
        const Int l = p - 1;
        const Bool b1 = ( foundInterval == nThreads-1 );
        const Bool b2 = ( foundInterval == -1 );
        const Bool b3 = ( l < f );
        const Int s3 = Select ( b3, p, -1 );
        const Int s2 = Select ( b2, first, s3 );
        const Int s1 = Select ( b1, last + 1, s2 );

        return Stage ( fFunctor, f, l, Select ( result == -1, s1, result ), iter + 1 );
    }
}

// -----------------------------------------------------------------------------

template< typename ValueT >
template< typename FunctorT >
Int BinarySearchImpl< ValueT > :: operator() ( FunctorT&& fFunctor )
{
    const int nThreads = d_invocation.localCount();
    const int length = d_array.size();
    const float q1 = static_cast< float >( 1.0 / ( nThreads - 1.0 ) );

    d_tmpArray [ d_iPrevCmp ] = d_bUpperBound ? 0 : 1;
    const auto& interval = d_tmpArray [ 2*nThreads + 1 ];

    const float ffirst = 0.0f;
    const float flast = static_cast< float >( length - 1 );
    const float a = ( flast - ffirst ) * q1;
    const float ffirstb = ffirst + 0.5f;
    const Int idx = StaticCast< Int >( Floor ( a * d_fnThisThread + ffirstb ) );
    d_tmpArray [ d_nThisThread ] = idx;
    const ValueT item = d_array [ idx ];
    const ValueT arg1 = ( d_bUpperBound ? d_value : item );
    const ValueT arg2 = ( d_bUpperBound ? item : d_value );
    const Int c = Select ( fFunctor ( arg1, arg2 ), 1, 0 );
    d_tmpArray [ d_iCmp ] = c;

    If ( d_nThisThread == 0 );
        interval = nThreads - 1;
    Fi();

    WorkgroupBarrier();

    if ( d_bUpperBound )
        If ( d_tmpArray [ d_iPrevCmp ] != 1 && d_tmpArray [ d_iCmp ] == 1 );
    else
        If ( d_tmpArray [ d_iPrevCmp ] == 1 && d_tmpArray [ d_iCmp ] != 1 );

        interval = d_nPrevThread;
    Fi();

    WorkgroupBarrier();

    const Int foundInterval = interval;

    if ( d_nIterations == 1 )
    {
        const Int f = d_tmpArray [ foundInterval ] + 1;
        const Int p = d_tmpArray [ foundInterval + 1 ];
        const Bool b1 = ( foundInterval == nThreads-1 );
        const Bool b2 = ( foundInterval == -1 );
        const Int s2 = Select ( b2, Int(0), p );
        const Int s1 = Select ( b1, Int(length), s2 );
        return s1;
    }
    else
    {
        const Int f = d_tmpArray [ foundInterval ] + 1;
        const Int p = d_tmpArray [ foundInterval + 1 ];
        const Int l = p - 1;
        const Bool b1 = ( foundInterval == nThreads-1 );
        const Bool b2 = ( foundInterval == -1 );
        const Bool b3 = ( l < f );
        const Int s3 = Select ( b3, p, -1 );
        const Int s2 = Select ( b2, Int(0), s3 );
        const Int s1 = Select ( b1, Int(length), s2 );

        return Stage ( fFunctor, f, l, s1, 1 );
    }
}

// -----------------------------------------------------------------------------

template< typename ValueT, typename FunctorT >
Int BinarySearch (
    const WArray< ValueT >& arr, const ValueT& v, FunctorT&& fFunctor, 
    WArray< Int >& tmpArr, const GroupInvocation& inv, bool bUpperBound )
{
    /*
        Layout of work array (WArray< Int > tmpArr):

        0 .. nThreads-1: intervals
        nThreads .. 2*nThreads: cmp
        2*nThreads + 1: interval

        Required size: 2*nThreads + 2.
    */

    BinarySearchImpl< ValueT > bsearch ( arr, v, tmpArr, inv, bUpperBound );
    return bsearch ( fFunctor );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
VPP_INLINE void Apply ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = arr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Apply ( fFunctor, lvt.size(), inv );
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename ValueT >
VPP_INLINE void Fill ( const ArrayT& arr, const ValueT& val, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = arr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator; 
    indexGenerator.Fill ( lvt, val, inv );
}

// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
VPP_INLINE void Generate ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = arr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator; 
    indexGenerator.Generate ( lvt, fFunctor, inv );
}

// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
VPP_INLINE void Transform (
    const ArrayT& inArr, const ArrayT& outArr,
    FunctorT&& fFunctor, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt1 = inArr;
    const lvalue_type& lvt2 = outArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Transform ( lvt1, lvt2, fFunctor, inv );
}

// -----------------------------------------------------------------------------

template< typename ArrayT >
VPP_INLINE void Copy (
    const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt1 = inArr;
    const lvalue_type& lvt2 = outArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Copy ( lvt1, lvt2, inv );
}

// -----------------------------------------------------------------------------

template< class SourceArrayT, typename ArrayT >
VPP_INLINE void Load (
    const SourceArrayT& inArr, const Int& sourceStartIndex, const ArrayT& outArr,
    const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = outArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Load ( inArr, false, sourceStartIndex, lvt, inv );
}

// -----------------------------------------------------------------------------

template< class SourceArrayT, typename ArrayT >
VPP_INLINE void Load (
    const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
    const ArrayT& outArr, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = outArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Load ( inArr, bColumnMajor, sourceStartIndex, lvt, inv );
}

// -----------------------------------------------------------------------------

template< class TargetArrayT, typename ArrayT >
VPP_INLINE void Store (
    const ArrayT& inArr, const TargetArrayT& outArr, const Int& destStartIndex, 
    const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = inArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Store ( lvt, outArr, false, destStartIndex, inv );
}

// -----------------------------------------------------------------------------

template< class TargetArrayT, typename ArrayT >
VPP_INLINE void Store (
    const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor, 
    const Int& destStartIndex, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = inArr;
    detail::TIndexGenerator< lvalue_type::dimensions, lvalue_type > indexGenerator;
    indexGenerator.Store ( lvt, outArr, bColumnMajor, destStartIndex, inv );
}

// -----------------------------------------------------------------------------

//template< typename ValueT >
//VPP_INLINE void Reverse (
//    WArray< ValueT >& inArr, WArray< ValueT >& outArr,
//    const GroupInvocation& inv )
//{
//    // UWAGA to nie bedzie dzialac
//
//    const int s = inArr.size();
//    const Int s1 = s - 1;
//
//    applyToIndexedItems (
//        [ & inArr, & outArr, & s1 ]( const Int& i ) { outArr [ s1 - i ] = inArr [ i ]; },
//        s, inv
//    );
//}

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
ValueT Reduce (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv )
{
    const int nLocalThreads = inv.localCount();
    const Int nThisThread = inv.LocalId();
    const Int totalSize = arr.size();
    const Int blockSize = ( arr.size() / nLocalThreads ) + ( ( arr.size() % nLocalThreads ) != 0 );
    const Int offset = blockSize * nThisThread;
    
    temporaryVal = ValueT();

    VInt temporaryIdx;

    For ( temporaryIdx, offset, Min ( offset + blockSize, totalSize ) );
        temporaryVal = fFunctor ( temporaryVal, arr [ temporaryIdx ] );
    Rof();

    tmpArr [ nThisThread ] = temporaryVal;

    WorkgroupBarrier();

    temporaryIdx = nLocalThreads / 2;

    Do(); While ( temporaryIdx > 0 );
        If ( nThisThread < temporaryIdx );
            tmpArr [ nThisThread ] = fFunctor (
                tmpArr [ nThisThread ], tmpArr [ temporaryIdx + nThisThread ] );
        Fi();
        temporaryIdx >>= 1;
    Od();

    WorkgroupBarrier();

    return tmpArr [ 0 ];
}

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
void InclusiveScan (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv )
{
    const int s = arr.size();

    if ( s < 2 )
        return;

    if ( detail::InclusiveScanSmall ( arr, s, fFunctor, inv ) )
        return;
    
    const int nLocalThreads = inv.localCount();
    const Int nThisThread = inv.LocalId();
    const Int totalSize = s;
    const Int blockSize = ( s / nLocalThreads ) + ( ( s % nLocalThreads ) != 0 );
    const Int offset = blockSize * nThisThread;

    temporaryVal = arr [ offset ];

    VInt temporaryIdx;

    For ( temporaryIdx, offset + 1, Min ( offset + blockSize, totalSize ) );
        temporaryVal = fFunctor ( temporaryVal, arr [ temporaryIdx ] );
        arr [ temporaryIdx ] = temporaryVal;
    Rof();

    tmpArr [ nThisThread ] = temporaryVal;

    WorkgroupBarrier();

    detail::ExclusiveScanSmall ( tmpArr, nLocalThreads, fFunctor, inv );

    For ( temporaryIdx, offset, Min ( offset + blockSize, totalSize ) );
        arr [ temporaryIdx ] = fFunctor ( tmpArr [ nThisThread ], arr [ temporaryIdx ] );
    Rof();

    WorkgroupBarrier();
}

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
void ExclusiveScan (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv )
{
    const int s = arr.size();

    switch ( s )
    {
        case 0:
            return;

        case 1:
            arr [ 0 ] = ValueT();
            return;

        default:
            break;
    }

    if ( detail::ExclusiveScanSmall ( arr, s, fFunctor, inv ) )
        return;
    
    const int nLocalThreads = inv.localCount();
    const Int nThisThread = inv.LocalId();
    const Int totalSize = s;
    const Int blockSize = ( s / nLocalThreads ) + ( ( s % nLocalThreads ) != 0 );
    const Int offset = blockSize * nThisThread;

    const ValueT zero;

    temporaryVal = zero;

    VInt temporaryIdx;

    For ( temporaryIdx, offset, Min ( offset + blockSize, totalSize ) );
    {
        const ValueT c = arr [ temporaryIdx ];
        arr [ temporaryIdx ] = temporaryVal;
        temporaryVal = fFunctor ( temporaryVal, c );
    }
    Rof();

    tmpArr [ nThisThread ] = temporaryVal;

    WorkgroupBarrier();

    detail::ExclusiveScanSmall ( tmpArr, nLocalThreads, fFunctor, inv );

    For ( temporaryIdx, offset, Min ( offset + blockSize, totalSize ) );
        arr [ temporaryIdx ] = fFunctor ( tmpArr [ nThisThread ], arr [ temporaryIdx ] );
    Rof();

    WorkgroupBarrier();
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void Sort (
    const ArrayT& arr, FunctorT&& fFunctor,
    const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    const lvalue_type& lvt = arr;

    const int s = lvt.size();

    if ( s >= 2 )
    {
        const int t = inv.localCount();
        const int r = vpp::detail::highestBit ( s - 1 ) + 1;
        const int st = ( 1u << (r-1) );

        if ( st <= t )
            detail::SortBatcherBitonicSmall ( lvt, fFunctor, inv );
        else
            detail::SortBatcherBitonicLarge ( lvt, fFunctor, inv );
    }
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename ValueT, typename FunctorT >
VPP_INLINE Int LowerBound (
    const ArrayT& arr, const ValueT& v, FunctorT&& fFunctor, 
    WArray< Int >& tmpArr, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    typedef typename lvalue_type::base_type item_type;
    const lvalue_type& lvt = arr;
    const item_type val = v;

    return detail::BinarySearch ( lvt, val, fFunctor, tmpArr, inv, false );
}

// -----------------------------------------------------------------------------

template< typename ArrayT, typename ValueT, typename FunctorT >
VPP_INLINE Int UpperBound (
    const ArrayT& arr, const ValueT& v, FunctorT&& fFunctor, 
    WArray< Int >& tmpArr, const GroupInvocation& inv )
{
    typedef typename ArrayT::lvalue_type lvalue_type;
    typedef typename lvalue_type::base_type item_type;
    const lvalue_type& lvt = arr;
    const item_type val = v;

    return detail::BinarySearch ( lvt, val, fFunctor, tmpArr, inv, true );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/*
    Nowe idee:
    - czy do funkcji mozna przekazac jakos referencje na tablice shared?

    - support dla typu Byte w shaderach
      - troche pizda bo bajty sa supportowane tylko w trybie Kernel
      - ale mozna wrapper zrobic
      - mozna do tego wrappera uzyc OpBitFieldInsert, OpBitFieldSExtract,
        OpBitFieldUExtract
      - VByteArray, VSharedField< VByteArray >

    - debugger:
      sciagnac i obczaic to:
         https://www.lunarg.com/vlf-simplifies-vulkan-validation-layer-creation/

    - algorytm sortowania dowolnego zakresu w tablicy (oparty na wersji "large")

    - algorytm szybkiego wyszukiwania (rownosciowego) w zbiorze nieuporzadkowanym

      po prostu porownujemy w kazdym threadzie i jesli znajdziemy rowny to wpisujemy
      numer threadu do zmiennej share'owanej

   - j/w ale dla lower_bound i upper_bound

   - algorytm unique dla zakresu uporzadkowanego 

   - szybka implementacja struktury typu std::set< ... >

     zbior musi miec z gory okreslony max rozmiar i zaalokowany obszar,
     przynajmniej dopoki nie znajdziemy sposobu jak alokowac dynamiczny
     obszar w shared memory

     ten zaalokowany obszar dzieli sie na 2 podobszary (kazdy moze byc
     pusty, rowniez oba):
     - obszar uporzadkowany (posortowany)
     - obszar nieuporzadkowany

     Obszar nieuporzadkowany znajduje sie tuz za obszarem uporzadkowanym
     i zawiera max T elementow (T to liczba threadow).

     W obszarze nieuporzadkowanym elementy musza byc unikalne.
     W obszarze uporzadkowanym elementy moga sie powtarzac, tworzac spojne serie.

     Wyszukiwanie:
     - jezeli obszar nieuporzadkowany jest niepusty to
       - uzyj w/w algorytmu szukania dla zbioru nieuporzadkowanego 
       - jesli element znaleziony to zwroc go i koncz
     - jesli pusty lub nie znaleziono to
       - uzyj w/w algorytmu szukania dla zbioru uporzadkowanego 
       - jesli element znaleziony to zwroc go i koncz
     - w przec. razie zwroc ze elementu nie ma

     Nawet dla duzych zbiorow to bedzie max 4 szukania grupowe.

     Wstawianie:
     - wyszukaj element w/w algorytmem zeby sprawdzic czy juz aby go nie ma
     - jesli nie ma to
       - jesli wielkosc zbioru nieuporzadkowanego jest rowna T,
         odpal sortowanie zakresu obejmujace oba zbiory (nieuporz i uporz)
         przez co utworzy sie jeden zbior uporz obejmujacy oba te obszary,
         a nowy zbior nieuporz bedzie pusty
       - wykonaj takze algorytm unique dla wynikowego zbioru uporz.
       - teraz mamy pewnosc ze wielkosc zbioru nieuporz < T
       - wstaw element do tego zbioru (dopisz na koniec)

       Koszt:
       - max 4 szukania grupowe (wyszukiwanie)
       - sortowanie, ale zamortyzowane bo wywolywane raz na 32 wstawienia
         czyli praktycznie O(1)
       - bardzo szybkie wstawienie elementu O(1)

    Usuwanie:
     - sprobuj wyszukac w zbiorze nieuporz.
     - jesli jest to usun go, a w puste miejsce przemiesc ostatni element
     - jesli nie ma to:
       - wykonaj algorytmy upper  i lower bound dla zbioru uporz.
       - jezeli przedzial niepusty to:
         - jezeli przedzial znajduje sie na poczatku to wez element tuz za nim
         - w przec. razie wez element tuz przed nim
         - zastap wszystkie elementy przedzialu tym sasiednim elementem,
           tworzac serie jednakowych wartosci

     - co iles usuniec (albo max dlugosc serii?) przesortowac zakresy

        Koszt: zalezy jak sie uda zaimplementowac upper i lower bound oraz
        nadpisywanie elementu (to akurat jako rownolegle search & replace
        powinno sie dac).

        Generalnie troche wiecej niz wstawianie alee tez szybkie.

    Koszt pamieci: zero narzutu (tylko klucze), ale struktura musi byc prealokowana.


    - czy w spirv istnieja jakies sposoby dynamicznej alokacji pamieci?
      nie za bardzo



*/

// -----------------------------------------------------------------------------
} // namespace group
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace ct
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCTGROUPALG_HPP
