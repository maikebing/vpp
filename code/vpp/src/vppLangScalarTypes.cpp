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
#include "../include/vppLangScalarTypes.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

// Rules are in spec Appendix A

// -----------------------------------------------------------------------------

KPointerBase :: KPointerBase ( spv::StorageClass storageClass ) :
    d_storageClass ( storageClass )
{
    switch ( d_storageClass )
    {
        case spv::StorageClassWorkgroup:
        case spv::StorageClassUniform:
        case spv::StorageClassImage:
            break;

        case spv::StorageClassStorageBuffer:
            KShaderTranslator::get()->requireVersion11();
            break;

        default:
            raiseUsageError ( "Only images, uniform/storage buffers and shared variables can have atomic pointers" );
            break;
    }
}

// -----------------------------------------------------------------------------

spv::Scope KPointerBase :: getScope() const
{
    if ( d_storageClass == spv::StorageClassWorkgroup )
        return spv::ScopeWorkgroup;
    else
        return spv::ScopeDevice;
}

// -----------------------------------------------------------------------------

spv::MemorySemanticsMask KPointerBase :: getSemanticsMask ( spv::Op op ) const
{
    spv::MemorySemanticsMask memoryMask;

    switch ( d_storageClass )
    {
        case spv::StorageClassWorkgroup:
            memoryMask = spv::MemorySemanticsWorkgroupMemoryMask;
            break;

        case spv::StorageClassImage:
            memoryMask = spv::MemorySemanticsImageMemoryMask;
            break;

        default:
            memoryMask = spv::MemorySemanticsUniformMemoryMask;
            break;
    }

    if ( op == spv::OpAtomicLoad || op == spv::OpAtomicStore )
        return spv::MemorySemanticsMaskNone | memoryMask;
    else
        return spv::MemorySemanticsAcquireReleaseMask | memoryMask;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
