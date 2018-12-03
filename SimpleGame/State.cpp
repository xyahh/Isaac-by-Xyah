#include "stdafx.h"
#include "State.h"
#include "CyanEngine.h"
#include "Indices.h"

void State::HandleInput(size_t ObjectIndex)
{
	pInput->ReceiveLocalInput();
	pInput->ProcessInput(ObjectIndex);
}

/*--------------------------------------------------------------------------------------*/

/*Null State */

void NullState::Enter(size_t ObjectIndex)
{
}

void NullState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);
}

void NullState::Exit(size_t ObjectIndex)
{
}

/* Idle State */

void IdleState::Enter(size_t ObjectIndex)
{
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void IdleState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(DX GetZ(Velocity)))
		Engine.ChangeState(ObjectIndex, ST::IN_AIR);
	else if (!Zero(DX2 Magnitude(Velocity)))
		Engine.ChangeState(ObjectIndex, ST::MOVE);
}

void IdleState::Exit(size_t ObjectIndex)
{
	
}

/* Move State */

void MoveState::Enter(size_t ObjectIndex)
{
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	BodySprite.SetFrameRate(30.f);
	BodySprite.ResetSprite();
}

void MoveState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);

	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	DX XMVECTOR Velocity = ObjPhysics.GetVelocity();
	DX XMVECTOR Force = ObjPhysics.GetForce();

	if (!Zero(DX GetZ(Velocity)))
		Engine.ChangeState(ObjectIndex, ST::IN_AIR);
	else if (Zero(DX2 Magnitude(Velocity)) && Zero(DX2 Magnitude(Force)))
		Engine.ChangeState(ObjectIndex, ST::IDLE);
	//u_int Dir = GetVector2Direction(APhysics.GetForce());
	//Engine.GetSprites(ObjectIndex)[0].SetDirection(Dir);
	//Engine.GetSprites(ObjectIndex)[1].SetDirection(Dir);
		
}

void MoveState::Exit(size_t ObjectIndex)
{
}

/* Charge Jump State */

void ChargeJumpState::Enter(size_t ObjectIndex)
{
	RageAmount = 0.f;
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD);
	HeadSprite.ResetSprite();
	HeadSprite.NextFrame();
}

void ChargeJumpState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);
	RageAmount += UPDATE_TIME * RageRate;


	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(DX2 Magnitude(Velocity)))
		BodySprite.SetFrameRate(10.f);
	else
	{
		BodySprite.SetFrameRate(0.f);
		BodySprite.ResetSprite();
	}
		

	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f);
	Clamp(0.f, &RageAmount, 1.f);
}

void ChargeJumpState::Exit(size_t ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD).ResetSprite();
	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f, 1.f, 1.f);
	Engine.GetPhysics(ObjectIndex).ApplyForce({ 0.f, 0.f, Force * (1.f + RageAmount) });
}

/* In Air State */

void InAirState::Enter(size_t ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	GroundFriction = ObjPhysics.GetFriction();
	ObjPhysics.SetFriction(AirResistance);

	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void InAirState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (Zero(DX GetZ(Velocity)))
	{
		if (Zero(DX2 Magnitude(Velocity)))
			Engine.ChangeState(ObjectIndex, ST::IDLE);
		else
			Engine.ChangeState(ObjectIndex, ST::MOVE);
	}
}

void InAirState::Exit(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetFriction(GroundFriction);
}

/* Charge Slam State */

void ChargeSlamState::Enter(size_t ObjectIndex)
{
	RageAmount = 0.f;
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	ObjPhysics.SetVelocity(DX XMVectorZero());
	ObjGravity = ObjPhysics.GetGravity();
	ObjPhysics.SetGravity(0.f);

	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD);
	HeadSprite.ResetSprite();
	HeadSprite.NextFrame();
}

void ChargeSlamState::Update(size_t ObjectIndex)
{
	RageAmount += UPDATE_TIME * RageRate;
	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f);
	if (RageAmount >= 1.f)
		Engine.ChangeState(ObjectIndex, ST::SLAM);
}

