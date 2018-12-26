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
    \brief Represents attachment graph node.

    This is a lightweight class, can be passed by value. Its purpose however
    is to be used only within custom render graph definitions. It represents attachment
    of given format participating in the render graph.
    
    Attachment nodes must be explicitly associated with process nodes. The kind
    of the attachment (e.g. color output, depth, input, etc.) is determined
    during this association, because single attachment might be shared by multiple
    processes and perform different roles (e.g. output from one process and input
    to another). Define this association using one of the following methods of the
    Process class, called from the RenderGraph derived class constructor:
        - Process::addColorOutput()
        - Process::addReusedColorOutput()
        - Process::setDepthOutput()
        - Process::setReusedDepthOutput()
        - Process::setDepthInput()
        - Process::addColorAndResolveOutput()
        - Process::addPreservedOutput()
        - Process::addInput()

    On the other hand, an attachment must be also associated with some physical
    image, or part of image (e.g. single layer in image array). This is represented
    by an object of ImageView< ... > or FrameImageView class. Such image view must be
    provided for each attachment, however there are two ways to do this:
    - Immediately when constructing Attachment object, by providing the view object
      directly to the constructor.
    - Later, as an element in the vector supplied to FrameBuffer object construction.

    The former method is simpler, the latter one allows rendering the same graph
    to different sets of images. This can be very useful e.g. when implementing
    double buffering.

    These approaches can also be mixed. You can construct some attachments with
    fixed view, and others without the view. Later, when constructing
    the FrameBuffer, provide real FrameImageView object for these non-fixed
    attachments. For the rest, provide empty FrameImageView() values
    to the FrameBuffer, and it will read the fixed view from the corresponding
    Attachment object.
*/

template< class FormatT >
class Attachment
{
public:
    /**
        \brief Constructs attachment mode without fixed image view.

        Note that you must still provide image size.

        This constructor is intended to be used for Attachment objects being
        fields of a RenderGraph object. The parent RenderGraph is automatically
        determined as currently constructed graph.
    */

    Attachment (
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose = RENDER );

    /**
        \brief Constructs attachment mode without fixed image view.

        Note that you must still provide image size.

        The parent RenderGraph object is specified explicitly here.
    */
    Attachment (
        RenderGraph* pGraph,
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose = RENDER );

    /**
        \brief Constructs attachment mode with fixed image view.

        Provide a reference to ImageView< ... > object or FrameImageView
        as the argument. The node will be beound statically to specified image.

        This constructor is intended to be used for Attachment objects being
        fields of a RenderGraph object. The parent RenderGraph is automatically
        determined as currently constructed graph.
    */
    template< class ViewT >
    Attachment (
        const ViewT& imageView );

    /**
        \brief Constructs attachment mode with fixed image view.

        Provide a reference to ImageView< ... > object or FrameImageView
        as the second argument. The node will be beound statically to specified
        image.

        The parent RenderGraph object is specified explicitly here.
    */
    template< class ViewT >
    Attachment (
        RenderGraph* pGraph,
        const ViewT& imageView );

    /** \brief Retrieves the parent render graph object. */
    RenderGraph* graph() const;

    /** \brief Retrieves the index of the attachment. */
    std::uint32_t index() const;

    /** \brief Retrieves image width of the attachment. */
    std::uint32_t width() const;

    /** \brief Retrieves image height of the attachment. */
    std::uint32_t height() const;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Represents display attachment graph node.

    Thie is a special kind of color attachment allowing to render an image onto
    a Surface. This is the way to display results on the screen.

    First, you must obtain a Surface. Then construct a Display object in place of
    Attachment object. The format is determined automatically. Finally, call
    addColorOutput on the Process object.
*/

class Display
{
public:
    Display (
        const Surface& hSurface );

    Display (
        RenderGraph* pGraph,
        const Surface& hSurface );

    RenderGraph* graph() const;
    std::uint32_t index() const;
    std::uint32_t width() const;
    std::uint32_t height() const;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Represents process graph node.

    This node performs actual rendering. Internally it constructs a Vulkan
    subpass.

