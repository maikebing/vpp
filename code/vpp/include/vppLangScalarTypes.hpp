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

#ifndef INC_VPPLANGSCALARTYPES_HPP
#define INC_VPPLANGSCALARTYPES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPFORMATS_HPP
#include "vppFormats.hpp"
#endif

#ifndef INC_VPPSHADERLANGUAGETRANSLATOR_HPP
#include "vppLangTranslator.hpp"
#endif

#ifndef INC_VPPEXCEPTIONS_HPP
#include "vppExceptions.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< typename ScalarT >
struct scalar_traits {};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< bool >
{
    static const size_t component_count = 1;
    static const bool isBool = true;
    static const bool isInteger = false;
    static const bool isSignedInt = false;
    static const bool isUnsignedInt = false;
    static const bool isFloat = false;
    static const bool is64bit = false;
    static const bool isFlatShaded = false;

    static VPP_INLINE KId makeConstant ( bool v )
    {
        return KId ( KShaderTranslator::get()->makeBoolConstant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeBoolType() );
    }
};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< int >
{
    static const size_t component_count = 1;
    static const bool isBool = false;
    static const bool isInteger = true;
    static const bool isSignedInt = true;
    static const bool isUnsignedInt = false;
    static const bool isFloat = false;
    static const bool is64bit = false;
    static const bool isFlatShaded = true;

    static VPP_INLINE KId makeConstant ( int v )
    {
        return KId ( KShaderTranslator::get()->makeIntConstant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeIntType ( 32 ) );
    }

    static const spv::Op opAdd = spv::OpIAdd;
    static const spv::Op opSub = spv::OpISub;
    static const spv::Op opMul = spv::OpIMul;
    static const spv::Op opDiv = spv::OpSDiv;
    static const spv::Op opMod = spv::OpSRem;
    static const spv::Op opMod2 = spv::OpSMod;
    static const spv::Op opShl = spv::OpShiftLeftLogical;
    static const spv::Op opShr = spv::OpShiftRightArithmetic;
    static const spv::Op opBitOr = spv::OpBitwiseOr;
    static const spv::Op opBitXor = spv::OpBitwiseXor;
    static const spv::Op opBitAnd = spv::OpBitwiseAnd;
    static const spv::Op opNeg = spv::OpSNegate;
    static const spv::Op opBitNot = spv::OpNot;
    static const spv::Op opEqual = spv::OpIEqual;
    static const spv::Op opNotEqual = spv::OpINotEqual;
    static const spv::Op opGtr = spv::OpSGreaterThan;
    static const spv::Op opGtrEq = spv::OpSGreaterThanEqual;
    static const spv::Op opLss = spv::OpSLessThan;
    static const spv::Op opLssEq = spv::OpSLessThanEqual;
};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< unsigned int >
{
    static const size_t component_count = 1;
    static const bool isBool = false;
    static const bool isInteger = true;
    static const bool isSignedInt = false;
    static const bool isUnsignedInt = true;
    static const bool isFloat = false;
    static const bool is64bit = false;
    static const bool isFlatShaded = true;

    static VPP_INLINE KId makeConstant ( unsigned int v )
    {
        return KId ( KShaderTranslator::get()->makeUintConstant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeUintType ( 32 ) );
    }

    static const spv::Op opAdd = spv::OpIAdd;
    static const spv::Op opSub = spv::OpISub;
    static const spv::Op opMul = spv::OpIMul;
    static const spv::Op opDiv = spv::OpUDiv;
    static const spv::Op opMod = spv::OpUMod;
    static const spv::Op opMod2 = spv::OpUMod;
    static const spv::Op opShl = spv::OpShiftLeftLogical;
    static const spv::Op opShr = spv::OpShiftRightLogical;
    static const spv::Op opBitOr = spv::OpBitwiseOr;
    static const spv::Op opBitXor = spv::OpBitwiseXor;
    static const spv::Op opBitAnd = spv::OpBitwiseAnd;
    static const spv::Op opBitNot = spv::OpNot;
    static const spv::Op opEqual = spv::OpIEqual;
    static const spv::Op opNotEqual = spv::OpINotEqual;
    static const spv::Op opGtr = spv::OpUGreaterThan;
    static const spv::Op opGtrEq = spv::OpUGreaterThanEqual;
    static const spv::Op opLss = spv::OpULessThan;
    static const spv::Op opLssEq = spv::OpULessThanEqual;
};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< float >
{
    static const size_t component_count = 1;
    static const bool isBool = false;
    static const bool isInteger = false;
    static const bool isSignedInt = false;
    static const bool isUnsignedInt = false;
    static const bool isFloat = true;
    static const bool is64bit = false;
    static const bool isFlatShaded = false;

    static VPP_INLINE KId makeConstant ( float v )
    {
        return KId ( KShaderTranslator::get()->makeFloatConstant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeFloatType ( 32 ) );
    }

    static const spv::Op opAdd = spv::OpFAdd;
    static const spv::Op opSub = spv::OpFSub;
    static const spv::Op opMul = spv::OpFMul;
    static const spv::Op opDiv = spv::OpFDiv;
    static const spv::Op opMod = spv::OpFRem;
    static const spv::Op opMod2 = spv::OpFMod;
    static const spv::Op opNeg = spv::OpFNegate;
    static const spv::Op opEqual = spv::OpFOrdEqual;
    static const spv::Op opNotEqual = spv::OpFUnordNotEqual;
    static const spv::Op opGtr = spv::OpFOrdGreaterThan;
    static const spv::Op opGtrEq = spv::OpFOrdGreaterThanEqual;
    static const spv::Op opLss = spv::OpFOrdLessThan;
    static const spv::Op opLssEq = spv::OpFOrdLessThanEqual;
};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< double >
{
    static const size_t component_count = 2;
    static const bool isBool = false;
    static const bool isInteger = false;
    static const bool isSignedInt = false;
    static const bool isUnsignedInt = false;
    static const bool isFloat = true;
    static const bool is64bit = true;
    static const bool isFlatShaded = true;

    static VPP_INLINE KId makeConstant ( double v )
    {
        return KId ( KShaderTranslator::get()->makeDoubleConstant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeFloatType ( 64 ) );
    }

    static const spv::Op opAdd = spv::OpFAdd;
    static const spv::Op opSub = spv::OpFSub;
    static const spv::Op opMul = spv::OpFMul;
    static const spv::Op opDiv = spv::OpFDiv;
    static const spv::Op opMod = spv::OpFRem;
    static const spv::Op opMod2 = spv::OpFMod;
    static const spv::Op opNeg = spv::OpFNegate;
    static const spv::Op opEqual = spv::OpFOrdEqual;
    static const spv::Op opNotEqual = spv::OpFUnordNotEqual;
    static const spv::Op opGtr = spv::OpFOrdGreaterThan;
    static const spv::Op opGtrEq = spv::OpFOrdGreaterThanEqual;
    static const spv::Op opLss = spv::OpFOrdLessThan;
    static const spv::Op opLssEq = spv::OpFOrdLessThanEqual;
};

