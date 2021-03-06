#include "stdafx.h"
#include "State.h"
#include "CyanEngine.h"

NS_STATE_START

GlobalState Global;
IdleState Idle;
MoveState Move;
InAirState InAir(0.f);
ChargeJumpState ChargeJump(1.f, 40'000.f);
ChargeSlamState ChargeSlam(1.f);
SlamState Slam(200'000.f);
ShootState Shoot(5.f, 1'000.f);
DamagedState Damaged(2.f, 10.f);

NS_STATE_END

/* Idle State */

void IdleState::Enter(const IDType& ObjectIndex)
{
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, "Head");
	HeadSprite.SetDirection(Direction::Down);
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	BodySprite.SetDirection(Direction::Down);
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void IdleState::Update(const IDType& ObjectIndex)
{
	 SSE_VECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetForce();
	if (!Zero( GetZ(Velocity)))
		Engine.ChangeState(ObjectIndex, &STATE::InAir);
	else if (!Zero(Magnitude2(Velocity)))
		Engine.ChangeState(ObjectIndex, &STATE::Move);
}

void IdleState::Exit(const IDType& ObjectIndex)
{
	
}

/* Move State */

void MoveState::Enter(const IDType& ObjectIndex)
{
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	BodySprite.SetFrameRate(30.f);
}

void MoveState::Update(const IDType& ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	 SSE_VECTOR Velocity = ObjPhysics.GetVelocity();
	 SSE_VECTOR Force = ObjPhysics.GetForce();

	if (!Zero( GetZ(Velocity)))
		Engine.ChangeState(ObjectIndex, &STATE::InAir);
	else if (Zero(Magnitude2(Velocity)) && Zero(Magnitude2(Force)))
		Engine.ChangeState(ObjectIndex, &STATE::Idle);
}

void MoveState::Exit(const IDType& ObjectIndex)
{
}

/* Charge Jump State */

void ChargeJumpState::Enter(const IDType& ObjectIndex)
{
	RageAmount = 0.f;
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, "Head");
	HeadSprite.ResetSprite();
	HeadSprite.NextFrame();
}

void ChargeJumpState::Update(const IDType& ObjectIndex)
{
	RageAmount += UPDATE_TIME * RageRate;

	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	SSE_VECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(Magnitude2(Velocity)))
		BodySprite.SetFrameRate(10.f);
	else
	{
		BodySprite.SetFrameRate(0.f);
		BodySprite.ResetSprite();
	}
	Engine.GetSprite(ObjectIndex, "Head").SetColor({ 1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f });
	Clamp(0.f, &RageAmount, 1.f);
}

void ChargeJumpState::Exit(const IDType& ObjectIndex)
{
	Sprite& Head = Engine.GetSprite(ObjectIndex, "Head");

	Head.ResetSprite();
	Head.SetColor({ 1.f, 1.f, 1.f, 1.f });

	float Amount = Force * (1.f + RageAmount);
	Engine.GetPhysics(ObjectIndex).ApplyForce({ 0.f, 0.f,  Amount});
}

/* In Air State */

void InAirState::Enter(const IDType& ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	GroundFriction = ObjPhysics.GetFriction();
	ObjPhysics.SetFriction(AirResistance);
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	BodySprite.SetFrameRate(0.f);
	BodySprite.ResetSprite();
}

void InAirState::Update(const IDType& ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	if (Zero( GetZ(ObjPhysics.GetPosition())) && Zero( GetZ(ObjPhysics.GetForce())))
	{
		if (Zero(Magnitude2(ObjPhysics.GetVelocity())))
			Engine.ChangeState(ObjectIndex, &STATE::Idle);
		else
			Engine.ChangeState(ObjectIndex, &STATE::Move);
	}
}

void InAirState::Exit(const IDType& ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetFriction(GroundFriction);
}

/* Charge Slam State */

void ChargeSlamState::Enter(const IDType& ObjectIndex)
{
	RageAmount = 0.f;
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	ObjPhysics.SetVelocity(VectorZero());
	ObjGravity = ObjPhysics.GetGravity();
	ObjPhysics.SetGravity(0.f);

	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, "Head");
	HeadSprite.ResetSprite();
	HeadSprite.NextFrame();
}

void ChargeSlamState::Update(const IDType& ObjectIndex)
{
	RageAmount += UPDATE_TIME * RageRate;
	Engine.GetSprite(ObjectIndex, "Head").SetColor({ 1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f });
	if (RageAmount >= 1.f)
		Engine.ChangeState(ObjectIndex, &STATE::Slam);
}

void ChargeSlamState::Exit(const IDType& ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).SetGravity(ObjGravity);
	Engine.GetSprite(ObjectIndex, "Head").ResetSprite();
}


/* Slam State*/

void SlamState::Enter(const IDType& ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	ObjPhysics.ApplyForce({ 0.f, 0.f, -SlamForce });
}

void SlamState::Update(const IDType& ObjectIndex)
{
	Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
	if (Zero( GetZ(ObjPhysics.GetPosition())))
		Engine.ChangeState(ObjectIndex, &STATE::Idle);
}

