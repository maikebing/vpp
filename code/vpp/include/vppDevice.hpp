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

#ifndef INC_VPPDEVICE_HPP
#define INC_VPPDEVICE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPPHYSICALDEVICE_HPP
#include "vppPhysicalDevice.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class DeviceImpl;

// -----------------------------------------------------------------------------

class Device : public TSharedReference< DeviceImpl >
{
public:
    Device();

    Device (
        const PhysicalDevice& hPhysicalDevice );

    Device (
        const PhysicalDevice& hPhysicalDevice,
        const VkPhysicalDeviceFeatures& features );

    Device (
        const PhysicalDevice& hPhysicalDevice,
        const std::vector< float >& queuePriorities );

    Device (
        const PhysicalDevice& hPhysicalDevice,
        const std::vector< float >& queuePriorities,
        const VkPhysicalDeviceFeatures& features );

    ~Device();

    VkDevice handle() const;
    const PhysicalDevice& physical() const;

    std::uint32_t queueFamily ( EQueueType queueType ) const;
    std::uint32_t queueCount ( EQueueType queueType ) const;
    VPP_DLLAPI float queuePriority ( std::uint32_t iQueue ) const;

    VPP_DLLAPI CommandPool& defaultCmdPool ( EQueueType queueType = Q_GRAPHICS ) const;
    VPP_DLLAPI PipelineCache& defaultPipelineCache() const;
    
    VkResult waitForIdle() const;
};

// -----------------------------------------------------------------------------

class DeviceImpl : public TSharedObject< DeviceImpl >
{
public:
    VPP_DLLAPI DeviceImpl (
        const PhysicalDevice& hPhysicalDevice,
        const std::vector< float >& queuePriorities,
        const VkPhysicalDeviceFeatures* pFeatures );

    VPP_DLLAPI DeviceImpl (
        const PhysicalDevice& hPhysicalDevice,
        const VkPhysicalDeviceFeatures* pFeatures );

    VPP_DLLAPI ~DeviceImpl();

    VPP_INLINE bool compareObjects ( const DeviceImpl* pRHS ) const
    {
        return this < pRHS;
    }

private:
    static const std::vector< float >& getDefaultQueuePriorities();

private:
    friend class Device;

    PhysicalDevice d_hPhysicalDevice;
    VkDevice d_handle;
    VkResult d_result;

    std::uint32_t d_graphicsQueueFamily;
    std::uint32_t d_graphicsQueueCount;
    std::vector< float > d_queuePriorities;

    std::uint32_t d_transferQueueFamily;
    std::uint32_t d_transferQueueCount;

    CommandPool* d_pDefaultGraphicsCmdPool;
    CommandPool* d_pDefaultTransferCmdPool;
    PipelineCache* d_pDefaultPipelineCache;

    VPP_EXTSYNC_MTX_DECLARE;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

VPP_INLINE Device :: Device()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Device :: Device (
    const PhysicalDevice& hPhysicalDevice ) :
        TSharedReference< DeviceImpl > (
            new DeviceImpl ( hPhysicalDevice, 0 ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Device :: Device (
    const PhysicalDevice& hPhysicalDevice,
    const VkPhysicalDeviceFeatures& features ) :
        TSharedReference< DeviceImpl > (
            new DeviceImpl ( hPhysicalDevice, & features ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Device :: Device (
    const PhysicalDevice& hPhysicalDevice,
    const std::vector< float >& queuePriorities ) :
        TSharedReference< DeviceImpl > (
            new DeviceImpl ( hPhysicalDevice, queuePriorities, 0 ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Device :: Device (
    const PhysicalDevice& hPhysicalDevice,
    const std::vector< float >& queuePriorities,
    const VkPhysicalDeviceFeatures& features ) :
        TSharedReference< DeviceImpl > (
            new DeviceImpl ( hPhysicalDevice, queuePriorities, & features ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Device :: ~Device()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkDevice Device :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE const PhysicalDevice& Device :: physical() const
{
    return get()->d_hPhysicalDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Device :: queueFamily ( EQueueType queueType ) const
{
    return queueType == Q_TRANSFER ?
        get()->d_transferQueueFamily : get()->d_graphicsQueueFamily;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t Device :: queueCount ( EQueueType queueType ) const
{
    return queueType == Q_TRANSFER ?
        get()->d_transferQueueCount : get()->d_graphicsQueueCount;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult Device :: waitForIdle() const
{
    return ::vkDeviceWaitIdle ( get()->d_handle );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPDEVICE_HPP