// -----------------------------------------------------------------------------

template<>
struct scalar_traits< float16_t >
{
    static const size_t component_count = 1;
    static const bool isBool = false;
    static const bool isInteger = false;
    static const bool isSignedInt = false;
    static const bool isUnsignedInt = false;
    static const bool isFloat = true;
    static const bool is64bit = false;
    static const bool isFlatShaded = false;

    static VPP_INLINE KId makeConstant ( float v )
    {
        return KId ( KShaderTranslator::get()->makeFloat16Constant ( v ) );
    }

    static VPP_INLINE KId getTypeId()
    {
        return KId ( KShaderTranslator::get()->makeFloatType ( 16 ) );
    }

    static const spv::Op opAdd = spv::OpFAdd;
    static const spv::Op opSub = spv::OpFSub;
    static const spv::Op opMul = spv::OpFMul;
    static const spv::Op opDiv = spv::OpFDiv;
    static const spv::Op opMod = spv::OpFRem;
    static const spv::Op opMod2 = spv::OpFMod;
    static const spv::Op opNeg = spv::OpFNegate;
    static const spv::Op opEqual = spv::OpFOrdEqual;
    static const spv::Op opNotEqual = spv::OpFUnordNotEqual;
    static const spv::Op opGtr = spv::OpFOrdGreaterThan;
    static const spv::Op opGtrEq = spv::OpFOrdGreaterThanEqual;
    static const spv::Op opLss = spv::OpFOrdLessThan;
    static const spv::Op opLssEq = spv::OpFOrdLessThanEqual;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define VPP_DEFINE_SCALAR_OPERATOR1( OP ) \
VPP_INLINE rvalue_type operator OP () const \
{ rvalue_type rhs = *this; return OP rhs; } \

#define VPP_DEFINE_SCALAR_OPERATOR2( OP ) \
template< typename ArgT > \
VPP_INLINE rvalue_type operator OP ( const ArgT& s ) const \
{ rvalue_type lhs = *this; rvalue_type rhs = s; return lhs OP rhs; } \

#define VPP_DEFINE_SCALAR_OPERATORC( OP ) \
template< typename ArgT > \
VPP_INLINE comparison_type operator OP ( const ArgT& s ) const \
{ rvalue_type lhs = *this; rvalue_type rhs = s; return lhs OP rhs; } \

#define VPP_DEFINE_SCALAR_OPERATORS1 \
    VPP_DEFINE_SCALAR_OPERATOR1( - ) \
    VPP_DEFINE_SCALAR_OPERATOR1( ~ ) \

#define VPP_DEFINE_SCALAR_OPERATORS2 \
    VPP_DEFINE_SCALAR_OPERATOR2( + ) \
    VPP_DEFINE_SCALAR_OPERATOR2( - ) \
    VPP_DEFINE_SCALAR_OPERATOR2( / ) \
    VPP_DEFINE_SCALAR_OPERATOR2( % ) \
    VPP_DEFINE_SCALAR_OPERATOR2( << ) \
    VPP_DEFINE_SCALAR_OPERATOR2( >> ) \
    VPP_DEFINE_SCALAR_OPERATOR2( | ) \
    VPP_DEFINE_SCALAR_OPERATOR2( ^ ) \
    VPP_DEFINE_SCALAR_OPERATOR2( & ) \
    VPP_DEFINE_SCALAR_OPERATORC( == ) \
    VPP_DEFINE_SCALAR_OPERATORC( != ) \
    VPP_DEFINE_SCALAR_OPERATORC( < ) \
    VPP_DEFINE_SCALAR_OPERATORC( > ) \
    VPP_DEFINE_SCALAR_OPERATORC( <= ) \
    VPP_DEFINE_SCALAR_OPERATORC( >= ) \

// -----------------------------------------------------------------------------

#define VPP_DEFINE_LBASE_OPERATOR2( OP, ValueT, ScalarT ) \
VPP_INLINE ValueT operator OP ( ScalarT s, const ValueT& rhs ) \
{ const ValueT lhs = s; return lhs OP rhs; } \

#define VPP_DEFINE_LBASE_OPERATORS( ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( +, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( *, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( -, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( /, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( %, ValueT, ScalarT ) \

#define VPP_DEFINE_LBASE_INT_OPERATORS( ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( <<, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( >>, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( |, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( &, ValueT, ScalarT ) \
    VPP_DEFINE_LBASE_OPERATOR2( ^, ValueT, ScalarT ) \

#define VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( OPM, OP ) \
template< typename RightT > \
VPP_INLINE rvalue_type operator OPM ( const RightT& rhs ) \
{ rvalue_type lhs = *this; *this = lhs OP rhs; return *this; } \

#define VPP_DEFINE_MUTATING_SCALAR_OPERATORS \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( +=, + ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( -=, - ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( *=, * ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( /=, / ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( %=, % ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( <<=, << ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( >>=, >> ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( &=, & ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( |=, | ) \
    VPP_DEFINE_MUTATING_SCALAR_OPERATOR2( ^=, ^ ) \

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KValue
{
public:
    typedef KValue rvalue_type;
    typedef KValue scalar_type;

    static const size_t item_count = 0;

    VPP_INLINE KValue ( KId id ) :
        d_id ( id )
    {
    }

    VPP_INLINE KId id() const
    {
        return d_id;
    }

private:
    KId d_id;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Bool : public KValue
{
public:
    typedef bool scalar_type;
    typedef Bool rvalue_type;
    typedef Bool item_type;
    typedef Bool comparison_type;
    static const bool external_linkage = false;
    static const bool indexable = false;
    static const size_t component_count = 1;
    static const size_t location_count = 1;
    static const size_t item_count = 1;

    VPP_INLINE Bool ( KId id ) :
        KValue ( id )
    {
    }

    VPP_INLINE Bool ( bool value ) :
        KValue ( KId ( KShaderTranslator::get()->makeBoolConstant ( value ) ) )
    {
    }

    static VPP_INLINE KId getType()
    {
        return KId ( KShaderTranslator::get()->makeBoolType() );
    }
  
    VPP_INLINE Bool operator|| ( const Bool& rhs ) const
    {
        return KId ( KShaderTranslator::get()->createBinOp (
            spv::OpLogicalOr, getType(), id(), rhs.id() ) );
    }

    VPP_INLINE Bool operator&& ( const Bool& rhs ) const
    {
        return KId ( KShaderTranslator::get()->createBinOp (
            spv::OpLogicalAnd, getType(), id(), rhs.id() ) );
    }

    VPP_INLINE Bool operator!() const
    {
        return KId ( KShaderTranslator::get()->createUnaryOp (
            spv::OpLogicalNot, getType(), id() ) );
    }

    VPP_INLINE Bool operator== ( const Bool& rhs ) const
    {
        return KId ( KShaderTranslator::get()->createBinOp (
            spv::OpLogicalEqual, getType(), id(), rhs.id() ) );
    }

    VPP_INLINE Bool operator!= ( const Bool& rhs ) const
    {
        return KId ( KShaderTranslator::get()->createBinOp (
            spv::OpLogicalNotEqual, getType(), id(), rhs.id() ) );
    }
};

// -----------------------------------------------------------------------------

class VBool : public KValue
{
public:
    typedef Bool rvalue_type;

    VPP_INLINE VBool() :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType() ) )
    {
    }

