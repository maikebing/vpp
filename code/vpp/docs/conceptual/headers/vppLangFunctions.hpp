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

/**
    \brief Computes nearest whole number to x.
    
    The fraction 0.5 will round in a direction chosen by the implementation,
    presumably the direction that is fastest. This includes the possibility
    that Round x is the same value as RoundEven x for all values of x.
*/

Float Round ( const Float& x );
Vec2 Round ( const Vec2& x );    /**< \overload */
Vec3 Round ( const Vec3& x );    /**< \overload */
Vec4 Round ( const Vec4& x );    /**< \overload */

Double Round ( const Double& x );    /**< \overload */
DVec2 Round ( const DVec2& x );    /**< \overload */
DVec3 Round ( const DVec3& x );    /**< \overload */
DVec4 Round ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes nearest whole number to x.
    
    A fractional part of 0.5 will round toward the nearest even whole number.
*/

Float RoundEven ( const Float& x );
Vec2 RoundEven ( const Vec2& x );    /**< \overload */
Vec3 RoundEven ( const Vec3& x );    /**< \overload */
Vec4 RoundEven ( const Vec4& x );    /**< \overload */

Double RoundEven ( const Double& x );    /**< \overload */
DVec2 RoundEven ( const DVec2& x );    /**< \overload */
DVec3 RoundEven ( const DVec3& x );    /**< \overload */
DVec4 RoundEven ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes nearest whole number to x whose absolute value is not larger
    than the absolute value of x.
*/

Float Trunc ( const Float& x );
Vec2 Trunc ( const Vec2& x );    /**< \overload */
Vec3 Trunc ( const Vec3& x );    /**< \overload */
Vec4 Trunc ( const Vec4& x );    /**< \overload */

Double Trunc ( const Double& x );    /**< \overload */
DVec2 Trunc ( const DVec2& x );    /**< \overload */
DVec3 Trunc ( const DVec3& x );    /**< \overload */
DVec4 Trunc ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Result is x if x >= 0. Otherwise result is -x.
*/

Int Abs ( const Int& x );
IVec2 Abs ( const IVec2& x );    /**< \overload */
IVec3 Abs ( const IVec3& x );    /**< \overload */
IVec4 Abs ( const IVec4& x );    /**< \overload */

Float Abs ( const Float& x );    /**< \overload */
Vec2 Abs ( const Vec2& x );    /**< \overload */
Vec3 Abs ( const Vec3& x );    /**< \overload */
Vec4 Abs ( const Vec4& x );    /**< \overload */

Double Abs ( const Double& x );    /**< \overload */
DVec2 Abs ( const DVec2& x );    /**< \overload */
DVec3 Abs ( const DVec3& x );    /**< \overload */
DVec4 Abs ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Result is 1 if x > 0, 0 if x == 0, or -1 if x < 0.
*/

Int Sign ( const Int& x );
IVec2 Sign ( const IVec2& x );    /**< \overload */
IVec3 Sign ( const IVec3& x );    /**< \overload */
IVec4 Sign ( const IVec4& x );    /**< \overload */

Float Sign ( const Float& x );    /**< \overload */
Vec2 Sign ( const Vec2& x );    /**< \overload */
Vec3 Sign ( const Vec3& x );    /**< \overload */
Vec4 Sign ( const Vec4& x );    /**< \overload */

Double Sign ( const Double& x );    /**< \overload */
DVec2 Sign ( const DVec2& x );    /**< \overload */
DVec3 Sign ( const DVec3& x );    /**< \overload */
DVec4 Sign ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes nearest whole number that is less than or equal to x.
*/

Float Floor ( const Float& x );
Vec2 Floor ( const Vec2& x );    /**< \overload */
Vec3 Floor ( const Vec3& x );    /**< \overload */
Vec4 Floor ( const Vec4& x );    /**< \overload */

Double Floor ( const Double& x );    /**< \overload */
DVec2 Floor ( const DVec2& x );    /**< \overload */
DVec3 Floor ( const DVec3& x );    /**< \overload */
DVec4 Floor ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes nearest whole number that is greater than or equal to x.
*/

Float Ceil ( const Float& x );
Vec2 Ceil ( const Vec2& x );    /**< \overload */
Vec3 Ceil ( const Vec3& x );    /**< \overload */
Vec4 Ceil ( const Vec4& x );    /**< \overload */

Double Ceil ( const Double& x );    /**< \overload */
DVec2 Ceil ( const DVec2& x );    /**< \overload */
DVec3 Ceil ( const DVec3& x );    /**< \overload */
DVec4 Ceil ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes x - Floor(x).
*/

Float Fract ( const Float& x );
Vec2 Fract ( const Vec2& x );    /**< \overload */
Vec3 Fract ( const Vec3& x );    /**< \overload */
Vec4 Fract ( const Vec4& x );    /**< \overload */

Double Fract ( const Double& x );    /**< \overload */
DVec2 Fract ( const DVec2& x );    /**< \overload */
DVec3 Fract ( const DVec3& x );    /**< \overload */
DVec4 Fract ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Converts degrees to radians.
*/

Float Radians ( const Float& x );
Vec2 Radians ( const Vec2& x );    /**< \overload */
Vec3 Radians ( const Vec3& x );    /**< \overload */
Vec4 Radians ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Converts radians to degrees.
*/

Float Degrees ( const Float& x );
Vec2 Degrees ( const Vec2& x );    /**< \overload */
Vec3 Degrees ( const Vec3& x );    /**< \overload */
Vec4 Degrees ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief The standard trigonometric sine of x radians.
*/

Float Sin ( const Float& x );
Vec2 Sin ( const Vec2& x );    /**< \overload */
Vec3 Sin ( const Vec3& x );    /**< \overload */
Vec4 Sin ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief The standard trigonometric cosine of x radians.
*/

