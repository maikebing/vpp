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

#ifndef INC_VPPRENDERGRAPH_HPP
#define INC_VPPRENDERGRAPH_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMANDS_HPP
#include "vppCommands.hpp"
#endif

#ifndef INC_VPPIMAGEINFO_HPP
#include "vppImageInfo.hpp"
#endif

#ifndef INC_VPPDEVICEMEMORY_HPP
#include "vppDeviceMemory.hpp"
#endif

#ifndef INC_VPPATTACHMENTCONFIG_HPP
#include "vppAttachmentConfig.hpp"
#endif

#ifndef INC_VPPCOMMANDBUFFER_HPP
#include "vppCommandBuffer.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct SProcessInfo
{
public:
    SProcessInfo ( std::uint32_t processIndex );

public:
    std::uint32_t d_processIndex;

    std::vector< VkAttachmentReference > d_inputNodes;
    std::vector< VkAttachmentReference > d_outputColorNodes;
    std::vector< VkAttachmentReference > d_outputResolveNodes;
    std::vector< VkAttachmentReference > d_outputDepthNodes;
    std::vector< std::uint32_t > d_outputPreserveNodes;

    std::vector< std::function< void () > > d_commands;
};

// -----------------------------------------------------------------------------

VPP_INLINE SProcessInfo :: SProcessInfo ( std::uint32_t processIndex ) :
    d_processIndex ( processIndex )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SPrePostProcessInfo
{
public:
    SPrePostProcessInfo ( std::uint32_t processIndex );

public:
    std::uint32_t d_processIndex;
    std::vector< std::function< void () > > d_commands;
};

// -----------------------------------------------------------------------------

