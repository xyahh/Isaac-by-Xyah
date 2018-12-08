#include "stdafx.h"
#include "State.h"
#include "CyanEngine.h"

/*Null State */

void NullState::Enter(size_t ObjectIndex)
{
}

void NullState::Update(size_t ObjectIndex)
{
}

void NullState::Exit(size_t ObjectIndex)
{
}

/* Idle State */

void IdleState::Enter(size_t ObjectIndex)
{
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD);
	HeadSprite.SetDirection(Direction::Down);
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	BodySprite.SetDirection(Direction::Down);
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void IdleState::Update(size_t ObjectIndex)
{
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetForce();
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
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	DX XMVECTOR Velocity = ObjPhysics.GetVelocity();
	DX XMVECTOR Force = ObjPhysics.GetForce();

	if (!Zero(DX GetZ(Velocity)))
		Engine.ChangeState(ObjectIndex, ST::IN_AIR);
	else if (Zero(DX2 Magnitude(Velocity)) && Zero(DX2 Magnitude(Force)))
		Engine.ChangeState(ObjectIndex, ST::IDLE);		
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
	float Amount = Force * (1.f + RageAmount);
	Engine.GetPhysics(ObjectIndex).ApplyForce({ 0.f, 0.f,  Amount});
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
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	if (Zero(DX GetZ(ObjPhysics.GetPosition())) && Zero(DX GetZ(ObjPhysics.GetForce())))
	{
		if (Zero(DX2 Magnitude(ObjPhysics.GetVelocity())))
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
	if (Zero(DX GetZ(ObjPhysics.GetPosition())))
		Engine.ChangeState(ObjectIndex, ST::IDLE);
}

void SlamState::Exit(size_t ObjectIndex)
{
	size_t Effect;
	Engine.AddObject(&Effect);
	size_t EffSpIdx;
	Engine.AddSprite(&EffSpIdx, Effect);
	Sprite& EffectSprite = Engine.GetSprite(Effect, EffSpIdx);
	EffectSprite.SetSpriteType(SpriteType::Grid);
	EffectSprite.SetTexture(TEX::EXPLOSION);
	EffectSprite.SetSize({ 5.f, 5.f });
	EffectSprite.SetFrameRate(60);
	EffectSprite.SetDirection(5);
	EffectSprite.SetTotal({ 9, 9 });
	EffectSprite.AddEvent(SpriteEvent::LoopEnd, [Effect]() 
	{
		Engine.DeleteObject(Effect);
	});
	Physics& EffectPhysics = Engine.GetPhysics(Effect);
	EffectPhysics.SetPosition(Engine.GetPhysics(ObjectIndex).GetPosition());
	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f, 1.f, 1.f);
}

/* Shooting State*/

void ShootState::Enter(size_t ObjectIndex)
{
	Time = 0.f;
	Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD).NextFrame();
}

void ShootState::Update(size_t ObjectIndex)
{
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD);
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::BODY);
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(DX2 Magnitude(Velocity)))
		BodySprite.SetFrameRate(30.f);
	else
	{
		BodySprite.SetFrameRate(0.f);
		BodySprite.ResetSprite();
	}

	Time += UPDATE_TIME * ShootingRate;
	if (Time >= 1.f)
	{
		Time = 0.f;
		HeadSprite.NextFrame();
		size_t Tear, TearSprite;
		Engine.AddObject(&Tear);
		Engine.AddSprite(&TearSprite, Tear);
		Sprite& TSprite = Engine.GetSprite(Tear, TearSprite);
		TSprite.SetTexture(TexID);
		TSprite.SetSize({ 0.5f, 0.5f });
		TSprite.SetOffset({ 0.f, 0.25f });

		Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
		Physics& TPhysics = Engine.GetPhysics(Tear);
		TPhysics.SetPosition(ObjPhysics.GetPosition());
		TPhysics.SetMass(0.5f);
		TPhysics.SetFriction(0.2f);
		TPhysics.Box().SetDimensions({ 0.5f, 0.125f, 0.5f });

		u_int Dir = Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD).GetDirection();
		TPhysics.SetCollision(&Collision::Projectile);
		TPhysics.SetVelocity(ObjPhysics.GetVelocity());
		TPhysics.ApplyForce(DX Scale(GetDirectionVector(Dir), ShootingForce));
	}
}

void ShootState::Exit(size_t ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, OBJ::SPRITE::HEAD).ResetSprite();
}


