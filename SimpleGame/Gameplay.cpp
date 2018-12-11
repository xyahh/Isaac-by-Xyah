#include "stdafx.h"
#include "Gameplay.h"

void Gameplay::Enter()
{
	Engine.AddTexture(&TEX::BASIC_BODY, "./Resources/Characters/basic_body.png");
	Engine.AddTexture(&TEX::ISAAC_HEAD, "./Resources/Characters/cain_head.png");
	Engine.AddTexture(&TEX::EXPLOSION, "./Resources/explosion.png");
	Engine.AddTexture(&TEX::TEAR, "./Resources/tear.png");
	size_t SOUND_TEST;
	Engine.AddSound(&SOUND_TEST, "./Resources/Sounds/Main.mp3", true);
	//Engine.GetSound(SOUND_TEST).Play();

	//Actor
	{
		Engine.AddObject(&OBJ::PLAYER);
	
		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER);

		ActorDescriptor.Type = ObjectType::Actor;
		ActorDescriptor.Value = 100.f; // 100 HP
		ActorDescriptor.Team = OBJ::PLAYER;

		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({0.5f, 0.25f, 1.5f});
		ActorPhysics.SetPosition({ 4.f, 4.f, 0.f });

		

		Engine.AddSprite(&OBJ::SPRITE::BODY, OBJ::PLAYER);
		Engine.AddSprite(&OBJ::SPRITE::HEAD, OBJ::PLAYER);

		auto& Body = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::BODY);
		auto& Head = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::HEAD);

		ActorPhysics.SetMass(70.f);
		
		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(TEX::BASIC_BODY);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);

		Head.SetTexture(TEX::ISAAC_HEAD);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	}

	size_t PLAYER2;

	//Actor
	{
		Engine.AddObject(&PLAYER2);

		auto& ActorDescriptor = Engine.GetDescriptor(PLAYER2);
		auto& ActorPhysics = Engine.GetPhysics(PLAYER2);
		auto& ActorGraphics = Engine.GetGraphics(PLAYER2);

		ActorDescriptor.Type = ObjectType::Actor;
		ActorDescriptor.Value = 100.f; // 100 HP
		ActorDescriptor.Team = PLAYER2;
		ActorDescriptor.AddEvent(DescriptorEvent::ValueZeroNegative, [PLAYER2]()
		{
			STD cout << "Player2 Died!\n";
			Engine.DeleteObject(PLAYER2);
		});

		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({ 0.5f, 0.25f, 1.5f });
		ActorPhysics.SetFriction(1.f);

		size_t BODY;
		size_t HEAD;

		Engine.AddSprite(&BODY, PLAYER2);
		Engine.AddSprite(&HEAD, PLAYER2);

		auto& Body = Engine.GetSprite(PLAYER2, BODY);
		auto& Head = Engine.GetSprite(PLAYER2, HEAD);

		ActorPhysics.SetMass(70.f);

		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(TEX::BASIC_BODY);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);

		Head.SetTexture(TEX::ISAAC_HEAD);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	}

	//Actor States
	{
		Engine.AddStatePrototype<IdleState>(&ST::IDLE);
		Engine.AddStatePrototype<MoveState>(&ST::MOVE);
		Engine.AddStatePrototype<InAirState>(&ST::IN_AIR, 0.0f);
		Engine.AddStatePrototype<ChargeJumpState>(&ST::CHARGE_JUMP, 1.f, 40'000.f);
		Engine.AddStatePrototype<ChargeSlamState>(&ST::CHARGE_SLAM, 1.f);
		Engine.AddStatePrototype<SlamState>(&ST::SLAM, 200'000.f);
		Engine.AddStatePrototype<ShootState>(&ST::SHOOT, TEX::TEAR, 5.f, 2'000.f);
		Engine.AddStatePrototype<DamagedState>(&ST::DAMAGED, 2.f, 10.f);
	}

	//Commands
	{
		float Move = 1'500.f;
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

		Engine.AddCommand<FaceCommand>(&CMD::FACE_UP,  OBJ::SPRITE::HEAD, Direction::Up);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_DOWN, OBJ::SPRITE::HEAD, Direction::Down);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_LEFT, OBJ::SPRITE::HEAD, Direction::Left);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_RIGHT, OBJ::SPRITE::HEAD, Direction::Right);
	}

	//Actor Input
	{
		/* Adding the Inputs to Player */
		Engine.AddController(OBJ::PLAYER, ST::IDLE);
		Engine.AddController(OBJ::PLAYER, ST::MOVE);
		Engine.AddController(OBJ::PLAYER, ST::CHARGE_JUMP);
		Engine.AddController(OBJ::PLAYER, ST::IN_AIR);
		Engine.AddController(OBJ::PLAYER, ST::SHOOT);

		Engine.AddController(PLAYER2, ST::IDLE);

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

		MoveInput.MapControl('W', CMD::FACE_UP);
		MoveInput.MapControl('A', CMD::FACE_LEFT);
		MoveInput.MapControl('S', CMD::FACE_DOWN);
		MoveInput.MapControl('D', CMD::FACE_RIGHT);

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

		ChargeJump.MapControl('W', CMD::FACE_UP);
		ChargeJump.MapControl('A', CMD::FACE_LEFT);
		ChargeJump.MapControl('S', CMD::FACE_DOWN);
		ChargeJump.MapControl('D', CMD::FACE_RIGHT);

		ChargeJump.MapControl(VK_SPACE, CMD::START_IN_AIR);
		
		Controller&  InAirInput = Engine.GetController(OBJ::PLAYER, ST::IN_AIR);
		InAirInput.MapControl(VK_SPACE, CMD::START_CHARGE_SLAM);
		
		Controller& ShootInput = Engine.GetController(OBJ::PLAYER, ST::SHOOT);
		ShootInput.MapControl('W', CMD::MOVE_UP);
		ShootInput.MapControl('A', CMD::MOVE_LEFT);
		ShootInput.MapControl('S', CMD::MOVE_DOWN);
		ShootInput.MapControl('D', CMD::MOVE_RIGHT);

		ShootInput.MapControl(VK_RIGHT, CMD::FACE_RIGHT);
		ShootInput.MapControl(VK_LEFT,	CMD::FACE_LEFT);
		ShootInput.MapControl(VK_UP,	CMD::FACE_UP);
		ShootInput.MapControl(VK_DOWN,	CMD::FACE_DOWN);

		ShootInput.MapControl(VK_RIGHT, CMD::END_SHOOT);
		ShootInput.MapControl(VK_LEFT,	CMD::END_SHOOT);
		ShootInput.MapControl(VK_UP,	CMD::END_SHOOT);
		ShootInput.MapControl(VK_DOWN,	CMD::END_SHOOT);
		
		

		Engine.ChangeState(OBJ::PLAYER, ST::IDLE);
		Engine.ChangeState(PLAYER2, ST::IDLE);
	}

	//Boundaries
	{

		float Min = 12.f;
		float Max = 30.f;
		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, 10.f });
			BPhysics.SetPosition({ 16.f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, 10.f });
			BPhysics.SetPosition({ -16.f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			size_t Boundary;
			Engine.AddObject(&Boundary);
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Max, Min, 10.f });
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