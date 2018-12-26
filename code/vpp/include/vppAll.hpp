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

#ifndef INC_VPPALL_HPP
#define INC_VPPALL_HPP

// -----------------------------------------------------------------------------

#include "vppCommon.hpp"
#include "vppExtSync.hpp"
#include "vppSharedObject.hpp"
#include "vppMutex.hpp"
#include "vppTypes.hpp"

#include "vppExceptions.hpp"
#include "vppGeometry.hpp"
#include "vppImageInfo.hpp"

#include "vppBarriers.hpp"
#include "vppBlendOperators.hpp"
#include "vppInitialValues.hpp"
#include "vppCommandBuffer.hpp"
#include "vppViewport.hpp"

#include "vppFormats.hpp"
#include "vppPhysicalDevice.hpp"
#include "vppDevice.hpp"
#include "vppDeviceMemory.hpp"
#include "vppBuffer.hpp"
#include "vppInstance.hpp"
#include "vppSurface.hpp"
#include "vppRenderingOptions.hpp"

#include "vppCommandPool.hpp"
#include "vppPipeline.hpp"
#include "vppPipelineCache.hpp"
#include "vppQueryPool.hpp"
#include "vppSynchronization.hpp"
#include "vppImage.hpp"
#include "vppUsageChecks.hpp"
#include "vppCommands.hpp"
#include "vppSampler.hpp"
#include "vppQueue.hpp"
#include "vppFrameImageView.hpp"
#include "vppAttachmentConfig.hpp"

#include "vppRenderGraph.hpp"
#include "vppRenderGraphNodes.hpp"

#include "vppShaderModule.hpp"
#include "vppLangTranslator.hpp"
#include "vppLangScalarTypes.hpp"
#include "vppLangAccessor.hpp"
#include "vppLangVectorTypes.hpp"
#include "vppLangMatrixTypes.hpp"
#include "vppLangConstructs.hpp"
#include "vppLangConversions.hpp"
#include "vppLangFunctions.hpp"

#include "vppBufferView.hpp"
#include "vppImageView.hpp"

#include "vppPipelineConfig.hpp"
#include "vppShader.hpp"
#include "vppPipelineLayout.hpp"
#include "vppShaderDataBlock.hpp"
#include "vppFramebuffer.hpp"
#include "vppRenderPass.hpp"
#include "vppComputePass.hpp"
#include "vppSwapChain.hpp"

#include "vppLangAggregates.hpp"
#include "vppLangBuiltins.hpp"
#include "vppLangImages.hpp"
#include "vppLangImgFun.hpp"
#include "vppLangInterface.hpp"

#include "vppCompiledProcedures.hpp"
#include "vppComputationEngine.hpp"
#include "vppImageOperations.hpp"
#include "vppContainers.hpp"

#include "vppCommandBufferRecorder.hpp"
#include "vppDebugReporter.hpp"
#include "vppRenderManager.hpp"

#include "vppWholeScreenPatch.hpp"

#include "vppSupportMath.hpp"

// -----------------------------------------------------------------------------

#endif // INC_VPPALL_HPP
