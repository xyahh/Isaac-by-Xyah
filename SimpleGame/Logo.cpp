#include "stdafx.h"
#include "Framework.h"
#include "Logo.h"
#include "Menu.h"

void Logo::Init()
{
	//Engine.ReserveObjects(0, 0, 1, 0);
	//
	//Engine.AddVisual("Logo");
	//Engine.AddTexture("Logo", "./Resources/logo.png");
	//Graphics& g = Engine.GetGraphics("Logo");
	//g.AddSprite("Logo");
	//Sprite& s = g.GetSprite("Logo");
	//s.SetTexID("Logo");
	//s.SetSize({ 9.f, 3.f });	
	//FadeMax = 1.5f;
	//Fade = 0.f;
}

void Logo::Exit()
{
	//Engine.DeleteComponents(TEXTURES | OBJECTS);
}

void Logo::Render(float fInterpolation)
{
	if (Fade < 0.f)
		m_Framework->ToScene(new Menu);
	Engine.Render(fInterpolation);
}

void Logo::Update()
{
	//Engine.GetGraphics("Logo").SetColor(1.f, 1.f, 1.f, Fade);
	Fade += UPDATE_TIME / FadeMax;
	if (Fade > fabs(FadeMax))
		FadeMax *= -1.f;		
}

