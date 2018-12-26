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

#ifndef INC_VPPSURFACE_HPP
#define INC_VPPSURFACE_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPINSTANCE_HPP
#include "vppInstance.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

struct SurfaceInfo
{
    #ifdef VK_USE_PLATFORM_WIN32_KHR
        HINSTANCE hInstance;
        HWND hWnd;
    #endif
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class SurfaceImpl;

// -----------------------------------------------------------------------------

class Surface : public TSharedReference< SurfaceImpl >
{
public:
    Surface();

    Surface (
        const SurfaceInfo& surfaceInfo,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    Surface (
        VkSurfaceKHR hSurface,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    ~Surface();

    VkSurfaceKHR handle() const;
    VkResult result () const;

    VPP_DLLAPI bool getCapabilities ( VkSurfaceCapabilitiesKHR* pResult ) const;
    VPP_DLLAPI bool getFormats ( SurfaceFormats* pResult ) const;
    VPP_DLLAPI bool getPresentationModes ( PresentationModes* pResult ) const;
    VPP_DLLAPI VkExtent2D getSize() const;
    VPP_DLLAPI VkFormat getDefaultFormat ( VkColorSpaceKHR* pColorSpace = 0 ) const;
    VPP_DLLAPI void getParametersAsText ( std::ostream& sst ) const;
};

// -----------------------------------------------------------------------------

class SurfaceImpl : public TSharedObject< SurfaceImpl >
{
public:
    VPP_DLLAPI SurfaceImpl (
        const SurfaceInfo& surfaceInfo,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    VPP_DLLAPI SurfaceImpl (
        VkSurfaceKHR hSurface,
        const PhysicalDevice& hDevice,
        const Instance& hInstance );

    VPP_DLLAPI ~SurfaceImpl();

private:
    void createSurface ( const SurfaceInfo& surfaceInfo );

private:
    friend class Surface;
    Instance d_instance;
    PhysicalDevice d_device;
    VkSurfaceKHR d_handle;
    VkResult d_result;
};

// -----------------------------------------------------------------------------

VPP_INLINE Surface :: Surface()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Surface :: Surface (
    const SurfaceInfo& surfaceInfo,
    const PhysicalDevice& hDevice,
    const Instance& hInstance ) :
        TSharedReference< SurfaceImpl >( new SurfaceImpl ( surfaceInfo, hDevice, hInstance ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Surface :: Surface (
    VkSurfaceKHR hSurface,
    const PhysicalDevice& hDevice,
    const Instance& hInstance ) :
        TSharedReference< SurfaceImpl >( new SurfaceImpl ( hSurface, hDevice, hInstance ) )
{
}

// -----------------------------------------------------------------------------

VPP_INLINE Surface :: ~Surface()
{
}

// -----------------------------------------------------------------------------

VPP_INLINE VkSurfaceKHR Surface :: handle() const
{
    return get()->d_handle;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkResult Surface :: result() const
{
    return get()->d_result;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSURFACE_HPP
