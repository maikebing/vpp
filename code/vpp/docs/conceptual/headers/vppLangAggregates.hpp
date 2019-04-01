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
    \brief Enumeration type allowing to select the flavor of shader structural
    type: the CPU or GPU version.
    
    Use as a template argument to instantiate wanted version. This applies to
    subclass templates of the following: VertexStruct, InstanceStruct, UniformStruct,
    LocalStruct. Also the value of ETag type is passed to members of these
    structures: Attribute, UniformFld, Field.
*/

enum ETag
{
    /** \brief Selects CPU version of the structure. */
    CPU,

    /** \brief Selects GPU version of the structure. */
    GPU,

    /** \brief Internal use only. */
    DEF
};

// -----------------------------------------------------------------------------
/**
    \brief Base class for local data structures.

    Inherit from this class to define your GPU local data structure.

    Local data structures reside on GPU only and therefore are not associated
    with any CPU-side types. Objects of these types are never seen by CPU.
    As a result, type definition is simpler than in case of VertexStruct,
    InstanceStruct or UniformStruct.

    The template argument should be the name of your template which is being
    derived.

    Use Field template to define fields inside the structure.

    Structures defined with LocalStruct are typically used with ioStructure
    binding points, to define inter-shader communication variables.

    There is only GPU-side version of the structure, hence ETag is not used.

    Example of type definition:

    \code
        struct GStdPipelineData : public LocalStruct< GStdPipelineData >
        {
            Field< Vec4 > d_worldVertex;
            Field< Vec4 > d_worldNormal;
            Field< Vec4 > d_worldTangent;

            Field< Vec4 > d_textureUV01;
            Field< Vec4 > d_textureUV23;
            Field< Vec4 > d_textureUV45;
            Field< Vec4 > d_textureUV67;

            Field< IVec4 > d_parameters;
        };
    \endcode
*/

template< class DefinitionT >
class LocalStruct
{
};

// -----------------------------------------------------------------------------
/**
    \brief Template for defining local structure data fields.

    Place Field inside LocalStruct to define a data field.

    This field can have any scalar, vector or matrix shader type. Specify
    the type as the first template argument.

    The second optional argument is bitwise OR of the following flags:

    - IN_FLAT: no interpolation will be done.
    - IN_NOPERSPECTIVE: linear, non-perspective correct, interpolation will be used.
    - IN_CENTROID: when used with multi-sampling rasterization, allows a single interpolation location for an entire pixel.
    - IN_SAMPLE: when used with multi-sampling rasterization, requires per-sample interpolation.
    - VAR_RELAXEDPR: relaxed precision will be used.

    The flags allow to set interpolation mode for the fragment shader,
    per structure member. By default they are equal to zero, which means default
    precision and interpolation mode.
*/

template< class FieldT, std::uint32_t FLAGS = 0 >
class Field
{
public:
};

// -----------------------------------------------------------------------------
/**
    \brief Declares mutable shader local variable of array type.

    This allows to create local arrays of specified type (scalar, vector or
    matrix) and the size determined at shader compilation time.

    Beware that mutable local variables may degrade performance on GPU because they
    consume general purpose registers for each local thread. Arrays obviously
    take even more registers.
*/

template< class ItemT >
class VArray 
{
public:
    /** \brief Constucts local array of specified size.
    
        The size may come from CPU-level variable. It can not be changed after constructing
        the array.

        The lifetime of constructed array is until the end of the shader. Consider
        reusing the array if possible in order to avoid excessive register usage.
    */

    VArray ( int s );

    /**
        \brief Allows read/write access to elements of the array.
    */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;

    /**
        \brief Returns GPU-level value equal to the size of the array.
    */
    Int Size() const;

    /**
        \brief Returns CPU-level value equal to the size of the array.
    */
    int size() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Declares workgroup-scoped variable of array type.

    This allows to create arrays of specified type (scalar, vector or
    matrix) and the size determined at shader compilation time, shared between
    threads within single workgroup.

    This enables the following possibilities:
    - Fast inter-thread communication within single workgroup.
    - Group oriented paradigm of parallel programming. This involves solving single
      problem on single workgroup (compute unit). Multiple compute units are allocated
      to different problem instances, hence they do not require communication. Also
      single workgroup can execute some generally sequential algorithm, but with
      parallelizable subroutines (like sorting, searching or reducing). Shared arrays
      are natural way to construct working space for such algorithms. VPP facilitates
      such scenarios by providing group-scoped algorithm library in vpp::ct::group namespace.
    - Flexible memory allocation for individual threads.

    Workgroups are called also thread groups in some proprietary APIs. Also usually a single
    workgroup maps to so-called \b warp (32 threads) or \b wavefront (64 threads)
    on the GPU - depending on particular GPU vendor. It is not necessary though, as the
    workgroup size is configurable.

    Shared arrays consume space inside the shared memory block. On current devices, it
    is typically 32 to 48 kB. No general purpose registers are allocated. This type of array
    usually does not impose performance penalties, as long as all arrays fit inside the block.

