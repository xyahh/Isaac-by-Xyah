#include "stdafx.h"
#include "Framework.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"

int WINAPI WinMain(HINSTANCE   hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow)  
{
	World::SetScale(0.025f, 1);
	Fw.Initialize("Binding of Isaac", 800, 800, true);
	Fw.Play<Logo>();
	return 	Fw.Run();
}
