#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"
#include "Indices.h"

void Gameplay::Init()
{
	//Actor
	{
		TEX::BASIC_BODY = Engine.AddTexture("./Resources/Characters/basic_body.png");
		TEX::ISAAC_HEAD = Engine.AddTexture("./Resources/Characters/isaac_head.png");

		TEX::EXPLOSION = Engine.AddTexture("./Resources/explosion.png");

		OBJ::PLAYER = Engine.AddObject();

		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER);

		ActorDescriptor.SetType(ObjectType::Actor);
		ActorDescriptor.SetValue(100.f); // 100 HP
		
		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({0.5f, 0.25f, 1.5f});

		OBJ::SPRITE::BODY = Engine.AddSprite(OBJ::PLAYER);
		OBJ::SPRITE::HEAD = Engine.AddSprite(OBJ::PLAYER);

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

	//Actor States
	{
		ST::IDLE = Engine.AddStatePrototype(OBJ::PLAYER, new IdleState);
		ST::MOVE = Engine.AddStatePrototype(OBJ::PLAYER, new MoveState);
		ST::IN_AIR = Engine.AddStatePrototype(OBJ::PLAYER, new InAirState(0.2f));
		ST::CHARGE_JUMP = Engine.AddStatePrototype(OBJ::PLAYER, new ChargeJumpState(1.f, 40'000.f));
		ST::CHARGE_SLAM = Engine.AddStatePrototype(OBJ::PLAYER, new ChargeSlamState(1.f));
		ST::SLAM = Engine.AddStatePrototype(OBJ::PLAYER, new SlamState(200'000.f));
	}
	
	//Commands
	{
		float Move = 1'500.f;
		CMD::MOVE_UP = Engine.AddCommand(new ForceCommand({ 0.f, Move, 0.f }));
		CMD::MOVE_DOWN = Engine.AddCommand(new ForceCommand({ 0.f, -Move, 0.f }));
		CMD::MOVE_LEFT = Engine.AddCommand(new ForceCommand({ -Move, 0.f, 0.f }));
		CMD::MOVE_RIGHT = Engine.AddCommand(new ForceCommand({ Move, 0.f, 0.f }));

		float SlowMove = 750.f;
		CMD::SLOW_MOVE_UP = Engine.AddCommand(new ForceCommand({ 0.f, SlowMove, 0.f }));
		CMD::SLOW_MOVE_DOWN = Engine.AddCommand(new ForceCommand({ 0.f, -SlowMove, 0.f }));
		CMD::SLOW_MOVE_LEFT = Engine.AddCommand(new ForceCommand({ -SlowMove, 0.f, 0.f }));
		CMD::SLOW_MOVE_RIGHT = Engine.AddCommand(new ForceCommand({ SlowMove, 0.f, 0.f }));

		CMD::START_MOVE = Engine.AddCommand(new StateCommand(ST::MOVE));
		CMD::CHARGE_JUMP = Engine.AddCommand(new StateCommand(ST::CHARGE_JUMP));
		CMD::START_IN_AIR = Engine.AddCommand(new StateCommand(ST::IN_AIR, ST_CMD::ON_RELEASE | ST_CMD::CHANGE_STATE));
		CMD::START_CHARGE_SLAM = Engine.AddCommand(new StateCommand(ST::CHARGE_SLAM));
	}

	//Actor Input
	{
		auto IdleInput = Engine.GetStateInput(OBJ::PLAYER, ST::IDLE);

		IdleInput->AddKey('W', CMD::START_MOVE);
		IdleInput->AddKey('A', CMD::START_MOVE);
		IdleInput->AddKey('S', CMD::START_MOVE);
		IdleInput->AddKey('D', CMD::START_MOVE);
		IdleInput->AddKey(VK_SPACE, CMD::CHARGE_JUMP);

		IdleInput->EmplaceLocalInput();

		auto MoveInput = Engine.GetStateInput(OBJ::PLAYER, ST::MOVE);

		MoveInput->AddKey('W', CMD::MOVE_UP);
		MoveInput->AddKey('A', CMD::MOVE_LEFT);
		MoveInput->AddKey('S', CMD::MOVE_DOWN);
		MoveInput->AddKey('D', CMD::MOVE_RIGHT);
		MoveInput->AddKey(VK_SPACE, CMD::CHARGE_JUMP);

		MoveInput->EmplaceLocalInput();

		auto ChargeJump = Engine.GetStateInput(OBJ::PLAYER, ST::CHARGE_JUMP);

		ChargeJump->AddKey('W', CMD::SLOW_MOVE_UP);
		ChargeJump->AddKey('A', CMD::SLOW_MOVE_LEFT);
		ChargeJump->AddKey('S', CMD::SLOW_MOVE_DOWN);
		ChargeJump->AddKey('D', CMD::SLOW_MOVE_RIGHT);
		ChargeJump->AddKey(VK_SPACE, CMD::START_IN_AIR);
		ChargeJump->EmplaceLocalInput();

		auto InAirInput = Engine.GetStateInput(OBJ::PLAYER, ST::IN_AIR);

		InAirInput->AddKey(VK_SPACE, CMD::START_CHARGE_SLAM);
		InAirInput->EmplaceLocalInput();

		Engine.ChangeState(OBJ::PLAYER, ST::IDLE);
	}

	//Test Obj
	{
		size_t TestObj = Engine.AddObject();
		
		Descriptor& TestDesc = Engine.GetDescriptor(TestObj);
		
		TestDesc.SetType(ObjectType::Structure);
		TestDesc.SetValue(0.2f); //Friction
		
		Physics& ObjPhysics = Engine.GetPhysics(TestObj);
		ObjPhysics.Box().SetDimensions({ 2.f, 2.f, 2.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ 5.f, 0.f, 0.f });
	}

	{
		size_t TestObj = Engine.AddObject();

		Descriptor& TestDesc = Engine.GetDescriptor(TestObj);

		TestDesc.SetType(ObjectType::Structure);
		TestDesc.SetValue(0.2f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(TestObj);
		ObjPhysics.Box().SetDimensions({ 7.f, 5.f, 3.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ -10.f, 0.f, 0.f });
	}
	//Floor
	{
		size_t Floor = Engine.AddObject();

		Descriptor& TestDesc = Engine.GetDescriptor(Floor);

		TestDesc.SetType(ObjectType::Structure);
		TestDesc.SetValue(1.f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(Floor);
		ObjPhysics.Box().SetDimensions({ 10.f, 10.f, 1.f });
		ObjPhysics.SetGravity(0.f);
		ObjPhysics.SetCollision(&Collision::Structure);
		ObjPhysics.SetPosition({ 0.f, 0.f, -1.f });
	}
	
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
