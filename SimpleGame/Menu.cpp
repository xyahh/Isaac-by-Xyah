#include "stdafx.h"
#include "Menu.h"
#include "Gameplay.h"
#include "Framework.h"


void Menu::Init()
{
	//Engine.ReserveObjects(0, 0, 1, 1);
	//
	//Engine.AddSound("Menu", "./Resources/Sounds/menu.mp3", true);
	//
	//Engine.GetSound("Menu").Play();
	//
	//Engine.AddVisual("Title");
	//Graphics& VisualGraphics = Engine.GetGraphics("Title");
	//Engine.AddTexture("Title", "./Resources/title.png");
	//VisualGraphics.AddSprite("Title");
	//Sprite& Title = VisualGraphics.GetSprite("Title");
	//
	//Title.SetSize({ 20.f, 20.f });
	//Title.SetTexID("Title");
	//
	//Engine.AddCommand("Next", new ShiftSceneCommand(*m_Framework, new Gameplay));
	//
	//Engine.AddStateType("GlobalState", new GlobalState);
	//Engine.GetStateType("GlobalState")->GetInput("").AddKeyboardInput(VK_RETURN, "Next");
	//
	//Engine.AddNonActorState("GlobalState");
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
