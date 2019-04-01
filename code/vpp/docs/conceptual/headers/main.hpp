namespace vpp
{

/*! \mainpage

VPP is a high-level C++ rendering and GPU computation API based on Vulkan.

Vulkan is the new API for 3D graphics rendering and GPU computation developed by
Khronos Group.

VPP adds high-level, object-oriented C++ interface over Vulkan. The goal is to make
Vulkan programming easier and more intuitive. VPP provides the following features:
- Intuitive, object-oriented abstractions over Vulkan concepts: devices, queues, render passes, pipelines, shader resources, synchronization objects and others.
- Isolation from the C-style interface of Vulkan.
- Easy, conceptual design of render passes and pipelines.
- Unique ability to write all shader code directly in C++ (as class methods), without any need to use GLSL or other intermediate language. No separate compiler passes required. All C++ 14 compliant compilers supported. No compiler extensions required. No hardware vendor / OS dependencies. All types of shaders supported.
- C++ shaders are automatically integrated into a pipeline and can access shader resources in straghtforward, object-oriented manner. Bindings and locations are automatically generated. Entire Vulkan shader interface is supported. Shaders may easily access: vertex inputs, uniform buffers, all kinds of image resources, attachments, push constants, all builtin variables, inter-shader communication variables.
- C++ shaders can be subject to advanced C++ programming and design techniques: parametrization, templating, macros, modularization, OO design.
- Support for multithreading and automatic host synchronization of objects where Vulkan requires it.
- Compile-time detection of some Vulkan usage errors.
- Integration with third party libraries (currently GLM).
- Easier management of Vulkan extensions with automatic dependency resolving for common extensions.
- Selected extension functionality seamlessly built-in into VPP API (e.g. 64-bit atomic variables).

This document describes VPP classes, functions and data types grouped by their purpose.
For some introductory texts explaining various topics, click the <b>Related Pages</b> button
on the navigation panel.

All VPP elements are contained in the \c vpp namespace.

\section sysClasses System classes
Classes providing basic interface to devices and operating system.
        - Instance
        - PhysicalDevice
        - DeviceFeatures
        - Device
        - Surface
        - SwapChain

\section renderPassConstruction Render and compute pass construction classes
These classes provide high-level building blocks for the rendering or computation system.
        - RenderGraph
        - Process
        - Preprocess
        - Postprocess
        - Attachment
        - Display
        - RenderPass
        - FrameBuffer
        - ComputePass
        - PipelineCache

\section resObjects Resource objects
Classes representing major resources: data buffers, images and samplers.
        - Buf
        - Buffer
        - Img
        - Image
        - ImageInfo
        - ImageAttributes
        - SNormalizedSampler
        - SUnnormalizedSampler
        - NormalizedSampler
        - UnnormalizedSampler

\section memAlloc Memory allocation
Classes providing memory allocation services, both on GPU and CPU side.
        - DeviceMemory
        - MappableDeviceMemory
        - MemProfile
        - MemoryBinding

\section dataContainers Data containers
These classes help to construct various types of data buffers.
        - \ref gvector
        - \ref dgvector
        - IndirectCommands
        - IndexedIndirectCommands
        - DispatchIndirectCommands
        - Indices

\section pipelineConstruction Pipeline construction classes
High-level building blocks for rendering and computation pipelines.
        - PipelineConfig
        - PipelineLayout
        - Pipeline
        - ComputePipelineConfig
        - ComputePipelineLayout
        - ComputePipeline

\section cmdExecHi High level execution
These classes are high level interfaces to general mechanisms: rendering, computation
and auxiliary processing. Using them is optional, as they provide very easy usage
at the expense of limited control over what is going on under the hood. Recommended for
beginners and to make simple projects.
        - RenderManager
        - ComputationEngine
        - CompiledProcedures

\section cmdExecLo Recording and execution of commands
These classes are responsible for recording and running Vulkan commands.
        - CommandBuffer
        - CommandPool
        - CommandBufferRecorder
        - Queue
        - NonRenderingCommands
        - UniversalCommands
        - ExtendedCommands

\section syncPrimitives Synchronization primitives
Synchronization mechanisms for parallel execution.
        - Semaphore
        - Fence
        - Event
        - Barriers
        - MemoryBarrier
        - BufferMemoryBarrier
        - ImageMemoryBarrier
        - BarrierList

\section renderOptions Rendering configuration classes
Configuring various parameters.
        - RenderingOptions
        - Viewport

\section bindPoints Resource binding points

These classes should be used within custom PipelineConfig subclasses
to define resource binding points. A binding point is auxiliary object
representing some resource accessible in GPU-level shaders. After
declaring a binding point, you can bind some data buffer to it and
then read or write the data in the shader.

There are the following binding point classes and templates:

\subsection bpVertex Vertex and instance data
        - inVertexData

\subsection vpUniform Uniform, storage and texel data buffers
        - inUniformBuffer
        - inUniformBufferDyn
        - ioBuffer
        - ioBufferDyn
        - ioImageBuffer
        - inTextureBuffer

\subsection bpPushcon Push constants
        - inPushConstant

\subsection bpImg Images and textures
        - ioImage
        - inSampler
        - inConstSampler
        - inTexture
        - inSampledTexture
        - inConstSampledTexture

\subsection bpArr Arrays of resource binding points
        - arrayOf
        - \ref multi

\subsection bpAttach Render graph attachments
        - inAttachment
        - outAttachment

\subsection bpIO Inter-shader communication
        - ioVariable
        - ioStructure

\section bpAccessors Binding point accessors
        - UniformVar
        - UniformArray
        - UniformSimpleArray
        - TexelArray
        - Input
        - Output

\section resViews Resource views
        - VertexBufferView
        - VertexIndexBufferView
        - UniformBufferView
        - StorageBufferView
        - IndirectBufferView
        - TexelBufferView
        - ImageView
        - ImageViewAttributes
        - FrameImageView

\section resBindHelpers Resource binding helpers
        - \ref bind
        - \ref multi

\section shBindPoints Shader binding points
These classes also are meant to be used within PipelineConfig subclasses.
They declare shader code blocks. A shader is plain C++ method which must
be registered by binding it to appropriate shader binding point. You
provide the method pointer to the binding point constructor.
        - vertexShader
        - geometryShader
        - tessControlShader
        - tessEvalShader
        - fragmentShader
        - computeShader

\section shInterfaces Shader interface classes
Shader interface objects are provided by VPP to shader routines. Each shader
type has both its own binding point type, and interface type. A pointer to the
interface object is supplied to registered shader method. You can access built-in
variables and some special functions through that pointer. Available variables
and functions depend on shader type.
        - VertexShader
        - GeometryShader
        - TessControlShader
        - TessEvalShader
        - FragmentShader
        - ComputeShader

\section shSimpleTypes Shader language simple data types

Those are simple types which you can use in shader code. By convention,
their names start with capital letters, to distinguish them from native
C++ types. 

There are three kinds of simple types: constants (r-values), mutable
variables (l-values) and group-scoped mutable variables.

Constants are preferable, as they guarantee optimal performance.

Regular mutable variables can degrade performance, as they consume GPU registers
which are usually limited. When registers run out, the shader compiler will
start to allocate GPU memory, which is much slower than registers. Therefore
use constants if possible. Mutable type names by convention start with capital
V letter.

Group-scoped mutable variables are allocated within special memory block on
GPU computation unit. This block typically has 32-48 kB of fast memory, shared
between local threads. This is intended as a way to communicate between
threads in single workgroup. These variables do not consume registers and have
low performance overhead. Group-scoped type names by convention start with capital
W letter.

Simple types cover scalar, vector and matrix types.
        - Int, UInt, Float, Double, Bool, Int64, UInt64
        - VInt, VUInt, VFloat, VDouble, VBool, VInt64, VUInt64
        - WInt, WUInt, WFloat, WDouble, WBool, WInt64, WUInt64
        - IVec2, UVec2, Vec2, DVec2, BVec2,
        - VIVec2, VUVec2, VVec2, VDVec2, VBVec2,
        - WIVec2, WUVec2, WVec2, WDVec2, WBVec2,
        - IVec3, UVec3, Vec3, DVec3, BVec3,
        - VIVec3, VUVec3, VVec3, VDVec3, VBVec3,
        - WIVec3, WUVec3, WVec3, WDVec3, WBVec3,
        - IVec4, UVec4, Vec4, DVec4, BVec4,
        - VIVec4, VUVec4, VVec4, VDVec4, VBVec4,
        - WIVec4, WUVec4, WVec4, WDVec4, WBVec4,
        - Mat2 Mat3 Mat4 Mat3x2 Mat4x2 Mat2x3 Mat4x3 Mat2x4 Mat3x4
        - VMat2 VMat3 VMat4 VMat4x2 VMat2x3 VMat4x3 VMat2x4 VMat3x4
        - WMat2 WMat3 WMat4 WMat4x2 WMat2x3 WMat4x3 WMat2x4 WMat3x4
        - DMat2 DMat3 DMat4 DMat4x2 DMat2x3 DMat4x3 DMat2x4 DMat3x4
        - VDMat2 VDMat3 VDMat4 VDMat4x2 VDMat2x3 VDMat4x3 VDMat2x4 VDMat3x4
        - WDMat2 WDMat3 WDMat4 WDMat4x2 WDMat2x3 WDMat4x3 WDMat2x4 WDMat3x4

\section shCompoundTypes Shader language compound data types

Compound types also come in two kinds, but different than scalar types. There
are array and structural compound types.

Array types always denote local mutable variables. There are no constant arrays.
Also, array types are NOT used for non-local variables, like uniform buffers.
Those object have their own means to access the data items (binding point
accessors), which resemble array syntax but are not arrays described here.

Structural types come in several flavors differing in how they interact with
CPU side. Those of them which are shared with CPU side, require for each field
either CPU data type which maps to GPU type (UniformStruct), or Vulkan format
specification (VertexStruct, InstanceStruct). LocalStruct types are local
and require only GPU field types to be specified.
        - VArray - local arrays, accessible to single GPU thread.
        - WArray, WArray2, WArray3 - shared (workgroup-scoped) arrays of 1, 2 and 3 dimensions.
        - VertexStruct - defines GPU data structure for a vertex.
        - InstanceStruct - defines GPU data structure for an instance.
        - UniformStruct - defines GPU data structure to be contained inside uniform buffer (also storage or push constant).
        - LocalStruct - defines GPU data structure for local storage, e.g. passing data between shaders.
        - Attribute - defines a member of VertexStruct or InstanceStruct.
        - Field - defines a member of LocalStruct.
        - UniformFld - defines a member of UniformStruct.

\section shSpecialTypes Shader language special data types
        - Pointer

\section shLangConstructs Shader language constructs
        - If(), Else(), Fi()
        - Do(), While(), Od()
        - For(), Rof()
        - Switch(), Case(), Default(), Break(), EndSwitch()
        - Function, Par, Begin(), End(), Return()
        - Select()
        - StaticCast(), ReinterpretCast()
        - Ignore(),
        - WorkgroupBarrier()

\section shLangFunctions Shader language functions
        - Round(), RoundEven(), Trunc(), Abs(), Sign(), Floor(), Ceil(), Fract()
        - Radians(), Degrees(), Sin(), Cos(), Tan(), Asin(), Acos(), Atan(), Atan2()
        - Sinh(), Cosh(), Tanh(), Asinh(), Acosh(), Atanh()
        - Pow(), Exp(), Log(), Exp2(), Log2(), Sqrt(), InverseSqrt()
        - Determinant(), MatrixInverse(), Transpose()
        - Min(), Max(), Clamp()
        - Mix(), Step(), SmoothStep(), Fma()
        - PackSnorm4x8(), PackUnorm4x8(), PackSnorm2x16(), PackUnorm2x16(), PackHalf2x16(), PackDouble2x32()
        - UnpackSnorm2x16(), UnpackUnorm2x16(), UnpackHalf2x16(), UnpackSnorm4x8(), UnpackUnorm4x8(), UnpackDouble2x32()
        - Dot(), OuterProduct(), Length(), Distance(), Cross(), Normalize()
        - FaceForward(), Reflect(), Refract()
        - FindLsb(), FindMsb(), BitCount(), BitReverse()
        - NMin(), NMax(), NClamp()
        - Mod(), Modf(), Frexp(), Ldexp()
        - IsNaN(), IsInf(), IsAnyComponentTrue(), IsAllComponentsTrue()

\section shLangImgFunctions Shader language functions for accessing textures and images
        - UseImage(),
        - MakeSampledTexture(), ExtractSampledTexture()
        - ImageSize(), TextureSize()
        - ImageQuerySamples(), ImageQueryLevels(), TextureQueryLod()
        - SubpassLoad()
        - ImageLoad(), ImageStore()
        - TexelFetch(), TexelFetchLod(), TexelFetchOffset()
        - Texture(), TextureLod(), TextureProj(), TextureLodProj()
        - TextureDref(), TextureLodDref(), TextureProjDref(), TextureLodProjDref()
        - TextureGather(), TextureGatherDref()

\section dbgHelpers Debugging helpers
        - StreamDebugReporter
        - DebugReporter
        - Shader

\section shLangGroupAlgs Group computation algorithms
        - ct::group::GroupInvocation
        - ct::group::Apply(), ct::group::Fill(), ct::group::Generate(), ct::group::Transform()
        - ct::group::Copy(), ct::group::Load(), ct::group::Store(),
        - ct::group::Reduce(), ct::group::InclusiveScan, ct::group::ExclusiveScan
        - ct::group::Sort(),
        - ct::group::LowerBound(), ct::group::UpperBound
*/
}