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
    \brief Declares mutable shader variable of array type.

*/

template< class ItemT, int SIZE >
class VArray 
{
public:
    VArray();

    template< typename IndexT >
    inline auto operator[]( IndexT index ) const;

    inline Int Size() const;
    inline int size() const;
};

// -----------------------------------------------------------------------------

/**
    \brief Convenience template for 2-dimensional local shader arrays.
*/

template< class ItemT, int COLS, int ROWS >
using VArray2 = VArray< VArray< ItemT, ROWS >, COLS >;

/**
    \brief Convenience template for 3-dimensional local shader arrays.
*/
template< class ItemT, int LAYERS, int COLS, int ROWS >
using VArray3 = VArray< VArray< VArray< ItemT, ROWS >, COLS >, LAYERS >;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
