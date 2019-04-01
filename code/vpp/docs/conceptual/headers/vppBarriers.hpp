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
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Utility base class for barrier classes.

    Contains some type definitions. Do not use directly.
*/

class Bar
{
public:
    /**
        \brief Enumeration specifying how the resource was (or will be) accessed.
    */

    enum EStage
    {
        /**
            \brief Resource has never been accessed before, or its content does not matter.
        
            This is valid only for source stages when specifying image barriers. The purpose
            is to trigger initializing a layout from undefined layout. This operation is
            performed automatically by VPP during e.g. clearing an image. You will rarely need
            to use this.
        */
        NONE,

        /**
            \brief Resource is accessed during processing of indirect draw commands.
        
            This is valid only for indirect command buffers. The resource should be a buffer
            with Buf::INDIRECT usage enabled. Also it makes sense only as a barrier destination
            stage, where the buffer is being read. Nothing can be written during indirect
            command processing. It is a fixed pipeline stage.
        */
        INDIRECT,

        /**
            \brief Resource is accessed during gathering vertices for a draw command.
        
            This is valid only for vertex and index buffers. The resource should be a buffer
            with Buf::VERTEX or Buf::INDEX usage enabled. Also it makes sense only as a barrier
            destination stage, where the buffer is being read. Nothing can be written during vertex
            processing. It is a fixed pipeline stage.
        */
        VTXIN,

        /**
            \brief Resource is accessed during vertex shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
        */
        VSHADER,

        /** \brief Same as VSHADER. */
        VERTEX,

        /**
            \brief Resource is accessed during tessellation control shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
        */
        TCSHADER,

        /**
            \brief Resource is accessed during tessellation evaluation shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
        */
        TESHADER,

        /**
            \brief Resource is accessed during geometry shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
        */
        GSHADER,

        /** \brief Same as GSHADER. */
        GEOMETRY,

        /**
            \brief Resource is accessed during fragment shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
            
            Note that this is not the stage where depth or color attachments are being processed.
            There are separate stages for that. This stage covers only shader code execution.
        */
        FSHADER,

        /** \brief Same as FSHADER. */
        FRAGMENT,

        /**
            \brief Resource is accessed during early fragment tests.

            Resource must be an image with Img::DEPTH usage enabled. The only access possible
            during this stage is reading (and possibly modification) of a depth/stencil image.
            Other types of resources make no sense here and are forbidden.
        */
        EDEPTH,

        /**
            \brief Resource is accessed during late fragment tests.

            Resource must be an image with Img::DEPTH usage enabled. The only access possible
            during this stage is reading (and possibly modification) of a depth/stencil image.
            Other types of resources make no sense here and are forbidden.
        */
        LDEPTH,

        /**
            \brief Resource is accessed during color output.

            Resource must be an image with Img::COLOR usage enabled. The only access possible
            during this stage is reading/writing of a color image. Other types of resources
            make no sense here and are forbidden.
        */
        CLROUT,

        /**
            \brief Resource is accessed during compute shader execution.

            Can be any buffer or image. For barrier source stage, where the resource is being
            written, it must not be read-only.
        */
        CSHADER,

        /** \brief Same as CSHADER. */
        COMPUTE,

        /**
            \brief Resource is accessed during data transfer.

            Data transfer usage covers various transfer, clear, fill and resolve operations
            on images and buffers (including commits and loads on vpp::gvector objects).

            The resource can be any buffer or image. For barrier source stage, where
            the resource is being written, it must not be read-only.
        */
        TRANSFER,

        /**
            \brief Resource is accessed from the CPU side.

            This covers scenarios where the image or buffer has been allocated in
            memory shared between CPU and GPU and is being accessed from both sides.
            The barrier ensures proper synchronization between to sides. For non-coherent
            memory blocks, additional call to flush / invalidate memory routine might
            be necessary (vkFlushMappedMemoryRanges or vkInvalidateMappedMemoryRanges).
            The vpp::gvector container does it automatically.

            The resource can be any buffer or image. For barrier source stage, where
            the resource is being written, it must not be read-only.
        */
        HOST
    };
};

// -----------------------------------------------------------------------------

/**
    \brief General class encapsulating description of a set of pipeline barriers.

    In Vulkan you can issue multiple barriers in single call. The vpp::Barriers class
    is a unified interface to barrier descriptions. It is accepted by general barrier
    creating command: vpp::UniversalCommands::cmdPipelineBarrier().

    Caution: vpp::Barriers does not take ownership of the data (it does not make a copy
    in memory), although some of its derived classes do.

    Most of the time you will want to use one of the derived subclasses. Direct usage
    of vpp::Barriers is required only for complex, muilti-barrier scenarios where
    you need to specify barriers using low-level Vulkan structures.
*/

class Barriers : public Bar
{
public:
    /** \brief Constructs empty barrier container. */
    Barriers();

    /** \brief Constructs a copy of other barrier container. */
    Barriers ( const Barriers& rhs );

    /** \brief Assigns a set of Vulkan global memory barriers.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the vector, only references it.
    */
    void setBarriers ( const std::vector< VkMemoryBarrier >& bar );

    /** \brief Assigns a set of Vulkan buffer barriers.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the vector, only references it.
    */
    void setBarriers ( const std::vector< VkBufferMemoryBarrier >& bar );

    /** \brief Assigns a set of Vulkan image barriers.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the vector, only references it.
    */
    void setBarriers ( const std::vector< VkImageMemoryBarrier >& bar );

    /** \brief Assigns single Vulkan global memory barrier.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the structure, only references it.
    */
    void setBarriers ( const VkMemoryBarrier& bar );

