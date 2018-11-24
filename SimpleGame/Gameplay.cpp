#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"

void Gameplay::Init()
{
	Engine.ReserveObjects(4, 100, 1, 10); 
	Engine.AddSoundsByFile("./Resources/Init/Sounds.csv");
	Engine.AddTexturesByFile("./Resources/Init/Textures.csv");
	//Engine.GetSound("Main").Play();
	Gamepad1.Connect();
	Engine.AddCommandsByFile("./Resources/Init/Commands.csv");

	Engine.AddCommand("Zoom", new FxCommand([](const id_type& ActorID)
	{
		static bool ZoomedOut = true;
		if (ZoomedOut) World::SetScale(0.01f, 1);
		else World::SetScale(0.025f, 1);
		ZoomedOut = !ZoomedOut;
	}));

	Engine.AddStatesByFile("./Resources/Init/States.csv");
	Engine.AddInputsByFile("./Resources/Init/Input.csv");
	Engine.AddActorsByFile("./Resources/Init/Actors.csv");

	/* Visuals */
	Engine.AddVisual("Map", PHYSICAL_UNITS | BACK_DRAW);
	Engine.GetVisualGraphics("Map").SetTexID("Map");
	Engine.GetVisualGraphics("Map").SetSize(20.f, 20.f);
}

void Gameplay::Exit()
{
	Engine.DeleteComponents();
}

void Gameplay::Render(float fInterpolation) 
{ 
	Engine.Render(fInterpolation);
}

void Gameplay::Update() 
{ 
	//Engine.GetObjectGraphics(ObjectID).ObjectSprite.FrameLinearAdvance();
	Engine.Update();
}
