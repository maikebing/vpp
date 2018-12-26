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

#ifndef INC_VPPATTACHMENTCONFIG_HPP
#define INC_VPPATTACHMENTCONFIG_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPIMAGEINFO_HPP
#include "vppImageInfo.hpp"
#endif

#ifndef INC_VPPFRAMEIMAGEVIEW_HPP
#include "vppFrameImageView.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class KAttachmentConfig
{
public:
    typedef std::vector< ImageInfo > Infos;
    typedef std::vector< SAttachmentDescription > Descriptions;
    typedef std::vector< FrameImageView > PredefinedViews;

    KAttachmentConfig();

    KAttachmentConfig (
        const Infos& infos,
        const Descriptions& descriptions );
    
    const Infos& getInfos() const;
    const Descriptions& getDescriptions() const;
    const PredefinedViews& getPredefinedViews() const;

    std::uint32_t getDescriptionCount() const;
    const VkAttachmentDescription* getDescriptionArray() const;

    VkAttachmentDescription& getDescription ( size_t index );
    const VkAttachmentDescription& getDescription ( size_t index ) const;

    ImageInfo& getInfo ( size_t index );
    const ImageInfo& getInfo ( size_t index ) const;

    FrameImageView getPredefinedView ( size_t index ) const;

    VkAttachmentDescription& addNewAttachment ( 
        std::uint32_t width,
        std::uint32_t height,
        EImagePurpose imagePurpose,
        std::uint32_t us,
        VkFormat fmt );

    VkAttachmentDescription& addNewAttachment ( 
        const ImageInfo& imageInfo );

    VkAttachmentDescription& addNewAttachment ( 
        const FrameImageView& imageView );

    bool operator< ( const KAttachmentConfig& rhs ) const;
    bool operator== ( const KAttachmentConfig& rhs ) const;
    bool operator!= ( const KAttachmentConfig& rhs ) const;

private:
    Infos d_infos;
    Descriptions d_descriptions;
    PredefinedViews d_predefinedViews;

private:
    KAttachmentConfig ( const KAttachmentConfig& ) = delete;
    const KAttachmentConfig& operator= ( const KAttachmentConfig& ) = delete;
};

// -----------------------------------------------------------------------------

VPP_INLINE KAttachmentConfig :: KAttachmentConfig()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE KAttachmentConfig :: KAttachmentConfig (
    const Infos& infos,
    const Descriptions& descriptions ) :
        d_infos ( infos ),
        d_descriptions ( descriptions )
{}

// -----------------------------------------------------------------------------

VPP_INLINE const KAttachmentConfig::Infos& KAttachmentConfig :: getInfos() const
{
    return d_infos;
}

// -----------------------------------------------------------------------------

VPP_INLINE const KAttachmentConfig::Descriptions& KAttachmentConfig :: getDescriptions() const
{
    return d_descriptions;
}

// -----------------------------------------------------------------------------

VPP_INLINE const KAttachmentConfig::PredefinedViews& KAttachmentConfig :: getPredefinedViews() const
{
    return d_predefinedViews;
}

// -----------------------------------------------------------------------------

VPP_INLINE std::uint32_t KAttachmentConfig :: getDescriptionCount() const
{
    return static_cast< std::uint32_t >( d_descriptions.size() );
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkAttachmentDescription* KAttachmentConfig :: getDescriptionArray() const
{
    return & d_descriptions [ 0 ];
}

// -----------------------------------------------------------------------------

VPP_INLINE VkAttachmentDescription& KAttachmentConfig :: getDescription ( size_t index )
{
    return d_descriptions [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const VkAttachmentDescription& KAttachmentConfig :: getDescription ( size_t index ) const
{
    return d_descriptions [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE ImageInfo& KAttachmentConfig :: getInfo ( size_t index )
{
    return d_infos [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE const ImageInfo& KAttachmentConfig :: getInfo ( size_t index ) const
{
    return d_infos [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE FrameImageView KAttachmentConfig :: getPredefinedView ( size_t index ) const
{
    return d_predefinedViews [ index ];
}

// -----------------------------------------------------------------------------

VPP_INLINE VkAttachmentDescription& KAttachmentConfig :: addNewAttachment (
    std::uint32_t width,
    std::uint32_t height,
    EImagePurpose imagePurpose,
    std::uint32_t us,
    VkFormat fmt )
{
    d_descriptions.push_back ( SAttachmentDescription() );
    d_infos.emplace_back ( width, height, imagePurpose, us, fmt );
    d_predefinedViews.push_back ( FrameImageView() );

    VkAttachmentDescription& newDesc = d_descriptions.back();
    std::memset ( & newDesc, 0, sizeof ( VkAttachmentDescription ) );
    newDesc.format = fmt;

    return newDesc;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkAttachmentDescription& KAttachmentConfig :: addNewAttachment (
    const ImageInfo& imageInfo )
{
    d_descriptions.push_back ( SAttachmentDescription() );
    d_infos.emplace_back ( imageInfo );
    d_predefinedViews.push_back ( FrameImageView() );

    VkAttachmentDescription& newDesc = d_descriptions.back();
    std::memset ( & newDesc, 0, sizeof ( VkAttachmentDescription ) );
    newDesc.format = imageInfo.format;

    return newDesc;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkAttachmentDescription& KAttachmentConfig :: addNewAttachment ( 
    const FrameImageView& imageView )
{
    VkAttachmentDescription& newDesc = addNewAttachment ( imageView.image().info() );
    d_predefinedViews.back() = imageView;
    return newDesc;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KAttachmentConfig :: operator< ( const KAttachmentConfig& rhs ) const
{
    if ( d_infos != rhs.d_infos )
        return d_infos < rhs.d_infos;
    else
        return d_descriptions < rhs.d_descriptions;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KAttachmentConfig :: operator== ( const KAttachmentConfig& rhs ) const
{
    return d_infos == rhs.d_infos && d_descriptions == rhs.d_descriptions;
}

// -----------------------------------------------------------------------------

VPP_INLINE bool KAttachmentConfig :: operator!= ( const KAttachmentConfig& rhs ) const
{
    return ! operator== ( rhs );
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPATTACHMENTCONFIG_HPP
