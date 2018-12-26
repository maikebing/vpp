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
    \brief A platform-dependent structure containing information required
           to create a surface.
*/

struct SurfaceInfo
{
    #ifdef VK_USE_PLATFORM_WIN32_KHR
        HINSTANCE hInstance;
        HWND hWnd;
    #endif
};

// -----------------------------------------------------------------------------
/**
    \brief Represents a surface visible on the screen.

    A Surface is generalization of physical screen or window. This is abstract
    concept, portable and isolated from window system details.

    In most applications you want to display some image on the screen. Creating
    a Surface object is the first step. Next, you need to create a SwapChain
    object from Surface and Device objects. Now you can obtain a set of image
    objects which can act as rendering targets.

    Also you can query the Surface object for various parameters and capabilities.
*/

class Surface
{
public:
    /** \brief Constructs null reference. */
    Surface();

    /** \brief Constructs Surface object. */
    Surface (
        const SurfaceInfo& surfaceInfo,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    /** \brief Constructs Surface object from existng Vulkan surface handle. */
    Surface (
        VkSurfaceKHR hSurface,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    /** \brief Retrieves the Vulkan handle. */
    VkSurfaceKHR handle() const;

    /** \brief Retrieves surface capabilities. */
    bool getCapabilities ( VkSurfaceCapabilitiesKHR* pResult ) const;

    /** \brief Retrieves a list of formats supported by the surface. */
    bool getFormats ( SurfaceFormats* pResult ) const;

    /** \brief Retrieves a list of queuing modes supported by the surface. */
    bool getPresentationModes ( PresentationModes* pResult ) const;

    /** \brief Retrieves the surface size in pixels. */
    VkExtent2D getSize() const;

    /** \brief Retrieves the default format and color space. */
    VkFormat getDefaultFormat ( VkColorSpaceKHR* pColorSpace = 0 ) const;

    /** \brief Retrieves various surface parameters in text format for diagnostic logging. */
    void getParametersAsText ( std::ostream& sst ) const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
