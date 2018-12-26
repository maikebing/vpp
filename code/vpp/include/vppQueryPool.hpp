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

#ifndef INC_VPPQUERYPOOL_HPP
#define INC_VPPQUERYPOOL_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPDEVICE_HPP
#include "vppDevice.hpp"
#endif

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KQueryPoolImpl;

// -----------------------------------------------------------------------------

class QueryPool : public TSharedReference< KQueryPoolImpl >
{
public:
    enum EType
    {
        OCCLUSION = VK_QUERY_TYPE_OCCLUSION,
        PIPELINE_STATISTICS = VK_QUERY_TYPE_PIPELINE_STATISTICS,
        TIMESTAMP = VK_QUERY_TYPE_TIMESTAMP
    };

    enum EPipelineStats
    {
        INPUT_VERTICES = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
        INPUT_PRIMITIVES = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
        INV_VERTEX = VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
        INV_GEOMETRY = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
        GEOMETRY_PRIMITIVES = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
        INV_CLIPPING = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
        CLIPPING_PRIMITIVES = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
        INV_FRAGMENT = VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
        TESS_PATCHES = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
        INV_TESS = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
        INV_COMPUTE = VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT
    };

    QueryPool (
        EType type,
        std::uint32_t count,
        const Device& hDevice,
        std::uint32_t pipelineStats = 0 );

    VkQueryPool handle() const;
    EType type() const;
    std::uint32_t count() const;
    std::uint32_t pipelineStats() const;
    std::uint32_t valueCount() const;

    void cmdResetAll ( CommandBuffer hCmdBuffer );

    bool readQueryValues (
        void* pDestBuffer, std::uint32_t flags,
        std::uint32_t firstQuery = 0,
        std::uint32_t queryCount = std::numeric_limits< std::uint32_t >::max()
    ) const;
};

// -----------------------------------------------------------------------------

class KQueryPoolImpl : public TSharedObject< KQueryPoolImpl >
{
public:
    KQueryPoolImpl (
        QueryPool::EType type,
        std::uint32_t count,
        const Device& hDevice,
        std::uint32_t pipelineStats );

    ~KQueryPoolImpl();

private:
    friend class QueryPool;
    Device d_hDevice;
    VkQueryPool d_handle;
    VkResult d_result;

    QueryPool::EType d_type;
    std::uint32_t d_count;
    std::uint32_t d_pipelineStats;
};

// -----------------------------------------------------------------------------

VPP_INLINE QueryPool :: QueryPool (
    EType type,
    std::uint32_t count,
    const Device& hDevice,
    std::uint32_t pipelineStats ) :
        TSharedReference< KQueryPoolImpl >(
            new KQueryPoolImpl ( type, count, hDevice, pipelineStats ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkQueryPool QueryPool :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE QueryPool::EType QueryPool :: type() const
{
    return get()->d_type;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t QueryPool :: count() const
{
    return get()->d_count;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t QueryPool :: pipelineStats() const
{
    return get()->d_pipelineStats;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t QueryPool :: valueCount() const
{
    if ( get()->d_type != PIPELINE_STATISTICS )
        return 1u;
    else
    {
        // count '1' bits in the mask
        std::uint32_t value = get()->d_pipelineStats;
        value = value - ( ( value >> 1 ) & 0x55555555u );
        value = ( value & 0x33333333 ) + ( ( value >> 2 ) & 0x33333333u );
        return ( ( ( value + ( value >> 4 ) ) & 0x0F0F0F0Fu ) * 0x01010101u ) >> 24;
    }
}

// -----------------------------------------------------------------------------

VPP_INLINE void QueryPool :: cmdResetAll ( CommandBuffer hCmdBuffer )
{
    ::vkCmdResetQueryPool (
        hCmdBuffer.handle(), get()->d_handle, 0, get()->d_count );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KTimerArrayImpl : public TSharedObject< KTimerArrayImpl >
{
public:
    KTimerArrayImpl (
        std::uint32_t timerCount,
        const Device& hDevice );

    ~KTimerArrayImpl();

private:
    friend class TimerArray;

    Device d_hDevice;

    std::uint32_t d_timerCount;
    std::uint32_t d_timestampCount;

    QueryPool d_queryPool;

    std::uint64_t* d_pValues;
    std::uint64_t* d_pAlignedValues;

    double d_timeUnitNS;
};

// -----------------------------------------------------------------------------

class TimerArray : public TSharedReference< KTimerArrayImpl >
{
public:
    TimerArray ( std::uint32_t timerCount, const Device& hDevice );

    void retrieve();

    void cmdResetAll ( CommandBuffer hCmdBuffer );
    void cmdMeasureBegin ( std::uint32_t iTimer, CommandBuffer hCmdBuffer );
    void cmdMeasureEnd ( std::uint32_t iTimer, CommandBuffer hCmdBuffer );

    double interval_ns ( std::uint32_t iTimer ) const;
    double interval_us ( std::uint32_t iTimer ) const;
    double interval_ms ( std::uint32_t iTimer ) const;
    double interval_s ( std::uint32_t iTimer ) const;
};

// -----------------------------------------------------------------------------

VPP_INLINE TimerArray :: TimerArray (
    std::uint32_t timerCount,
    const Device& hDevice ) :
        TSharedReference< KTimerArrayImpl >(
            new KTimerArrayImpl ( timerCount, hDevice ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE void TimerArray :: retrieve()
{
    get()->d_queryPool.readQueryValues (
        get()->d_pAlignedValues,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT );
}

// -----------------------------------------------------------------------------

VPP_INLINE void TimerArray :: cmdResetAll ( CommandBuffer hCmdBuffer )
{
    get()->d_queryPool.cmdResetAll ( hCmdBuffer );
}

// -----------------------------------------------------------------------------

VPP_INLINE void TimerArray :: cmdMeasureBegin (
    std::uint32_t iTimer, CommandBuffer hCmdBuffer )
{
    const std::uint32_t iQuery = 2*iTimer;

    ::vkCmdWriteTimestamp (
        hCmdBuffer.handle(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        get()->d_queryPool.handle(), iQuery );
}

// -----------------------------------------------------------------------------

VPP_INLINE void TimerArray :: cmdMeasureEnd (
    std::uint32_t iTimer, CommandBuffer hCmdBuffer )
{
    const std::uint32_t iQuery = 2*iTimer + 1;

    ::vkCmdWriteTimestamp (
        hCmdBuffer.handle(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        get()->d_queryPool.handle(), iQuery );
}

// -----------------------------------------------------------------------------

VPP_INLINE double TimerArray :: interval_ns ( std::uint32_t iTimer ) const
{
    const std::uint32_t iQuery = 2*iTimer;
    const std::uint64_t* pValues = get()->d_pAlignedValues;

    const double interval =
        static_cast< double >( pValues [ iQuery + 1 ] - pValues [ iQuery ] );

    return interval * get()->d_timeUnitNS;
}

// -----------------------------------------------------------------------------

VPP_INLINE double TimerArray :: interval_us ( std::uint32_t iTimer ) const
{
    return interval_ns ( iTimer ) * 0.001;
}

// -----------------------------------------------------------------------------

VPP_INLINE double TimerArray :: interval_ms ( std::uint32_t iTimer ) const
{
    return interval_ns ( iTimer ) * 0.000001;
}

// -----------------------------------------------------------------------------

VPP_INLINE double TimerArray :: interval_s ( std::uint32_t iTimer ) const
{
    return interval_ns ( iTimer ) * 0.000000001;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPQUERYPOOL_HPP