    Each Process node is associated with additional objects of several kinds,
    in order to specify how and what to render.
    
    - A sequence of rendering commands, forming a step-by-step description
      of the process. Among these there can be commands peforming: drawing
      (regular, indexed or indirect), resource binding, synchronization,
      parameter and push constant setting, pipeline selection. The sequence
      of commands can be attached to a Process node by using the << operator
      and supplying a C++ lambda function containing the commands. The code
      inside lambda will be called by VPP, automatically converted to Vulkan
      command buffer and submitted at appropriate time. VPP handles all
      of these tasks. You just need to provide the lambda.
    
    - One or more rendering pipelines. A pipeline is an object describing
      the structure of the rendering process in detail, including its
      inputs, outputs, resources and shaders. To define a pipeline you
      need to make a subclass of PipelineConfig class and provide the
      class name to the PipelineLayout template. An instance of PipelineLayout
      is in turn being associated with a Process node. Multiple PipelineLayout
      objects can be attached to single Process node. There is a command which
      selects one of them to be active at any given moment. In order to add
      a PipelineLayout instance to a Process node, you need to have the RenderPass
      object (made from RenderGraph) which manages pipelines. Call the addPipeline
      method on that object.

    - When attaching a PipelineLayout instance to a Process node, you must also
      specify additional object of RenderingOptions class. This object contains
      some important parameters of the rendering pipeline, e.g.: polygon
      drawing mode (filled, wireframe, points), back/front face determination
      and culling, depth and stencil tests configuration, viewport for the
      rasterizer, etc. This object is supplied to addPipeline method of the
      RenderPass object as well.

    - A number of Attachment nodes, representing destination images, as
      well as source images for processes which transform data. Attachments
      are bound to the Process node by calling its methods: addColorOutput,
      addReusedColorOutput, setDepthOutput, addInput, etc. Which method to
      call depends on the kind of attachment.

    - Optionally, one or more dependency arcs, specifying additional
      ordering of operation between processes and ensuring the data produced
      by one is available to another when needed. Dependencies are added by
      calling the addDependency method, or automatically when adding certain
      attachments.
*/

class Process
{
public:
    /** \brief Constructs process node for currently defined graph.
    
        Place a Process node in your render graph class (derived from RenderGraph).
        Do not specify any arguments to the constructor. This is the recommended
        method of adding nodes. The constructor will automatically determine
        currently constructed graph instance.
    */
    Process();

    /** \brief Constructs process node for specified graph.
    
        Process nodes can also be defined outside RenderGraph class. In such
        case, specify parent graph to the constructor explicitly.
    */
    Process ( RenderGraph* pGraph );

    /** \brief Constructs process node for specified graph.

        This constructor allows to specify both target render graph and process
        index. This is usually not needed, as the graph assigns indices automatically.
        Use only if you know what you are doing.
    */
    Process ( RenderGraph* pGraph, std::uint32_t index );

    /**
        \brief Retrieves the index of the Process node.
        
        Returned index is equal to Vulkan index of the subpass,
        as Process node is actually a wrapper over a subpass.
    */

    std::uint32_t index() const;

    /**
        \brief Retrieves the parent render graph.
    */
    RenderGraph* graph() const;

