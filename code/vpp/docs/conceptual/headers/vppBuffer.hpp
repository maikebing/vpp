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
    \brief Generic class representing Vulkan buffers.

    This object is reference-counted and may be passed by value.

    It is not recommended to create these objects directly. Use Buffer instead.

    This class serves mainly as a base class for the Buffer template. It contains
    some useful methods that are inherited. Also many VPP functions accept Buf
    as an argument, so you can provide any Buffer template instance.
*/

class Buf
{
public:
    /**
        \brief %Buffer usage flags. Can be bitwise-combined to create multipurpose
        buffers.
    */
    enum EUsageFlags
    {
        SOURCE = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,          /**< \brief Source buffer for transfer operations. */
        TARGET = VK_BUFFER_USAGE_TRANSFER_DST_BIT,          /**< \brief Target buffer for transfer operations. */
        UNITEX = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,  /**< \brief Read-only formatted data (uniform texel buffer). */
        STORTEX = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, /**< \brief Read & write formatted data (storage texel buffer). */
        UNIFORM = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,       /**< \brief Read-only common data (uniform buffer). */
        STORAGE = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,       /**< \brief Read & write array (storage buffer). */
        INDEX = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,           /**< \brief Vertex index array for indexed primitives. */
        VERTEX = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,         /**< \brief Vertex attributes array. */
        INDIRECT = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT      /**< \brief %Buffer holding indirect draw ranges. */
    };

    enum ECreationFlags
    {
        SPARSE_BINDING = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
        SPARSE_RESIDENCY = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
        SPARSE_ALIASED = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT
    };

    /** \brief Constructs null reference. */
    Buf();

    /** \brief Constructs a buffer of given size and usage (bitwise combination of
        EUsageFlags values), associated with given device.
    */
    Buf (
        VkDeviceSize bufferSize,
        unsigned int usageMask,
        const Device& hDevice,
        unsigned int flags = 0 );

    /** \brief
        Constructs a buffer of given size and usage (bitwise combination of
        EUsageFlags values), associated with given device. This constructor
        also takes a list of queue families allowed to access the buffer
        in concurrent mode.
    */
    Buf (
        VkDeviceSize bufferSize,
        unsigned int usageMask,
        const Device& hDevice,
        const std::vector< unsigned int >& queueFamilyIndices,
        unsigned int flags = 0 );

    /** \brief Checks whether this is not a null reference. */
    operator bool() const;

    /** \brief Retrieves Vulkan handle of the buffer. */
    VkBuffer handle() const;

    /** \brief Retrieves the device associated with the buffer. */
    const Device& device() const;

    /** \brief Retrieves the size of the buffer. */
    VkDeviceSize size() const;

    /** \brief Retrieves usage flags of the buffer. */
    unsigned int getUsage() const;

    /** \brief Allocates and binds memory for the buffer. */
    template< class MemoryT >
    MemoryT bindMemory ( const MemProfile& memProfile ) const;
};

// -----------------------------------------------------------------------------

/**
    \brief Typed Vulkan buffer of specified purpose. Most functions operating
    on buffers require typed buffer, therefore this is the class you want to
    use most of the time. 
    
    Also note that VPP provides a container class \ref gvector,
    which is derived from Buffer and offers STL-like interface.

    In case of using Buffer class directly, you need to specify appropriate usage 
    flags as the template parameter. It is a bitwise \c or of enumeration values
    inherited from Buf base class: Buf::SOURCE, Buf::TARGET, Buf::UNITEX,
    Buf::STORTEX, Buf::UNIFORM, Buf::STORAGE, Buf::INDEX, Buf::VERTEX, Buf::INDIRECT.
    
    Buffers defined with Buf::INDEX, Buf::VERTEX and Buf::INDIRECT flags can
    serve as geometric data source for the rendering pipeline. This is the primary
    method for delivering the geometry.
    
    Uniform buffers (Buf::UNIFORM) can provide auxiliary read-only data for rendering,
    e.g. transformation matrices.
    
    Storage buffers (Buf::STORAGE) can be read and written in shaders, so they are
    a good way to define some computation workspace, exchange large block of data
    between shaders, or read back computation results to the CPU. 

    Uniform texel buffers (Buf::UNITEX, Buf::STORTEX) are special type of buffers which
    share some semantics with images, but have less restrictions regarding the size.

    Transfer source buffers (Buf::SOURCE) can be used to transfer data on GPU
    level to other buffers or images. Their specific application is to participate
    in image or texture loading from the CPU memory to GPU. First, read the
    image binary data into the buffer on the CPU side, then execute cmdCopyBufferToImage()
    command.

    Transfer target buffers (Buf::TARGET) can serve as data receivers. One possible
    application is reading rendered images back to CPU memory. This is exactly opposite
    process to the routine described above.

    When the purpose of the buffer is determined, it is convenient to make
    a \c typedef like this:

    \code
        typedef vpp::Buffer< Buf::SOURCE | Buf::STORAGE > MyBuffer;
    \endcode

    Create the buffer using the constructor, which accepts size and device.
    The buffer object is reference-counted and may be passed by value.

    Newly created buffer does not have any memory allocated yet. In order to allocate
    memory, you can choose one of the following ways:
    - Call bindDeviceMemory() function. It takes the buffer and MemProfile
      value specifying where to allocate the memory. It allocates and binds the
      memory and returns a MemoryBinding object. From that object, you can retrieve
      both the buffer and newly created DeviceMemory object. The buffer can now
      be used in contexts requiring a buffer bound to memory (e.g. transfers).
    - Call bindMappableMemory() function. It is the same as bindDeviceMemory(),
      but the MemoryBinding object now returns a MappableDeviceMemory object.
      This object has additional functionality - the memory can be mappend and
      accessed directly on CPU side.
    - Call bindMemory() function (inherited from Buf) directly on Buffer object.
      As the template argument, specify either DeviceMemory or MappableDeviceMemory.
      As the function argument, provide MemProfile value. This function
      returns a DeviceMemory or MappableDeviceMemory object. Apart from syntax,
      the operation is the same as above functions.

    See the docs of DeviceMemory, MappableDeviceMemory and MemProfile
    classes for more information on memory allocation.
*/

template< unsigned int USAGE >
class Buffer : public Buf
{
public:
    static const unsigned int usage = USAGE;

    /** \brief Constructs null reference. */
    Buffer();

    /** \brief Constructs a buffer of given size and usage (bitwise combination of
        EUsageFlags values), associated with given device.
    */
    Buffer (
        VkDeviceSize size,
        const Device& hDevice,
        unsigned int flags = 0,
        unsigned int addUsage = 0 );

    /** \brief
        Constructs a buffer of given size and usage (bitwise combination of
        EUsageFlags values), associated with given device. This constructor
        also takes a list of queue families allowed to access the buffer
        in concurrent mode.
    */
    Buffer (
        VkDeviceSize size,
        const Device& hDevice,
        const std::vector< unsigned int >& queueFamilyIndices,
        unsigned int flags = 0,
        unsigned int addUsage = 0 );

    /**
        \brief Construct new reference to existing buffer, with compatible usage.
    */
    template< unsigned int USAGE2 >
    Buffer ( const Buffer< USAGE2 >& other );
};

// -----------------------------------------------------------------------------

/** \brief Predefined class for index buffers, used in indexed draws. */
typedef Buffer< Buf::INDEX > IndexBuffer;

/** \brief Predefined class for vertex buffers. */
typedef Buffer< Buf::VERTEX > VertexBuffer;

/** \brief Predefined class for indirect buffers, used in indirect draws. */
typedef Buffer< Buf::INDIRECT > IndirectBuffer;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
