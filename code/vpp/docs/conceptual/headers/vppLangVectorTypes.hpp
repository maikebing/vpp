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
    \brief Shader (GPU-side) data type for 4-element boolean vectors.

    Use this type inside shader code as a counterpart of CPU-side bool[4] type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    For mutable variable type, see VBVec4. Beware that mutable variables
    can degrade performance on GPU, therefore BVec4 is preferable, unless
    you really want a mutable variable.
*/

class BVec4
{
public:
    /**
        \brief Vector initialization with curly braces syntax.

        For example:

        \code
            BVec4 { true, false, false, false }
        \endcode
    */

    BVec4 ( const std::initializer_list< bool >& init );

    /**
        \brief Vector initialization to single constant value.
    */

    BVec4 ( bool init );

    /**
        \brief Vector initialization from another vector.
    */

    BVec4 ( const BVec4& rhs );

    /**
        \brief Vector initialization from GPU-side value.

        The value can be a scalar, or another vector.
    */

    template< class Arg1T >
    explicit BVec4 ( const Arg1T& arg1 );

    /**
        \brief Vector initialization from two GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T >
    BVec4 ( const Arg1T& arg1, const Arg2T& arg2 );

    /**
        \brief Vector initialization from three GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T >
    BVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

    /**
        \brief Vector initialization from four GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    BVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

    /** \brief Logical AND operation on vector components. */
    BVec4 operator&& ( const BVec4& rhs ) const;

    /** \brief Logical OR operation on vector components. */
    BVec4 operator|| ( const BVec4& rhs ) const;

    /** \brief Logical NOT operation on vector components. */
    BVec4 operator!() const;

    /** \brief Comparison (equal) operation on vector components. */
    BVec4 operator== ( const BVec4& rhs ) const;

    /** \brief Comparison (not equal) operation on vector components. */
    BVec4 operator!= ( const BVec4& rhs ) const;

    /** \brief Comparison (greater) operation on vector components. */
    BVec4 operator> ( const BVec4& rhs ) const;

    /** \brief Comparison (greater or equal) operation on vector components. */
    BVec4 operator>= ( const BVec4& rhs ) const;

    /** \brief Comparison (less) operation on vector components. */
    BVec4 operator< ( const BVec4& rhs ) const;

    /** \brief Comparison (less or equal) operation on vector components. */
    BVec4 operator<= ( const BVec4& rhs ) const;

    /** \brief Computes a permutation of vector components. */
    auto operator[]( ESwizzle sw ) const;

    /** \brief Number of elements in this vector. */
    static const size_t item_count = 4;
};

// -----------------------------------------------------------------------------
/**
    \brief Shader (GPU-side) data type for mutable variables of
    4-element boolean vector type.
    
    Use this type inside shader code as a counterpart of CPU-side bool[4] type,
    when it is required to change value of the vector.

    This is a l-value type. It can be initialized and changed any time.

    Beware that mutable variables can degrade performance on GPU, therefore
    BVec4 is preferable, unless you really want a mutable variable.
*/

class VBVec4
{
    /** \brief Constructs uninitialized vector variable. */
    VBVec4();

    /** \brief Constructs vector variable and initializes it with given value. */
    VBVec4 ( const BVec4& rhs );

    /** \brief Constructs vector variable and initializes it with given value. */
    VBVec4 ( const VBVec4& rhs );

    /** \brief Constructs vector variable and initializes it with given constant
        (curly braces syntax).
    */
    VBVec4 ( const std::initializer_list< bool >& initValue );

    /**
        \brief Constructs vector variable and initializes it from two GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T >
    VBVec4 ( const Arg1T& arg1, const Arg2T& arg2 );

    /**
        \brief Constructs vector variable and initializes it from three GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T >
    VBVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

    /**
        \brief Constructs vector variable and initializes it from four GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    VBVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

    /**
        \brief Assigns new value to vector variable.
    */
    const VBVec4& operator= ( const BVec4& rhs );

