#pragma once
#include <chrono>
#include "Dependencies/GL/glew.h"
#include "Dependencies/GL/freeglut.h"

class Framework
{
	/* Chrono Time */
	using TimeDuration = STD chrono::duration<float>;
	using Time = STD chrono::high_resolution_clock;
	using TimePoint = Time::time_point;

public:	

	Framework() {}
	~Framework() {}

	bool Initialize(const STD string& Title, int Width, int Height, bool EnableDevConsole=false);
	void Close();

	void GetWindowSizei(int* WinWidth, int * WinHeight) const;
	void GetWindowSizef(float * WinWidth, float * WinHeight) const;

	int Run();

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL WINAPI CloseConsole(DWORD dwCtrlType);

	template<class T>
	void Play()
	{
		if(m_ShiftScene == NULL)
			m_ShiftScene = new T;
	}
	
private:

	void ResetClock();
	void ChangeScenes();
	
private:
	HGLRC		m_HRC			{ NULL };
	HDC			m_HDC			{ NULL };
	HWND		m_HWND			{ NULL };
	RECT		m_ClientRect	{};

	HINSTANCE	m_hInstance;

	STD string  m_WindowTitle	{ "Default Title" };
	int			m_WindowWidth	{ 500 };
	int			m_WindowHeight	{ 500 };

	Scene*		m_CurrentScene	{ NULL };
	Scene*		m_ShiftScene	{ NULL };

// Framework Timer

	TimePoint	m_CurrentTime;
	TimePoint	m_PreviousTime;
	float		m_TimeFrame;
	float		m_TimeAccumulator;
	
};

extern Framework Fw;

