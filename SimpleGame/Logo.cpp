#include "stdafx.h"
#include "Framework.h"
#include "Logo.h"
#include "Menu.h"

size_t LOGO;

void Logo::Init()
{
	size_t LOGO_TEX;
	Engine.AddObject(&LOGO);
	Engine.AddTexture(&LOGO_TEX, "./Resources/logo.png");

	size_t LOGO_SPRITE;
	Engine.AddSprite(&LOGO_SPRITE, LOGO);
	Sprite& LogoSprite = Engine.GetSprite(LOGO, LOGO_SPRITE);
	LogoSprite.SetTexture(LOGO_TEX);	
	LogoSprite.SetSize({ 9.f, 3.f });
	FadeMax = 1.5f;
	Fade = 0.f;
}

void Logo::Exit()
{
	Engine.DeleteComponents();
}

void Logo::Render(float fInterpolation)
{
	if (Fade < 0.f)
		m_Framework->Play<Menu>();
	Engine.Render(fInterpolation);
}

void Logo::Update()
{
	Engine.GetGraphics(LOGO).SetColor(1.f, 1.f, 1.f, Fade);
	Fade += UPDATE_TIME / FadeMax;
	if (Fade > fabs(FadeMax))
		FadeMax *= -1.f;		
}

