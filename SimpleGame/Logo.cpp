#include "stdafx.h"
#include "Logo.h"
#include "Menu.h"

void Logo::Enter()
{
	size_t LOGO = Engine.AddObject();
	Engine.AddTexture("Logo", "./Resources/logo_anim.png");
	
	IDType& LogoID = Engine.LocateObject(LOGO);
	Engine.AddSprite(LogoID, "Logo");

	Sprite& LogoSprite = Engine.GetSprite(LogoID, "Logo");
	LogoSprite.SetTexture("Logo");	
	LogoSprite.SetSize({ 15.f, 1.5f });
	LogoSprite.SetTotal({ 11, 22 });
	LogoSprite.SetSpriteType(SpriteShape::Grid);
	LogoSprite.SetFrameRate(60.f);
	LogoSprite.AddEvent(SpriteEvent::LoopEnd, []() 
	{
		Engine.GetWindow().PlayScene<Menu>();
	});
}

void Logo::Exit()
{
	Engine.DeleteComponents();
}
