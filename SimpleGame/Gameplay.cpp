#include "stdafx.h"
#include "Gameplay.h"

void Gameplay::AddActor(const STD string& ActorName, size_t Team,  SSE_VECTOR Position, 
	const STD string& HeadTex, const STD string& BodyTex, BasicCollision* pCollision)
{
	Engine.AddObject(ActorName);

	IDType& Actor = Engine.LocateObject(ActorName);

	auto& ActorDescriptor = Engine.GetDescriptor(Actor);
	auto& ActorPhysics = Engine.GetPhysics(Actor);

	ActorDescriptor.Type = ObjectType::Actor;
	ActorDescriptor.Value = 100.f; // 100 HP
	ActorDescriptor.Team = Team;

	ActorPhysics.SetCollision(pCollision);
	ActorPhysics.Box().SetDimensions({ 0.5f, 0.5f, 1.5f });
	ActorPhysics.SetPosition(Position);

	Engine.AddSprite(Actor, "Shadow");
	Engine.AddSprite(Actor, "Body");
	Engine.AddSprite(Actor, "Head");


	auto& Shadow = Engine.GetSprite(Actor, "Shadow");
	auto& Body = Engine.GetSprite(Actor, "Body");
	auto& Head = Engine.GetSprite(Actor, "Head");

	ActorPhysics.SetMass(70.f);

	float HeadSize = 1.25f;
	float BodySize = 0.75f;

	Shadow.SetTexture("Shadow");
	Shadow.SetLayerGroup(LayerGroup::Background);
	Shadow.SetSize({ HeadSize, HeadSize });

	Body.SetTexture(BodyTex);
	Body.SetSize({ BodySize, BodySize });
	Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
	Body.SetTotal({ 10, 4 });
	Body.SetDirection(Direction::Down);

	Head.SetTexture(HeadTex);
	Head.SetSize({ HeadSize, HeadSize });
	Head.SetTotal({ 2, 4 });
	Head.SetDirection(Direction::Down);
	Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
}