    Use ComputeShader::getTotalWorkgroupMemory() and ComputeShader::getFreeWorkgroupMemory()
    functions to determine the total and still available sizes of the shared memory block.
    Exceeding the size with too many allocations will result in exception thrown by VPP
    during shader compilation. It is recommended to check these sizes before allocating
    large arrays. Various devices may provide different sizes. At the moment of writing this,
    the rule of thumb is as follows:
    - NVIDIA GPUs: 48 kB,
    - Radeon GPUs: 32 kB,
    - some mobile GPUs: 16 kB (this is the minimum required by Vulkan standard).

    Caution: one type of performance penalty that might occur with shared memory is the RAW
    latency. If you write to some location and immediately read from the same location,
    delay might occur which can impose quite large slowdown. Try to avoid such kind of
    accesses.
*/

template< class ItemT >
class WArray 
{
public:
    /** \brief Constucts workgroup-scoped array of specified size.
    
        The size may come from CPU-level variable. It can not be changed after constructing
        the array.

        The lifetime of constructed array is until the end of the shader. Consider
        reusing the array if possible in order to avoid overflow of the shared memory block.
    */

    WArray ( int s );

    /**
        \brief Allows read/write access to elements of the array.
    */
    template< typename IndexT >
    auto operator[]( IndexT index ) const;

    /**
        \brief Returns GPU-level value equal to the size of the array.
    */
    Int Size() const;

    /**
        \brief Returns CPU-level value equal to the size of the array.
    */
    int size() const;
};

// -----------------------------------------------------------------------------
/**
    \brief Declares workgroup-scoped variable of two-dimensional array type.

    This is the same as WArray, but with two dimensions instead of one.

    Can be used to represent a matrix of moderate size. Total size of the
    array should fit in shared memory block, which typically treanslates to
    ca. 128x128 matrix or slightly less.
    
    Arrangement of the matrix is row-major.
*/

template< class ItemT >
class WArray2
{
public:
    /** \brief Constructs a shared 2-dimensional array with specified number of rows and columns.
    
        The sizes may come from CPU-level variable. They can not be changed after constructing
        the array.

        The lifetime of constructed array is until the end of the shader. Consider
        reusing the array if possible in order to avoid overflow of the shared memory block.
    */

    WArray2 ( int nRows, int nColumns );

    /**
        \brief Allows read/write access to individual rows of the array.

        The rows are presented as WArray objects, allowing to use group algorithms
        on them.
    */
    template< typename IndexT >
    WArray< ItemT > operator[]( IndexT index ) const;

    /**
        \brief Allows read/write access to elements of the array.
    */
    template< typename IndexT >
    auto operator()( IndexT iRow, IndexT iCol ) const;

    /**
        \brief Returns GPU-level value equal to number of rows in the array.
    */
    Int Size() const;
    
    /**
        \brief Returns CPU-level value equal to number of rows in the array.
    */
    int size() const;

    /**
        \brief Returns GPU-level value equal to number of rows in the array.
    */
    Int Rows() const;

    /**
        \brief Returns CPU-level value equal to number of rows in the array.
    */
    int rows() const;

    /**
        \brief Returns GPU-level value equal to number of columns in the array.
    */
    Int Cols() const;

    /**
        \brief Returns CPU-level value equal to number of columns in the array.
    */
    int cols() const;
};

// -----------------------------------------------------------------------------

template< class ItemT >
class WArray3
{
public:
    /** \brief Constructs a shared 3-dimensional array with specified number of layers, rows and columns.
    
        The sizes may come from CPU-level variable. They can not be changed after constructing
        the array.

        The lifetime of constructed array is until the end of the shader. Consider
        reusing the array if possible in order to avoid overflow of the shared memory block.
    */
    WArray3 ( int nl, int nr, int nc );

    /**
        \brief Allows read/write access to individual 2-dimensional layers of the array.

        The rows are presented as WArray2 objects, allowing to use group algorithms
        on them.
    */
    template< typename IndexT >
    WArray2< ItemT > operator[]( const IndexT& index ) const;

    /**
        \brief Allows read/write access to elements of the array.
    */
    template< typename IndexT >
    auto operator()( const IndexT& iLayer, const IndexT& iRow, const IndexT& iCol ) const;

    /**
        \brief Returns GPU-level value equal to number of layers in the array.
    */
    Int Size() const;

    /**
        \brief Returns CPU-level value equal to number of layers in the array.
    */
    int size() const;

    /**
        \brief Returns GPU-level value equal to number of layers in the array.
    */
    Int Layers() const;

    /**
        \brief Returns CPU-level value equal to number of layers in the array.
    */
    int layers() const;

    /**
        \brief Returns GPU-level value equal to number of rows in the array.
    */
    Int Rows() const;

    /**
        \brief Returns CPU-level value equal to number of rows in the array.
    */
    int rows() const;

    /**
        \brief Returns GPU-level value equal to number of columns in the array.
    */
    Int Cols() const;

    /**
        \brief Returns CPU-level value equal to number of columns in the array.
    */
    int cols() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
