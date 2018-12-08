#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"
//#include "Menu.h"

void Gameplay::Init()
{
	Engine.AddTexture(&TEX::BASIC_BODY, "./Resources/Characters/basic_body.png");
	Engine.AddTexture(&TEX::ISAAC_HEAD, "./Resources/Characters/cain_head.png");
	Engine.AddTexture(&TEX::EXPLOSION, "./Resources/explosion.png");

	size_t SOUND_TEST;
	Engine.AddSound(&SOUND_TEST, "./Resources/Sounds/Main.mp3", true);
	Engine.GetSound(SOUND_TEST).Play();

	//Actor
	{
		
		Engine.AddObject(&OBJ::PLAYER);
	
		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER);

		ActorDescriptor.SetType(ObjectType::Actor);
		ActorDescriptor.SetValue(100.f); // 100 HP
		
		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({0.5f, 0.25f, 1.5f});
		ActorPhysics.SetPosition({ 4.f, 4.f, 0.f });

		Engine.AddSprite(&OBJ::SPRITE::BODY, OBJ::PLAYER);
		Engine.AddSprite(&OBJ::SPRITE::HEAD, OBJ::PLAYER);

		auto& Body = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::BODY);
		auto& Head = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::HEAD);

		ActorGraphics.SetColor(1.f, 1.f, 1.f, 1.f);
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

	//size_t PLAYER2;

	////Actor
	//{

	//	Engine.AddObject(&PLAYER2);

	//	auto& ActorDescriptor = Engine.GetDescriptor(PLAYER2);
	//	auto& ActorPhysics = Engine.GetPhysics(PLAYER2);
	//	auto& ActorGraphics = Engine.GetGraphics(PLAYER2);

	//	ActorDescriptor.SetType(ObjectType::Actor);
	//	ActorDescriptor.SetValue(100.f); // 100 HP

	//	ActorPhysics.SetCollision(&Collision::Actor);
	//	ActorPhysics.Box().SetDimensions({ 0.5f, 0.25f, 1.5f });

	//	size_t BODY;
	//	size_t HEAD;

	//	Engine.AddSprite(&BODY, PLAYER2);
	//	Engine.AddSprite(&HEAD, PLAYER2);

	//	auto& Body = Engine.GetSprite(PLAYER2, BODY);
	//	auto& Head = Engine.GetSprite(PLAYER2, HEAD);

	//	ActorGraphics.SetColor(1.f, 1.f, 1.f, 1.f);
	//	ActorPhysics.SetMass(70.f);

	//	float HeadSize = 1.25f;
	//	float BodySize = 0.75f;

	//	Body.SetTexture(TEX::BASIC_BODY);
	//	Body.SetSize({ BodySize, BodySize });
	//	Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
	//	Body.SetTotal({ 10, 4 });
	//	Body.SetDirection(Direction::Down);

	//	Head.SetTexture(TEX::ISAAC_HEAD);
	//	Head.SetSize({ HeadSize, HeadSize });
	//	Head.SetTotal({ 2, 4 });
	//	Head.SetDirection(Direction::Down);
	//	Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	//}


	//Actor States
	{
		Engine.AddStatePrototype<IdleState>(&ST::IDLE);
		Engine.AddStatePrototype<MoveState>(&ST::MOVE);
		Engine.AddStatePrototype<InAirState>(&ST::IN_AIR, 0.0f);
		Engine.AddStatePrototype<ChargeJumpState>(&ST::CHARGE_JUMP, 1.f, 20'000.f);
		Engine.AddStatePrototype<ChargeSlamState>(&ST::CHARGE_SLAM, 1.f);
		Engine.AddStatePrototype<SlamState>(&ST::SLAM, 200'000.f);
		Engine.AddStatePrototype<ShootState>(&ST::SHOOT, 5.f, 100.f);
	}

	//size_t TEST;
	//Commands
	{
		//Engine.AddCommand<SceneCommand<Menu>>(&TEST);

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
		Engine.AddObjectState(OBJ::PLAYER, ST::IDLE);
		Engine.AddObjectState(OBJ::PLAYER, ST::MOVE);
		Engine.AddObjectState(OBJ::PLAYER, ST::CHARGE_JUMP);
		Engine.AddObjectState(OBJ::PLAYER, ST::IN_AIR);
		Engine.AddObjectState(OBJ::PLAYER, ST::SHOOT);

		//Engine.AddObjectState(PLAYER2, ST::IDLE);

		//Input& IdleInput = Engine.GetStateInput(OBJ::PLAYER, ST::IDLE);
		//IdleInput.AddKeyMapping('W', CMD::START_MOVE);
		//IdleInput.AddKeyMapping('A', CMD::START_MOVE);
		//IdleInput.AddKeyMapping('S', CMD::START_MOVE);
		//IdleInput.AddKeyMapping('D', CMD::START_MOVE);
		//IdleInput.AddKeyMapping(VK_SPACE, CMD::START_CHARGE_JUMP);
		//IdleInput.AddKeyMapping(VK_RIGHT, CMD::START_SHOOT);
		//IdleInput.AddKeyMapping(VK_LEFT, CMD::START_SHOOT);
		//IdleInput.AddKeyMapping(VK_UP, CMD::START_SHOOT);
		//IdleInput.AddKeyMapping(VK_DOWN, CMD::START_SHOOT);
		//
		//Input&  MoveInput = Engine.GetStateInput(OBJ::PLAYER, ST::MOVE);
		//MoveInput.AddKeyMapping('W', CMD::MOVE_UP);
		//MoveInput.AddKeyMapping('A', CMD::MOVE_LEFT);
		//MoveInput.AddKeyMapping('S', CMD::MOVE_DOWN);
		//MoveInput.AddKeyMapping('D', CMD::MOVE_RIGHT);
		//MoveInput.AddKeyMapping(VK_SPACE, CMD::START_CHARGE_JUMP);
		//MoveInput.AddKeyMapping(VK_RIGHT, CMD::START_SHOOT);
		//MoveInput.AddKeyMapping(VK_LEFT, CMD::START_SHOOT);
		//MoveInput.AddKeyMapping(VK_UP, CMD::START_SHOOT);
		//MoveInput.AddKeyMapping(VK_DOWN, CMD::START_SHOOT);
		//
		//Input&  ChargeJump = Engine.GetStateInput(OBJ::PLAYER, ST::CHARGE_JUMP);
		//ChargeJump.AddKeyMapping('W', CMD::SLOW_MOVE_UP);
		//ChargeJump.AddKeyMapping('A', CMD::SLOW_MOVE_LEFT);
		//ChargeJump.AddKeyMapping('S', CMD::SLOW_MOVE_DOWN);
		//ChargeJump.AddKeyMapping('D', CMD::SLOW_MOVE_RIGHT);
		//ChargeJump.AddKeyMapping(VK_SPACE, CMD::START_IN_AIR);
		//
		//Input&  InAirInput = Engine.GetStateInput(OBJ::PLAYER, ST::IN_AIR);
		//InAirInput.AddKeyMapping(VK_SPACE, CMD::START_CHARGE_SLAM);
		//
		//Input& ShootInput = Engine.GetStateInput(OBJ::PLAYER, ST::SHOOT);
		//ShootInput.AddKeyMapping('W', CMD::MOVE_UP);
		//ShootInput.AddKeyMapping('A', CMD::MOVE_LEFT);
		//ShootInput.AddKeyMapping('S', CMD::MOVE_DOWN);
		//ShootInput.AddKeyMapping('D', CMD::MOVE_RIGHT);
		//ShootInput.AddKeyMapping(VK_RIGHT,CMD::FACE_RIGHT);
		//ShootInput.AddKeyMapping(VK_LEFT,	CMD::FACE_LEFT);
		//ShootInput.AddKeyMapping(VK_UP,	CMD::FACE_UP);
		//ShootInput.AddKeyMapping(VK_DOWN,	CMD::FACE_DOWN);
		//ShootInput.AddKeyMapping(VK_RIGHT,CMD::END_SHOOT);
		//ShootInput.AddKeyMapping(VK_LEFT,	CMD::END_SHOOT);
		//ShootInput.AddKeyMapping(VK_UP,	CMD::END_SHOOT);
		//ShootInput.AddKeyMapping(VK_DOWN,	CMD::END_SHOOT);
		
		

		Engine.ChangeState(OBJ::PLAYER, ST::IDLE);
	}


	//Input& IdleInput = Engine.GetStateInput(PLAYER2, ST::IDLE);
	//IdleInput.AddKeyMapping('I', CMD::START_MOVE);
	//IdleInput.AddKeyMapping('J', CMD::START_MOVE);
	//IdleInput.AddKeyMapping('K', CMD::START_MOVE);
	//IdleInput.AddKeyMapping('L', CMD::START_MOVE);
	//
	//
	//Input&  MoveInput = Engine.GetStateInput(PLAYER2, ST::MOVE);
	//MoveInput.AddKeyMapping('I', CMD::MOVE_UP);
	//MoveInput.AddKeyMapping('J', CMD::MOVE_LEFT);
	//MoveInput.AddKeyMapping('K', CMD::MOVE_DOWN);
	//MoveInput.AddKeyMapping('L', CMD::MOVE_RIGHT);
	//
	//Engine.ChangeState(PLAYER2, ST::IDLE);
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
	Engine.Update();
}
