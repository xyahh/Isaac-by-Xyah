#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"

void Gameplay::Init()
{
	Engine.ReserveObjects(4, 100, 1, 10); 
	Engine.AddSoundsByFile("./Resources/Init/Sounds.csv");
	Engine.AddTexturesByFile("./Resources/Init/Textures.csv");
	Engine.AddCommandsByFile("./Resources/Init/Commands.csv");
	Engine.AddStatesByFile("./Resources/Init/States.csv");
	Engine.AddInputsByFile("./Resources/Init/Input.csv");
	Engine.AddActorsByFile("./Resources/Init/Actors.csv");
	Gamepad1.Connect();

	{
		id_type o = Engine.AddObject("Wall");
		Physics& p = Engine.GetEntityPhysics(o);
		p.SetCollision(&Collision::Basic);
		p.GetBox().SetDimensions({ 1.75f, 17.5f, 10.f });
		p.SetPosition({ 10.f, 0.f, 0.f });
	}

	{
		id_type o = Engine.AddObject("Wall");
		Physics& p = Engine.GetEntityPhysics(o);
		p.SetCollision(&Collision::Basic);
		p.GetBox().SetDimensions({ 17.5f, 3.f, 10.f });
		p.SetPosition({ 0.f, 10.f, 0.f });
	}
	
	{
		id_type o = Engine.AddObject("Wall");
		Physics& p = Engine.GetEntityPhysics(o);
		p.SetCollision(&Collision::Basic);
		p.GetBox().SetDimensions({ 17.5f, 3.f, 10.f });
		p.SetPosition({ 0.f, -10.f, 0.f });
	}

	{
		id_type o = Engine.AddObject("Wall");
		Physics& p = Engine.GetEntityPhysics(o);
		p.SetCollision(&Collision::Basic);
		p.GetBox().SetDimensions({ 1.75f, 17.5f, 10.f });
		p.SetPosition({ -10.f, 0.f, 0.f });
	}

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