    VPP_INLINE VBool ( const Bool& rvalue ) :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType() ) )
    {
        KShaderTranslator::get()->createStore ( rvalue.id(), id() );
    }

    VPP_INLINE const VBool& operator= ( const Bool& rhs )
    {
        KShaderTranslator::get()->createStore ( rhs.id(), id() );
        return *this;
    }

    VPP_INLINE operator rvalue_type() const
    {
        return rvalue_type ( KId ( KShaderTranslator::get()->createLoad ( id() ) ) );
    }

    static VPP_INLINE KId getType()
    {
        return rvalue_type::getType();
    }

    VPP_INLINE Bool operator|| ( const Bool& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs || rhs;
    }

    VPP_INLINE Bool operator&& ( const Bool& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs && rhs;
    }

    VPP_INLINE Bool operator!() const
    {
        rvalue_type rhs = *this;
        return ! rhs;
    }

    VPP_INLINE Bool operator== ( const Bool& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs == rhs;
    }

    VPP_INLINE Bool operator!= ( const Bool& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs != rhs;
    }
};

// -----------------------------------------------------------------------------

class Void : public KValue
{
public:
    typedef Void rvalue_type;
    static const bool indexable = false;

    VPP_INLINE Void ( KId id ) :
        KValue ( id )
    {
    }

