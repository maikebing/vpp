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
#include "../include/vppLangVectorTypes.hpp"

#ifndef INC_VPPSPIRVGLSLSTD450_HPP
#include "../include/vppGLSLstd450.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

PHVec4 :: operator Vec4() const
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    pTranslator->clearAccessChain();
    pTranslator->setAccessChainRValue ( id() );
    pTranslator->accessChainPush ( pTranslator->getArrayIndex ( 0 ) );

    const Int item0 ( KId (
        pTranslator->accessChainLoad (
            pTranslator->getPrecision(), Int::getType() ) ) );

    pTranslator->clearAccessChain();
    pTranslator->setAccessChainRValue ( id() );
    pTranslator->accessChainPush ( pTranslator->getArrayIndex ( 1 ) );

    const Int item1 ( KId (
        pTranslator->accessChainLoad (
            pTranslator->getPrecision(), Int::getType() ) ) );

    std::vector< spv::Id > args0 ( 1 );
    std::vector< spv::Id > args1 ( 1 );
    args0 [ 0 ] = item0.id();
    args1 [ 0 ] = item1.id();

    const Vec2 result0 ( KId (
        pTranslator->createBuiltinCall (
            Vec2::getType(), pTranslator->getBuiltinFunctions(),
            GLSLstd450UnpackHalf2x16, args0 ) ) );

    const Vec2 result1 ( KId (
        pTranslator->createBuiltinCall (
            Vec2::getType(), pTranslator->getBuiltinFunctions(),
            GLSLstd450UnpackHalf2x16, args1 ) ) );

    return Vec4 ( result0, result1 );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
