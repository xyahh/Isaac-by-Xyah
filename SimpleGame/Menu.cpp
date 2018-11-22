#include "stdafx.h"
#include "Menu.h"
#include "Gameplay.h"
#include "Framework.h"


void Menu::Init()
{
	Engine.ReserveObjects(0, 0, 1, 1);

	Engine.AddSound("Menu", "./Resources/Sounds/menu.mp3", true);

	Engine.GetSound("Menu").Play();

	Engine.AddVisual("Title");
	Engine.GetVisualGraphics("Title").SetSize( 800.f, 800.f);
	Engine.GetVisualGraphics("Title").SetOffset( 0.f, 50.f );
	Engine.AddTexture("Title", "./Resources/title.png");
	Engine.GetVisualGraphics("Title").SetTexID("Title");
	
	Engine.AddCommand("Next", new ShiftSceneCommand(*m_Framework, new Gameplay));
	
	Engine.AddStateType("GlobalState", new GlobalState);
	Engine.GetStateType("GlobalState")->GetInput("").AddKeyboardInput(VK_RETURN, "Next");

	Engine.AddNonActorState("GlobalState");
}

void Menu::Exit()
{
	Engine.DeleteComponents();
}

void Menu::Render(float fInterpolation) 
{ 
	Engine.Render(fInterpolation);
}

void Menu::Update() 
{ 
	Engine.Update(); 
}
