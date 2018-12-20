#pragma once

class Window
{
	friend Cyan;

public:
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
			Engine.ResetClock();
			/*
			Add all the Keys that were mapped
			in the Scene Initialization
			*/
			Engine.UpdateInput();
		});
	}

private:

	Window() {}
	~Window() {}

	bool Initialize(const STD string& Title, int Width, int Height, const STD string& DevConsoleTitle);
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

	Scene*		m_Scene	{ NULL };
};

