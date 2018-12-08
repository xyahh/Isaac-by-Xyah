#include "stdafx.h"
#include "Window.h"

#include "CyanEngine.h"
#include "DevConsole.h"
#include "resource.h"
#include "Scene.h"

#include "Dependencies/GL/glew.h"
#include "Dependencies/GL/freeglut.h"

bool Window::Initialize(const STD string & Title, int Width, int Height, bool EnableDevConsole)
{
	if(EnableDevConsole)
		DevConsole::Create();

	m_WindowTitle = Title;
	m_WindowWidth = Width;
	m_WindowHeight = Height;

	STD wstring WinTitle{ Title.begin(), Title.end() };
	RECT WindowRect
	{
		0, 0,
		Width, Height
	};

	WNDCLASS    wc;
	DWORD       dwExStyle;
	DWORD       dwStyle;

	LPCTSTR lpszClass = TEXT("OpenGL");
	m_hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)[](HWND h, UINT u, WPARAM w, LPARAM l)->LRESULT 
	{ return Engine.GetFramework().WndProc(h, u, w, l); };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lpszClass;
	RegisterClass(&wc);

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	RECT ScreenSize;
	GetWindowRect(GetDesktopWindow(), &ScreenSize);

	POINT Center
	{
		(ScreenSize.right - WindowRect.right) / 2 ,
		(ScreenSize.bottom - WindowRect.bottom) / 2
	};

	// Create The Window
	m_HWND = CreateWindowEx
	(
		dwExStyle,
		lpszClass,
		WinTitle.c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		Center.x, Center.y,
		Width, Height,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	m_HDC = GetDC(m_HWND);

	GLuint PixelFormat = ChoosePixelFormat(m_HDC, &pfd);
	SetPixelFormat(m_HDC, PixelFormat, &pfd);
	m_HRC = wglCreateContext(m_HDC);
	wglMakeCurrent(m_HDC, m_HRC);

	ShowWindow(m_HWND, SW_SHOW);  
	SetForegroundWindow(m_HWND);  
	SetFocus(m_HWND);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(NEAREST, FARTHEST);

	UpdateWindow(m_HWND);
	return TRUE;
}

LRESULT CALLBACK Window::WndProc(HWND  hWnd, UINT    uMsg, WPARAM  wParam, LPARAM  lParam) const
{
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}
	case WM_CLOSE:
	{
		Engine.Destroy();
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE:
	{
		GetClientRect(m_HWND, (LPRECT)&m_ClientRect);
		return 0;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::Close()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_HRC);
	ReleaseDC(m_HWND, m_HDC);
	DestroyWindow(m_HWND);
	UnregisterClass(L"OpenGL", m_hInstance);

	m_HRC = NULL;
	m_HDC = NULL;
	m_HWND = NULL;
	m_hInstance = NULL;
}

void Window::GetWindowSizei(int * WinWidth, int * WinHeight) const
{
	*WinWidth = m_WindowWidth;
	*WinHeight = m_WindowHeight;
}

void Window::GetWindowSizef(float * WinWidth, float * WinHeight) const
{
	*WinWidth = static_cast<float>(m_WindowWidth);
	*WinHeight = static_cast<float>(m_WindowHeight);
}

int Window::ProcMessage()
{
	if (PeekMessage(&m_MSG, NULL, 0, 0, PM_REMOVE))
	{
		if (m_MSG.message == WM_QUIT)
			return TRUE;
		TranslateMessage(&m_MSG);
		DispatchMessage(&m_MSG);
	}
	return FALSE;
}

void Window::SwapBuffers()
{
	::SwapBuffers(m_HDC);
}
