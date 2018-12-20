#include "stdafx.h"
#include "Menu.h"
#include "Gameplay.h"


void Menu::Enter()
{
	Engine.AddSound("Menu", "./Resources/Sounds/menu.mp3", true);
	Engine.GetSound("Menu").Play();
	Engine.AddTexture("Title", "./Resources/title.png");
	
	size_t TITLE = Engine.AddObject();
	IDType& TitleID = Engine.LocateObject(TITLE);

	Engine.AddSprite(TitleID, "Title");
	Sprite& TitleSprite = Engine.GetSprite(TitleID, "Title");
	TitleSprite.SetTexture("Title");
	TitleSprite.SetSize({ 20.f, 20.f });
	
	Engine.AddCommand<SceneCommand<Gameplay>>("StartGame");
	
	Engine.AddController(TitleID, &STATE::None);

	Controller& TitleInput = Engine.GetController(TitleID, &STATE::None);

	TitleInput.MapControl(VK_RETURN, "StartGame");

	Engine.ChangeState(TitleID, &STATE::None);
}

void Menu::Exit()
{
	Engine.DeleteComponents();
}