    /** \brief Adds color output attachment to be produced from scratch. Any previous
        content of the attachment is destroyed.

        The second argument (mandatory) is initial value of the color. The
        value type depends on the format. For RGBA formats it is a structure
        of four float numbers, e.g. { 0.0f, 0.0f, 0.0f, 1.0f }. For single
        component formats it is just a number.

        The third optional argument specifies in which layout the attachment
        should be saved to be used by subsequent render passes. If you want to
        continue using it as attachment, leave default value. If you want to
        use it as a texture, change it to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.
        To transfer the image to different location, use
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL. Note that this argument is applicable
        for reusing the attachment in completely different render pass -
        not an input node in current pass. For the latter usage you do not need to
        specify layout explicitly, as Vulkan will do necessary layout conversion
        automatically.
    */
    template< class FormatT >
    void addColorOutput (
        const Attachment< FormatT >& d,
        const typename FormatT::init_type& initValue,
        VkImageLayout destLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    /** \brief Adds color output attachment which has been already filled
        by previous render passes, to be modified by this render pass.

        Note that we mean previous render passes here, not previous Process
        nodes in current render graph. Use this method in multipass rendering
        engines, where several render graphs are defined and results are subsequently
        reused.

        The second optional argument specifies in which layout the attachment
        should be saved to be used by subsequent render passes.
    */
    template< class FormatT >
    void addReusedColorOutput (
        const Attachment< FormatT >& d,
        VkImageLayout destLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    /** \brief Adds color output attachment for display image.

        This variant initializes the display with black color.
    */
    void addColorOutput ( const Display& d );

    /** \brief Adds color output attachment for display image.

        This variant initializes the display with specified color.

        You can use vpp::clearColor() function to easily obtain
        a value of VkClearColorValue type.
    */
    void addColorOutput ( const Display& d, const VkClearColorValue& color );

    /** \brief Sets depth attachment to be produced from scratch. Any previous
        content of the attachment is destroyed.

        The second argument (mandatory) is initial value of the depth. The
        value type depends on the format, but usually it is just float.
        Specify 1.0f value in typical case, where minimum depth pixel
        (nearest to the observer) is wanted as a result.

        The third argument (optional) specifies whether the depth attachment
        should be preserved for use in more render passes. By default, the
        attachment is discarded after this pass. If you want to preserve it,
        specify in which layout it should be saved.
    */

    template< class FormatT >
    void setDepthOutput (
        const Attachment< FormatT >& d,
        const typename FormatT::init_type& initValue,
        VkImageLayout preserveInLayout = VK_IMAGE_LAYOUT_UNDEFINED );

    /** \brief Sets depth attachment which has been already filled by previous
        render passes, to be modified by this render pass.

        Note that we mean previous render passes here, not previous Process
        nodes in current render graph. Use this method in multipass rendering
        engines, where several render graphs are defined and results are subsequently
        reused. Typical example is separate D-pass, where depth information is
        collected first, and then used to optimize proper rendering of the scene.
        This method allows additional modification of the depth.

        The second optional argument specifies whether the depth attachment
        should be preserved for use in more render passes. By default, the
        attachment is discarded after this pass. If you want to preserve it,
        specify in which layout it should be saved.
    */

    template< class FormatT >
    void setReusedDepthOutput (
        const Attachment< FormatT >& d,
        VkImageLayout preserveInLayout = VK_IMAGE_LAYOUT_UNDEFINED );

    /** \brief Sets depth attachment which has been already filled by previous
        render passes, to be read by this render pass. The data is read-only.

        Note that we mean previous render passes here, not previous Process
        nodes in current render graph. Use this method in multipass rendering
        engines, where several render graphs are defined and results are subsequently
        reused. Typical example is separate D-pass, where depth information is
        collected first, and then used to optimize proper rendering of the scene.
        This method is applicable for the final pass, where depth is not modified
        any more.
    */
    template< class FormatT >
    void setDepthInput ( const Attachment< FormatT >& d );

    template< class FormatT >
    void addColorAndResolveOutput (
        const Attachment< FormatT >& dc,
        const Attachment< FormatT >& dr,
        const typename FormatT::init_type& initValue );

    void addPreservedOutput ( const BaseAttachment& d );

    /**
        \brief Adds input attachment to be read by this process.

        The attachment should be filled by another Process node inside current render graph.
        In that process, the attachment may perform a role of color or depth output
        attachment.

        This method is used to pass partial results inside single render graph,
        as opposed to methods like setDepthInput() or addReusedColorOutput() which
        are meant for usage with multiple render passes.

        The first argument is the attachment node reference.

        The second argument is the current layout of the attachment. It may be
        one of the following layouts:

        - VK_IMAGE_LAYOUT_GENERAL (default),
        - VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        - VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.

        See chapter 13.1.11 of the official Vulkan specification for details.

        The third argument specifies whether VPP should automatically add
        a dependency. The dependency is always required for input attachment.
        By default, VPP adds a dependency from ATTACHMENT_WRITE to SHADER_READ
        stage with by-region mode enabled. If you want to configure the dependency
        manually, pass false value here and use addDependency method to add the
        dependency from the producer process to this process. Otherwise just
        leave the default true value.
    */
    template< class FormatT >
    void addInput (
        const Attachment< FormatT >& d,
        VkImageLayout nodeImageLayout = VK_IMAGE_LAYOUT_GENERAL,
        bool bAddDependency = true );

    /**
        \brief Adds a dependency arc from current process to specified process.

        Note that \c srcSubpass and \c dstSubpass members of provided
        \c VkSubpassDependency structure are ignored. The indices are taken
        from source and target processes. You can just assign zero for these
        members.
    */

    void addDependency (
        const Process& targetProcess,
        const VkSubpassDependency& dependency );
};

// -----------------------------------------------------------------------------
/**
    \brief Represents preparation process graph node.

    This node performs optional preparation before rendering. It does not
    have any attachments, pipelines, resources nor dependencies. It has
    only a command sequence, supplied in the same way as for the Process node,
    by means of the << operator and C++ lambda function.

    Commands allowed in the sequence are somewhat different kind than
    the ones meant to be used with Process nodes. These are so called non-rendering
    commands. In Vulkan specification they are being refered as executed
    outside render pass instance. Among these commands are mainly various
    clear, fill and copy commands. Also some auxiliary rendering commands
    may be used here: parameter and push constant setting, synchronization,
    image layout conversion, resource binding, pipeline selection and some
    others. There are no draw commands allowed, though.

    This sequence is executed before any render pass.
*/

class Preprocess
{
public:
    /** \brief Constructs preprocess node for currently defined graph.
    
        Place a Preprocess node in your render graph class (derived from RenderGraph).
        Do not specify any arguments to the constructor. This is the recommended
        method of adding nodes. The constructor will automatically determine
        currently constructed graph instance.
    */
    Preprocess();

    /** \brief Constructs preprocess node for specified graph.
    
        Preprocess nodes can also be defined outside RenderGraph class. In such
        case, specify parent graph to the constructor explicitly.
    */
    Preprocess ( RenderGraph* pGraph );

    /**
        \brief Retrieves the index of the Preprocess node.
        
        Preprocess nodes have separate indexes from other types of nodes.
    */

    std::uint32_t index() const;

    /**
        \brief Retrieves the parent render graph.
    */
    RenderGraph* graph() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Represents finalization process graph node.

    This node performs optional finalization after rendering. It is
    very similar to Preprocess, with the difference that it is being
    executed after all render passes. Possible uses are e.g. copying
    some data back to CPU side, preparing the data for next render
    pass in chain, etc.
*/

class Postprocess
{
public:
    /** \brief Constructs postprocess node for currently defined graph.
    
        Place a Postprocess node in your render graph class (derived from RenderGraph).
        Do not specify any arguments to the constructor. This is the recommended
        method of adding nodes. The constructor will automatically determine
        currently constructed graph instance.
    */
    Postprocess();

    /** \brief Constructs postprocess node for specified graph.
    
        Postprocess nodes can also be defined outside RenderGraph class. In such
        case, specify parent graph to the constructor explicitly.
    */
    Postprocess ( RenderGraph* pGraph );

    /**
        \brief Retrieves the index of the Postprocess node.
        
        Postprocess nodes have separate indexes from other types of nodes.
    */
    std::uint32_t index() const;

    /**
        \brief Retrieves the parent render graph.
    */
    RenderGraph* graph() const;
};

// -----------------------------------------------------------------------------

/**
    \brief Defines a command sequence for rendering process.
*/

void operator<< ( const Process& target, const std::function< void () >& cmds );

/**
    \brief Defines a command sequence for preparation process.
*/

void operator<< ( const Preprocess& target, const std::function< void () >& cmds );

/**
    \brief Defines a command sequence for finalization process.
*/
void operator<< ( const Postprocess& target, const std::function< void () >& cmds );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
