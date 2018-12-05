#include "stdafx.h"
#include "Framework.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("OpenGL");
LPCTSTR WindowName = TEXT("Binding of Isaac");
HGLRC	gHRC{ NULL };
HDC		gHDC{ NULL };
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE   hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow)  
{
	World::SetScale(0.025f, 1);
	HWND		hWnd;
	GLuint      PixelFormat;
	WNDCLASS    wc;
	DWORD       dwExStyle;
	DWORD       dwStyle;
	RECT        WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long) 800;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long) 800;

	g_hInst = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lpszClass;

	if (!RegisterClass(&wc))                                
	{
		MessageBox
		(
			NULL,
			L"Failed To Register The Window Class.",
			L"ERROR",
			MB_OK | MB_ICONEXCLAMATION
		);
		return FALSE;                                        
	}
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,
		lpszClass,
		WindowName,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		g_hInst,
		NULL)))
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	//static  PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
	//{
	//	sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
	//	1,                                          // Version Number
	//	PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
	//	PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
	//	PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
	//	PFD_TYPE_RGBA,                              // Request An RGBA Format
	//	32,											 // Select Our Color Depth
	//	0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
	//	0,                                          // No Alpha Buffer
	//	0,                                          // Shift Bit Ignored
	//	0,                                          // No Accumulation Buffer
	//	0, 0, 0, 0,                                 // Accumulation Bits Ignored
	//	16,                                         // 16Bit Z-Buffer (Depth Buffer)  
	//	0,                                          // No Stencil Buffer
	//	0,                                          // No Auxiliary Buffer
	//	PFD_MAIN_PLANE,                             // Main Drawing Layer
	//	0,                                          // Reserved
	//	0, 0, 0                                     // Layer Masks Ignored
	//};
	//pfd.
	if (!(gHDC = GetDC(hWnd)))                         // Did We Get A Device Context?
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(gHDC, &pfd))) // Did Windows Find A Matching Pixel Format?
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!SetPixelFormat(gHDC, PixelFormat, &pfd))       // Are We Able To Set The Pixel Format?
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!(gHRC = wglCreateContext(gHDC)))               // Are We Able To Get A Rendering Context?
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!wglMakeCurrent(gHDC, gHRC))                    // Try To Activate The Rendering Context
	{
		//KillGLWindow();                             // Reset The Display
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);                       // Show The Window
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);
	glewInit();

	UpdateWindow(hWnd);
	Fw.ToScene(new Gameplay);
	Fw.Run();

	MSG     Message;
	while (TRUE) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT) break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			Fw.Loop();
			SwapBuffers(gHDC);
		}
	}


	Fw.Close();
	// Shutdown
	//KillGLWindow();                                 // Kill The Window
	return (Message.wParam);                            // Exit The Program
}

LRESULT CALLBACK WndProc(HWND  hWnd, UINT    uMsg, WPARAM  wParam, LPARAM  lParam)
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
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
	{
		//keys[wParam] = TRUE;          
		return 0;
	}

	case WM_KEYUP:
	{
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

//int main(int argc, char **argv)
//{	
//	/* Scale Set (0.01m : 1 px) */
//	World::SetScale(0.025f, 1);
//	
//	/* Game Loop */
//	Fw.Initialize("The Binding of Isaac", 800, 800, argc, argv);
//	Fw.ToScene(new Gameplay);
//	Fw.Run();
//	Fw.Close();
//
//    return FALSE;
//}

