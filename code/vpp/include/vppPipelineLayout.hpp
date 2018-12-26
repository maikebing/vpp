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

#ifndef INC_VPPPIPELINELAYOUT_HPP
#define INC_VPPPIPELINELAYOUT_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINECONFIG_HPP
#include "vppPipelineConfig.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
    
class KPipelineLayoutImpl;

// -----------------------------------------------------------------------------

class PipelineLayoutBase : public TSharedReference< KPipelineLayoutImpl >
{
public:
    PipelineLayoutBase();

    VkPipelineLayout handle() const;
    const Device& device() const;

    bool isComputePipeline() const;

    const PipelineConfig& config() const;

    size_t getDescriptorSetCount() const;
    const DescriptorSetLayoutHandles& getDescriptorSetLayoutHandles() const;
    const DescriptorPoolSizes& getDescriptorPoolSizes() const;
    const PipelineConfig::ShaderTable& getShaderTable() const;

protected:
    PipelineLayoutBase ( KPipelineLayoutImpl* pImpl, bool bComputePipeline );

private:
    bool d_bComputePipeline;
};

// -----------------------------------------------------------------------------

class KPipelineLayoutImpl : public TSharedObject< KPipelineLayoutImpl >
{
public:
    KPipelineLayoutImpl ( const Device& hDevice, const PipelineConfig* pConfig );
    virtual ~KPipelineLayoutImpl();

    VPP_INLINE bool compareObjects ( const KPipelineLayoutImpl* pRHS ) const
    {
        return this < pRHS;
    }

protected:
    friend class PipelineLayoutBase;
    Device d_hDevice;
    VkPipelineLayout d_handle;
    VkResult d_result;

    const PipelineConfig* d_pConfig;
    
    typedef std::vector< detail::KDescriptorSetLayout > DescriptorSetLayouts;

    DescriptorSetLayouts d_descriptorSetLayouts;
    DescriptorSetLayoutHandles d_descriptorSetLayoutHandles;
    DescriptorPoolSizes d_descriptorPoolSizes;

    PipelineConfig::ShaderTable d_shaderTable;
};

// -----------------------------------------------------------------------------