void SlamState::Exit(const IDType& ObjectIndex)
{
	IDType& EffectID = Engine.AddObject();

	Descriptor& EffectDesc = Engine.GetDescriptor(EffectID);
	EffectDesc.Type = (ObjectType::Projectile);
	EffectDesc.Value = (5.f);
	EffectDesc.Team = (Engine.GetDescriptor(ObjectIndex).Team);

	Engine.GetSound().Play("Explosion");

	Engine.AddSprite(EffectID, "Sprite");
	Sprite& EffectSprite = Engine.GetSprite(EffectID, "Sprite");
	EffectSprite.SetLayerGroup(LayerGroup::Background);
	EffectSprite.SetSpriteType(SpriteShape::Grid);
	EffectSprite.SetTexture("Explosion");
	EffectSprite.SetSize({ 10.f, 10.f });
	EffectSprite.SetFrameRate(60);
	EffectSprite.SetDirection(5);
	EffectSprite.SetTotal({ 9, 9 });
	EffectSprite.AddEvent(SpriteEvent::LoopEnd, [EffectID]()
	{
		Engine.DeleteObject(EffectID);
	});

	Physics& EffectPhysics = Engine.GetPhysics(EffectID);
	EffectPhysics.Box().SetDimensions({ 8.5f, 8.5f, 0.5f });
	EffectPhysics.SetCollision(&Collision::Explosion);

	SSE_VECTOR Position = Engine.GetPhysics(ObjectIndex).GetPosition();
	EffectPhysics.SetPosition(Position);
	Engine.GetSprite(ObjectIndex, "Head").SetColor({ 1.f, 1.f, 1.f, 1.f });
}

/* Shooting State*/

void ShootState::Enter(const IDType& ObjectIndex)
{
	Time = 0.f;
	Engine.GetSprite(ObjectIndex, "Head").NextFrame();
}

void ShootState::Update(const IDType& ObjectIndex)
{
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, "Head");
	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	SSE_VECTOR Velocity = Engine.GetPhysics(ObjectIndex).GetVelocity();
	if (!Zero(Magnitude2(Velocity)))
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

		Engine.GetSound().Play("Pop");
		IDType& TearID = Engine.AddObject();

		Engine.AddSprite(TearID, "Shadow");
		Engine.AddSprite(TearID, "Tear");

		Sprite& TSprite = Engine.GetSprite(TearID, "Shadow");
		Sprite& TShadow = Engine.GetSprite(TearID, "Tear");
		TShadow.SetTexture("Shadow");
		TShadow.SetLayerGroup(LayerGroup::Background);
		TShadow.SetSize({ 0.5f, 0.5f });

		TSprite.SetTexture("Tear");
		TSprite.SetSize({ 0.5f, 0.5f });
		TSprite.SetOffset({ 0.f, 0.25f });

		Descriptor& TearDesc = Engine.GetDescriptor(TearID);
		TearDesc.Type = (ObjectType::Projectile);
		TearDesc.Value = (5.f);
		TearDesc.Team = (Engine.GetDescriptor(ObjectIndex).Team);

		Physics& ObjPhysics = Engine.GetPhysics(ObjectIndex);
		Physics& TPhysics = Engine.GetPhysics(TearID);
		TPhysics.SetPosition( Add(ObjPhysics.GetPosition(),
			{ 0.f, 0.f, 0.75f }));
		TPhysics.SetMass(0.5f);
		TPhysics.SetFriction(0.2f);
		TPhysics.SetGravity(1.f);
		TPhysics.Box().SetDimensions({ 0.5f, 0.125f, 0.5f });

		u_int Dir = Engine.GetSprite(ObjectIndex, "Head").GetDirection();
		TPhysics.SetCollision(&Collision::Projectile);
		TPhysics.SetVelocity(ObjPhysics.GetVelocity());
		TPhysics.ApplyForce( Scale(GetDirectionVector(Dir), ShootingForce));
	}
}

void ShootState::Exit(const IDType& ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, "Head").ResetSprite();
}

/* Damaged State*/

void DamagedState::Enter(const IDType& ObjectIndex)
{
	DurationTimer = 0.f;
	BlinkingTimer = 0.f;
	Alpha = 0;
	Engine.GetSprite(ObjectIndex, "Body").SetAlpha(0.f);
	Engine.GetSprite(ObjectIndex, "Head").SetAlpha(0.f);
}

void DamagedState::Update(const IDType& ObjectIndex)
{
	DurationTimer += UPDATE_TIME;
	BlinkingTimer += UPDATE_TIME * BlinkingRate;

	Sprite& BodySprite = Engine.GetSprite(ObjectIndex, "Body");
	Sprite& HeadSprite = Engine.GetSprite(ObjectIndex, "Head");

	Physics& p = Engine.GetPhysics(ObjectIndex);
	SSE_VECTOR Velocity = p.GetVelocity();
	if (!Zero(Magnitude2(Velocity)))
		BodySprite.SetFrameRate(10.f);
	else
	{
		BodySprite.SetFrameRate(0.f);
		BodySprite.ResetSprite();
	}

	if (!Zero(GetZ(Velocity)))
		p.SetFriction(0.1f);
	else
		p.SetFriction(1.f);

	if (BlinkingTimer >= 1.f)
	{
		float fAlpha = static_cast<float>(Alpha);
		BodySprite.SetAlpha(fAlpha);
		HeadSprite.SetAlpha(fAlpha);
		Alpha = (Alpha + 1) % 2;
		BlinkingTimer = 0.f;
	}

	if (DurationTimer >= Duration)
	{
		Engine.ChangeState(ObjectIndex, &STATE::Idle);
		DurationTimer = 0.f;
	}
}

void DamagedState::Exit(const IDType& ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, "Body").SetAlpha(1.f);
	Engine.GetSprite(ObjectIndex, "Head").SetAlpha(1.f);
}