    /**
        \brief Assigns new value to vector variable.
    */
    const VBVec4& operator= ( const VBVec4& rhs );

    /**
        \brief Retrieves the r-value from vector variable.
    */
    operator BVec4() const;

    /** \brief Component selection operation. */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;

    /** \brief Number of elements in this vector. */
    static const size_t item_count = 4;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 3-element boolean vectors.

    This class is identical to BVec4 class in all aspects except the number
    of components.
*/

class BVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3-element boolean vector type.

    This class is identical to VBVec4 class in all aspects except the number
    of components.
*/

class VBVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2-element boolean vectors.

    This class is identical to BVec4 class in all aspects except the number
    of components.
*/

class BVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2-element boolean vector type.

    This class is identical to VBVec4 class in all aspects except the number
    of components.
*/

class VBVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 4-element 32-bit float vectors.

    Use this type inside shader code as a counterpart of CPU-side float[4] type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    Constant initialization can be performed using initializer list syntax, i.e.:

    \code
        float pi = 3.1415926f;
        Vec4 v = Vec4 { 1.0f * pi, 2.0f * pi, 3.0f * pi, 4.0f * pi };
    \endcode

    Note that from the GPU point of view, CPU-side variables are constants
    and can be used in initializers.

    Initialization with GPU-side values (variable or constant) is performed
    using the constructor syntax, e.g.:

    \code
        Float x, y, z;
        Vec3 coords ( x, y, z );
        Vec4 hcoords = Vec4 ( coords, Float ( 1.0f ) );
    \endcode

    For mutable variable type, see VVec4. Beware that mutable variables
    can degrade performance on GPU, therefore Vec4 is preferable, unless
    you really want a mutable variable.
*/

class Vec4
{
public:
    /**
        \brief Vector initialization with curly braces syntax.

        For example:

        \code
            Vec4 { 1.0f, 0.5f, 2.0f, 1.0f }
        \endcode
    */

    Vec4 ( const std::initializer_list< float >& init );

    /**
        \brief Vector initialization to single constant value.
    */

    Vec4 ( float init );

    /**
        \brief Vector initialization from another vector.
    */

    Vec4 ( const Vec4& rhs );

    /**
        \brief Vector initialization from GPU-side value.

        The value can be a scalar, or another vector.
    */

    template< class Arg1T >
    explicit Vec4 ( const Arg1T& arg1 );

    /**
        \brief Vector initialization from two GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T >
    Vec4 ( const Arg1T& arg1, const Arg2T& arg2 );

    /**
        \brief Vector initialization from three GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T, class Arg3T >
    Vec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

    /**
        \brief Vector initialization from four GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    Vec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

    /** \brief Addition operation on vector components. */
    Vec4 operator+ ( const Vec4& rhs ) const;

    /** \brief Subtraction operation on vector components. */
    Vec4 operator- ( const Vec4& rhs ) const;

    /** \brief Multiplication operation on vector components. */
    Vec4 operator* ( const Vec4& rhs ) const;

    /** \brief Division operation on vector components. */
    Vec4 operator/ ( const Vec4& rhs ) const;

    /** \brief Remainder operation on vector components. */
    Vec4 operator% ( const Vec4& rhs ) const;

    /** \brief Negation operation on vector components. */
    Vec4 operator-() const;

    /** \brief Comparison (equal) operation on vector components. */
    BVec4 operator== ( const Vec4& rhs ) const;

    /** \brief Comparison (not equal) operation on vector components. */
    BVec4 operator!= ( const Vec4& rhs ) const;

    /** \brief Comparison (greater) operation on vector components. */
    BVec4 operator> ( const Vec4& rhs ) const;

    /** \brief Comparison (greater or equal) operation on vector components. */
    BVec4 operator>= ( const Vec4& rhs ) const;

