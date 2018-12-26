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

#ifndef INC_VPPBLENDOPERATORS_HPP
#define INC_VPPBLENDOPERATORS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPTYPES_HPP
#include "vppTypes.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

// direct operands

enum EC0 { C0 };
enum EC1 { C1 };
enum ECS { CS, Cs };
enum ECD { CD, Cd };
enum ECC { CC, Cc };
enum EAS { AS, As };
enum EAD { AD, Ad };
enum EAC { AC, Ac };
enum EASAT { ASAT, Asat };
enum ECS1 { CS1, Cs1 };
enum EAS1 { AS1, As1 };

// -----------------------------------------------------------------------------
// negated operands

enum ECSN { CSN };
enum ECDN { CDN };
enum ECCN { CCN };
enum EASN { ASN };
enum EADN { ADN };
enum EACN { ACN };
enum ECS1N { CS1N };
enum EAS1N { AS1N };
enum EASATN { ASATN };

VPP_INLINE EC1 operator- ( EC1, EC0 ) { return EC1(); };
VPP_INLINE ECSN operator- ( EC1, ECS ) { return ECSN(); };
VPP_INLINE ECDN operator- ( EC1, ECD ) { return ECDN(); };
VPP_INLINE ECCN operator- ( EC1, ECC ) { return ECCN(); };
VPP_INLINE EASN operator- ( EC1, EAS ) { return EASN(); };
VPP_INLINE EADN operator- ( EC1, EAD ) { return EADN(); };
VPP_INLINE EACN operator- ( EC1, EAC ) { return EACN(); };
VPP_INLINE ECS1N operator- ( EC1, ECS1 ) { return ECS1N(); };
VPP_INLINE EAS1N operator- ( EC1, EAS1 ) { return EAS1N(); };
VPP_INLINE EASATN operator- ( EC1, EASAT ) { return EASATN(); };

// -----------------------------------------------------------------------------
// operand -> factor

template< typename OperandT > struct TOperandTraits { static const bool isValid = false; };
template<> struct TOperandTraits< EC0 >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ZERO; static const bool isValid = true; };
template<> struct TOperandTraits< EC1 >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE; static const bool isValid = true; };
template<> struct TOperandTraits< ECS >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_SRC_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< ECD >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_DST_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< ECC >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_CONSTANT_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< EAS >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_SRC_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< EAD >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_DST_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< EAC >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_CONSTANT_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< EASAT >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE; static const bool isValid = true; };
template<> struct TOperandTraits< ECS1 >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_SRC1_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< EAS1 >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_SRC1_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< ECSN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< ECDN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< ECCN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< EASN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< EADN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< EACN >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA; static const bool isValid = true; };
template<> struct TOperandTraits< ECS1N >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR; static const bool isValid = true; };
template<> struct TOperandTraits< EAS1N >{ static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA; static const bool isValid = true; };

// -----------------------------------------------------------------------------
// operations
// -----------------------------------------------------------------------------

template< typename OP1, typename OP2 >
struct TBlendAdd;

template< typename OP1, typename OP2 >
struct TBlendSub;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< bool DEST, typename OPERAND >
struct TBlendMul
{
    static_assert ( TOperandTraits< OPERAND >::isValid, "Invalid blending operand" );

    static const VkBlendFactor SOURCE_FACTOR =
        DEST ? VK_BLEND_FACTOR_ZERO : TOperandTraits< OPERAND >::FACTOR;

    static const VkBlendFactor DEST_FACTOR =
        DEST ? TOperandTraits< OPERAND >::FACTOR : VK_BLEND_FACTOR_ZERO;

    static const VkBlendOp BLEND_OP = VK_BLEND_OP_ADD;

    template< class OP2 >
    VPP_INLINE TBlendAdd< TBlendMul< DEST, OPERAND >, OP2 > operator+ ( OP2 ) const
    {
        return TBlendAdd< TBlendMul< DEST, OPERAND >, OP2 >();
    }

