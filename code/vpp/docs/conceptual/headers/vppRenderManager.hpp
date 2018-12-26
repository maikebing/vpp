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
    \brief Simple rendering manager.

    This class provides a (very) simple rendering engine working with
    a SwapChain. It is suitable for examples, demos or quick experiments.

    Place a RenderManager object inside your rendering engine class.
    It takes a SwapChain reference in the constructor. Optionally
    you may specify custom Queue (if not, the manager will use
    default queue of the device, which is recommended).

    The pattern of using RenderManager is based on frames (which
    must be timed externally). For each frame:
    - call beginFrame()
    - call render(), supplying your RenderPass instance,
    - call endFrame().

    Rendering work is done by the manager in render() method on provided
    RenderPass. It gathers commands from the render graph, records them
    into internally maintained command buffer and submits to the device
    for execution.

    Additional parameter controls whether to cache the command buffer.
    There are two possible cases:
    - If commands in your render graph are static, or at least did not change
      since the last frame - specify \c CACHE_CMDS value. The last created
      buffer will be reused, which saves time.
    - If the commands have changed, specify \c REBUILD_CMDS. The buffer will
      be reset and the commands re-recorded.
    
    This object is reference counted and can be passed by value.
*/

class RenderManager
{
public:
    /** \brief Constructs a null reference. */
    RenderManager();

    /**
        \brief Constructs the render manager with specified swap chain.
    */
    RenderManager ( const SwapChain& hSwapChain );

    /**
        \brief Constructs the render manager with specified surface.

        This constructor automatically creates a swap chain for provided
        device and surface.

        It is recommended to leave default values of \c imageCount
        and \c imageQueuingMode. Many devices have limitations regarding
        selection of these, so it is best to rely on automatic detection.
    */
    RenderManager (
        Device hDevice,
        Surface hSurface,
        std::uint32_t imageCount = 0,
        VkPresentModeKHR imageQueuingMode = VK_PRESENT_MODE_MAILBOX_KHR );

    /**
        \brief Initializes rendering of current frame.

        Call before rendering a frame.

        This method ensures that rendering of previous frame has finished
        (waits if needed) and acquires a target image from the swapchain.
    */
    void beginFrame();

    /**
        \brief Finalizes rendering of current frame.

        Call after rendering a frame.

        This method sends currently rendered target image back to the swapchain
        for presentation.
    */
    void endFrame();

    /**
        \brief Selects the caching behavior for command buffers.
    */
    enum ECommandsCaching
    {
        CACHE_CMDS,     /**< \brief Retrieve buffer from cache if possible. */
        REBUILD_CMDS    /**< \brief Rebuild the buffer. */
    };

    /**
        \brief Renders current frame.

        This method renders the frame by using provided render pass.

        By default, it will cache and reuse commands. Specify \c REBUILD_CMDS
        as the second parameter to regenerate command sequences.
    */
    void render ( const RenderPass& hRenderPass, ECommandsCaching caching = CACHE_CMDS );

    /**
        \brief Retrieves the device.
    */
    const Device& device() const;

    /**
        \brief Retrieves the target surface.
    */
    const Surface& surface() const;

    /**
        \brief Retrieves the command queue into which the commands are sent.

        This is the first queue of default graphics queue family on the device.
    */
    const Queue& queue() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
