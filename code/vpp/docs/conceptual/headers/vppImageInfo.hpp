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
    \brief Enumeration specifying general purpose of an image.

    This is more general distinction than Img::EUsageFlags.
*/
enum EImagePurpose
{
    RENDER,       /**< \brief All images used for normal rendering. */
    DISPLAY,      /**< \brief Tags an attachment to be associated with a swapchain image view. */
    SWAPCHAIN,    /**< \brief Special image created as a part of the swapchain. */
    RENDER_DEBUG  /**< \brief Internally created image for debugging purposes. */
};

// -----------------------------------------------------------------------------

enum EImageType
{
    IMG_TYPE_1D,
    IMG_TYPE_2D,
    IMG_TYPE_3D,
    IMG_TYPE_CUBE_2D,
    IMG_TYPE_RECT_2D
};

// -----------------------------------------------------------------------------
/**
    \brief A structure containing image parameters.

    This is a run-time structure containing a complete set of image parameters.

    It can be used both to define images (untyped) and obtain information
    about existing image.
*/

struct ImageInfo
{
    /**
        \brief Constructs an image info with all parameters.

        Example:

        \code
            vpp::ImageInfo myImgInfo (
                vpp::RENDER,
                vpp::IMG_TYPE_2D,
                VK_FORMAT_R8G8B8A8_UINT,
                { 1024, 768, 1 },
                1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
                vpp::Img::SAMPLED | vpp::Img::STORAGE, 0
            );
        \endcode
    */

    ImageInfo (
        EImagePurpose _purpose,
        EImageType _type,
        VkFormat _format,
        const VkExtent3D& _extent,
        unsigned int _mipLevels,
        unsigned int _arrayLayers,
        VkSampleCountFlagBits _multisampling,
        VkImageTiling _tiling,
        unsigned int _usage,
        unsigned int _flags );
    
    /**
        \brief Constructs an image info with most important parameters.

        Example:

        \code
            vpp::ImageInfo myImgInfo (
                640, 480,
                vpp::RENDER,
                vpp::Img::SAMPLED | vpp::Img::STORAGE,
                VK_FORMAT_R8G8B8A8_UINT
            );
        \endcode
    */
    ImageInfo (
        unsigned int width,
        unsigned int height,
        EImagePurpose _purpose,
        unsigned int usage,
        VkFormat fmt );
    
    /** \brief Retrieves default aspect mask guessed from format. */
    std::uint32_t getAspect() const;

    /** \brief Retrieves Vulkan view type from image type. */
    VkImageViewType getViewType() const;

    /** \brief Retrieves Vulkan image type. */
    VkImageType getImageType() const;

    /** \brief Retrieves the number of image dimensions. */
    std::uint32_t getDimensions() const;

    /** \brief Comparison operator for maps. */
    bool operator< ( const ImageInfo& rhs ) const;

    /** \brief Comparison operator for equality. */
    bool operator== ( const ImageInfo& rhs ) const;

    /** \brief Comparison operator for inequality. */
    bool operator!= ( const ImageInfo& rhs ) const;

    /** \brief Overall purpose of an image. Usually RENDER or SWAPCHAIN. */
    EImagePurpose purpose;

    /** \brief Type of image. Determins the number of dimensions. */
    EImageType type;

    /** \brief %Image format. A value of Vulkan format enumeration. */
    VkFormat format;

    /** \brief %Image size. */
    VkExtent3D extent;

    /** \brief Number of levels in a mip-mapped image. */
    unsigned int mipLevels;

    /** \brief Number of layers in an arrayed image. */
    unsigned int arrayLayers;

    /** \brief Number of samples in a multisampled image. */
    VkSampleCountFlagBits multisampling;

    /** \brief %Image tiling - usually VK_IMAGE_TILING_OPTIMAL. */
    VkImageTiling tiling;

    /** \brief Usage flags - bitwise or of EUsageFlags values. */
    unsigned int usage;

    /** \brief Additional flags - by default 0. */
    unsigned int flags;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
