#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"

BaseCollision* coll[2]{ NULL, NULL };
BaseCollision* collvent = NULL;
BaseCollision* collwall[4]{ NULL, NULL, NULL, NULL };

u_int ISAAC_HEAD
, ISAAC_BODY
, BACKGROUND
, BULLET
, HP
, LANDING
, VENT;

u_int ObjectID;

void Gameplay::Init()
{
	Engine.ReserveObjects(4, 100, 1, 10);

	//Sound
	{
		Engine.AddSound("Main",	   "./Resources/Sounds/main.mp3",    true );
		Engine.AddSound("ShootInf", "./Resources/Sounds/inflate.mp3", false);
		Engine.AddSound("ShootRel", "./Resources/Sounds/ploop.mp3",   false);
		//Engine.GetSound("Main").Play();
	}

	Engine.AddTextures
	({ {"./Resources/Characters/cain_head.png"  , ISAAC_HEAD},
		{"./Resources/Characters/basic_body.png", ISAAC_BODY},
		{"./Resources/Levels/Depths.png"		, BACKGROUND},
		{"./Resources/bullet.png"				, BULLET	},
		{"./Resources/hp_test.png"				, HP		},
		{"./Resources/landing.png"				, LANDING	},
		{"./Resources/vent.png"					, VENT		}});

	Gamepad1.Connect();

	//Commands
	{
		float Force{ 1'000.f };
		float Shoot{ 1'000.f };

		Engine.AddCommand("MoveUp",		new ForceCommand({ 0.f,  Force, 0.f }));
		Engine.AddCommand("MoveDown",	new ForceCommand({ 0.f, -Force, 0.f }));
		Engine.AddCommand("MoveLeft",	new ForceCommand({ -Force, 0.f, 0.f }));
		Engine.AddCommand("MoveRight",	new ForceCommand({ Force, 0.f, 0.f }));

		Engine.AddCommand("StartSlamming",	new NewStateOnPressCommand("SlamCharging"));
		Engine.AddCommand("StartJumping",	new NewStateOnPressCommand("Jumping"));
		Engine.AddCommand("StartMoving",		new NewStateOnPressCommand("Moving"));
		Engine.AddCommand("StartShooting",	new NewStateOnPressCommand("Shooting"));

		Engine.AddCommand("Zoom", new FxCommand([](const id_type& ActorID)
		{
			static bool ZoomedOut = true;
			if (ZoomedOut) World::SetScale(0.01f, 1);
			else World::SetScale(0.025f, 1);
			ZoomedOut = !ZoomedOut;
		}));
	}

	//States
	{
		State::SetDefaultState("Idle");

		Engine.AddStateType("Global",			new GlobalState);
		Engine.AddStateType("Idle",				new IdleState);
		Engine.AddStateType("Moving",			new MovingState);
		Engine.AddStateType("Jumping",			new JumpingState(45'000.f));
		Engine.AddStateType("Slamming",			new SlammingState(LANDING, 100'000.f, 5.f));
		Engine.AddStateType("SlamCharging",		new SlamChargingState(1.f, "Slamming"));

		Engine.AddGlobalState("Global");

		//Player1
		Engine.GetStateType("Global")->GetInput("").AddKeyboardInput(VK_ESCAPE, "Zoom");

		Engine.GetStateType("Idle")->GetInput("Player").AddKeyboardInput('W', "StartMoving");
		Engine.GetStateType("Idle")->GetInput("Player").AddKeyboardInput('S', "StartMoving");
		Engine.GetStateType("Idle")->GetInput("Player").AddKeyboardInput('A', "StartMoving");
		Engine.GetStateType("Idle")->GetInput("Player").AddKeyboardInput('D', "StartMoving");
		Engine.GetStateType("Idle")->GetInput("Player").AddKeyboardInput(VK_SPACE, "StartJumping");

		Engine.GetStateType("Moving")->GetInput("Player").AddKeyboardInput('W', "MoveUp");
		Engine.GetStateType("Moving")->GetInput("Player").AddKeyboardInput('S', "MoveDown");
		Engine.GetStateType("Moving")->GetInput("Player").AddKeyboardInput('A', "MoveLeft");
		Engine.GetStateType("Moving")->GetInput("Player").AddKeyboardInput('D', "MoveRight");
		Engine.GetStateType("Moving")->GetInput("Player").AddKeyboardInput(VK_SPACE, "StartJumping");

		Engine.GetStateType("Jumping")->GetInput("Player").AddKeyboardInput(VK_RETURN, "StartSlamming");
	}

	//Actors & Visuals
	{
		Engine.AddActor("Player", "Idle");
		Engine.AddActor("Player2", "Idle");

		float HeadSize = 0.75f;
		float BodySize = 0.375f;

		Engine.GetActorGraphics("Player").Head.SetTexID(ISAAC_HEAD);
		Engine.GetActorGraphics("Player").Body.SetTexID(ISAAC_BODY);
		Engine.GetActorGraphics("Player").Body.SetFrameRate(20);
		Engine.GetActorGraphics("Player").Head.SetSize({ HeadSize, HeadSize });
		Engine.GetActorGraphics("Player").Body.SetSize({ BodySize, BodySize });
		Engine.GetActorGraphics("Player").SetSpriteOffset(BodySize* 0.5f + HeadSize * 0.5f, BodySize*0.5f);
		Engine.GetActorGraphics("Player").Head.SetSpriteInfo({ 0, 0, 2,  4 });
		Engine.GetActorGraphics("Player").Body.SetSpriteInfo({ 0, 0, 10, 4 });
		Engine.GetActorGraphics("Player").Head.SetDirection(1);

		coll[0] = new ActorCollision;
		coll[1] = new ActorCollision;
		Engine.GetActorPhysics("Player").SetFriction(1.f);
		Engine.GetActorPhysics("Player").SetMass(70.f);
		Engine.GetActorPhysics("Player").GetBox().SetDimensions({ 0.5f, 0.1f, 0.75f });
		Engine.GetActorPhysics("Player").SetCollision(coll[0]);

		Engine.GetActorGraphics("Player2").Head.SetTexID(ISAAC_HEAD);
		Engine.GetActorGraphics("Player2").Body.SetTexID(ISAAC_BODY);
		Engine.GetActorGraphics("Player2").Body.SetFrameRate(20);
		Engine.GetActorGraphics("Player2").Head.SetSize({ HeadSize, HeadSize });
		Engine.GetActorGraphics("Player2").Body.SetSize({ BodySize, BodySize });
		Engine.GetActorGraphics("Player2").SetSpriteOffset(BodySize* 0.5f + HeadSize * 0.5f, BodySize*0.5f);
		Engine.GetActorGraphics("Player2").Head.SetSpriteInfo({ 0, 0, 2,  4 });
		Engine.GetActorGraphics("Player2").Body.SetSpriteInfo({ 0, 0, 10, 4 });
		Engine.GetActorGraphics("Player2").Head.SetDirection(1);
		Engine.GetActorPhysics("Player2").SetPosition({ 0.f, 2.f, 0.f });
		Engine.GetActorPhysics("Player2").SetFriction(1.f);
		Engine.GetActorPhysics("Player2").SetMass(70.f);
		Engine.GetActorPhysics("Player2").GetBox().SetDimensions({ 0.5f, 0.1f, 0.75f });
		Engine.GetActorPhysics("Player2").SetCollision(coll[1]);

	

		/* Visuals */
		Engine.AddVisual("Map", PHYSICAL_UNITS | BACK_DRAW);
		Engine.GetVisualGraphics("Map").SetTexID(BACKGROUND);
		Engine.GetVisualGraphics("Map").SetSize(20.f, 20.f);


		//Map Walls
		for (int i = 0; i < 2; ++i)
		{
			collwall[i + 2] = new BaseCollision;
			u_int Walls = Engine.AddObject();
			int j = i * 2.f - 1.f;
			Engine.GetObjectPhysics(Walls).SetCollision(collwall[i + 2]);
			Engine.GetObjectPhysics(Walls).SetPosition({ j*10.f, 0.f, 0.f });
			Engine.GetObjectPhysics(Walls).GetBox().SetDimensions({ 1.5f, 17.95f, 2.f });
		}
		for (int i = 0; i < 2; ++i)
		{
			collwall[i] = new BaseCollision;
			u_int Walls = Engine.AddObject();
			int j = i * 2.f - 1.f;
			Engine.GetObjectPhysics(Walls).SetCollision(collwall[i]);
			Engine.GetObjectPhysics(Walls).SetPosition({ 0.f, j*10.f, 0.f });
			Engine.GetObjectPhysics(Walls).GetBox().SetDimensions({ 17.95f, 3.125, 2.f });
		}
		
		
	}
}

void Gameplay::Exit()
{
	delete coll[0];
	delete coll[1];
	delete collwall[0];
	delete collwall[1];
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
