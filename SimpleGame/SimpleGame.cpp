#include "stdafx.h"
#include "CyanEngine.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"

int WINAPI WinMain(HINSTANCE   hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow)  
{
	Engine.Init("Binding of Isaac", 800, 800, true);
	Engine.GetFramework().PlayScene<Gameplay>();
	World::SetScale(0.025f, 1);
	Engine.MainLoop();
	return FALSE;
}
