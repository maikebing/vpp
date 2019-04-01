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
namespace vpp {
// -----------------------------------------------------------------------------
/**
    \brief General type conversion function.

    StaticCast() is the analog of C++ \c static_cast operator. In VPP it can (and
    must) be used to convert between simple types. It can handle scalar types
    as well as vector and matrix types of same sizes. Some examples:

    \code
        Int vint = ...;
        UInt vuint = ...;
        Float vfloat = ...;
        Int64 vint64 = ...;
        UInt64 vuint64 = ...;

        IVec4 vivec4 = ...;
        Vec4 vfvec4 = ...;

        Int vint2 = StaticCast< Int >( vuint );
        Int vint3 = StaticCast< Int >( vfloat );
        Int vint4 = StaticCast< Int >( vint64 );
        Int vint5 = StaticCast< Int >( vuint64 );

        UInt vuint2 = StaticCast< UInt >( vint );
        Float vfloat2 = StaticCast< Float >( vuint );
        Int64 vint642 = StaticCast< Int64 >( vuint );
        
        Vec4 vfvec42 = StaticCast< Vec4 >( vivec4 );

        // ...
    \endcode

    It generally works in similar fashion as in C++. However, in VPP usage of this
    operator is much more frequent because VPP deliberately does not implement
    automatic type conversions of C++. This is to minimize code complexity, avoid
    possible compiler problems and cryptic error messages as well as reduce confusion
    and "surprise" to the user. Generally VPP will not convert different types
    (even if they only differ in signedness) unless you ask it to do so.

    For scalar types (but not vectors) VPP offers a shorthand on the form of
    additional constructor for a scalar type. For example:

    \code
        Int vint2 = Int ( vuint );
        Int vint3 = Int ( vfloat );
        Int vint4 = Int ( vint64 );
        Int vint5 = Int ( vuint64 );

        UInt vuint2 = UInt ( vint );
        Float vfloat2 = Float ( vuint );
        Int64 vint642 = Int64 ( vuint );
        
        // ...
    \endcode

    These constructors are equivalent to calling StaticCast() on their argument.
    They are declared as \c explicit.
*/

template< typename TargetT, typename SourceT >
TargetT StaticCast ( const SourceT& source );

// -----------------------------------------------------------------------------
/**
    \brief Bit pattern type conversion function.

    ReinterpretCast() works much like C++ \c reinterpret_cast operator. It can convert
    between scalars or vectors made from scalars of same size. This operator preserves
    bit pattern but not value. One of possible uses is to convert between floating-point
    and integer types, in order to utilise various bit tricks to speed up floating
    point approximations.
*/

template< typename TargetT, typename SourceT >
TargetT ReinterpretCast ( const SourceT& source );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

