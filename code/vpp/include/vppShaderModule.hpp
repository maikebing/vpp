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

#ifndef INC_VPPSHADERMODULE_HPP
#define INC_VPPSHADERMODULE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KShaderModuleImpl;

// -----------------------------------------------------------------------------

class KShaderModule : public TSharedReference< KShaderModuleImpl >
{
public:
    KShaderModule();

    KShaderModule (
        const uint32_t* pCode,
        size_t size,
        Device hDevice );

    VkShaderModule handle() const;
    Device device() const;
};

// -----------------------------------------------------------------------------

class KShaderModuleImpl : public TSharedObject< KShaderModuleImpl >
{
public:
    KShaderModuleImpl (
        const uint32_t* pCode,
        size_t size,
        Device hDevice );

    ~KShaderModuleImpl();

    VPP_INLINE bool compareObjects ( const KShaderModuleImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class KShaderModule;
    Device d_hDevice;
    VkResult d_result;
    VkShaderModule d_handle;
};

// -----------------------------------------------------------------------------

VPP_INLINE KShaderModuleImpl :: KShaderModuleImpl (
    const uint32_t* pCode,
    size_t size,
    Device hDevice ) :
        d_hDevice ( hDevice ),
        d_result(),
        d_handle()
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo;
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.pNext = 0;
    shaderModuleCreateInfo.flags = 0;
    shaderModuleCreateInfo.codeSize = static_cast< std::uint32_t >( size );
    shaderModuleCreateInfo.pCode = pCode;

    d_result = ::vkCreateShaderModule (
        d_hDevice.handle(), & shaderModuleCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderModuleImpl :: ~KShaderModuleImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyShaderModule ( d_hDevice.handle(), d_handle, 0 );
}


// -----------------------------------------------------------------------------

VPP_INLINE KShaderModule :: KShaderModule()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderModule :: KShaderModule (
    const uint32_t* pCode,
    size_t size,
    Device hDevice ) :
        TSharedReference< KShaderModuleImpl >(
            new KShaderModuleImpl ( pCode, size, hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkShaderModule KShaderModule :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE Device KShaderModule :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHADERMODULE_HPP