Float Cos ( const Float& x );
Vec2 Cos ( const Vec2& x );    /**< \overload */
Vec3 Cos ( const Vec3& x );    /**< \overload */
Vec4 Cos ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief The standard trigonometric tangent of x radians.
*/

Float Tan ( const Float& x );
Vec2 Tan ( const Vec2& x );    /**< \overload */
Vec3 Tan ( const Vec3& x );    /**< \overload */
Vec4 Tan ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc sine. Result is an angle, in radians, whose sine is x.
    
    The range of result values is [-pi/2, pi/2]. Result is undefined if Abs(x) > 1.
*/

Float Asin ( const Float& x );
Vec2 Asin ( const Vec2& x );    /**< \overload */
Vec3 Asin ( const Vec3& x );    /**< \overload */
Vec4 Asin ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc cosine. Result is an angle, in radians, whose cosine is x.
    
    The range of result values is [0, pi]. Result is undefined if Abs(x) > 1.
*/

Float Acos ( const Float& x );
Vec2 Acos ( const Vec2& x );    /**< \overload */
Vec3 Acos ( const Vec3& x );    /**< \overload */
Vec4 Acos ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc tangent. Result is an angle, in radians, whose tangent is x.
    The range of result values is [-pi, pi].
*/

Float Atan ( const Float& x );
Vec2 Atan ( const Vec2& x );    /**< \overload */
Vec3 Atan ( const Vec3& x );    /**< \overload */
Vec4 Atan ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Hyperbolic sine of x radians.
*/

Float Sinh ( const Float& x );
Vec2 Sinh ( const Vec2& x );    /**< \overload */
Vec3 Sinh ( const Vec3& x );    /**< \overload */
Vec4 Sinh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Hyperbolic cosine of x radians.
*/

Float Cosh ( const Float& x );
Vec2 Cosh ( const Vec2& x );    /**< \overload */
Vec3 Cosh ( const Vec3& x );    /**< \overload */
Vec4 Cosh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Hyperbolic tangent of x radians.
*/

Float Tanh ( const Float& x );
Vec2 Tanh ( const Vec2& x );    /**< \overload */
Vec3 Tanh ( const Vec3& x );    /**< \overload */
Vec4 Tanh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc hyperbolic sine.
    
    Result is the inverse of Sinh.
*/

Float Asinh ( const Float& x );
Vec2 Asinh ( const Vec2& x );    /**< \overload */
Vec3 Asinh ( const Vec3& x );    /**< \overload */
Vec4 Asinh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc hyperbolic cosine.

    Result is the non-negative inverse of Cosh. Result is undefined if x < 1.
*/

Float Acosh ( const Float& x );
Vec2 Acosh ( const Vec2& x );    /**< \overload */
Vec3 Acosh ( const Vec3& x );    /**< \overload */
Vec4 Acosh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc hyperbolic tangent.
    
    Result is the inverse of Tanh. Result is undefined if Abs(x) >= 1.
*/

Float Atanh ( const Float& x );
Vec2 Atanh ( const Vec2& x );    /**< \overload */
Vec3 Atanh ( const Vec3& x );    /**< \overload */
Vec4 Atanh ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Arc tangent.
    
    Result is an angle, in radians, whose tangent is y/x.
    The signs of x and y are used to determine what quadrant the angle is in.
    The range of result values is [-pi, pi].
    Result is undefined if x and y are both 0.
*/

