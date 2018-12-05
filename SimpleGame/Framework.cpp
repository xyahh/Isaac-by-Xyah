#include "stdafx.h"
#include "Framework.h"

#include <iostream>

#include "Scene.h"
#include "CyanEngine.h"

Framework Fw;




Framework::~Framework()
{
	Close();
}

BOOL Framework::Initialize(const STD string & strWindowsTitle, int width, int height, int argc, char* argv[])
{
	m_WindowTitle = strWindowsTitle;
	m_WindowWidth = width;
	m_WindowHeight = height;

	//// Initialize GL stuff
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//glutInitWindowPosition(
	//	(glutGet(GLUT_SCREEN_WIDTH) - m_WindowWidth) / 2,
	//	(glutGet(GLUT_SCREEN_HEIGHT) - m_WindowHeight) / 2
	//);
	//glutInitWindowSize(m_WindowWidth, m_WindowHeight);
	//glutCreateWindow(m_WindowTitle.c_str());
	//
	////Disable Maximize and Resizing
	//HWND hWnd = FindWindowA(NULL, m_WindowTitle.c_str());
	//DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	//dwStyle &= ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	//SetWindowLong(hWnd, GWL_STYLE, dwStyle);



	return 1;

}

void Framework::Fullscreen()
{
	m_WindowWidth = glutGet(GLUT_SCREEN_WIDTH);
	m_WindowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	glutFullScreen();
}

void Framework::Close()
{
	if (m_HRC)                                // Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))  
			MessageBox
			(
				NULL, 
				L"Release Of DC And RC Failed.", 
				L"SHUTDOWN ERROR", 
				MB_OK | MB_ICONINFORMATION
			);
		if (!wglDeleteContext(m_HRC))  
			MessageBox
			(
				NULL, 
				L"Release Rendering Context Failed.", 
				L"SHUTDOWN ERROR", 
				MB_OK | MB_ICONINFORMATION
			);
		m_HRC = NULL;                           // Set RC To NULL
	}
	if (m_HDC && !ReleaseDC(m_HWND, m_HDC))                    // Are We Able To Release The DC
	{
		MessageBox
		(
			NULL, 
			L"Release Device Context Failed.", 
			L"SHUTDOWN ERROR", 
			MB_OK | MB_ICONINFORMATION
		);
		m_HDC = NULL;                           // Set DC To NULL
	}
	if (m_HWND && !DestroyWindow(m_HWND))                   // Are We Able To Destroy The Window?
	{
		MessageBox
		(
			NULL, 
			L"Could Not Release hWnd.", 
			L"SHUTDOWN ERROR", 
			MB_OK | MB_ICONINFORMATION
		);
		m_HWND = NULL;                          // Set hWnd To NULL
	}
	if (!UnregisterClass(L"OpenGL", m_hInstance))               // Are We Able To Unregister Class
	{
		MessageBox
		(
			NULL, 
			L"Could Not Unregister Class.", 
			L"SHUTDOWN ERROR", 
			MB_OK | MB_ICONINFORMATION
		);
		m_hInstance = NULL;                         // Set hInstance To NULL
	}
	Engine.Destroy();
}

void Framework::BindFunctions()
{
	//SetConsoleCtrlHandler([](DWORD dw)->BOOL
	//{
	//	return Fw.CloseConsole(dw);
	//}, TRUE);
	//glutDisplayFunc([]()
	//{
	//	Fw.Loop();
	//});
	//glutIdleFunc([]()
	//{
	//	Fw.Loop();
	//});
	//glutCloseFunc([]()
	//{
	//	Fw.Close();
	//});
	//atexit([]()
	//{
	//	Fw.Close();
	//});

	//glutKeyboardFunc([](unsigned char Key, int, int) 
	//{
	//	printf("PRESS\n");
	//	Fw.KeyboardInput(Key, true);
	//});

	//glutKeyboardUpFunc([](unsigned char Key, int, int)
	//{
	//	printf("RELEASE\n");
	//	Fw.KeyboardInput(Key, false);
	//});

	//glutSpecialFunc([](int Key, int, int)
	//{
	//	Fw.KeyboardInput(Key, true);
	//});
	//
	//glutSpecialUpFunc([](int Key, int, int)
	//{
	//	Fw.KeyboardInput(Key, false);
	//});
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

void Framework::KeyboardInput(int Value, bool Pressed)
{
	if (!m_CurrentScene) return;
	m_CurrentScene->HandleInput(Value, Pressed);
}

BOOL WINAPI Framework::CloseConsole(DWORD dwCtrlType)
{
	Close();
	return TRUE;
}

void Framework::ToScene(Scene*&& pScene)
{
	m_ShiftScene = pScene;
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

void Framework::Run()
{
	Scene::m_Framework = this;
	Engine.Initialize();
	//BindFunctions();
	ChangeScenes();
	//glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	//GLUT_KEY_UP;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(NEAREST, FARTHEST);

	ResetClock();
	//glutMainLoop();
}

void Framework::Loop()
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

	m_CurrentScene->Render(m_TimeAccumulator * UPDATE_FREQUENCY);

	if(m_ShiftScene != NULL)
		ChangeScenes();
}	
