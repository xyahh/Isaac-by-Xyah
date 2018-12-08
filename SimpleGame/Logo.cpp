#include "stdafx.h"
#include "Logo.h"
#include "Menu.h"

size_t LOGO;

void Logo::Enter()
{
	size_t LOGO_TEX;
	Engine.AddObject(&LOGO);
	Engine.AddTexture(&LOGO_TEX, "./Resources/logo.png");

	size_t LOGO_SPRITE;
	Engine.AddSprite(&LOGO_SPRITE, LOGO);
	Sprite& LogoSprite = Engine.GetSprite(LOGO, LOGO_SPRITE);
	LogoSprite.SetTexture(LOGO_TEX);	
	LogoSprite.SetSize({ 9.f, 3.f });
}

void Logo::Exit()
{
	Engine.DeleteComponents();
}
