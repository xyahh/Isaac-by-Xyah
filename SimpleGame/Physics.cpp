#include "stdafx.h"
#include "Physics.h"
#include "CyanEngine.h"

/* Basic Physics */

SSE_VECTOR SSE_CALLCONV Physics::GetPosition() const
{
	return LoadFloat3(m_Position);
}

void Physics::SetCollision(BasicCollision* collision)
{
	m_Collision = collision;
}

BasicCollision * Physics::GetCollision() const
{
	return m_Collision;
}

void Physics::HandleCollision(size_t MyID, Physics* OtherPhysics, size_t OtherID)
{

	SSE_VECTOR Min, Max, OtherMin, OtherMax;
	Collision::GetExtents(&Min, &Max, GetPosition(), m_Box);
	Collision::GetExtents(&OtherMin, &OtherMax, OtherPhysics->GetPosition(), OtherPhysics->m_Box);

	if (Collision::HandleCollision(Min, Max, OtherMin, OtherMax))
	{
		SSE_VECTOR Normal = Collision::GetNormal(Min, Max, OtherMin, OtherMax);
		if (m_Collision)
			m_Collision->OnCollision(MyID, this, OtherID, 
				OtherPhysics, Normal);
		if (OtherPhysics->m_Collision)
			OtherPhysics->GetCollision()->OnCollision(OtherID, OtherPhysics, MyID, this, Normal);
	}
}

Collision::BBox & Physics::Box()
{
	return m_Box;
}

/* Moveable Physics */

void Physics::SetMass(float mass)
{
	m_Mass = mass;
}

float Physics::GetMass() const
{
	return m_Mass;
}


void SSE_CALLCONV Physics::ApplyForce( SSE_VECTOR_PARAM1 Force)
{
	 SSE_VECTOR AddedAccel =  Scale(Force, 1.f / m_Mass);
	m_Acceleration = StoreFloat3( Add(LoadFloat3(m_Acceleration), AddedAccel));
}

void Physics::SetPosition(float x, float y, float z)
{
	m_Position =  FLOAT3(x, y, z);
	m_PrevPos = m_Position;
}

void SSE_CALLCONV Physics::SetPosition( SSE_VECTOR_PARAM1 v)
{
	m_Position = StoreFloat3(v);
	m_PrevPos = StoreFloat3(v);
}

 SSE_VECTOR SSE_CALLCONV Physics::GetPrevPosition() const
{
	return LoadFloat3(m_PrevPos);
}

void SSE_CALLCONV Physics::SetForce( SSE_VECTOR_PARAM1 Force)
{
	m_Acceleration = StoreFloat3( Scale(Force, 1.f / m_Mass));
}

 SSE_VECTOR Physics::GetForce() const
{
	return  Scale(LoadFloat3(m_Acceleration), m_Mass);
}

 SSE_VECTOR SSE_CALLCONV Physics::GetDeltaPosition() const
{
	return LoadFloat3(m_DeltaPos);
}

void SSE_CALLCONV Physics::SetDeltaPosition( SSE_VECTOR_PARAM1 v)
{
	m_DeltaPos = StoreFloat3(v);
}

void SSE_CALLCONV Physics::SetVelocity( SSE_VECTOR_PARAM1 v)
{
	m_Velocity = StoreFloat3(v);
}

 SSE_VECTOR SSE_CALLCONV Physics::GetVelocity() const
{
	return LoadFloat3(m_Velocity);
}

void Physics::SetFriction(float Coeff)
{
	m_Friction = Coeff;
}

float Physics::GetFriction() const
{
	return m_Friction;
}

float Physics::GetGravity() const
{
	return -1.f * m_Gravity;
}

void Physics::SetGravity(float Gravity)
{
	m_Gravity = -1.f * Gravity;
}

void Physics::Update()
{
	/* Interpolation */
	m_PrevPos = m_Position;

	SSE_VECTOR Position = LoadFloat3(m_Position);
	SSE_VECTOR Velocity = LoadFloat3(m_Velocity);
	SSE_VECTOR Acceleration = LoadFloat3(m_Acceleration);

	/* --- Gravity -----------------------------------------------------------------------------*/
	Acceleration =  Add(Acceleration, { 0.f, 0.f, m_Gravity });
	/* -----------------------------------------------------------------------------------------*/

	/* --- Friction ----------------------------------------------------------------------------*/
	float FrictionAccel = m_Friction * m_Gravity;
	float FrictionSpeed = FrictionAccel * UPDATE_TIME;

	SSE_VECTOR FrictionVelocity =  Scale(Normalize3(Velocity), FrictionSpeed);
	SSE_VECTOR PreviousVelocity = Velocity;

	Velocity =  Add(Velocity, FrictionVelocity);

	SSE_VECTOR FrictionError =  GreaterOrEqual
	(
		 Multiply(Velocity, PreviousVelocity),
		 VectorZero()
	);
	
	Velocity =  Multiply(Velocity,  Evaluate(FrictionError));
	/* -----------------------------------------------------------------------------------------*/

	Velocity =  Add(Velocity,  Scale(Acceleration, UPDATE_TIME));
	Position =  Add(Position,  Scale(Velocity, UPDATE_TIME));


	/* --- Reset & Store For Next Physics Cycle -----------------------------------------------*/
	Acceleration = VectorZero();
	//m_Friction = 0.f;

	if ( GetZ(Position) < 0.f)
	{
		 SetZ(&Position, 0.f);
		 SetZ(&Velocity, 0.f);
	}
	m_Acceleration =  StoreFloat3(Acceleration);
	m_Velocity		= StoreFloat3(Velocity);
	m_Position		= StoreFloat3(Position);
	/* -----------------------------------------------------------------------------------------*/
}
