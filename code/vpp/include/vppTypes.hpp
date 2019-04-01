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

#ifndef INC_VPPTYPES_HPP
#define INC_VPPTYPES_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPSHAREDOBJECT_HPP
#include "vppSharedObject.hpp"
#endif

#ifndef INC_VPPMUTEX_HPP
#include "vppMutex.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

typedef std::vector< VkQueueFamilyProperties > QueueFamilyProperties;
typedef std::vector< VkAttachmentDescription > AttachmentDescriptions;
typedef std::vector< VkSubpassDescription > SubpassDescriptions;
typedef std::vector< VkSubpassDependency > SubpassDependencies;
typedef std::vector< VkClearValue > ClearValues;
typedef std::vector< VkImageView > ImageViews;
typedef std::vector< VkImage > Images;
typedef std::vector< VkDescriptorSet > DescriptorSets;
typedef std::vector< VkDescriptorPoolSize > DescriptorPoolSizes;
typedef std::vector< VkDescriptorSetLayout > DescriptorSetLayoutHandles;
typedef std::vector< VkGraphicsPipelineCreateInfo > PipelineCreateInfos;
typedef std::vector< VkComputePipelineCreateInfo > ComputePipelineCreateInfos;
typedef std::vector< VkSurfaceFormatKHR > SurfaceFormats;
typedef std::vector< VkPresentModeKHR > PresentationModes;
typedef std::vector< VkExtensionProperties > ExtensionProperties;

class Instance;
class PhysicalDevice;
class Device;
class DebugReporter;
struct SVulkanVersion;

class Surface;

class CommandPool;
class CommandBuffer;

class FrameBuffer;

class RenderGraph;
class RenderPass;
class ComputePass;

class Process;
class Preprocess;
class Postprocess;
class BaseAttachment;

class PipelineConfig;
class PipelineLayoutBase;
class PipelineCache;

class RenderingOptions;

class Buf;
class Img;

class ShaderDataBlock;

class QueryPool;
class Event;
class Barriers;
class Pipeline;

class FrameImageView;
class VertexBufferView;
class VertexIndexBufferView;
class UniformBufferView;
class IndirectBufferView;

class KShader;
struct SDynamicParameters;
class vertexShader;
class tessControlShader;
class tessEvalShader;
class geometryShader;
class fragmentShader;
class computeShader;

template< unsigned int USAGE > class Buffer;
template< class AttributesT > class Image;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct NoHostMember {};

// -----------------------------------------------------------------------------

template< typename MemberT >
struct StructMemberTraits
{
    static const bool has_member_info = true;
    static const bool is_unknown = true;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef NoHostMember scalar_type;
    typedef NoHostMember rvalue_type;
    typedef NoHostMember lvalue_type;
    typedef NoHostMember data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< NoHostMember >
{
    static const bool has_member_info = false;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = 0;
    static const unsigned int row_count = 0u;
    static const unsigned int column_count = 0u;
    typedef NoHostMember scalar_type;
    typedef NoHostMember rvalue_type;
    typedef NoHostMember lvalue_type;
    typedef NoHostMember data_type;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KId
{
public:
    explicit KId ( unsigned int value );
    operator unsigned int() const;

private:
    unsigned int d_value;
};

// -----------------------------------------------------------------------------

VPP_INLINE KId :: KId ( unsigned int value ) :
    d_value ( value )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KId :: operator unsigned int() const
{
    return d_value;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

enum ETessellationTopology
{
    TT_TRIANGLES,
    TT_QUADS,
    TT_ISOLINES
};

// -----------------------------------------------------------------------------

enum ETessellationSpacing
{
    TS_EQUAL,
    TS_FRACTIONAL_EVEN,
    TS_FRACTIONAL_ODD
};

// -----------------------------------------------------------------------------

enum EGeometryInputTopology
{
    GTI_POINTS,
    GTI_LINES,
    GTI_ADJLINES,
    GTI_TRIANGLES,
    GTI_ADJTRIANGLES
};

// -----------------------------------------------------------------------------

enum EGeometryOutputTopology
{
    GTO_POINTS,
    GTO_LINESTRIP,
    GTO_TRIANGLESTRIP
};

// -----------------------------------------------------------------------------

enum EShader
{
    SH_VERTEX,
    SH_TESS_CONTROL,
    SH_TESS_EVAL,
    SH_GEOMETRY,
    SH_FRAGMENT,
    SH_COMPUTE
};

// -----------------------------------------------------------------------------

enum EVariableFlags
{
    IN_FLAT = 1,
    IN_NOPERSPECTIVE = 2,
    IN_CENTROID = 4,
    IN_SAMPLE = 8,

    VAR_RELAXEDPR = 16
};

// -----------------------------------------------------------------------------

enum EMemoryQualifiers
{
    MEM_COHERENT = 1,
    MEM_VOLATILE = 2,
    MEM_RESTRICT = 4,
    MEM_READONLY = 8,
    MEM_WRITEONLY = 16
};

// -----------------------------------------------------------------------------

enum EQueueType
{
    Q_GRAPHICS,
    Q_TRANSFER,

    Q_count
};

// -----------------------------------------------------------------------------

struct SLocalGroupSize
{
    int x;
    int y;
    int z;
};

// -----------------------------------------------------------------------------

static const VkPresentModeKHR QM_IMMEDIATE = VK_PRESENT_MODE_IMMEDIATE_KHR;
static const VkPresentModeKHR QM_MAILBOX = VK_PRESENT_MODE_MAILBOX_KHR;
static const VkPresentModeKHR QM_FIFO = VK_PRESENT_MODE_FIFO_KHR;
static const VkPresentModeKHR QM_FIFO_RELAXED = VK_PRESENT_MODE_FIFO_RELAXED_KHR;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SVulkanVersion
{
    std::string toString() const;
    unsigned int id() const;

    bool operator== ( const SVulkanVersion& rhs ) const;
    bool operator!= ( const SVulkanVersion& rhs ) const;
    bool operator< ( const SVulkanVersion& rhs ) const;

    static SVulkanVersion fromId ( unsigned int id );

    unsigned int major;
    unsigned int minor;
    unsigned int patch;
};

// -----------------------------------------------------------------------------

VPP_INLINE std::string SVulkanVersion :: toString() const
{
    std::string result;
    result.reserve ( 10 );
    result.append ( std::to_string ( major ) );
    result.push_back ( '.' );
    result.append ( std::to_string ( minor ) );
    result.push_back ( '.' );
    result.append ( std::to_string ( patch ) );
    return result;
}

// -----------------------------------------------------------------------------

VPP_INLINE unsigned int SVulkanVersion :: id() const
{
    return VK_MAKE_VERSION( major, minor, patch );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool SVulkanVersion :: operator== ( const SVulkanVersion& rhs ) const
{
    return major == rhs.major && minor == rhs.minor && patch == rhs.patch;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool SVulkanVersion :: operator!= ( const SVulkanVersion& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------

VPP_INLINE bool SVulkanVersion :: operator< ( const SVulkanVersion& rhs ) const
{
    if ( major < rhs.major )
        return true;
    if ( rhs.major < rhs.major )
        return false;
    if ( minor < rhs.minor )
        return true;
    if ( rhs.minor < rhs.minor )
        return false;
    return patch < rhs.patch;
}

// -----------------------------------------------------------------------------

VPP_INLINE SVulkanVersion SVulkanVersion :: fromId ( unsigned int id )
{
    return SVulkanVersion { VK_VERSION_MAJOR(id), VK_VERSION_MINOR(id), VK_VERSION_PATCH(id) };
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPTYPES_HPP
