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
    \brief Shader (GPU-side) data type for boolean values.

    Use this type inside shader code as a counterpart of CPU-side bool type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VBool. Beware that mutable variables
    can degrade performance on GPU, therefore Bool is preferable, unless
    you really want a mutable variable.
*/

class Bool
{
public:
    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a constant, or a parameter passed to
        shader specific for that shader. The constructor is called once when
        compiling the pipeline.
    */
    Bool ( bool value );

    /** \brief Standard logical or operator. */
    Bool operator|| ( const Bool& rhs ) const;

    /** \brief Standard logical and operator. */
    Bool operator&& ( const Bool& rhs ) const;

    /** \brief Standard logical not operator. */
    Bool operator!() const;

    /** \brief Standard comparison operator (true if equal). */
    Bool operator== ( const Bool& rhs ) const;

    /** \brief Standard comparison operator (true if not equal). */
    Bool operator!= ( const Bool& rhs ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of boolean type.

    Use this type inside shader code as a counterpart of CPU-side bool type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU,
    consider using Bool, unless you really want a mutable variable.
*/

class VBool
{
public:
    VBool();
    VBool ( const Bool& rvalue );

    const VBool& operator= ( const Bool& rhs );
    operator Bool() const;

    Bool operator|| ( const Bool& rhs ) const;
    Bool operator&& ( const Bool& rhs ) const;
    Bool operator!() const;

    Bool operator== ( const Bool& rhs ) const;
    Bool operator!= ( const Bool& rhs ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) void data type.

    Use this type inside shader code as a counterpart of CPU-side void type.

    This type has no values. It is use only ocassionally, e.g. in function
    definitions.
*/

class Void
{
public:
};

// -----------------------------------------------------------------------------
/*
template< typename ScalarT >
class TLValue : public KValue
{
public:
    TLValue();
    TLValue ( const TRValue< ScalarT >& rvalue );

    operator rvalue_type() const;
    const TLValue< ScalarT >& operator= ( const TRValue< ScalarT >& rhs );

    VPP_DEFINE_SCALAR_OPERATORS;
    VPP_DEFINE_MUTATING_SCALAR_OPERATORS;

    inline rvalue_type operator* ( const rvalue_type& rhs ) const
    {
        rvalue_type lhs = *this;
        return lhs * rhs;
    }

    template< typename RightT >
    inline auto operator* ( const RightT& rhs ) const
    {
        typedef typename detail::TGetRV< RightT >::type rhs_type;
        const rvalue_type rValue1 = *this;
        const rhs_type rValue2 = rhs;
        return rValue1 * rValue2;
    }

    rvalue_type operator++();
    rvalue_type operator++ ( int );
    rvalue_type operator--();
    rvalue_type operator-- ( int );
};
*/
// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 32-bit signed integer values.

    Use this type inside shader code as a counterpart of CPU-side int type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VInt. Beware that mutable variables
    can degrade performance on GPU, therefore Int is preferable, unless
    you really want a mutable variable (e.g. a loop counter).
*/

class Int
{
public:
    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a constant, or a parameter passed to
        shader specific for that shader. The constructor is called once when
        compiling the pipeline.
    */
    Int ( int value );

    /** \brief Standard addition operator. */
    Int operator+ ( const Int& rhs ) const;

    /** \brief Standard subtraction operator. */
    Int operator- ( const Int& rhs ) const;

    /** \brief Standard multiplication operator. */
    Int operator* ( const Int& rhs ) const;

    /** \brief Standard division operator. */
    Int operator/ ( const Int& rhs ) const;

    /** \brief Standard remainder operator. Result sign is taken from the first operand. */
    Int operator% ( const Int& rhs ) const;

    /** \brief Standard left shift operator. */
    Int operator<< ( const Int& rhs ) const;

    /** \brief Standard right shift operator. */
    Int operator>> ( const Int& rhs ) const;

    /** \brief Standard bitwise or operator. */
    Int operator| ( const Int& rhs ) const;

    /** \brief Standard bitwise xor operator. */
    Int operator^ ( const Int& rhs ) const;

    /** \brief Standard bitwise and operator. */
    Int operator& ( const Int& rhs ) const;

    /** \brief Standard sign reversal operator. */
    Int operator-() const;

    /** \brief Standard bitwise negation operator. */
    Int operator~() const;

    /** \brief Standard comparison operator (true if equal). */
    Bool operator== ( const Int& rhs ) const;

    /** \brief Standard comparison operator (true if not equal). */
    Bool operator!= ( const Int& rhs ) const;

    /** \brief Standard comparison operator (true if greater). */
    Bool operator> ( const Int& rhs ) const;

    /** \brief Standard comparison operator (true if greater or equal). */
    Bool operator>= ( const Int& rhs ) const;

    /** \brief Standard comparison operator (true if less). */
    Bool operator< ( const Int& rhs ) const;

    /** \brief Standard comparison operator (true if less or equal). */
    Bool operator<= ( const Int& rhs ) const;
};

// -----------------------------------------------------------------------------

/** 
    \brief Shader (GPU-side) data type for 32-bit unsigned integer values.

    Use this type inside shader code as a counterpart of CPU-side unsigned int type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VUInt. Beware that mutable variables
    can degrade performance on GPU, therefore UInt is preferable, unless
    you really want a mutable variable (e.g. a loop counter).
*/

class UInt
{
public:
    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a constant, or a parameter passed to
        shader specific for that shader. The constructor is called once when
        compiling the pipeline.
    */
    UInt ( unsigned int value );

    /** \brief Standard addition operator. */
    UInt operator+ ( const UInt& rhs ) const;

    /** \brief Standard subtraction operator. */
    UInt operator- ( const UInt& rhs ) const;

    /** \brief Standard multiplication operator. */
    UInt operator* ( const UInt& rhs ) const;

    /** \brief Standard division operator. */
    UInt operator/ ( const UInt& rhs ) const;

    /** \brief Standard remainder operator. */
    UInt operator% ( const UInt& rhs ) const;

    /** \brief Standard left shift operator. */
    UInt operator<< ( const UInt& rhs ) const;

    /** \brief Standard right shift operator. */
    UInt operator>> ( const UInt& rhs ) const;

    /** \brief Standard bitwise or operator. */
    UInt operator| ( const UInt& rhs ) const;

    /** \brief Standard bitwise xor operator. */
    UInt operator^ ( const UInt& rhs ) const;

    /** \brief Standard bitwise and operator. */
    UInt operator& ( const UInt& rhs ) const;

    /** \brief Standard sign reversal operator. */
    UInt operator-() const;

    /** \brief Standard bitwise negation operator. */
    UInt operator~() const;

    /** \brief Standard comparison operator (true if equal). */
    Bool operator== ( const UInt& rhs ) const;

    /** \brief Standard comparison operator (true if not equal). */
    Bool operator!= ( const UInt& rhs ) const;

    /** \brief Standard comparison operator (true if greater). */
    Bool operator> ( const UInt& rhs ) const;

    /** \brief Standard comparison operator (true if greater or equal). */
    Bool operator>= ( const UInt& rhs ) const;

    /** \brief Standard comparison operator (true if less). */
    Bool operator< ( const UInt& rhs ) const;

    /** \brief Standard comparison operator (true if less or equal). */
    Bool operator<= ( const UInt& rhs ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 32-bit floating point values.

    Use this type inside shader code as a counterpart of CPU-side float type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VFloat. Beware that mutable variables
    can degrade performance on GPU, therefore Float is preferable, unless
    you really want a mutable variable.
*/

class Float
{
public:
    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a constant, or a parameter passed to
        shader specific for that shader. The constructor is called once when
        compiling the pipeline.
    */
    Float ( float value );

    /** \brief Standard addition operator. */
    Float operator+ ( const Float& rhs ) const;

    /** \brief Standard subtraction operator. */
    Float operator- ( const Float& rhs ) const;

    /** \brief Standard multiplication operator. */
    Float operator* ( const Float& rhs ) const;

    /** \brief Standard division operator. */
    Float operator/ ( const Float& rhs ) const;

    /** \brief Standard remainder operator. Result sign is taken from the first operand. */
    Float operator% ( const Float& rhs ) const;

    /** \brief Standard sign reversal operator. */
    Float operator-() const;

    /** \brief Standard comparison operator (true if equal). */
    Bool operator== ( const Float& rhs ) const;

    /** \brief Standard comparison operator (true if not equal). */
    Bool operator!= ( const Float& rhs ) const;

    /** \brief Standard comparison operator (true if greater). */
    Bool operator> ( const Float& rhs ) const;

    /** \brief Standard comparison operator (true if greater or equal). */
    Bool operator>= ( const Float& rhs ) const;

    /** \brief Standard comparison operator (true if less). */
    Bool operator< ( const Float& rhs ) const;

    /** \brief Standard comparison operator (true if less or equal). */
    Bool operator<= ( const Float& rhs ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 64-bit floating point values.

    Use this type inside shader code as a counterpart of CPU-side double type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VDouble. Beware that mutable variables
    can degrade performance on GPU, therefore Double is preferable, unless
    you really want a mutable variable.
*/

class Double
{
public:
    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a constant, or a parameter passed to
        shader specific for that shader. The constructor is called once when
        compiling the pipeline.
    */
    Double ( double value );

    /** \brief Standard addition operator. */
    Double operator+ ( const Double& rhs ) const;

    /** \brief Standard subtraction operator. */
    Double operator- ( const Double& rhs ) const;

    /** \brief Standard multiplication operator. */
    Double operator* ( const Double& rhs ) const;

    /** \brief Standard division operator. */
    Double operator/ ( const Double& rhs ) const;

    /** \brief Standard remainder operator. Result sign is taken from the first operand. */
    Double operator% ( const Double& rhs ) const;

    /** \brief Standard sign reversal operator. */
    Double operator-() const;

    /** \brief Standard comparison operator (true if equal). */
    Bool operator== ( const Double& rhs ) const;

    /** \brief Standard comparison operator (true if not equal). */
    Bool operator!= ( const Double& rhs ) const;

    /** \brief Standard comparison operator (true if greater). */
    Bool operator> ( const Double& rhs ) const;

    /** \brief Standard comparison operator (true if greater or equal). */
    Bool operator>= ( const Double& rhs ) const;

    /** \brief Standard comparison operator (true if less). */
    Bool operator< ( const Double& rhs ) const;

    /** \brief Standard comparison operator (true if less or equal). */
    Bool operator<= ( const Double& rhs ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 32-bit signed integer type.

    Use this type inside shader code as a counterpart of CPU-side int type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Int, unless you really want a mutable variable.
*/

class VInt
{
    /** \brief Constructs uninitialized variable. */
    VInt();

    /** \brief Constructs initialized variable. */
    VInt ( const Int& rvalue );

    /** \brief Retrieves the value. */
    operator Int() const;

    /** \brief Assigns the value. */
    const VInt& operator= ( const Int& rhs );

    /** \brief Performs addition and assigns the result. */
    Int operator+= ( const Int& rhs );

    /** \brief Performs subtraction and assigns the result. */
    Int operator-= ( const Int& rhs );

    /** \brief Performs multiplication and assigns the result. */
    Int operator*= ( const Int& rhs );

    /** \brief Performs division and assigns the result. */
    Int operator/= ( const Int& rhs );

    /** \brief Computes remainder and assigns the result. */
    Int operator%= ( const Int& rhs );

    /** \brief Performs left shift and assigns the result. */
    Int operator<<= ( const Int& rhs );

    /** \brief Performs right shift and assigns the result. */
    Int operator>>= ( const Int& rhs );

    /** \brief Computes bitwise AND and assigns the result. */
    Int operator&= ( const Int& rhs );

    /** \brief Computes bitwise OR and assigns the result. */
    Int operator|= ( const Int& rhs );

    /** \brief Computes bitwise XOR and assigns the result. */
    Int operator^= ( const Int& rhs );

    /** \brief Performs preincrementation. */
    Int operator++();

    /** \brief Performs postincrementation. */
    Int operator++ ( int );

    /** \brief Performs predecrementation. */
    Int operator--();

    /** \brief Performs postdecrementation. */
    Int operator-- ( int );
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 32-bit unsigned integer type.

    Use this type inside shader code as a counterpart of CPU-side unsigned int type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using UInt, unless you really want a mutable variable.
*/

class VUInt
{
    /** \brief Constructs uninitialized variable. */
    VUInt();

    /** \brief Constructs initialized variable. */
    VUInt ( const UInt& rvalue );

    /** \brief Retrieves the value. */
    operator UInt() const;

    /** \brief Assigns the value. */
    const VUInt& operator= ( const UInt& rhs );

    /** \brief Performs addition and assigns the result. */
    UInt operator+= ( const UInt& rhs );

    /** \brief Performs subtraction and assigns the result. */
    UInt operator-= ( const UInt& rhs );

    /** \brief Performs multiplication and assigns the result. */
    UInt operator*= ( const UInt& rhs );

    /** \brief Performs division and assigns the result. */
    UInt operator/= ( const UInt& rhs );

    /** \brief Computes remainder and assigns the result. */
    UInt operator%= ( const UInt& rhs );

    /** \brief Performs left shift and assigns the result. */
    UInt operator<<= ( const UInt& rhs );

    /** \brief Performs right shift and assigns the result. */
    UInt operator>>= ( const UInt& rhs );

    /** \brief Computes bitwise AND and assigns the result. */
    UInt operator&= ( const UInt& rhs );

    /** \brief Computes bitwise OR and assigns the result. */
    UInt operator|= ( const UInt& rhs );

    /** \brief Computes bitwise XOR and assigns the result. */
    UInt operator^= ( const UInt& rhs );

    /** \brief Performs preincrementation. */
    UInt operator++();

    /** \brief Performs postincrementation. */
    UInt operator++ ( int );

    /** \brief Performs predecrementation. */
    UInt operator--();

    /** \brief Performs postdecrementation. */
    UInt operator-- ( int );
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 32-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side float type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Float, unless you really want a mutable variable.
*/

class VFloat
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 64-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side double type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Double, unless you really want a mutable variable.
*/

class VDouble
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for pointers to mutable variables.

    Use this type inside shader code as a counterpart of CPU-side pointer type.

    This is a r-value type. You do not construct the object directly, but rather
    use the \b & operator to take a pointer to specified variable of simple type,
    or array element.

    Pointers are used mainly for atomic operations. This allows implementation
    of many lock-free parallel algorithms.

    Pointers in the Vulkan compute model are abstract and cannot be subject to
    any arithmetic. A pointer variable always points to the same object and
    can be only dereferenced.

    Example of using:

    \code
        Shared(); VArray< UInt, 256 > workArea;

        // ...

        const UInt prevValue = ( & ( workArea [ index ] ) ).Increment();


    \endcode
*/

template< class ScalarT >
class Pointer
{
public:
    /**
        \brief Reads the target variable.
    */
    ScalarT Load() const;

    /**
        \brief Stores specified value to the target variable.
    */
    void Store ( const ScalarT& value ) const;

    /**
        \brief Atomically exchanges specified value with the target variable. Returns
        previous value.
    */
    ScalarT Exchange ( const ScalarT& value ) const;

    /**
        \brief Atomically exchanges specified value with the target variable,
        but only if current value is equal to the oldValue parameter. Returns
        previous value.
    */
    ScalarT CompareExchange (
        const ScalarT& newValue,
        const ScalarT& oldValue ) const;

    /**
        \brief Atomically exchanges specified value with the target variable,
        but only if current value is equal to the oldValue parameter. Returns
        previous value.
    */
    ScalarT CompareExchangeWeak (
        const ScalarT& newValue,
        const ScalarT& oldValue ) const;

    /**
        \brief Atomically increments the target variable.
    */
    ScalarT Increment() const;

    /**
        \brief Atomically decrements the target variable.
    */
    ScalarT Decrement() const;

    /**
        \brief Atomically adds specified value to the target variable. Returns
        previous value.
    */
    ScalarT Add ( const ScalarT& value ) const;

    /**
        \brief Atomically subtracts specified value from the target variable. Returns
        previous value.
    */
    ScalarT Sub ( const ScalarT& value ) const;

    /**
        \brief Atomically computes minimum of specified value and the target variable
        and stores it to the target variable. Returns previous value.
    */
    ScalarT Min ( const ScalarT& value ) const;

    /**
        \brief Atomically computes maximum of specified value and the target variable
        and stores it to the target variable. Returns previous value.
    */
    ScalarT Max ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise AND of specified value and the target variable
        and stores it to the target variable. Returns previous value.
    */
    ScalarT And ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise OR of specified value and the target variable
        and stores it to the target variable. Returns previous value.
    */
    ScalarT Or ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise XOR of specified value and the target variable
        and stores it to the target variable. Returns previous value.
    */
    ScalarT Xor ( const ScalarT& value ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
