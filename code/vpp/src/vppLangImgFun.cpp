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

#include "ph.hpp"
#include "../include/vppLangImgFun.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace impl {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

spv::Id makeOffsetArray ( const GatherOffsets& offsets )
{
    KShaderTranslator* pTranslator = KShaderTranslator::get();

    const spv::Id intType = pTranslator->makeIntType ( 32 );
    const spv::Id vecType2 = pTranslator->makeVectorType ( intType, 2 );
    const spv::Id vecType4 = pTranslator->makeVectorType ( vecType2, 4 );

    const spv::Id id00 = pTranslator->makeIntConstant ( offsets.offsets [ 0 ][ 0 ] );
    const spv::Id id01 = pTranslator->makeIntConstant ( offsets.offsets [ 0 ][ 1 ] );
    const spv::Id id10 = pTranslator->makeIntConstant ( offsets.offsets [ 1 ][ 0 ] );
    const spv::Id id11 = pTranslator->makeIntConstant ( offsets.offsets [ 1 ][ 1 ] );
    const spv::Id id20 = pTranslator->makeIntConstant ( offsets.offsets [ 2 ][ 0 ] );
    const spv::Id id21 = pTranslator->makeIntConstant ( offsets.offsets [ 2 ][ 1 ] );
    const spv::Id id30 = pTranslator->makeIntConstant ( offsets.offsets [ 3 ][ 0 ] );
    const spv::Id id31 = pTranslator->makeIntConstant ( offsets.offsets [ 3 ][ 1 ] );

    std::vector< spv::Id > components;
    components.reserve ( 4 );
    components.resize ( 2 );

    components [ 0 ] = id00;
    components [ 1 ] = id01;
    const spv::Id id0 = pTranslator->makeCompositeConstant ( vecType2, components );

    components [ 0 ] = id10;
    components [ 1 ] = id11;
    const spv::Id id1 = pTranslator->makeCompositeConstant ( vecType2, components );

    components [ 0 ] = id20;
    components [ 1 ] = id21;
    const spv::Id id2 = pTranslator->makeCompositeConstant ( vecType2, components );

    components [ 0 ] = id30;
    components [ 1 ] = id31;
    const spv::Id id3 = pTranslator->makeCompositeConstant ( vecType2, components );

    components.resize ( 4 );
    components [ 0 ] = id0;
    components [ 1 ] = id1;
    components [ 2 ] = id2;
    components [ 3 ] = id3;

    return pTranslator->makeCompositeConstant ( vecType4, components );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace impl
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
