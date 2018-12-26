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
    \brief Untyped image view class. Useful for constructing FrameBuffer objects.

    Sometimes it is useful to construct an image view without strictly defined
    type at compile time, for example if we have only a generic Img image reference.
    
    FrameImageView provides such possibility. It is not a template, just plain
    class that can be constructed from Img object or SImageInfo structure.

    This kind of image view can't be bound to rendering or computation pipelines,
    however it can be used to construct frame buffers and specify attachment targets.

    This object is reference-counted and may be passed by value.
*/

class FrameImageView
{
public:
    /** \brief Constructs null reference. */
    FrameImageView();
    
    /** \brief Constructs an image view for single layer of specified image.
    
        The image is given as Img object. Also optionally provide the
        layer index. For arrayed images it will select single layer.
        This is useful in most cases of usage in output attachments,
        as these attachments are typically assumed to be single layered
        (although there may be exceptions).
    */
    explicit FrameImageView ( const Img& hImage, std::uint32_t layer = 0 );

    /** \brief Constructs an image view for multiple layers of specified image.
    
        The image is given as Img object. Also provide the resource
        range, defining layers, MIP levels and aspects covered by this view.
    */
    FrameImageView ( const Img& hImage, const VkImageSubresourceRange& sr );

    /** \brief Constructs both image view and image at once.

        This constructor creates an image from given SImageInfo structure
        and image view for it.
    */
    FrameImageView ( const SImageInfo& imageInfo, const Device& hDevice );

    /**
        \brief Constructs an image view for single layer of specified image.
        
        A convenience wrapper for the single layer constructor, accepting
        a memory binding object containing the image.
    */
    template< class MemoryT, class ResourceT >
    FrameImageView (
        const MemoryBinding< ResourceT, MemoryT >& binding,
        std::uint32_t layer = 0 );

    /** \brief Retrieves Vulkan handle of the view. */
    operator VkImageView() const;

    /** \brief Retrieves the image. */
    const Img& image() const;

    /** \brief Retrieves the device. */
    const Device& device() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
