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

    The lifetime of all local mutable variables (including VBool)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VBool type has also a workgroup-scoped counterpart called WBool.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
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
    \brief Shader (GPU-side) data type for workgroup variables of boolean type.

    Use this type inside shader code as a counterpart of CPU-side bool type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WBool type has also a local counterpart called VBool. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WBool
{
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
};

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
    /** \brief Construct a zero r-value.
    */
    Int();

    /** \brief Construct a r-value from specified C++ value.

        The source value can be either a C++ constant, or a variable (e.g. a parameter
        passed to shader specific for that shader). The constructor is called once when
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

    /** \brief Standard remainder operator.
    
        This operator generates a remainder computation instruction.

        Caution: the behavior of integer remainder operation for negative operands
        on real hardware seems to be incorrect on certain GPUs (problem detected
        on NVIDIA GTX 960). There is also inconsistency of specifications. The
        SPIR-V specification requests proper remainder calculation for negative
        operands. It states explicitly:

        Remainder: When dividing a by b, a remainder r is defined to be a value
        that satisfies r + q × b = a where q is a whole number and |r| < |b|.

        However, GLSL specification states that remainder is undefined if any
        operand is negative.

        The actual hardware (or at least some of it) follows the GLSL path,
        not SPIR-V. Therefore it is recommended to avoid supplying negative operands
        to this operator. 
        
        This issue can be considered a bug in Vulkan implementations. VPP does
        not cause this (generated instruction has been verified to be correct), so
        please do not report this as a bug in VPP.
    */
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
    /** \brief Construct a zero r-value.
    */
    UInt();

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
    /** \brief Construct a zero r-value.
    */
    Float();

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
    /** \brief Construct a zero r-value.
    */
    Double();

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
    \brief Shader (GPU-side) data type for 64-bit signed integer values.

    Use this type inside shader code as a counterpart of CPU-side std::int64_t type.

    This class is identical to Int in every aspect except for number of bits.
*/

class Int64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 64-bit unsigned integer values.

    Use this type inside shader code as a counterpart of CPU-side std::uint64_t type.

    This class is identical to UInt in every aspect except for number of bits.
*/

class UInt64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 32-bit signed integer type.

    Use this type inside shader code as a counterpart of CPU-side int type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Int, unless you really want a mutable variable.

    The lifetime of all local mutable variables (including VInt)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VInt type has also a workgroup-scoped counterpart called WInt.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
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

    The lifetime of all local mutable variables (including VUInt)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VUInt type has also a workgroup-scoped counterpart called WUInt.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
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
    \brief Shader (GPU-side) data type for workgroup variables of 32-bit signed integer type.

    Use this type inside shader code as a counterpart of CPU-side int type.

    This is a l-value type. You can assign to it at will.

    These values are shared between threads within single workgroup. Workgroups are also called
    thread groups in some proprietary APIs. Usually a single workgroup maps to so-called
    \b warp (32 threads) or \b wavefront (64 threads) on the GPU - depending on particular
    GPU vendor. It is not necessary though, as the workgroup size is configurable.

    Shared variables consume space inside the shared memory block. On current devices, it
    is typically 32 to 48 kB. No general purpose registers are allocated. This type of variable
    usually does not impose performance penalties, as long as all variables fit inside the block.

    The lifetime of all shared variables spans from the declaration to the end
    of current shader execution. They are like \c static variables in C++ on that matter.
    This is different from ordinary mutable variables.

    Shared variables are also useful as base for atomic variables. In VPP, atomic operations
    are performed by taking a pointer from the location (a shared variable, uniform buffer location
    or image texel) and executing the operation on a pointer. For that end, shared variables
    have additional <tt>operator &</tt> providing the pointer. See Pointer class for more information.

    The WInt type has also a local counterpart called VInt. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WInt
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for workgroup variables of 32-bit unsigned integer type.

    Use this type inside shader code as a counterpart of CPU-side unsigned int type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WUInt type has also a local counterpart called VUInt. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WUInt
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 64-bit signed integer type.

    This class is identical to VInt in every aspect except for number of bits.
*/

class VInt64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 64-bit unsigned integer type.

    This class is identical to VUInt in every aspect except for number of bits.
*/