    static VPP_INLINE KId getType()
    {
        return KId ( KShaderTranslator::get()->makeVoidType() );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class OperandType >
struct TOperators
{
    static VPP_INLINE OperandType makeBinaryOp (
        spv::Op op, const OperandType& op1, const OperandType& op2 )
    {
        return OperandType ( KId (
            KShaderTranslator::get()->createBinOp (
                op, OperandType::getType(), op1.id(), op2.id() ) ) );
    }

    static VPP_INLINE OperandType makeUnaryOp (
        spv::Op op, const OperandType& op1 )
    {
        return OperandType ( KId (
            KShaderTranslator::get()->createUnaryOp (
                op, OperandType::getType(), op1.id() ) ) );
    }

    static VPP_INLINE Bool makeRelationalOp (
        spv::Op op, const OperandType& op1, const OperandType& op2 )
    {
        return Bool ( KId (
            KShaderTranslator::get()->createBinOp (
                op, Bool::getType(), op1.id(), op2.id() ) ) );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class BaseType, size_t SIZE >
class TRVector;

template< class BaseType, size_t SIZE >
class TLVector;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ScalarT >
class TRValue : public KValue
{
public:
    typedef ScalarT scalar_type;
    typedef TRValue< ScalarT > rvalue_type;
    typedef TRValue< ScalarT > item_type;
    typedef Bool comparison_type;

    static const bool external_linkage = true;
    static const bool indexable = false;
    static const size_t component_count = scalar_traits< ScalarT >::component_count;
    static const size_t location_count = 1;
    static const size_t item_count = 1;

    VPP_INLINE TRValue ( KId id ) :
        KValue ( id )
    {
    }

    VPP_INLINE TRValue ( ScalarT value ) :
        KValue ( scalar_traits< ScalarT >::makeConstant ( value ) )
    {
    }

    VPP_INLINE TRValue ( const TRValue< ScalarT >& rhs ) :
        KValue ( rhs.id() )
    {
    }

    template< typename OtherScalarT >
    explicit TRValue ( const TRValue< OtherScalarT >& rhs );

    VPP_INLINE const TRValue< ScalarT > operator= ( const TRValue< ScalarT >& )
    {
        static_assert ( TFalse< ScalarT >::value, "R-value cannot be assigned" );
        return *this;
    }

    VPP_INLINE TRValue operator+ ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opAdd, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator- ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opSub, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator* ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opMul, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator/ ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opDiv, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator% ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opMod, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator<< ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opShl, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator>> ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opShr, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator| ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opBitOr, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator^ ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opBitXor, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator& ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeBinaryOp (
            scalar_traits< ScalarT >::opBitAnd, id(), rhs.id() );
    }

    VPP_INLINE TRValue operator-() const
    {
        return TOperators< TRValue >::makeUnaryOp (
            scalar_traits< ScalarT >::opNeg, id() );
    }

    VPP_INLINE TRValue operator~() const
    {
        return TOperators< TRValue >::makeUnaryOp (
            scalar_traits< ScalarT >::opBitNot, id() );
    }

    VPP_INLINE Bool operator== ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opEqual, id(), rhs.id() );
    }

    VPP_INLINE Bool operator!= ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opNotEqual, id(), rhs.id() );
    }

    VPP_INLINE Bool operator> ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opGtr, id(), rhs.id() );
    }

    VPP_INLINE Bool operator>= ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opGtrEq, id(), rhs.id() );
    }

    VPP_INLINE Bool operator< ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opLss, id(), rhs.id() );
    }

    VPP_INLINE Bool operator<= ( const TRValue& rhs ) const
    {
        return TOperators< TRValue >::makeRelationalOp (
            scalar_traits< ScalarT >::opLssEq, id(), rhs.id() );
    }

    VPP_DEFINE_SCALAR_OPERATORS2;

    VPP_INLINE TRValue operator* ( const ScalarT& s ) const
    {
        rvalue_type rhs = s;
        return *this * rhs;
    }

    static VPP_INLINE KId getType()
    {
        return KId ( scalar_traits< ScalarT >::getTypeId() );
    }
};

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< typename ArgT > struct TGetRV { typedef typename ArgT::rvalue_type type; };
template<> struct TGetRV< int > { typedef TRValue< int > type; };
template<> struct TGetRV< unsigned int > { typedef TRValue< unsigned int > type; };
template<> struct TGetRV< float > { typedef TRValue< float > type; };
template<> struct TGetRV< double > { typedef TRValue< double > type; };
template<> struct TGetRV< bool > { typedef Bool type; };

