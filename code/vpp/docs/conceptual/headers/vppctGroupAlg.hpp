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

/*! \file */ 

// -----------------------------------------------------------------------------
/** \brief The VPP namespace. */
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/** \brief The VPP Computation Toolkit namespace. */
namespace ct {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/** \brief The group algorithms namespace. */
namespace group {
// -----------------------------------------------------------------------------

class GroupInvocation
{
public:
    GroupInvocation ( ComputeShader* pShader );

    ComputeShader* shader() const;
    const vpp::Int& LocalId() const;
    const vpp::Int& LocalCount() const;
    int localCount() const;
};

// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
void Apply ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT, typename ValueT >
void Fill ( const ArrayT& arr, const ValueT& val, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
void Generate ( const ArrayT& arr, FunctorT&& fFunctor, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT, class FunctorT >
void Transform (
    const ArrayT& inArr, const ArrayT& outArr,
    FunctorT&& fFunctor, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT >
void Copy (
    const ArrayT& inArr, const ArrayT& outArr, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< class SourceArrayT, typename ArrayT >
void Load (
    const SourceArrayT& inArr, const Int& sourceStartIndex, const ArrayT& outArr,
    const GroupInvocation& inv );

template< class SourceArrayT, typename ArrayT >
void Load (
    const SourceArrayT& inArr, bool bColumnMajor, const Int& sourceStartIndex,
    const ArrayT& outArr, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< class TargetArrayT, typename ArrayT >
void Store (
    const ArrayT& inArr, const TargetArrayT& outArr, const Int& destStartIndex, 
    const GroupInvocation& inv );

template< class TargetArrayT, typename ArrayT >
void Store (
    const ArrayT& inArr, const TargetArrayT& outArr, bool bColumnMajor, 
    const Int& destStartIndex, const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
ValueT Reduce (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
void InclusiveScan (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ValueT, typename VValueT, class FunctorT >
void ExclusiveScan (
    WArray< ValueT >& arr, FunctorT&& fFunctor,
    VValueT& temporaryVal, WArray< ValueT >& tmpArr,
    const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT, typename FunctorT >
void Sort (
    const ArrayT& arr, FunctorT&& fFunctor,
    const GroupInvocation& inv );

// -----------------------------------------------------------------------------

template< typename ArrayT, typename ValueT, typename FunctorT >
Int LowerBound (
    const ArrayT& arr, const ValueT& v, FunctorT&& fFunctor, 
    WArray< Int >& tmpArr, const GroupInvocation& inv );

template< typename ArrayT, typename ValueT, typename FunctorT >
Int UpperBound (
    const ArrayT& arr, const ValueT& v, FunctorT&& fFunctor, 
    WArray< Int >& tmpArr, const GroupInvocation& inv );

// -----------------------------------------------------------------------------
} // namespace group
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace ct
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
