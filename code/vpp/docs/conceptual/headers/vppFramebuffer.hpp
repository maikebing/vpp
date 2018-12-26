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
    \brief Defines a vector of physical images to be used as targets for rendering attachments.

    The framebuffer is associated with given RenderPass object and, by extension,
    with the render graph. Each item in the vector corresponds to single attachment
    node defined inside the render graph, in exactly same order as they are listed
    in the subclass definition (including both Attachment and Display nodes).

    An item slot may however point to null view, that means a default-constructed
    FrameImageView object. In such case, VPP will try to use fixed view specified
    in the constructor of the corresponding node. There should be either view valid
    (or both), it is an error when there is a null view in the vector and no
    fixed view in the node simultaneously.

    If specified non-null view (valid FrameImageView), this view will override any
    fixed view set for corresponding node during its construction.

    If all views are fixed, you can avoid using FrameBuffer class completely.
    This is done by means of RenderManager class that manages FrameBuffer
    instances internally.

    If the only non-fixed views are Display nodes (which implicitly use
    swapped images from a swapchain), you should also use RenderManager
    because RenderManager handles swapchains.

    Use the FrameBuffer when you need to render into off-screen image targets
    that change. That also enforces using CommandBufferRecorder class to 
    control rendering. Simpler RenderManager class does not support specifying
    explicit FrameBuffer. In such case, you must do several lower level things
    manually, that otherwise would be done by RenderManager:
    - Create at least one FrameBuffer object (might rely completely on fixed
      views, but still must be created).
    - If your render graph contain Display nodes, you must create a SwapChain
      object for each such node.
    - Also if the preceding point holds, the number of FrameBuffer objects
      you create must be equal to the number of views in single SwapChain.
      This is because for each Display node you must provide a view taken
      from corresponding SwapChain, and there are multiple such views
      (e.g. 3 for triple buffering). For each such view index, there must
      be separate FrameBuffer. Vulkan does not allow to switch single view
      inside the framebuffer, only to swap entire framebuffers.
    - Your rendering engine must handle multiple buffering on its own.
      In all cases, you must manage multiple FrameBuffer objects. On each
      rendered frame, supply appropriate one to the CommandBufferRecorder for
      rendering, and different one for displaying. 
    - Consult the RenderManager source code for simple example how to implement
      things listed above.

    FrameBuffer accepts view references in untyped form (FrameImageView).
    You can obtain a FrameImageView from each typed ImageView< ... > instance
    by calling its \c frameView() method.

    This object is reference-counted and may be passed by value.
*/

class FrameBuffer
{
public:
    /** \brief Constructs null reference. */
    FrameBuffer();

    /** \brief Constructs a frame buffer from specified image vector.
    
        This constructor also handles the null views present inside
        the vector (if any). They will be replaced by fixed views retrieved
        from the nodes of the render graph (accessed via render pass).
    */
    FrameBuffer (
        const std::vector< FrameImageView >& imageViews,
        const RenderPass& hRenderPass );

    /** \brief Constructs a frame buffer assuming all views are fixed and specified
         during the construction of attachment objects.
    */
    FrameBuffer (
        const RenderPass& hRenderPass );

    /** \brief Retrieves the Vulkan handle. */
    VkFramebuffer handle() const;

    /** \brief Retrieves the size of the frame. */
    VkRect2D area() const;

    /** \brief Retrieves the view reference for specified image index. */
    FrameImageView imageView ( size_t index ) const;

    /** \brief Retrieves the count of images. */
    size_t imageViewCount() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