// -----------------------------------------------------------------------------

#define VPP_RVTYPE( SourceT ) typename ::vpp::detail::TGetRV< SourceT >::type

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class ScalarT >
class Pointer : public KValue, public KExceptionThrower
{
public:
    typedef ScalarT target_type;

    VPP_INLINE Pointer ( const KId& valueId, spv::StorageClass storageClass ) :
        KValue ( valueId ),
        d_storageClass ( storageClass )
    {
    }

    VPP_INLINE spv::Scope getScope() const
    {
        switch ( d_storageClass )
        {
            case spv::StorageClassWorkgroup:
                return spv::ScopeWorkgroup;

            case spv::StorageClassImage:
                return spv::ScopeDevice;

            default:
                raiseUsageError ( "Only images and shared variables can have atomic pointers" );
                return spv::ScopeDevice;
        }
    }

    static VPP_INLINE spv::MemorySemanticsMask getSemanticsMask()
    {
        return spv::MemorySemanticsAcquireReleaseMask;
    }

    VPP_INLINE target_type Load() const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );

        return KId ( pTranslator->createOp (
            spv::OpAtomicLoad, target_type::getType(), operands ) );
    }

    VPP_INLINE void Store ( const target_type& value ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 4 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 3 ] = value.id();

        pTranslator->createNoResultOp ( spv::OpAtomicStore, operands );
    }

    VPP_INLINE target_type Exchange ( const target_type& value ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 4 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 3 ] = value.id();

        return KId ( pTranslator->createOp (
            spv::OpAtomicExchange, target_type::getType(), operands ) );
    }

    VPP_INLINE target_type CompareExchange (
        const target_type& newValue,
        const target_type& oldValue ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 6 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 3 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 4 ] = newValue.id();
        operands [ 5 ] = oldValue.id();

        return KId ( pTranslator->createOp (
            spv::OpAtomicCompareExchange, target_type::getType(), operands ) );
    }

    VPP_INLINE target_type CompareExchangeWeak (
        const target_type& newValue,
        const target_type& oldValue ) const
    {
        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 6 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 3 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 4 ] = newValue.id();
        operands [ 5 ] = oldValue.id();

        return KId ( pTranslator->createOp (
            spv::OpAtomicCompareExchangeWeak, target_type::getType(), operands ) );
    }

    VPP_INLINE target_type Increment() const
    {
        typedef typename target_type::scalar_type native_type;

        static_assert (
            scalar_traits< native_type >::isInteger,
            "This operation requires integer type." );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );

        return KId ( pTranslator->createOp (
            spv::OpAtomicIIncrement, target_type::getType(), operands ) );
    }

    VPP_INLINE target_type Decrement() const
    {
        typedef typename target_type::scalar_type native_type;

        static_assert (
            scalar_traits< native_type >::isInteger,
            "This operation requires integer type." );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 3 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );

        return KId ( pTranslator->createOp (
            spv::OpAtomicIDecrement, target_type::getType(), operands ) );
    }

    VPP_INLINE target_type Add ( const target_type& value ) const
    {
        return opArithmetic ( value, spv::OpAtomicIAdd );
    }

    VPP_INLINE target_type Sub ( const target_type& value ) const
    {
        return opArithmetic ( value, spv::OpAtomicISub );
    }

    VPP_INLINE target_type Min ( const target_type& value ) const
    {
        typedef typename target_type::scalar_type native_type;

        return opArithmetic (
            value, ( scalar_traits< native_type >::isSignedInt ?
                spv::OpAtomicSMin : spv::OpAtomicUMin ) );
    }

    VPP_INLINE target_type Max ( const target_type& value ) const
    {
        typedef typename target_type::scalar_type native_type;

        return opArithmetic (
            value, ( scalar_traits< native_type >::isSignedInt ?
                spv::OpAtomicSMax : spv::OpAtomicUMax ) );
    }

    VPP_INLINE target_type And ( const target_type& value ) const
    {
        return opArithmetic ( value, spv::OpAtomicAnd );
    }

    VPP_INLINE target_type Or ( const target_type& value ) const
    {
        return opArithmetic ( value, spv::OpAtomicOr );
    }

    VPP_INLINE target_type Xor ( const target_type& value ) const
    {
        return opArithmetic ( value, spv::OpAtomicXor );
    }

