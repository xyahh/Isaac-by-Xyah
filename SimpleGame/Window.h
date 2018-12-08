#pragma once

class Window
{
	friend Cyan;

public:
	
	void GetWindowSizei(int* WinWidth, int * WinHeight) const;
	void GetWindowSizef(float * WinWidth, float * WinHeight) const;
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) const;

	template<class T>
	void PlayScene()
	{
		Engine.QueueAction([&]()
		{
			if (m_Scene)
			{
				m_Scene->Exit();
				delete m_Scene;
			}
			m_Scene = new T;
			m_Scene->Enter();
		
			Engine.UpdateInput();
		});
	}

private:

	Window() {}
	~Window() {}

	bool Initialize(const STD string& Title, int Width, int Height, bool EnableDevConsole=false);
	void Close();
	int ProcMessage();
	void SwapBuffers();
	

	MSG			m_MSG;
	
	HGLRC		m_HRC			{ NULL };
	HDC			m_HDC			{ NULL };
	HWND		m_HWND			{ NULL };
	RECT		m_ClientRect	{};

	HINSTANCE	m_hInstance;

	STD string  m_WindowTitle	{ "Default Title" };
	int			m_WindowWidth	{ 500 };
	int			m_WindowHeight	{ 500 };

	Scene*		m_Scene	{ NULL };
};

