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

#include "ph.hpp"
#include "../include/vppLangBuiltins.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace var {
// -----------------------------------------------------------------------------

inWorkgroupSize :: operator rvalue_type() const
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    if ( ! d_id )
    {
        std::vector< spv::Id > comps;
        comps.reserve ( 3 );
        comps.push_back ( pTranslator->makeIntConstant ( d_localSize.x ) );
        comps.push_back ( pTranslator->makeIntConstant ( d_localSize.y ) );
        comps.push_back ( pTranslator->makeIntConstant ( d_localSize.z ) );

        d_id = KId ( pTranslator->makeCompositeConstant (
            IVec3::getType(), comps ) );

        pTranslator->addDecoration (
            d_id, spv::DecorationBuiltIn, spv::BuiltInWorkgroupSize );
    }

    return IVec3 ( d_id );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

KId KVertexDataBlockBase :: getArrayType() const
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    return KId (
        pTranslator->makeArrayType ( 
            getSingleType(), pTranslator->makeIntConstant ( d_size ), 0 ) );
}

// -----------------------------------------------------------------------------

KId KVertexDataBlockBase :: getSingleType() const
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();
    const KId floatType = Float::getType();

    const unsigned int nMembers = 2 + ( d_clipPlanes != 0 ) + ( d_cullPlanes != 0 );

    std::vector< spv::Id > structMembers ( nMembers );
    unsigned int iMember = 0;

    structMembers [ iMember++ ] = Vec4::getType();
    structMembers [ iMember++ ] = floatType;

    if ( d_clipPlanes != 0 )
    {
        structMembers [ iMember++ ] = pTranslator->makeArrayType ( 
            floatType, pTranslator->makeIntConstant ( d_clipPlanes ), 0 );

        pTranslator->addCapability ( spv::CapabilityClipDistance );
    }

    if ( d_cullPlanes != 0 )
    {
        structMembers [ iMember++ ] = pTranslator->makeArrayType ( 
            floatType, pTranslator->makeIntConstant ( d_cullPlanes ), 0 );

        pTranslator->addCapability ( spv::CapabilityCullDistance );
    }

    const spv::Id structType =
        pTranslator->makeStructType ( structMembers, "gl_PerVertex" );

    pTranslator->addDecoration ( structType, spv::DecorationBlock );
    iMember = 0;

    pTranslator->addMemberDecoration ( structType, iMember, spv::DecorationBuiltIn, spv::BuiltInPosition );
    pTranslator->addMemberName ( structType, iMember++, "gl_Position" );

    pTranslator->addMemberDecoration ( structType, iMember, spv::DecorationBuiltIn, spv::BuiltInPointSize );
    pTranslator->addMemberName ( structType, iMember++, "gl_PointSize" );

    if ( d_clipPlanes != 0 )
    {
        pTranslator->addMemberDecoration ( structType, iMember, spv::DecorationBuiltIn, spv::BuiltInClipDistance );
        pTranslator->addMemberName ( structType, iMember++, "gl_ClipDistance" );
    }

    if ( d_cullPlanes != 0 )
    {
        pTranslator->addMemberDecoration ( structType, iMember, spv::DecorationBuiltIn, spv::BuiltInCullDistance );
        pTranslator->addMemberName ( structType, iMember++, "gl_CullDistance" );
    }

    return KId ( structType );
}

// -----------------------------------------------------------------------------
} // namespace var
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
