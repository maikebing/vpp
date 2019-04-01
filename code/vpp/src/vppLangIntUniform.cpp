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
#include "../include/vppLangIntUniform.hpp"
#include "../include/vppExceptions.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

KUniformAccess :: KUniformAccess ( spv::Decoration decoration )
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        pTranslator->setStructDecoration ( decoration );
    }
    else
        throw XUsageError ( "UniformVar can be used only inside VPP shader code." );
}

// -----------------------------------------------------------------------------

KId KUniformAccess :: init (
    spv::Decoration decoration,
    spv::StorageClass storageClass,
    std::uint32_t set,
    std::uint32_t binding )
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        const bool bAddBlockDecoration = ! structInfo.d_bDefined;

        pTranslator->popStructType();

        if ( bAddBlockDecoration )
            pTranslator->addDecoration ( structInfo.d_typeId, decoration );

        return pTranslator->registerUniformBuffer ( structInfo.d_typeId, set, binding, storageClass );
    }
    else
        throw XUsageError ( "UniformVar can be used only inside VPP shader code." );
}

// -----------------------------------------------------------------------------

std::pair< KId, KId > KUniformAccess :: init (
    spv::Decoration decoration,
    spv::StorageClass storageClass,
    std::uint32_t set,
    std::uint32_t binding,
    std::uint32_t count )
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        const bool bAddDecorations = ! structInfo.d_bDefined;

        pTranslator->popStructType();

        const KId arrayType ( pTranslator->makeArrayType ( 
            structInfo.d_typeId, pTranslator->makeIntConstant ( count ), 0 ) );

        if ( bAddDecorations )
            pTranslator->addDecoration ( structInfo.d_typeId, decoration );

        const KId varId =
            pTranslator->registerUniformBuffer ( arrayType, set, binding, storageClass );

        return std::make_pair ( varId, arrayType );
    }
    else
        throw XUsageError ( "UniformVar can be used only inside VPP shader code." );
}

// -----------------------------------------------------------------------------

std::pair< KId, KId > KUniformAccess :: init (
    spv::Decoration decoration,
    spv::StorageClass storageClass,
    std::uint32_t set,
    std::uint32_t binding,
    std::uint32_t arraySize,
    std::uint32_t stride )
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        KShaderTranslator::SStructInfo& structInfo = pTranslator->currentStructType();
        const bool bAddDecorations = ! structInfo.d_bDefined;

        pTranslator->popStructType();

        const KId arrayType = KId (
            arraySize > 0 ?
                pTranslator->makeArrayType ( 
                    structInfo.d_typeId,
                    pTranslator->makeIntConstant ( arraySize ),
                    stride 
                )
                :
                pTranslator->makeRuntimeArray ( structInfo.d_typeId )
        );

        std::vector< spv::Id > memberTypes ( 1, arrayType );

        std::ostringstream sstName;

        sstName << "UniformArray_"
            << set << "_" << binding << "_" << arraySize << "_" << stride;

        const std::string& name = sstName.str();

        const KId externStructId = KId (
            pTranslator->makeStructType ( memberTypes, name.c_str() ) );

        if ( bAddDecorations )
        {
            pTranslator->addDecoration ( externStructId, decoration );

            pTranslator->addDecoration (
                arrayType, spv::DecorationArrayStride, stride );
        }

        const KId varId =
            pTranslator->registerUniformBuffer ( externStructId, set, binding, storageClass );

        return std::make_pair ( varId, externStructId );
    }
    else
        throw XUsageError ( "UniformArray can be used only inside VPP shader code." );
}

// -----------------------------------------------------------------------------

std::pair< KId, KId > KUniformAccess :: init (
    const KId& itemTypeId,
    spv::Decoration decoration,
    spv::StorageClass storageClass,
    std::uint32_t set,
    std::uint32_t binding,
    std::uint32_t arraySize,
    std::uint32_t stride )
{
    if ( KShaderTranslator* pTranslator = KShaderTranslator::get() )
    {
        const KId arrayType = KId (
            arraySize > 0 ?
                pTranslator->makeArrayType ( 
                    itemTypeId,
                    pTranslator->makeIntConstant ( arraySize ),
                    stride 
                )
                :
                pTranslator->makeRuntimeArray ( itemTypeId )
        );

        std::vector< spv::Id > memberTypes ( 1, arrayType );

        std::ostringstream sstName;

        sstName << "UniformArray_"
            << set << "_" << binding << "_" << arraySize << "_" << stride;

        const std::string& name = sstName.str();

        const KId externStructId = KId (
            pTranslator->makeStructType ( memberTypes, name.c_str() ) );

        pTranslator->addDecoration ( externStructId, decoration );

        pTranslator->addDecoration (
            arrayType, spv::DecorationArrayStride, stride );

        pTranslator->addMemberDecoration ( externStructId, 0, spv::DecorationOffset, 0 );

        const KId varId =
            pTranslator->registerUniformBuffer ( externStructId, set, binding, storageClass );

        return std::make_pair ( varId, externStructId );
    }
    else
        throw XUsageError ( "UniformSimpleArray can be used only inside VPP shader code." );
}

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