    template< class OP2 >
    VPP_INLINE TBlendSub< TBlendMul< DEST, OPERAND >, OP2 > operator- ( OP2 ) const
    {
        return TBlendSub< TBlendMul< DEST, OPERAND >, OP2 >();
    }
};

// -----------------------------------------------------------------------------

template< typename OPERAND >
VPP_INLINE TBlendMul< false, OPERAND > operator* ( ECS, OPERAND )
{
    return TBlendMul< false, OPERAND >();
}

// -----------------------------------------------------------------------------

template< typename OPERAND >
VPP_INLINE TBlendMul< true, OPERAND > operator* ( ECD, OPERAND )
{
    return TBlendMul< true, OPERAND >();
}

// -----------------------------------------------------------------------------

template< typename OPERAND >
VPP_INLINE TBlendMul< false, OPERAND > operator* ( OPERAND, ECS )
{
    return TBlendMul< false, OPERAND >();
}

// -----------------------------------------------------------------------------

template< typename OPERAND >
VPP_INLINE TBlendMul< true, OPERAND > operator* ( OPERAND, ECD )
{
    return TBlendMul< true, OPERAND >();
}

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMul< false, ECS > operator* ( ECS, ECS )
{
    return TBlendMul< false, ECS >();
}

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMul< true, ECS > operator* ( ECS, ECD )
{
    return TBlendMul< true, ECS >();
}

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMul< true, ECD > operator* ( ECD, ECD )
{
    return TBlendMul< true, ECD >();
}

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMul< false, ECD > operator* ( ECD, ECS )
{
    return TBlendMul< false, ECD >();
}

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMul< false, ECD > operator* ( EASATN, ECS )
{
    return TBlendMul< false, ECD >();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename TermT >
struct TOperatorTraits
{
    static const bool VALID = false;
};

// -----------------------------------------------------------------------------

template< typename OPERAND >
struct TOperatorTraits< TBlendMul< false, OPERAND > >
{
    static const VkBlendFactor FACTOR = TOperandTraits< OPERAND >::FACTOR;
    static const bool DEST = false;
    static const bool SIMPLE = false;
    static const bool VALID = true;
};

// -----------------------------------------------------------------------------

template< typename OPERAND >
struct TOperatorTraits< TBlendMul< true, OPERAND > >
{
    static const VkBlendFactor FACTOR = TOperandTraits< OPERAND >::FACTOR;
    static const bool DEST = true;
    static const bool SIMPLE = false;
    static const bool VALID = true;
};

// -----------------------------------------------------------------------------

template<>
struct TOperatorTraits< ECS >
{
    static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE;
    static const bool DEST = false;
    static const bool SIMPLE = true;
    static const bool VALID = true;
};

// -----------------------------------------------------------------------------

template<>
struct TOperatorTraits< ECD >
{
    static const VkBlendFactor FACTOR = VK_BLEND_FACTOR_ONE;
    static const bool DEST = true;
    static const bool SIMPLE = true;
    static const bool VALID = true;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename OP1, typename OP2 >
struct TBlendAdd
{
    static const VkBlendFactor SOURCE_FACTOR =
        TOperatorTraits< OP1 >::DEST ? TOperatorTraits< OP2 >::FACTOR : TOperatorTraits< OP1 >::FACTOR;

    static const VkBlendFactor DEST_FACTOR =
        TOperatorTraits< OP1 >::DEST ? TOperatorTraits< OP1 >::FACTOR : TOperatorTraits< OP2 >::FACTOR;

    static const VkBlendOp BLEND_OP = VK_BLEND_OP_ADD;

    static_assert (
        TOperatorTraits< OP1 >::DEST != TOperatorTraits< OP2 >::DEST,
        "Source or dest operand can't be used twice in blending equation" );
};

// -----------------------------------------------------------------------------

template< typename OP2 >
VPP_INLINE TBlendAdd< TBlendMul< false, EC1 >, OP2 > operator+ ( ECS, OP2 )
{
    return TBlendAdd< TBlendMul< false, EC1 >, OP2 >();
}

// -----------------------------------------------------------------------------

template< typename OP2 >
VPP_INLINE TBlendAdd< TBlendMul< true, EC1 >, OP2 > operator+ ( ECD, OP2 )
{
    return TBlendAdd< TBlendMul< true, EC1 >, OP2 >();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename OP1, typename OP2 >
struct TBlendSub
{
    static const VkBlendFactor SOURCE_FACTOR =
        TOperatorTraits< OP1 >::DEST ? TOperatorTraits< OP2 >::FACTOR : TOperatorTraits< OP1 >::FACTOR;

    static const VkBlendFactor DEST_FACTOR =
        TOperatorTraits< OP1 >::DEST ? TOperatorTraits< OP1 >::FACTOR : TOperatorTraits< OP2 >::FACTOR;

    static const VkBlendOp BLEND_OP =
        TOperatorTraits< OP1 >::DEST ? VK_BLEND_OP_REVERSE_SUBTRACT : VK_BLEND_OP_SUBTRACT;

    static_assert (
        TOperatorTraits< OP1 >::DEST != TOperatorTraits< OP2 >::DEST,
        "Source or dest operand can't be used twice in blending equation" );
};

// -----------------------------------------------------------------------------

template< typename OP2 >
VPP_INLINE TBlendSub< TBlendMul< false, EC1 >, OP2 > operator- ( ECS, OP2 )
{
    return TBlendSub< TBlendMul< false, EC1 >, OP2 >();
}

// -----------------------------------------------------------------------------

template< typename OP2 >
VPP_INLINE TBlendSub< TBlendMul< true, EC1 >, OP2 > operator- ( ECD, OP2 )
{
    return TBlendSub< TBlendMul< true, EC1 >, OP2 >();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename OP1, typename OP2 >
struct TBlendMin
{
    static_assert (
        TOperatorTraits< OP1 >::FACTOR == VK_BLEND_FACTOR_ONE
        && TOperatorTraits< OP2 >::FACTOR == VK_BLEND_FACTOR_ONE,
        "For min and max blend operations, multiplication is not supported" );

    static const VkBlendFactor SOURCE_FACTOR = VK_BLEND_FACTOR_ONE;
    static const VkBlendFactor DEST_FACTOR = VK_BLEND_FACTOR_ONE;
    static const VkBlendOp BLEND_OP = VK_BLEND_OP_MIN;
};

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMin< ECS, ECD > BlendMin ( ECS, ECD )
{
    return TBlendMin< ECS, ECD >();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename OP1, typename OP2 >
struct TBlendMax
{
    static_assert (
        TOperatorTraits< OP1 >::FACTOR == VK_BLEND_FACTOR_ONE
        && TOperatorTraits< OP2 >::FACTOR == VK_BLEND_FACTOR_ONE,
        "For min and max blend operations, multiplication is not supported" );

    static const VkBlendFactor SOURCE_FACTOR = VK_BLEND_FACTOR_ONE;
    static const VkBlendFactor DEST_FACTOR = VK_BLEND_FACTOR_ONE;
    static const VkBlendOp BLEND_OP = VK_BLEND_OP_MAX;
};

// -----------------------------------------------------------------------------

VPP_INLINE TBlendMax< ECS, ECD > BlendMax ( ECS, ECD )
{
    return TBlendMax< ECS, ECD >();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< typename Equation > struct TEquationCheck { static const bool isValid = true; };
template<> struct TEquationCheck< EC1 > { static const bool isValid = false; };
template<> struct TEquationCheck< ECC > { static const bool isValid = false; };
template<> struct TEquationCheck< EAS > { static const bool isValid = false; };
template<> struct TEquationCheck< EAD > { static const bool isValid = false; };
template<> struct TEquationCheck< EAC > { static const bool isValid = false; };
template<> struct TEquationCheck< EASAT > { static const bool isValid = false; };
template<> struct TEquationCheck< EASATN > { static const bool isValid = false; };
template<> struct TEquationCheck< ECS1 > { static const bool isValid = false; };
template<> struct TEquationCheck< EAS1 > { static const bool isValid = false; };

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct BlendOperation
{
    template< typename Equation >
    VPP_INLINE BlendOperation ( const Equation eq )
    {
        static_assert (
            TEquationCheck< Equation >::isValid,
            "Invalid blending expression" );

        d_sourceFactor = Equation::SOURCE_FACTOR;
        d_destFactor = Equation::DEST_FACTOR;
        d_operation = Equation::BLEND_OP;
    };

    VPP_INLINE BlendOperation ( ECS )
    {
        d_sourceFactor = VK_BLEND_FACTOR_ONE;
        d_destFactor = VK_BLEND_FACTOR_ZERO;
        d_operation = VK_BLEND_OP_ADD;
    }

    VPP_INLINE BlendOperation ( ECD )
    {
        d_sourceFactor = VK_BLEND_FACTOR_ZERO;
        d_destFactor = VK_BLEND_FACTOR_ONE;
        d_operation = VK_BLEND_OP_ADD;
    }

    VPP_INLINE BlendOperation ( EC0 )
    {
        d_sourceFactor = VK_BLEND_FACTOR_ZERO;
        d_destFactor = VK_BLEND_FACTOR_ZERO;
        d_operation = VK_BLEND_OP_ADD;
    }

    VkBlendFactor d_sourceFactor;
    VkBlendFactor d_destFactor;
    VkBlendOp d_operation;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum class write
{
    none = 0,
    r = VK_COLOR_COMPONENT_R_BIT,
    g = VK_COLOR_COMPONENT_G_BIT,
    b = VK_COLOR_COMPONENT_B_BIT,
    a = VK_COLOR_COMPONENT_A_BIT,
    rg = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
    rb = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT,
    ra = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT,
    gb = VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
    ga = VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT,
    ba = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    rgb = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
    rga = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT,
    rba = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    gba = VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    rgba = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class BlendConfig : public VkPipelineColorBlendAttachmentState
{
public:
    BlendConfig ( write wr = write::rgba );
    BlendConfig ( const BlendOperation& op, write wr = write::rgba );
    BlendConfig ( const BlendOperation& colorOp, const BlendOperation& alphaOp, write wr = write::rgba );
};

// -----------------------------------------------------------------------------

VPP_INLINE BlendConfig :: BlendConfig ( write wr )
{
    blendEnable = false;
    srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendOp = VK_BLEND_OP_ADD;
    srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    alphaBlendOp = VK_BLEND_OP_ADD;
    colorWriteMask = static_cast< VkColorComponentFlags >( wr );
}

// -----------------------------------------------------------------------------

VPP_INLINE BlendConfig :: BlendConfig ( const BlendOperation& op, write wr )
{
    blendEnable = true;
    srcColorBlendFactor = op.d_sourceFactor;
    dstColorBlendFactor = op.d_destFactor;
    colorBlendOp = op.d_operation;
    srcAlphaBlendFactor = op.d_sourceFactor;
    dstAlphaBlendFactor = op.d_destFactor;
    alphaBlendOp = op.d_operation;
    colorWriteMask = static_cast< VkColorComponentFlags >( wr );
}

// -----------------------------------------------------------------------------

VPP_INLINE BlendConfig :: BlendConfig (
    const BlendOperation& colorOp, const BlendOperation& alphaOp, write wr )
{
    blendEnable = true;
    srcColorBlendFactor = colorOp.d_sourceFactor;
    dstColorBlendFactor = colorOp.d_destFactor;
    colorBlendOp = colorOp.d_operation;
    srcAlphaBlendFactor = alphaOp.d_sourceFactor;
    dstAlphaBlendFactor = alphaOp.d_destFactor;
    alphaBlendOp = alphaOp.d_operation;
    colorWriteMask = static_cast< VkColorComponentFlags >( wr );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPBLENDOPERATORS_HPP