VPP_INLINE KPipelineLayoutImpl :: KPipelineLayoutImpl (
    const Device& hDevice, const PipelineConfig* pConfig ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result(),
        d_pConfig ( pConfig )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KPipelineLayoutImpl :: ~KPipelineLayoutImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyPipelineLayout ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

template< class DefinitionT >
class TPipelineLayoutImpl :
    public KPipelineLayoutImpl,
    public DefinitionT
{
public:
    // For graphic pipelines.
    template< typename... Args >
    TPipelineLayoutImpl ( const Process& hProcess, const Device& hDevice, Args... args );

    // For compute pipelines.
    template< typename... Args >
    TPipelineLayoutImpl ( const Device& hDevice, Args... args );

    virtual ~TPipelineLayoutImpl() {}

    virtual VkPipelineLayout getCompiledHandle() const
    {
        return d_handle;
    }

private:
    void initialize ( const Device& hDevice );
};

// -----------------------------------------------------------------------------

template< class DefinitionT >
class TDefinitionRef
{
public:
    VPP_INLINE TDefinitionRef ( TPipelineLayoutImpl< DefinitionT >* pDefinition ) :
        d_pDefinition ( pDefinition )
    {}

    VPP_INLINE DefinitionT& definition()
    {
        return *d_pDefinition;
    }

    VPP_INLINE const DefinitionT& definition() const
    {
        return *d_pDefinition;
    }

    VPP_INLINE TPipelineLayoutImpl< DefinitionT >* internalDefinition()
    {
        return d_pDefinition;
    }

private:
    TPipelineLayoutImpl< DefinitionT >* d_pDefinition;
};

// -----------------------------------------------------------------------------

template< class DefinitionT >
class PipelineLayout :
    public TDefinitionRef< DefinitionT >,
    public PipelineLayoutBase
{
public:
    template< typename... Args >
    PipelineLayout ( const Process& hProcess, const Device& hDevice, Args... args );
};

// -----------------------------------------------------------------------------

template< class DefinitionT >
class ComputePipelineLayout :
    public TDefinitionRef< DefinitionT >,
    public PipelineLayoutBase
{
public:
    template< typename... Args >
    ComputePipelineLayout ( const Device& hDevice, Args... args );
};

// -----------------------------------------------------------------------------

template< class DefinitionT >
template< typename... Args >
TPipelineLayoutImpl< DefinitionT > :: TPipelineLayoutImpl (
    const Process& hProcess, const Device& hDevice, Args... args ) :
        KPipelineLayoutImpl ( hDevice, this ),
        DefinitionT ( hProcess, hDevice, args... )
{
    initialize ( hDevice );
}

// -----------------------------------------------------------------------------

template< class DefinitionT >
template< typename... Args >
TPipelineLayoutImpl< DefinitionT > :: TPipelineLayoutImpl (
    const Device& hDevice, Args... args ) :
        KPipelineLayoutImpl ( hDevice, this ),
        DefinitionT ( hDevice, args... )
{
    initialize ( hDevice );
}

// -----------------------------------------------------------------------------

template< class DefinitionT >
void TPipelineLayoutImpl< DefinitionT > :: initialize ( const Device& hDevice )
{
    this->fillShaderTable ( & d_shaderTable, hDevice );

    detail::KResourceSets resourceSets ( *this );

    const size_t nSets = resourceSets.getSetCount();
    d_descriptorSetLayouts.reserve ( nSets );

    for ( size_t iSet = 0; iSet != nSets; ++iSet )
        d_descriptorSetLayouts.emplace_back ( resourceSets, iSet, hDevice );

    d_descriptorSetLayoutHandles.resize ( d_descriptorSetLayouts.size() );

    std::transform (
        d_descriptorSetLayouts.begin(), d_descriptorSetLayouts.end(),
        d_descriptorSetLayoutHandles.begin(),
        []( const detail::KDescriptorSetLayout& hSet ) { return hSet.handle(); } );

    for ( const auto& iRes : this->getResources() )
    {
        if ( static_cast< size_t >( iRes.descriptorType ) >= d_descriptorPoolSizes.size() )
            d_descriptorPoolSizes.resize ( iRes.descriptorType + 1 );

        d_descriptorPoolSizes [ iRes.descriptorType ].type = iRes.descriptorType;
        d_descriptorPoolSizes [ iRes.descriptorType ].descriptorCount += iRes.descriptorCount;
    }
    
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = 0;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = static_cast< std::uint32_t >( d_descriptorSetLayoutHandles.size() );
    pipelineLayoutCreateInfo.pSetLayouts = & d_descriptorSetLayoutHandles [ 0 ];
    pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast< std::uint32_t >( this->getConstants().size() );
    pipelineLayoutCreateInfo.pPushConstantRanges = & this->getConstants()[ 0 ];

    d_result = ::vkCreatePipelineLayout (
        hDevice.handle(), & pipelineLayoutCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE PipelineLayoutBase :: PipelineLayoutBase() :
    d_bComputePipeline ( false )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE PipelineLayoutBase :: PipelineLayoutBase (
    KPipelineLayoutImpl* pImpl, bool bComputePipeline ) :
        TSharedReference< KPipelineLayoutImpl > ( pImpl ),
        d_bComputePipeline ( bComputePipeline )
{
}

// -----------------------------------------------------------------------------

template< class DefinitionT >
template< typename... Args >
VPP_INLINE PipelineLayout< DefinitionT > :: PipelineLayout (
    const Process& hProcess, const Device& hDevice, Args... args ) :
        TDefinitionRef< DefinitionT >( 
            new TPipelineLayoutImpl< DefinitionT >( hProcess, hDevice, args... ) ),
        PipelineLayoutBase ( this->internalDefinition(), false )
{
};

// -----------------------------------------------------------------------------

template< class DefinitionT >
template< typename... Args >
VPP_INLINE ComputePipelineLayout< DefinitionT > :: ComputePipelineLayout (
    const Device& hDevice, Args... args ) :
        TDefinitionRef< DefinitionT >( 
            new TPipelineLayoutImpl< DefinitionT >( hDevice, args... ) ),
        PipelineLayoutBase ( this->internalDefinition(), true )
{
};

// -----------------------------------------------------------------------------

VPP_INLINE VkPipelineLayout PipelineLayoutBase :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& PipelineLayoutBase :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool PipelineLayoutBase :: isComputePipeline() const
{
    return d_bComputePipeline;
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineConfig& PipelineLayoutBase :: config() const
{
    return *get()->d_pConfig;
}

// -----------------------------------------------------------------------------

VPP_INLINE size_t PipelineLayoutBase :: getDescriptorSetCount() const
{
    return get()->d_descriptorSetLayouts.size();
}

// -----------------------------------------------------------------------------

VPP_INLINE const DescriptorSetLayoutHandles& PipelineLayoutBase :: getDescriptorSetLayoutHandles() const
{
    return get()->d_descriptorSetLayoutHandles;
}

// -----------------------------------------------------------------------------

VPP_INLINE const DescriptorPoolSizes& PipelineLayoutBase :: getDescriptorPoolSizes() const
{
    return get()->d_descriptorPoolSizes;
}

// -----------------------------------------------------------------------------

VPP_INLINE const PipelineConfig::ShaderTable& PipelineLayoutBase :: getShaderTable() const
{
    return get()->d_shaderTable;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPPIPELINELAYOUT_HPP
