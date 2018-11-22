#include "stdafx.h"
#include "Framework.h"
#include "Logo.h"
#include "Menu.h"

void Logo::Init()
{
	Engine.ReserveObjects(0, 0, 1, 0);

	Engine.AddVisual("Logo");
	Engine.AddTexture("Logo", "./Resources/logo.png");
	Engine.GetVisualGraphics("Logo").SetTexID("Logo");
	Engine.GetVisualGraphics("Logo").SetSize(300.f, 100.f);
	
	FadeMax = 1.5f;
	Fade = 0.f;
}

void Logo::Exit()
{
	Engine.DeleteComponents(TEXTURES | VISUALS);
}

void Logo::Render(float fInterpolation)
{
	if (Fade < 0.f)
		m_Framework->ToScene(new Menu);
	Engine.Render(fInterpolation);
}

void Logo::Update()
{
	Engine.GetVisualGraphics("Logo").SetColor(1.f, 1.f, 1.f, Fade);
	Fade += UPDATE_TIME / FadeMax;
	if (Fade > fabs(FadeMax))
		FadeMax *= -1.f;		
}

