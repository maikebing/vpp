// -----------------------------------------------------------------------------

#include <vppAll.hpp>
#include "vppExCommonApp.hpp"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
#endif

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

XWindowError :: XWindowError ( const char* pMsg ) :
    std::runtime_error ( pMsg )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

ExampleApp* ExampleApp :: s_pApplication = 0;

// -----------------------------------------------------------------------------

ExampleApp :: ExampleApp (
    const char* pClassName,
    const char* pTitle,
    const vpp::SurfaceInfo& surfaceInfo ) :
        m_surfaceInfo ( surfaceInfo ),
        m_timer ( 0.0f ),
        m_timerSpeed ( 1.0f ),
        m_zoom ( 1.0f )
{
    m_surfaceInfo.hWnd = setupWindow (
        m_surfaceInfo.hInstance, pClassName, pTitle );

    RECT clientRect;
    ::GetClientRect ( m_surfaceInfo.hWnd, & clientRect );

    m_width = clientRect.right;
    m_height = clientRect.bottom;

    s_pApplication = this;
}

// -----------------------------------------------------------------------------

void ExampleApp :: setTimerSpeed ( float s )
{
    m_timerSpeed = s;
}

// -----------------------------------------------------------------------------

void ExampleApp :: handleMessages ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_CLOSE:
            ::DestroyWindow ( hWnd );
            ::PostQuitMessage ( 0 );
            break;

        case WM_PAINT:
            ::ValidateRect ( hWnd, NULL );
            break;

        case WM_KEYDOWN:
            switch ( wParam )
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }

            onKeyPressed ( static_cast< unsigned int >( wParam ) );
            break;

	    case WM_MOUSEMOVE:
		    if (wParam & MK_RBUTTON)
		    {
			    int32_t posx = LOWORD(lParam);
			    int32_t posy = HIWORD(lParam);
			    m_zoom += ( m_mousePos.y - (float)posy) * .005f * m_zoomSpeed;
			    m_camera.translate ( glm::vec3 (
                    -0.0f, 0.0f,
                    ( m_mousePos.y - (float)posy) * .005f * m_zoomSpeed ) );
			    m_mousePos = glm::vec2((float)posx, (float)posy);
			    onViewChanged();
		    }
		    if (wParam & MK_LBUTTON)
		    {
			    int32_t posx = LOWORD(lParam);
			    int32_t posy = HIWORD(lParam);
			    m_rotation.x += (m_mousePos.y - (float)posy) * 1.25f * m_rotationSpeed;
			    m_rotation.y -= (m_mousePos.x - (float)posx) * 1.25f * m_rotationSpeed;
			    m_camera.rotate ( glm::vec3 ( 
                    ( m_mousePos.y - (float)posy) * m_camera.rotationSpeed,
                    -( m_mousePos.x - (float)posx) * m_camera.rotationSpeed, 0.0f ) );
			    m_mousePos = glm::vec2((float)posx, (float)posy);
			    onViewChanged();
		    }
		    if (wParam & MK_MBUTTON)
		    {
			    int32_t posx = LOWORD(lParam);
			    int32_t posy = HIWORD(lParam);
			    m_cameraPos.x -= (m_mousePos.x - (float)posx) * 0.01f;
			    m_cameraPos.y -= (m_mousePos.y - (float)posy) * 0.01f;
			    m_camera.translate ( glm::vec3 (
                    -( m_mousePos.x - (float)posx) * 0.01f,
                    -( m_mousePos.y - (float)posy) * 0.01f, 0.0f ) );
			    onViewChanged();
			    m_mousePos.x = (float)posx;
			    m_mousePos.y = (float)posy;
		    }
		    break;
    }
}

// -----------------------------------------------------------------------------

LRESULT CALLBACK ExampleApp :: wndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( s_pApplication )
        s_pApplication->handleMessages ( hWnd, uMsg, wParam, lParam );

    return ( ::DefWindowProc ( hWnd, uMsg, wParam, lParam ) );
}

// -----------------------------------------------------------------------------

HWND ExampleApp :: setupWindow (
    HINSTANCE hInstance,
    const char* pClassName,
    const char* pTitle )
{
    WNDCLASSEX wndClass;

    wndClass.cbSize = sizeof ( WNDCLASSEX );
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = wndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = ::LoadIcon ( 0, IDI_APPLICATION );
    wndClass.hCursor = ::LoadCursor ( 0, IDC_ARROW );
    wndClass.hbrBackground =  ( HBRUSH ) ::GetStockObject ( BLACK_BRUSH );
    wndClass.lpszMenuName = 0;
    wndClass.lpszClassName = pClassName;
    wndClass.hIconSm = ::LoadIcon ( 0, IDI_WINLOGO );

    if ( ! ::RegisterClassEx ( & wndClass ) )
        throw XWindowError ( "Error registering window class" );

    int screenWidth = ::GetSystemMetrics ( SM_CXSCREEN );
    int screenHeight = ::GetSystemMetrics ( SM_CYSCREEN );

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    unsigned int width = 1280;
    unsigned int height = 720;

    RECT windowRect;
    windowRect.left = (long)screenWidth / 2 - width / 2;
    windowRect.right = (long)width;
    windowRect.top = (long)screenHeight / 2 - height / 2;
    windowRect.bottom = (long)height;

    AdjustWindowRectEx ( & windowRect, dwStyle, FALSE, dwExStyle );

    HWND window = ::CreateWindowEx (
        0, pClassName, pTitle,
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        windowRect.left, windowRect.top, windowRect.right, windowRect.bottom,
        0, 0, hInstance, 0
    );

    if ( ! window )
        throw XWindowError ( "Error creating the window" );

    ::ShowWindow ( window, SW_SHOW );
    ::SetForegroundWindow ( window );
    ::SetFocus ( window );

    return window;
}

// -----------------------------------------------------------------------------

void ExampleApp :: run()
{
    MSG msg;

    while ( TRUE )
    {
        auto tStart = std::chrono::high_resolution_clock::now();

        if ( ::PeekMessage ( & msg, 0, 0, 0, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
            {
                break;
            }
            else
            {
                ::TranslateMessage ( & msg );
                ::DispatchMessage ( & msg );
            }
        }

        render();

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		float frameTimer = (float)tDiff / 1000.0f;

		//if (!paused)
		{
			m_timer += m_timerSpeed * frameTimer;

			if ( m_timer > 1.0f )
				m_timer -= 1.0f;
		}
    }
}

// -----------------------------------------------------------------------------

void ExampleApp :: onViewChanged()
{
}

// -----------------------------------------------------------------------------

void ExampleApp :: onKeyPressed ( unsigned int keyCode )
{
}

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------
