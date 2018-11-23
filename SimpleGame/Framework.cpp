#include "stdafx.h"
#include "Framework.h"

#include <iostream>

#include "Dependencies/GL/glew.h"
#include "Dependencies/GL/freeglut.h"
#include "Scene.h"

Framework Fw;

Framework::~Framework()
{
	Close();
}

void Framework::Initialize(const STD string & strWindowsTitle, int width, int height, int argc, char* argv[])
{
	m_WindowTitle = strWindowsTitle;
	m_WindowWidth = width;
	m_WindowHeight = height;

	// Initialize GL stuff
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - m_WindowWidth) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - m_WindowHeight) / 2
	);
	glutInitWindowSize(m_WindowWidth, m_WindowHeight);
	glutCreateWindow(m_WindowTitle.c_str());

	//Disable Maximize and Resizing
	HWND hWnd = FindWindowA(NULL, m_WindowTitle.c_str());
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle &= ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);

	glewInit();
}

void Framework::Fullscreen()
{
	m_WindowWidth = glutGet(GLUT_SCREEN_WIDTH);
	m_WindowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	glutFullScreen();
}

void Framework::Close()
{
	SafeClose();
}

void Framework::SafeClose()
{
	if (m_CurrentScene)
	{
		m_CurrentScene->Exit();
		delete m_CurrentScene;
	}
	m_CurrentScene = NULL;
	m_ShiftScene = NULL;
	Engine.Destroy();
}

void Framework::BindFunctions()
{
	SetConsoleCtrlHandler([](DWORD dw)->BOOL
	{
		return Fw.ConsoleHandler(dw);
	}, TRUE);
	glutDisplayFunc([]()
	{
		Fw.Loop();
	});
	glutIdleFunc([]()
	{
		Fw.Loop();
	});
	glutCloseFunc([]()
	{
		Fw.Close();
	});
	atexit([]()
	{
		Fw.Close();
	});
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

BOOL WINAPI Framework::ConsoleHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
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
	Engine.Initialize(m_WindowWidth, m_WindowHeight);
	Scene::m_Framework = this;
	BindFunctions();
	ChangeScenes();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(NEAREST, FARTHEST);

	ResetClock();
	glutMainLoop();
}

void Framework::Loop()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
	glutSwapBuffers();
}	
