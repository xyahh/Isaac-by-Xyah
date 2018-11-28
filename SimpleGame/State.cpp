#include "stdafx.h"
#include "State.h"
#include "CyanEngine.h"

void State::Update(const id_type& ActorID)
{
	Engine.GetStateType(Engine.GetActorState(ActorID).StateID)->m_Input[ActorID].HandleInput(ActorID);
}

Input& State::GetInput(const id_type& ActorID)
{
	return m_Input[ActorID];
}

void State::ChangeState(const id_type& ActorID, const id_type & NewStateID)
{
	Engine.UpdateState(ActorID, NewStateID);
}

u_int XM_CALLCONV State::GetVector2Direction(DX FXMVECTOR v)
{
	DX XMFLOAT2 Direction = DX2 Store(v);
	float Highest = 0.f;
	if (Direction.y > Highest)
	{
		Highest = Direction.y;
		m_Direction = DIRECTION::UP;
	}
	if (-Direction.y > Highest)
	{
		Highest = -Direction.y;
		m_Direction = DIRECTION::DOWN;
	}
	if (-Direction.x > Highest)
	{
		Highest = -Direction.x;
		m_Direction = DIRECTION::LEFT;
	}
	if (Direction.x > Highest)
	{
		Highest = Direction.x;
		m_Direction = DIRECTION::RIGHT;
	}
	return m_Direction;
}

u_int State::GetActorFacingDirection(const id_type & ActorID)
{
	return Engine.GetEntityGraphics(ActorID).GetSprite("Head").GetDirection();
}

DX XMVECTOR State::GetDirectionVector2(u_int Direction)
{
	switch (Direction)
	{
	case DIRECTION::UP: return { 0.f, 1.f, 0.f };
	case DIRECTION::DOWN: return { 0.f, -1.f, 0.f };
	case DIRECTION::LEFT: return { -1.f, 0.f, 0.f };
	case DIRECTION::RIGHT: return { 1.f, 0.f, 0.f };
	}
	return DX XMVECTOR();
}

/*--------------------------------------------------------------------------------------*/

/*Global State */
void GlobalState::Enter(const id_type& ActorID)
{
}

void GlobalState::Update(const id_type& ActorID)
{
	State::Update(ActorID);
}

void GlobalState::Exit(const id_type& ActorID)
{
}

/* Idle State */
void IdleState::Enter(const id_type& ActorID)
{
	Engine.GetEntityGraphics(ActorID).GetSprite("Body").ResetSprite();
	
}

void IdleState::Update(const id_type& ActorID)
{
	return  State::Update(ActorID);
}

void IdleState::Exit(const id_type& ActorID)
{
	
}


/* Moving State */
void MovingState::Enter(const id_type& ActorID)
{
}

void MovingState::Update(const id_type& ActorID)
{

	Graphics& AGraphics = Engine.GetEntityGraphics(ActorID);
	Physics& APhysics = Engine.GetEntityPhysics(ActorID);
	AGraphics.GetSprite("Body").FrameLinearUpdate();
	State::Update(ActorID);
	if (Zero(DX2 Magnitude(APhysics.GetVelocity())) && Zero(DX2 Magnitude(APhysics.GetForce())))
		ChangeState(ActorID, "Idle");
	else
	{
		u_int Dir = GetVector2Direction(APhysics.GetForce());
		AGraphics.GetSprite("Body").SetDirection(Dir);
		AGraphics.GetSprite("Head").SetDirection(Dir);
	}
		
}

void MovingState::Exit(const id_type& ActorID)
{
}


/* In Air State */
void InAirState::Enter(const id_type& ActorID)
{
	Physics& p = Engine.GetEntityPhysics(ActorID);
	GroundFriction = p.GetFriction();
	p.SetFriction(0.2f);
}

void InAirState::Update(const id_type& ActorID)
{
	State::Update(ActorID);
	DX XMVECTOR Velocity = Engine.GetEntityPhysics(ActorID).GetVelocity();

	if (Zero(DX GetZ(Velocity)))
	{
		if (Zero(DX2 Magnitude(Velocity)))
			ChangeState(ActorID, "Idle");
		else
			ChangeState(ActorID, "Moving");
	}
}

void InAirState::Exit(const id_type& ActorID)
{
	Engine.GetEntityPhysics(ActorID).SetFriction(GroundFriction);
}


/* Charging Slam State */

