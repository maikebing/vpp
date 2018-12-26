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
    \brief Set of commands that may be used outside rendering context.

    Rendering context occurs inside lambda functions passed to the Process
    nodes. Commands described below should not be used there, however they may
    be used inside Preprocess and Postprocess lambda functions, as well as
    outside all of these lambda functions.

    All VPP commands accept optional CommandBuffer argument. This argument
    is always the last one. The rules regarding the argument are as follows:
    - Inside lambda functions passed to Preprocess or Postprocess,
      omit the argument. VPP will record the command into the implicit
      command buffer managed by VPP.
    - Outside lambda functions, provide a CommandBuffer object for this
      argument. The command will be recorded into supplied command buffer.

    The commands are implemented as static methods inside NonRenderingCommands
    class. It allows to easily group these commands. Derive from NonRenderingCommands
    if you need to call the commands frequently, or alternatively prefix
    the names with <tt>NonRenderingCommands::</tt>.
*/

class NonRenderingCommands
{
public:
    /**
        \brief Fills specified buffer (or a region of it) with constant value.
    */
    static void cmdFillBuffer (
        const Buf& hDstBuffer,
        size_t offset, size_t size, std::uint32_t value,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies entire contents from one buffer to another.
    */
    static void cmdCopyBuffer (
        const Buf& hSrcBuffer,
        const Buf& hDstBuffer,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies a list of regions from one buffer to another.
    */
    static void cmdCopyBuffer (
        const Buf& hSrcBuffer,
        const Buf& hDstBuffer,
        const std::vector< VkBufferCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Modifies a buffer region with specified data.

        Caution: maximum size of data block is limited. Assume that at most
        64 kB of data can be written this way.
    */
    static void cmdUpdateBuffer (
        const Buf& hDstBuffer,
        size_t offset, size_t size, const std::uint32_t* pData,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies entire contents from one image to another.
    */
    static void cmdCopyImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies a list of regions from one image to another.
    */
    static void cmdCopyImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies entire contents from a buffer to an image.
    */
    static void cmdCopyBufferToImage (
        const Buf& hSrcBuffer,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies a list of regions from a buffer to an image.
    */
    static void cmdCopyBufferToImage (
        const Buf& hSrcBuffer,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkBufferImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies entire contents from an image to a buffer.
    */
    static void cmdCopyImageToBuffer (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Buf& hDstBuffer,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies a list of regions from an image to a buffer.
    */
    static void cmdCopyImageToBuffer (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Buf& hDstBuffer,
        const std::vector< VkBufferImageCopy >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies source image region into destination image,
               potentially performing format conversion, arbitrary scaling
               and filtering.
    */
    static void cmdBlitImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
        const VkOffset3D& dstBegin, const VkOffset3D& dstEnd,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies source image region into destination image,
               potentially performing format conversion, but no scaling.
    */
    static void cmdBlitImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcBegin, const VkOffset3D& srcEnd,
        const VkOffset3D& dstBegin,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Copies source image regions into destination image,
               potentially performing format conversion, arbitrary scaling
               and filtering.
    */
    static void cmdBlitImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageBlit >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Resolves source multisampled image region into destination regular image.

        This command does not perform any format conversions, nor scaling. Both
        images must be in same format.

        \c srcImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL

        \c dstImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL.
    */
    static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const VkOffset3D& srcOffset, const VkOffset3D& dstOffset,
        const VkExtent3D& size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Resolves source multisampled image into destination regular image.

        This command does not perform any format conversions, nor scaling. Both
        images must be in same format.

        \c srcImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL

        \c dstImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL.
    */
    static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Resolves a list of source multisampled image regions into destination regular image regions.

        This command does not perform any format conversions, nor scaling. Both
        images must be in same format.

        \c srcImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL

        \c dstImageLayout must be \c VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL or \c VK_IMAGE_LAYOUT_GENERAL.
    */
    static void cmdResolveImage (
        const Img& hSrcImage,
        VkImageLayout srcImageLayout,
        const Img& hDstImage,
        VkImageLayout dstImageLayout,
        const std::vector< VkImageResolve >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Clears an image with specified color.
    */
    static void cmdClearColorImage (
        const Img& hImage,
        VkImageLayout imageLayout,
        const VkClearColorValue& color,
        CommandBuffer hCmdBuffer = CommandBuffer() );
    
    /**
        \brief Clears a list of regions in specified image with specified color.
    */
    static void cmdClearColorImage (
        const Img& hImage,
        VkImageLayout imageLayout,
        const VkClearColorValue& color,
        const std::vector< VkImageSubresourceRange >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Clears a depth or stencil image with specified value.
    */
    static void cmdClearDepthStencilImage (
        const Img& hImage,
        VkImageLayout imageLayout,
        float depth, std::uint32_t stencil,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Clears a list of regions in depth or stencil image with specified value.
    */
    static void cmdClearDepthStencilImage (
        const Img& hImage,
        VkImageLayout imageLayout,
        float depth, std::uint32_t stencil,
        const std::vector< VkImageSubresourceRange >& regions,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Resets a range of queries in a query pool.
    */
    static void cmdResetQueryPool (
        const QueryPool& hPool,
        std::uint32_t first,
        std::uint32_t count,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    /**
        \brief Resets all queries in a query pool.
    */
    static void cmdResetQueryPool (
        const QueryPool& hPool,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdCopyQueryPoolResults (
        const Buf& hDstBuffer,
        const QueryPool& hPool,
        VkQueryResultFlags flags,
        std::uint32_t count,
        std::uint32_t first,
        VkDeviceSize dstOffset,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdCopyQueryPoolResults (
        const Buf& hDstBuffer,
        const QueryPool& hPool,
        VkQueryResultFlags flags,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
/**
    \brief Set of commands that may be used anywhere (inside or outside rendering
           context).

    These commands can be issued in any context, rendering or not.

    All VPP commands accept optional CommandBuffer argument. This argument
    is always the last one. The rules regarding the argument are as follows:
    - Inside lambda functions passed to Process, Preprocess or Postprocess,
      omit the argument. VPP will record the command into the implicit
      command buffer managed by VPP.
    - Outside lambda functions, provide a CommandBuffer object for this
      argument. The command will be recorded into supplied command buffer.

    The commands are implemented as static methods inside UniversalCommands
    class. It allows to easily group these commands. Derive from UniversalCommands
    if you need to call the commands frequently, or alternatively prefix
    the names with <tt>UniversalCommands::</tt>.
*/

class UniversalCommands
{
public:
    static void cmdExecuteCommands (
        const VkCommandBuffer* pBuffers,
        size_t size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdExecuteCommands (
        const CommandBuffer* pBuffers,
        size_t size,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdExecuteCommands (
        const std::vector< CommandBuffer >& buffers,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdWaitEvents (
        const std::vector< Event >& events,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        const Barriers& barriers = Barriers(),
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdPipelineBarrier (
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkDependencyFlags dependencyFlags,
        const Barriers& barriers,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdImagePipelineBarrier (
        const Img& image,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        bool bByRegion,
        VkImageLayout oldLayout = VK_IMAGE_LAYOUT_GENERAL,
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_GENERAL,
        int mipLevel = -1,
        int arrayLayer = -1,
        CommandBuffer hCmdBuffer = CommandBuffer()
    );

    static void cmdBufferPipelineBarrier (
        const Buf& hBuffer,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        CommandBuffer hCmdBuffer = CommandBuffer()
    );

    static void cmdSetViewport (
        const VkViewport& viewport,
        std::uint32_t index = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetViewport (
        const std::vector< VkViewport >& viewports,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetLineWidth (
        float lineWidth,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetDepthBias (
        float constantFactor,
        float clamp,
        float slopeFactor,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetDepthBounds (
        float minDepthBounds,
        float maxDepthBounds,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetScissor (
        const VkRect2D& scissor,
        std::uint32_t index = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetScissor (
        const std::vector< VkRect2D >& scissors,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetStencilCompareMask (
        VkStencilFaceFlags faceMask,
        std::uint32_t compareMask,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetStencilWriteMask ( 
        VkStencilFaceFlags faceMask,
        std::uint32_t writeMask,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetStencilReference ( 
        VkStencilFaceFlags faceMask,
        std::uint32_t reference,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdSetBlendConstants (
        const float blendConstants [ 4 ],
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBeginQuery (
        const QueryPool& hPool,
        std::uint32_t query,
        VkQueryControlFlags flags,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdEndQuery (
        const QueryPool& hPool,
        std::uint32_t query,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdWriteTimestamp (
        const QueryPool& hPool,
        std::uint32_t query,
        VkPipelineStageFlagBits pipelineStage,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBindPipeline (
        const Pipeline& hPipeline,
        VkPipelineBindPoint pipelineBindPoint,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBindDescriptorSets (
        const ShaderDataBlock& hDataBlock,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBindIndexInput (
        const Buf& hBuffer,
        VkIndexType indexType,
        VkDeviceSize offset,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBindVertexBuffers (
        const std::vector< Buf >& buffers,
        const std::vector< VkDeviceSize >& offsets,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdBindVertexBuffers (
        const std::vector< Buf >& buffers,
        std::uint32_t first = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    static void cmdPushConstants (
        const PipelineLayoutBase& hPipelineLayout,
        VkShaderStageFlags stageFlags,
        const void* pValues,
        std::uint32_t size,
        std::uint32_t offset = 0,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ExtendedCommands :
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    static void cmdChangeImageLayout (
        const Img& hImage,
        VkImageLayout oldImageLayout, 
        VkImageLayout newImageLayout,
        std::uint32_t levelCount = 1,
        std::uint32_t layerCount = 1,
        std::uint32_t baseMipLevel = 0,
        std::uint32_t baseArrayLayer = 0,
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        CommandBuffer hCmdBuffer = CommandBuffer() );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
