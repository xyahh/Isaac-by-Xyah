#include "stdafx.h"
#include "Menu.h"
#include "Gameplay.h"
#include "Framework.h"


void Menu::Init()
{
	size_t MENU_SOUND;
	Engine.AddSound(&MENU_SOUND, "./Resources/Sounds/menu.mp3", true);
	Engine.GetSound(MENU_SOUND).Play();
	
	size_t TITLE_TEX;
	Engine.AddTexture(&TITLE_TEX, "./Resources/title.png");

	size_t TITLE;
	Engine.AddObject(&TITLE);

	size_t TITLE_SPRITE;
	Engine.AddSprite(&TITLE_SPRITE, TITLE);
	Sprite& TitleSprite = Engine.GetSprite(TITLE, TITLE_SPRITE);
	TitleSprite.SetTexture(TITLE_TEX);
	TitleSprite.SetSize({ 20.f, 20.f });

	size_t START_GAME;
	Engine.AddCommand<SceneCommand<Gameplay>>(&START_GAME);

	size_t NULL_STATE;
	Engine.AddStatePrototype<NullState>(&NULL_STATE);

	Engine.AddObjectState(TITLE, NULL_STATE);
	Input& TitleInput = Engine.GetStateInput(TITLE, NULL_STATE);
	
	TitleInput.AddKeyMapping(VK_RETURN, START_GAME);

	Engine.ChangeState(TITLE, NULL_STATE);
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