void Gameplay::Enter()
{
	Engine.AddTexture("BasicBody", "./Resources/Characters/basic_body.png");
	Engine.AddTexture("IsaacHead", "./Resources/Characters/isaac_head.png");

	Engine.AddTexture("Shadow", "./Resources/shadow.png");

	Engine.AddTexture("ZombieBody", "./Resources/Characters/zombie_body.png");
	Engine.AddTexture("ZombieHead", "./Resources/Characters/zombie_head.png");

	Engine.AddTexture("Depths", "./Resources/Levels/Depths.png");
	Engine.AddTexture("Explosion", "./Resources/explosion.png");
	Engine.AddTexture("Tear", "./Resources/tear.png");

	Engine.AddSound("Main", "./Resources/Sounds/Main.mp3", TRUE);

	//Map
	{
		size_t DEPTHS =	Engine.AddObject();
		Engine.AddSprite(Engine.LocateObject(DEPTHS), "Depths");

		Sprite& Map = Engine.GetSprite(Engine.LocateObject(DEPTHS), "Depths");
		
		Map.SetSize({20.f, 20.f });
		Map.SetTexture("Depths");
		Map.SetLayerGroup(LayerGroup::Background);
	}

	//Engine.GetSound(SOUND_TEST).Play();
	AddActor("Player", 0, { 0.f, 0.f, 0.f }, "IsaacHead", "BasicBody", &Collision::Actor);
	AddActor("TestMonster", 1, { 0.f, 3.f, 0.f }, "ZombieHead", "ZombieBody", &Collision::Monster);


	//Actor States
	{
		Engine.AddStatePrototype<IdleState>("Idle");
		Engine.AddStatePrototype<MoveState>("Move");
		Engine.AddStatePrototype<InAirState>("InAir", 0.0f);
		Engine.AddStatePrototype<ChargeJumpState>("ChargeJump", 1.f, 40'000.f);
		Engine.AddStatePrototype<ChargeSlamState>("ChargeSlam", 1.f);
		Engine.AddStatePrototype<SlamState>("Slam", 200'000.f);
		Engine.AddStatePrototype<ShootState>("Shoot", 5.f, 1'000.f);
		Engine.AddStatePrototype<DamagedState>("Damaged", 2.f, 10.f);
	}

	//Commands
	{
		float Move = 1'500;
		Engine.AddCommand<ForceCommand>("MoveUp"   , 0.f, Move, 0.f);
		Engine.AddCommand<ForceCommand>("MoveDown" , 0.f, -Move, 0.f);
		Engine.AddCommand<ForceCommand>("MoveLeft" , -Move, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>("MoveRight", Move, 0.f, 0.f);

		float SlowMove = 750.f;
		Engine.AddCommand<ForceCommand>("SlowMoveUp"   , 0.f, SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveDown"  , 0.f, -SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveLeft" , -SlowMove, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveRight", SlowMove, 0.f, 0.f);

		Engine.AddCommand<StateCommand>("StartMove", "Move");
		Engine.AddCommand<StateCommand>("StartChargeJump", "ChargeJump");
		Engine.AddCommand<StateCommand>("StartChargeSlam", "ChargeSlam");
		Engine.AddCommand<StateCommand>("StartInAir", "InAir", ST_CMD::ON_RELEASE | ST_CMD::CHANGE_STATE);
		Engine.AddCommand<StateCommand>("StartShoot", "Shoot", ST_CMD::ON_PRESS | ST_CMD::PUSH_STATE);
		Engine.AddCommand<StateCommand>("EndShoot", "", ST_CMD::ON_RELEASE | ST_CMD::POP_STATE);

		Engine.AddCommand<FaceCommand>("HeadFaceUp"   , "Head", Direction::Up);
		Engine.AddCommand<FaceCommand>("HeadFaceDown" , "Head", Direction::Down);
		Engine.AddCommand<FaceCommand>("HeadFaceLeft" , "Head", Direction::Left);
		Engine.AddCommand<FaceCommand>("HeadFaceRight", "Head", Direction::Right);

		Engine.AddCommand<FaceCommand>("BodyFaceUp"		, "Body", Direction::Up);
		Engine.AddCommand<FaceCommand>("BodyFaceDown"	, "Body", Direction::Down);
		Engine.AddCommand<FaceCommand>("BodyFaceLeft"	, "Body", Direction::Left);
		Engine.AddCommand<FaceCommand>("BodyFaceRight"	, "Body", Direction::Right);
	}

	//Actor Input
	{
		/* Adding the Inputs to Player */
		IDType& Player = Engine.LocateObject("Player");
		IDType& Monster = Engine.LocateObject("TestMonster");

		Engine.AddController(Player, "Idle");
		Engine.AddController(Player, "Move");
		Engine.AddController(Player, "ChargeJump");
		Engine.AddController(Player, "InAir");
		Engine.AddController(Player, "Shoot");

		Controller& IdleInput = Engine.GetController(Player, "Idle");

		IdleInput.MapControl('W', "StartMove");
		IdleInput.MapControl('A', "StartMove");
		IdleInput.MapControl('S', "StartMove");
		IdleInput.MapControl('D', "StartMove");

		IdleInput.MapControl(VK_SPACE, "StartChargeJump");
		IdleInput.MapControl(VK_RIGHT, "StartShoot");
		IdleInput.MapControl(VK_LEFT,  "StartShoot");
		IdleInput.MapControl(VK_UP,    "StartShoot");
		IdleInput.MapControl(VK_DOWN,  "StartShoot");
		
		Controller&  MoveInput = Engine.GetController(Player, "Move");

		MoveInput.MapControl('W', "MoveUp");
		MoveInput.MapControl('S', "MoveDown");
		MoveInput.MapControl('A', "MoveLeft");
		MoveInput.MapControl('D', "MoveRight");

		MoveInput.MapControl('W', "HeadFaceUp");
		MoveInput.MapControl('S', "HeadFaceDown");
		MoveInput.MapControl('A', "HeadFaceLeft");
		MoveInput.MapControl('D', "HeadFaceRight");

		MoveInput.MapControl('W', "BodyFaceUp");
		MoveInput.MapControl('S', "BodyFaceDown");
		MoveInput.MapControl('A', "BodyFaceLeft");
		MoveInput.MapControl('D', "BodyFaceRight");

		MoveInput.MapControl(VK_SPACE, "StartChargeJump");
		MoveInput.MapControl(VK_RIGHT,	"StartShoot");
		MoveInput.MapControl(VK_LEFT,	"StartShoot");
		MoveInput.MapControl(VK_UP,		"StartShoot");
		MoveInput.MapControl(VK_DOWN,	"StartShoot");
		
		Controller&  ChargeJump = Engine.GetController(Player, "ChargeJump");

		ChargeJump.MapControl('W', "SlowMoveUp");
		ChargeJump.MapControl('S', "SlowMoveDown");
		ChargeJump.MapControl('A', "SlowMoveLeft");
		ChargeJump.MapControl('D', "SlowMoveRight");

		ChargeJump.MapControl('W', "BodyFaceUp");
		ChargeJump.MapControl('S', "BodyFaceDown");
		ChargeJump.MapControl('A', "BodyFaceLeft");
		ChargeJump.MapControl('D', "BodyFaceRight");

		ChargeJump.MapControl('W', "HeadFaceUp");
		ChargeJump.MapControl('S', "HeadFaceDown");
		ChargeJump.MapControl('A', "HeadFaceLeft");
		ChargeJump.MapControl('D', "HeadFaceRight");
		ChargeJump.MapControl(VK_SPACE, "StartInAir");
		
		Controller&  InAirInput = Engine.GetController(Player, "InAir");

		InAirInput.MapControl(VK_SPACE, "StartChargeSlam");
		
		Controller& ShootInput = Engine.GetController(Player, "Shoot");

		ShootInput.MapControl('W', "MoveUp");
		ShootInput.MapControl('S', "MoveDown");
		ShootInput.MapControl('A', "MoveLeft");
		ShootInput.MapControl('D', "MoveRight");;

		ShootInput.MapControl('W', "BodyFaceUp");
		ShootInput.MapControl('S', "BodyFaceDown");
		ShootInput.MapControl('A', "BodyFaceLeft");
		ShootInput.MapControl('D', "BodyFaceRight");

		ShootInput.MapControl(VK_RIGHT, "HeadFaceRight");
		ShootInput.MapControl(VK_LEFT,  "HeadFaceLeft");
		ShootInput.MapControl(VK_UP,	"HeadFaceUp");
		ShootInput.MapControl(VK_DOWN,  "HeadFaceDown");

		ShootInput.MapControl(VK_RIGHT, "EndShoot");
		ShootInput.MapControl(VK_LEFT,	"EndShoot");
		ShootInput.MapControl(VK_UP,	"EndShoot");
		ShootInput.MapControl(VK_DOWN,	"EndShoot");
		
		Engine.ChangeState(Player, "Idle");
		Engine.ChangeState(Monster, "Idle");
	}

	//Boundaries
	{

		float Min = 15.f;
		float Max = 30.f;
		{
			size_t Boundary;
			Boundary = Engine.AddObject();
			IDType& BoundaryID = Engine.LocateObject(Boundary);
			Engine.GetDescriptor(BoundaryID).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(BoundaryID);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ 16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Boundary = Engine.AddObject();
			IDType& BoundaryID = Engine.LocateObject(Boundary);
			Engine.GetDescriptor(BoundaryID).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(BoundaryID);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ -16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Boundary = Engine.AddObject();
			IDType& BoundaryID = Engine.LocateObject(Boundary);
			Engine.GetDescriptor(BoundaryID).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(BoundaryID);
			BPhysics.Box().SetDimensions({ Max, Min, Max });
			BPhysics.SetPosition({ 0.f, 16.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Boundary = Engine.AddObject();
			IDType& BoundaryID = Engine.LocateObject(Boundary);
			Engine.GetDescriptor(BoundaryID).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(BoundaryID);
			BPhysics.Box().SetDimensions({ Max, Min, 10.f });
			BPhysics.SetPosition({ 0.f, -16.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

	}
}

void Gameplay::Exit()
{
	Engine.DeleteComponents();
}