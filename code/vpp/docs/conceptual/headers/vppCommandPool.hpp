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
    \brief Allocates and manages the lifetime of command buffers.

*/

class CommandPool
{
public:
    /**
        \brief Creates a command pool for specified device and queue type.
    */

    CommandPool ( const Device& hDevice, EQueueType queueType, std::uint32_t flags = 0 );

    /**
        \brief Resets all child buffers for reusing.

        These buffers will be ready for recording again.
    */
    VkResult reset();

    /**
        \brief Freees all child buffers.

        These buffers will be destroyed.
    */
    VkResult release();

    /**
        \brief Checks whether this is valid command pool.
    */
    bool valid() const;

    /**
        \brief Retrieves the device.
    */
    const Device& device() const;

    /**
        \brief Retrieves the Vulkan handle.
    */
    VkCommandPool handle() const;

    enum EBufferLevel
    {
        PRIMARY = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        SECONDARY = VK_COMMAND_BUFFER_LEVEL_SECONDARY
    };

    /**
        \brief Creates single command buffer.
    */
    CommandBuffer createBuffer ( EBufferLevel level = PRIMARY );

    /**
        \brief Frees single command buffer.
    */
    void freeBuffer ( const CommandBuffer& buffer );

    /**
        \brief Creates multiple command buffers.
    */
    VkResult createBuffers (
        std::uint32_t count,
        std::vector< CommandBuffer >* pBuffers,
        EBufferLevel level = PRIMARY );

    /**
        \brief Frees multiple command buffers.
    */
    void freeBuffers (
        const std::vector< CommandBuffer >& buffers );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
