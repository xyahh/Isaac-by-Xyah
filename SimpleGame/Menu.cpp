#include "stdafx.h"
#include "Menu.h"
#include "Gameplay.h"


void Menu::Enter()
{
	Engine.GetSound().Add("Menu", "./Resources/Sounds/menu.mp3", true);
	Engine.GetSound().Play("Menu");
	Engine.AddTexture("Title", "./Resources/title.png");
	
	IDType& TitleID = Engine.AddObject();

	Engine.AddSprite(TitleID, "Title");
	Sprite& TitleSprite = Engine.GetSprite(TitleID, "Title");
	TitleSprite.SetTexture("Title");
	TitleSprite.SetSize({ 20.f, 20.f });
	
	Engine.AddCommand<SceneCommand<Gameplay>>("StartGame");
	
	Engine.AddController(TitleID, &STATE::Global);

	Controller& TitleInput = Engine.GetController(TitleID, &STATE::Global);

	TitleInput.MapControl(VK_RETURN, "StartGame");

	Engine.ChangeState(TitleID, &STATE::Global);
}

void Menu::Exit()
{
	Engine.DeleteComponents();
}