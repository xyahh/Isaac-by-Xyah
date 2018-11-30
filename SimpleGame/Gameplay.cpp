#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"
#include "Indices.h"

void Gameplay::Init()
{
	//Actor
	{
		size_t BodyTex = Engine.AddTexture("./Resources/Characters/basic_body.png");
		size_t HeadTex = Engine.AddTexture("./Resources/Characters/isaac_head.png");

		OBJ::PLAYER = Engine.AddObject(ObjectType::Actor);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER);

		size_t BodyIdx = Engine.AddSprite(OBJ::PLAYER);
		size_t HeadIdx = Engine.AddSprite(OBJ::PLAYER);

		auto& Body = Engine.GetSprite(OBJ::PLAYER, BodyIdx);
		auto& Head = Engine.GetSprite(OBJ::PLAYER, HeadIdx);

		
		ActorGraphics.SetColor(1.f, 1.f, 1.f, 1.f);
		ActorPhysics.SetMass(70.f);

		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(BodyTex);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);
		Body.SetFrameRate(30.f);

		Head.SetTexture(HeadTex);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetFrameRate(2.f);
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });

		
		
	}

	ST::IDLE = Engine.AddStatePrototype(OBJ::PLAYER, new IdleState);
	ST::MOVING = Engine.AddStatePrototype(OBJ::PLAYER, new MovingState);
	ST::JUMPING = Engine.AddStatePrototype(OBJ::PLAYER,	new InAirState);

	CMD::MOVE_UP = Engine.AddCommand(new ForceCommand({ 0.f, 1'500.f, 0.f }));
	CMD::MOVE_DOWN = Engine.AddCommand(new ForceCommand({ 0.f, -1'500.f, 0.f }));
	CMD::MOVE_LEFT = Engine.AddCommand(new ForceCommand({ -1'500.f, 0.f, 0.f }));
	CMD::MOVE_RIGHT = Engine.AddCommand(new ForceCommand({ 1'500.f, 0.f, 0.f }));
	CMD::JUMP = Engine.AddCommand(new ForceCommand({ 0.f, 0.f, 20'000.f }));

	CMD::START_MOVING = Engine.AddCommand(new StateCommand(ST::MOVING, ST_CMD::ON_PRESS | ST_CMD::CHANGE_STATE));
	CMD::START_JUMPING = Engine.AddCommand(new StateCommand(ST::JUMPING, ST_CMD::ON_PRESS | ST_CMD::CHANGE_STATE));

	auto IdleInput = Engine.GetStateInput(OBJ::PLAYER, ST::IDLE);

	IdleInput->AddKey('W', CMD::START_MOVING);
	IdleInput->AddKey('A', CMD::START_MOVING);
	IdleInput->AddKey('S', CMD::START_MOVING);
	IdleInput->AddKey('D', CMD::START_MOVING);
	IdleInput->AddKey(VK_SPACE, CMD::JUMP);

	IdleInput->EnableLocalInput();

	auto MoveInput = Engine.GetStateInput(OBJ::PLAYER, ST::MOVING);

	MoveInput->AddKey('W', CMD::MOVE_UP);
	MoveInput->AddKey('A', CMD::MOVE_LEFT);
	MoveInput->AddKey('S', CMD::MOVE_DOWN);
	MoveInput->AddKey('D', CMD::MOVE_RIGHT);
	MoveInput->AddKey(VK_SPACE, CMD::JUMP);

	MoveInput->EnableLocalInput();
	
	Engine.ChangeState(OBJ::PLAYER, ST::IDLE);
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
