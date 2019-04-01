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
    \brief Shader (GPU-side) data type for 4x4 32-bit float matrices.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant array), or an expression computed on GPU
    side. The value can not be changed.

    Constant initialization can be performed using initializer list syntax, i.e.:

    \code
        float s = 1.0f;

        Mat4 u = Mat4 {
               s, 0.0f, 0.0f, 0.0f,
            0.0f,    s, 0.0f, 0.0f,
            0.0f, 0.0f,    s, 0.0f,
            0.0f, 0.0f, 0.0f,    s
        };
    \endcode

    Note that from the GPU point of view, CPU-side variables are constants
    and can be used in initializers.

    For mutable variable type, see VMat4. Beware that mutable variables
    can degrade performance on GPU, therefore Mat4 is preferable, unless
    you really want a mutable variable.
*/

class Mat4
{
public:
    /**
        \brief Matrix initialization (by constants) with curly braces syntax.
    */

    Mat4 ( const std::initializer_list< float >& init );

    /**
        \brief Matrix initialization (by vector expressions) with curly braces syntax.
    */
    Mat4 ( const std::initializer_list< Vec4 >& init );

    /**
        \brief Matrix initialization (by scalar expressions) with curly braces syntax.
    */
    Mat4 ( const std::initializer_list< Float >& init );

    /** \brief Addition operation on matrix components. */
    Mat4 operator+ ( const Mat4& rhs ) const;

    /** \brief Subtraction operation on matrix components. */
    Mat4 operator- ( const Mat4& rhs ) const;

    /** \brief Matrix multiplication by 2-column matrix. */
    Mat2x4 operator* ( const Mat2x4& rhs ) const;

    /** \brief Matrix multiplication by 3-column matrix. */
    Mat3x4 operator* ( const Mat3x4& rhs ) const;

    /** \brief Matrix multiplication by 4-column matrix. */
    Mat4 operator* ( const Mat4& rhs ) const;

    /** \brief Matrix multiplication by vector. */
    Vec4 operator* ( const Vec4& rhs ) const;

    /** \brief Matrix multiplication by scalar. */
    Mat4 operator* ( const Float& rhs ) const;

    /** \brief Division operation on matrix components. */
    Mat4 operator/ ( const Mat4& rhs ) const;

    /** \brief Remainder operation on matrix components. */
    Mat4 operator% ( const Mat4& rhs ) const;

    /** \brief Negation operation on matrix components. */
    Mat4 operator-() const;

    /** \brief Retrieves a column vector from matrix variable. */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2x2 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat2 {};

/** 
    \brief Shader (GPU-side) data type for 3x3 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat3 {};

/** 
    \brief Shader (GPU-side) data type for 3 columns, 2 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat3x2 {};

/** 
    \brief Shader (GPU-side) data type for 4 columns, 2 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat4x2 {};

/** 
    \brief Shader (GPU-side) data type for 2 columns, 3 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat2x3 {};

/** 
    \brief Shader (GPU-side) data type for 4 columns, 3 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat4x3 {};

/** 
    \brief Shader (GPU-side) data type for 2 columns, 4 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat2x4 {};

/** 
    \brief Shader (GPU-side) data type for 3 columns, 4 rows 32-bit float matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class Mat3x4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 4x4 32-bit integer matrices.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant array), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VIMat4. Beware that mutable variables
    can degrade performance on GPU, therefore IMat4 is preferable, unless
    you really want a mutable variable.
*/

class IMat4
{
public:
    /**
        \brief Matrix initialization (by constants) with curly braces syntax.
    */
    IMat4 ( const std::initializer_list< int >& init );

    /**
        \brief Matrix initialization (by vector expressions) with curly braces syntax.
    */
    IMat4 ( const std::initializer_list< IVec4 >& init );

    /**
        \brief Matrix initialization (by scalar expressions) with curly braces syntax.
    */
    IMat4 ( const std::initializer_list< Int >& init );

    /** \brief Addition operation on matrix components. */
    IMat4 operator+ ( const IMat4& rhs ) const;

    /** \brief Subtraction operation on matrix components. */
    IMat4 operator- ( const IMat4& rhs ) const;

    /** \brief Matrix multiplication by 2-column matrix. */
    IMat2x4 operator* ( const IMat2x4& rhs ) const;

    /** \brief Matrix multiplication by 3-column matrix. */
    IMat3x4 operator* ( const IMat3x4& rhs ) const;