void ChargeSlamState::Exit(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetGravity(ObjGravity);
	Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD).ResetSprite();
}


/* Slam State*/

void SlamState::Enter(size_t ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	ObjPhysics.ApplyForce({ 0.f, 0.f, -SlamForce });
}

void SlamState::Update(size_t ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	if (Zero(DX GetZ(ObjPhysics.GetVelocity()) && Zero(DX GetZ(ObjPhysics.GetForce()))))
		Engine.ChangeState(ObjectIndex, ST::IDLE);
}

void SlamState::Exit(size_t ObjectIndex)
{
	//size_t Effect = Engine.AddObject();
	//
	//size_t EffSpIdx = Engine.AddSprite(Effect);
	//Sprite& EffectSprite = Engine.GetSprite(Effect, EffSpIdx);
	//EffectSprite.SetSpriteType(SPRITETYPE::GRID);
	//EffectSprite.SetTexture(TEX::EXPLOSION);
	//EffectSprite.SetSize({ 5.f, 5.f });
	//EffectSprite.SetFrameRate(60);
	//EffectSprite.SetTotal({ 9, 9 });
	//
	//Engine.GetPhysics(Effect).SetPosition(
	//	DX Add(Engine.GetPhysics(ObjectIndex).GetPosition(), 
	//		{ 0.f, 0.1f, -0.1f })
	//);
	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f, 1.f, 1.f);
}

/* Shooting State*/

void ShootState::Enter(size_t ObjectIndex)
{
	//Growth = 0.5f;
	//Engine.GetGraphics(ActorID).GetSprite("Head").FrameLinearNext();
	//
	//BulletID = Engine.AddObject("Bullet");
	//Graphics& BulletGraphics = Engine.GetGraphics(BulletID);
	//BulletGraphics.AddSprite("Bullet");
	//Sprite& BulletSprite = BulletGraphics.GetSprite("Bullet");
	//BulletSprite.SetTexture(TexID);
	//BulletSprite.SetSize({ Growth, Growth });
	//
	//Physics& BulletPhysics = Engine.GetPhysics(BulletID);
	//BulletPhysics.SetPosition(Engine.GetPhysics(ActorID).GetPosition());
	//BulletPhysics.SetMass(0.5f);
	//BulletPhysics.SetFriction(0.2f);
}

void ShootState::Update(size_t ObjectIndex)
{
	//State::Update(ActorID);
	//Physics& ActorPhysics = Engine.GetPhysics(ActorID);
	//Physics& BulletPhysics = Engine.GetPhysics(BulletID);
	//Graphics& ActorGraphics = Engine.GetGraphics(ActorID);
	//Graphics& BulletGraphics = Engine.GetGraphics(BulletID);
	//
	//Growth += UPDATE_TIME * GrowingRate;
	//Clamp(0.5f, &Growth, 2.f);
	//
	//ActorGraphics.GetSprite("Body").SetDirection(GetVector2Direction(ActorPhysics.GetForce()));
	//if (Zero(DX2 Magnitude(ActorPhysics.GetVelocity())))
	//	ActorGraphics.GetSprite("Body").ResetSprite();
	//else 
	//	ActorGraphics.GetSprite("Body").FrameLinearUpdate();
	//
	//BulletGraphics.GetSprite("Bullet").SetSize({ Growth, Growth });	
	//BulletPhysics.SetPosition(ActorPhysics.GetPosition());
	//BulletPhysics.GetBox().SetDimensions({ Growth, Growth*0.25f, Growth });
}

void ShootState::Exit(size_t ObjectIndex)
{
	//Physics& BulletPhysics = Engine.GetPhysics(BulletID);
	//DX XMVECTOR For = DX Scale(GetDirectionVector2(GetActorFacingDirection(ActorID)), Force);
	//BulletPhysics.ApplyForce(For);
	//BulletPhysics.SetCollision(&Collision::Bullet);
	//BulletPhysics.SetVelocity(Engine.GetPhysics(ActorID).GetVelocity());
	//Engine.GetGraphics(ActorID).GetSprite("Head").ResetSprite();
}

