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

There are two kinds of simple types: constants (r-values) and mutable
(l-values). Constants are preferable, as they guarantee optimal performance.
Mutable variables can degrade performance, as they consume GPU registers
which are usually limited. When registers run out, the shader compiler will
start to allocate GPU memory, which is much slower than registers. Therefore
use constants if possible. Mutable type names by convention start with capital
V letter.

Simple types cover scalar, vector and matrix types.
        - Int, UInt, Float, Double, Bool
        - VInt, VUInt, VFloat, VDouble, VBool
        - IVec2, UVec2, Vec2, DVec2, BVec2,
        - VIVec2, VUVec2, VVec2, VDVec2, VBVec2,
        - IVec3, UVec3, Vec3, DVec3, BVec3,
        - VIVec3, VUVec3, VVec3, VDVec3, VBVec3,
        - IVec4, UVec4, Vec4, DVec4, BVec4,
        - VIVec4, VUVec4, VVec4, VDVec4, VBVec4,
        - Mat2 Mat3 Mat4 Mat3x2 Mat4x2 Mat2x3 Mat4x3 Mat2x4 Mat3x4
        - VMat2 VMat3 VMat4 VMat4x2 VMat2x3 VMat4x3 VMat2x4 VMat3x4
        - DMat2 DMat3 DMat4 DMat4x2 DMat2x3 DMat4x3 DMat2x4 DMat3x4
        - VDMat2 VDMat3 VDMat4 VDMat4x2 VDMat2x3 VDMat4x3 VDMat2x4 VDMat3x4
        - IMat2 IMat3 IMat4 IMat4x2 IMat2x3 IMat4x3 IMat2x4 IMat3x4
        - VIMat2 VIMat3 VIMat4 VIMat4x2 VIMat2x3 VIMat4x3 VIMat2x4 VIMat3x4
        - UMat2 UMat3 UMat4 UMat3x2 UMat4x2 UMat2x3 UMat4x3 UMat2x4 UMat3x4
        - VUMat2 VUMat3 VUMat4 VUMat3x2 VUMat4x2 VUMat2x3 VUMat4x3 VUMat2x4 VUMat3x4

\section shCompoundTypes Shader language compound data types

Compound types also come in two kinds, but different than scalar types. There
are array and structural compound types.

Array types always denote local mutable variables. There are no constant arrays.
Also, array types are not used for non-local variables, like uniform buffers.
Those object have their own means to access the data items (binding point
accessors), which resemble array syntax but are not arrays described here.

Structural types come in several flavors differing in how they interact with
CPU side. Those of them which are shared with CPU side, require for each field
either CPU data type which maps to GPU type (UniformStruct), or Vulkan format
specification (VertexStruct, InstanceStruct). LocalStruct types are local
and require only GPU field types to be specified.
        - VArray, VArray2, VArray3 - local arrays of 1, 2 and 3 dimensions.
        - VertexStruct - defines GPU data structure for a vertex.
        - InstanceStruct - defines GPU data structure for an instance.
        - UniformStruct - defines GPU data structure to be contained inside uniform buffer (also storage or push constant).
        - LocalStruct - defines GPU data structure for local storage, e.g. passing data between shaders.
        - Attribute - defines a member of VertexStruct or InstanceStruct.
        - Field - defines a member of LocalStruct.
        - UniformFld - defines a member of UniformStruct.

\section shLangConstructs Shader language constructs
        - If(), Else(), Fi()
        - Do(), While(), Od()
        - For(), Rof()
        - Switch(), Case(), Default(), Break(), EndSwitch()
        - Function, Par, Begin(), End(), Return()
        - Select()
        - Shared(), Static()
        - StaticCast(), ReinterpretCast()
        - Ignore(),
        - WorkgroupBarrier(), DeviceBarrier()

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
*/

}