    /** \brief Matrix multiplication by 4-column matrix. */
    IMat4 operator* ( const IMat4& rhs ) const;

    /** \brief Matrix multiplication by vector. */
    IVec4 operator* ( const IVec4& rhs ) const;

    /** \brief Matrix multiplication by scalar. */
    IMat4 operator* ( const Int& rhs ) const;

    /** \brief Division operation on matrix components. */
    IMat4 operator/ ( const IMat4& rhs ) const;

    /** \brief Remainder operation on matrix components. */
    IMat4 operator% ( const IMat4& rhs ) const;

    /** \brief Shift to left operation on matrix components. */
    IMat4 operator<< ( const IMat4& rhs ) const;

    /** \brief Shift to right operation on matrix components. */
    IMat4 operator>> ( const IMat4& rhs ) const;

    /** \brief Bitwise AND operation on matrix components. */
    IMat4 operator& ( const IMat4& rhs ) const;

    /** \brief Bitwise OR operation on matrix components. */
    IMat4 operator| ( const IMat4& rhs ) const;

    /** \brief Bitwise XOR operation on matrix components. */
    IMat4 operator^ ( const IMat4& rhs ) const;

    /** \brief Arithmetic negation operation on matrix components. */
    IMat4 operator-() const;

    /** \brief Bitwise negation operation on matrix components. */
    IMat4 operator~() const;

    /** \brief Retrieves a column vector from matrix variable. */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;
};

// -----------------------------------------------------------------------------

/** 
    \brief Shader (GPU-side) data type for 2x2 32-bit integer matrices.

    This class is identical to Mat4 class in all aspects except the number
    of components.
*/

class IMat2 {};

/** 
    \brief Shader (GPU-side) data type for 3x3 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat3 {};

/** 
    \brief Shader (GPU-side) data type for 3 columns, 2 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat3x2 {};

/** 
    \brief Shader (GPU-side) data type for 4 columns, 2 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat4x2 {};

/** 
    \brief Shader (GPU-side) data type for 2 columns, 3 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat2x3 {};

/** 
    \brief Shader (GPU-side) data type for 4 columns, 3 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat4x3 {};

/** 
    \brief Shader (GPU-side) data type for 2 columns, 4 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat2x4 {};

/** 
    \brief Shader (GPU-side) data type for 3 columns, 4 rows 32-bit integer matrices.

    This class is identical to IMat4 class in all aspects except the number
    of components.
*/

class IMat3x4 {};

// -----------------------------------------------------------------------------
/**
    \brief Shader (GPU-side) data type for mutable variables of
    4x4 32-bit floating point matrix type.
    
    This is a l-value type. It can be initialized and changed any time.

    Beware that mutable variables can degrade performance on GPU, therefore
    Mat4 is preferable, unless you really want a mutable variable.

    The lifetime of all local mutable variables (including VMat4)
    spans from the declaration to the end of current C++ block. This is
    the same as for ordinary C++ variable. However, each mutable variable
    allocates a variable slot (GPU register) which exists for entire time
    of shader execution (or shader-level function execution). VPP tries
    to optimize variable slot usage by reusing slots that were freed because
    their variables went out of scope. For that reuse to take place, the
    type of new variable must be identical to the type of some free slot.

    Therefore you can safely create a lot of mutable variables as long as they
    are in separate C++ blocks and have the same type.

    The VMat4 type has also a workgroup-scoped counterpart called WMat4.
    Caution: the reusing semantics does not apply to workgroup-scoped variables
    (they are permanent).
*/

class VMat4
{
public:
    /** \brief Constructs uninitialized matrix variable. */
    VMat4();

    /** \brief Constructs matrix variable and initializes it with given value. */
    VMat4 ( const Mat4& rhs );

    /** \brief Constructs matrix variable and initializes it with given value. */
    VMat4 ( const VMat4& rhs );

    /**
        \brief Assigns new value to matrix variable.
    */
    const VMat4& operator= ( const Mat4& rhs );

    /**
        \brief Retrieves the r-value from matrix variable.
    */
    operator Mat4() const;

    /** \brief Retrieves a column vector from matrix variable. */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2x2 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat2 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3x3 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat3 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3 columns, 2 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat3x2 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    4 columns, 2 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat4x2 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2 columns, 3 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat2x3 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    4 columns, 3 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat4x3 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2 columns, 4 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat2x4 {};

/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3 columns, 4 rows 32-bit float matrix type.

    This class is identical to VMat4 class in all aspects except the number
    of components.
*/

class VMat3x4 {};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