Float Atan2 ( const Float& y, const Float& x );
Vec2 Atan2 ( const Vec2& y, const Vec2& x );    /**< \overload */
Vec3 Atan2 ( const Vec3& y, const Vec3& x );    /**< \overload */
Vec4 Atan2 ( const Vec4& y, const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes x raised to the y power.

    Result is undefined if x < 0. Result is undefined if x = 0 and y <= 0.
*/

Float Pow ( const Float& x, const Float& y );
Vec2 Pow ( const Vec2& x, const Vec2& y );    /**< \overload */
Vec3 Pow ( const Vec3& x, const Vec3& y );    /**< \overload */
Vec4 Pow ( const Vec4& x, const Vec4& y );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the natural exponentiation of x (e^x).
*/

Float Exp ( const Float& x );
Vec2 Exp ( const Vec2& x );    /**< \overload */
Vec3 Exp ( const Vec3& x );    /**< \overload */
Vec4 Exp ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the natural logarithm of x.
    
    Result is undefined if x <= 0.
*/

Float Log ( const Float& x );
Vec2 Log ( const Vec2& x );    /**< \overload */
Vec3 Log ( const Vec3& x );    /**< \overload */
Vec4 Log ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes 2 raised to the x power (2^x).
*/

Float Exp2 ( const Float& x );
Vec2 Exp2 ( const Vec2& x );    /**< \overload */
Vec3 Exp2 ( const Vec3& x );    /**< \overload */
Vec4 Exp2 ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the base-2 logarithm of x.
    
    Result is undefined if x <= 0.
*/

Float Log2 ( const Float& x );
Vec2 Log2 ( const Vec2& x );    /**< \overload */
Vec3 Log2 ( const Vec3& x );    /**< \overload */
Vec4 Log2 ( const Vec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the square root of x.
    
    Result is undefined if x < 0.
*/

Float Sqrt ( const Float& x );
Vec2 Sqrt ( const Vec2& x );    /**< \overload */
Vec3 Sqrt ( const Vec3& x );    /**< \overload */
Vec4 Sqrt ( const Vec4& x );    /**< \overload */

Double Sqrt ( const Double& x );    /**< \overload */
DVec2 Sqrt ( const DVec2& x );    /**< \overload */
DVec3 Sqrt ( const DVec3& x );    /**< \overload */
DVec4 Sqrt ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the reciprocal of Sqrt(x).
    
    Result is undefined if x <= 0.
*/

Float InverseSqrt ( const Float& x );
Vec2 InverseSqrt ( const Vec2& x );    /**< \overload */
Vec3 InverseSqrt ( const Vec3& x );    /**< \overload */
Vec4 InverseSqrt ( const Vec4& x );    /**< \overload */

Double InverseSqrt ( const Double& value );    /**< \overload */
DVec2 InverseSqrt ( const DVec2& value );    /**< \overload */
DVec3 InverseSqrt ( const DVec3& value );    /**< \overload */
DVec4 InverseSqrt ( const DVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the determinant of x.
*/

Float Determinant ( const Mat2& x );
Float Determinant ( const Mat3& x );    /**< \overload */
Float Determinant ( const Mat4& x );    /**< \overload */

Double Determinant ( const DMat2& x );    /**< \overload */
Double Determinant ( const DMat3& x );    /**< \overload */
Double Determinant ( const DMat4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes a matrix that is the inverse of x.
    
    The values in the result are undefined if x is singular or poorly conditioned
    (nearly singular).
*/

Mat2 MatrixInverse ( const Mat2& x );
Mat3 MatrixInverse ( const Mat3& x );    /**< \overload */
Mat4 MatrixInverse ( const Mat4& x );    /**< \overload */

DMat2 MatrixInverse ( const DMat2& x );    /**< \overload */
DMat3 MatrixInverse ( const DMat3& x );    /**< \overload */
DMat4 MatrixInverse ( const DMat4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the transposition of a matrix.
*/

template< typename ScalarT, size_t COLS, size_t ROWS >
Matrix< ScalarT, ROWS, COLS > Transpose ( const Matrix< ScalarT, COLS, ROWS >& value );

// -----------------------------------------------------------------------------

/**
    \brief Result is y if y < x, otherwise result is x.
    
    Result is undefined if one of the operands is a NaN.
*/

Int Min ( const Int& x, const Int& y );
IVec2 Min ( const IVec2& x, const IVec2& y );    /**< \overload */
IVec3 Min ( const IVec3& x, const IVec3& y );    /**< \overload */
IVec4 Min ( const IVec4& x, const IVec4& y );    /**< \overload */

UInt Min ( const UInt& x, const UInt& y );    /**< \overload */
UVec2 Min ( const UVec2& x, const UVec2& y );    /**< \overload */
UVec3 Min ( const UVec3& x, const UVec3& y );    /**< \overload */
UVec4 Min ( const UVec4& x, const UVec4& y );    /**< \overload */

Float Min ( const Float& x, const Float& y );    /**< \overload */
Vec2 Min ( const Vec2& x, const Vec2& y );    /**< \overload */
Vec3 Min ( const Vec3& x, const Vec3& y );    /**< \overload */
Vec4 Min ( const Vec4& x, const Vec4& y );    /**< \overload */

Double Min ( const Double& x, const Double& y );    /**< \overload */
DVec2 Min ( const DVec2& x, const DVec2& y );    /**< \overload */
DVec3 Min ( const DVec3& x, const DVec3& y );    /**< \overload */
DVec4 Min ( const DVec4& x, const DVec4& y );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Result is y if x < y, otherwise result is x.
    
    Result is undefined if one of the operands is a NaN.
*/

Int Max ( const Int& x, const Int& y );
IVec2 Max ( const IVec2& x, const IVec2& y );    /**< \overload */
IVec3 Max ( const IVec3& x, const IVec3& y );    /**< \overload */
IVec4 Max ( const IVec4& x, const IVec4& y );    /**< \overload */

UInt Max ( const UInt& x, const UInt& y );    /**< \overload */
UVec2 Max ( const UVec2& x, const UVec2& y );    /**< \overload */
UVec3 Max ( const UVec3& x, const UVec3& y );    /**< \overload */
UVec4 Max ( const UVec4& x, const UVec4& y );    /**< \overload */

Float Max ( const Float& x, const Float& y );    /**< \overload */
Vec2 Max ( const Vec2& x, const Vec2& y );    /**< \overload */
Vec3 Max ( const Vec3& x, const Vec3& y );    /**< \overload */
Vec4 Max ( const Vec4& x, const Vec4& y );    /**< \overload */

Double Max ( const Double& x, const Double& y );    /**< \overload */
DVec2 Max ( const DVec2& x, const DVec2& y );    /**< \overload */
DVec3 Max ( const DVec3& x, const DVec3& y );    /**< \overload */
DVec4 Max ( const DVec4& x, const DVec4& y );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the value of Min ( Max ( x, minVal ), maxVal ).
    
    Result is undefined if minVal > maxVal. 
*/

Int Clamp ( const Int& x, const Int& minVal, const Int& maxVal );
IVec2 Clamp ( const IVec2& x, const IVec2& minVal, const IVec2& maxVal );    /**< \overload */
IVec3 Clamp ( const IVec3& x, const IVec3& minVal, const IVec3& maxVal );    /**< \overload */
IVec4 Clamp ( const IVec4& x, const IVec4& minVal, const IVec4& maxVal );    /**< \overload */

UInt Clamp ( const UInt& x, const UInt& minVal, const UInt& maxVal );    /**< \overload */
UVec2 Clamp ( const UVec2& x, const UVec2& minVal, const UVec2& maxVal );    /**< \overload */
UVec3 Clamp ( const UVec3& x, const UVec3& minVal, const UVec3& maxVal );    /**< \overload */
UVec4 Clamp ( const UVec4& x, const UVec4& minVal, const UVec4& maxVal );    /**< \overload */

Float Clamp ( const Float& x, const Float& minVal, const Float& maxVal );    /**< \overload */
Vec2 Clamp ( const Vec2& x, const Vec2& minVal, const Vec2& maxVal );    /**< \overload */
Vec3 Clamp ( const Vec3& x, const Vec3& minVal, const Vec3& maxVal );    /**< \overload */
Vec4 Clamp ( const Vec4& x, const Vec4& minVal, const Vec4& maxVal );    /**< \overload */

Double Clamp ( const Double& x, const Double& minVal, const Double& maxVal );    /**< \overload */
DVec2 Clamp ( const DVec2& x, const DVec2& minVal, const DVec2& maxVal );    /**< \overload */
DVec3 Clamp ( const DVec3& x, const DVec3& minVal, const DVec3& maxVal );    /**< \overload */
DVec4 Clamp ( const DVec4& x, const DVec4& minVal, const DVec4& maxVal );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes the linear blend of x and y, i.e., x * (1 - a) + y * a.
*/

Float Mix ( const Float& x, const Float& y, const Float& a );
Vec2 Mix ( const Vec2& x, const Vec2& y, const Vec2& a );    /**< \overload */
Vec3 Mix ( const Vec3& x, const Vec3& y, const Vec3& a );    /**< \overload */
Vec4 Mix ( const Vec4& x, const Vec4& y, const Vec4& a );    /**< \overload */

Double Mix ( const Double& x, const Double& y, const Double& a );    /**< \overload */
DVec2 Mix ( const DVec2& x, const DVec2& y, const DVec2& a );    /**< \overload */
DVec3 Mix ( const DVec3& x, const DVec3& y, const DVec3& a );    /**< \overload */
DVec4 Mix ( const DVec4& x, const DVec4& y, const DVec4& a );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Result is 0 if x < edge. Otherwise result is 1.
*/

Float Step ( const Float& edge, const Float& x );
Vec2 Step ( const Vec2& edge, const Vec2& x );    /**< \overload */
Vec3 Step ( const Vec3& edge, const Vec3& x );    /**< \overload */
Vec4 Step ( const Vec4& edge, const Vec4& x );    /**< \overload */

Double Step ( const Double& edge, const Double& x );    /**< \overload */
DVec2 Step ( const DVec2& edge, const DVec2& x );    /**< \overload */
DVec3 Step ( const DVec3& edge, const DVec3& x );    /**< \overload */
DVec4 Step ( const DVec4& edge, const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Result is 0 if x <= edge0, 1 if x >= edge1, or smooth Hermite interpolation
    between 0 and 1 when edge0 < x < edge1.
    
    This is equivalent to:
        t*t*(3 - 2*t), where t = Clamp ( (x-edge0) / (edge1-edge0), 0, 1 )

    Result is undefined if edge0 >= edge1.
*/

Float SmoothStep ( const Float& edge0, const Float& edge1, const Float& x );
Vec2 SmoothStep ( const Vec2& edge0, const Vec2& edge1, const Vec2& x );    /**< \overload */
Vec3 SmoothStep ( const Vec3& edge0, const Vec3& edge1, const Vec3& x );    /**< \overload */
Vec4 SmoothStep ( const Vec4& edge0, const Vec4& edge1, const Vec4& x );    /**< \overload */

Double SmoothStep ( const Double& edge0, const Double& edge1, const Double& x );    /**< \overload */
DVec2 SmoothStep ( const DVec2& edge0, const DVec2& edge1, const DVec2& x );    /**< \overload */
DVec3 SmoothStep ( const DVec3& edge0, const DVec3& edge1, const DVec3& x );    /**< \overload */
DVec4 SmoothStep ( const DVec4& edge0, const DVec4& edge1, const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Computes a * b + c.
*/

Float Fma ( const Float& edge0, const Float& edge1, const Float& x );
Vec2 Fma ( const Vec2& edge0, const Vec2& edge1, const Vec2& x );    /**< \overload */
Vec3 Fma ( const Vec3& edge0, const Vec3& edge1, const Vec3& x );    /**< \overload */
Vec4 Fma ( const Vec4& edge0, const Vec4& edge1, const Vec4& x );    /**< \overload */

Double Fma ( const Double& a, const Double& b, const Double& c );    /**< \overload */
DVec2 Fma ( const DVec2& a, const DVec2& b, const DVec2& c );    /**< \overload */
DVec3 Fma ( const DVec3& a, const DVec3& b, const DVec3& c );    /**< \overload */
DVec4 Fma ( const DVec4& a, const DVec4& b, const DVec4& c );    /**< \overload */

// -----------------------------------------------------------------------------

/**
    \brief Converts each component of the signed normalized floating-point value v
           into 8-bit integer values and packs them into the result.

    The conversion for component c of v to fixed point is done as follows:
    \code
    Round ( Clamp ( c, -1, +1 ) * 127.0 )
    \endcode

    The first component of the vector will be written to the least significant
    bits of the output. The last component will be written to the most significant
    bits.
*/

UInt PackSnorm4x8 ( const Vec4& v );

/**
    \brief Converts each component of the positive normalized floating-point value v
           into 8-bit integer values and packs them into the result.

    The conversion for component c of v to fixed point is done as follows:
    \code
    Round ( Clamp ( c, 0, +1 ) * 255.0 )
    \endcode

    The first component of the vector will be written to the least significant
    bits of the output. The last component will be written to the most significant
    bits.
*/

UInt PackUnorm4x8 ( const Vec4& v );

/**
    \brief Converts each component of the signed normalized floating-point value v
    into 16-bit integer values and packs them into the result.

    The conversion for component c of v to fixed point is done as follows:
    \code
    Round ( Clamp ( c, -1, +1 ) * 32767.0 )
    \endcode

    The first component of the vector will be written to the least significant
    bits of the output. The last component will be written to the most significant
    bits.
*/

UInt PackSnorm2x16 ( const Vec2& v );

/**
    \brief Converts each component of the positive normalized floating-point value v
    into 16-bit integer values and packs them into the result.

    The conversion for component c of v to fixed point is done as follows:
    \code
    Round ( Clamp ( c, 0, +1 ) * 65535.0 )
    \endcode

    The first component of the vector will be written to the least significant
    bits of the output. The last component will be written to the most significant
    bits.
*/

UInt PackUnorm2x16 ( const Vec2& v );

/**
    \brief Converts the components of a two-component floating-point vector
    to the 16-bit float, then packs them into a 32-bit unsigned integer.

    The first vector component specifies the 16 least-significant bits of the result.
    The second component specifies the 16 most-significant bits.
*/

UInt PackHalf2x16 ( const Vec2& v );

/**
    \brief Packs the components of v into a 64-bit value.
    
    If an IEEE 754 Inf or NaN is created, it will not signal, and the resulting
    floating-point value is unspecified. Otherwise, the bit-level representation of
    v is preserved.
    
    The first vector component specifies the 32 least significant bits. The second
    component specifies the 32 most significant bits.
*/

Double PackDouble2x32 ( const UVec2& v );

/**
    \brief Unpacks a single 32-bit unsigned integer p into a pair of 16-bit signed
           integers and converts them to normalized floating-point values.

    The conversion for unpacked fixed-point value f to floating point is done as follows:
    \code
    Clamp ( f / 32767.0, -1, +1 )
    \endcode

    The first component of the result will be extracted from the least significant
    bits of the input. The last component will be extracted from the most significant
    bits.
*/

Vec2 UnpackSnorm2x16 ( const UInt& p );

/**
    \brief Unpacks a single 32-bit unsigned integer p into a pair of 16-bit unsigned
           integers. and converts them to normalized floating-point values.
           
    The conversion for unpacked fixed-point value f to floating point is done as follows:
    \code
    f / 65535.0
    \endcode

    The first component of the result will be extracted from the least significant bits
    of the input. The last component will be extracted from the most significant bits.
*/

Vec2 UnpackUnorm2x16 ( const UInt& p );

/**
    \brief Unpacks a 32-bit unsigned integer into a pair of 16-bit values, interpreted
           as 16-bit floating-point numbers, and converts them to 32-bit floating-point 
           values.
           
    Subnormal numbers are either preserved or flushed to zero, consistently within an
    implemenation.

    The first component of the vector is obtained from the 16 least-significant bits
    of v. The second component is obtained from the 16 most-significant bits of v.
*/

Vec2 UnpackHalf2x16 ( const UInt& v );

/**
    \brief Unpacks a single 32-bit unsigned integer p into four 8-bit signed integers
           and converts them to normalized floating-point values.
           
    The conversion for unpacked fixed-point value f to floating point is done as follows:
    \code
    Clamp ( f / 127.0, -1, +1 )
    \endcode

    The first component of the result will be extracted from the least significant bits
    of the input. The last component will be extracted from the most significant bits.
*/

Vec4 UnpackSnorm4x8 ( const UInt& p );

/**
    \brief Unpacks a single 32-bit unsigned integer p into four 8-bit unsigned integers
           and converts them to normalized floating-point values
           
    The conversion for unpacked fixed-point value f to floating point is done as follows:
    \code
    f / 255.0
    \endcode

    The first component of the result will be extracted from the least significant bits
    of the input. The last component will be extracted from the most significant bits.
*/

Vec4 UnpackUnorm4x8 ( const UInt& p );

/**
    \brief Computes the two-component unsigned integer vector representation of v.
           The bit-level representation of v is preserved.
           
    The first component of the vector contains the 32 least significant bits
    of the \c double. The second component consists of the 32 most significant bits.
*/

UVec2 UnpackDouble2x32 ( const Double& value );

// -----------------------------------------------------------------------------
/**
    \brief Computes dot product of two vectors.
*/

Float Dot ( const Vec2& lhs, const Vec2& rhs );
Float Dot ( const Vec3& lhs, const Vec3& rhs );    /**< \overload */
Float Dot ( const Vec4& lhs, const Vec4& rhs );    /**< \overload */

Double Dot ( const DVec2& lhs, const DVec2& rhs );    /**< \overload */
Double Dot ( const DVec3& lhs, const DVec3& rhs );    /**< \overload */
Double Dot ( const DVec4& lhs, const DVec4& rhs );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes linear-algebraic outer product of two vectors.
*/

template< size_t COLS, size_t ROWS >
inline TRMatrix< Float, COLS, ROWS > OuterProduct (
    const TRVector< Float, ROWS >& lhs,
    const TRVector< Float, COLS >& rhs );


template< size_t COLS, size_t ROWS >
inline TRMatrix< Double, COLS, ROWS > OuterProduct (
    const TRVector< Double, ROWS >& lhs,
    const TRVector< Double, COLS >& rhs );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the length of a vector.
*/

Float Length ( const Float& value );
Float Length ( const Vec2& value );    /**< \overload */
Float Length ( const Vec3& value );    /**< \overload */
Float Length ( const Vec4& value );    /**< \overload */

Double Length ( const Double& value );    /**< \overload */
Double Length ( const DVec2& value );    /**< \overload */
Double Length ( const DVec3& value );    /**< \overload */
Double Length ( const DVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the distance between v1 and v2.
*/

Float Distance ( const Float& v1, const Float& v2 );
Float Distance ( const Vec2& v1, const Vec2& v2 );    /**< \overload */
Float Distance ( const Vec3& v1, const Vec3& v2 );    /**< \overload */
Float Distance ( const Vec4& v1, const Vec4& v2 );    /**< \overload */

Double Distance ( const Double& v1, const Double& v2 );    /**< \overload */
Double Distance ( const DVec2& v1, const DVec2& v2 );    /**< \overload */
Double Distance ( const DVec3& v1, const DVec3& v2 );    /**< \overload */
Double Distance ( const DVec4& v1, const DVec4& v2 );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the cross product of x and y.
*/

Vec3 Cross ( const Vec3& x, const Vec3& y );
DVec3 Cross ( const DVec3& x, const DVec3& y );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the vector in the same direction as x but with a length of 1.
*/

Float Normalize ( const Float& x );
Vec2 Normalize ( const Vec2& x );    /**< \overload */
Vec3 Normalize ( const Vec3& x );    /**< \overload */
Vec4 Normalize ( const Vec4& x );    /**< \overload */

Double Normalize ( const Double& x );    /**< \overload */
DVec2 Normalize ( const DVec2& x );    /**< \overload */
DVec3 Normalize ( const DVec3& x );    /**< \overload */
DVec4 Normalize ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief If the dot product of \c nref and \c i is negative,
           the result is n, otherwise it is -n.
*/

Float FaceForward ( const Float& n, const Float& i, const Float& nref );
Vec2 FaceForward ( const Vec2& n, const Vec2& i, const Vec2& nref );    /**< \overload */
Vec3 FaceForward ( const Vec3& n, const Vec3& i, const Vec3& nref );    /**< \overload */
Vec4 FaceForward ( const Vec4& n, const Vec4& i, const Vec4& nref );    /**< \overload */

Double FaceForward ( const Double& n, const Double& i, const Double& nref );    /**< \overload */
DVec2 FaceForward ( const DVec2& n, const DVec2& i, const DVec2& nref );    /**< \overload */
DVec3 FaceForward ( const DVec3& n, const DVec3& i, const DVec3& nref );    /**< \overload */
DVec4 FaceForward ( const DVec4& n, const DVec4& i, const DVec4& nref );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes reflected vector to the vector \c i.

    For the incident vector \c i and surface orientation \c n, the result is the
    reflection direction:
    \code
    i - 2 * Dot ( n, i ) * n
    \endcode

    Vector n must already be normalized in order to achieve the desired result.
*/

Float Reflect ( const Float& i, const Float& n );
Vec2 Reflect ( const Vec2& i, const Vec2& n );    /**< \overload */
Vec3 Reflect ( const Vec3& i, const Vec3& n );    /**< \overload */
Vec4 Reflect ( const Vec4& i, const Vec4& n );    /**< \overload */

Double Reflect ( const Double& i, const Double& n );    /**< \overload */
DVec2 Reflect ( const DVec2& i, const DVec2& n );    /**< \overload */
DVec3 Reflect ( const DVec3& i, const DVec3& n );    /**< \overload */
DVec4 Reflect ( const DVec4& i, const DVec4& n );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes refracted vector to the vector \c i.

    For the incident vector \c i and surface normal \c n, and the ratio of indices
    of refraction eta, the result is the refraction vector. The result is computed by:
    
    \code
        k = 1.0 - eta * eta * ( 1.0 - Dot ( n, i ) * Dot ( n, i ) )
    \endcode

    If \c k < 0 the result is 0, otherwise, the result is:
    \code
        eta * i - ( eta * Dot ( n, i ) + Sqrt(k) ) * n
    \endcode

    The input parameters for the incident vector \c i and the surface normal \c n
    must already be normalized to get the desired results.
*/

Float Refract ( const Float& i, const Float& n, const Float& eta );
Vec2 Refract ( const Vec2& i, const Vec2& n, const Float& eta );    /**< \overload */
Vec3 Refract ( const Vec3& i, const Vec3& n, const Float& eta );    /**< \overload */
Vec4 Refract ( const Vec4& i, const Vec4& n, const Float& eta );    /**< \overload */

Double Refract ( const Double& i, const Double& n, const Float& eta );    /**< \overload */
DVec2 Refract ( const DVec2& i, const DVec2& n, const Float& eta );    /**< \overload */
DVec3 Refract ( const DVec3& i, const DVec3& n, const Float& eta );    /**< \overload */
DVec4 Refract ( const DVec4& i, const DVec4& n, const Float& eta );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the bit number of the least-significant 1-bit in the binary
    representation of \c v.
    
    If \c v is 0, the result is -1.
*/

Int FindLsb ( const Int& v );
IVec2 FindLsb ( const IVec2& v );    /**< \overload */
IVec3 FindLsb ( const IVec3& v );    /**< \overload */
IVec4 FindLsb ( const IVec4& v );    /**< \overload */

UInt FindLsb ( const UInt& v );    /**< \overload */
UVec2 FindLsb ( const UVec2& v );    /**< \overload */
UVec3 FindLsb ( const UVec3& v );    /**< \overload */
UVec4 FindLsb ( const UVec4& v );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the bit number of the most-significant 1-bit in the binary
    representation of \c v.

    For unsigned or or positive numbers, the result will be the bit number of
    the most significant 1-bit. For signed negative numbers, the result will
    be the bit number of the most significant 0-bit. For a value of 0 or -1,
    the result is -1.
*/

Int FindMsb ( const Int& value );
IVec2 FindMsb ( const IVec2& value );    /**< \overload */
IVec3 FindMsb ( const IVec3& value );    /**< \overload */
IVec4 FindMsb ( const IVec4& value );    /**< \overload */

UInt FindMsb ( const UInt& value );    /**< \overload */
UVec2 FindMsb ( const UVec2& value );    /**< \overload */
UVec3 FindMsb ( const UVec3& value );    /**< \overload */
UVec4 FindMsb ( const UVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is y if y < x, otherwise result is x.
    
    If one operand is a NaN, the other operand is the result.
    If both operands are NaN, the result is a NaN.
*/

Float NMin ( const Float& x, const Float& y );
Vec2 NMin ( const Vec2& x, const Vec2& y );    /**< \overload */
Vec3 NMin ( const Vec3& x, const Vec3& y );    /**< \overload */
Vec4 NMin ( const Vec4& x, const Vec4& y );    /**< \overload */

Double NMin ( const Double& x, const Double& y );    /**< \overload */
DVec2 NMin ( const DVec2& x, const DVec2& y );    /**< \overload */
DVec3 NMin ( const DVec3& x, const DVec3& y );    /**< \overload */
DVec4 NMin ( const DVec4& x, const DVec4& y );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is y if x < y, otherwise result is x.
    
    If one operand is a NaN, the other operand is the result.
    If both operands are NaN, the result is a NaN.
*/

Float NMax ( const Float& x, const Float& y );
Vec2 NMax ( const Vec2& x, const Vec2& y );    /**< \overload */
Vec3 NMax ( const Vec3& x, const Vec3& y );    /**< \overload */
Vec4 NMax ( const Vec4& x, const Vec4& y );    /**< \overload */

Double NMax ( const Double& x, const Double& y );    /**< \overload */
DVec2 NMax ( const DVec2& x, const DVec2& y );    /**< \overload */
DVec3 NMax ( const DVec3& x, const DVec3& y );    /**< \overload */
DVec4 NMax ( const DVec4& x, const DVec4& y );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is min ( max ( x, minVal), maxVal ).
    
    Result is undefined if minVal > maxVal.The semantics used by min() and max()
    are those of NMin and NMax.
*/

Float NClamp ( const Float& x, const Float& minVal, const Float& maxVal );
Vec2 NClamp ( const Vec2& x, const Vec2& minVal, const Vec2& maxVal );    /**< \overload */
Vec3 NClamp ( const Vec3& x, const Vec3& minVal, const Vec3& maxVal );    /**< \overload */
Vec4 NClamp ( const Vec4& x, const Vec4& minVal, const Vec4& maxVal );    /**< \overload */

Double NClamp ( const Double& x, const Double& minVal, const Double& maxVal );    /**< \overload */
DVec2 NClamp ( const DVec2& x, const DVec2& minVal, const DVec2& maxVal );    /**< \overload */
DVec3 NClamp ( const DVec3& x, const DVec3& minVal, const DVec3& maxVal );    /**< \overload */
DVec4 NClamp ( const DVec4& x, const DVec4& minVal, const DVec4& maxVal );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Modulo operation of \c lhs modulo \c rhs.
    
    The sign of a non-zero result comes from the right operand.
*/

Int Mod ( const Int& lhs, const Int& rhs );
IVec2 Mod ( const IVec2& lhs, const IVec2& rhs );    /**< \overload */
IVec3 Mod ( const IVec3& lhs, const IVec3& rhs );    /**< \overload */
IVec4 Mod ( const IVec4& lhs, const IVec4& rhs );    /**< \overload */
IMat2 Mod ( const IMat2& lhs, const IMat2& rhs );    /**< \overload */
IMat3 Mod ( const IMat3& lhs, const IMat3& rhs );    /**< \overload */
IMat4 Mod ( const IMat4& lhs, const IMat4& rhs );    /**< \overload */

UInt Mod ( const UInt& lhs, const UInt& rhs );    /**< \overload */
UVec2 Mod ( const UVec2& lhs, const UVec2& rhs );    /**< \overload */
UVec3 Mod ( const UVec3& lhs, const UVec3& rhs );    /**< \overload */
UVec4 Mod ( const UVec4& lhs, const UVec4& rhs );    /**< \overload */
UMat2 Mod ( const UMat2& lhs, const UMat2& rhs );    /**< \overload */
UMat3 Mod ( const UMat3& lhs, const UMat3& rhs );    /**< \overload */
UMat4 Mod ( const UMat4& lhs, const UMat4& rhs );    /**< \overload */

Float Mod ( const Float& lhs, const Float& rhs );    /**< \overload */
Vec2 Mod ( const Vec2& lhs, const Vec2& rhs );    /**< \overload */
Vec3 Mod ( const Vec3& lhs, const Vec3& rhs );    /**< \overload */
Vec4 Mod ( const Vec4& lhs, const Vec4& rhs );    /**< \overload */
Mat2 Mod ( const Mat2& lhs, const Mat2& rhs );    /**< \overload */
Mat3 Mod ( const Mat3& lhs, const Mat3& rhs );    /**< \overload */
Mat4 Mod ( const Mat4& lhs, const Mat4& rhs );    /**< \overload */

Double Mod ( const Double& lhs, const Double& rhs );    /**< \overload */
DVec2 Mod ( const DVec2& lhs, const DVec2& rhs );    /**< \overload */
DVec3 Mod ( const DVec3& lhs, const DVec3& rhs );    /**< \overload */
DVec4 Mod ( const DVec4& lhs, const DVec4& rhs );    /**< \overload */
DMat2 Mod ( const DMat2& lhs, const DMat2& rhs );    /**< \overload */
DMat3 Mod ( const DMat3& lhs, const DMat3& rhs );    /**< \overload */
DMat4 Mod ( const DMat4& lhs, const DMat4& rhs );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Computes the fractional part of x (as the first result)
    and the whole number part (second result).
    
    Both results will have the same sign as x.
*/

std::pair< Float, Float > Modf ( const Float& x );
std::pair< Vec2, Vec2 > Modf ( const Vec2& x );    /**< \overload */
std::pair< Vec3, Vec3 > Modf ( const Vec3& x );    /**< \overload */
std::pair< Vec4, Vec4 > Modf ( const Vec4& x );    /**< \overload */

std::pair< Double, Double > Modf ( const Double& x );    /**< \overload */
std::pair< DVec2, DVec2 > Modf ( const DVec2& x );    /**< \overload */
std::pair< DVec3, DVec3 > Modf ( const DVec3& x );    /**< \overload */
std::pair< DVec4, DVec4 > Modf ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Builds a floating-point number from \c x and the corresponding integral
           exponent of two in \c exp.

    Computes the result as:
    \code
    x * 2^exp
    \endcode

    If this product is too large to be represented in the floating-point type,
    the result is undefined. If \c exp is greater than +128 (single precision)
    or +1024 (double precision), the result undefined. If \c exp is less than
    -126 (single precision) or -1022 (double precision), the result may be
    flushed to zero. Additionally, splitting the value into a significand and
    exponent using Frexp and then reconstructing a floating-point value using
    Ldexp should yield the original input for zero and all finite non-denormalized
    values.
*/

Float Ldexp ( const Float& x, const Int& exp );
Vec2 Ldexp ( const Vec2& x, const IVec2& exp );    /**< \overload */
Vec3 Ldexp ( const Vec3& x, const IVec3& exp );    /**< \overload */
Vec4 Ldexp ( const Vec4& x, const IVec4& exp );    /**< \overload */

Double Ldexp ( const Double& x, const Int& exp );    /**< \overload */
DVec2 Ldexp ( const DVec2& x, const IVec2& exp );    /**< \overload */
DVec3 Ldexp ( const DVec3& x, const IVec3& exp );    /**< \overload */
DVec4 Ldexp ( const DVec4& x, const IVec4& exp );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Splits x into a floating-point significand in the range [0.5, 1.0)
           and an integral exponent of two,

    The computation is being performed according to the formula:
    \code
    x = significand * 2^exponent
    \endcode

    The significand is the first result and the exponent is the second result.
    
    For a floating-point value of zero, the significand and exponent are both zero.
    
    For a floating-point value that is an infinity or is not a number,
    the result is undefined.

    If an implementation supports negative 0, Frexp (-0) should result in -0.
    Otherwise it will result in 0.
*/

std::pair< Float, Int > Frexp ( const Float& x );
std::pair< Vec2, IVec2 > Frexp ( const Vec2& x );    /**< \overload */
std::pair< Vec3, IVec3 > Frexp ( const Vec3& x );    /**< \overload */
std::pair< Vec4, IVec4 > Frexp ( const Vec4& x );    /**< \overload */

std::pair< Double, Int > Frexp ( const Double& x );    /**< \overload */
std::pair< DVec2, IVec2 > Frexp ( const DVec2& x );    /**< \overload */
std::pair< DVec3, IVec3 > Frexp ( const DVec3& x );    /**< \overload */
std::pair< DVec4, IVec4 > Frexp ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is true if x is an IEEE NaN, otherwise result is false.*/

Bool IsNaN ( const Float& x );
BVec2 IsNaN ( const Vec2& x );    /**< \overload */
BVec3 IsNaN ( const Vec3& x );    /**< \overload */
BVec4 IsNaN ( const Vec4& x );    /**< \overload */

Bool IsNaN ( const Double& x );    /**< \overload */
BVec2 IsNaN ( const DVec2& x );    /**< \overload */
BVec3 IsNaN ( const DVec3& x );    /**< \overload */
BVec4 IsNaN ( const DVec4& x );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is true if x is an IEEE Inf, otherwise result is false.
*/

Bool IsInf ( const Float& value );
BVec2 IsInf ( const Vec2& value );    /**< \overload */
BVec3 IsInf ( const Vec3& value );    /**< \overload */
BVec4 IsInf ( const Vec4& value );    /**< \overload */

Bool IsInf ( const Double& value );    /**< \overload */
BVec2 IsInf ( const DVec2& value );    /**< \overload */
BVec3 IsInf ( const DVec3& value );    /**< \overload */
BVec4 IsInf ( const DVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is true if any component of the vector is true, otherwise false.
*/

Bool IsAnyComponentTrue ( const BVec2& v );
Bool IsAnyComponentTrue ( const BVec3& v );    /**< \overload */
Bool IsAnyComponentTrue ( const BVec4& v );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Result is true if all components of the vector are true, otherwise false.
*/

Bool IsAllComponentsTrue ( const BVec2& v );
Bool IsAllComponentsTrue ( const BVec3& v );    /**< \overload */
Bool IsAllComponentsTrue ( const BVec4& v );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Reverses the bits in specified value.

    Results are computed per component.
*/

Int BitCount ( const Int& value );
IVec2 BitCount ( const IVec2& value );    /**< \overload */
IVec3 BitCount ( const IVec3& value );    /**< \overload */
IVec4 BitCount ( const IVec4& value );    /**< \overload */

Int BitCount ( const UInt& value );    /**< \overload */
IVec2 BitCount ( const UVec2& value );    /**< \overload */
IVec3 BitCount ( const UVec3& value );    /**< \overload */
IVec4 BitCount ( const UVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Counts the number of set bits in specified value.

    Results are computed per component.
*/

Int BitReverse ( const Int& value );
IVec2 BitReverse ( const IVec2& value );    /**< \overload */
IVec3 BitReverse ( const IVec3& value );    /**< \overload */
IVec4 BitReverse ( const IVec4& value );    /**< \overload */

UInt BitReverse ( const UInt& value );    /**< \overload */
UVec2 BitReverse ( const UVec2& value );    /**< \overload */
UVec3 BitReverse ( const UVec3& value );    /**< \overload */
UVec4 BitReverse ( const UVec4& value );    /**< \overload */

// -----------------------------------------------------------------------------
/**
    \brief Declares image binding point as used in the shader.

    In general, image binding points do not require to be declared before accessing
    them. There is one exception, though: if the first access to the image binding
    point is inside conditional block, then it must be declared earlier in the code
    to prevent SPIR-V error. The error is caused by the fact that image can't be
    introduced conditionally. 

    An example:

    \code
        void MyPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            // ...

            UseImage ( m_myImage );  // <---- add this to declare image unconditionally

            If ( ... condition ... );
                ImageStore ( m_myImage, IVec2 { x, y }, value );  // first access is conditional
            Fi();

            // ...
        }
    \endcode
*/

template< class ImageT >
void UseImage ( const ImageT& img );

// -----------------------------------------------------------------------------
/**
    \brief Computes the expression given as argument and ignores the result.

    This is useful to suppress validation warnings about unused
    objects (e.g. buffer accessors). Can be handy if you temporarily comment out
    some code to pinpoint a bug.
*/

template< class ValueT >
void Ignore ( const ValueT& value );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
