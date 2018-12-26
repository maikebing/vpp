// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/** \page pageBuildVS How to build and use VPP under Visual Studio

    \section secBuildVS How to build and use VPP under Visual Studio

    \subsection secBuildProps Build properties

    Current (development) version of VPP supports Visual Studio 2015 and 2017,
    any edition.

    You can build VPP under Visual Studio in two ways:
    - Build stand-alone library, with help of provided solution file.
    - Incorporate VPP build into your project, by adding the project into
      your own solution file.

    Both cases are very similar and both involves editing a file called
    \c vppBuild.props. The file is located in the following directory in
    VPP source distribution:

    \code
    <VPP root>/build/<vsVVVV>/vppBuild.props
    \endcode

    Default contents of this file are as follows:

    \code
        <?xml version="1.0" encoding="utf-8"?>
        <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
          <ImportGroup Label="PropertySheets" />
          <PropertyGroup Label="UserMacros">
            <VPP_BUILD_BINDIR>../../../../bin/windows/$(Platform)/$(Configuration)/bin</VPP_BUILD_BINDIR>
            <VPP_BUILD_LIBDIR>../../../../bin/windows/$(Platform)/$(Configuration)/lib</VPP_BUILD_LIBDIR>
            <VPP_BUILD_INTDIR>../../../../bin/windows/$(Platform)/$(Configuration)/obj/$(ProjectName)</VPP_BUILD_INTDIR>
            <VPP_VULKAN_DIR>$(VULKAN_PATH)</VPP_VULKAN_DIR>
            <VPP_STATICLIB32_NAME>VppStatic32</VPP_STATICLIB32_NAME>
            <VPP_SHAREDLIB32_NAME>VppShared32</VPP_SHAREDLIB32_NAME>
            <VPP_STATICLIB64_NAME>VppStatic64</VPP_STATICLIB64_NAME>
            <VPP_SHAREDLIB64_NAME>VppShared64</VPP_SHAREDLIB64_NAME>
          </PropertyGroup>
          <PropertyGroup />
          <ItemDefinitionGroup />
          <ItemGroup>
            <BuildMacro Include="VPP_BUILD_BINDIR">
              <Value>$(VPP_BUILD_BINDIR)</Value>
              <EnvironmentVariable>true</EnvironmentVariable>
            </BuildMacro>
            <BuildMacro Include="VPP_BUILD_LIBDIR">
              <Value>$(VPP_BUILD_LIBDIR)</Value>
              <EnvironmentVariable>true</EnvironmentVariable>
            </BuildMacro>
            <BuildMacro Include="VPP_BUILD_INTDIR">
              <Value>$(VPP_BUILD_INTDIR)</Value>
              <EnvironmentVariable>true</EnvironmentVariable>
            </BuildMacro>
            <BuildMacro Include="VPP_VULKAN_DIR">
              <Value>$(VPP_VULKAN_DIR)</Value>
              <EnvironmentVariable>true</EnvironmentVariable>
            </BuildMacro>
          </ItemGroup>
        </Project>
    \endcode

    The general rule is that the \c vppBuild.props file should be always
    in the same directory as the solution (\c .sln) file you are building
    VPP from. In case of standalone build, the default directory is
    the proper one. In case of your own solution, copy the file to the
    place where the solution file exists and edit it there.

    Configurable macros are located in the first part of the file. Currently
    there are the following macros:
    - \c VPP_BUILD_BINDIR - where binary DLL will be generated in case of
      building a DLL configuration.
    - \c VPP_BUILD_LIBDIR - where static libraries and import libraries
      will be generated.
    - \c VPP_BUILD_INTDIR - where intermediate files will be stored.
    - \c VPP_VULKAN_DIR - root directory of the Vulkan SDK.
    - \c VPP_STATICLIB32_NAME - name of 32-bit static version, without extension.
    - \c VPP_SHAREDLIB32_NAME - name of 32-bit DLL version, without extension.
    - \c VPP_STATICLIB64_NAME - name of 64-bit static version, without extension.
    - \c VPP_SHAREDLIB64_NAME - name of 64-bit DLL version, without extension.

    Relative paths are calculated with respect to the location of VPP project file,
    which is appripriate to standalone build. If you integrate VPP into your own
    project and want to specify output path relatively to it, use Visual Studio
    macros (e.g. <tt>$(SolutionDir)</tt> like this:

    \code
        <VPP_BUILD_BINDIR>$(SolutionDir)/../build/vs2015/$(Platform)/$(Configuration)/bin</VPP_BUILD_BINDIR>
        <VPP_BUILD_LIBDIR>$(SolutionDir)/../build/vs2015/$(Platform)/$(Configuration)/lib</VPP_BUILD_LIBDIR>
        <VPP_BUILD_INTDIR>$(SolutionDir)/../build/vs2015/$(Platform)/$(Configuration)/obj/$(ProjectName)</VPP_BUILD_INTDIR>
    \endcode

    By default VPP finds Vulkan by means of <tt>VULKAN_PATH</tt> environment
    variable, but you can specify any directory (or other environment variable name)
    here.

    Also the names of final files are configurable. By default, names of generated files
    will be as follows: \c VppStatic32.lib, \c VppStatic64.lib, \c VppShared32.dll,
    \c VppShared32.lib, \c VppShared64.dll, \c VppShared64.lib. The \c .lib files
    will be used as inputs to linker. You can either specify them manually in
    Additional Dependencies field, or use Link Library Dependencies option.

    \subsection secConfigs Configurations and platforms

    VPP provides the following configurations under Visual Studio:
    \c Debug, \c DebugDLL, \c Release, \c ReleaseDLL. Both 32-bit and 64-bit
    variants are supported.

    \subsection secProject Project file

    The project file for inclusion in your own solutions is located in
    the following directory:

    \code
    <VPP root>\code\vpp\build\<vsVVVV>\vpp.vcxproj
    \endcode
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/** \page pageGeneralOverview General overview of rendering with VPP

    There are several key concepts to familiarize with to implement 
    a rendering engine with VPP (and also core Vulkan):

    1. A render graph: defines the data flow. This is a directed acyclic graph.
      The graph consists of two kinds of nodes. vpp::Process nodes represent rendering
      processes - programs which generate or transform images somehow. vpp::Attachment
      nodes represent images the processes operate on. A process can output images,
      but also consume images (possibly rendered by another process in chain).
      In such case, a dependency arc is being created between the processes
      to ensure that pixels needed by latter process are already generated by
      the former process.

      To define a render graph, derive a class from vpp::RenderGraph base class.
      Read the appropriate documentation sections for more detail. For simple
      applications you will likely need only a single process.

    2. A rendering pipeline. This object defines how single process operate.
      It is the core abstraction in rendering. It encapsulates all the needed
      information, which is divided into the following aspects:
      - Vertex sources.
      - Auxiliary data sources.
      - Output data targets.
      - Shaders.

      A vertex source is a mechanism to pass geometry data to the rendering
      pipeline. Vulkan renders geometry arranged into points, lines,
      triangles, or tessellation patches. Defining coordinates of these primitives
      are being passed in vertex buffers. You must define format for this data.
      For more info on this topic, read more about vpp::VertexStruct, vpp::InstanceStruct,
      vpp::Attribute, vpp::inVertexData, vpp::VertexBufferView, vpp::gvector.

      Auxiliary data sources allows you to supply more data for
      your rendering engine: textures, view and projection parameters,
      constants, etc. This kind of data is being passed to the pipeline through
      binding points. VPP offers a number of binding point classes for 
      various types of resources, e.g.:
      vpp::inUniformBuffer, vpp::inUniformBufferDyn, vpp::ioBuffer, vpp::ioBufferDyn,
      vpp::ioImageBuffer, vpp::inTextureBuffer, vpp::ioImage, vpp::inSampler,
      vpp::inConstSampler, vpp::inTexture, vpp::inSampledTexture, vpp::inConstSampledTexture.
      You can then bind actual data buffers to binding points.

      Data buffers are represented by vpp::Buffer and vpp::Image subclasses (including
      very convenient \ref vpp::gvector template). Bindings are stored inside special small
      objects of a class called vpp::ShaderDataBlock. You can quickly bind/unbind entire
      vpp::ShaderDataBlock (containing multiple resources) between draw calls.

      Shaders are actual programs being run on the GPU. Shaders receive data
      from vertex sources, bound resources, images rendered by processes earlier
      in the chain (aka input attachments). They write pixels and other data to
      process output images (aka output attachments) or bound writable buffers.
      VPP provides the following classes for shader support: vpp::vertexShader,
      vpp::geometryShader, vpp::tessControlShader, vpp::tessEvalShader, vpp::fragmentShader,
      vpp::computeShader. You write shaders directly in C++ and bind the routines
      to objects of these classes.

      Because VPP found a way to execute code written in C++ on the GPU, it also
      provides a lot of support classes to GPU-level coding. These are, among
      others:
      - GPU data types (e.g. vpp::Float, vpp::Int, vpp::Vec3, etc.) and arrays of
        these (vpp::VArray, vpp::VArray2, vpp::VArray3).
      - Computational functions (e.g. vpp::Sin(), vpp::Cos(), vpp::Floor(),
        vpp::Determinant(), etc.),
      - Accessors for built-in shader variables (e.g. vpp::VertexShader,
        vpp::FragmentShader - note that they start from capital letter to distinguish
        them from shader binding point classes mentioned above).
      - Image sampling and manipulation functions (for example vpp::ImageLoad(),
        vpp::ImageStore(), vpp::Texture(), vpp::TexelFetch(), etc.).
      - Control constructs, like vpp::If(), vpp::Switch(), vpp::Do(), etc.
        These are necessary, as regular C++ keywords are not overloadable.

      In general, these functions and classes form a set similar to GLSlang
      library.

      There are some more mechanisms (e.g. push constants and queries) useful
      in specific situations.

      In order to define a rendering pipeline class, derive your class from
      vpp::PipelineConfig or vpp::ComputePipelineConfig.

    3. Rendering options and parameters. There is a lot of important parameters
      affecting rendering process in a pipeline. For example: whether to draw
      filled polygons or wireframe? Do we need to discard polygons facing backwards?
      Is the Z buffer used to determine visibility? Is the stencil buffer used?

      VPP defines a unified container for all such options in the form of
      vpp::RenderingOptions class.

      One of important rendering options is the viewport configuration.
      VPP provides vpp::Viewport class to store viewport dimensions.
      vpp::Viewport objects are registered in vpp::RenderingOptions object.

    4. Interfacing with the window system displaying images on screen. See
      vpp::Surface and vpp::SwapChain for more details.

    5. Control mechanisms - they specify how to put things in motion. Among
      these are: queues, command buffers, commands, synchronization primitives.

      One of the simplest high level class belonging to this category is
      vpp::RenderManager which encapsulates all the mechanisms inside and hides
      them under simple interface. This class can be useful to develop examples,
      small applications, quick experiments with rendering graphs and pipelines,
      etc.

      For more advanced graphic engines, you will need to interact with Vulkan
      concepts directly. VPP provides convenient C++ wrappers over them. See
      the docs for classes: vpp::Queue, vpp::CommandBuffer, vpp::CommandBufferPool,
      vpp::CommandBufferRecorder, vpp::Semaphore, vpp::Fence, vpp::Event. VPP also
      provides extensive list of commands, corresponding to Vulkan rendering commands,
      and in some cases being higher level wrappers over them. See the docs for classes:
      vpp::NonRenderingCommands, vpp::UniversalCommands, vpp::ExtendedCommands for
      more info on commands.

    6. Device information. This is an important part - to know what rendering 
       device the system is utilising and what are its capabilities. vpp::PhysicalDevice
       class allows to query for that information. vpp::Device class represents the
       logical device (GPU) and is a parameter to most VPP functions. In case the
       system has several devices (multiple GPUs), you can work on all off them
       in parallel by using multiple vpp::Device objects.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/** \page pageShadersExplanation How do C++ shaders work

    VPP completely removes the need of using external languages (like GLSL
    or GLSLang) to write shaders. Just write them in C++. Also it does not
    need any special support from the compiler. It suffices to have a C++14
    compliant compiler.

    You write shaders just as regular methods of your vpp::PipelineConfig
    (or vpp::ComputePipelineConfig) derived subclass. Rendering resources
    (vertices, textures, buffers, etc.) are accessed just as class fields.
    You can call other methods from these methods. You can create reusable
    libraries of shader code. You can use templates, classes, virtual functions,
    object-oriented code, generic code, anything you want -- or almost,
    as there are some limitations.

    To understand what these are, you need to know how the mechanism work.
    It is actually quite simple.

    In order to run code on GPU under Vulkan, you have to supply it in
    SPIR-V format. SPIR-V is a standard defining a kind of virtual machine,
    just like Java VM. But this code is not being run by an interpreter
    (although it could, e.g. for debugging purposes), but rather translated
    inside a GPU driver to the machine code of the GPU. So this is basically
    a JIT compilation going on. All these details are hidden, you just need to
    generate a SPIR-V module and send it to Vulkan.

    How to generate a SPIR-V module? This is just some data block in specific
    format. So VPP can do it without any problems. But how to generate
    such code from the C++ code, that is an integral part of the process
    we are running and which generates the SPIR-V? Do we need source code,
    or special compiler introspection features (like Java)? The answer is: no!

    The trick is that we have a C++ regular method which does one thing when being
    called: it translates ITSELF into SPIR-V. That's all.

    This process is correct, because C++ executes code just in the same order
    as it is written, according to strict rules. There are some ambiguities,
    e.g. subexpression evaluation order is undefined. But exactly the same
    situation we have in GLSL/GLSLang. So we can have e.g. an overloaded 
    addition operator, which does just this:
    - assume that C++ already evaluated both operands in unspecified order
      (C++ standard guarantees that),
    - get expression identifiers of the operands,
    - issue an \c OpFAdd instruction to SPIR-V output stream.

    As long as dependencies are satisfied (this process guarantees that),
    every expression will be correctly translated to SPIR-V.

    But what about control constructs, like \c if, \c for, \c switch, etc?
    There is more complicated work to be done. First of all, we can't use
    regular C++ keywords. They are not overloadable in C++ so we can't
    force them to do other things for us than those they were made to.
    Therefore, VPP introduces its own counterparts starting with capital
    letters: vpp:If(), vpp::For(), vpp::Switch(), etc.

    Now, thanks to some clever planning and bookkeeping, VPP can generate
    proper SPIR-V control structures from these.

    Combining extensive operator overloading with some special functions,
    VPP can achieve the goal: have a method in C++ being translated into
    SPIR-V as it is being run.

    What are the drawbacks and limitations of the process? There are some,
    but in practice they are not that severe. for example:

    - For block control constructs, you must use opening and closing
      keyword (like in old languages like Algol or Modula-2), for example:
      \code
        using namespace vpp;

        Bool bExpression = ...; // some value

        // 'if' example
        If ( bExpression );
            // ... code
        Else();
            // ... another code
        Fi();

        // 'for' example
        Int nIterations = ...; // some value
        VInt i;

        For ( i, 0, nIterations );
        {
            // iterated code, i goes from 0 to nIterations-1
            // Also this one creates a C++ scope, allowing local variables.
        }
        Rof();

      \endcode

      As you can see, the code looks slightly different than regular C++,
      but not really that different.

    - The compiler will not detect all of errors in the code. It will
      complain on all syntax and type errors, but will miss some errors
      like missing Rof() or Fi(). Such errors are easily found by hand, though.
      Just remember that every If() must have matching Fi(), and so on.

    - Static variables of GPU types do not have much sense. However, if
      you want to store some constants as <tt>static const</tt> variables
      (e.g. the value of PI), there will be no problem. They will be just
      regular C++ constants.

    - C++ is generally more strict about data types than GLSLang, so you
      might need explicit cast sometimes, e.g. to cast Int to UInt.

    - VPP distinguishes between mutable variables like vpp::VInt and immutable
      ones like vpp::Int. This is however performance-oriented decision
      rather than limitation. Mutable variables on GPU exhaust register pool
      very quickly, leading to horribly slow shaders when abused. Therefore
      the plain \c Int is immutable and slightly more fancy \c VInt is mutable.

    There are as well some good things, that might not be apparent initially:

    - Interoperability between C++ constructs and VPP constructs is quite useful.
      Actually C++ acts like a macro language to the GPU code. So you can
      still use \c if or \c switch to select blocks of code, based on C++
      expression which is constant during execution of GPU code. This is
      easy method to make parameterized shader variants, e.g. to use different
      lighting equations depending on a parameter. Also regular \c for construct
      might be useful to create unfolded loops with predefined iteration counts.

    - Any C++ variable used in GPU expression will be treated as constant and
      its current value will be compiled into SPIR-V. This is another opportunity
      for parametrization.

    - When you call a C++ function from a shader method, all shader code
      generated inside the called function will be appropriately inlined inside
      the calling shader. You can pass references to mutable or immutable variables
      and they will just work as expected. The function may come from any
      source, even external library or some template. (In addition to that,
      VPP of course offers a special syntax for regular GPU-level functions,
      so you may avoid inlining if it is undesirable).

    - You can define classes having GPU-typed fields and running GPU code.
      The limitation is that they can be used only as local objects
      (no heap allocation, as there is no such thing on GPU).
      
    - You can define templates of functions and classes and they will 
      work as expected.

    - Alternatively you can use preprocessor macros instead and this
      also will work as expected.

    - Evaluation and execution order in resulting SPIR-V will exactly
      match evaluation and execution order in C++. This means that
      operators with well-defined evaluation order (like boolean
      <tt>||</tt>) will behave just as expected in SPIR-V.

    - It is easy to define C++ functions that do various "special things"
      in SPIR-V, e.g. set an execution mode instead of generating an
      instruction. VPP uses this possibility liberally. One of more
      interesting applications is vpp::DebugProbe() debugging aid.

    - VPP shaders do not use location and binding indices explicitly
      to match resources. Instead you directly access binding points
      which are declared as fields in the class. All resources are
      matched by name, in type-safe manner.

    - Compute shaders are fully supported. This enables creation of
      reusable GPU libraries, e.g. for mathematical algorithms or
      other applications. For now, such libraries exist but require
      complex proprietary technologies, like CUDA. VPP allows creation
      of GPU libraries which have no external dependencies, are extremely
      easy to integrate and run on any hardware that supports \c GLCompute
      shader model (probably all Vulkan-supporting GPUs).

    A simple example of coding shaders in VPP:

    \code
        // Define data structure for vertices. There are two attributes.
        // Blocks of 4*float will be interleaved.
        template< vpp::ETag TAG >
        struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
        {
            vpp::Attribute< TAG, float, float, float, float > m_position;
            vpp::Attribute< TAG, float, float, float, float > m_color;
        };

        // Define GPU-level version of the structure.
        typedef TVertexAttr< vpp::CPU > CVertexAttr;

        // Define CPU-level version of the structure. GPU and CPU version
        // layouts are compatible.
        typedef TVertexAttr< vpp::GPU > GVertexAttr;

        // Define data structure for frame parameters (matrices).
        template< vpp::ETag TAG >
        struct TFramePar : public vpp::UniformStruct< TAG, TFramePar >
        {
            inline TFramePar() {}

            inline TFramePar (
                const glm::mat4& m2w, const glm::mat4& w2v, const glm::mat4& v2p ) :
                    m_model2world ( m2w ), m_world2view ( w2v ), m_view2projected ( v2p )
            {}

            // This defines a field of 4x4 matrix type.
            vpp::UniformFld< TAG, glm::mat4 > m_model2world;
            vpp::UniformFld< TAG, glm::mat4 > m_world2view;
            vpp::UniformFld< TAG, glm::mat4 > m_view2projected;
        };

        // Define GPU-level version of the structure.
        typedef TFramePar< vpp::GPU > GFramePar;

        // Define CPU-level version of the structure. GPU and CPU version
        // layouts are compatible.
        typedef TFramePar< vpp::CPU > CFramePar;

        // Define user pipeline class.
        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            MyPipeline (
                const vpp::Process& pr, const vpp::Device& dev, const vpp::Display& outImage );

            // GPU-level method defining a vertex shader.
            void fVertexShader ( vpp::VertexShader* pShader );

            // GPU-level method defining a fragment shader.
            void fFragmentShader ( vpp::FragmentShader* pShader );

        private:
            // Binding point for vertices.
            vpp::inVertexData< TVertexAttr > m_vertices;

            // Binding point for frame parameters (world matrix, etc.).
            vpp::inUniformBuffer m_framePar;

            // Inter-shader communication variable to pass some data to fragment shader.
            // This will by default create a 'varying' variable (with interpolation).
            vpp::ioVariable< vpp::Vec4, vpp::vertexShader, vpp::fragmentShader > m_ioColor;

            // Resulting image.
            vpp::outAttachment< FormatF32x4 > m_outColor;

            // Binding point for the vertex shader.
            vpp::vertexShader m_vertexShader;

            // Binding point for the fragment shader.
            vpp::fragmentShader m_fragmentShader;
        };

        MyPipeline :: MyPipeline (
            const vpp::Process& pr, const vpp::Device& dev, const vpp::Display& outImage ) :
                // The pipeline needs to know which process it is accociated with.
                vpp::PipelineConfig ( pr ),
                // Output directly to the Display node (associated with some screen Surface).
                m_outColor ( outImage ),
                // Bind the vertex shader method.
                m_vertexShader ( this, & MyPipeline::fVertexShader ),
                // Bind the fragment shader method.
                m_fragmentShader ( this, & MyPipeline::fFragmentShader )
        {
        }

        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            // This code will be translated to SPIR-V and execute on GPU!

            // Accessor for m_framePar binding point.
            UniformVar< TFramePar, decltype ( m_framePar ) > inFramePar ( m_framePar );

            // Just read the values in type-safe manner.
            // Note that structure and fields are identified by name.
            const Mat4 m2w = inFramePar [ & GFramePar::m_model2world ];
            const Mat4 w2v = inFramePar [ & GFramePar::m_world2view ];
            const Mat4 v2p = inFramePar [ & GFramePar::m_view2projected ];

            // Read vertices just as above. Vertices do not need an accessor.
            const Vec4 inPos = m_vertices [ & GVertexAttr::m_position ];
            const Vec4 inColor = m_vertices [ & GVertexAttr::m_color ];

            // Computation.
            const Vec4 result = v2p * w2v * m2w * inPos; 

            // Write result to predefined shader variable.
            pShader->outVertex.position = result;

            // Accessor for additional output variable, passing data to the
            // fragment shader.
            Output< decltype ( m_ioColor ) > outColor ( m_ioColor );

            // Write the data.
            outColor = inColor;
        }

        void MyPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            // This code will be translated to SPIR-V and execute on GPU!

            // Accessor for an input variable, passing data from the
            // vertex shader. Note it uses the same m_ioColor binding point
            // as the counterpart on writing side.
            Input< decltype ( m_ioColor ) > inColor ( m_ioColor );

            // Just read the value.
            const Vec4 color = inColor;

            // Just write the value to the output image (output images
            // do not need accessors).
            m_outColor = color;
        }
    \endcode

    In general, VPP achieves the level of abstraction and conciseness of
    the code better than OpenGL, while maintaining high performance of core
    Vulkan, benefitting from type-safety of C++, and enjoying extreme simplicity
    and accessibility to new developers.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageRenderGraphs How do render graphs work

    \section secHowDoRenderGraphs How do render graphs work

    \subsection secRenderGraphBasics Render graph basics

    Render graph represents a high-level structure of the rendering engine.

    The graph is constructed from building blocks connected by dependency arcs.
    Two main types of these blocks are:
    - vpp::Attachment nodes, which represent images on which the renderer operate.
    - vpp::Process nodes, which represent active processes working on these images.

    A process node may consume one or more attachments and produce one or
    more attachments. An attachment may be produced by one process, and
    then consumed by another one. The latter process is dependent on
    the former one. In case of many processes, this forms a dependency graph.

    Processes on GPU are being executed in parallel. Dependency graph ensures
    that the data consumed by a process are already prepared by the producing
    process. Usually this is not being done on whole image level, but rather
    the image is divided into smaller segments, which may be as small as single
    pixel. This enables better parallelism.

    Additionally, there can also be two special nodes in ther graph: vpp::Preprocess
    and vpp::Postprocess. The former one is executed before entire graph and allows
    for preparation steps. The latter one is executed in the end, after entire
    graph is finished. It allows for finalization steps.

    Instead of an vpp::Attachment node, you can use a vpp::Display node which is just a special
    kind of attachment associated with a vpp::Surface (on-screen display object).

    In order to define a render graph, derive your class from the vpp::RenderGraph
    base class. Inside your class, put fields of type vpp::Process, vpp::Preprocess,
    vpp::Postprocess, vpp::Attachment and vpp::Display. These classes are explained
    on their respective documentation pages.

    The most important information stored by vpp::Process, vpp::Preprocess and
    vpp::Postprocess nodes is the command sequence. The sequence is made of
    commands performing things like:
    - selecting current pipeline for the rendering process (as there might
      be more than one defined),
    - selecting data buffers,
    - drawing geometry primitives from selected buffers,
    - synchronization,
    - filling and copying images and buffers,
    - setting certain pipeline parameters which are dynamic,
    - managing queries and timestamps.

    You can write a complete rendering program by using these commands.
    This is being done by means of C++ lambda functions. These functions are
    registered in corresponding vpp::Process, vpp::Preprocess or vpp::Postprocess
    node by using the \c << operator. An example of simple render graph and
    command sequence definition is presented below. For brevity, details
    concerning objects other than render graph are omitted.

    \code
        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph ( const vpp::Surface& hSurface ) :
                m_display ( hSurface )
            {
                // Add output attachment (image). This one will be a display surface.
                m_render.addColorOutput ( m_display );
            }

        public:
            // Initialization node.
            vpp::Preprocess m_init;

            // Rendering node.
            vpp::Process m_render;

            // Output image node.
            vpp::Display m_display;
        };

        class MyRenderer
        {
        public:
            MyRenderer (
                const vpp::Surface& hSurface,
                const vpp::Device& hDevice );

        private:
            // ...
            MyRenderGraph m_renderGraph;

            vpp::RenderPass m_renderPass;
            vpp::ShaderDataBlock m_dataBlock;
            vpp::PipelineLayout< MyPipeline > m_pipelineLayout;

            // ...

            // Data buffers - type definitions not shown for brevity.
            VertexAttrBuffer m_vertexBuffer;
            vpp::Indices m_indexBuffer;
            CFrameParBuffer m_frameParBuffer;
        };

        MyRenderer :: MyRenderer (
            const vpp::Surface& hSurface
            const vpp::Device& hDevice ) :
                m_renderGraph ( hSurface ),
                m_renderPass ( m_renderGraph, hDevice ),
            // ...
        {
            // Register a command sequence for the initialization node.
            // Note that a C++ lambda function is being used in conjunction
            // with operator<<.
            m_renderGraph.m_init << [ this ]()
            {
                // Commands for synchronization of the buffers.
                m_vertexBuffer.cmdCommit();
                m_indexBuffer.cmdCommit();
                m_frameParBuffer.cmdCommit();
            };

            // Register a command sequence for the rendering node.
            // Note that a C++ lambda function is being used in conjunction
            // with operator<<.
            m_renderGraph.m_render << [ this ]()
            {
                // This command selects a pipeline for subsequent commands.
                // There may be multiple pipelines.
                // Pipeline definition and registration are not shown in this example.
                m_renderPass.pipeline ( 0, 0 ).cmdBind();

                // This command selects current set of data buffers.
                // There may be multiple sets.
                m_dataBlock.cmdBind();

                // This command binds vertex buffers to the pipeline.
                m_pipelineLayout.definition().bindVertices ( m_vertexBuffer, m_indexBuffer ); 

                // This command requests indexed draw operation.
                m_renderGraph.cmdDrawIndexed ( 3, 1, 0, 0, 0 );
            };
        }

    \endcode

    There are a couple of important things about this example.

    First of all, as it can be seen clearly, render graphs work together
    with other objects: pipelines, data blocks, buffers. Commands inside
    registered sequences can (and in fact must) access other objects.
    Many of them are actually methods of these objects, intended to be
    called from inside lambda functions of render graphs. Those methods
    have usually the \c cmd prefix in their names. As lambda functions
    must access external objects, the most convenient way to initialize
    render graphs is to do it from the enclosing object, in this case
    \c MyRenderer. The lifetime of accessed objects must not be shorter
    than the render graph itself. The pattern shown in the example
    assures it.

    Defined command sequence need not to be static (like in the example).
    You can use any kind of algorithm to generate commands, presumably
    corresponding to mesh organization in your rendering engine. There
    may be many draw calls, interleaved by vertex and other buffer binds,
    ane even pipeline changes. Lambda functions give you the full flexibility.

    \subsection secRenderPassBasics Render pass basics

    Another question is, what is the vpp::RenderPass object and what it does.
    The vpp::RenderPass is really a compiled form of render graph. A render
    graph is abstract representation existing on VPP level only. A render
    pass is the actual Vulkan object, compiled from the render graph for
    specified device.

    That is, a render pass is constructed from the render graph and device
    as in the example below.

    \code
        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph ( const vpp::Surface& hSurface );
            // ...
        };

        class MyRenderer
        {
        public:
            MyRenderer (
                const vpp::Surface& hSurface,
                const vpp::Device& hDevice );

        private:
            // This is the render graph object.
            MyRenderGraph m_renderGraph;

            // This is the render pass object.
            vpp::RenderPass m_renderPass;

            // ...
        };

        MyRenderer :: MyRenderer (
            const vpp::Surface& hSurface
            const vpp::Device& hDevice ) :
                // Initialize render graph with your custom arguments required by the constructor.
                m_renderGraph ( hSurface ),
                // Initialize render pass with render graph and device.
                m_renderPass ( m_renderGraph, hDevice ),
            // ...
        {
            // ...
        }
    \endcode

    Some VPP operations require abstract form (vpp::RenderGraph) and some others
    take compiled form (vpp::RenderPass). One notable example of the latter is
    registering rendering pipelines for render graph processes. This is being done 
    by means of vpp::RenderPass::addPipeline() method and requires a compiled form
    of a pipeline (vpp::PipelineLayout) as well.

    \subsection secAdvanced Advanced topics

    The following considerations are more advanced and you can skip them
    on first read (esp. if you are not familiar with core Vulkan).

    A question that might arise is where these lambda functions are called
    from. Directly this is done by the vpp::CommandBufferRecorder class, which
    takes a vpp::RenderPass object (among other parameters), gets source render
    graph for it, gather all registered lambdas and record them into
    supplied Vulkan command buffer.

    You can use vpp::CommandBufferRecorder directly, but this is considered
    advanced usage. A simple way is the vpp::RenderManager class, which
    just have simple render() method that hides all these details.
    The method first calls vpp::CommandBufferRecorder for a command buffer,
    and then sends the buffer to the device for execution.

    Lambda functions provide something we call implicit context. As it
    can be seen in the example, you do not provide any reference to
    Vulkan command buffer inside the lambdas -- although core Vulkan
    requires supplying that parameter. This is because the commands
    can detect that they are being called from lambdas and obtain
    appropriate command buffer reference from calling vpp::CommandBufferRecorder
    object. This mechanism simplifies syntax and reduces amount of possible
    errors.

    How VPP abstractions used in the example maps to core Vulkan concepts
    and objects? Here is the translation table:
    - vpp::RenderGraph - no direct counterpart in Vulkan.
    - vpp::RenderPass - encapsulates \c VkRenderPass.
    - vpp::Process - defines a single subpass within a render pass.
                Registered commands are issued between \c vkCmdBeginRenderPass
                and \c vkCmdEndRenderPass.
    - vpp::Preprocess - commands issued before \c vkCmdBeginRenderPass.
    - vpp::Postprocess - commands issued after \c vkCmdEndRenderPass.
    - vpp::ShaderDataBlock - encapsulates a descriptor set.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pagePipelines How do pipelines work

    \section secHowDoPipelines How do pipelines work

    After we have defined a render graph, we need to specify for each process node
    (vpp::Process) how actual rendering wil be done by that process and what additional resources
    will it consume. Same applies for computational engines. In both cases,
    the object representing a complete rendering or computation program is called
    a pipeline.

    Constructing a pipeline is actually a three stage process.
    
    First, we actually design a pipeline. That means writing shader code and defining
    resources to be used in that code. This is the most important and involved part.
    The result is an abstract object representing a pipeline configuration.

    Second, we construct a compiled form of the pipeline configuration. This is
    very simple step, being done entirely by VPP, from given reference to the
    pipeline configuration and device. The result is called a pipeline layout.

    Finally, we register the pipeline layout in the render pass (being a compiled
    form of the render graph itself), for specified process node. We also provide
    a set of rendering options here, allowing to further customize rendering.
    This step gives us a fully realized pipeline object, which can be selected for
    drawing commands. There can be multiple pipelines registered for single process,
    selectable at will inside a rendering command sequence. Among there can be:
    - instances of completely different pipeline layouts, differing by
      configuration class or parameters,
    - instances of the same pipeline layout, differing by provided option set.
      
    Subsections below describe these three steps in more detail.
    
    \subsection secPipelineConfigs Defining custom pipelines - pipeline configurations

    In order to define your custom pipeline configuration, create a class derived from
    vpp::PipelineConfig or vpp::ComputePipelineConfig. In this chapter, we will assume
    that we are making a rendering pipeline.

    An example:

    \code
        class MyPipelineConfig : public vpp::PipelineConfig
        {
        public:
            MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev );
            MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev, int myParam );

            // ...

        private:
            vpp::Device m_device;
            int m_myParam;
        };

        MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev )
            vpp::PipelineConfig ( pr ),
            m_device ( dev ),
            m_myParam ( 0 )
            // ...
        {}

        MyPipelineConfig :: MyPipelineConfig ( const vpp::Process& pr, const vpp::Device& dev, int myParam )
            vpp::PipelineConfig ( pr ),
            m_device ( dev ),
            m_myParam ( myParam )
            // ...
        {}
    \endcode

    By convention, the first two arguments to the constructor should be
    vpp::Process and vpp::Device. You must pass the vpp::Process immediately
    to the base class constructor. This is not needed for vpp::Device, but
    it might be useful later, so just remember it in a field. Also as you can see,
    there is a possibility to define any number of your own optional parameters.
    
    For more detail about pipeline configurations, please see documentation
    pages for vpp::PipelineConfig and vpp::ComputePipelineConfig.

    Also note that you need to specify some rendering options right now,
    on pipeline configuration level, although actually most of the options
    are being set much later (by means of vpp::RenderingOptions objects).
    Among these exceptions are:
    - pixel blending mode,
    - pixel logic operation,
    - geometrical primitive topology,
    - number of control points in tessellation patches.

    Not setting these options will result in using default values, which are:
    no blending, no logic operations, triangle topology, unspecified
    tessellation points (valid if no tessellation is being used).

    \subsection secPipelineLayouts Compiled form - pipeline layouts

    A pipeline layout is a compiled form of pipeline configuration. Technically
    more precise would be to call it "half-compiled" as not all aspects are
    compiled yet, but we'll stick to that term. 

    In order to construct a pipeline layout, create an instance of
    vpp::PipelineLayout or vpp::ComputePipelineLayout template, giving your
    pipeline configuration class as the template argument:

    \code
            class MyRenderer
            {
                MyRenderer();
                MyRenderer ( int myParam );
                // ...
                vpp::PipelineLayout< MyPipelineConfig > m_pipelineLayout;

                // ...
            };

            // in constructor of the renderer class
            MyRenderer() :
                // ...
                m_pipelineLayout ( m_renderGraph.m_process, m_device ),
                // ...
            {
                // ...
            }

            MyRenderer ( int myParam ) :
                // ...
                m_pipelineLayout (
                    m_renderGraph.m_process, m_device, myParam ),
                // ...
            {
                // ...
            }
    \endcode

    Note that you never create a pipeline configuration object yourself - the pipeline
    layout does it internally. Pass mandatory vpp::Process and vpp::Device arguments.
    Also you may pass any custom arguments you defined for pipeline configuration
    constructor. Get the vpp::Process reference from your render graph, the vpp::Device
    should be already stored as a field in your rendering engine class, as this
    is required for many other purposes also.

    To retrieve a reference to your pipeline configuration object maintained
    internally by vpp::PipelineLayout, call the vpp::PipelineLayout::definition()
    method. A need to do this arises frequently in command sequence definitions,
    like this:

    \code
        m_renderGraph.m_render << [ this ]()
        {
            // ...

            m_pipelineLayout.definition().bindVertices (
                *m_pVertexAttrBuffer, *m_pIndexBuffer );
        };
    \endcode

    This is because \c bindVertices() is inherited from the vpp::PipelineConfig class.
    You can call also your own methods this way, as \c definition() returns a reference
    to your class, not the base vpp::PipelineConfig.

    See the docs for vpp::PipelineLayout and vpp::ComputePipelineLayout for more
    information.

    \subsection secPipelines Fully realized form - pipelines

    This is the final stage where the pipeline layout is being registered
    in the render pass and attached to specified vpp::Process node. Also
    it is being associated with vpp::RenderingOptions object, configuring
    most rendering options (except few ones that are set on vpp::PipelineConfig
    level).

    To perform this step, you need to call the vpp::RenderPass::addPipeline() method,
    with the following arguments:
    - a reference (or numerical index) of the vpp::Process node within the render graph,
    - the vpp::PipelineLayout object itself,
    - already prepared vpp::RenderingOptions object.

    This method call returns a numerical index of the pipeline within particular
    process. It is important when you register more than one pipeline for a process,
    as any number of such registrations is allowed. Nevertheless, single pipeline
    layout instance must be registered to only one process. The relation between
    processes and pipeline layouts is "one to many" type.

    An example - single pipeline.

    \code
        m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_options );
        // ...

        m_renderGraph.m_render << [ this ]()
        {
            // First index is zero, as there is only one process.
            // Second index is zero, as there is only one pipeline.
            m_renderPass.pipeline ( 0, 0 ).cmdBind();
            // ...
        };
    \endcode

    An example - multiple pipelines.

    \code
        // A pipeline with color rendering.
        const unsigned int colorPipeline =
            m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_optionsC );

        // The same pipeline layout, configured for wireframe rendering.
        const unsigned int wframePipeline =
            m_renderPass.addPipeline ( m_renderGraph.m_render, m_pipelineLayout, m_optionsW );

        m_renderGraph.m_render << [ this, colorPipeline, wframePipeline ]()
        {
            // Select the pipeline.
            const unsigned int pipelineId = m_bWireframe ? wframePipeline : colorPipeline;

            // Bind selected pipeline.
            m_renderPass.pipeline ( m_renderGraph.m_render.index(), pipelineId ).cmdBind();
        };
    \endcode

    The vpp::RenderPass::pipeline() method retrieves an object of type vpp::Pipeline.
    This is the actual, fully compiled Vulkan pipeline object. This object is
    needed to bind the pipeline for subsequent draw operations. The vpp::Pipeline
    objects are maintained internally by VPP and are guaranteed to be accessible
    only inside command sequences (lambda functions) for vpp::Process nodes.
    Do not use this method elsewhere, unless for advanced senarios and knowing
    some VPP internals. The only generally usable function of these objects is
    to bind them to current command sequence via vpp::Pipeline::cmdBind() method.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageSupplyData Methods of supplying data

    After constructing a rendering engine from building blocks like render
    graphs and pipelines, we need to think of providing some actual data
    to it for processing. There is also a question concerning how to retrieve
    the results - or redirect them to the screen to display.

    There are two aspects of the problem to consider:
    - What objects are available to store the data physically?
    - How to bind these objects to the rendering engine, composed of render graphs
      and pipelines?

    We will answer these questions in this chapter.

    \section secBuffers Buffers

    Buffers generally store one-dimensional, non-image data, which can however be structured
    in complex ways.

    \subsection secBufferClasses Basic buffer classes in VPP

    First we will start from simple, non-structured buffers. The most basic, elementary
    buffer class in VPP is vpp::Buf. It is not recommended to create these directly,
    however vpp::Buf is a base class for slighly more specific vpp::Buffer and there
    are many functions and methods in VPP accepting just vpp::Buf.

    The difference between the two is that vpp::Buffer is not a class but rather
    a template, carrying the information about the buffer \b usage inside its type.
    Using a vpp::Buffer allows for more type safety and error checking. The vpp::Buf
    takes usage as a parameter, but all constructed vpp::Buf objects are identical
    and it is possible to supply a wrong buffer in wrong place.

    Both types take size (in bytes) and the target vpp::Device. Also in both cases
    the actual memory allocation is deferred - buffers do not allocate memory
    when being constructed, but rather at the moment of memory binding so the
    process is two-step.

    Buffer classes are reference counted, and you may pass these objects around
    by value. For input arguments, a \c const reference is recommended for
    higher performance (as reference counters in VPP are atomics, which brings
    some small overhead when passing them as values).
    
    \subsection secBufferUsages Buffer types and usages

    The usage is determined by a bitwise \c or of usage flags, which are taken
    from enumeration inside vpp::Buf class (in fact these constants are equal
    to their core Vulkan counterparts). See the docs for vpp::Buf::EUsageFlags
    and vpp::Buffer for a description.

    You define a buffer usage by providing the bitwise \c or result to the
    vpp::Buffer class as an argument. For example:

    \code
        typedef vpp::Buffer< vpp::Buf::SOURCE > StagingBuffer;
    \endcode

    It is convenient to make a \c typedef for every buffer kind you are
    using in your program.

    \subsection secBufferMemory Memory allocation

    Since buffers do not allocate memory immediately when they are created,
    a separate allocation function is required. The function is named
    vpp::Buf::bindMemory(). It is actually a function template, taking
    a parameter which can be one of these two classes:
    - vpp::DeviceMemory,
    - vpp::MappableDeviceMemory.

    The difference between these two is that vpp::MappableDeviceMemory can be
    mapped into the address space of the CPU and its contents may be accessed
    from CPU program. vpp::DeviceMemory does not have that capability, but
    the advantage is that it may be allocated inside GPU-only memory, which
    guarantees much better performance. In practice, use vpp::MappableDeviceMemory
    only if you really need to access the memory directly.

    An example of buffer usage:

    \code
        typedef vpp::Buffer< vpp::Buf::VERTEX > MyVtxBuffer;

        MyVtxBuffer createVertexBuffer ( unsigned int size, const vpp::Device& hDevice )
        {
            using namespace vpp;

            // Create the buffer, no memory allocated yet.
            MyVtxBuffer hBuffer ( size, hDevice );

            // Allocate the memory and get the memory object.
            // This buffer will be allocated on the CPU side, because we requested
            // HOST_STATIC profile. This host-side memory is visible to GPU, but
            // slower than GPU-side memory.
            MappableDeviceMemory hMemory =
                hBuffer.bindMemory< MappableDeviceMemory >( MemProfile::HOST_STATIC );

            // Map the memory to CPU address space.
            hMemory.map();

            // Get starting and ending addresses.
            unsigned char* pBufferBegin = hMemory.beginMapped();
            unsigned char* pBufferEnd = hMemory.endMapped();

            // fill the area between pBufferBegin, pBufferEnd
            // ... some code here

            // Ensure that writes are flushed.
            hMemory.syncToDevice();

            // Unmap the memory.
            hMemory.unmap();

            return hBuffer;
        }
    \endcode

    An important thing is what are vpp::MemProfile constants. They specify
    what kind of memory we are requesting. See the docs for vpp::MemProfile
    for description of these constants. In most scenarios either \c DEVICE_STATIC
    or \c HOST_STATIC will be appropriate.

    Another way to bind memory to a buffer is to use external functions
    vpp::bindDeviceMemory() and vpp::bindMappableMemory(). They take
    a buffer reference and vpp::MemProfile constant. They return an object
    of type vpp::MemoryBinding, which holds references to both the buffer
    and memory object bound to it. It is sometimes convenient to have
    a compound object like this. Also some VPP functions require vpp::MemoryBinding
    objects as parameters, to ensure that buffers they operate on are bound
    to memory.

    An example:

    \code
        using namespace vpp;
        typedef Buffer< Buf::SOURCE > StagingBuffer;

        StagingBuffer stagingBuffer ( tex2D.size(), m_device );

        // stagingBufferMemory will be of type MemoryBinding<...>
        auto stagingBufferMemory = bindMappableMemory ( stagingBuffer, MemProfile::HOST_STATIC );

        stagingBufferMemory.memory().map();
        memcpy ( stagingBufferMemory.memory().beginMapped(), tex2D.data(), tex2D.size() );
        stagingBufferMemory.memory().unmap();
    \endcode

    \subsection secBufferVectors Vectors

    While buffers are low-level concept, basically offering ranges of unformatted
    memory, VPP also offers much more advanced mechanism: vectors.

    The vpp::gvector template is basically a STL-like vector operating on top of
    a buffer. The vector internally manages the buffer, as well as memory allocation
    and binding. From user's perpective it behaves much like \c std::vector. The
    only limitation is that maximum size must be specified in advance.

    Vectors are quite versatile and can be used in all places where either a buffer
    (vpp::Buf subclass) is needed, or a memory binding (vpp::MemoryBinding).

    An example of using vpp::gvector to define vertices:

    \code
        template< vpp::ETag TAG >
        struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
        {
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_pos;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_normal;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_uv;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_color;
        };

        typedef TVertexAttr< vpp::CPU > CVertexAttr;
        typedef TVertexAttr< vpp::GPU > GVertexAttr;
        typedef vpp::gvector< CVertexAttr, vpp::Buf::VERTEX > VertexAttrBuffer;

        class MyRenderingEngine
        {
        public:
            MyRenderingEngine ( const vpp::Device& hDevice ) :
                m_device ( hDevice )
            {
                // m_init is a vpp::Preprocess node
                m_renderGraph.m_init << [ this ]()
                {
                    // ...

                    // This will copy the vector contents to the device before
                    // rendering begins.
                    m_pVertexAttrBuffer->cmdCommit();
                };
            }

            void loadMeshes ( const MeshLoader::Meshes& meshesVector )
            {
                unsigned int nTotalVertices = ...; // compute number of vertices in meshes

                // Create the vector. nTotalVertices is the maximum capacity,
                // for now the size is zero.
                m_pVertexAttrBuffer.reset ( new VertexAttrBuffer (
                    nTotalVertices, MemProfile::DEVICE_STATIC, m_device ) );

	            for ( uint32_t m = 0; m < meshesVector.size(); m++ )
	            {
                    const MeshLoader::Mesh& mesh = meshesVector [ m ];

		            for ( uint32_t i = 0; i < mesh.m_vertices.size(); i++ )
		            {
                        const MeshLoader::Vertex& vtx = mesh.m_vertices [ i ];

                        CVertexAttr vertex;
                        vertex.m_pos = glm::vec4 ( vtx.m_pos, 1.0f );
			            vertex.m_normal = glm::vec4 ( vtx.m_normal, 1.0f );
			            vertex.m_uv = glm::vec4 ( vtx.m_tex, 0.0f, 0.0f );
			            vertex.m_color = glm::vec4 ( vtx.m_color, 1.0f );

                        // Insert the vertex into the vpp::gvector.
			            m_pVertexAttrBuffer->push_back ( vertex );
		            }
	            }
            }

        private:
            vpp::Device m_device;
            MyRenderGraph m_renderGraph;
            std::unique_ptr< VertexAttrBuffer > m_pVertexAttrBuffer;
            // ...
        };

    \endcode

    You can find more information in vpp::gvector documentation.

    Vectors can be used for any buffers, not just vertices. An example of
    defining a vector of frame parameters (matrices, etc.) as uniform buffer:

    \code
        template< vpp::ETag TAG >
        struct TFrameParams : public vpp::UniformStruct< TAG, TFrameParams >
        {
            vpp::UniformFld< TAG, glm::mat4 > m_projection;
            vpp::UniformFld< TAG, glm::mat4> m_model;
            vpp::UniformFld< TAG, glm::vec4 > m_lightPos;
        };

        typedef TFrameParams< vpp::GPU > GFrameParams;
        typedef TFrameParams< vpp::CPU > CFrameParams;

        typedef vpp::gvector< CFrameParams, vpp::Buf::UNIFORM > FrameParamsBuffer;
    \endcode

    \subsection secBufferViews Buffer views

    Buffer views are intermediate object classes which participate in binding
    buffers to binding points in pipelines. The task of a buffer view is to define
    a slice of the buffer which will be visible from the shader.

    Most buffer views are simple, "throw away" objects which can be constructed
    temporarily only to bind actual buffer. The only exception is vpp::TexelBufferView
    template, which defines a view on core Vulkan level and this view must exist
    as long as the pipeline is using it.

    There are several buffer view classes, intended to be used with different kinds
    of buffers:
    
    - vpp::VertexBufferView: a view for vpp::Buf::VERTEX buffers, to be used with
      vpp::inVertexData binding point. Can be temporary. Can define a start offset
      (in bytes) of the viewed slice (which extends to the end of the buffer).
      
    - vpp::VertexIndexBufferView: a view for vpp::Buf::INDEX buffers, to be used
      with implicit binding point for indices (vpp::PipelineConfig::cmdBindIndexInput()).
      Can be temporary. Can define a start offset (in bytes) of the viewed slice (which
      extends to the end of the buffer).

    - vpp::UniformBufferView: a view for vpp::Buf::UNIFORM and vpp::Buf::STORAGE buffers,
      to be used with vpp::inUniformBuffer, vpp::inUniformBufferDyn, vpp::ioBuffer,
      vpp::ioBufferDyn binding points. Can be temporary. Can define a start offset
      and length (in bytes) of the viewed slice.

    - vpp::IndirectBufferView: a view for vpp::Buf::INDIRECT buffers,
      to be used with implicit binding point for indirect draws
      (RenderGraph::cmdDrawIndirect(), RenderGraph::cmdDrawIndexedIndirect()).
      Can be temporary.

    - vpp::TexelBufferView: a view for vpp::Buf::UNITEX and vpp::Buf::STORTEX buffers,
      to be used with vpp::inTexture and vpp::ioImage binding points. These are buffers
      that are being treated as images in the shaders. The view must exist as long it it
      being used. Can define a start offset and length (in bytes) of the viewed slice.

    All buffer views take a vpp::MemoryBinding object in constructors.
    That means you can use either a regular buffer bound to memory, or vpp::gvector.

    \subsection secBufferBindingPoints Buffer binding points

    Binding points are objects declared within your custom pipeline as class fields,
    whose purpose is to allow to access external data from shaders. The shaders
    are written C++ code but executed on GPU. Binding points are hybrid between
    CPU-level and GPU-level objects, acting as go-betweens for both worlds.

    On the GPU level, binding points are accessed from shaders just as regular
    class fields.

    On the CPU level, binding points are being bound to data buffers with help
    of view objects and shader data blocks.

    Binding points exist both for buffers and images. Here is the list of binding
    points for buffers:
    - vpp::inUniformBuffer
    - vpp::inUniformBufferDyn
    - vpp::ioBuffer
    - vpp::ioBufferDyn
    - vpp::ioImageBuffer
    - vpp::inTextureBuffer

    See the documentation for respective classes for more information.

    Any of them should be placed in your class derived from vpp::PipelineConfig
    or vpp::ComputePipelineConfig, as a field (usually private).

    Note that we did not list a binding point for vertices (vpp::inVertexData)
    here, although it is also a binding point working with buffers. This is because
    vertex buffers are treated differently by Vulkan. For example, they are not
    part of shader data blocks desribed below. Vertex binding points will be covered
    in separate section.

    \subsection secShaderDataBlocks Shader data blocks

    A buffer (or image) binding is an association between some binding point
    and some buffer view. A shader data block is a container for such associations.
    It remembers the bindings for a number of binding points.

    A shader data block must be selected for subsequent drawing operations
    in rendering or computation command sequence. That means, all affected binding
    points will be bound at once. You can later switch to another shader data block,
    which has some (or all) binding points associated with different data.
    Switching shader data blocks is very fast.
    
    Currently VPP offers shader data blocks which act on all binding points
    defined in particular pipeline configuration (except vertex data). Therefore
    vpp::ShaderDataBlock takes a vpp::PipelineLayout as the constructor parameter.

    \subsection secBufferBindingUpdates Binding buffers to points

    In order to bind a buffer or some buffers to binding points in particular
    pipeline, first you need to have a vpp::ShaderDataBlock in which the binding
    will be stored. Create one or more shader data blocks right next to your
    vpp::PipelineLayout object. Also define a method in your vpp::PipelineConfig
    (or vpp::ComputePipelineConfig) subclass that takes a pointer to
    vpp::ShaderDataBlock as well as set of buffer and image views to bind.

    In the method, call the vpp::ShaderDataBlock::update() method on supplied data
    block. This method has somewhat interesting interface, as it takes an
    <b>assignment list</b> allowing to bind multiple buffers and images at once.

    An assignment list, is just as the name says, a list of assignments separated
    by commas. Entire list should be enclosed with additional parentheses,
    to avoid confusing the C++ parser (so that it won't search for overload
    with multiple arguments). Each assignment on the list has the form:

    \code
        <binding point> = <buffer view object>
    \endcode

    Binding point is a class field (e.g. of type vpp::inUniformBuffer or other
    binding point type) and buffer view object is a reference to one of buffer view
    classes mentioned in earlier subsection (e.g. a vpp::VertexBufferView).

    Because we are inside a method defined inside vpp::PipelineConfig subclass,
    you access binding points directly as fields. The views must be provided as
    arguments.

    An example:

    \code
        typedef vpp::format< vpp::texture_format > TextureLoaderFmt;

        typedef vpp::ImageAttributes<
            TextureLoaderFmt, vpp::RENDER, vpp::IMG_TYPE_2D,
            vpp::Img::SAMPLED | vpp::Img::TARGET,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > TextureLoaderAttr;

        typedef vpp::Image< TextureLoaderAttr > TextureLoaderImage;
        typedef vpp::ImageViewAttributes< TextureLoaderImage > TextureLoaderViewAttr;
        typedef vpp::ImageView< TextureLoaderViewAttr > TextureLoaderView;

        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...

            void setDataBuffers (
                const vpp::UniformBufferView& fpv,
                const TextureLoaderView& texv,
                vpp::ShaderDataBlock* pDataBlock );

        private:
            vpp::inUniformBuffer m_framePar;
            vpp::inConstSampledTexture< TextureLoaderView > m_colorMap;
            // ...
        };

        void MyPipeline :: setDataBuffers (
            const vpp::UniformBufferView& fpv,
            const TextureLoaderView& texv,
            vpp::ShaderDataBlock* pDataBlock )
        {
            // Never forget about extra parentheses!
            pDataBlock->update ((
                m_framePar = fpv,
                m_colorMap = texv
            ));
        }

    \endcode

    This example shows binding of both a buffer and an image. The rules are
    exactly the same for both types of resources.

    A different question is when to call the \c setDataBuffers method.
    It's up to you, but it must be some place where vpp::ShaderDataBlock
    objects as well as all buffer and image objects are already constructed.
    On the other hand, it must be done before shader data block is actually used.

    A shader data block is used when it is being bound to a pipeline
    inside command sequence.

    \subsection secBufferBindingVertex Binding vertex buffers to pipelines

    As we have already mentioned, vertex (or in general geometry) data is treated
    in a special way by Vulkan. This kind of data is being stored in buffers, but
    it is not bound via shader data blocks.

    The binding point class for vertex data is vpp::inVertexData. It is a template
    which takes somewhat unusual parameter - a structure definition. This is because
    you must define the format of your vertex data, so that Vulkan knows how to
    parse it. Vertex data is the only kind of user-defined data which is actually
    processed by fixed-function hardware on GPU. Therefore the format must be
    determined before accessing the data. See the section below for instructions
    on how to define a vertex data structure and how to distinguish it from
    \b instance data structure.

    To bind a vertex buffer (or several of them) to vpp::inVertexData binding
    points, the best way is to start with writing a helper method, just the 
    same as for regular buffers in previous section. This time, it will not
    take any vpp::ShaderDataBlock, just the buffer references.

    Instead of updating a shader data block, it calls a command
    vpp::PipelineConfig::cmdBindVertexInput(). This will bind the vertex buffer
    to the pipeline and make it the vertex source for subsequent draw commands.

    Calling the command directly means that the \c bindVertices method must
    be called also directly from within vpp::Process command sequence (that
    is, from within the lambda function). This is the same place where you
    issue draw commands, switch pipelines and shader data blocks. Vertex buffers
    is another part that can be switched multiple times. You can have e.g.
    a separate buffer for each mesh.

    The vpp::PipelineConfig::cmdBindVertexInput() command accepts the same kind
    of assignment list as vpp::ShaderDataBlock::update(). You should assign
    only vpp::inVertexData binding points here, otherwise you will get a nasty
    C++ compiler error.

    An example:

    \code
        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...
            void bindVertices ( const vpp::VertexBufferView& vert )
            {
                // Do not forget about extra parentheses!!!
                cmdBindVertexInput (( m_vertices = vert ));
            }

        private:
            vpp::inVertexData< TVertexAttr > m_vertices;
            // ...
        };

        ...
        // In your rendering engine intialization routine...
        {
            ...

            m_renderGraph.m_render << [ this ]()
            {
                m_renderPass.pipeline ( 0, 0 ).cmdBind();
                m_dataBlock.cmdBind();

                // This calls the method defined above to bind the vertices.
                m_pipelineLayout.definition().bindVertices ( m_rectVertexBuffer ); 

                // This will read geometry from m_rectVertexBuffer.
                m_renderGraph.cmdDraw ( 4, 1, 0, 0 );
            };
        }

    \endcode

    You can also define more than one vpp::inVertexData binding point and
    have several vertex buffers. What needs to be well understood here is that all
    these buffers (imagine they are data tables organized in columns) define
    the same set of vertices. One buffer can hold positions, another one
    normals, yet another one UV coordinates -- it is up to you to decide.

    Generally it may be better for performance to pack all basic attributes
    into single buffer (with a structure). However, sometimes certain data
    is optional. For example anything else than positions is unused inside
    D-passes (depth-only rendering), so it might be better for performance
    to create one buffer with positions (used in D-pass and color pass)
    and another one with the rest of the attributes (for color pass only).

    VPP offer full flexibility here. It is up to you to experiment,
    benchmark and decide which layout is optimal in your scenario.

    \subsection secBufferBindingVertex Binding index buffers to pipelines

    Index buffers are another kind of buffers which are not treated like regular
    data buffers. These do not even have an explicit binding point. Otherwise
    they are similar to vertex buffers and always go along with them (indices
    define the ordering of vertices and groups them into primitives, e.g. triangles).

    When you need to bind an index buffer, do it in the same \c bindVertices method 
    as the vertex buffer:

    \code
        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...
            void bindVertices (
                const vpp::VertexBufferView& vert,
                const vpp::VertexIndexBufferView& ind )
            {
                // Do not forget about extra parentheses!!!
                cmdBindVertexInput (( m_vertices = vert ));

                // This one accepts only single buffer. No assignment list
                // and no extra parentheses required.
                cmdBindIndexInput ( ind );
            }

        private:
            vpp::inVertexData< TVertexAttr > m_vertices;
            // ...
        };
    \endcode

    Index buffers contain packed 32-bit or 64-bit integer values. These indices
    point to individual vertices in the vertex buffer - and in fact, in all defined
    vertex buffers in parallel (as they define the same set). Because of that,
    there is a need only for single index buffer without any customizable structure.
    That's why there is no explicit binding point for index buffers - it would
    contain no useful information.

    To create index buffer, you can use a convenience class called vpp::Indices.
    This is a \c typedef to \ref vpp::gvector, configured to contain 32-bit index
    values.

    \subsection secBufferDefiningVertex Defining the structure for vertex buffers

    Although buffers transmit opaque data blobs, vertex data is obviously structured.
    Each vertex has a number of attributes. Typically one of them is the position
    in 3D or 4D homogenous space. Other attributes might include a normal vector
    direction, tangent vector direction, UV coordinates, etc. This quite resembles 
    a C++ structure and in fact the vertex buffer is an array of structures.

    VPP offers a simple way to define vertex data structures with syntax
    very similar to regular C++ structures and with all benefits of them.
    Due to very specific usage of these structures (they are accessed
    both on CPU and GPU) a simple C++ \c struct or \c class would not
    suffice without requiring special support from the compiler, which
    VPP completely avoids. Therefore the structure definition is a template,
    and its fields are instantiations of templates.

    A simple vertex structure definition looks like this:

    \code
        // The definition is always a template taking vpp::ETag parameter.
        // The parameter specifies whether we want a CPU or GPU version of the structure.
        // The ETag enumeration has two public (non-internal) values: vpp::CPU and vpp::GPU.
        template< vpp::ETag TAG >
        struct TVertexAttr :

            // Always inherit from vpp::VertexStruct when defining a vertex structure.
            // Pass along the tag and our own template name (that is, VPP uses CRT pattern here).
            public vpp::VertexStruct< TAG, TVertexAttr >

        {
            // Now define the attributes. Pass the tag along. Also give the type of
            // the attribute, as seen on CPU. VPP will infer GPU type automatically.
            // The types can be either the same as in vpp::format template, or
            // single vpp::ext< ... > with external data type (e.g. GLM matrix or vector).
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_pos;
            vpp::Attribute< TAG, float, float, float, float > m_normal;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_uv;
        };
    \endcode

    As you can see, the definition is very simple and is actually a regular
    C++ type definition, although templatized.

    Note that you can also use inheritance if you wish, and/or define any
    methods with either CPU or GPU scope. Do not define a constructor or destructor,
    though.

    What can we do with such definition? First, it is always recommended to make
    atr least first two typedefs from the three ones shown below (best all three):

    \code
        typedef TVertexAttr< vpp::CPU > CVertexAttr;
        typedef TVertexAttr< vpp::GPU > GVertexAttr;
        typedef vpp::gvector< CVertexAttr, vpp::Buf::VERTEX > VertexAttrBuffer;
    \endcode

    Now you have 3 types and can do some nice things with them:
    - Use \c CVertexAttr as a regular C++ data structure with layout compatible
      with GPU side. For example, \c m_pos field will be visible in \c CVertexAttr
      structure and you will be able to read or assign a \c glm::vec4 to it.
      For the \ref vpp::format like fields, these fields will be equivalents
      of \c vpp::format< ... >::data_type types.
    - Use \c GVertexAttr as a way to access these fields on GPU side. Accessing
      fields on GPU is done by means of the indexing operator (\c []), because
      the dot is not overloadable. To access e.g. \c m_pos field, you will write
      <tt>[ & GVertexAttr::m_pos ]</tt> in some place of your shader.
    - Use \c VertexAttrBuffer to pass around vertex buffers, and send them to GPU.
      This is just a STL-styled vector holding vertices, that you can bind to GPU
      pipeline with single method call. Cool!
    
    \subsection secBufferDefiningInstance Defining the structure for instance buffers

    Instanced drawing is a method to efficiently render large number of similar
    objects, sharing the same mesh. Basically it is just drawing the same mesh
    over and over, with different parameters (e.g. varying model matrix). You
    specify that your mesh has e.g. 2000 vertices and you want to draw it 100
    times. Then, the GPU will repeat drawing 100 times, each time drawing the
    same mesh of 2k vertices but picking a new model matrix (which is being used
    in the vertex shader and affects the transformation of the mesh).

    Parameters for instances come from buffers quite similar to vertex buffers.
    They are called instance buffers and the difference is that a new item from
    instance buffer is picked when next instance is being started.

    Although instance buffers are implemented in almost the same way in core
    Vulkan as vertex buffers, VPP gives them recognition on the structure definition
    level. This is because of some complex internals, but also design logic. Instance
    buffers conceptually hold very different data than vertex buffers and there
    is no sense in mixing them together.

    An example:

    \code
        template< vpp::ETag TAG >
        struct TInstanceStd : public vpp::InstanceStruct< TAG, TInstanceStd >
        {
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2world;
            vpp::Attribute< TAG, vpp::ext< glm::mat4 > > m_model2worldNorm;
            vpp::Attribute< TAG, float, float, float, float > m_auxParams;
        };

        typedef TInstanceStd< vpp::CPU > CInstanceStd;
        typedef TInstanceStd< vpp::GPU > GInstanceStd;
        typedef vpp::gvector< CInstanceStd, vpp::Buf::VERTEX > InstanceStdBuffer;
    \endcode

    As you can see, apart from using vpp::InstanceStruct instead of vpp::VertexStruct
    everything is the same. Also vpp::Buf::VERTEX for the buffer type is fine.

    \subsection secBufferDefiningUniform Defining the structure for uniform buffers

    Uniform buffers are another type of data buffers designed to pass the data
    to the rendering engine (or from, as there are both read-only and read-write
    variants).

    First let's look at read-only uniform buffers. They are suitable to pass
    highest level data to shaders, not vertex-scoped nor instance-scoped, but
    rather frame-scoped. This is the ideal place to put all the data which
    does not change for entire frame, like the projection matrix or positions
    of light sources.

    Defining the structure for uniform buffers is quite similar to vertex
    and instance buffers, with some small differences. They can be seen in the
    following example:

    \code
        // Template parameter is the exactly the same as for vertex buffers.
        template< vpp::ETag TAG >
        struct TFrameParams :

            // Now we have a base class called UniformStruct, but otherwise it is the same syntax.
            public vpp::UniformStruct< TAG, TFrameParams >

        {
            // Fields are now defined with UniformFld template. It it almost the same, but:
            // - no option to use fields based on vpp::format< ... >,
            // - fields must be declared as custom math types (GLM, etc.),
            // - no vpp::ext< ... > is required.
            vpp::UniformFld< TAG, glm::mat4 > m_projection;
            vpp::UniformFld< TAG, glm::vec4 > m_lightPos;
        };

        // These typedefs are identical as for vertex buffers.
        typedef TFrameParams< vpp::GPU > GFrameParams;
        typedef TFrameParams< vpp::CPU > CFrameParams;

        // Here we have only different vector usage flag.
        typedef vpp::gvector< CFrameParams, vpp::Buf::UNIFORM > FrameParamsBuffer;
    \endcode

    The definition above is applicable to read-only buffers. For buffers that are
    potentially read-write, there is only one thing to change:
    
    \code
        // Vector usage flag now specifies STORAGE buffer. This is read-write.
        typedef vpp::gvector< CFrameParams, vpp::Buf::STORAGE > FrameParamsBuffer;
    \endcode

    \section secImages Images

    Images store data organized into 1, 2 or 3 geometrical dimensions
    and additionally can have multiple layers and/or MIP maps. They have no
    user-defined structure, but rather a \b format which is a single piece
    of information (enumeration value in core Vulkan).

    \subsection secImageClasses Basic image classes in VPP

    The counterpart of generic vpp::Buf class for images is vpp::Img. It has
    all parameters of the image specified at runtime and it is a universal
    image type, acceptable by many VPP functions. Just like with vpp::Buf,
    it is better to avoid creating them explicitly. Instead use the vpp::Image
    template. This is type-safe image definition which is suitable for dealing
    with image binding points in pipelines. The vpp::Image template carries enough
    information to enable accessing these images from C++ shaders (via binding
    points).

    Definition of image type is somewhat more complex than a buffer. It consists
    of several \b typedef declarations and it is done in stages. Look at the
    following simple example:

    \code
        // First, name your format. This particular format has one component
        // per pixel, which is 8-bit unsigned value mapped linearly into [0.0, 1.0]
        // floating point range.
        // With this format, you will see 'unsigned char' (0..255) on the CPU side, 
        // but Float on the GPU side.
        typedef vpp::format< vpp::unorm8_t > TextureFmt;

        // ImageAttributes template is always the next step after format. It gives
        // all needed information about the image.
        typedef vpp::ImageAttributes<
            
            // Format type, just defined.
            TextureFmt,

            // For typical usages, always put vpp::RENDER here. Other values
            // have currently only internal use.
            vpp::RENDER,

            // Dimensionality of the image. Use 1D, 2D, 3D, or CUBE_2D.
            vpp::IMG_TYPE_2D,
            
            // Image usage flags. See next section.
            vpp::Img::SAMPLED | vpp::Img::TARGET,

            // Image tiling mode. See next section.
            VK_IMAGE_TILING_OPTIMAL,

            // Image sample count. For multisampled anti-aliasing. For now, use 1 bit
            // (no MSAA).
            VK_SAMPLE_COUNT_1_BIT,

            // Whether the image is MIP-mapped. Typically enabled for textures.
            false,

            // Whether the image is arrayed. Useful in various scenarios.
            false

        >
        TextureAttr;  // our attributes type name, used later.

        // These typedefs are purely mechanical. This one defines image type from
        // given attributes.
        typedef vpp::Image< TextureAttr > TextureImage;

        // This one defines the view attributes from given image. Can have
        // some extra parameters, not shown here.
        typedef vpp::ImageViewAttributes< TextureImage > TextureViewAttr;

        // Finally, this one defines an image view type.
        typedef vpp::ImageView< TextureViewAttr > TextureView;
    \endcode

    These typedefs define several data types. Two of these - \c TextureImage and
    \c TextureView you will be using regularly to handle images and views
    of this type. For example, to create an image on the device, and a view for it:

    \code
        TextureImage myTexImg (
            VkExtent3D { width, height, 1 },
            MemProfile::DEVICE_STATIC,
            m_device
        );

        TextureView myTexView ( myTexImg );
    \endcode

    As you can see, after all these type definitions are being made, creating
    actual images becomes trivial.

    Another important thing is that image binding points are templates
    that require a view class to instantiate, like this:

    \code
        vpp::inConstSampledTexture< TextureView > m_bitmap;
    \endcode

    This is the same \c TextureView as above. You can now bind a \c TextureView
    object to this binding point.

    \subsection secImageDynFormat Images with dynamic format

    Sometimes it is unsuitable to specify the format as a template argument, because
    the format is varying or not known during compilation. It is a common case
    for textures loaded from external sources, which can have various compressed
    formats determined only when the texture file is being examined.

    VPP supports this scenario by introducing a special \ref vpp::texture_format
    syntax. It is used as in the example below:

    \code
        typedef vpp::format< vpp::texture_format > TextureFmt;

        typedef vpp::ImageAttributes<
            TextureFmt, vpp::RENDER, vpp::IMG_TYPE_2D,
            vpp::Img::SAMPLED | vpp::Img::TARGET,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > TextureAttr;

        typedef vpp::Image< TextureAttr > TextureImage;
        typedef vpp::ImageViewAttributes< TextureImage > TextureViewAttr;
        typedef vpp::ImageView< FontTextureViewAttr > TextureView;
    \endcode

    The format defined in this way has some limitations. It is guaranteed to
    work only for read-only sampled textures. In other cases, support depends
    on actual rendering device capabilities (so assume there is none).

    The format must be ultimately specified somewhere and the proper place
    is alternative version of vpp::Image constructor:

    \code
        TextureImage myTexImg (
            VK_FORMAT_BC3_UNORM_BLOCK,
            VkExtent3D { width, height, 1 },
            MemProfile::DEVICE_STATIC,
            m_device
        );

        TextureView myTexView ( myTexImg );
    \endcode

    \subsection secImageUsages Image types and usages

    vpp::ImageAttributes requires several parameters determining the type and possible
    uasge of the image. The most important ones are the format and usage mask.

    Format must be a \ref vpp::format instance. Please see the docs for vpp::format
    for more information about formats.

    Image usage flags is the fourth parameter on the list. This is very important parameter
    for the core Vulkan, specifying how the image will be used. It should be a bitwise
    \c or of flags defined in the vpp::Img class. See the docs for vpp::Img::EUsageFlags
    for a description of these flags.

    Most frequently used values are:
    - vpp::Img::SAMPLED - for sampled, read-only textures,
    - vpp::Img::STORAGE - for read/write images,
    - vpp::Img::COLOR - for images used as color attachments (rendering results),
    - vpp::Img::DEPTH - for images used as Z-buffer attachments,
    - vpp::Img::INPUT - for images used as intermediate images in render graphs.

    If you are copying the data from or to an image, also include vpp::Img::SOURCE
    and/or vpp::Img::TARGET flags.

    In case of intermediate nodes in render graphs, combine vpp::Img::INPUT with
    vpp::Img::COLOR (or vpp::Img::DEPTH).

    Other flag combinations might result in suboptimal performance, or be unsupported
    on particular hardware. Best to avoid them unless you know what you are doing.

    \subsection secImageMemory Memory allocation

    vpp::Img and vpp::Image constructors which have a vpp::MemProfile argument
    automatically allocate memory for the image according to the profile. Other
    constructors do not do this.

    You can manually bind memory to an image by using vpp::bindDeviceMemory()
    and vpp::bindMappableMemory() functions, just the same as for buffers.

    \subsection secImageSamplers Samplers

    Retrieving data from textures is much more complex than just reading pixel value
    from a bitmap at <tt>(x,y)</tt> position. Actually this simple scheme of data
    access is applicable to \b storage images (vpp::Img::STORAGE), but textures
    (vpp::Img::SAMPLED) follow much more involved algorithm, called sampling.

    Sampling is configurable by user, and that means a separate object is needed
    to hold sampling parameters. This object is simply called a sampler. A sampler
    can be associated with a texture in one of three ways:
    - permanently,
    - by binding,
    - in shader code on the GPU.

    The first way is the fastest one but the last is the most versatile. More information
    on that topic is contained in the section \ref secImageBindingPoints.

    In sampling, the coordinates of a texel to retrieve are floating-point, not integer.
    The texture surface is "continuous" and "infinite" in some sense (such that the
    coordinates cover entire range of \c float type). There can be however several
    conventions regarding how the coordinates map onto actual image surface.

    First of all, VPP offers two kinds of samplers: normalized and unnormalized ones.

    Normalized samplers have the primary coordinate range equal to <tt>[0, 1]</tt>.
    The \c 0 value maps to the left or top point and \c 1 to right or bottom.
    This is compatible with UV maps that texture editing programs typically produce
    (UV maps associate a vertex in a mesh with some point in the texture).

    Unnormalized samplers have the range starting from \c 0, but ending at the
    exact value of the image dimension (e.g. width, height or depth in case of
    3D images). This is according to definition in section 15.1.1 of the official
    Vulkan specs. Sometimes this is useful -- if you know the size of the texture
    and want to have direct control where the texels come from. Unnormalized
    samplers however have some limitations regarding the configuration options
    they support.

    In VPP, normalized sampler configuration is defined by setting fields of
    vpp::SNormalizedSampler structure. For unnormalized samplers, there is
    vpp::SUnnormalizedSampler structure, which has generally less options.

    These structure do not define proper sampler object yet. For that, use
    vpp::NormalizedSampler and vpp::UnnormalizedSampler classes. They take
    the structures as construction parameters, as well as the device. Both
    objects represent a Vulkan sampler object.

    Normalized samplers support the following features:
    - Multiple modes of coordinate wrapping (e.g. repeated, mirrored). This
      determines what to do when a coordinate falls outside the primary range.
    - Several modes for border color (used for some of coordinate wrapping modes).
    - Threshold comparisons (aka depth comparisons or shadow mapping) instead
      of regular texel read.
    - Separate filtering (interpolation) configuration for upscaling, downscaling and
      MIP-mapping.
    - Anisotropic filtering.
    - LOD bias.
    - LOD clamping.

    Unnormalized samplers are much more limited and they support:
    - Smaller selection of coordinate wrapping modes.
    - Same modes for border color.
    - Filtering for upscaling and downscaling is the same, and for MIP-mapping is fixed.
    - LOD bias.
    - No depth comparisons, anisotropic filtering, LOD clamping.

    \subsection secImageViews Image views

    Image views perform similar role as buffer views. They are intermediate objects
    used while binding images to binding points. Also a view allows to define a slice
    for the image constisting of a subset of its array levels and MIP maps (but not
    a window within its pixel area). Use this e.g. to treat some selected array level
    of the image as single-leveled image.

    An image view always is a Vulkan object and its lifetime must at least as long
    as operations on it are performed.

    Image view classes are defined from vpp::ImageView template, like in the example
    below:

    \code
        typedef vpp::format< vpp::unorm8_t > TextureFmt;

        typedef vpp::ImageAttributes<
            TextureFmt, vpp::RENDER, vpp::IMG_TYPE_2D,
            VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > TextureAttr;

        typedef vpp::Image< TextureAttr > TextureImage;
        typedef vpp::ImageViewAttributes< TextureImage > TextureViewAttr;
        typedef vpp::ImageView< TextureViewAttr > TextureView;
    \endcode

    The vpp::ImageViewAttributes intermediate template offers several
    additional parameters which are defined per view, rather than
    per image. All of these are optional.

    The first one is associated sampler type. There are normalized
    and unnormalized samplers as explained above, and the view is
    configured for either one. By default, if you do not specify any
    parameter, normalized sampler is assumed. However, if you specify
    vpp::SAMPLER_UNNORMALIZED constant, the view will allow unnormalized
    samplers (as well as normalized ones) at the cost of some limitations
    for underlying image (it can be either 1D or 2D only).

    Example:

    \code
        // Default - normalized samplers only.
        typedef vpp::ImageViewAttributes< TextureImage > TextureViewAttr;

        // Allow unnormalized samplers.
        typedef vpp::ImageViewAttributes< TextureImage, vpp::SAMPLER_UNNORMALIZED > TextureViewAttr;

        // Normalized samplers only.
        typedef vpp::ImageViewAttributes< TextureImage, vpp::SAMPLER_NORMALIZED > TextureViewAttr;
    \endcode

    Second optional parameter is called the aspect mask. This is very rarely used parameter
    taking a bitwise \c or of the following values (usually just one):
    - \c VK_IMAGE_ASPECT_COLOR_BIT
    - \c VK_IMAGE_ASPECT_DEPTH_BIT
    - \c VK_IMAGE_ASPECT_STENCIL_BIT

    This parameter determines kind of data accessed by this image view. Regular
    images contain color data. There can be also depth images containing pixel Z coordinates,
    as well as stencil images with bitmasks of user-defined meaning. Also there
    can be combined depth+stencil images. This parameter is useful mostly for these.
    You can select which part (depth or stencil) you want to access.

    In most cases just do not specify anything for this parameter, as VPP will determine
    the aspect automatically (except for combined depth+stencil images).

    Example:

    \code
        typedef vpp::ImageViewAttributes<
            DepthTextureImage,
            vpp::SAMPLER_NORMALIZED,
            VK_IMAGE_ASPECT_DEPTH_BIT > DepthTextureViewAttr;
    \endcode

    Third optional parameter is also very rarely used. It allows to override the image
    format. This way you can access the image pixels as if they were in different format
    as they really are, similar to the way \c reinterpret_cast or unions in C++ work.

    If the overriding format is specified, it should be a \ref vpp::format template
    instance. In most cases, leave this parameter unset, as VPP will simply use the same
    format as the image is in.

    \subsection secImageBindingPoints Image binding points

    Image binding points are placed inside a vpp::PipelineConfig or vpp::ComputePipelineConfig
    derived class, jsut as other binding points. All of them take the image view type as
    the first argument (some may take more optional arguments). There are the following
    binding point classes:
        - vpp::ioImage - for read/write unsampled (storage) images,
        - vpp::inTexture - for read-only sampled images (textures) combined with a sampler on GPU level (from the shader code).
        - vpp::inSampledTexture - for read-only sampled images (textures) combined with a sampler on CPU level during binding.
        - vpp::inConstSampledTexture - for read-only sampled images (textures) combined with a sampler on CPU level during construction.

    There are also binding points for samplers. These are also templates, taking the sampler
    type (either vpp::NormalizedSampler or vpp::UnnormalizedSampler) as the only argument:
        - inSampler - for samplers required to be bound to a shader data block,
        - inConstSampler - for samplers defined statically, during the construction.

    Those binding points which have the \c Const word in names, are statically bound to samplers.
    This is faster and simpler, but the sampler parameters can not be changed.

    vpp::inSampledTexture and vpp::inSampler allow to change the sampler by
    binding -- either by binding a different sampler to a shader data block, or by switching to
    a different shader data block. This allows to use different samplers for each draw command.

    vpp::inTexture image is combined with a sampler directly in the shader code. This allows
    to change a sampler even inside single draw call. It migh be slower than static or
    bound variants.

    \subsection secImageBindingUpdates Binding images to points

    Images are bound to binding points in the pipeline in exactly same way as buffers do.
    See section \ref secBufferBindingUpdates for an introduction.

    Images participate in same shader data blocks as buffers, are updated by means
    of the same vpp::ShaderDataBlock::update() method and can in fact be updated
    in the same method call as buffers (by mixing buffers and images on the same
    assignment list). You can proceed either this way, or write separate updating
    method for images (or some images) - it is up to you.

    An example of binding both a buffer and image in single call:

    \code
        typedef vpp::format< vpp::texture_format > TextureLoaderFmt;

        typedef vpp::ImageAttributes<
            TextureLoaderFmt, vpp::RENDER, vpp::IMG_TYPE_2D,
            vpp::Img::SAMPLED | vpp::Img::TARGET,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > TextureLoaderAttr;

        typedef vpp::Image< TextureLoaderAttr > TextureLoaderImage;
        typedef vpp::ImageViewAttributes< TextureLoaderImage > TextureLoaderViewAttr;
        typedef vpp::ImageView< TextureLoaderViewAttr > TextureLoaderView;

        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...

            void setDataBuffers (
                const vpp::UniformBufferView& fpv,
                const TextureLoaderView& texv,
                vpp::ShaderDataBlock* pDataBlock );

        private:
            vpp::inUniformBuffer m_framePar;
            vpp::inConstSampledTexture< TextureLoaderView > m_colorMap;
            // ...
        };

        void MyPipeline :: setDataBuffers (
            const vpp::UniformBufferView& fpv,
            const TextureLoaderView& texv,
            vpp::ShaderDataBlock* pDataBlock )
        {
            // Never forget about extra parentheses!
            pDataBlock->update ((
                
                // this binds the buffer
                m_framePar = fpv,

                // this binds the image
                m_colorMap = texv
            ));
        }

    \endcode

    Slightly more complicated form must be used when you bind both image
    and sampler simultaneously. This can happen only for vpp::inSampledTexture
    binding points. You need to use vpp::bind() function. As the first argument
    give the image view as above. As the second one, specify the sampler
    (normalized or unnormalized). Example:

    \code
        // ...
        vpp::inSampledTexture< TextureLoaderView > m_colorMap;
        // ...

        void MyPipeline :: setDataBuffers (
            const vpp::UniformBufferView& fpv,
            const TextureLoaderView& texv,
            const vpp::NormalizedSampler& sampler,
            vpp::ShaderDataBlock* pDataBlock )
        {
            // Never forget about extra parentheses!
            pDataBlock->update ((
                
                // this binds the buffer
                m_framePar = fpv,

                // this binds the image
                m_colorMap = vpp::bind ( texv, sampler )
            ));
        }
    \endcode

    Both forms accept one more optional parameter: image layout. This is
    a Vulkan layout code specifying in which layout the image is in when
    accessing it in a shader. Usually it is not specified and VPP detects
    automatically viable layout from the following allowed values:
    - VK_IMAGE_LAYOUT_GENERAL (selected for storage images),
    - VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL (selected for color textures
      and input attachments),
    - VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL (selected for depth/stencil
      textures and input attachments).

    If you have special needs regarding the layout, you can override it
    by specifying the layout as the final argument to the vpp::bind() function.
    VPP will pass it to the \c VkDescriptorImageInfo structure unchanged.
    This is advanced usage and do it only when you know what you are doing
    (can result in validation/runtime errors).

    \subsection secImageBindingOutAttachments Binding images to output attachments

    Output attachments are images that are the results of particular process in the
    render graph. All cases listed below are possible:
    - Single output attachment (most typical).
    - Multiple output attachments. There can be several output attachments per process,
      but this number is limited depending on the hardware. The minimum limit is
      4 output attachments. Typically mobile GPUs offer only the minimum limit.
      Currently all desktop GPUs allow at least 8 output attachments.
    - Zero output attachments. Images can be generated also by other means, e.g.
      storage images bound via vpp::ioImage binding points.

    Each output attachment should have a vpp::outAttachment binding point
    declared in thep pipeline configuration subclass. This binding point
    requires passing a reference to the corresponding render graph node
    in the constructor. This can be either:
    - a vpp::Attachment<> node,
    - a vpp::Display node.

    Output attachments binding points are not bound to actual images via
    shader data blocks, but rather through their corresponding nodes in
    render graph.

    A set of such bindings for all output attachments in particular render
    pass (and graph) is called a \b framebuffer in Vulkan. It is somewhat
    analogous to shader data block, but it is not switchable during rendering.

    One particular case is permanent binding of images to render graph nodes.
    In such case, you do not need to construct the framebuffer explicitly,
    as VPP maintains it internally. To use this simple variant, just pass
    image views to constructors of vpp::Display and vpp::Attachment nodes.

    In case of a display node, the role of image view is being performed by
    a vpp::Surface object. This way you can render directly to screen or a window.
    This configuration is shown in the example below.

    \code
        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            MyPipeline (
                const vpp::Process& pr,
                const vpp::Device& dev,
                const vpp::Display& outImage ) :
                    vpp::PipelineConfig ( pr ),

                    // Binds the binding point to the render graph node.
                    m_outColor ( outImage ),
                    // ...
            {}

            void fFragmentShader ( vpp::FragmentShader* pShader )
            {
                // Writing to the output attachment is very simple - just use
                // the assignment operator.
                // This example simply fills entire image with red color.
                m_outColor = vpp::Vec4 ( 1.0f, 0.0f, 0.0f, 1.0f );
            }

        private:
            vpp::outAttachment< FormatF32x4 > m_outColor;
            // ...
        };

        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph ( const vpp::Surface& hSurface ) :
                // This binds actual image (display surface) to the output node.
                m_display ( hSurface )
            {
                // Registers the output node for the process.
                m_render.addColorOutput ( m_display );
            }

        public:
            vpp::Preprocess m_init;
            vpp::Process m_render;
            vpp::Display m_display;
        };

        class MyRenderingEngine
        {
        public:
            MyRenderingEngine ( ... ) :
                // ...
                m_renderGraph ( m_surface ),

                // This passes m_display node to the pipeline and this will be bound
                // to m_outColor output attachment binding point.
                m_pipelineLayout ( m_renderGraph.m_render, m_device, m_renderGraph.m_display ),
                // ...
            {
                // ...
            }

        private:
            // ...
            MyRenderGraph m_renderGraph;
            vpp::PipelineLayout< MyPipeline > m_pipelineLayout;
            // ...
        };

    \endcode

    For vpp::Attachment node, pass an image view directly to the constructor, like
    in the following example:

    \code
        // This example shows two images: a color display and a depth (Z-buffer)
        // off-screen image.

        // Definitions below define image and view types for the Z-buffer.

        typedef vpp::format< float, vpp::DT > FormatDepth;

        typedef vpp::ImageAttributes<
            FormatDepth, vpp::RENDER, vpp::IMG_TYPE_2D, vpp::Img::DEPTH,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > DepthBufferAttr;

        typedef vpp::Image< DepthBufferAttr > DepthBufferImage;
        typedef vpp::ImageViewAttributes< DepthBufferImage > DepthBufferViewAttr;
        typedef vpp::ImageView< DepthBufferViewAttr > DepthBufferView;

        // Render graph

        class MyRenderGraph : public vpp::RenderGraph
        {
        public:
            MyRenderGraph (
                // Target surface for rendering.
                const vpp::Surface& hSurface,

                // A view pointing to image used as the Z-buffer.
                const DepthBufferView& depthBufferView );

                    // Initialize the display node with surface.
                    m_display ( hSurface ),

                    // Initialize the attachment node with the view.
                    m_depthBuffer ( depthBufferView )
            {
                // Register outputs in the rendering process.
                m_render.addColorOutput ( m_display );
                m_render.setDepthOutput ( m_depthBuffer, 1.0f );
            }

        public:
            vpp::Preprocess m_init;
            vpp::Process m_render;

            // Output image node.
            vpp::Display m_display;

            // Z-buffer image node
            vpp::Attachment< FormatDepth > m_depthBuffer;
        };
    \endcode

    As shown in both examples above, we can configure rendering without creating
    Vulkan framebuffer explicitly. This is suitable for scenarios where we do not
    want to change rendering targets.

    In case if the target images must not be bound permanently to render graph
    nodes, we can use FrameBuffer objects directly.











    \subsection secImageDisplays Swapchains, surfaces and displays

    \subsection secImageBindingInAttachments Binding images to input attachments

        vpp::inAttachment

    \section secOtherData Other data

    \subsection secPushConstants Push constants

        - vpp::inPushConstant

    \subsection secIndirectBuffers Indirect drawing buffers


    \subsection secQueries Queries

    \subsection secTimestamps Timestamps
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageComputation How to perform GPU-level computation

    \section secComputation How to perform GPU-level computation

    Computation is another application for GPUs besides image rendering.
    Because of high capabilities of modern GPUs this is very viable alternative
    to traditional CPU computing, at least in areas where large precision
    is not needed.

    Up till now, there existed several proprietary GPU computation technologies,
    most known being CUDA and OpenCL. VPP differs from them in following ways:
    - Being based entirely on Vulkan (which itself is portable across almost all major
      platforms and GPUs), programs written with VPP are highly portable and platform
      agnostic.
    - VPP does not require any special language nor compiler/preprocessor for source code.
      Having just C++ 14 compiler is sufficient.
    - Computation kernels are written entirely in C++ and can benefit from all
      C++ object-oriented and generic programming capabilities.
    - VPP automatically translates the code to SPIR-V representation, which is
      then being executed under the \c GLCompute execution model. While lacking
      certain features of the \c Kernel mode (used for OpenCL), it is also supported
      by large variety of hardware. That means, your GPU routines will be able to run
      on anything that supports Vulkan (desktop, mobile, etc.).
    - Creating reusable libraries of GPU code and deploying them is extremely
      easy, just as regular C++ code which runs on CPU.
    - All VPP is liberally (BSD) licensed open-source code. No proprietary, closed
      elements here.

    This chapter shows how to write computational programs using VPP. There can be
    two possible approaches, depending on how much control you want to have over
    the entire process.

    The easy method is to use vpp::ComputationEngine and vpp::Computation classes.
    They offer a way to execute a GPU program by just calling a function from C++.

    The more involved way, giving more control, is to use the vpp::ComputePass
    along with vpp::CommandBufferRecorder, vpp::CommandBuffer and vpp::Queue
    in order to schedule Vulkan commands manually. This method is harder but much
    more flexible, allowing to integrate multiple computation and rendering
    processes into single engine.

    For both methods, the basic premises and computation model are the same.

    \subsection subCompPipelines Computation pipelines

    First of all, you must make one or more computation pipeline classes.
    Derive these classes from vpp::ComputePipelineConfig base class. Compute
    pipelines are nearly identical to rendering pipelines, but they are not
    associated with processes in a render graph and can't use any attachments.
    Note that compute pipelines may also access and/or produce images, although
    not through attachments but only sampled/storage images.

    The only kind of shader allowed in computation pipelines are compute shaders.
    They are very similar to rendering shaders, but there is no inter-shader
    exchange variables (as there is only single shader in a pipeline) and no
    vertex data. All data consumed and produced by the shader must be contained
    in buffers, images and push constants.

    See the docs for vpp::ComputePipelineConfig for examples and more details.

    \subsection subCompPasses Compute passes

    The next step is to make a class derived from either vpp::ComputePass
    (low level method) or vpp::Computation (high level method). See introduction
    in the docs for vpp::ComputePass for details about computation model,
    local and global workgroups, compute shaders, local and shared variables,
    etc.

    The vpp::Computation base class is derived itself from vpp::ComputePass.
    Using it instead vpp::ComputePass gives you a compute pass with additional
    capabilities. vpp::Computation defines several overloads of \c operator()
    allowing to call the object in syntactically similar way to regular
    method call. These overloads launch computation and optionally wait for
    results. You can also use Vulkan synchronization objects (vpp::Fence,
    vpp::Semaphore) to synchronize these computations.

    You can have several computations in single computation engine. They
    can be arranged in a way that the computation listed latter in the
    owner class can wait for the results of preceding computation. The
    vpp::ComputationEngine maintains an implicit chain of Vulkan semaphores
    to achieve that effect.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageCode How to write shader and computation code

    \section secCode How to write shader and computation code

    In section \ref pageShadersExplanation we have explained how the C++ shaders
    work internally. Here is a list of typical elements and constructs found in
    these shaders. Consult individual pages for these items for more information.

    \subsection subPreamble Opening directive

    The first thing that is usually found in each VPP shader is the \c using
    directive, for easy access to VPP types:

    \code
        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;
        
            // ...
        }
    \endcode

    \subsection subAccessingVars Accessing binding points

    Some binding points require accessors - objects declared in the shader code.
    But not all, as there exist binding points accessed directly.

    \subsubsection ssAccVertexBuffers Accessing vertex and instance data buffers

    Vertex and instance data are supplied via vpp::inVertexData binding points.
    They are read-only and accessed directly: Example:

    \code
        vpp::inVertexData< TVertexPos > m_inVertexData;
        vpp::inVertexData< TInstancePar > m_inInstanceData;

        void fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;
            Vec4 vpos = m_inVertexData [ & GVertexPos::m_modelPosition ];
            Mat4 modelMatrix = m_inInstanceData [ & GInstancePar::m_model2world ];
        }
    \endcode

    \subsubsection ssAccUniformBuffers Accessing uniform buffers

    Uniform buffers are bound via vpp::inUniformBuffer or vpp::inUniformBufferDyn binding points.
    They are untyped, read-only and require an accessor which provides the type. The accessor
    can be one of the following:
    - vpp::UniformVar for single structure in the buffer.
    - vpp::UniformArray for array of structures in the buffer.
    - vpp::UniformSimpleArray for array of scalars, vectors or matrices in the buffer.

    The example below shows vpp::UniformVar. The remaining ones have identical syntax,
    but require one more level of indirection (the \c [] indexing operator) to access
    individual array element.

    \code
        vpp::inUniformBuffer m_framePar;

        void fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

            const Mat4 proj = inFramePar [ & GFrameParams::m_projection ];
            const Mat4 model = inFramePar [ & GFrameParams::m_model ];
        }

        // Another version, assuming that m_framePar holds entire array...
        void fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            UniformArray< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

            Int idx = ...; // compute array index somehow

            const Mat4 proj = inFramePar [ idx ][ & GFrameParams::m_projection ];
            const Mat4 model = inFramePar [ idx ][ & GFrameParams::m_model ];
        }
    \endcode

    \subsubsection ssAccStorageBuffers Accessing storage buffers

    Storage buffers are similar to uniform buffers, but they are read-write. 
    Their binding point types are vpp::ioBuffer and vpp::ioBufferDyn. Other
    details are the same. An example (showing simple array variant this time):

    \code
        vpp::ioBuffer m_target;

        void fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            UniformSimpleArray< float, decltype ( m_target ) > outTarget ( m_target );

            outTarget [ 0 ] = outTarget [ 0 ] * 2.0f;
        }
    \endcode

    \subsubsection ssAccTexelBuffers Accessing texel buffers

    Texel buffers are a hybrid of images and buffers. They are one-dimensional,
    can hold only arrays of simple data and are accessed via image functions
    (e.g. \c ImageStore or \c TexelFetch). VPP also provides the TexelArray
    accessor to allow using these buffers like regular buffers, with indexing
    operator instead of function calls. This is similar to vpp::UniformSimpleArray
    accessor. Corresponding binding points are vpp::inTextureBuffer and
    vpp::ioImageBuffer.

    \code
        typedef vpp::format< float, float, float, float > Fmt4xF32;
        vpp::inTextureBuffer< Fmt4xF32 > m_inTexBuf;
        vpp::ioImageBuffer< Fmt4xF32 > m_ioImgBuf;

        void fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            const IVec3 workgroupId = pShader->inWorkgroupId;
            const IVec3 localId = pShader->inLocalInvocationId;
            const Int g = workgroupId [ X ];
            const Int l = localId [ X ];
            const Int index = ( g << 5 ) | l;

            TexelArray< decltype ( m_inTexBuf ) > inTexBuf ( m_inTexBuf );
            TexelArray< decltype ( m_ioImgBuf ) > ioImgBuf ( m_ioImgBuf );
            const Int bufferLength = inTexBuf.Size();

            If ( index < bufferLength );
            {
                const Vec4 va = inTexBuf [ index ];
                ioImgBuf [ index ] = va;
            }
            Fi();
        }
    \endcode

    Caution: differently than uniform buffers, texel buffers require vpp::TexelBufferView
    objects to be explicitly constructed and stored along with corresponding buffers.
    Forgetting to do so will result in undefined behavior of texel buffers (sometimes work,
    sometimes not), currently the validation layer does not detect this. In order to make
    this error harder to occur, constructors of vpp::TexelBufferView are explicit.

    \subsubsection ssAccPushCon Accessing push constants

    Push constants are accessed in shaders in the same way as single-structure uniform
    buffers. They are actually small read-only uniform buffers with implicitly allocated
    memory.

    \code
        template< vpp::ETag TAG >
        struct TConstantData : public vpp::UniformStruct< TAG, TConstantData >
        {
            vpp::UniformFld< TAG, int > m_field1
            vpp::UniformFld< TAG, int > m_field2;
            vpp::UniformFld< TAG, float > m_field3;
            vpp::UniformFld< TAG, unsigned int > m_field4;
            vpp::UniformFld< TAG, vpp::matr2 > m_matrixField;
        };

        typedef TConstantData< vpp::GPU > GConstantData;
        typedef TConstantData< vpp::CPU > CConstantData;

        class MyPipeline : public vpp::ComputePipelineConfig
        {
        public:
            // ...

            inline CConstantData& data() { return m_sourceData.data(); }

            inline void setField1 ( int v ) { m_sourceData.data().m_field1 = v; }
            // ...
            
            inline void push() { m_sourceData.cmdPush(); }

            void fComputeShader ( vpp::ComputeShader* pShader );

        public:
            vpp::inPushConstant< TConstantData > m_sourceData;
            vpp::computeShader m_shader;

            // ...
        };

        void MyPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            // Use same syntax as for uniform buffers.
            UniformVar< TConstantData, decltype ( m_sourceData ) > inSourceData ( m_sourceData );

            const Int f1 = inSourceData [ & GConstantData::m_field1 ];
            const Int f2 = inSourceData [ & GConstantData::m_field2 ];
            const Float f3 = inSourceData [ & GConstantData::m_field3 ];
            const UInt f4 = inSourceData [ & GConstantData::m_field4 ];
            const Mat2 fm = inSourceData [ & GConstantData::m_matrixField ];
        }
    \endcode

    Push constants are written on the CPU side directly. In the example above, use
    the <tt>data()</tt> method to obtain a reference to \c CConstantData structure, 
    and write the values directly (like in \c setField1 method). To transfer
    the structure value, use \c cmdPush method on the vpp::inPushConstant object.

    This should be done from a command sequence (lambda function). The \c cmdPush
    makes a local copy of the entire structure and schedules a command for the GPU
    to update its own copy of the structure using the local one as the source. 
    These values will be visible in shaders launched by subsequent draw or computation
    commands.

    This way you can actually schedule multiple structure updates from single command
    sequence. Each \c cmdPush memorizes whatever values were set in the
    vpp::inPushConstant::data() object at the time of calling and generates a command
    to be executed later, to set exactly these values. So the following pattern is possible:

    \code
        m_sourceData.data().m_field1 = ...; value11
        m_sourceData.data().m_field2 = ...; value21
        // ...
        m_sourceData.cmdPush();

        cmdDraw ( ... ); // shaders will use value11, value21 ...

        m_sourceData.data().m_field1 = ...; value12
        m_sourceData.data().m_field2 = ...; value22
        // ...
        m_sourceData.cmdPush();

        cmdDraw ( ... ); // shaders will use value12, value22 ...
    \endcode

    \subsubsection ssAccInterShader Accessing inter-shader variables

    Inter-shader variables allow to pass some data from one shader to the next shader
    in pipeline. This always must be strictly following shader, skipping shaders
    is not allowed.

    The binding point is vpp::ioVariable or vpp::ioStructure. Provide the data type
    as well as source and target shaders for the template.

    These binding points need an accessor, to tell whether we are writing to the
    variable or reeading from it. These accessors are named vpp::Output and vpp::Input
    respectively.

    Passing the data to the fragment shader (from any other shader type) involves
    automatic interpolation, with the exception of integer data.

    Example:

    \code
        vpp::ioVariable< vpp::Vec2, vpp::vertexShader, vpp::fragmentShader > m_ioUV;

        void fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            Output< decltype ( m_ioUV ) > outUV ( m_ioUV );

            outUV = m_vertices [ & GVertexAttr::m_uv ][ XY ];
        }

        void fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            Input< decltype ( m_ioUV ) > inUV ( m_ioUV );

            const Vec4 color = Texture ( m_colorMap, inUV );

            // ...
        }
    \endcode

    \subsubsection ssAccTextures Accessing textures (sampled images)

    Textures are read-only images working together with a sampler. Binding
    points vpp::inConstSampledTexture and vpp::inSampledTexture expose
    textures already associated with a sampler, so there is no need to
    worry about it in the shader.

    In order to read from a texture, use the vpp::Texture function or any
    other function from \c Texture family (there are a lot of them).
    All these functions require coordinates, and sometimes other arguments.
    Textures do not need accessors. An example:

    \code
        vpp::inConstSampledTexture< TextureLoaderView > m_colorMap;

        void fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            Input< decltype ( m_ioUV ) > inUV ( m_ioUV );

            const Vec4 texelColor = Texture ( m_colorMap, inUV );

            // ...
        }
    \endcode

    \subsubsection ssAccStorageImg Accessing storage images

    Storage images are images accessed without sampling. Individual pixels
    are being read or written directly. Binding point type associated with
    storage images is vpp::ioImage. There are a number of functions which
    take this binding point as an argument. No accessors are needed.
    Example:

    \code
        vpp::ioImage< MyImgView > m_img1;
        vpp::ioImage< MyImgView > m_img2;

        void fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            const IVec3 workgroupId = pShader->inWorkgroupId;
            const IVec3 localId = pShader->inLocalInvocationId;
            const Int g = workgroupId [ X ];
            const Int l = localId [ X ];
            const Int index = ( g << 5 ) | l;

            const IVec2 imgSize = ImageSize ( m_img1 );
            const Int width = imgSize [ X ];
            const Int height = imgSize [ Y ];
            const Int x = index % width;
            const Int y = index / width;

            If ( y < height );
            {
                const IVec2 coords = IVec2 ( x, y );
                const Vec4 v = ImageLoad ( m_img1, coords );
                ImageStore ( m_img2, coords, v );
            }
            Fi();
        }
    \endcode

    The above example copies pixels from one image to another. vpp::ImageSize()
    can be used to retrieve the size of the image. vpp::ImageLoad() reads a pixel,
    vpp::ImageStore() writes it. Coordinates are expressed as an integer vector
    with number of components equal to image dimensionality (plus one, if the image
    is arrayed).

    \subsubsection ssAccInputAtt Accessing input attachments

    Input attachments allow to receive data from another vpp::Process node in
    the rendering graph. An input attachment in target process is simultaneously
    output attachment in the source process. VPP and Vulkan automatically maintain
    a dependency between the processes.

    Input attachments are accessed (read-only) via vpp::inAttachment binding point.

    Each input attachment requires allocation of image and image view. The image
    should have vpp::Img::INPUT usage bit set, as well as one of output attachment
    usage bits (vpp::Img::COLOR or vpp::Img::DEPTH). The vpp::inAttachment template
    should be parameterized with the view type, just like texture or storage image.

    Reading of input attachments may occur only in fragment shaders. No other shader
    type is allowed. Reading is being performed by means of vpp::SubpassLoad() function.
    Coordinates are relative to current pixel position in particular fragment shader call. 
    Usually call the overload without arguments, as it supplies zero offset and simply
    reads current pixel.

    Reading is fully synchronized by Vulkan. The pixel value you read is guaranteed to be
    final pixel value generated by preceding process. This is true even if the pixel
    is being written multiple times or constructed incrementally (via blending or logical
    operations).

    Example:

    \code
        typedef vpp::format< vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t, vpp::unorm8_t > format_type;

        typedef vpp::ImageAttributes<
            format_type, vpp::RENDER, vpp::IMG_TYPE_2D,
            vpp::Img::COLOR | vpp::Img::INPUT,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > AttAttrType;

        typedef vpp::Image< AttAttrType > AttImgType;
        typedef vpp::ImageViewAttributes< AttImgType > AttVaType;
        typedef vpp::ImageView< AttVaType > AttViewType;

        class MyPipeline : public vpp::WholeScreenPatchPipelineConfig
        {
        public:
            MyPipeline (
                const vpp::Process& pr,
                const vpp::Device& dev,
                const vpp::Attachment< format_type >& input,
                const vpp::Attachment< format_type >& output ) :
                    vpp::WholeScreenPatchPipelineConfig ( pr, dev ),
                    m_inColor ( input ),
                    m_outColor ( output ),
                    m_fragmentShader ( this, & MyPipeline::fFragmentShader )
            {}

            void setDataBuffers (
                const AttViewType& inputView,
                vpp::ShaderDataBlock* pDataBlock )
            {
                pDataBlock->update (( m_inColor = inputView ));
            }

            void fFragmentShader ( vpp::FragmentShader* pShader )
            {
                using namespace vpp;
                const Vec4 pix = SubpassLoad ( m_inColor );
                m_outColor = pix;
            }

        private:
            vpp::inAttachment< AttViewType > m_inColor;
            vpp::outAttachment< format_type > m_outColor;
            vpp::fragmentShader m_fragmentShader;
        };

    \endcode
    
    \subsubsection ssAccSamplers Accessing samplers

    Samplers can be associated with images in static or dynamic way. Dynamic samplers
    are pipeline objects just as images themselves. They have binding points and
    must be bound to shader blocks.

    Binding point type for samplers is vpp::inSampler. This is a template which takes either
    vpp::NormalizedSampler or vpp::UnnormalizedSampler type.

    In shaders, the only thing to do with these samplers is to associate them with
    texture image represented by vpp::inTexture binding point. This is being done by
    calling the vpp::MakeSampledTexture() function, as in the example below. This function
    returns some opaque type that is allowed to be used in vpp::Texture and other
    texture reading functions.

    vpp::MakeSampledTexture() must not be called within conditional blocks.

    Example:

    \code
        typedef vpp::format< float, float, float, float > FmtType;
        typedef typename FmtType::data_type DataType;

        typedef vpp::ImageAttributes<
            FmtType, vpp::RENDER, vpp::IMG_TYPE_2D,
            vpp::Img::SAMPLED | vpp::Img::TARGET,
            VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
            false, false > TextureAttr;

        typedef vpp::Image< TextureAttr > TextureImage;
        typedef vpp::ImageViewAttributes< TextureImage, vpp::SAMPLER_UNNORMALIZED > TextureViewAttr;
        typedef vpp::ImageView< TextureViewAttr > TextureView;

        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...

            void setBuffers (
                const TextureView& tv,
                const vpp::NormalizedSampler& nsamp,
                const vpp::UnnormalizedSampler& usamp,
                vpp::ShaderDataBlock* pDataBlock );
            {
                pDataBlock->update ((
                    m_texture = tv,
                    m_normSampler = nsamp,
                    m_unnormSampler = usamp
                ));
            }

            void fFragmentShader ( vpp::FragmentShader* pShader );
            {
                using namespace vpp;

                const Vec2 coords = pShader->inFragCoord [ XY ];
    
                const auto normSampledTexture = MakeSampledTexture ( m_texture, m_normSampler );
                const auto unnormSampledTexture = MakeSampledTexture ( m_texture, m_unnormSampler );

                const Vec4 pixValue1 = Texture ( unnormSampledTexture, coords );
                // do something with pixValue1...

                const Vec2 is = StaticCast< Vec2 >( ImageSize ( d_texture ) );
                const Vec2 scaledCoords = coords / is;

                const Vec4 pixValue2 = Texture ( normSampledTexture, scaledCoords );
                // do something with pixValue2...
            }

        private:
            vpp::inSampler< vpp::NormalizedSampler > m_normSampler;
            vpp::inSampler< vpp::UnnormalizedSampler > m_unnormSampler;
            vpp::inTexture< TextureView > m_texture;

            // ...

            vpp::fragmentShader m_fragmentShader;
        };

    \endcode

    Another variant of samplers, "semi-dynamic", are represented with
    vpp::inConstSampler binding points. They use the same syntax as vpp::inSampler,
    but are not bound to shader data blocks. Instead, these binding points require
    corresponding samplers to be passed directly to the constructor.

    Example:

    \code
        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            // ...

            MyPipeline :: MyPipeline (
                const vpp::Process& pr,
                const vpp::Device& dev,
                const vpp::NormalizedSampler& nsamp,
                const vpp::UnnormalizedSampler& usamp ) :
                    vpp::PipelineConfig ( pr ),
                    m_normSampler ( nsamp ),
                    m_unnormSampler ( usamp ),
                    m_fragmentShader ( this, & SampTestPipeline::fFragmentShader )
            {
            }

            // ...

            void fFragmentShader ( vpp::FragmentShader* pShader );
            {
                using namespace vpp;

                const Vec2 coords = pShader->inFragCoord [ XY ];
    
                const auto normSampledTexture = MakeSampledTexture ( m_texture, m_normSampler );
                const auto unnormSampledTexture = MakeSampledTexture ( m_texture, m_unnormSampler );

                const Vec4 pixValue1 = Texture ( unnormSampledTexture, coords );
                // do something with pixValue1...

                const Vec2 is = StaticCast< Vec2 >( ImageSize ( d_texture ) );
                const Vec2 scaledCoords = coords / is;

                const Vec4 pixValue2 = Texture ( normSampledTexture, scaledCoords );
                // do something with pixValue2...
            }

        private:
            vpp::inConstSampler< vpp::NormalizedSampler > m_normSampler;
            vpp::inConstSampler< vpp::UnnormalizedSampler > m_unnormSampler;
            vpp::inTexture< TextureView > m_texture;

            // ...

            vpp::fragmentShader m_fragmentShader;
        };


    \endcode

    \subsubsection ssAccOutputAtt Accessing output attachments

    Output attachments are accessed though vpp::outAttachment binding points.
    There is only one thing you can do with them - write current pixel value.
    This is accomplished just by using the assignment (\c = ) operator. This
    syntax is different than most other resources, but simpler.

    An example:

    \code
        vpp::outAttachment< FormatF32x4 > m_outColor;

        void fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

            Input< decltype ( m_ioNormal ) > inNormal ( m_ioNormal );
            Input< decltype ( m_ioColor ) > inColor ( m_ioColor );
            Input< decltype ( m_ioUV ) > inUV ( m_ioUV );
            Input< decltype ( m_ioViewVec ) > inViewVec ( m_ioViewVec );
            Input< decltype ( m_ioLightVec ) > inLightVec ( m_ioLightVec );

            const Vec4 color = Texture ( m_colorMap, inUV );
            const Vec3 vecN = Normalize ( inNormal );
            const Vec3 vecL = Normalize ( inLightVec );
            const Vec3 vecV = Normalize ( inViewVec );
            const Vec3 vecR = Reflect ( -vecL, vecN );
            const Vec3 diffuse = Max ( Dot ( vecN, vecL ), 0.0f ) * inColor;
            const Vec3 specular = Pow ( Max ( Dot ( vecR, vecV ), 0.0f ), 16.0f ) * Vec3 ( 0.75f );

            m_outColor = Vec4 ( diffuse * color [ XYZ ] + specular, 1.0f );
        }
    \endcode

    \subsubsection ssAccResourceArrays Accessing binding point arrays
    
    Arrays of binding points are created by means of vpp::arrayOf template. Individual
    points are then referenced ny using extra bracket operator \c [], as shown in section
    \ref ssIndBuffersArrays.

    \subsection subDeclaringVars Declaring and using local variables

    \subsubsection ssImmutableVars Immutable variables

    Immutable variables can be declared and initialized, but may not be changed later.
    On the other hand, they are very easily optimized and contribute to very efficient
    code. In most algorithms, there is a need for only a few variables that are mutable
    (e.g. loop control), the rest of them may be immutable.

    And example below shows several immutable variables. Using \c const specifier is
    optional, they are immutable regardless you declare them \c const or not.

    \code
        void fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            // ...

            const Vec4 color = Texture ( m_colorMap, inUV );
            const Vec3 vecN = Normalize ( inNormal );
            const Vec3 vecL = Normalize ( inLightVec );
            const Vec3 vecV = Normalize ( inViewVec );
            const Vec3 vecR = Reflect ( -vecL, vecN );
            const Vec3 diffuse = Max ( Dot ( vecN, vecL ), 0.0f ) * inColor;
            const Vec3 specular = Pow ( Max ( Dot ( vecR, vecV ), 0.0f ), 16.0f ) * Vec3 ( 0.75f );

            // ...
        }
    \endcode

    \subsubsection ssMutableVars Mutable variables and arrays

    Mutable variables can be changed at will. But the price to pay for that is high.
    For each mutable variable, the shader compiler must allocate a permanent
    register on GPU and must do it for each concurrent thread. Typical GPU these days
    can run at least a thousand of threads simultaneously, and the register pool is
    several thousands (later or more expensive GPUs may have more). The register pool
    can be quickly exhausted by using more than 8-10 simple mutable variables per thread.
    When there is no more registers, the compiler will allocate regular memory and that
    will slow down your shader 10 times.

    Therefore use immutable variables by default. Declare mutable ones only if
    needed. Reuse them throughout your shader. Note that C++ optimizing compiler will not
    be able to optimize usage of these variables, so you must do it yourself.

    You can also declare mutable arrays of fixed size. This is done by means of vpp::VArray
    template. Specify item type as the first parameter and size as the second. All remarks
    about efficiency concern arrays as well - so declare only small arrays.

    Some trivial examples:

    \code
        VArray< Float, 6 > v;
        VInt i = 0;
        VInt j = 0;

        i = 1;
        j = 2;

        v [ j ] = i;
        v [ j + 1 ] = v [ j ] + 1;
    \endcode

    \subsubsection ssSharedVars Shared variables

    The GPU runs large number of threads in parallel (1000 or more). Those threads
    are logically organized into workgroups. A workgroup is smaller group of threads
    (typically 32 or 64) that run on single Computation Unit on the GPU.
    
    As of writing this (2018), contemporary GPU architectures became very similar
    to multicore SIMD CPU architectures, like regular Intel or AMD processors.
    A regular processor has e.g. 8 cores and 8-way SIMD in single core (AVX and
    AVX2). Now imagine a processor with 32-way SIMD and 32 cores -- this is roughly
    an entry-level GPU. Each core also has its own data/instruction caches and GPR
    register pool. An unique feature of GPUs is allowing to control explicitly what
    variables are shared over entire core. These variables are called \b shared
    variables.

    Shared variables have two distinguishing traits:
    - A single copy of such variable is create per entire workgroup, which are
      much less numerous than threads. Hence the register usage is much less
      than regular mutable variables.
    - The variable is visible across all threads inside the group and can be used
      for communication.

    To declare a variable as shared inside shader code, use the vpp::Shared()
    function. Place it before the declaration, like this:

    \code
        Shared(); VInt vi;
        Shared(); VArray< Vec3, 256 > lightPositions;
    \endcode

    Shared arrays are also possible and they are much less performance-sensitive.
    Actually shared arrays are very practical method for data exchange between threads
    in a single group, or temporary data storage when you can ensure that different
    threads do not access same fields simultaneously (otherwise consider using atomic
    variables, described in next section).

    \subsubsection ssAtomicVars Atomic variables
    \code
    \endcode

    \subsection subBuiltinVars Accessing built-in variables

    Built-in variables are very important aspect of shader code authoring. These
    variables are predefined for each shader type and are accessible via the shader
    object given as the parameter to the shader. For example:

    \code
        void fFragmentShader ( vpp::FragmentShader* pShader )
        {
            pShader-> ... // built-in variables are there
        }
    \endcode

    Each shader type has its own object representation: vpp::VertexShader,
    vpp::GeometryShader, vpp::TessControlShader, vpp::TessEvalShader, vpp::FragmentShader,
    and vpp::ComputeShader. Each of these objects define its own set of built-in
    variables, specific to that shader type. Shader objects also offer some methods
    (described elsewhere).

    Built-in variables have different types. See the docs of each shader object for
    the list of variables and their types.

    Some examples:

    \code
        void MyPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            const IVec3 workgroupId = pShader->inWorkgroupId;
            const IVec3 localId = pShader->inLocalInvocationId;
            const Int g = workgroupId [ X ];
            const Int l = localId [ X ];

            // ...
        }

        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            const Int vertexIndex = pShader->inVertexIndex;
            const Int instanceIndex = pShader->inInstanceIndex;
            // ...
        }

        void MyPipeline :: fFragmentShader ( vpp::FragmentShader* pShader )
        {
            using namespace vpp;

            const Vec2 pixelCoords = pShader->inFragCoord [ XY ];

            // ...
        }

    \endcode

    \subsection subIndexing Indexing operators

    Vectors and arrays are important part of programming, both CPU and GPU.
    VPP offers overloaded indexing operators (\c []) allowing to access
    individual elements of various aggregates: local arrays, buffers,
    arrayed built-in variables, vectors, matrices, structures, etc.
    Subsequent sections describe them in more detail.

    \subsubsection ssIndLocalArr Local arrays

    Local arrays are described in section \ref ssMutableVars. The indexing
    operator for them accepts any integer expression (variable or constant).
    Examples:

    \code
        VInt vi;
        Int i = 1;
        VArray< Float, 6 > v;

        v [ vi ] = v [ i ];
        v [ i + 1 ] = v [ 0 ];

        const int macroIndex = ...; // CPU computed expression
        const Int t = v [ macroIndex ];
    \endcode

    \subsubsection ssIndVectors Vectors

    Immutable or mutable vectors of types like vpp::Vec2, vpp::Vec3, vpp::Vec4,
    vpp::IVec2, vpp::IVec3, vpp::IVec4, vpp::VVec2, vpp::VVec3, vpp::VVec4,
    vpp::VIVec2, vpp::VIVec3, vpp::VIVec4, etc. can be indexed much like local
    arrays. Any integer expression can be used inside the \c [] operator. The
    index is zero based, like in arrays. The value must be less than vector
    size, otherwise the result is undefined. For mutable vectors, assignment
    to indexed locations is permitted.

    Examples:
    \code
        IVec2 ivec2 { 11, 13 };
        Int v21 = ivec2 [ 1 ];
        Int idx2 = ...; // some index
        Int v22 = ivec2 [ idx2 ];

        VVec4 vvec4;
        VInt vidx;

        For ( vidx, 0, 4 );
            vvec4 [ vidx ] = StaticCast< Float >( vidx );
        Rof();

        Float v41 = vvec4 [ 1 ];
        Int idx4 = ...; // some index
        Float v44 = vvec4 [ idx4 ];
    \endcode

    \subsubsection ssIndVecSwizzles Vector swizzles

    Swizzles are special method of indexing vectors, involving using of
    component names rather than numeric indices. Entire slices of vectors
    can be easily specified by these names, for example:
    - <tt>ivec4 [ XYZW ]</tt> means a vector formed from the following 
      components of the original vector: 0, 1, 2, 3. Actually it is identical
      to the source vector.
    - <tt>ivec4 [ WZYX ]</tt> means a vector formed from the following 
      components of the original vector: 3, 2, 1, 0. The component order
      is reversed.
    - <tt>ivec4 [ YYYY ]</tt> means a vector formed from the following 
      components of the original vector: 1, 1, 1, 1. This is just the
      \c Y component copied into remaining components.
    - <tt>ivec4 [ XYZ ]</tt> is a 3-element vector built from 3 lower
      elements of the source vector. This one is very often used to
      truncate 4D vector to 3D vector in shaders.
    - <tt>ivec4 [ W ]</tt> is a scalar equal to the last element of
      the source vector.

    These are just a few examples. Any combination of letters
    \c X, \c Y, \c Z, \c W may be used. The length of the combination
    must be equal or less than the size of original vector. Swizzles
    can be used for any vector length and component type.

    Swizzle names are defined as enumeration types: vpp::ESwizzle1, vpp::ESwizzle2,
    vpp::ESwizzle3, vpp::ESwizzle4. Because of that, they require either
    a <tt>using namespace vpp;</tt> directive, or explicit qualification with
    <tt>vpp::</tt> prefix.

    Swizzles can be combined with other indexing operators, but the swizzle either
    must be the last one of them in chain, or all indices coming \b after the swizzle
    must be constants or CPU variables.

    Example:
    \code
        template< vpp::ETag TAG >
        struct TVertexAttr : public vpp::VertexStruct< TAG, TVertexAttr >
        {
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_pos;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_normal;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_uv;
            vpp::Attribute< TAG, vpp::ext< glm::vec4 > > m_color;
        };

        template< vpp::ETag TAG >
        struct TFrameParams : public vpp::UniformStruct< TAG, TFrameParams >
        {
            vpp::UniformFld< TAG, glm::mat4 > m_projection;
            vpp::UniformFld< TAG, glm::mat4> m_model;
            vpp::UniformFld< TAG, glm::vec4 > m_lightPos;
        };

        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioNormal;
        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioColor;
        vpp::ioVariable< vpp::Vec2, vpp::vertexShader, vpp::fragmentShader > m_ioUV;
        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioViewVec;
        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioLightVec;

        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

            Output< decltype ( m_ioNormal ) > outNormal ( m_ioNormal );
            Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
            Output< decltype ( m_ioUV ) > outUV ( m_ioUV );
            Output< decltype ( m_ioViewVec ) > outViewVec ( m_ioViewVec );
            Output< decltype ( m_ioLightVec ) > outLightVec ( m_ioLightVec );

            // The following code uses swizzles to truncate vectors.

            outColor = m_vertices [ & GVertexAttr::m_color ][ XYZ ];
            outUV = m_vertices [ & GVertexAttr::m_uv ][ XY ];

            const Mat4 proj = inFramePar [ & GFrameParams::m_projection ];
            const Mat4 model = inFramePar [ & GFrameParams::m_model ];
            const Mat3 model3 = Mat3 ( model );

            const Vec4 pos = model * m_vertices [ & GVertexAttr::m_pos ];
            pShader->outVertex.position = proj * pos;

            outNormal = model3 * m_vertices [ & GVertexAttr::m_normal ][ XYZ ];

            const Vec3 lPos = model3 * inFramePar [ & GFrameParams::m_lightPos ][ XYZ ];
            outLightVec = lPos - pos [ XYZ ];

            outViewVec = -pos [ XYZ ];
        }

    \endcode

    Swizzles can also be used to write components, i.e. on the left side of
    the assignment operator. Examples:

    \code
        VVec4 vv4;

        vv4 [ X ] = ...; // compute the value
        vv4 [ Y ] = ...; // compute the value
        vv4 [ Z ] = ...; // compute the value
        vv4 [ W ] = ...; // compute the value

        // this reverses the components
        vv4 [ WZYX ] = vv4;
    \endcode

    \subsubsection ssIndMatrices Matrices

    Matrix variables can also be mutable or immutable. Some examples of immutable
    matrix types: vpp::Mat2, vpp::Mat3, vpp::Mat4, vpp::Mat4x2, vpp::Mat3x4,
    vpp::IMat4x2, vpp::UMat2x3, etc. Mutable versions have prefix \c V, e.g.
    vpp::VMat4, vpp::VIMat3x2, etc.

    Matrices are indexed similarly to vectors. A matrix is equivalent to a vector
    of columns (vector of vectors), therefore the first indexing operator applied
    to it selects a column, and the second one selects an element within column.

    \code
        IMat2 imat2
        {
            11, 12,
            21, 22
        };

        Int a11 = imat2 [ 0 ][ 0 ];  // equal to 11
        Int a12 = imat2 [ 1 ][ 0 ];  // equal to 12
        Int a21 = imat2 [ 0 ][ 1 ];  // equal to 21
        Int a22 = imat2 [ 1 ][ 1 ];  // equal to 22
    \endcode

    The value of the index can be any integer expression (variable or constant).

    If a matrix is indexed with only one indexing operator, the result is
    a column vector. For example:

    \code
        IMat2 imat2
        {
            11, 12,
            21, 22
        };

        // A vector of elements: { 11, 21 }
        IVec2 imat20 = imat2 [ 0 ];

        // A vector of elements: { 12, 22 }
        IVec2 imat21 = imat2 [ 1 ];

    \endcode

    For mutable types like vpp::VMat2 or vpp::VIMat2 indexing operators can be used on
    the left side of assignments. Examples:

    \code
        IMat2 imat2 { 11, 12, 21, 22 };

        // Sets vimat2 equal to imat2.
        VIMat2 vimat2 = imat2;

        // Changes elements of vimat2.
        vimat2 [ 0 ][ 0 ] = 111;
        vimat2 [ 1 ][ 0 ] = 112;
        vimat2 [ 0 ][ 1 ] = 121;
        vimat2 [ 1 ][ 1 ] = 122;

        // Sets columns of vimat2.
        vimat2 [ 0 ] = imat2 [ 0 ];
        vimat2 [ 1 ] = imat2 [ 1 ];

    \endcode

    \subsubsection ssIndBuffersSingle Buffers containing single structure

    This section concerns uniform or storage buffers which hold only single
    structure. This can be e.g. parameters and matrices for entire rendering frame. 

    Buffers like these are accessed by means of vpp::UniformVar accessor object.
    Use it as in the example below.

    \code
        template< vpp::ETag TAG >
        struct TFrameParams : public vpp::UniformStruct< TAG, TFrameParams >
        {
            vpp::UniformFld< TAG, glm::mat4 > m_projection;
            vpp::UniformFld< TAG, glm::mat4> m_model;
            vpp::UniformFld< TAG, glm::vec4 > m_lightPos;
        };

        typedef TFrameParams< vpp::GPU > GFrameParams;
        typedef TFrameParams< vpp::CPU > CFrameParams;

        class MyPipeline : public vpp::PipelineConfig
        {
        public:
            vpp::inUniformBuffer m_framePar;

            void fVertexShader ( vpp::VertexShader* pShader )
            {
                using namespace vpp;

                UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );

                const Mat4 proj = inFramePar [ & GFrameParams::m_projection ];
                const Mat4 model = inFramePar [ & GFrameParams::m_model ];
            }

            // ...
        }
    \endcode

    The accessor provides indexing operator allowing to access fields defined
    within vpp::UniformStruct definition. This operator can be used to read
    fields, as well as to write in case the buffer is vpp::ioBuffer.

    The vpp::UniformVar accessor is also used with vpp::inPushConstant binding
    points, to access push constants. This access is read-only. From this perspective,
    push constants do not differ from uniform buffers. However, they are written
    on the CPU side in a different manner and are faster.

    \subsubsection ssIndBuffersMultiple Buffers containing multiple structures

    If there is an entire array of structures within the buffer, use
    vpp::UniformArray accesor instead of vpp::UniformVar. The binding point types
    are the same. This accessor defines and index operator taking integer
    index (any integer expression is allowed, variable or constant) and the
    result can be indexed with a field name.

    \code

    \endcode

    \subsubsection ssIndBuffersSimple Buffers containing multiple simple objects

    There can be two major kinds of simple data buffers: regular uniform buffers
    and texel buffers.
    
    Regular uniform buffers ???


    vpp::UniformSimpleArray
    vpp::TexelArray

    \subsubsection ssIndBuffersArrays Arrays of buffers

    Arrays of buffers (and images) are a different thing from buffers containing arrays.
    This time we have multiple \b buffers themselves. These buffer arrays are declared 
    with the help of vpp::arrayOf template, like in the following code (shows examples
    for all supported arrays):

    \code
        vpp::arrayOf< vpp::inUniformBuffer > m_inUniformBufferArr;
        vpp::arrayOf< vpp::inUniformBufferDyn > m_inUniformBufferDynArr;
        vpp::arrayOf< vpp::ioBuffer > m_ioBufferArr;
        vpp::arrayOf< vpp::ioBufferDyn > m_ioBufferDynArr;
        vpp::arrayOf< vpp::ioImageBuffer< FormatUInt32 > > m_ioImageBufferArr;
        vpp::arrayOf< vpp::inTextureBuffer< FormatUInt32 > > m_inTextureBufferArr;
        vpp::arrayOf< vpp::ioImage< VarConstU32View > > m_ioImageArr;
        vpp::arrayOf< vpp::inSampler< vpp::NormalizedSampler > > m_inSamplerArr;
        vpp::arrayOf< vpp::inConstSampler< vpp::UnnormalizedSampler > > m_inConstSamplerArr;
        vpp::arrayOf< vpp::inTexture< VarConstU32View > > m_inTextureArr;
        vpp::arrayOf< vpp::inSampledTexture< StdTextureView > > m_inSampledTextureArr;
        vpp::arrayOf< vpp::inConstSampledTexture< StdTextureView > > m_inConstSampledTextureArr;
        vpp::arrayOf< vpp::inSampledTexture< DepthTextureView > > m_inSampledDepthTextureArr;
        vpp::arrayOf< vpp::inConstSampledTexture< DepthTextureView > > m_inConstSampledDepthTextureArr;
    \endcode

    All of these arrays provide additional level of indexing when being accessed in the shader.
    Accepted indexes are any integer expressions, variable or constant.

    The syntax differs slightly for buffers and images, because buffers use accessors
    and images do not. For buffers, you apply extra indexing to the accessor. Example:

    \code
        Int idx = ...; // some index of buffer within the array, may be computed dynamically.

        // Examples for reading arrayed buffer. The extra index goes first.
        IVec4 q1 = varUniformBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ];
        Int q1x = varUniformBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ][ X ];

        // For comparison, the same for regular uniform buffer.
        IVec4 p1 = varUniformBuffer [ & GFrameSelector::d_frameSelectorParams ];
        Int p1x = varUniformBuffer [ & GFrameSelector::d_frameSelectorParams ][ X ];

        // Works same way for writes:
        varIoBufferArr [ idx ][ & GFrameSelector::d_frameSelectorParams ] = q3;
        varIoBufferDynArr [ idx ][ & GFrameSelector::d_frameSelectorParams ] = q4;

    \endcode

    For images, apply the indexing operator to the binding point name:

    \code
        Int idx = ...; // some index of image within the array, may be computed dynamically.

        // Reads a texel from one of the images within the array.
        Vec4 s103 = Texture ( d_inSampledTextureArr [ idx ], c );

        // For comparison, same thing for single image.
        Vec4 s101 = Texture ( d_inSampledTexture, c );

    \endcode

    There is one thing for images that might be confusing. There are two different methods
    of arraying images. The one shown above involves an array of separately bound images.
    Actually this is an array of independent binding points. A different vpp::Image or vpp::Img
    object may be bound to each item in the array. Use vpp::multi template to make such selective
    bindings.
    
    The other kind of image arraying is to declare an image itself as arrayed (multilayered)
    so that it contains multiple layers. In such case this is single image, bound to single point.
    You specify layer index as an extra coordinate passed to functions like vpp::Texture.

    These two methods works completely independently from each other and in fact may be mixed.

    \subsubsection ssIndBuffersVertex Vertex and instance buffers

    Vertex and instance buffers are associated with structures defined by means of
    vpp::VertexStruct and vpp::InstanceStruct. Although they are physically arrays
    of structures, the vertex shader has access only to single, "current" element
    at any given moment. You apply the indexing operator directly to vpp::inVertexData
    binding point in the pipeline and provide a pointer to structure field, just like
    for single-element buffers (section \ref ssIndBuffersSingle). An example:

    \code
        // The binding point for vertices.
        vpp::inVertexData< TVertexAttr > m_vertices;

        vpp::inUniformBuffer m_framePar;
        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioNormal;
        vpp::ioVariable< vpp::Vec3, vpp::vertexShader, vpp::fragmentShader > m_ioColor;
        vpp::ioVariable< vpp::Vec2, vpp::vertexShader, vpp::fragmentShader > m_ioUV;

        vpp::vertexShader m_vertexShader;

        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            UniformVar< TFrameParams, decltype ( m_framePar ) > inFramePar ( m_framePar );
            Output< decltype ( m_ioNormal ) > outNormal ( m_ioNormal );
            Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
            Output< decltype ( m_ioUV ) > outUV ( m_ioUV );

            // Read some vertex attributes and pass them immediately to the next shader.
            // We specify only the field index, because the shader has only access to single
            // vertex. Actually for each vertex there is separate vertex shader call.
            outColor = m_vertices [ & GVertexAttr::m_color ][ XYZ ];
            outUV = m_vertices [ & GVertexAttr::m_uv ][ XY ];

            // This reads data from single element buffer - note that the syntax is
            // identical as for uniform buffers.
            const Mat4 proj = inFramePar [ & GFrameParams::m_projection ];
            const Mat4 model = inFramePar [ & GFrameParams::m_model ];
            const Mat3 model3 = Mat3 ( model );

            // Read vertex position, transform and write to the standard output
            // built-in variable. 
            const Vec4 pos = model * m_vertices [ & GVertexAttr::m_pos ];
            pShader->outVertex.position = proj * pos;

            // Read vertex normal, transform and pass it to the next shader.
            outNormal = model3 * m_vertices [ & GVertexAttr::m_normal ][ XYZ ];

            // ...
        }
    \endcode


    \subsubsection ssIndInterShader Inter-shader variables of structural types


    \subsection subControlsConstructs Basic control constructs

    VPP shader language offers a lot of constructs to control flow
    of the execution. All of these have different syntax from corresponding
    C++ constructs, as C++ keywords like \c if or \c for are not overloadable.

    Nevertheless, VPP and C++ statements may be mixed in code, which gives
    interesting possibilities. Generally, C++ constructs behave as a metalanguage
    to VPP constructs.

    In sections below short descriptions and examples are shown for
    these control constructs. Refer to individial docs pages for more information.

    \subsubsection ssIf 'If/Else/Fi' conditionals

    vpp::If() and vpp::Else() are counterparts of C++ \c if and \c else statements.
    Use as in the following example. Do not forget about vpp::Fi() at the end.

    \code
        const Bool bCanAddCluster = ( activeClusters < 8 );

        If ( bCanAddCluster );
        {
            clusterCenters [ activeClusters ] = dirVector;
            thr = threshold;

            ImageStore (
                d_partitionedSetOutput,
                IVec3 ( i, lightIndex, zoneIndex ),
                StaticCast< UInt >( j ) );

            ++activeClusters;
        }
        Else();
        {
            clusterCenters [ nearestCluster ] =
                ( dirVector + clusterCenters [ nearestCluster ] ) * Float ( 0.5f );

            thr = halfThreshold;

            ImageStore (
                d_partitionedSetOutput,
                IVec3 ( i, lightIndex, zoneIndex ),
                StaticCast< UInt >( nearestCluster ) );
        }
        Fi();
    \endcode

    \subsubsection ssSelect 'Select' conditionals

    vpp::Select() implements the conditional question mark operator (\c ?: )
    from C++. This operator is not overloadable, hence the need of separate construct.
    The order of arguments is the same as in the conditional operator.

    \code
        const Bool bCondition = ( ... );
        const Int valueIfTrue = ( ... );
        const Int valueIfFalse = ( ... );

        const Int j = Select ( bCondition, valueIfTrue, valueIfFalse );
    \endcode

    \subsubsection ssDoWhile 'Do/While' loops

    vpp::Do() and vpp::While() form basic looping construct. Always use them
    together and close the block with vpp::Od().

    \code
        VArray< Float, 6 > v;
        VInt i = 0;
        VInt j = 0;

        Do(); While ( i < 5 );
        {
            const Int i1 = i + 1;
            const Float vi = v [ i ];
            const Float vi1 = v [ i1 ];

            const Bool bValid = 
                vi < vi1
                && evaluatePoly0 ( vi, k5, k4, k3, k2, k1, k0 ) < 0
                && evaluatePoly0 ( vi1, k5, k4, k3, k2, k1, k0 ) > 0;

            v [ j ] =     Select ( bValid, vi, v [ j ] );
            v [ j + 1 ] = Select ( bValid, vi1, v [ j + 1 ] );
            i =           Select ( bValid, i + 2, i1 );
            j =           Select ( bValid, j + 2, j );
        }
        Od();
    \endcode

    \subsubsection ssFor 'For' loops

    vpp::For() implements a simplified \c for loop. It takes 3 or 4 arguments. The first
    one is the control variable which must be already declared mutable variable of
    type vpp::Int or vpp::UInt. The second argument is the starting value. The third
    one is ending limit value, the loop will be repeated as long as the control variable
    is \b less than the ending value. Optional fourth argument is the step that will be
    added to the control variable in each loop turn. By default it is 1.

    As with other constructs, vpp::For() has corresponding block closing instruction called
    vpp::Rof().

    \code
        const Int nr = j >> 1;

        For ( i, 0, nr ); 
        {
            const Int br = i + i;
            const Int er = br + 1;
            const Float vbr = v [ br ];
            const Float ver = v [ er ];

            // Find a root in [vbr, ver]. Initial guess is in v[i].
            v [ i ] = ( vbr + ver )*0.5f;
        
            For ( j, 0, 6 ); 
            {
                // Schroeder's root finding method.
                // v [ i ] is the current parameter value.
                const Float x = v [ i ];
            
                const Float f0 = evaluatePoly0 ( x, k5, k4, k3, k2, k1, k0 );
                const Float f1 = evaluatePoly1 ( x, k5, k4, k3, k2, k1 );
                const Float f2 = evaluatePoly2 ( x, k5, k4, k3, k2 );
                const Float r = f0 / f1;
                const Float d = r + ( f2/(2.0f*f1) )*r*r;

                v [ i ] = x - d;
            }
            Rof();
        }
        Rof();
    \endcode

    \subsubsection ssSwitch 'Switch' conditionals

    vpp::Switch() construct is similar to the C++ one. Just as in C++ you need to use
    vpp::Break() to stop the control flow, otherwise fall-through behavior will occur.

    \code
        const Int c = ...;
        VInt x;

        Switch ( c );
            Case ( 0 );
                x = doSomething0();
                Break();

            Case ( 1 );
                x = doSomething1();
                Break();

            Case ( 2 );
                x = doSomething2();
                Break();

            Default();
                x = doSomethingOther();
                Break();

        EndSwitch();

    \endcode

    \subsubsection ssTypeConv Type conversions

    vpp::StaticCast() and vpp::ReinterpretCast() are explicit type conversion
    operators. 
    
    vpp::StaticCast() converts data while preserving the value (or its approximation),
    just like in C++. You must use this operator somewhat more frequently in VPP
    than in C++, as VPP performs less implicit type conversions.

    Example:

    \code
        const Float f = ...; 
        const Int i = StaticCast< Int >( Ceil ( f ) );

    \endcode

    vpp::ReinterpretCast() converts data while preserving binary representation,
    similar to C++. It is allowed to be used for numeric types only (no pointers).
    One particular application is to manipulate bits in IEEE-754 floats and
    doubles, for some fast approximations.

    \code
        // Will be equal to IEEE-754 binary representation of 1.0
        Int i = ReinterpretCast< Int >( Float ( 1.0f ) );
    \endcode

    \subsubsection ssFunctions GPU-level functions

    Those constructs allow to create functions in GPU code. These functions may be
    called then with various arguments. Function definitions should occur outside
    other constructs, preferably at the beginning of the shader.

    To define a function, use vpp::Function template. The first argument is the
    return type. More optional arguments specify function argument types. Runtime
    string argument is the visible name of the function in SPIR-V dumps (useful
    for debugging).

    Next, comes the vpp::Par declarations, one for each function argument. They
    are needed to access arguments in the function code. You can use these names
    in expressions. Function arguments are read-only. Default or variable number
    arguments are not supported.

    vpp::Function and vpp::Par are class templates.

    Next, between vpp::Begin() and vpp::End(), the function body is located.
    Note that vpp::Begin() and vpp::End() do not create C++ scope, so it is
    recommended to create it yourself by introducing curly braces pair. This
    is optional however, and may be omitted if your function is simple and
    does not create local variables.

    Curly braces may be also moved to higher level, enlosing vpp::Par
    declaration as well, so that they do not pollute the main shader scope.
    This is shown in the second (\c binomial) function in the example below.

    Calling functions is straightforward, syntax is the same as in C++.

    \code
        Function< Int, Int > factorial ( "factorial" );
            Par< Int > factX;
        Begin();
        {
            VInt t = 0;
            VInt r = 1;

            For ( t, 2, factX+1 );
                r *= t;
            Rof();

            Return ( r );
        }
        End();

        Function< Int, Int, Int > binomial ( "binomial" );
        {
            Par< Int > n;
            Par< Int > k;
            Begin();
            {
                Return ( factorial ( n ) / ( factorial ( k )*factorial ( n-k ) ) );
            }
            End();
        }

        // ...

        Int f = factorial ( 5 );
        Int b = binomial ( 5, 2 );

        // ...
    \endcode

*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageReusableFun How to create reusable GPU functions

    \section secReusableFun How to create reusable GPU functions

    We have already shown in \ref pageCode how to create functions in GPU code.
    But those were only local to the shader. What to do if you want a reusable
    function across multiple shaders, or even a function library?

    There are two approaches to this task. The first one is to create a \b macro
    function, while the second one results in a fully reusable GPU-level function.

    \subsection subGpuFnMacro Macro functions

    As we alreaty know, using regular C++ constructs inside GPU code works like
    they were macro constructs. The code is unfolded and inlined inside generated
    SPIR-V. Therefore we can define a function like this:

    \code
        vpp::Int Gcd ( const vpp::Int& _n, const vpp::Int& _k )
        {
            VInt x = _n;
            VInt y = _k;
            VInt s = 0;
            VInt t = 0;

            If ( x < y );
                t = x; x = y; y = t;
            Fi();

            Do(); While ( ( ( x & 1 ) | ( y & 1 ) ) == 0 );
                x >>= 1u; y >>= 1u; ++s;
            Od();

            Do(); While ( x != 0 );
            {
                Do(); While ( ( x & 1 ) == 0 );
                    x >>= 1u;
                Od();

                Do(); While ( ( y & 1 ) == 0 );
                    y >>= 1u;
                Od();

                If ( x >= y );
                    x = ( x - y ) >> 1u;
                Else();
                    y = ( y - x ) >> 1u;
                Fi();
            }
            Od();

            // In macros, use plain C++ 'return'.
            return y << s;
        }

    \endcode

    What will happen if we call such \c Gcd function from GPU code, like this?
    \code
        Int p = Gcd ( a, b );
        // ...
    \endcode

    The entire \c Gcd code will be inlined into the caller's code and the program
    will behave as expected - i.e. as if the \c Gcd function were called.

    This way is very useful for small functions. The \c Gcd function is not a very
    good candidate, though. It defines several mutable variables and if being
    called multiple times, these variables will be declared each time. This produces
    excessive number of mutable variables and can hurt performance (immutable variables
    do not have this problem). C++ optimizer will not be able to help here.
    Code optimizers in drivers theoretically could, but we should not rely on this.
    
    For such complex functions with local mutable variables, a full-fledged GPU function
    is better.

    \subsection subGpuFnReal Real GPU functions

    So we are falling back to vpp::Function() syntax. This time however, we will
    wrap it into a C++ class:

    \code
        class GFunGCD : public vpp::Function< vpp::Int, vpp::Int, vpp::Int >
        {
        public:
            GFunGCD();

        private:
            vpp::Par< vpp::Int > _n;
            vpp::Par< vpp::Int > _k;
        };

        GFunGCD :: GFunGCD() : vpp::Function< vpp::Int, vpp::Int, vpp::Int >( "gcd" )
        {
            using namespace vpp;

            Begin();

                VInt x = _n;
                VInt y = _k;
                VInt s = 0;
                VInt t = 0;

                If ( x < y );
                    t = x; x = y; y = t;
                Fi();

                Do(); While ( ( ( x & 1 ) | ( y & 1 ) ) == 0 );
                    x >>= 1u; y >>= 1u; ++s;
                Od();

                Do(); While ( x != 0 );
                {
                    Do(); While ( ( x & 1 ) == 0 );
                        x >>= 1u;
                    Od();

                    Do(); While ( ( y & 1 ) == 0 );
                        y >>= 1u;
                    Od();

                    If ( x >= y );
                        x = ( x - y ) >> 1u;
                    Else();
                        y = ( y - x ) >> 1u;
                    Fi();
                }
                Od();

                // Note that here is 'Return' here now, not 'return'!
                Return ( y << s );

            End();
        }

    \endcode

    Then, in the client code we use it like this:
    \code
        // Declare it once at the beginning.
        GFunGCD Gcd;

        // ...

        // Use it multiple times later ...

        Int g1 = Gcd ( a1, b1 );
        Int g2 = Gcd ( a2, b2 );
        // ...

    \endcode

    Variables will not be declared multiple times, because they are hidden inside
    SPIR-V level function definition. The definition itself is generated when
    \c GFunGCD object initializes. The calls which occur below, just generate
    a SPIR-V call instruction.

    As you can see, creating reusable GPU funtions is simple - just make
    C++ functors of them, using the syntax presented above.

    In case of small functions that do not create mutable variables, consider
    using macro functions as they have a chance to be faster.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \page pageDebugging Debugging VPP programs

    \section secDebugging Debugging VPP programs

    VPP offers some features helpful for debugging. You can find a short
    overview of them in this section.

    \subsection secDebugProbes Debug probes

    It is still harder to debug shader code than regular C++ code, as we have
    no source level debugger available. One of particular simple methods to
    use in such cases, known even to beginner programmers, is to temporarily
    add logging to the program in the form of \c printf statements. For graphics
    applications however this is impractical, as we are working on millions
    of pixels in parallel. Better way would be to have a <tt>"printf"</tt>
    which generates images. That's exactly what debug probes in VPP do.

    Images generated by debug probes are not displayed anywhere, but there
    is one way that they are intended to be viewed: use rendering debugger
    like RenderDoc. These debug probe images will appear as additional
    images in the pipeline state and you can conveniently examine them.

    Debug probes are extremely useful when you do not get result from your
    shader that you expected and suspect a computation error somewhere.
    They can help e.g. to visualize intermediate results.

    More information on debug probes can be found on the documentation page
    for vpp::Shader and vpp::FragmentShader classes, which implement probe
    functionality as methods.

    \subsection secValidation Vulkan validation support

    VPP provides support for Vulkan validation layer. In order to turn on
    the validation, supply \c Instance::VALIDATION flag to the constructor
    of vpp::Instance object.

    This however does not suffice, as we did not specify where the validation
    warnings should go. vpp::StreamDebugReporter class allows simple redirection
    of these messages into \c std::ostream. To gather the messages, create
    a \c std::ostream (with lifetime as long as your vpp::Instance object)
    and vpp::StreamDebugReporter object. Supply references to the stream and
    instance objects in its constructor. This is sufficient to have validation
    messages routed to specified stream.

    An example:

    \code
        class MyRenderingEngine
        {
            private:
                #ifdef _DEBUG
                    static const unsigned int INSTANCE_FLAGS = vpp::Instance::VALIDATION;
                #else
                    static const unsigned int INSTANCE_FLAGS = 0u;
                #endif

                std::ostringstream m_validationLog;
                vpp::Instance m_instance;
                vpp::StreamDebugReporter m_debugReporter;

                // ...

            public:
                MyRenderingEngine :: MyRenderingEngine() :
                    m_instance ( INSTANCE_FLAGS ),
                    m_debugReporter ( m_validationLog, m_instance ),
                    // ...
                {
                    // ...
                }

                // ...
        };

    \endcode

    It can be also a file stream or any other kind of C++ output stream.

    In case if more advanced error processing is needed, you ca create your own
    subclass of vpp::DebugReporter class and use it instead of vpp::StreamDebugReporter.

    \code
        class MyDebugReporter : public vpp::DebugReporter
        {
        public:
            virtual VkBool32 debugReport (
                VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT objectType,
                uint64_t object,
                size_t location,
                int32_t messageCode,
                const char* pLayerPrefix,
                const char* pMessage );

            virtual void shaderCompilationLog (
                const std::string& shaderCode,
                const char* pShaderType );
        };
    \endcode

    You can override two methods here (both are optional): \c debugReport
    and \c shaderCompilationLog. Mosty likely you will be interested in the
    first one. The second is explained below.

    When overriding \c debugReport, return \c VK_FALSE value, as Vulkan
    specification recommends. Returning \c VK_TRUE will abort your program.

    \subsection secShaderLog Shader compilation log

    Although VPP tranparently handles SPIR-V generation, sometimes it is
    useful to look at the disassembled version of generated SPIR-V code.
    Use this feature e.g. if you suspect a bug in VPP. In order to obtain
    the code dump, you need to do two things:
    
    - On the global level, supply \c SHADERS flag to the constructor of
      vpp::DebugReporter or vpp::StreamDebugReporter. This globally enables
      shader dumps - but will not cause any dumps to be generated yet.

    - In particular shader you want to dump, use special function named
      \c DebugCodeDump(), like in the example below.

    \code
        void MyPipeline :: fVertexShader ( vpp::VertexShader* pShader )
        {
            using namespace vpp;

            // This enables the code dump. Put anywhere in the shader code.
            pShader->DebugCodeDump();

            UniformVar< TFramePar, decltype ( m_framePar ) > inFramePar ( m_framePar );

            const Mat4 m2w = inFramePar [ & GFramePar::m_model2world ];
            const Mat4 w2v = inFramePar [ & GFramePar::m_world2view ];
            const Mat4 v2p = inFramePar [ & GFramePar::m_view2projected ];

            const Vec4 inPos = m_vertices [ & GVertexAttr::m_position ];
            const Vec4 inColor = m_vertices [ & GVertexAttr::m_color ];

            const Vec4 result = v2p * w2v * m2w * inPos; 

            pShader->outVertex.position = result;

            Output< decltype ( m_ioColor ) > outColor ( m_ioColor );
            outColor = inColor;
        }
    \endcode

    The vertex shader shown above will emit SPIR-V code like this:

    \code
        -----------------------------------------------------
        Shader compilation (vertex)
 
        -------------------- Code start ---------------------
        // Module Version 10000
        // Generated by (magic number): 1
        // Id's are bound by 39

                                      Capability Shader
                       1:             ExtInstImport  "GLSL.std.450"
                                      MemoryModel Logical GLSL450
                                      EntryPoint Vertex 4  "main"
                                      Source GLSL 450
                                      Name 4  "main"
                                      Name 9  "struct vppui::TFramePar<2>"
                                      Name 31  "gl_PerVertex"
                                      MemberName 31(gl_PerVertex) 0  "gl_Position"
                                      MemberName 31(gl_PerVertex) 1  "gl_PointSize"
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 0 Offset 0
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 0 ColMajor
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 0 MatrixStride 16
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 1 Offset 64
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 1 ColMajor
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 1 MatrixStride 16
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 2 Offset 128
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 2 ColMajor
                                      MemberDecorate 9(struct vppui::TFramePar<2>) 2 MatrixStride 16
                                      Decorate 9(struct vppui::TFramePar<2>) Block
                                      Decorate 11 DescriptorSet 0
                                      Decorate 11 Binding 0
                                      Decorate 24 Binding 0
                                      Decorate 24 Location 0
                                      Decorate 26 Binding 0
                                      Decorate 26 Location 1
                                      Decorate 31(gl_PerVertex) Block
                                      MemberDecorate 31(gl_PerVertex) 0 BuiltIn Position
                                      MemberDecorate 31(gl_PerVertex) 1 BuiltIn PointSize
                                      Decorate 38 Location 0
                       2:             TypeVoid
                       3:             TypeFunction 2
                       6:             TypeFloat 32
                       7:             TypeVector 6(float) 4
                       8:             TypeMatrix 7(fvec4) 4
        9(struct vppui::TFramePar<2>):             TypeStruct 8 8 8
                      10:             TypePointer Uniform 9(struct vppui::TFramePar<2>)
                      11:     10(ptr) Variable Uniform
                      12:             TypeInt 32 0
                      13:     12(int) Constant 0
                      14:             TypePointer Uniform 8
                      17:     12(int) Constant 1
                      20:     12(int) Constant 2
                      23:             TypePointer Input 7(fvec4)
                      24:     23(ptr) Variable Input
                      26:     23(ptr) Variable Input
        31(gl_PerVertex):             TypeStruct 7(fvec4) 6(float)
                      32:             TypePointer Output 31(gl_PerVertex)
                      33:     32(ptr) Variable Output
                      34:             TypeInt 32 1
                      35:     34(int) Constant 0
                      36:             TypePointer Output 7(fvec4)
                      38:     36(ptr) Variable Output
                 4(main):           2 Function None 3
                       5:             Label
                      15:     14(ptr) AccessChain 11 13
                      16:           8 Load 15
                      18:     14(ptr) AccessChain 11 17
                      19:           8 Load 18
                      21:     14(ptr) AccessChain 11 20
                      22:           8 Load 21
                      25:    7(fvec4) Load 24
                      27:    7(fvec4) Load 26
                      28:           8 MatrixTimesMatrix 22 19
                      29:           8 MatrixTimesMatrix 28 16
                      30:    7(fvec4) MatrixTimesVector 29 25
                      37:     36(ptr) AccessChain 33 35
                                      Store 37 30
                                      Store 38 27
                                      FunctionEnd
        --------------------- Code end ----------------------

    \endcode
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/** \page pageSupportGLM How to use VPP with GLM

    \section secUsageGLM How to use VPP with GLM

    VPP provides easy interoperability mechanism for third party mathematical 
    libraries. You need one of these libraries (or your own one) to represent
    vectors and matrices.

    One of the most popular 3D math libraries is GLM (https://glm.g-truc.net).
    VPP contains built-in support for GLM.

    To use GLM with VPP, you need to do two things:
    - Include \c glm.hpp and other GLM headers first.
    - Include \c vppSupportGLM.hpp header after GLM headers.

    The \c vppSupportGLM.hpp header is not automatically included by
    \c vppAll.hpp.

    Including \c vppSupportGLM.hpp enables usage of GLM vector and matrix types
    as template arguments of vpp::Attribute (with vpp::ext helper) and vpp::UniformFld
    declarations.
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