    /** \brief Comparison (less) operation on vector components. */
    BVec4 operator< ( const Vec4& rhs ) const;

    /** \brief Comparison (less or equal) operation on vector components. */
    BVec4 operator<= ( const Vec4& rhs ) const;

    /** \brief Multiplication operation of vector and scalar. */
    Vec4 operator* ( const Float& rhs ) const;

    /** \brief Multiplication operation of vector and 2-column matrix. */
    Vec2 operator* ( const Mat2x4& rhs ) const;

    /** \brief Multiplication operation of vector and 3-column matrix. */
    Vec3 operator* ( const Mat3x4& rhs ) const;

    /** \brief Multiplication operation of vector and 4-column matrix. */
    Vec4 operator* ( const Mat4& rhs ) const;

    /** \brief Component selection operation. */
    auto operator[]( ESwizzle sw ) const;

    /** \brief Number of elements in this vector. */
    static const size_t item_count = 4;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Shader (GPU-side) data type for mutable variables of
    4-element 32-bit float vector type.
    
    Use this type inside shader code as a counterpart of CPU-side float[4] type,
    when it is required to change value of the vector.

    This is a l-value type. It can be initialized and changed any time.

    Beware that mutable variables can degrade performance on GPU, therefore
    Vec4 is preferable, unless you really want a mutable variable.
*/

class VVec4
{
public:
    /** \brief Constructs uninitialized vector variable. */
    VVec4();

    /** \brief Constructs vector variable and initializes it with given value. */
    VVec4 ( const Vec4& rhs );

    /** \brief Constructs vector variable and initializes it with given value. */
    VVec4 ( const VVec4& rhs );

    /** \brief Constructs vector variable and initializes it with given constant
        (curly braces syntax).
    */
    VVec4 ( const std::initializer_list< float >& initValue );

    /**
        \brief Constructs vector variable and initializes it from two GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T >
    VVec4 ( const Arg1T& arg1, const Arg2T& arg2 );

    /**
        \brief Constructs vector variable and initializes it from three GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T >
    VVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

    /**
        \brief Constructs vector variable and initializes it from four GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */
    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    VVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

    /**
        \brief Assigns new value to vector variable.
    */
    const VVec4& operator= ( const Vec4& rhs );

    /**
        \brief Assigns new value to vector variable.
    */
    const VVec4& operator= ( const VVec4& rhs );

    /**
        \brief Retrieves the r-value from vector variable.
    */
    operator Vec4() const;

    /** \brief Performs addition and assigns result to vector variable. */
    inline Vec4 operator+= ( const Vec4& rhs );

    /** \brief Performs subtraction and assigns result to vector variable. */
    inline Vec4 operator-= ( const Vec4& rhs );

    /** \brief Performs multiplication and assigns result to vector variable. */
    inline Vec4 operator*= ( const Vec4& rhs );

    /** \brief Performs division and assigns result to vector variable. */
    inline Vec4 operator/= ( const Vec4& rhs );

    /** \brief Computes remainder and assigns result to vector variable. */
    inline Vec4 operator%= ( const Vec4& rhs );

    /** \brief Performs shift to the left and assigns result to vector variable. */
    inline Vec4 operator<<= ( const Vec4& rhs );

    /** \brief Performs shift to the right and assigns result to vector variable. */
    inline Vec4 operator>>= ( const Vec4& rhs );

    /** \brief Performs bitwise AND and assigns result to vector variable. */
    inline Vec4 operator&= ( const Vec4& rhs );

    /** \brief Performs bitwise OR and assigns result to vector variable. */
    inline Vec4 operator|= ( const Vec4& rhs );

    /** \brief Performs bitwise XOR and assigns result to vector variable. */
    inline Vec4 operator^= ( const Vec4& rhs );

    /** \brief Component selection operation. */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;

    /** \brief Number of elements in this vector. */
    static const size_t item_count = 4;
};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 3-element 32-bit float vectors.