void SlamChargingState::Enter(const id_type & ActorID)
{
	Physics& p = Engine.GetEntityPhysics(ActorID);
	Gravity = p.GetGravity();
	p.SetGravity(0.f);
	RageAmount = 0.f;
	p.SetVelocity(DX XMVectorZero());
}

void SlamChargingState::Update(const id_type & ActorID)
{
	RageAmount += UPDATE_TIME * RageRate;
	Engine.GetEntityGraphics(ActorID).SetColor(1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f);
	if (RageAmount >= 1.f)
		ChangeState(ActorID, GroundSlamStateID);
}

void SlamChargingState::Exit(const id_type & ActorID)
{
	Engine.GetEntityPhysics(ActorID).SetGravity(Gravity);
}


/* Ground Slam State*/
void SlammingState::Enter(const id_type& ActorID)
{
	Physics& ActorPhysics = Engine.GetEntityPhysics(ActorID);
	ActorPhysics.ApplyForce({ 0.f, 0.f, -100'000.f });
}

void SlammingState::Update(const id_type& ActorID)
{
	Physics& p = Engine.GetEntityPhysics(ActorID);
	if (Zero(DX GetZ(p.GetVelocity()) && Zero(DX GetZ(p.GetForce()))))
		ChangeState(ActorID, "Idle");
}

void SlammingState::Exit(const id_type& ActorID)
{
	EffectID = Engine.AddEffect();
	EffectGraphics& e = Engine.GetEffect(EffectID);
	e.Effect.SetTexID(TexID);
	e.Effect.SetSize({ 5.f, 5.f });
	e.Effect.SetFrameRate(60);
	e.Effect.SetSpriteInfo({ 0, 0, 9, 9 });
	e.SetPosition(DX Add(Engine.GetEntityPhysics(ActorID).GetPosition(), { 0.f, 0.1f, -0.1f }));
	Engine.GetEntityGraphics(ActorID).SetColor(1.f, 1.f, 1.f, 1.f);
}

/* Shooting State*/
void ShootingState::Enter(const id_type & ActorID)
{
	Growth = 0.5f;
	Engine.GetEntityGraphics(ActorID).GetSprite("Head").FrameLinearNext();

	BulletID = Engine.AddObject("Bullet");
	Graphics& BulletGraphics = Engine.GetEntityGraphics(BulletID);
	BulletGraphics.AddSprite("Bullet");
	Sprite& BulletSprite = BulletGraphics.GetSprite("Bullet");
	BulletSprite.SetTexID(TexID);
	BulletSprite.SetSize({ Growth, Growth });

	Physics& BulletPhysics = Engine.GetEntityPhysics(BulletID);
	BulletPhysics.SetPosition(Engine.GetEntityPhysics(ActorID).GetPosition());
	BulletPhysics.SetMass(0.5f);
	BulletPhysics.SetFriction(0.2f);
}

void ShootingState::Update(const id_type & ActorID)
{
	State::Update(ActorID);
	Physics& ActorPhysics = Engine.GetEntityPhysics(ActorID);
	Physics& BulletPhysics = Engine.GetEntityPhysics(BulletID);
	Graphics& ActorGraphics = Engine.GetEntityGraphics(ActorID);
	Graphics& BulletGraphics = Engine.GetEntityGraphics(BulletID);

	Growth += UPDATE_TIME * GrowingRate;
	Clamp(0.5f, &Growth, 2.f);

	ActorGraphics.GetSprite("Body").SetDirection(GetVector2Direction(ActorPhysics.GetForce()));
	if (Zero(DX2 Magnitude(ActorPhysics.GetVelocity())))
		ActorGraphics.GetSprite("Body").ResetSprite();
	else 
		ActorGraphics.GetSprite("Body").FrameLinearUpdate();

	BulletGraphics.GetSprite("Bullet").SetSize({ Growth, Growth });	
	BulletPhysics.SetPosition(ActorPhysics.GetPosition());
	BulletPhysics.GetBox().SetDimensions({ Growth, Growth*0.25f, Growth });
}

void ShootingState::Exit(const id_type & ActorID)
{
	Physics& BulletPhysics = Engine.GetEntityPhysics(BulletID);
	DX XMVECTOR For = DX Scale(GetDirectionVector2(GetActorFacingDirection(ActorID)), Force);
	BulletPhysics.ApplyForce(For);
	BulletPhysics.SetCollision(&Collision::Bullet);
	BulletPhysics.SetVelocity(Engine.GetEntityPhysics(ActorID).GetVelocity());
	Engine.GetEntityGraphics(ActorID).GetSprite("Head").ResetSprite();
}
