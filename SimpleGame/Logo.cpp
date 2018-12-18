#include "stdafx.h"
#include "Logo.h"
#include "Menu.h"

void Logo::Enter()
{
	size_t LOGO;
	size_t LOGO_TEX;
	Engine.AddObject(&LOGO);
	Engine.AddTexture(&LOGO_TEX, "./Resources/logo_anim.png");

	size_t LOGO_SPRITE;
	Engine.AddSprite(&LOGO_SPRITE, LOGO);

	Sprite& LogoSprite = Engine.GetSprite(LOGO, LOGO_SPRITE);
	LogoSprite.SetTexture(LOGO_TEX);	
	LogoSprite.SetSize({ 15.f, 1.5f });
	LogoSprite.SetTotal({ 11, 22 });
	LogoSprite.SetSpriteType(SpriteType::Grid);
	LogoSprite.SetFrameRate(60.f);
	LogoSprite.AddEvent(SpriteEvent::LoopEnd, []() 
	{
		Engine.GetFramework().PlayScene<Menu>();
	});
}

void Logo::Exit()
{
	Engine.DeleteComponents();
}
