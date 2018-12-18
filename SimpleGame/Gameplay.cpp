#include "stdafx.h"
#include "Gameplay.h"

void Gameplay::AddActor(size_t * ID, size_t Team,  SSE_VECTOR Position, size_t HeadTex, size_t BodyTex, BasicCollision* pCollision)
{
	Engine.AddObject(ID);

	auto& ActorDescriptor = Engine.GetDescriptor(*ID);
	auto& ActorPhysics = Engine.GetPhysics(*ID);

	ActorDescriptor.Type = ObjectType::Actor;
	ActorDescriptor.Value = 100.f; // 100 HP
	ActorDescriptor.Team = Team;

	ActorPhysics.SetCollision(pCollision);
	ActorPhysics.Box().SetDimensions({ 0.5f, 0.5f, 1.5f });
	ActorPhysics.SetPosition(Position);

	Engine.AddSprite(&OBJ::SPRITE::BODY, *ID);
	Engine.AddSprite(&OBJ::SPRITE::HEAD, *ID);

	auto& Body = Engine.GetSprite(*ID, OBJ::SPRITE::BODY);
	auto& Head = Engine.GetSprite(*ID, OBJ::SPRITE::HEAD);

	ActorPhysics.SetMass(70.f);

	float HeadSize = 1.25f;
	float BodySize = 0.75f;

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
	size_t SOUND_TEST;
	
	size_t ZOMBIE_HEAD, ZOMBIE_BODY;
	Engine.AddTexture(&TEX::BASIC_BODY, "./Resources/Characters/basic_body.png");
	Engine.AddTexture(&TEX::ISAAC_HEAD, "./Resources/Characters/isaac_head.png");

	Engine.AddTexture(&ZOMBIE_BODY, "./Resources/Characters/zombie_body.png");
	Engine.AddTexture(&ZOMBIE_HEAD, "./Resources/Characters/zombie_head.png");

	Engine.AddTexture(&TEX::DEPTHS, "./Resources/Levels/Depths.png");
	Engine.AddTexture(&TEX::EXPLOSION, "./Resources/explosion.png");
	Engine.AddTexture(&TEX::TEAR, "./Resources/tear.png");
	Engine.AddSound(&SOUND_TEST, "./Resources/Sounds/Main.mp3", TRUE);

	//Map
	{
		size_t DEPTHS;
		Engine.AddObject(&DEPTHS);
		size_t MAP;
		Engine.AddSprite(&MAP, DEPTHS);
		Sprite& Map = Engine.GetSprite(DEPTHS, MAP);
		Map.SetSize({ 20.f, 20.f });
		Map.SetTexture(TEX::DEPTHS);
		Map.SetLayerGroup(LayerGroup::Background);
	}

	size_t TEST_MONSTER;

	//Engine.GetSound(SOUND_TEST).Play();
	AddActor(&OBJ::PLAYER, 0, { 0.f, 0.f, 0.f }, TEX::ISAAC_HEAD, TEX::BASIC_BODY, &Collision::Actor);
	AddActor(&TEST_MONSTER, 1, { 0.f, 3.f, 0.f }, ZOMBIE_HEAD, ZOMBIE_BODY, &Collision::Monster);


	//Actor States
	{
		Engine.AddStatePrototype<IdleState>(&ST::IDLE);
		Engine.AddStatePrototype<MoveState>(&ST::MOVE);
		Engine.AddStatePrototype<InAirState>(&ST::IN_AIR, 0.0f);
		Engine.AddStatePrototype<ChargeJumpState>(&ST::CHARGE_JUMP, 1.f, 40'000.f);
		Engine.AddStatePrototype<ChargeSlamState>(&ST::CHARGE_SLAM, 1.f);
		Engine.AddStatePrototype<SlamState>(&ST::SLAM, 200'000.f);
		Engine.AddStatePrototype<ShootState>(&ST::SHOOT, TEX::TEAR, 5.f, 1'000.f);
		Engine.AddStatePrototype<DamagedState>(&ST::DAMAGED, 2.f, 10.f);
	}

	//Commands
	{
		float Move = 1'500;
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_UP, 0.f, Move, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_DOWN, 0.f, -Move, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_LEFT, -Move, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_RIGHT, Move, 0.f, 0.f);

		float SlowMove = 750.f;
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_UP, 0.f, SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_DOWN, 0.f, -SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_LEFT, -SlowMove, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_RIGHT, SlowMove, 0.f, 0.f);

		Engine.AddCommand<StateCommand>(&CMD::START_MOVE, ST::MOVE);
		Engine.AddCommand<StateCommand>(&CMD::START_CHARGE_JUMP, ST::CHARGE_JUMP);
		Engine.AddCommand<StateCommand>(&CMD::START_CHARGE_SLAM, ST::CHARGE_SLAM);
		Engine.AddCommand<StateCommand>(&CMD::START_IN_AIR, ST::IN_AIR, ST_CMD::ON_RELEASE | ST_CMD::CHANGE_STATE);
		Engine.AddCommand<StateCommand>(&CMD::START_SHOOT, ST::SHOOT, ST_CMD::ON_PRESS | ST_CMD::PUSH_STATE);
		Engine.AddCommand<StateCommand>(&CMD::END_SHOOT, NULL, ST_CMD::ON_RELEASE | ST_CMD::POP_STATE);

		Engine.AddCommand<FaceCommand>(&CMD::HEAD_FACE_UP,  OBJ::SPRITE::HEAD, Direction::Up);
		Engine.AddCommand<FaceCommand>(&CMD::HEAD_FACE_DOWN, OBJ::SPRITE::HEAD, Direction::Down);
		Engine.AddCommand<FaceCommand>(&CMD::HEAD_FACE_LEFT, OBJ::SPRITE::HEAD, Direction::Left);
		Engine.AddCommand<FaceCommand>(&CMD::HEAD_FACE_RIGHT, OBJ::SPRITE::HEAD, Direction::Right);

		Engine.AddCommand<FaceCommand>(&CMD::BODY_FACE_UP, OBJ::SPRITE::BODY, Direction::Up);
		Engine.AddCommand<FaceCommand>(&CMD::BODY_FACE_DOWN, OBJ::SPRITE::BODY, Direction::Down);
		Engine.AddCommand<FaceCommand>(&CMD::BODY_FACE_LEFT, OBJ::SPRITE::BODY, Direction::Left);
		Engine.AddCommand<FaceCommand>(&CMD::BODY_FACE_RIGHT, OBJ::SPRITE::BODY, Direction::Right);
	}

	//Actor Input
	{
		/* Adding the Inputs to Player */
		Engine.AddController(OBJ::PLAYER, ST::IDLE);
		Engine.AddController(OBJ::PLAYER, ST::MOVE);
		Engine.AddController(OBJ::PLAYER, ST::CHARGE_JUMP);
		Engine.AddController(OBJ::PLAYER, ST::IN_AIR);
		Engine.AddController(OBJ::PLAYER, ST::SHOOT);

		Controller& IdleInput = Engine.GetController(OBJ::PLAYER, ST::IDLE);

		IdleInput.MapControl('W', CMD::START_MOVE);
		IdleInput.MapControl('A', CMD::START_MOVE);
		IdleInput.MapControl('S', CMD::START_MOVE);
		IdleInput.MapControl('D', CMD::START_MOVE);

		IdleInput.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		IdleInput.MapControl(VK_RIGHT, CMD::START_SHOOT);
		IdleInput.MapControl(VK_LEFT, CMD::START_SHOOT);
		IdleInput.MapControl(VK_UP, CMD::START_SHOOT);
		IdleInput.MapControl(VK_DOWN, CMD::START_SHOOT);
		
		Controller&  MoveInput = Engine.GetController(OBJ::PLAYER, ST::MOVE);

		MoveInput.MapControl('W', CMD::MOVE_UP);
		MoveInput.MapControl('A', CMD::MOVE_LEFT);
		MoveInput.MapControl('S', CMD::MOVE_DOWN);
		MoveInput.MapControl('D', CMD::MOVE_RIGHT);

		MoveInput.MapControl('W', CMD::HEAD_FACE_UP);
		MoveInput.MapControl('A', CMD::HEAD_FACE_LEFT);
		MoveInput.MapControl('S', CMD::HEAD_FACE_DOWN);
		MoveInput.MapControl('D', CMD::HEAD_FACE_RIGHT);

		MoveInput.MapControl('W', CMD::BODY_FACE_UP);
		MoveInput.MapControl('A', CMD::BODY_FACE_LEFT);
		MoveInput.MapControl('S', CMD::BODY_FACE_DOWN);
		MoveInput.MapControl('D', CMD::BODY_FACE_RIGHT);

		MoveInput.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		MoveInput.MapControl(VK_RIGHT, CMD::START_SHOOT);
		MoveInput.MapControl(VK_LEFT, CMD::START_SHOOT);
		MoveInput.MapControl(VK_UP, CMD::START_SHOOT);
		MoveInput.MapControl(VK_DOWN, CMD::START_SHOOT);
		
		Controller&  ChargeJump = Engine.GetController (OBJ::PLAYER, ST::CHARGE_JUMP);

		ChargeJump.MapControl('W', CMD::SLOW_MOVE_UP);
		ChargeJump.MapControl('A', CMD::SLOW_MOVE_LEFT);
		ChargeJump.MapControl('S', CMD::SLOW_MOVE_DOWN);
		ChargeJump.MapControl('D', CMD::SLOW_MOVE_RIGHT);

		ChargeJump.MapControl('W', CMD::BODY_FACE_UP);
		ChargeJump.MapControl('A', CMD::BODY_FACE_LEFT);
		ChargeJump.MapControl('S', CMD::BODY_FACE_DOWN);
		ChargeJump.MapControl('D', CMD::BODY_FACE_RIGHT);

		ChargeJump.MapControl('W', CMD::HEAD_FACE_UP);
		ChargeJump.MapControl('A', CMD::HEAD_FACE_LEFT);
		ChargeJump.MapControl('S', CMD::HEAD_FACE_DOWN);
		ChargeJump.MapControl('D', CMD::HEAD_FACE_RIGHT);

		ChargeJump.MapControl(VK_SPACE, CMD::START_IN_AIR);
		
		Controller&  InAirInput = Engine.GetController(OBJ::PLAYER, ST::IN_AIR);
		InAirInput.MapControl(VK_SPACE, CMD::START_CHARGE_SLAM);
		
		Controller& ShootInput = Engine.GetController(OBJ::PLAYER, ST::SHOOT);
		ShootInput.MapControl('W', CMD::MOVE_UP);
		ShootInput.MapControl('A', CMD::MOVE_LEFT);
		ShootInput.MapControl('S', CMD::MOVE_DOWN);
		ShootInput.MapControl('D', CMD::MOVE_RIGHT);

		ShootInput.MapControl('W', CMD::BODY_FACE_UP);
		ShootInput.MapControl('A', CMD::BODY_FACE_LEFT);
		ShootInput.MapControl('S', CMD::BODY_FACE_DOWN);
		ShootInput.MapControl('D', CMD::BODY_FACE_RIGHT);

		ShootInput.MapControl(VK_RIGHT, CMD::HEAD_FACE_RIGHT);
		ShootInput.MapControl(VK_LEFT,	CMD::HEAD_FACE_LEFT);
		ShootInput.MapControl(VK_UP,	CMD::HEAD_FACE_UP);
		ShootInput.MapControl(VK_DOWN,	CMD::HEAD_FACE_DOWN);

		ShootInput.MapControl(VK_RIGHT, CMD::END_SHOOT);
		ShootInput.MapControl(VK_LEFT,	CMD::END_SHOOT);
		ShootInput.MapControl(VK_UP,	CMD::END_SHOOT);
		ShootInput.MapControl(VK_DOWN,	CMD::END_SHOOT);
		
		Engine.ChangeState(OBJ::PLAYER, ST::IDLE);
		Engine.ChangeState(TEST_MONSTER, ST::IDLE);
	}

	//Boundaries
	{

		float Min = 15.f;
		float Max = 30.f;
		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ 16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ -16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Max, Min, Max });
			BPhysics.SetPosition({ 0.f, 16.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
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