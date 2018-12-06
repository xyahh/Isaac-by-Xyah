#include "stdafx.h"
#include "Framework.h"

#include <iostream>

#include "Scene.h"
#include "CyanEngine.h"
#include "DevConsole.h"
#include "resource.h"

Framework Fw;
bool Framework::Initialize(const STD string & Title, int Width, int Height)
{
#ifdef CYAN_DEBUG_CONSOLE
	DevConsole::Create();
#endif

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
	{ 
		return Fw.WndProc(h, u, w, l); 
	};
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

	UpdateWindow(m_HWND);

	
	return TRUE;
}

LRESULT CALLBACK Framework::WndProc(HWND  hWnd, UINT    uMsg, WPARAM  wParam, LPARAM  lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

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
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
	{
		if(m_CurrentScene)
			m_CurrentScene->HandleInput(wParam, true);
		return 0;
	}

	case WM_KEYUP:
	{
		if (m_CurrentScene)
			m_CurrentScene->HandleInput(wParam, false);
		//keys[wParam] = FALSE;         
		return 0;
	}

	case WM_SIZE:
	{

		return 0;
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Framework::Close()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_HRC);
	m_HRC = NULL;
	ReleaseDC(m_HWND, m_HDC);
	DestroyWindow(m_HWND);
	m_HDC = NULL;
	m_HWND = NULL;
	UnregisterClass(L"OpenGL", m_hInstance);
	m_hInstance = NULL;
	Engine.Destroy();
}

void Framework::ResetClock()
{
	m_CurrentTime = Time::now();
	m_PreviousTime = Time::now();
	m_TimeAccumulator = 0.f;
	m_TimeFrame = 0.f;
}

void Framework::GetWindowSizei(int * WinWidth, int * WinHeight) const
{
	*WinWidth = m_WindowWidth;
	*WinHeight = m_WindowHeight;
}

void Framework::GetWindowSizef(float * WinWidth, float * WinHeight) const
{
	*WinWidth = static_cast<float>(m_WindowWidth);
	*WinHeight = static_cast<float>(m_WindowHeight);
}

BOOL WINAPI Framework::CloseConsole(DWORD dwCtrlType)
{
	Close();
	return TRUE;
}

void Framework::ChangeScenes()
{
	if (m_CurrentScene)
	{
		m_CurrentScene->Exit();
		delete m_CurrentScene;
	}
	m_CurrentScene = m_ShiftScene;
	m_ShiftScene = NULL;
	m_CurrentScene->Init();
}

int Framework::Run()
{
	Scene::m_Framework = this;
	Engine.Initialize();
	ChangeScenes();

	atexit([]()
	{
		Fw.Close();
	});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(NEAREST, FARTHEST);

	ResetClock();
	MSG Message;
	while (TRUE) 
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) 
		{
			if (Message.message == WM_QUIT) break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClearDepth(FARTHEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_CurrentTime = Time::now();
			m_TimeFrame = TimeDuration(m_CurrentTime - m_PreviousTime).count();
			m_PreviousTime = m_CurrentTime;
			m_TimeAccumulator += m_TimeFrame;

			while (m_TimeAccumulator >= UPDATE_TIME)
			{
				m_CurrentScene->Update();
				m_TimeAccumulator -= UPDATE_TIME;
			}
			
			m_CurrentScene->Render(m_TimeAccumulator * UPDATE_TIME);

			if (m_ShiftScene != NULL)
				ChangeScenes();

			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(m_HDC, hPen);
			Ellipse(m_HDC, 100, 100, 200, 200);
			SelectObject(m_HDC, oldPen);
			DeleteObject(hPen);
		
			SwapBuffers(m_HDC);

			
		}

	}
	
	Close();
	return (Message.wParam);
}