    This class is identical to Vec4 class in all aspects except the number
    of components.
*/

class Vec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3-element 32-bit float vector type.

    This class is identical to VVec4 class in all aspects except the number
    of components.
*/

class VVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2-element 32-bit float vectors.

    This class is identical to Vec4 class in all aspects except the number
    of components.
*/

class Vec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2-element 32-bit float vector type.

    This class is identical to VVec4 class in all aspects except the number
    of components.
*/

class VVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 4-element 32-bit integer vectors.

    Use this type inside shader code as a counterpart of CPU-side float[4] type.

    This is a r-value type. You must initialize it with value, either
    CPU-side one (an explicit constant), or an expression computed on GPU
    side. The value can not be changed.

    Constant initialization can be performed with initializer list syntax, i.e.:

    \code
        int i = 1;
        IVec4 v = IVec4 { 1, i, 1 + i, 0 };
    \endcode

    Note that from the GPU point of view, CPU-side variables are constants
    and can be used in initializers.

    Initialization with GPU-side values (variable or constant) is performed
    using the constructor syntax, e.g.:

    \code
        Int x, y, z;
        IVec3 coords ( x, y, z );
        IVec4 hcoords = IVec4 ( coords, Int ( 1 ) );
    \endcode

    For mutable variable type, see VIVec4. Beware that mutable variables
    can degrade performance on GPU, therefore IVec4 is preferable, unless
    you really want a mutable variable.
*/

class IVec4
{
public:
    /**
        \brief Vector initialization with curly braces syntax.

        For example:

        \code
            Vec4 { 1.0f, 0.5f, 2.0f, 1.0f }
        \endcode
    */

    IVec4 ( const std::initializer_list< int >& init );

    /**
        \brief Vector initialization to single constant value.
    */

    IVec4 ( int init );

    /**
        \brief Vector initialization from another vector.
    */

    IVec4 ( const IVec4& rhs );

    /**
        \brief Vector initialization from GPU-side value.

        The value can be a scalar, or another vector.
    */

    template< class Arg1T >
    explicit IVec4 ( const Arg1T& arg1 );

    /**
        \brief Vector initialization from two GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T >
    IVec4 ( const Arg1T& arg1, const Arg2T& arg2 );

    /**
        \brief Vector initialization from three GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T, class Arg3T >
    IVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 );

    /**
        \brief Vector initialization from four GPU-side values.

        These values can be scalars or vectors. They will be concatenated
        to form the resulting vector.
    */

    template< class Arg1T, class Arg2T, class Arg3T, class Arg4T >
    IVec4 ( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4 );

    /** \brief Addition operation on vector components. */
    IVec4 operator+ ( const IVec4& rhs ) const;

    /** \brief Subtraction operation on vector components. */
    IVec4 operator- ( const IVec4& rhs ) const;

    /** \brief Multiplication operation on vector components. */
    IVec4 operator* ( const IVec4& rhs ) const;

    /** \brief Division operation on vector components. */
    IVec4 operator/ ( const IVec4& rhs ) const;

    /** \brief Remainder operation on vector components. */
    IVec4 operator% ( const IVec4& rhs ) const;

    /** \brief Shift to the left operation on vector components. */
    IVec4 operator<< ( const IVec4& rhs ) const;

    /** \brief Shift to the right operation on vector components. */
    IVec4 operator>> ( const IVec4& rhs ) const;

    /** \brief Bitwise or operation on vector components. */
    IVec4 operator| ( const IVec4& rhs ) const;

    /** \brief Bitwise xor operation on vector components. */
    IVec4 operator^ ( const IVec4& rhs ) const;

    /** \brief Bitwise and operation on vector components. */
    IVec4 operator& ( const IVec4& rhs ) const;

    /** \brief Arithmetic negation operation on vector components. */
    IVec4 operator-() const;

    /** \brief Bitwise negation operation on vector components. */
    IVec4 operator~() const;