class VUInt64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for workgroup variables of 64-bit unsigned integer type.

    Use this type inside shader code as a counterpart of CPU-side std::int64_t type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WInt64 type has also a local counterpart called VInt64. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WInt64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for workgroup variables of 64-bit unsigned integer type.

    Use this type inside shader code as a counterpart of CPU-side std::uint64_t type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WUInt64 type has also a local counterpart called VUInt64. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WUInt64
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 32-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side float type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Float, unless you really want a mutable variable.

    The lifetime of all local mutable variables (including VFloat)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VFloat type has also a workgroup-scoped counterpart called WFloat.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
*/

class VFloat
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for workgroup variables of 32-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side \c float type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WFloat type has also a local counterpart called VFloat. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WFloat
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of 64-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side double type.

    This is a l-value type. You can assign to it at will.

    Beware that mutable variables can degrade performance on GPU.
    Consider using Double, unless you really want a mutable variable.

    The lifetime of all local mutable variables (including VDouble)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VDouble type has also a workgroup-scoped counterpart called WDouble.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
*/

class VDouble
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for workgroup variables of 64-bit floating point type.

    Use this type inside shader code as a counterpart of CPU-side \c double type.

    For general description of workgroup variables, refer to WInt and WArray classes documentation.

    The WDouble type has also a local counterpart called VDouble. Apart from details mentioned above,
    the interface of both classes are the same.
*/

class WDouble
{
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for pointers to workgroup and global scoped variables.

    Use this type inside shader code as a counterpart of CPU-side pointer type.

    This is a r-value type. You do not construct the object directly, but rather
    use the \b & operator to take a pointer to specified variable of simple type
    or array element.

    The \b & operator can be applied to workgroup-scoped variable or element of
    array (with arbitrary number of dimensions). The type pointed to must be
    an integer or floating point scalar. Some operations are restricted to integers.

    There can be also pointers to scalar image elements, constructed by ioImage::GetPointer
    method instead of \b & operator.

    Pointers can also be taken for uniform buffer variables and array elements.

    Pointers are used mainly for atomic operations. This allows implementation
    of many lock-free parallel algorithms.

    Pointers in the Vulkan compute model are abstract and cannot be subject to
    any arithmetic. A pointer variable always points to the same object and
    can be only dereferenced.

    Example of using:

    \code
        WArray< UInt > workArea ( 256 );

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

        Types allowed: Int, UInt, Float.
    */
    ScalarT Load() const;

    /**
        \brief Stores specified value to the target variable.

        Types allowed: Int, UInt, Float.
    */
    void Store ( const ScalarT& value ) const;

    /**
        \brief Atomically exchanges specified value with the target variable. Returns
        previous value.

        Types allowed: Int, UInt, Float. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Exchange ( const ScalarT& value ) const;

    /**
        \brief Atomically exchanges specified value with the target variable,
        but only if current value is equal to the oldValue parameter. Returns
        previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT CompareExchange (
        const ScalarT& newValue,
        const ScalarT& oldValue ) const;

    /**
        \brief Atomically increments the target variable.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Increment() const;

    /**
        \brief Atomically decrements the target variable.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Decrement() const;

    /**
        \brief Atomically adds specified value to the target variable. Returns
        previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Add ( const ScalarT& value ) const;

    /**
        \brief Atomically subtracts specified value from the target variable. Returns
        previous value.

        Restricted to Int or UInt type.
    */
    ScalarT Sub ( const ScalarT& value ) const;

    /**
        \brief Atomically computes minimum of specified value and the target variable
        and stores it to the target variable. Returns previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Min ( const ScalarT& value ) const;

    /**
        \brief Atomically computes maximum of specified value and the target variable
        and stores it to the target variable. Returns previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Max ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise AND of specified value and the target variable
        and stores it to the target variable. Returns previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT And ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise OR of specified value and the target variable
        and stores it to the target variable. Returns previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Or ( const ScalarT& value ) const;

    /**
        \brief Atomically computes bitwise XOR of specified value and the target variable
        and stores it to the target variable. Returns previous value.

        Restricted to Int or UInt type. Also available for Int64 and UInt64 if
        \c fShaderBufferInt64Atomics and/or \c fShaderSharedInt64Atomics
        extension is enabled by calling to DeviceFeatures::enableIfSupported().
    */
    ScalarT Xor ( const ScalarT& value ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