private:
    VPP_INLINE target_type opArithmetic ( const target_type& value, spv::Op opCode ) const
    {
        typedef typename target_type::scalar_type native_type;

        static_assert (
            scalar_traits< native_type >::isInteger,
            "This operation requires integer type." );

        KShaderTranslator* pTranslator = KShaderTranslator::get();

        std::vector< spv::Id > operands ( 4 );
        operands [ 0 ] = id();
        operands [ 1 ] = pTranslator->getArrayIndex ( static_cast< int >( getScope() ) );
        operands [ 2 ] = pTranslator->getArrayIndex ( static_cast< int >( getSemanticsMask() ) );
        operands [ 3 ] = value.id();

        return KId ( pTranslator->createOp (
            opCode, target_type::getType(), operands ) );
    }

private:
    spv::StorageClass d_storageClass;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename ScalarT >
class TLValue : public KValue
{
public:
    typedef TRValue< ScalarT > rvalue_type;
    typedef Bool comparison_type;

    VPP_INLINE TLValue() :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType(), & d_storageClass ) )
    {
    }

    explicit VPP_INLINE TLValue ( KId id ) :
        KValue ( id )
    {
    }

    VPP_INLINE TLValue ( const TRValue< ScalarT >& rvalue ) :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType(), & d_storageClass ) )
    {
        KShaderTranslator::get()->createStore ( rvalue.id(), id() );
    }

    VPP_INLINE TLValue ( ScalarT value ) :
        TLValue ( TRValue< ScalarT >( value ) )
    {
    }

    VPP_INLINE TLValue ( const TLValue< ScalarT >& rhs ) :
        KValue ( KShaderTranslator::get()->registerLocalVariable ( getType(), & d_storageClass ) )
    {
        const rvalue_type rv = rhs;
        KShaderTranslator::get()->createStore ( rv.id(), id() );
    }

    VPP_INLINE operator rvalue_type() const
    {
        return rvalue_type ( KId ( KShaderTranslator::get()->createLoad ( id() ) ) );
    }

    template< typename RightT >
    VPP_INLINE const TLValue< ScalarT >& operator= ( const RightT& rhs )
    {
        const rvalue_type rv = rhs;
        KShaderTranslator::get()->createStore ( rv.id(), id() );
        return *this;
    }

    VPP_INLINE const TLValue< ScalarT >& operator= ( const TLValue< ScalarT >& rhs )
    {
        const rvalue_type rv = rhs;
        KShaderTranslator::get()->createStore ( rv.id(), id() );
        return *this;
    }

    VPP_DEFINE_SCALAR_OPERATORS1;
    VPP_DEFINE_SCALAR_OPERATORS2;
    VPP_DEFINE_MUTATING_SCALAR_OPERATORS;

    VPP_INLINE rvalue_type operator++()
    {
        rvalue_type prevValue = *this;
        rvalue_type nextValue = prevValue + rvalue_type ( 1 );
        *this = nextValue;
        return nextValue;
    }

    VPP_INLINE rvalue_type operator++ ( int )
    {
        rvalue_type prevValue = *this;
        rvalue_type nextValue = prevValue + rvalue_type ( 1 );
        *this = nextValue;
        return prevValue;
    }

    VPP_INLINE rvalue_type operator--()
    {
        rvalue_type prevValue = *this;
        rvalue_type nextValue = prevValue - rvalue_type ( 1 );
        *this = nextValue;
        return nextValue;
    }

    VPP_INLINE rvalue_type operator-- ( int )
    {
        rvalue_type prevValue = *this;
        rvalue_type nextValue = prevValue - rvalue_type ( 1 );
        *this = nextValue;
        return prevValue;
    }

    VPP_INLINE Pointer< rvalue_type > operator& () const
    {
        return Pointer< rvalue_type >( id(), d_storageClass );
    }

    static VPP_INLINE KId getType()
    {
        return rvalue_type::getType();
    }

    VPP_INLINE spv::StorageClass getStorageClass() const
    {
        return d_storageClass;
    }

