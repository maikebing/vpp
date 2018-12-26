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

#ifndef INC_VPPPIPELINECACHE_HPP
#define INC_VPPPIPELINECACHE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINE_HPP
#include "vppPipeline.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KPipelineCacheImpl;

// -----------------------------------------------------------------------------

class PipelineCache : public TSharedReference< KPipelineCacheImpl >
{
public:
    PipelineCache ( const Device& hDevice );

    VkPipelineCache handle() const;
};

// -----------------------------------------------------------------------------

class KPipelineCacheImpl : public TSharedObject< KPipelineCacheImpl >
{
public:
    KPipelineCacheImpl ( const Device& hDevice );
    ~KPipelineCacheImpl();

private:
    friend class PipelineCache;
    Device d_hDevice;
    VkPipelineCache d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE KPipelineCacheImpl :: KPipelineCacheImpl ( const Device& hDevice ) :
    d_hDevice ( hDevice ),
    d_handle(),
    d_result()
{
    VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
    vkPipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vkPipelineCacheCreateInfo.pNext = 0;
    vkPipelineCacheCreateInfo.flags = 0;
    vkPipelineCacheCreateInfo.initialDataSize = 0;
    vkPipelineCacheCreateInfo.pInitialData = 0;

    d_result = ::vkCreatePipelineCache (
        d_hDevice.handle(), & vkPipelineCacheCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KPipelineCacheImpl :: ~KPipelineCacheImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyPipelineCache ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE PipelineCache :: PipelineCache ( const Device& hDevice ) :
    TSharedReference< KPipelineCacheImpl >( new KPipelineCacheImpl ( hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPipelineCache PipelineCache :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPIPELINECACHE_HPP