VPP_INLINE SPrePostProcessInfo :: SPrePostProcessInfo ( std::uint32_t processIndex ) :
    d_processIndex ( processIndex )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class RenderGraphImpl;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class RenderGraph :
    public TSharedReference< RenderGraphImpl >,
    public NonRenderingCommands,
    public UniversalCommands
{
public:
    VPP_DLLAPI RenderGraph();
    VPP_DLLAPI ~RenderGraph();

    typedef std::function< void () > FCommand;
    typedef std::vector< FCommand > Commands;

public:
    VPP_DLLAPI VkAttachmentDescription& getAttachmentDescription ( std::uint32_t index );

    VPP_DLLAPI std::uint32_t getProcessCount() const;
    VPP_DLLAPI std::uint32_t getPreprocessCount() const;
    VPP_DLLAPI std::uint32_t getPostprocessCount() const;
    
    VPP_DLLAPI const Commands& getProcessCommands ( std::uint32_t index ) const;
    VPP_DLLAPI const Commands& getPreprocessCommands ( std::uint32_t index ) const;
    VPP_DLLAPI const Commands& getPostprocessCommands ( std::uint32_t index ) const;

    VPP_DLLAPI std::uint32_t getAttachmentCount() const;
    VPP_DLLAPI ImageInfo& getAttachmentInfo ( size_t index );
    VPP_DLLAPI const ImageInfo& getAttachmentInfo ( size_t index ) const;

    VPP_DLLAPI SProcessInfo& getProcessInfo ( size_t index );
    VPP_DLLAPI const SProcessInfo& getProcessInfo ( size_t index ) const;

    VPP_DLLAPI const KAttachmentConfig& getAttachmentConfig() const;
    VPP_DLLAPI const SubpassDescriptions& getSubpassDescriptions() const;
    VPP_DLLAPI const SubpassDependencies& getSubpassDependencies() const;
    VPP_DLLAPI const ClearValues& getClearValues() const;

    VPP_DLLAPI std::uint32_t findInputLocation (
        std::uint32_t processIndex, std::uint32_t nodeIndex ) const;

    VPP_DLLAPI std::uint32_t findOutputLocation (
        std::uint32_t processIndex, std::uint32_t nodeIndex ) const;

public:
    VPP_DLLAPI static void cmdDraw (
        std::uint32_t vertexCount,
        std::uint32_t instanceCount,
        std::uint32_t firstVertex,
        std::uint32_t firstInstance,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdDrawIndexed (
        std::uint32_t indexCount,
        std::uint32_t instanceCount,
        std::uint32_t firstIndex,
        std::int32_t vertexOffset,
        std::uint32_t firstInstance,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdDrawIndirect (
        const IndirectBufferView& data,
        VkDeviceSize offset,
        std::uint32_t drawCount,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdDrawIndexedIndirect (
        const IndirectBufferView& data,
        VkDeviceSize offset,
        std::uint32_t drawCount,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI static void cmdClearImages (
        std::uint32_t attachmentCount,
        const VkClearAttachment* pAttachments,
        std::uint32_t rectCount,
        const VkClearRect* pRects,
        CommandBuffer hCmdBuffer = CommandBuffer() );

    VPP_DLLAPI const FrameBuffer& framebuffer();

private:
    friend class BaseAttachment;
    friend class Process;
    friend class Preprocess;
    friend class Postprocess;
    friend class Dependency;
    friend class Stage;
    friend VPP_DLLAPI void operator<< ( const Process& target, const std::function< void () >& cmds );
    friend VPP_DLLAPI void operator<< ( const Preprocess& target, const std::function< void () >& cmds );
    friend VPP_DLLAPI void operator<< ( const Postprocess& target, const std::function< void () >& cmds );

    static RenderGraph* getInstance();

    VPP_DLLAPI std::uint32_t registerAttachment (
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose,
        VkFormat fmt );

    VPP_DLLAPI std::uint32_t registerAttachment ( const ImageInfo& imageInfo );
    VPP_DLLAPI std::uint32_t registerAttachment ( const FrameImageView& imageView );

    VPP_DLLAPI std::uint32_t registerProcess();
    VPP_DLLAPI std::uint32_t registerPreprocess();
    VPP_DLLAPI std::uint32_t registerPostprocess();

    VPP_DLLAPI void addProcessInputNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex,
        VkImageLayout nodeImageLayout,
        bool bAddDependency );

    VPP_DLLAPI void addProcessOutputColorNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex,
        VkImageLayout destLayout );

    VPP_DLLAPI void addProcessOutputColorNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex,
        const VkClearValue& clearValue,
        VkImageLayout destLayout );

    VPP_DLLAPI void addProcessOutputColorAndResolveNode (
        std::uint32_t processIndex,
        std::uint32_t colorNodeIndex,
        std::uint32_t resolveNodeIndex,
        const VkClearValue& clearValue );

    VPP_DLLAPI void addProcessOutputDepthNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex,
        const VkClearValue* pClearValue,
        VkImageLayout preserveInLayout );

    VPP_DLLAPI void addProcessInputDepthNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex );

    VPP_DLLAPI void addProcessOutputPreserveNode (
        std::uint32_t processIndex,
        std::uint32_t nodeIndex );

    VPP_DLLAPI std::uint32_t addProcessDependency (
        std::uint32_t from,
        std::uint32_t to,
        const VkSubpassDependency* pDependency );

    VPP_DLLAPI void setAllGraphicsDependency ( std::uint32_t depIndex );
    VPP_DLLAPI void setByPixelDependency ( std::uint32_t depIndex );

    VPP_DLLAPI void addProcessCommands (
        std::uint32_t processIndex, const std::function< void () >& cmds );

    VPP_DLLAPI void addPreprocessCommands (
        std::uint32_t preprocessIndex, const std::function< void () >& cmds );

    VPP_DLLAPI void addPostprocessCommands (
        std::uint32_t postprocessIndex, const std::function< void () >& cmds );

private:
    friend class RenderGraphImpl;

    typedef std::map<
        std::pair< std::uint32_t, std::uint32_t >,
        std::uint32_t
    > Process2SubpassDependency;

private:
    static thread_local RenderGraph* d_pThis;
};

// -----------------------------------------------------------------------------

VPP_INLINE RenderGraph* RenderGraph :: getInstance()
{
    return d_pThis;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class RenderGraphImpl : public TSharedObject< RenderGraphImpl >
{
public:
    VPP_DLLAPI RenderGraphImpl();
    VPP_DLLAPI ~RenderGraphImpl();

private:
    friend class RenderGraph;

    std::vector< SProcessInfo* > d_processNodes;
    std::vector< SPrePostProcessInfo > d_preprocessNodes;
    std::vector< SPrePostProcessInfo > d_postprocessNodes;

    KAttachmentConfig d_attachmentConfig;
    RenderGraph::Process2SubpassDependency d_processDependencies;
    SubpassDescriptions d_subpassDescriptions;
    SubpassDependencies d_subpassDependencies;
    ClearValues d_clearValues;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPRENDERGRAPH_HPP