private:
    spv::StorageClass d_storageClass;
};

// -----------------------------------------------------------------------------

template< typename ScalarT, typename RightT >
VPP_INLINE auto operator* ( const TLValue< ScalarT >& lhs, const RightT& rhs )
{
    const TRValue< ScalarT > lrv = lhs;
    const VPP_RVTYPE( RightT ) rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------

template< typename ScalarT, typename LeftT >
VPP_INLINE auto operator* ( const LeftT& lhs, const TLValue< ScalarT >& rhs )
{
    const VPP_RVTYPE( LeftT ) lrv = lhs;
    const TRValue< ScalarT > rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------

template< typename ScalarT, typename LeftT >
VPP_INLINE auto operator* ( const TLValue< ScalarT >& lhs, const TLValue< ScalarT >& rhs )
{
    const TRValue< ScalarT > lrv = lhs;
    const TRValue< ScalarT > rrv = rhs;
    return lrv * rrv;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef TRValue< int > Int;
typedef TLValue< int > VInt;

typedef TRValue< unsigned int > UInt;
typedef TLValue< unsigned int > VUInt;

typedef TRValue< float > Float;
typedef TLValue< float > VFloat;

typedef TRValue< double > Double;
typedef TLValue< double > VDouble;

typedef TRValue< float16_t > Half;
typedef TLValue< float16_t > VHalf;

// -----------------------------------------------------------------------------

VPP_DEFINE_LBASE_OPERATORS ( Int, int )
VPP_DEFINE_LBASE_OPERATORS ( UInt, unsigned int )
VPP_DEFINE_LBASE_INT_OPERATORS ( Int, int )
VPP_DEFINE_LBASE_INT_OPERATORS ( UInt, unsigned int )
VPP_DEFINE_LBASE_OPERATORS ( Float, float )
VPP_DEFINE_LBASE_OPERATORS ( Double, double )

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< int >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef int scalar_type;
    typedef TRValue< int > rvalue_type;
    typedef TLValue< int > lvalue_type;
    typedef int data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< unsigned int >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef unsigned int scalar_type;
    typedef TRValue< unsigned int > rvalue_type;
    typedef TLValue< unsigned int > lvalue_type;
    typedef unsigned int data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< float >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef float scalar_type;
    typedef TRValue< float > rvalue_type;
    typedef TLValue< float > lvalue_type;
    typedef float data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< double >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef double scalar_type;
    typedef TRValue< double > rvalue_type;
    typedef TLValue< double > lvalue_type;
    typedef double data_type;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPLANGSCALARTYPES_HPP
