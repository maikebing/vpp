#ifndef INC_VPPEXCOMMONAPP_HPP
#define INC_VPPEXCOMMONAPP_HPP

// -----------------------------------------------------------------------------

#ifdef _MSC_VER
    #define PLATFORM_WINDOWS
#endif

// -----------------------------------------------------------------------------

#include "vppExCommonCamera.hpp"

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#ifdef PLATFORM_WINDOWS
// -----------------------------------------------------------------------------

class ExampleApp
{
public:
    ExampleApp (
        const char* pClassName,
        const char* pTitle,
        const vpp::SurfaceInfo& surfaceInfo );

    void run();

protected:
    const vpp::SurfaceInfo& surfaceInfo() const;
    unsigned int width();
    unsigned int height();

    float timer() const;
    void setTimerSpeed ( float s );

    virtual void render() = 0;
    virtual void onViewChanged();
    virtual void onKeyPressed ( unsigned int keyCode );

private:
    void handleMessages ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    static LRESULT CALLBACK wndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    
    HWND setupWindow (
        HINSTANCE hinstance,
        const char* pClassName,
        const char* pTitle );

protected:
    vpp::SurfaceInfo m_surfaceInfo;
    unsigned int m_width;
    unsigned int m_height;

    float m_timer;
    float m_timerSpeed;

    Camera m_camera;

    float m_zoom;
	glm::vec3 m_rotation;
	glm::vec3 m_cameraPos;
    glm::vec2 m_mousePos;

	float m_rotationSpeed = 1.0f;
	float m_zoomSpeed = 1.0f;

private:
    static ExampleApp* s_pApplication;
};

// -----------------------------------------------------------------------------

inline const vpp::SurfaceInfo& ExampleApp :: surfaceInfo() const
{
    return m_surfaceInfo;
}

// -----------------------------------------------------------------------------

inline unsigned int ExampleApp :: width()
{
    return m_width;
}

// -----------------------------------------------------------------------------

inline unsigned int ExampleApp :: height()
{
    return m_height;
}

// -----------------------------------------------------------------------------

inline float ExampleApp :: timer() const
{
    return m_timer;
}

// -----------------------------------------------------------------------------
#endif // PLATFORM_WINDOWS
// -----------------------------------------------------------------------------

class XWindowError : public std::runtime_error
{
public:
    XWindowError ( const char* pMsg );
};

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------

#endif // INC_VPPEXCOMMONAPP_HPP
