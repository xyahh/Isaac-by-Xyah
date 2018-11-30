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

/*Global State */
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
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::PERSON::BODY);
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void IdleState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);
	DX XMVECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(DX GetZ(Velocity)) && DX GetZ(Velocity) > 0.f)
		Engine.ChangeState(ObjectIndex, ST::JUMPING);
	else if (!Zero(DX2 Magnitude(Velocity)))
		Engine.ChangeState(ObjectIndex, ST::MOVING);
}

void IdleState::Exit(size_t ObjectIndex)
{
	
}


/* Moving State */
void MovingState::Enter(size_t ObjectIndex)
{
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, OBJ::PERSON::BODY);
	BodySprite.SetFrameRate(30.f);
	BodySprite.ResetSprite();
}

void MovingState::Update(size_t ObjectIndex)
{
	HandleInput(ObjectIndex);

	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	DX XMVECTOR Velocity = ObjPhysics.GetVelocity();
	DX XMVECTOR Force = ObjPhysics.GetForce();

	if (!Zero(DX GetZ(Velocity)) && DX GetZ(Velocity) > 0.f)
		Engine.ChangeState(ObjectIndex, ST::JUMPING);
	else if (Zero(DX2 Magnitude(Velocity)) && Zero(DX2 Magnitude(Force)))
		Engine.ChangeState(ObjectIndex, ST::IDLE);
	//u_int Dir = GetVector2Direction(APhysics.GetForce());
	//Engine.GetSprites(ObjectIndex)[0].SetDirection(Dir);
	//Engine.GetSprites(ObjectIndex)[1].SetDirection(Dir);
		
}

void MovingState::Exit(size_t ObjectIndex)
{
}

/* Jump State */

void JumpState::Enter(size_t ActorID)
{
	RageAmount = 0.f;
}

void JumpState::Update(size_t ActorID)
{
	RageAmount += UPDATE_TIME * RageRate;
}

void JumpState::Exit(size_t ActorID)
{
	
}



/* In Air State */
void InAirState::Enter(size_t ObjectIndex)
{
	Physics& p = Engine.GetPhysics(ObjectIndex);
	GroundFriction = p.GetFriction();
	p.SetFriction(0.2f);
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
			Engine.ChangeState(ObjectIndex, ST::MOVING);
	}
}

void InAirState::Exit(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetFriction(GroundFriction);
}


/* Charging Slam State */

void SlamChargingState::Enter(size_t ObjectIndex)
{
	Physics& p = Engine.GetPhysics(ObjectIndex);
	Gravity = p.GetGravity();
	p.SetGravity(0.f);
	RageAmount = 0.f;
	p.SetVelocity(DX XMVectorZero());
}

void SlamChargingState::Update(size_t ObjectIndex)
{
	RageAmount += UPDATE_TIME * RageRate;
	Engine.GetGraphics(ObjectIndex).SetColor(1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f);
	//if (RageAmount >= 1.f)
	//	ChangeState(ObjectIndex, GroundSlamStateID);
}

void SlamChargingState::Exit(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetGravity(Gravity);
}


/* Ground Slam State*/
void SlammingState::Enter(size_t ObjectIndex)
{
	Physics& ActorPhysics = Engine.GetPhysics(ObjectIndex);
	ActorPhysics.ApplyForce({ 0.f, 0.f, -100'000.f });
}

void SlammingState::Update(size_t ObjectIndex)
{
	//Physics& p = Engine.GetPhysics(ObjectIndex);
	//if (Zero(DX GetZ(p.GetVelocity()) && Zero(DX GetZ(p.GetForce()))))
	//	ChangeState(ActorID, "Idle");
}

void SlammingState::Exit(size_t ObjectIndex)
{
	//EffectID = Engine.AddObject(ObjectType::Effect);
	//Graphics& e = Engine.GetGraphics(EffectID);
	//e.AddSprite("Explosion");
	//Sprite& s = e.GetSprite("Explosion");
	//s.SetTexture(TexID);
	//s.SetSize({ 5.f, 5.f });
	//s.SetFrameRate(60);
	//s.SetTotal({ 0, 0, 9, 9 });
	//e.SetPosition(DX Add(Engine.GetPhysics(ActorID).GetPosition(), { 0.f, 0.1f, -0.1f }));
	//Engine.GetGraphics(ActorID).SetColor(1.f, 1.f, 1.f, 1.f);
}

/* Shooting State*/
void ShootingState::Enter(size_t ObjectIndex)
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

void ShootingState::Update(size_t ObjectIndex)
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

void ShootingState::Exit(size_t ObjectIndex)
{
	//Physics& BulletPhysics = Engine.GetPhysics(BulletID);
	//DX XMVECTOR For = DX Scale(GetDirectionVector2(GetActorFacingDirection(ActorID)), Force);
	//BulletPhysics.ApplyForce(For);
	//BulletPhysics.SetCollision(&Collision::Bullet);
	//BulletPhysics.SetVelocity(Engine.GetPhysics(ActorID).GetVelocity());
	//Engine.GetGraphics(ActorID).GetSprite("Head").ResetSprite();
}

