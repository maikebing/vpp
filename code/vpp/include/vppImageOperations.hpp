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

#ifndef INC_VPPIMAGEOPERATIONS_HPP
#define INC_VPPIMAGEOPERATIONS_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMPILEDPROCEDURES_HPP
#include "vppCompiledProcedures.hpp"
#endif

#ifndef INC_VPPBUFFER_HPP
#include "vppBuffer.hpp"
#endif

#ifndef INC_VPPIMAGE_HPP
#include "vppImage.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class FillImageSolid : public CompiledProcedures
{
public:
    VPP_INLINE FillImageSolid (
        const Img& targetImage,
        const VkClearColorValue& color,
        VkImageLayout finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ) :
            CompiledProcedures ( targetImage.device(), Q_GRAPHICS ),
            d_targetImage ( targetImage ),
            d_color ( color ),
            d_finalLayout ( finalLayout )
    {
        init();
    }

    Procedure execute;

private:
    void init();

private:
    Img d_targetImage;
    VkClearColorValue d_color;
    VkImageLayout d_finalLayout;
};

// -----------------------------------------------------------------------------

class CopyImageToDevice : public CompiledProcedures
{
public:
    VPP_INLINE CopyImageToDevice (
        const Buf& sourceBuffer,
        const Img& targetImage,
        const std::vector< VkBufferImageCopy >& regions,
        VkImageLayout finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ) :
            CompiledProcedures ( sourceBuffer.device(), Q_GRAPHICS ),
            d_sourceBuffer ( sourceBuffer ),
            d_targetImage ( targetImage ),
            d_regions ( regions ),
            d_finalLayout ( finalLayout )
    {
        init();
    }

    Procedure execute;

private:
    void init();

private:
    Buf d_sourceBuffer;
    Img d_targetImage;
    std::vector< VkBufferImageCopy > d_regions;
    VkImageLayout d_finalLayout;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ImageT >
class TImageLoader
{
public:
    TImageLoader ( const Device& hDevice );
    ~TImageLoader();

    void load();
    void wait() const;

    ImageT result() const;

    virtual VkExtent3D getImageExtent() = 0;
    virtual std::uint32_t getImageMipLevels() = 0;
    virtual VkDeviceSize getImageDataSize() = 0;
    virtual VkFormat getImageFormat() = 0;
    virtual void loadImageData ( unsigned char* pBegin, unsigned char* pEnd ) = 0;
    virtual void defineLevels ( std::vector< VkBufferImageCopy >* pRegions ) = 0;

private:
    Device d_device;
    Fence d_loadFinished;
    MemoryBinding< Buf, MappableDeviceMemory > d_sourceMemory;
    MemoryBinding< ImageT, DeviceMemory > d_targetMemory;
    ImageT d_image;
};

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE TImageLoader< ImageT > :: TImageLoader ( const Device& hDevice ) :
    d_device ( hDevice ),
    d_loadFinished ( hDevice, true )
{
}

// -----------------------------------------------------------------------------

template< class ImageT >
void TImageLoader< ImageT > :: load()
{
    typedef typename ImageT::attributes_type attributes_type;

    static_assert (
        attributes_type::isUsageTransferDst,
        "Loaded image must have TRANSFER_DST usage bit set" );

    d_loadFinished.reset();

    const VkDeviceSize imageDataSize = getImageDataSize();

    Buf sourceBuffer ( imageDataSize, Buf::SOURCE, d_device );

    d_sourceMemory = MemoryBinding< Buf, MappableDeviceMemory > (
        sourceBuffer, MemProfile::HOST_STATIC );

    auto& memory = d_sourceMemory.memory();

    if ( memory.map() == VK_SUCCESS )
    {
        loadImageData ( memory.beginMapped(), memory.endMapped() );
        memory.unmap();

        d_image = ImageT ( getImageFormat(), getImageExtent(), d_device, getImageMipLevels() );

        d_targetMemory = MemoryBinding< ImageT, DeviceMemory > (
            d_image, MemProfile::DEVICE_STATIC );

        std::vector< VkBufferImageCopy > regions;
        defineLevels ( & regions );

        // FIXME: zapodajemy Queue::TRANSFER ale CommandPool uzywany wewnatrz
        // CopyImageToDevice jest polaczony z Queue::GRAPHICS i Vulkan sie
        // o to pluje

        CopyImageToDevice imageDataCopier ( d_sourceMemory, d_targetMemory, regions );
        imageDataCopier.execute ( Q_GRAPHICS, d_loadFinished );
    }
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE void TImageLoader< ImageT > :: wait() const
{
    d_loadFinished.wait();
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE ImageT TImageLoader< ImageT > :: result() const
{
    wait();
    return d_image;
}

// -----------------------------------------------------------------------------

template< class ImageT >
VPP_INLINE TImageLoader< ImageT > :: ~TImageLoader()
{
    wait();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct SLayoutInitializer
{
    SLayoutInitializer();
    SLayoutInitializer ( const Img& img );
    SLayoutInitializer ( const Img& img, VkImageLayout layout );
    
    Img image;
    VkImageLayout oldImageLayout; 
    VkImageLayout newImageLayout;
    std::uint32_t levelCount;
    std::uint32_t layerCount;
    std::uint32_t baseMipLevel;
    std::uint32_t baseArrayLayer;
    VkImageAspectFlags aspectMask;

private:
    static VkImageLayout usageToLayout ( unsigned int usage );
};

// -----------------------------------------------------------------------------

VPP_INLINE SLayoutInitializer :: SLayoutInitializer()
{
}

// -----------------------------------------------------------------------------

typedef std::vector< SLayoutInitializer > LayoutInitializers;

// -----------------------------------------------------------------------------

class KInitializeLayouts : public CompiledProcedures
{
public:
    VPP_INLINE KInitializeLayouts (
        const LayoutInitializers& initializers,
        Device hDevice ) :
            CompiledProcedures ( hDevice, Q_GRAPHICS ),
            d_initializers ( initializers ),
            d_device ( hDevice )
    {
        init();
    }

    Procedure execute;

private:
    void init();

private:
    const LayoutInitializers& d_initializers;
    Device d_device;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPIMAGEOPERATIONS_HPP
