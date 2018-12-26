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

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Represents logical rendering device.

    Logical rendering device is the primary interface to rendering. It actually
    represents a GPU unit in the computer. That is, this is the computation
    part of the graphic card, excluding the display subsystems like framebuffer
    and DAC (these are abstracted by Surface class).

    Device object is obtained from a PhysicalDevice object.

    Device object is required by most VPP class methods and functions.

    Also in VPP (unlike core Vulkan) the Device object provides certain default
    helper objects like CommandPool and PipelineCache. 

    This object is reference-counted and may be passed by value.
*/

class Device
{
public:
    /** \brief Constructs null reference. */
    Device();

    /** \brief Constructs logical device from physical device with default features. */
    Device (
        const PhysicalDevice& hPhysicalDevice );

    /** \brief Constructs logical device from physical device with specified features.
    
        Yopu can use DeviceFeatures object here, as it is derived from
        VkPhysicalDeviceFeatures.

        Example:

        \code
            vpp::PhysicalDevice hPhysicalDevice = ...;

            vpp::DeviceFeatures feat ( hPhysicalDevice );
            const bool bSupportsDepthBiasClamp = feat.enableIfSupported ( vpp::fDepthBiasClamp );
            const bool bSupportsAlphaToOne = feat.enableIfSupported ( vpp::fAlphaToOne );

            vpp::Device hDevice ( hPhysicalDevice, feat );
        \endcode
    */
    Device (
        const PhysicalDevice& hPhysicalDevice,
        const VkPhysicalDeviceFeatures& features );

    /** \brief Constructs logical device from physical device with specified queue priorities. */
    Device (
        const PhysicalDevice& hPhysicalDevice,
        const std::vector< float >& queuePriorities );

    /** \brief Constructs logical device from physical device with specified queue priorities
               and features.
    */
    Device (
        const PhysicalDevice& hPhysicalDevice,
        const std::vector< float >& queuePriorities,
        const VkPhysicalDeviceFeatures& features );

    /** \brief Decreases reference count. Deletes the device object when count reaches zero.*/
    ~Device();

    /** \brief Checks whether this is not a null reference. */
    operator bool() const;

    /** \brief Retrieves Vulkan handle for this device. */
    VkDevice handle() const;

    /** \brief Retrieves parent physical device for this logical device. */
    const PhysicalDevice& physical() const;

    /** \brief Retrieves default queue family index for queues of specified type. */
    unsigned int queueFamily ( EQueueType queueType ) const;

    /** \brief Retrieves queue count within default queue family of specified type. */
    unsigned int queueCount ( EQueueType queueType ) const;

    /** \brief Retrieves queue priority for specified queue index within default graphics queue family. */
    float queuePriority ( unsigned int iQueue ) const;

    /** \brief Retrieves default command allocation pool for this device and specified queue type.
    
        You can use this pool for convenience, or create your own.

        This command pool allows to reset individual command buffer allocated by it
        (that is, it has \c VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT set).

        Examples:

        \code
            vpp::Device hDevice = ...;
            vpp::CommandPool hGraphicsCmdPool = hDevice.defaultCmdPool();
            vpp::CommandPool hTransferCmdPool = hDevice.defaultCmdPool ( vpp::Q_TRANSFER );
        \endcode
    */
    CommandPool& defaultCmdPool ( EQueueType queueType = Q_GRAPHICS ) const;

    /** \brief Retrieves default pipeline cache for this device.
    
        You can use this cache for convenience, or create your own.

        Example:

        \code
            vpp::Device hDevice = ...;
            vpp::PipelineCache hCache = hDevice.defaultPipelineCache();
        \endcode
    */
    PipelineCache& defaultPipelineCache() const;
    
    /** \brief Waits until all operations performed by this device are finished. */
    VkResult waitForIdle();
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
