#include "stdafx.h"
#include "Physics.h"
#include "CyanEngine.h"

/* Basic Physics */

DX XMVECTOR XM_CALLCONV Physics::GetPosition() const
{
	return DX3 Load(m_Position);
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

	DX XMVECTOR Min, Max, OtherMin, OtherMax;
	Collision::GetExtents(&Min, &Max, GetPosition(), m_Box);
	Collision::GetExtents(&OtherMin, &OtherMax, OtherPhysics->GetPosition(), OtherPhysics->m_Box);

	if (Collision::HandleCollision(Min, Max, OtherMin, OtherMax))
	{
		DX XMVECTOR Normal = Collision::GetNormal(Min, Max, OtherMin, OtherMax);
		
		if (m_Collision)
			m_Collision->OnCollision(MyID, this, OtherID, OtherPhysics, Normal);
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


void XM_CALLCONV Physics::ApplyForce(DX FXMVECTOR Force)
{
	DX XMVECTOR AddedAccel = DX Scale(Force, 1.f / m_Mass);
	m_Acceleration = DX3 Store(DX Add(DX3 Load(m_Acceleration), AddedAccel));
}

void Physics::SetPosition(float x, float y, float z)
{
	m_Position = DX XMFLOAT3(x, y, z);
	m_PrevPos = m_Position;
}

void XM_CALLCONV Physics::SetPosition(DX FXMVECTOR v)
{
	m_Position = DX3 Store(v);
	m_PrevPos = DX3 Store(v);
}

DX XMVECTOR XM_CALLCONV Physics::GetPrevPosition() const
{
	return DX3 Load(m_PrevPos);
}

void XM_CALLCONV Physics::SetForce(DX FXMVECTOR Force)
{
	m_Acceleration = DX3 Store(DX Scale(Force, 1.f / m_Mass));
}

DX XMVECTOR Physics::GetForce() const
{
	return DX Scale(DX3 Load(m_Acceleration), m_Mass);
}

DX XMVECTOR XM_CALLCONV Physics::GetDeltaPosition() const
{
	return DX3 Load(m_DeltaPos);
}

void XM_CALLCONV Physics::SetDeltaPosition(DX FXMVECTOR v)
{
	m_DeltaPos = DX3 Store(v);
}

void XM_CALLCONV Physics::SetVelocity(DX FXMVECTOR v)
{
	m_Velocity = DX3 Store(v);
}

DX XMVECTOR XM_CALLCONV Physics::GetVelocity() const
{
	return DX3 Load(m_Velocity);
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

	DX XMVECTOR Position = DX3 Load(m_Position);
	DX XMVECTOR Velocity = DX3 Load(m_Velocity);
	DX XMVECTOR Acceleration = DX3 Load(m_Acceleration);


	/* --- Gravity -----------------------------------------------------------------------------*/
	Acceleration = DX Add(Acceleration, { 0.f, 0.f, m_Gravity });
	/* -----------------------------------------------------------------------------------------*/

	/* --- Friction ----------------------------------------------------------------------------*/
	float FrictionAccel = m_Friction * m_Gravity;
	float FrictionSpeed = FrictionAccel * UPDATE_TIME;

	DX XMVECTOR FrictionVelocity = DX Scale(DX3 Normalize(Velocity), FrictionSpeed);
	DX XMVECTOR PreviousVelocity = Velocity;

	Velocity = DX Add(Velocity, FrictionVelocity);

	DX XMVECTOR FrictionError = DX GreaterOrEqual
	(
		DX Multiply(Velocity, PreviousVelocity),
		DX XMVectorZero()
	);
	
	Velocity = DX Multiply(Velocity, DX Evaluate(FrictionError));
	/* -----------------------------------------------------------------------------------------*/

	Velocity = DX Add(Velocity, DX Scale(Acceleration, UPDATE_TIME));
	Position = DX Add(Position, DX Scale(Velocity, UPDATE_TIME));

	/* --- Reset & Store For Next Physics Cycle -----------------------------------------------*/
	Acceleration = DX XMVectorZero();
	//m_Friction = 0.f;

	if (DX GetZ(Position) < 0.f)
	{
		DX SetZ(&Position, 0.f);
		DX SetZ(&Velocity, 0.f);
	}

	m_Acceleration =  DX3 Store(Acceleration);
	m_Velocity		= DX3 Store(Velocity);
	m_Position		= DX3 Store(Position);
	/* -----------------------------------------------------------------------------------------*/

	
}