    /** \brief Assigns single Vulkan buffer barrier.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the structure, only references it.
    */
    void setBarriers ( const VkBufferMemoryBarrier& bar );

    /** \brief Assigns single Vulkan image barrier.
    
        Multiple invocations overwrite previously assigned sets. The memory,
        buffer and image categories are assigned independently of each other.

        Does not copy the structure, only references it.
    */
    void setBarriers ( const VkImageMemoryBarrier bar );

    /**
        \brief Constructs barriers object from supplied data (one element).

        It is equivalent to constructing empty container and calling setBarriers().
        You can specify either single Vulkan barrier structure of any type or
        a vector.
    */
    template< typename BarriersA >
    Barriers ( const BarriersA& ba );

    /**
        \brief Constructs barriers object from supplied data (two elements).

        It is equivalent to constructing empty container and calling setBarriers().
        You can specify either Vulkan barrier structures of any type or
        vectors.
    */
    template< typename BarriersA, typename BarriersB >
    Barriers ( const BarriersA& ba, const BarriersB& bb );

    /**
        \brief Constructs barriers object from supplied data (three elements).

        It is equivalent to constructing empty container and calling setBarriers().
        You can specify either Vulkan barrier structures of any type or
        vectors.
    */
    template< typename BarriersA, typename BarriersB, typename BarriersC >
    Barriers ( const BarriersA& ba, const BarriersB& bb, const BarriersC& bc );
};

// -----------------------------------------------------------------------------
/**
    \brief Single global memory barrier.

    A convenience subclass for Barriers. Holds single global memory barrier.
    Remembers all the required data.
*/

class MemoryBarrier :
    public VkMemoryBarrier,
    public Barriers
{
    MemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask );
};

// -----------------------------------------------------------------------------
/**
    \brief Single buffer barrier.

    A convenience subclass for Barriers. Holds single buffer barrier.
    Remembers all the required data.
*/

class BufferMemoryBarrier :
    public VkBufferMemoryBarrier,
    public Barriers
{
    BufferMemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        const Buf& buffer,
        VkDeviceSize offset = 0,
        VkDeviceSize size = VK_WHOLE_SIZE,
        uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED );
};

// -----------------------------------------------------------------------------
/**
    \brief Single image barrier.

    A convenience subclass for Barriers. Holds single image barrier.
    Remembers all the required data.
*/

class ImageMemoryBarrier :
    public VkImageMemoryBarrier,
    public Barriers
{
    ImageMemoryBarrier ( 
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        const Img& image,
        std::uint32_t baseArrayLayer = 0,
        std::uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS,
        std::uint32_t baseMipLevel = 0,
        std::uint32_t levelCount = VK_REMAINING_MIP_LEVELS,
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED );
};

// -----------------------------------------------------------------------------
/**
    \brief Multiple barriers with default parameters.

    A convenience subclass for Barriers. Holds multiple barriers of any types.
    Remembers all the required data. Automatically guesses most parameters.

    Use this class indirectly, by calling vpp::barriers() function template
    which constructs it. This is the variant easiest to use, requiring to
    specify minimum set of parameters. Access masks and layouts are detected
    automatically, which is appropriate for common scenarios.
*/

class BarrierList : public Barriers
{
};

// -----------------------------------------------------------------------------
/**
    \brief Creates a BarrierList object for specified resources.

    This function requires only source and destination stages, and also a list
    of resource references (vpp::Buf or vpp::Img). You can provide vpp::Bar::EStage
    values as the stages (they are equivalent to Vulkan enumeration). 

    Caution: this function throws vpp::XInvalidBarrier exception when it detects
    an error like invalid stage for particular type of resource, or lack of required
    usage flags on the resource. This situation indicates a bug in your program.

    An example:

    \code
        typedef vpp::gvector< unsigned int, vpp::Buf::STORAGE | vpp::Buf::TARGET > DataBuffer;

        DataBuffer m_sortResultBuffer;
        DataBuffer m_generateResultBuffer;
        DataBuffer m_reduceResultBuffer;

        // ...
        // inside the command sequence lambda:

        // this computation kernel fills the buffers:
        m_dataBlock.cmdBind();
        pipeline ( 0 ).cmdBind();
        cmdDispatch ( 64, 1, 1 );

        // this barrier ensures data is ready
        cmdPipelineBarrier ( barriers (
            Bar::COMPUTE, Bar::TRANSFER,
            m_sortResultBuffer, m_generateResultBuffer, m_reduceResultBuffer ) );

        // these calls transfer buffers back to the host
        m_sortResultBuffer.cmdLoadAll();
        m_generateResultBuffer.cmdLoadAll();
        m_reduceResultBuffer.cmdLoadAll();

    \endcode
*/

template< typename ... Args >
BarrierList barriers (
    VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage,
    Args&& ... args );

// -----------------------------------------------------------------------------
/**
    \brief Creates a BarrierList object for specified regions of an image.

    This function requires source and destination stages, the image and a list
    of its regions. You can provide vpp::Bar::EStage values as the stages
    (they are equivalent to Vulkan enumeration).

    For each provided region, separate barrier will be created.

    Caution: this function throws vpp::XInvalidBarrier exception when it detects
    an error like invalid stage for particular type of resource, or lack of required
    usage flags on the resource. This situation indicates a bug in your program.
*/

BarrierList barriers (
    const Img& hImage,
    const std::vector< VkImageSubresourceRange >& regions,
    VkPipelineStageFlags sourceStage, VkPipelineStageFlags targetStage );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