    /** \brief Comparison (equal) operation on vector components. */
    BVec4 operator== ( const IVec4& rhs ) const;

    /** \brief Comparison (not equal) operation on vector components. */
    BVec4 operator!= ( const IVec4& rhs ) const;

    /** \brief Comparison (greater) operation on vector components. */
    BVec4 operator> ( const IVec4& rhs ) const;

    /** \brief Comparison (greater or equal) operation on vector components. */
    BVec4 operator>= ( const IVec4& rhs ) const;

    /** \brief Comparison (less) operation on vector components. */
    BVec4 operator< ( const IVec4& rhs ) const;

    /** \brief Comparison (less or equal) operation on vector components. */
    BVec4 operator<= ( const IVec4& rhs ) const;

    /** \brief Component selection operation. */
    auto operator[]( ESwizzle sw ) const;

    /** \brief Number of elements in this vector. */
    static const size_t item_count = 4;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Shader (GPU-side) data type for mutable variables of
    4-element 32-bit integer vector type.
    
    Use this type inside shader code as a counterpart of CPU-side integer[4] type,
    when it is required to change value of the vector.

    This is a l-value type. It can be initialized and changed any time.

    Beware that mutable variables can degrade performance on GPU, therefore
    IVec4 is preferable, unless you really want a mutable variable.

    This class is very similar (except the data type) to floating-point
    counterpart, VVec4. For details, see VVec4 description.
*/

class VIVec4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 3-element 32-bit integer vectors.

    This class is identical to IVec4 class in all aspects except the number
    of components.
*/

class IVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3-element 32-bit integer vector type.

    This class is identical to VIVec4 class in all aspects except the number
    of components.
*/

class VIVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2-element 32-bit integer vectors.

    This class is identical to IVec4 class in all aspects except the number
    of components.
*/

class IVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2-element 32-bit integer vector type.

    This class is identical to VIVec4 class in all aspects except the number
    of components.
*/

class VIVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 4-element 32-bit unsigned integer vectors.

    This class is identical to IVec4 class in all aspects except the signedness.
    The component type is UInt. 
*/

class UVec4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    4-element 32-bit unsigned integer vector type.

    This class is identical to VIVec4 class in all aspects except the component type.
*/

class VUVec4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 3-element 32-bit unsigned integer vectors.

    This class is identical to IVec3 class in all aspects except the signedness.
    The component type is UInt. 
*/

class UVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3-element 32-bit unsigned integer vector type.

    This class is identical to VIVec3 class in all aspects except the component type.
*/

class VUVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2-element 32-bit unsigned integer vectors.

    This class is identical to IVec2 class in all aspects except the signedness.
    The component type is UInt. 
*/

class UVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2-element 32-bit unsigned integer vector type.

    This class is identical to VIVec2 class in all aspects except the component type.
*/

class VUVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 4-element 64-bit floating point vectors.

    This class is identical to Vec4 class in all aspects except the precision.
    The component type is Double.
*/

class DVec4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    4-element 64-bit float vector type.

    This class is identical to VVec4 class in all aspects except the precision.
*/

class VDVec4 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 3-element 64-bit floating point vectors.

    This class is identical to Vec3 class in all aspects except the precision.
    The component type is Double.
*/

class DVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    3-element 64-bit float vector type.

    This class is identical to VDVec4 class in all aspects except the number
    of components.
*/

class VDVec3 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for 2-element 64-bit floating point vectors.

    This class is identical to Vec2 class in all aspects except the precision.
    The component type is Double.
*/

class DVec2 {};

// -----------------------------------------------------------------------------
/** 
    \brief Shader (GPU-side) data type for mutable variables of
    2-element 64-bit float vector type.

    This class is identical to VDVec4 class in all aspects except the number
    of components.
*/

class VDVec2 {};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class PHVec4
{
public:
    PHVec4 ( const PHVec4& rhs );
    operator Vec4() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
