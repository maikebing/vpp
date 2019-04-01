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
#include "../include/vppQueryPool.hpp"
#include "../include/vppCommandBuffer.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

KQueryPoolImpl :: KQueryPoolImpl (
    QueryPool::EType type,
    std::uint32_t count,
    const Device& hDevice,
    std::uint32_t pipelineStats ) :
        d_hDevice ( hDevice ),
        d_handle(),
        d_result(),
        d_type ( type ),
        d_count ( count ),
        d_pipelineStats ( pipelineStats )
{
    VkQueryPoolCreateInfo vkQueryPoolCreateInfo;
    vkQueryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    vkQueryPoolCreateInfo.pNext = 0;
    vkQueryPoolCreateInfo.flags = 0;
    vkQueryPoolCreateInfo.queryType = static_cast< VkQueryType >( type );
    vkQueryPoolCreateInfo.queryCount = count;
    vkQueryPoolCreateInfo.pipelineStatistics = pipelineStats;

    d_result = ::vkCreateQueryPool (
        d_hDevice.handle(), & vkQueryPoolCreateInfo, 0, & d_handle );
}

// -----------------------------------------------------------------------------

KQueryPoolImpl :: ~KQueryPoolImpl()
{
    if ( d_result == VK_SUCCESS )
        ::vkDestroyQueryPool ( d_hDevice.handle(), d_handle, 0 );
}

// -----------------------------------------------------------------------------

bool QueryPool :: readQueryValues (
    void* pDestBuffer, std::uint32_t flags,
    std::uint32_t firstQuery, std::uint32_t queryCount ) const
{
    if ( get()->d_result != VK_SUCCESS )
        return false;

    const std::uint32_t totalCount = get()->d_count;
    const std::uint32_t realFirstQuery = std::min ( totalCount, firstQuery );
    const std::uint32_t available = totalCount - realFirstQuery;
    const std::uint32_t actualCount = std::min ( queryCount, available );

    if ( actualCount == 0 )
        return true;

    const VkDeviceSize singleElementSize =
        ( flags & VK_QUERY_RESULT_64_BIT ) ? 8u : 4u;

    const VkDeviceSize avaliabilityPresent =
        ( flags & VK_QUERY_RESULT_WITH_AVAILABILITY_BIT ) ? 1u : 0u;

    const VkDeviceSize stride =
        ( valueCount() + avaliabilityPresent ) * singleElementSize;

    const VkResult result = ::vkGetQueryPoolResults (
        get()->d_hDevice.handle(), get()->d_handle, 
        firstQuery, actualCount, static_cast< size_t >( stride * actualCount ),
        pDestBuffer, stride, flags
    );

    return result == VK_SUCCESS;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

KTimerArrayImpl :: KTimerArrayImpl (
    std::uint32_t timerCount,
    const Device& hDevice ) :
        d_hDevice ( hDevice ),
        d_timerCount ( timerCount ),
        d_timestampCount ( 2 * timerCount ),
        d_queryPool ( QueryPool::TIMESTAMP, d_timestampCount, hDevice ),
        d_pValues ( new std::uint64_t [ d_timestampCount + 1 ] ),
        d_pAlignedValues ( reinterpret_cast< std::uint64_t* >(
            ( reinterpret_cast< size_t >( d_pValues ) + 7 )
            & ~static_cast< size_t >( 7 ) ) ),
        d_timeUnitNS (
            hDevice.physical().properties().limits.timestampPeriod )
{
}

// -----------------------------------------------------------------------------

KTimerArrayImpl :: ~KTimerArrayImpl()
{
    delete [] d_pValues;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
