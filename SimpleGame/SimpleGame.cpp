#include "stdafx.h"
#include "CyanEngine.h"
#include "Logo.h"

int WINAPI WinMain(HINSTANCE   hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow)  
{
	Engine.Init("Binding of Isaac", 800, 800);
	Engine.GetWindow().PlayScene<Logo>();
	Engine.GetWorld().SetScale(0.025f, 1);
	Engine.MainLoop();
	return FALSE;
}
	
