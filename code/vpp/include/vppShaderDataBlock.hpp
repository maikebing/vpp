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

#ifndef INC_VPPSHADERDATABLOCK_HPP
#define INC_VPPSHADERDATABLOCK_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPIPELINELAYOUT_HPP
#include "vppPipelineLayout.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KShaderDataAllocatorImpl;

// -----------------------------------------------------------------------------

class KShaderDataAllocator : public TSharedReference< KShaderDataAllocatorImpl >
{
public:
    KShaderDataAllocator();

    KShaderDataAllocator (
        const Device& hDevice,
        bool bDeallocatable,
        std::uint32_t setCount,
        const std::vector< VkDescriptorPoolSize >& dpSizes );

    VkDescriptorPool handle() const;
    bool valid() const;
    void reset();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KShaderDataAllocatorImpl : public TSharedObject< KShaderDataAllocatorImpl >
{
public:
    KShaderDataAllocatorImpl (
        const Device& hDevice,
        bool bDeallocatable,
        std::uint32_t setCount,
        const std::vector< VkDescriptorPoolSize >& dpSizes );

    ~KShaderDataAllocatorImpl();

private:
    friend class KShaderDataAllocator;
    Device d_hDevice;
    VkDescriptorPool d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataAllocatorImpl :: KShaderDataAllocatorImpl (
    const Device& hDevice, 
    bool bDeallocatable,
    std::uint32_t setCount,
    const std::vector< VkDescriptorPoolSize >& dpSizes ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result()
{
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = 0;
    descriptorPoolCreateInfo.flags = ( bDeallocatable ? VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : 0 );
    descriptorPoolCreateInfo.maxSets = setCount;
    descriptorPoolCreateInfo.poolSizeCount = static_cast< std::uint32_t >( dpSizes.size() );
    descriptorPoolCreateInfo.pPoolSizes = & dpSizes [ 0 ];

    d_result = ::vkCreateDescriptorPool (
        d_hDevice.handle(), & descriptorPoolCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataAllocatorImpl :: ~KShaderDataAllocatorImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyDescriptorPool ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataAllocator :: KShaderDataAllocator()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataAllocator :: KShaderDataAllocator (
    const Device& hDevice,
    bool bDeallocatable,
    std::uint32_t setCount,
    const std::vector< VkDescriptorPoolSize >& dpSizes ) :
        TSharedReference< KShaderDataAllocatorImpl > (
            new KShaderDataAllocatorImpl ( hDevice, bDeallocatable, setCount, dpSizes ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDescriptorPool KShaderDataAllocator :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KShaderDataAllocator :: valid() const
{
    return get()->d_result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KShaderDataAllocator :: reset()
{
    if ( valid() )
        ::vkResetDescriptorPool ( get()->d_hDevice.handle(), get()->d_handle, 0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KShaderDataBlockImpl;

// -----------------------------------------------------------------------------

class ShaderDataBlock : public TSharedReference< KShaderDataBlockImpl >
{
public:
    ShaderDataBlock();
    ShaderDataBlock ( const PipelineLayoutBase& hLayout );

    VkDescriptorSet getDescriptorSet ( std::uint32_t iSet ) const;

    const Device& device() const;

    template< class AssignmentListT >
    void update ( const AssignmentListT& list );

    void cmdBind ( CommandBuffer hCmdBuffer = CommandBuffer() ) const;

    template< class ValueT >
    void setBoundItem ( std::uint32_t id, const ValueT& value );

    template< class ValueT >
    void setBoundItem ( std::uint32_t id, const TUpdateMultipleDescriptors< ValueT >& values );
};

// -----------------------------------------------------------------------------

class KShaderDataBlockImpl : public TSharedObject< KShaderDataBlockImpl >
{
public:
    KShaderDataBlockImpl ( const PipelineLayoutBase& hLayout );
    ~KShaderDataBlockImpl();

    VPP_INLINE bool compareObjects ( const KShaderDataBlockImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    friend class ShaderDataBlock;
    Device d_hDevice;
    VkResult d_result;
    KShaderDataAllocator d_descriptorPool;
    PipelineLayoutBase d_layout;
    DescriptorSets d_descriptorSets;

    typedef std::map<
        std::pair< std::uint32_t, std::uint32_t >,
        detail::KDescriptorItemHolder > IdIndex2ItemHolder;

    IdIndex2ItemHolder d_idIndex2itemHolder;
};

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataBlockImpl :: KShaderDataBlockImpl ( const PipelineLayoutBase& hLayout ) :
    d_hDevice ( hLayout.device() ),
    d_result(),
    d_layout ( hLayout )
{
    d_descriptorPool = KShaderDataAllocator (
        d_hDevice,
        true,
        static_cast< std::uint32_t >( hLayout.getDescriptorSetCount() ),
        hLayout.getDescriptorPoolSizes() );

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = 0;
    descriptorSetAllocateInfo.descriptorPool = d_descriptorPool.handle();
    descriptorSetAllocateInfo.descriptorSetCount = 
        static_cast< std::uint32_t >( hLayout.getDescriptorSetLayoutHandles().size() );
    descriptorSetAllocateInfo.pSetLayouts = & hLayout.getDescriptorSetLayoutHandles() [ 0 ];

    d_descriptorSets.resize ( descriptorSetAllocateInfo.descriptorSetCount );

    d_result = ::vkAllocateDescriptorSets (
        d_hDevice.handle(), & descriptorSetAllocateInfo, & d_descriptorSets [ 0 ] );
}

// -----------------------------------------------------------------------------

VPP_INLINE KShaderDataBlockImpl :: ~KShaderDataBlockImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkFreeDescriptorSets (
            d_hDevice.handle(), d_descriptorPool.handle(),
            static_cast< std::uint32_t >( d_descriptorSets.size() ),
            & d_descriptorSets [ 0 ] );
}

// -----------------------------------------------------------------------------

VPP_INLINE ShaderDataBlock :: ShaderDataBlock()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE ShaderDataBlock :: ShaderDataBlock (
    const PipelineLayoutBase& hLayout ) :
        TSharedReference< KShaderDataBlockImpl >(
            new KShaderDataBlockImpl ( hLayout ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDescriptorSet ShaderDataBlock :: getDescriptorSet ( std::uint32_t iSet ) const
{
    return get()->d_descriptorSets [ iSet ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const Device& ShaderDataBlock :: device() const
{
    return get()->d_hDevice;
}

// -----------------------------------------------------------------------------

template< class ValueT >
void ShaderDataBlock :: setBoundItem (
    std::uint32_t id, const ValueT& value )
{
    get()->d_idIndex2itemHolder [ std::make_pair ( id, 0 ) ] = value;
}

// -----------------------------------------------------------------------------

template< class ValueT >
void ShaderDataBlock :: setBoundItem (
    std::uint32_t id, const TUpdateMultipleDescriptors< ValueT >& values )
{
    for ( std::uint32_t i = 0; i != values.size(); ++i )
    {
        get()->d_idIndex2itemHolder [
            std::make_pair ( id, values.startIndex() + i ) ] = values [ i ];
    }
}

// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

template< class AssignmentListT >
class TUpdater
{
public:
    VPP_INLINE TUpdater ( ShaderDataBlock& block, const AssignmentListT& list ) :
        d_block ( block ),
        d_imageInfos ( list.getImageInfoCount() ),
        d_bufferInfos ( list.getBufferInfoCount() ),
        d_texelBufferViews ( list.getTexelBufferViewCount() )
    {
    }
    
    static const size_t ASSIGNER_COUNT = AssignmentListT::ASSIGNER_COUNT;
    static const size_t COPIER_COUNT = AssignmentListT::COPIER_COUNT;

    VPP_INLINE void copy (
        std::uint32_t sset,
        std::uint32_t sbinding,
        std::uint32_t soffset,
        std::uint32_t dset,
        std::uint32_t dbinding,
        std::uint32_t doffset,
        std::uint32_t count )
    {
        VkCopyDescriptorSet& currentSet = d_copiers [ d_currentCopier++ ];
        currentSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
        currentSet.pNext = 0;
        currentSet.srcSet = d_block.getDescriptorSet ( sset );
        currentSet.srcBinding = sbinding;
        currentSet.srcArrayElement = soffset;
        currentSet.dstSet = d_block.getDescriptorSet ( dset );
        currentSet.dstBinding = dbinding;
        currentSet.dstArrayElement = doffset;
        currentSet.descriptorCount = count;
    }

    VPP_INLINE VkDescriptorImageInfo* allocateSetUpdateInfo (
        VkWriteDescriptorSet& currentSet, VkDescriptorImageInfo*, size_t count )
    {
        VkDescriptorImageInfo* pInfo = & d_imageInfos [ d_currentImageInfo ];
        d_currentImageInfo += count;
        currentSet.pImageInfo = pInfo;
        currentSet.pBufferInfo = 0;
        currentSet.pTexelBufferView = 0;
        return pInfo;
    }

    VPP_INLINE VkDescriptorBufferInfo* allocateSetUpdateInfo (
        VkWriteDescriptorSet& currentSet, VkDescriptorBufferInfo*, size_t count )
    {
        VkDescriptorBufferInfo* pInfo = & d_bufferInfos [ d_currentBufferInfo ];
        d_currentBufferInfo += count;
        currentSet.pImageInfo = 0;
        currentSet.pBufferInfo = pInfo;
        currentSet.pTexelBufferView = 0;
        return pInfo;
    }

    VPP_INLINE VkBufferView* allocateSetUpdateInfo (
        VkWriteDescriptorSet& currentSet, VkBufferView*, size_t count )
    {
        VkBufferView* pInfo = & d_texelBufferViews [ d_currentTexelBufferView ];
        d_currentTexelBufferView += count;
        currentSet.pImageInfo = 0;
        currentSet.pBufferInfo = 0;
        currentSet.pTexelBufferView = pInfo;
        return pInfo;
    }

    template< VkDescriptorType TYPE, typename ViewT, typename ValueT >
    VPP_INLINE void write (
        std::uint32_t id,
        std::uint32_t set,
        std::uint32_t binding,
        const ValueT& value )
    {
        d_block.setBoundItem ( id, value );

        VkWriteDescriptorSet& currentSet = d_writers [ d_currentWriter++ ];
        currentSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        currentSet.pNext = 0;
        currentSet.dstSet = d_block.getDescriptorSet ( set );
        currentSet.dstBinding = binding;
        currentSet.dstArrayElement = detail::getValueStartIndex ( value );
        currentSet.descriptorCount = static_cast< std::uint32_t >( detail::getValueCount ( value ) );
        currentSet.descriptorType = TYPE;

        typedef detail::TDescriptorTypeTraits< TYPE, ViewT > type_traits;
        typename type_traits::update_dest_type* pTag = 0;

        type_traits::update (
            allocateSetUpdateInfo ( currentSet, pTag, currentSet.descriptorCount ),
            value
        );
    }

    VPP_INLINE void update ( const AssignmentListT& list )
    {
        d_currentWriter = 0;
        d_currentCopier = 0;
        d_currentImageInfo = 0;
        d_currentBufferInfo = 0;
        d_currentTexelBufferView = 0;

        list.for_each ( *this );

        ::vkUpdateDescriptorSets (
            d_block.device().handle(),
            ASSIGNER_COUNT, d_writers,
            COPIER_COUNT, d_copiers );
    }

private:
    ShaderDataBlock& d_block;

    std::vector< VkDescriptorImageInfo > d_imageInfos;
    std::vector< VkDescriptorBufferInfo > d_bufferInfos;
    std::vector< VkBufferView > d_texelBufferViews;

    VkWriteDescriptorSet d_writers [ ASSIGNER_COUNT + 1 ];
    VkCopyDescriptorSet d_copiers [ COPIER_COUNT + 1 ];

    size_t d_currentWriter;
    size_t d_currentCopier;
    size_t d_currentImageInfo;
    size_t d_currentBufferInfo;
    size_t d_currentTexelBufferView;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

template< class AssignmentListT >
VPP_INLINE void ShaderDataBlock :: update ( const AssignmentListT& list )
{
    detail::TUpdater< AssignmentListT > updater ( *this, list );
    updater.update ( list );

    get()->d_layout.config().bindDebugProbes ( *this );
}

// -----------------------------------------------------------------------------

VPP_INLINE void ShaderDataBlock :: cmdBind ( CommandBuffer hCommandBuffer ) const
{
    const VkCommandBuffer hCmdBuffer = hCommandBuffer ?
        hCommandBuffer.handle()
        : RenderingCommandContext::getCommandBufferHandle();

    ::vkCmdBindDescriptorSets (
        hCmdBuffer,
        ( get()->d_layout.isComputePipeline() ?
            VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS ),
        get()->d_layout.handle(), 0,
        static_cast< std::uint32_t >( get()->d_descriptorSets.size() ),
        & get()->d_descriptorSets [ 0 ],
        0, 0
    );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHADERDATABLOCK_HPP
