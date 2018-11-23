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
	Engine.UpdateState(ActorID, 0, NewStateID);
}

DX XMVECTOR State::GetActorForce(const id_type& ActorID) const
{
	return DX3 Load(Engine.GetActorPhysics(ActorID).m_Force);
}

u_int State::GetActorDirection(const id_type & ActorID)
{
	DX XMFLOAT2 Direction = DX2 Store(GetActorForce(ActorID));
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

/****************************************************************************************/

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
	Engine.GetActorGraphics(ActorID).Body.ResetSprite();
	
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

	ActorGraphics& Actor = Engine.GetActorGraphics(ActorID);
	Actor.Body.FrameLinearAdvance();
	State::Update(ActorID);
	if (Zero(DX2 Magnitude(Engine.GetActorPhysics(ActorID).GetVelocity())) && Zero(DX2 Magnitude(GetActorForce(ActorID))))
		ChangeState(ActorID, "Idle");
	else
	{
		u_int Dir = GetActorDirection(ActorID);
		Actor.Body.SetDirection(Dir);
		Actor.Head.SetDirection(Dir);
	}
		
}

void MovingState::Exit(const id_type& ActorID)
{
}


/* Jumping State */
void JumpingState::Enter(const id_type& ActorID)
{
	Physics& p = Engine.GetActorPhysics(ActorID);
	GroundFriction = p.GetFriction();
	p.SetFriction(0.2f);
	p.ApplyForce({ 0.f, 0.f, JumpForce });
}

void JumpingState::Update(const id_type& ActorID)
{
	if(DX GetZ(Engine.GetActorPhysics(ActorID).GetPosition()) > 1.f)
		State::Update(ActorID);

	DX XMVECTOR Velocity = Engine.GetActorPhysics(ActorID).GetVelocity();

	if (Zero(DX GetZ(Velocity)))
	{
		if (Zero(DX2 Magnitude(Velocity)))
			ChangeState(ActorID, "Idle");
		else
			ChangeState(ActorID, "Moving");
	}
}

void JumpingState::Exit(const id_type& ActorID)
{
	Engine.GetActorPhysics(ActorID).SetFriction(GroundFriction);
}


/* Charging Slam State */

void SlamChargingState::Enter(const id_type & ActorID)
{
	Physics& p = Engine.GetActorPhysics(ActorID);
	Gravity = p.GetGravity();
	p.SetGravity(0.f);
	RageAmount = 0.f;
	p.SetVelocity(DX XMVectorZero());
}

void SlamChargingState::Update(const id_type & ActorID)
{
	RageAmount += UPDATE_TIME * RageRate;
	Engine.GetActorGraphics(ActorID).SetColor(1.f, 1.f - RageAmount, 1.f - RageAmount, 1.f);
	if (RageAmount >= 1.f)
		ChangeState(ActorID, GroundSlamStateID);
}

void SlamChargingState::Exit(const id_type & ActorID)
{
	Engine.GetActorPhysics(ActorID).SetGravity(Gravity);
}


/* Ground Slam State*/
void SlammingState::Enter(const id_type& ActorID)
{
	Physics& ActorPhysics = Engine.GetActorPhysics(ActorID);
	ActorPhysics.ApplyForce({ 0.f, 0.f, -100'000.f });
}

void SlammingState::Update(const id_type& ActorID)
{
	if (Zero(DX GetZ(Engine.GetActorPhysics(ActorID).GetVelocity()) && Zero(DX GetZ(GetActorForce(ActorID)))))
		ChangeState(ActorID, "Idle");
}

void SlammingState::Exit(const id_type& ActorID)
{
	EffectID = Engine.AddEffect();
	EffectGraphics& e = Engine.GetEffect(EffectID);
	e.Effect.SetTexID(TexID);
	e.Effect.SetSize({ SlamSize, SlamSize });
	e.Effect.SetFrameRate(60);
	e.Effect.SetSpriteInfo({ 0, 0, 9, 9 });
	e.SetPosition(DX Add(Engine.GetActorPhysics(ActorID).GetPosition(), { 0.f, 0.1f, -0.1f }));
	Engine.GetActorGraphics(ActorID).SetColor(1.f, 1.f, 1.f, 1.f);
}

/* Shooting State*/
void ShootingState::Enter(const id_type & ActorID)
{
	BulletID = Engine.AddObject();
	Physics& p = Engine.GetObjectPhysics(BulletID);
	p.SetMass(0.5f);
	p.SetFriction(0.2f);
	ObjectGraphics& g = Engine.GetObjectGraphics(BulletID);
	g.ObjectSprite.SetTexID(TexID);
	g.ObjectSprite.SetSize({ 0.5f, 0.5f });
}

void ShootingState::Update(const id_type & ActorID)
{
}

void ShootingState::Exit(const id_type & ActorID)
{
}
