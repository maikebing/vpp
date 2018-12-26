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

#ifndef INC_VPPCOMPUTEPASS_HPP
#define INC_VPPCOMPUTEPASS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

#ifndef INC_VPPPIPELINELAYOUT_HPP
#include "vppPipelineLayout.hpp"
#endif

#ifndef INC_VPPPIPELINECACHE_HPP
#include "vppPipelineCache.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class ComputePassImpl;

// -----------------------------------------------------------------------------

class ComputePass :
    public TSharedReference< ComputePassImpl >,
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    ComputePass (
        const Device& hDevice );

    ComputePass (
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    ~ComputePass();

    typedef std::function< void () > FCommand;
    typedef std::vector< FCommand > Commands;

    const Device& device() const;
    const PipelineCache& pipelineCache() const;

    VPP_DLLAPI const ComputePipeline& pipeline ( std::uint32_t iPipeline ) const;

    template< class DefinitionT >
    std::uint32_t addPipeline ( const ComputePipelineLayout< DefinitionT> layout );

    VPP_DLLAPI void operator<< ( const std::function< void () >& cmds );

    const Commands& getCommands() const;

    VPP_DLLAPI static void cmdDispatch (
        std::uint32_t x = 1, std::uint32_t y = 1, std::uint32_t z = 1,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdDispatchIndirect (
        const IndirectBufferView& data,
        unsigned int index,
        CommandBuffer hCmdBuffer = CommandBuffer() );

public:
    void beginComputing();
    void endComputing();
    void createPipelines();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ComputePassImpl : public TSharedObject< ComputePassImpl >
{
public:
    VPP_DLLAPI ComputePassImpl (
        const Device& hDevice,
        const PipelineCache& hPipelineCache );

    VPP_DLLAPI ~ComputePassImpl();

    VPP_INLINE bool compareObjects ( const ComputePassImpl* pRHS ) const
    {
        return this < pRHS;
    }

    VPP_DLLAPI void createPipelines();
    VPP_DLLAPI void preparePipelineCreateInfo ( size_t iPipeline );
    
private:
    friend class ComputePass;
    Device d_hDevice;
    PipelineCache d_hPipelineCache;

    struct SPipelineData
    {
        SPipelineData();

        std::uint32_t d_pipelineIndex;

        PipelineLayoutBase d_layout;
        ComputePipeline d_pipeline;
    };

    ComputePipelineCreateInfos d_pipelineCreateInfos;
    std::vector< SPipelineData > d_pipelineData;

    std::vector< std::function< void () > > d_commands;

    bool d_bPipelinesCreated;

private:
    VPP_DLLAPI std::uint32_t createPipelineData ( const PipelineLayoutBase& lt );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE ComputePassImpl::SPipelineData :: SPipelineData() :
    d_pipelineIndex ( 0 )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE ComputePass :: ComputePass (
    const Device& hDevice ) :
        TSharedReference< ComputePassImpl >(
            new ComputePassImpl ( hDevice, hDevice.defaultPipelineCache() ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputePass :: ComputePass (
    const Device& hDevice,
    const PipelineCache& hPipelineCache ) :
        TSharedReference< ComputePassImpl >(
            new ComputePassImpl ( hDevice, hPipelineCache ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE ComputePass :: ~ComputePass()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& ComputePass :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineCache& ComputePass :: pipelineCache() const
{
    return get()->d_hPipelineCache;
}

// -----------------------------------------------------------------------------

template< class DefinitionT >
VPP_INLINE std::uint32_t ComputePass :: addPipeline (
    const ComputePipelineLayout< DefinitionT> layout )
{
    return get()->createPipelineData ( layout );
}

// -----------------------------------------------------------------------------

VPP_INLINE const ComputePass::Commands& ComputePass :: getCommands() const
{
    return get()->d_commands;
}

// -----------------------------------------------------------------------------

VPP_INLINE void ComputePass :: beginComputing()
{
    get()->createPipelines();
}

// -----------------------------------------------------------------------------

VPP_INLINE void ComputePass :: endComputing()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE void ComputePass :: createPipelines()
{
    get()->createPipelines();
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPCOMPUTEPASS_HPP
