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

#ifndef INC_VPPPIPELINE_HPP
#define INC_VPPPIPELINE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class PipelineImpl;

// -----------------------------------------------------------------------------

class PipelineBase : public TSharedReference< PipelineImpl >
{
public:
    PipelineBase();
    PipelineBase ( VkPipeline hPipeline, const Device& hDevice );

    VkPipeline handle() const;
};

// -----------------------------------------------------------------------------

class Pipeline : public PipelineBase
{
public:
    Pipeline();
    Pipeline ( VkPipeline hPipeline, const Device& hDevice );

    void cmdBind ( CommandBuffer hCmdBuffer = CommandBuffer() ) const;
};

// -----------------------------------------------------------------------------

class ComputePipeline : public PipelineBase
{
public:
    ComputePipeline();
    ComputePipeline ( VkPipeline hPipeline, const Device& hDevice );

    void cmdBind ( CommandBuffer hCmdBuffer  = CommandBuffer() ) const;
};

// -----------------------------------------------------------------------------

class PipelineImpl : public TSharedObject< PipelineImpl >
{
public:
    PipelineImpl ( VkPipeline hPipeline, const Device& hDevice );
    ~PipelineImpl();

    VPP_INLINE bool compareObjects ( const PipelineImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class PipelineBase;
    friend class Pipeline;
    friend class ComputePipeline;

    Device d_hDevice;
    VkPipeline d_handle;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE PipelineImpl :: PipelineImpl ( VkPipeline hPipeline, const Device& hDevice ) :
    d_hDevice ( hDevice ),
    d_handle ( hPipeline )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE PipelineImpl :: ~PipelineImpl()
{
    ::vkDestroyPipeline ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE PipelineBase :: PipelineBase()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE PipelineBase :: PipelineBase ( VkPipeline hPipeline, const Device& hDevice ) :
    TSharedReference< PipelineImpl >( new PipelineImpl ( hPipeline, hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkPipeline PipelineBase :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Pipeline :: Pipeline()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Pipeline :: Pipeline ( VkPipeline hPipeline, const Device& hDevice ) :
    PipelineBase ( hPipeline, hDevice )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE void Pipeline :: cmdBind ( CommandBuffer hCommandBuffer ) const
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindPipeline (
        hCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, get()->d_handle );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE ComputePipeline :: ComputePipeline()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputePipeline :: ComputePipeline ( VkPipeline hPipeline, const Device& hDevice ) :
    PipelineBase ( hPipeline, hDevice )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE void ComputePipeline :: cmdBind ( CommandBuffer hCommandBuffer ) const
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindPipeline (
        hCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, get()->d_handle );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPIPELINE_HPP
