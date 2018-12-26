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

#ifndef INC_VPPLANGFUNCTIONS_HPP
#define INC_VPPLANGFUNCTIONS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPLANGMATRIXTYPES_HPP
#include "vppLangMatrixTypes.hpp"
#endif

#ifndef INC_VPPSPIRVGLSLSTD450_HPP
#include "vppGLSLstd450.hpp"
#endif

// -----------------------------------------------------------------------------

// https://www.khronos.org/registry/spir-v/specs/1.0/GLSL.std.450.html

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace impl {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class ValueT >
VPP_INLINE ValueT callBuiltinFunction ( const ValueT& value )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 1 );
    args [ 0 ] = value.id();

    return ValueT ( KId (
        pTranslator->createBuiltinCall (
            ValueT::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class ValueT >
VPP_INLINE ValueT callBuiltinFunction2 ( const ValueT& value1, const ValueT& value2 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 2 );
    args [ 0 ] = value1.id();
    args [ 1 ] = value2.id();

    return ValueT ( KId (
        pTranslator->createBuiltinCall (
            ValueT::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class ValueT >
VPP_INLINE ValueT callBuiltinFunction3 (
    const ValueT& value1, const ValueT& value2, const ValueT& value3 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 3 );
    args [ 0 ] = value1.id();
    args [ 1 ] = value2.id();
    args [ 2 ] = value3.id();

    return ValueT ( KId (
        pTranslator->createBuiltinCall (
            ValueT::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class Value1T, class Value2T >
VPP_INLINE Value1T callBuiltinFunction1s ( const Value2T& value )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 1 );
    args [ 0 ] = value.id();

    return Value1T ( KId (
        pTranslator->createBuiltinCall (
            Value1T::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class ValueRT, class ValueT >
VPP_INLINE ValueRT callBuiltinFunction2r ( const ValueT& value1, const ValueT& value2 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 2 );
    args [ 0 ] = value1.id();
    args [ 1 ] = value2.id();

    return ValueRT ( KId (
        pTranslator->createBuiltinCall (
            ValueRT::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class Value1T, class Value2T >
VPP_INLINE Value1T callBuiltinFunction2s ( const Value1T& value1, const Value2T& value2 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 2 );
    args [ 0 ] = value1.id();
    args [ 1 ] = value2.id();

    return Value1T ( KId (
        pTranslator->createBuiltinCall (
            Value1T::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) ) );
}

// -----------------------------------------------------------------------------

template< GLSLstd450 FUNCTION, class Value1T, class Value2T >
VPP_INLINE std::pair< Value1T, Value2T > callBuiltinFunctionP ( const Value1T& value1 )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 2 );
    args [ 0 ] = value1.id();
    args [ 1 ] = pTranslator->registerLocalVariable ( Value2T::getType() );

    const KId resultId (
        pTranslator->createBuiltinCall (
            Value1T::getType(), pTranslator->getBuiltinFunctions(),
            FUNCTION, args ) );

    return std::make_pair (
        Value1T ( resultId ),
        Value2T ( KId ( pTranslator->createLoad ( args [ 1 ] ) ) ) );
}

// -----------------------------------------------------------------------------

template< spv::Op OP, class ReturnT, class ValueT >
VPP_INLINE ReturnT callNumericPredicate (
    const ValueT& value, spv::Capability cap = spv::CapabilityShader )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    if ( cap != spv::CapabilityShader )
        pTranslator->addCapability ( cap );

    return ReturnT ( KId ( pTranslator->createUnaryOp (
        OP, ReturnT::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

template< spv::Op OP, class ReturnT, class ValueT >
VPP_INLINE ReturnT callNumericPredicate2 (
    const ValueT& value1,
    const ValueT& value2,
    spv::Capability cap = spv::CapabilityShader )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    if ( cap != spv::CapabilityShader )
        pTranslator->addCapability ( cap );

    return ReturnT ( KId ( pTranslator->createBinOp (
        OP, ReturnT::getType(), value1.id(), value2.id() ) ) );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

VPP_INLINE Float Round ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Round >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Round ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Round >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Round ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Round >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Round ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Round >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float RoundEven ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450RoundEven >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > RoundEven ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450RoundEven >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double RoundEven ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450RoundEven >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > RoundEven ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450RoundEven >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Trunc ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Trunc >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Trunc ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Trunc >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Trunc ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Trunc >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Trunc ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Trunc >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Abs ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450FAbs >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Abs ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FAbs >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Abs ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450FAbs >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Abs ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FAbs >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int Abs ( const Int& value )
{
    return detail::callBuiltinFunction< GLSLstd450SAbs >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > Abs ( const TRVector< Int, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450SAbs >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Sign ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450FSign >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Sign ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FSign >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Sign ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450FSign >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Sign ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FSign >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int Sign ( const Int& value )
{
    return detail::callBuiltinFunction< GLSLstd450SSign >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > Sign ( const TRVector< Int, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450SSign >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Floor ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Floor >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Floor ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Floor >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Floor ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Floor >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Floor ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Floor >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Ceil ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Ceil >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Ceil ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Ceil >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Ceil ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Ceil >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Ceil ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Ceil >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Fract ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Fract >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Fract ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Fract >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Fract ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Fract >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Fract ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Fract >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Radians ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Radians >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Radians ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Radians >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Degrees ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Degrees >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Degrees ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Degrees >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Sin ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sin >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Sin ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sin >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Cos ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Cos >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Cos ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Cos >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Tan ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Tan >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Tan ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Tan >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Asin ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Asin >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Asin ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Asin >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Acos ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Acos >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Acos ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Acos >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Atan ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Atan >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Atan ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Atan >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Sinh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sinh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Sinh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sinh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Cosh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Cosh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Cosh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Cosh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Tanh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Tanh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Tanh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Tanh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Asinh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Asinh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Asinh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Asinh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Acosh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Acosh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Acosh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Acosh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Atanh ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Atanh >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Atanh ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Atanh >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Atan2 ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Atan2 >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Atan2 (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Atan2 >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Pow ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Pow >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Pow (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Pow >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Exp ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Exp >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Exp ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Exp >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Log ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Log >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Log ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Log >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Exp2 ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Exp2 >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Exp2 ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Exp2 >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Log2 ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Log2 >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Log2 ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Log2 >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Sqrt ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sqrt >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Sqrt ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sqrt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Sqrt ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sqrt >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Sqrt ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Sqrt >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float InverseSqrt ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450InverseSqrt >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > InverseSqrt ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450InverseSqrt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double InverseSqrt ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450InverseSqrt >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > InverseSqrt ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450InverseSqrt >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Float Determinant ( const TRMatrix< Float, SIZE, SIZE >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Determinant, Float >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Double Determinant ( const TRMatrix< Double, SIZE, SIZE >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Determinant, Double >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRMatrix< Float, SIZE, SIZE > MatrixInverse ( const TRMatrix< Float, SIZE, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450MatrixInverse >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRMatrix< Double, SIZE, SIZE > MatrixInverse ( const TRMatrix< Double, SIZE, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450MatrixInverse >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ScalarT, size_t COLS, size_t ROWS >
VPP_INLINE TRMatrix< ScalarT, ROWS, COLS > Transpose ( const TRMatrix< ScalarT, COLS, ROWS >& value )
{
    return TRMatrix< ScalarT, ROWS, COLS > ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpTranspose,
        TRMatrix< ScalarT, ROWS, COLS >::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Min ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Min (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Min ( const Double& value1, const Double& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Min (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int Min ( const Int& value1, const Int& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450SMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > Min (
    const TRVector< Int, SIZE >& value1,
    const TRVector< Int, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450SMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt Min ( const UInt& value1, const UInt& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450UMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > Min (
    const TRVector< UInt, SIZE >& value1,
    const TRVector< UInt, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450UMin >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Max ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Max (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Max ( const Double& value1, const Double& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Max (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450FMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int Max ( const Int& value1, const Int& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450SMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > Max (
    const TRVector< Int, SIZE >& value1,
    const TRVector< Int, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450SMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt Max ( const UInt& value1, const UInt& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450UMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > Max (
    const TRVector< UInt, SIZE >& value1,
    const TRVector< UInt, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450UMax >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Clamp ( const Float& value, const Float& minVal, const Float& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450FClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Clamp (
    const TRVector< Float, SIZE >& value,
    const TRVector< Float, SIZE >& minVal,
    const TRVector< Float, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450FClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Clamp ( const Double& value, const Double& minVal, const Double& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450FClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Clamp (
    const TRVector< Double, SIZE >& value,
    const TRVector< Double, SIZE >& minVal,
    const TRVector< Double, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450FClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int Clamp ( const Int& value, const Int& minVal, const Int& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450SClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > Clamp (
    const TRVector< Int, SIZE >& value,
    const TRVector< Int, SIZE >& minVal,
    const TRVector< Int, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450SClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt Clamp ( const UInt& value, const UInt& minVal, const UInt& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450UClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > Clamp (
    const TRVector< UInt, SIZE >& value,
    const TRVector< UInt, SIZE >& minVal,
    const TRVector< UInt, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450UClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Mix ( const Float& x, const Float& y, const Float& a )
{
    return detail::callBuiltinFunction3< GLSLstd450FMix >( x, y, a );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Mix (
    const TRVector< Float, SIZE >& x,
    const TRVector< Float, SIZE >& y,
    const TRVector< Float, SIZE >& a )
{
    return detail::callBuiltinFunction3< GLSLstd450FMix >( x, y, a );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Mix ( const Double& x, const Double& y, const Double& a )
{
    return detail::callBuiltinFunction3< GLSLstd450FMix >( x, y, a );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Mix (
    const TRVector< Double, SIZE >& x,
    const TRVector< Double, SIZE >& y,
    const TRVector< Double, SIZE >& a )
{
    return detail::callBuiltinFunction3< GLSLstd450FMix >( x, y, a );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Step ( const Float& edge, const Float& x )
{
    return detail::callBuiltinFunction2< GLSLstd450Step >( edge, x );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Step (
    const TRVector< Float, SIZE >& edge,
    const TRVector< Float, SIZE >& x )
{
    return detail::callBuiltinFunction2< GLSLstd450Step >( edge, x );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Step ( const Double& edge, const Double& x )
{
    return detail::callBuiltinFunction2< GLSLstd450Step >( edge, x );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Step (
    const TRVector< Double, SIZE >& edge,
    const TRVector< Double, SIZE >& x )
{
    return detail::callBuiltinFunction2< GLSLstd450Step >( edge, x );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float SmoothStep ( const Float& edge0, const Float& edge1, const Float& x )
{
    return detail::callBuiltinFunction3< GLSLstd450SmoothStep >( edge0, edge1, x );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > SmoothStep (
    const TRVector< Float, SIZE >& edge0,
    const TRVector< Float, SIZE >& edge1,
    const TRVector< Float, SIZE >& x )
{
    return detail::callBuiltinFunction3< GLSLstd450SmoothStep >( edge0, edge1, x );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double SmoothStep ( const Double& edge0, const Double& edge1, const Double& x )
{
    return detail::callBuiltinFunction3< GLSLstd450SmoothStep >( edge0, edge1, x );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > SmoothStep (
    const TRVector< Double, SIZE >& edge0,
    const TRVector< Double, SIZE >& edge1,
    const TRVector< Double, SIZE >& x )
{
    return detail::callBuiltinFunction3< GLSLstd450SmoothStep >( edge0, edge1, x );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Fma ( const Float& a, const Float& b, const Float& c )
{
    return detail::callBuiltinFunction3< GLSLstd450Fma >( a, b, c );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Fma (
    const TRVector< Float, SIZE >& a,
    const TRVector< Float, SIZE >& b,
    const TRVector< Float, SIZE >& c )
{
    return detail::callBuiltinFunction3< GLSLstd450Fma >( a, b, c );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Fma ( const Double& a, const Double& b, const Double& c )
{
    return detail::callBuiltinFunction3< GLSLstd450Fma >( a, b, c );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Fma (
    const TRVector< Double, SIZE >& a,
    const TRVector< Double, SIZE >& b,
    const TRVector< Double, SIZE >& c )
{
    return detail::callBuiltinFunction3< GLSLstd450Fma >( a, b, c );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Ldexp ( const Float& x, const Int& exp )
{
    return detail::callBuiltinFunction2s< GLSLstd450Ldexp >( x, exp );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Ldexp (
    const TRVector< Float, SIZE >& x,
    const TRVector< Int, SIZE >& exp )
{
    return detail::callBuiltinFunction2s< GLSLstd450Ldexp >( x, exp );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Ldexp ( const Double& x, const Int& exp )
{
    return detail::callBuiltinFunction2s< GLSLstd450Ldexp >( x, exp );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Ldexp (
    const TRVector< Double, SIZE >& x,
    const TRVector< Int, SIZE >& exp )
{
    return detail::callBuiltinFunction2s< GLSLstd450Ldexp >( x, exp );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE UInt PackSnorm4x8 ( const TRVector< Float, 4 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackSnorm4x8, UInt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt PackUnorm4x8 ( const TRVector< Float, 4 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackUnorm4x8, UInt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt PackSnorm2x16 ( const TRVector< Float, 2 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackSnorm2x16, UInt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt PackUnorm2x16 ( const TRVector< Float, 2 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackUnorm2x16, UInt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt PackHalf2x16 ( const TRVector< Float, 2 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackHalf2x16, UInt >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double PackDouble2x32 ( const TRVector< UInt, 2 >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450PackDouble2x32, Double >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 2 > UnpackSnorm2x16 ( const UInt& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackSnorm2x16, TRVector< Float, 2 > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 2 > UnpackUnorm2x16 ( const UInt& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackUnorm2x16, TRVector< Float, 2 > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 2 > UnpackHalf2x16 ( const UInt& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackHalf2x16, TRVector< Float, 2 > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 4 > UnpackSnorm4x8 ( const UInt& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackSnorm4x8, TRVector< Float, 4 > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 4 > UnpackUnorm4x8 ( const UInt& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackUnorm4x8, TRVector< Float, 4 > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< UInt, 2 > UnpackDouble2x32 ( const Double& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450UnpackDouble2x32, TRVector< UInt, 2 > >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Float Dot (
    const TRVector< Float, SIZE >& lhs,
    const TRVector< Float, SIZE >& rhs )
{
    return Float ( KId ( KShaderTranslator::get()->createBinOp (
        spv::OpDot, Float::getType(), lhs.id(), rhs.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Double Dot (
    const TRVector< Double, SIZE >& lhs,
    const TRVector< Double, SIZE >& rhs )
{
    return Double ( KId ( KShaderTranslator::get()->createBinOp (
        spv::OpDot, Double::getType(), lhs.id(), rhs.id() ) ) );
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t COLS, size_t ROWS >
VPP_INLINE TRMatrix< Float, COLS, ROWS > OuterProduct (
    const TRVector< Float, ROWS >& lhs,
    const TRVector< Float, COLS >& rhs )
{
    return TRMatrix< Float, COLS, ROWS > ( KId ( KShaderTranslator::get()->createBinOp (
        spv::OpOuterProduct,
        TRMatrix< Float, COLS, ROWS >::getType(), lhs.id(), rhs.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t COLS, size_t ROWS >
VPP_INLINE TRMatrix< Double, COLS, ROWS > OuterProduct (
    const TRVector< Double, ROWS >& lhs,
    const TRVector< Double, COLS >& rhs )
{
    return TRMatrix< Double, COLS, ROWS > ( KId ( KShaderTranslator::get()->createBinOp (
        spv::OpOuterProduct,
        TRMatrix< Double, COLS, ROWS >::getType(), lhs.id(), rhs.id() ) ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Length ( const Float& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Length, Float >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Float Length ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Length, Float >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Length ( const Double& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Length, Double >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Double Length ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction1s< GLSLstd450Length, Double >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Distance ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Distance >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Float Distance (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2r< GLSLstd450Distance, Float >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Distance ( const Double& value1, const Double& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Distance >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Double Distance (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callBuiltinFunction2r< GLSLstd450Distance, Double >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Float, 3 > Cross (
    const TRVector< Float, 3 >& value1,
    const TRVector< Float, 3 >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Cross >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE TRVector< Double, 3 > Cross (
    const TRVector< Double, 3 >& value1,
    const TRVector< Double, 3 >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450Cross >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Normalize ( const Float& value )
{
    return detail::callBuiltinFunction< GLSLstd450Normalize >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Normalize ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Normalize >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Normalize ( const Double& value )
{
    return detail::callBuiltinFunction< GLSLstd450Normalize >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Normalize ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450Normalize >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float FaceForward ( const Float& n, const Float& i, const Float& nref )
{
    return detail::callBuiltinFunction3< GLSLstd450FaceForward >( n, i, nref );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > FaceForward (
    const TRVector< Float, SIZE >& n,
    const TRVector< Float, SIZE >& i,
    const TRVector< Float, SIZE >& nref )
{
    return detail::callBuiltinFunction3< GLSLstd450FaceForward >( n, i, nref );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double FaceForward ( const Double& n, const Double& i, const Double& nref )
{
    return detail::callBuiltinFunction3< GLSLstd450FaceForward >( n, i, nref );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > FaceForward (
    const TRVector< Double, SIZE >& n,
    const TRVector< Double, SIZE >& i,
    const TRVector< Double, SIZE >& nref )
{
    return detail::callBuiltinFunction3< GLSLstd450FaceForward >( n, i, nref );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Reflect ( const Float& i, const Float& n )
{
    return detail::callBuiltinFunction2< GLSLstd450Reflect >( i, n );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Reflect (
    const TRVector< Float, SIZE >& i,
    const TRVector< Float, SIZE >& n )
{
    return detail::callBuiltinFunction2< GLSLstd450Reflect >( i, n );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Reflect ( const Double& i, const Double& n )
{
    return detail::callBuiltinFunction2< GLSLstd450Reflect >( i, n );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Reflect (
    const TRVector< Double, SIZE >& i,
    const TRVector< Double, SIZE >& n )
{
    return detail::callBuiltinFunction2< GLSLstd450Reflect >( i, n );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float Refract ( const Float& i, const Float& n, const Float& eta )
{
    return detail::callBuiltinFunction3< GLSLstd450Refract >( i, n, eta );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > Refract (
    const TRVector< Float, SIZE >& i,
    const TRVector< Float, SIZE >& n,
    const Float& eta )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 3 );
    args [ 0 ] = i.id();
    args [ 1 ] = n.id();
    args [ 2 ] = eta.id();

    return TRVector< Float, SIZE > ( KId (
        pTranslator->createBuiltinCall (
            TRVector< Float, SIZE >::getType(), pTranslator->getBuiltinFunctions(),
            GLSLstd450Refract, args ) ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double Refract ( const Double& i, const Double& n, const Float& eta )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 3 );
    args [ 0 ] = i.id();
    args [ 1 ] = n.id();
    args [ 2 ] = eta.id();

    return Double ( KId (
        pTranslator->createBuiltinCall (
            Double::getType(), pTranslator->getBuiltinFunctions(),
            GLSLstd450Refract, args ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > Refract (
    const TRVector< Double, SIZE >& i,
    const TRVector< Double, SIZE >& n,
    const Float& eta )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    std::vector< spv::Id > args ( 3 );
    args [ 0 ] = i.id();
    args [ 1 ] = n.id();
    args [ 2 ] = eta.id();

    return TRVector< Double, SIZE > ( KId (
        pTranslator->createBuiltinCall (
            TRVector< Double, SIZE >::getType(), pTranslator->getBuiltinFunctions(),
            GLSLstd450Refract, args ) ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Int FindLsb ( const Int& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindILsb >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > FindLsb ( const TRVector< Int, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindILsb >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt FindLsb ( const UInt& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindILsb >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > FindLsb ( const TRVector< UInt, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindILsb >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Int FindMsb ( const Int& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindSMsb >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > FindMsb ( const TRVector< Int, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindSMsb >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt FindMsb ( const UInt& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindUMsb >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > FindMsb ( const TRVector< UInt, SIZE >& value )
{
    return detail::callBuiltinFunction< GLSLstd450FindUMsb >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float NMin ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > NMin (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double NMin ( const Double& value1, const Double& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMin >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > NMin (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMin >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float NMax ( const Float& value1, const Float& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > NMax (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double NMax ( const Double& value1, const Double& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMax >( value1, value2 );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > NMax (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callBuiltinFunction2< GLSLstd450NMax >( value1, value2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Float NClamp ( const Float& value, const Float& minVal, const Float& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450NClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Float, SIZE > NClamp (
    const TRVector< Float, SIZE >& value,
    const TRVector< Float, SIZE >& minVal,
    const TRVector< Float, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450NClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

VPP_INLINE Double NClamp ( const Double& value, const Double& minVal, const Double& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450NClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Double, SIZE > NClamp (
    const TRVector< Double, SIZE >& value,
    const TRVector< Double, SIZE >& minVal,
    const TRVector< Double, SIZE >& maxVal )
{
    return detail::callBuiltinFunction3< GLSLstd450NClamp >( value, minVal, maxVal );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ScalarT >
VPP_INLINE TRValue< ScalarT > Mod (
    const TRValue< ScalarT >& lhs, const TRValue< ScalarT >& rhs )
{
    typedef TRValue< ScalarT > rvalue_type;

    static_assert (
        ! scalar_traits< ScalarT >::isBool,
        "Mod function does not support boolean types." );

    return rvalue_type ( KId ( KShaderTranslator::get()->createBinOp (
        scalar_traits< ScalarT >::opMod2,
        rvalue_type::getType(), lhs.id(), rhs.id() ) ) );
}

// -----------------------------------------------------------------------------

template< class BaseType, size_t SIZE >
VPP_INLINE TRVector< BaseType, SIZE > Mod (
    const TRVector< BaseType, SIZE >& lhs, const TRVector< BaseType, SIZE >& rhs )
{
    typedef BaseType base_type;
    typedef typename BaseType::scalar_type scalar_type;
    typedef TRVector< BaseType, SIZE > rvalue_type;

    static_assert (
        ! scalar_traits< scalar_type >::isBool,
        "Mod function does not support boolean types." );

    return rvalue_type ( KId ( KShaderTranslator::get()->createBinOp (
        scalar_traits< scalar_type >::opMod2,
        rvalue_type::getType(), lhs.id(), rhs.id() ) ) );
}

// -----------------------------------------------------------------------------

template< class BaseType, size_t COLS, size_t ROWS >
VPP_INLINE TRMatrix< BaseType, COLS, ROWS > Mod (
    const TRMatrix< BaseType, COLS, ROWS >& lhs,
    const TRMatrix< BaseType, COLS, ROWS >& rhs )
{
    typedef BaseType base_type;
    typedef typename BaseType::scalar_type scalar_type;

    static_assert (
        ! scalar_traits< scalar_type >::isBool,
        "Mod function does not support boolean types." );

    return lhs.componentWiseOperation (
        rhs, []( const TRVector< BaseType, ROWS >& v1, 
                 const TRVector< BaseType, ROWS >& v2 ) { return Mod ( v1, v2 ); } );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE std::pair< Float, Float > Modf ( const Float& value )
{
    return detail::callBuiltinFunctionP< GLSLstd450Modf, Float, Float >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE std::pair< TRVector< Float, SIZE >, TRVector< Float, SIZE > >
    Modf ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunctionP<
        GLSLstd450Modf, TRVector< Float, SIZE >, TRVector< Float, SIZE > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE std::pair< Double, Double > Modf ( const Double& value )
{
    return detail::callBuiltinFunctionP< GLSLstd450Modf, Double, Double >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE std::pair< TRVector< Double, SIZE >, TRVector< Double, SIZE > >
    Modf ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunctionP<
        GLSLstd450Modf, TRVector< Double, SIZE >, TRVector< Double, SIZE > >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE std::pair< Float, Int > Frexp ( const Float& value )
{
    return detail::callBuiltinFunctionP< GLSLstd450Frexp, Float, Int >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE std::pair< TRVector< Float, SIZE >, TRVector< Int, SIZE > >
    Frexp ( const TRVector< Float, SIZE >& value )
{
    return detail::callBuiltinFunctionP<
        GLSLstd450Frexp, TRVector< Float, SIZE >, TRVector< Int, SIZE > >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE std::pair< Double, Int > Frexp ( const Double& value )
{
    return detail::callBuiltinFunctionP< GLSLstd450Frexp, Double, Int >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE std::pair< TRVector< Double, SIZE >, TRVector< Int, SIZE > >
    Frexp ( const TRVector< Double, SIZE >& value )
{
    return detail::callBuiltinFunctionP<
        GLSLstd450Frexp, TRVector< Double, SIZE >, TRVector< Int, SIZE > >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsNaN ( const Float& value )
{
    return detail::callNumericPredicate< spv::OpIsNan, Bool, Float >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsNaN ( const Double& value )
{
    return detail::callNumericPredicate< spv::OpIsNan, Bool, Double >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsNaN ( const TRVector< Float, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsNan, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsNaN ( const TRVector< Double, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsNan, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >( value );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsInf ( const Float& value )
{
    return detail::callNumericPredicate< spv::OpIsInf, Bool, Float >( value );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsInf ( const Double& value )
{
    return detail::callNumericPredicate< spv::OpIsInf, Bool, Double >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsInf ( const TRVector< Float, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsInf, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsInf ( const TRVector< Double, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsInf, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >( value );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsFinite ( const Float& value )
{
    return detail::callNumericPredicate< spv::OpIsFinite, Bool, Float >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsFinite ( const Double& value )
{
    return detail::callNumericPredicate< spv::OpIsFinite, Bool, Double >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsFinite ( const TRVector< Float, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsFinite, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsFinite ( const TRVector< Double, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsFinite, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsNormal ( const Float& value )
{
    return detail::callNumericPredicate< spv::OpIsNormal, Bool, Float >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsNormal ( const Double& value )
{
    return detail::callNumericPredicate< spv::OpIsNormal, Bool, Double >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsNormal ( const TRVector< Float, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsNormal, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsNormal ( const TRVector< Double, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpIsNormal, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsSignBitSet ( const Float& value )
{
    return detail::callNumericPredicate< spv::OpSignBitSet, Bool, Float >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsSignBitSet ( const Double& value )
{
    return detail::callNumericPredicate< spv::OpSignBitSet, Bool, Double >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsSignBitSet ( const TRVector< Float, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpSignBitSet, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsSignBitSet ( const TRVector< Double, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpSignBitSet, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >( value, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Bool IsAnyComponentTrue ( const TRVector< Bool, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpAny, Bool, TRVector< Bool, SIZE > >( value );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE Bool IsAllComponentsTrue ( const TRVector< Bool, SIZE >& value )
{
    return detail::callNumericPredicate<
        spv::OpAll, Bool, TRVector< Bool, SIZE > >( value );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsLessOrGreater ( const Float& value1, const Float& value2 )
{
    return detail::callNumericPredicate2< spv::OpLessOrGreater, Bool, Float >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsLessOrGreater ( const Double& value1, const Double& value2 )
{
    return detail::callNumericPredicate2< spv::OpLessOrGreater, Bool, Double >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsLessOrGreater (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpLessOrGreater, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsLessOrGreater (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpLessOrGreater, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsOrdered ( const Float& value1, const Float& value2 )
{
    return detail::callNumericPredicate2< spv::OpOrdered, Bool, Float >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsOrdered ( const Double& value1, const Double& value2 )
{
    return detail::callNumericPredicate2< spv::OpOrdered, Bool, Double >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsOrdered (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpOrdered, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsOrdered (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpOrdered, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// Caution: requires Kernel capability. Currently do not use.
// -----------------------------------------------------------------------------

VPP_INLINE Bool IsUnordered ( const Float& value1, const Float& value2 )
{
    return detail::callNumericPredicate2< spv::OpUnordered, Bool, Float >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

VPP_INLINE Bool IsUnordered ( const Double& value1, const Double& value2 )
{
    return detail::callNumericPredicate2< spv::OpUnordered, Bool, Double >(
        value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsUnordered (
    const TRVector< Float, SIZE >& value1,
    const TRVector< Float, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpUnordered, TRVector< Bool, SIZE >, TRVector< Float, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Bool, SIZE > IsUnordered (
    const TRVector< Double, SIZE >& value1,
    const TRVector< Double, SIZE >& value2 )
{
    return detail::callNumericPredicate2<
        spv::OpUnordered, TRVector< Bool, SIZE >, TRVector< Double, SIZE > >(
            value1, value2, spv::CapabilityKernel );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Int BitCount ( const Int& value )
{
    return Int ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitCount, Int::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE Int BitCount ( const UInt& value )
{
    return Int ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitCount, Int::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > BitCount (
    const TRVector< Int, SIZE >& value )
{
    return TRVector< Int, SIZE > ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitCount, TRVector< Int, SIZE >::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > BitCount (
    const TRVector< UInt, SIZE >& value )
{
    return TRVector< Int, SIZE > ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitCount, TRVector< Int, SIZE >::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Int BitReverse ( const Int& value )
{
    return Int ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitReverse, Int::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE UInt BitReverse ( const UInt& value )
{
    return UInt ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitReverse, UInt::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< Int, SIZE > BitReverse (
    const TRVector< Int, SIZE >& value )
{
    return TRVector< Int, SIZE > ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitReverse, TRVector< Int, SIZE >::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------

template< size_t SIZE >
VPP_INLINE TRVector< UInt, SIZE > BitReverse (
    const TRVector< UInt, SIZE >& value )
{
    return TRVector< UInt, SIZE > ( KId ( KShaderTranslator::get()->createUnaryOp (
        spv::OpBitReverse, TRVector< UInt, SIZE >::getType(), value.id() ) ) );
}

// -----------------------------------------------------------------------------
} // namespace impl
// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Round ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Round ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto RoundEven ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::RoundEven ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Trunc ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Trunc ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Abs ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Abs ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Sign ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Sign ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Floor ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Floor ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Ceil ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Ceil ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Fract ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Fract ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Radians ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Radians ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Degrees ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Degrees ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Sin ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Sin ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Cos ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Cos ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Tan ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Tan ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Asin ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Asin ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Acos ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Acos ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Atan ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Atan ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Sinh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Sinh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Cosh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Cosh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Tanh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Tanh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Asinh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Asinh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Acosh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Acosh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Atanh ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Atanh ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Atan2 ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Atan2 ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Pow ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Pow ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Exp ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Exp ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Log ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Log ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Exp2 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Exp2 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Log2 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Log2 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Sqrt ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Sqrt ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto InverseSqrt ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::InverseSqrt ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Determinant ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Determinant ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto MatrixInverse ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::MatrixInverse ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Transpose ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Transpose ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Min ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Min ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Max ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Max ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto Clamp ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::Clamp ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto Mix ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::Mix ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Step ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Step ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto SmoothStep ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::SmoothStep ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto Fma ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::Fma ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Ldexp ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Ldexp ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackSnorm4x8 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackSnorm4x8 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackUnorm4x8 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackUnorm4x8 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackSnorm2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackSnorm2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackUnorm2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackUnorm2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackHalf2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackHalf2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto PackDouble2x32 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::PackDouble2x32 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackSnorm2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackSnorm2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackUnorm2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackUnorm2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackHalf2x16 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackHalf2x16 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackSnorm4x8 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackSnorm4x8 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackUnorm4x8 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackUnorm4x8 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto UnpackDouble2x32 ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::UnpackDouble2x32 ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Dot ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Dot ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto OuterProduct ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::OuterProduct ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Length ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Length ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Distance ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Distance ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Cross ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Cross ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Normalize ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Normalize ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto FaceForward ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::FaceForward ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Reflect ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Reflect ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto Refract ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::Refract ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto FindLsb ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::FindLsb ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto FindMsb ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::FindMsb ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto NMin ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::NMin ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto NMax ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::NMax ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T, class Arg3T >
VPP_INLINE auto NClamp ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    const typename detail::TGetRV< Arg3T >::type rArg3 = arg3;
    return impl::NClamp ( rArg1, rArg2, rArg3 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto Mod ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::Mod ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Modf ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Modf ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto Frexp ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::Frexp ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsNaN ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsNaN ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsInf ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsInf ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsFinite ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsFinite ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsNormal ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsNormal ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsSignBitSet ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsSignBitSet ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsAnyComponentTrue ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsAnyComponentTrue ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto IsAllComponentsTrue ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::IsAllComponentsTrue ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto IsLessOrGreater ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::IsLessOrGreater ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto IsOrdered ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::IsOrdered ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T, class Arg2T >
VPP_INLINE auto IsUnordered ( const Arg1T& arg1, const Arg2T& arg2 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    const typename detail::TGetRV< Arg2T >::type rArg2 = arg2;
    return impl::IsUnordered ( rArg1, rArg2 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto BitCount ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::BitCount ( rArg1 );
}

// -----------------------------------------------------------------------------

template< class Arg1T >
VPP_INLINE auto BitReverse ( const Arg1T& arg1 )
{
    const typename detail::TGetRV< Arg1T >::type rArg1 = arg1;
    return impl::BitReverse ( rArg1 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ValueT >
VPP_INLINE void Ignore ( const ValueT& value )
{
    const typename ValueT::rvalue_type rValue = value;
    ( void ) rValue;
}

// -----------------------------------------------------------------------------

template< class ValueT >
VPP_INLINE void UseImage ( const ValueT& value )
{
    const typename ValueT::rvalue_type rValue = value;
    ( void ) rValue;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGFUNCTIONS_HPP
