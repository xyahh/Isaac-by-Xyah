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

void Physics::HandleCollision(Physics * OtherPhysics)
{

	DX XMVECTOR Min, Max, OtherMin, OtherMax;
	Collision::GetExtents(&Min, &Max, GetPosition(), m_Box);
	Collision::GetExtents(&OtherMin, &OtherMax, OtherPhysics->GetPosition(), OtherPhysics->m_Box);

	if (Collision::HandleCollision(Min, Max, OtherMin, OtherMax))
	{
		DX XMVECTOR Normal = Collision::GetNormal(Min, Max, OtherMin, OtherMax);
		if (m_Collision)
			m_Collision->OnCollision(*this, *OtherPhysics, Normal);
		if (OtherPhysics->m_Collision)
			OtherPhysics->GetCollision()->OnCollision(*OtherPhysics, *this, Normal);
	}
}

Collision::Box & Physics::GetBox()
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


void XM_CALLCONV Physics::ApplyForce(DX FXMVECTOR force)
{
	m_Force = DX3 Store(DX Add(DX3 Load(m_Force), force));
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

void XM_CALLCONV Physics::SetForce(DX FXMVECTOR force)
{
	m_Force = DX3 Store(force);
}

DX XMVECTOR Physics::GetForce() const
{
	return DX3 Load(m_Force);
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
	return m_Gravity;
}

void Physics::SetGravity(float Gravity)
{
	m_Gravity = Gravity;
}

void Physics::Update()
{
	//For Interpolation
	m_PrevPos = m_Position;

	DX XMVECTOR Position =	DX3 Load(m_Position);
	DX XMVECTOR Velocity =	DX3 Load(m_Velocity);
	DX XMVECTOR Force =		DX3 Load(m_Force);

	//DX Print(Force);
	//Apply Gravity Force
	DX Add(&Force, { 0.f, 0.f, -m_Gravity * m_Mass });

	//2D Friction
	{
		float Speed = DX2 Magnitude(Velocity);
		if (!Zero(Speed))
		{
			float GroundFrictionForce = -m_Friction * m_Gravity;
			DX XMVECTOR FrictionV = DX Scale(DX2 Normalize(Velocity), GroundFrictionForce * UPDATE_TIME);
			DX Add(&Velocity, FrictionV);
			if (DX GetX(Velocity) * m_Velocity.x < 0.f) DX SetX(&Velocity, 0.f);
			if (DX GetY(Velocity) * m_Velocity.y < 0.f) DX SetY(&Velocity, 0.f);
			DX SetZ(&Velocity, m_Velocity.z);
		}
	}

	DX Add(&Velocity, DX Scale(Force, UPDATE_TIME / m_Mass));
	DX Add(&Position, DX Scale(Velocity, UPDATE_TIME));

	//Reset Force to 0
	Force = DX XMVectorZero();

	//For now since there are no Bounding Boxes
	if (DX GetZ(Position) < 0.f)
	{
		DX SetZ(&Position, 0.f);
		DX SetZ(&Velocity, 0.f);
	}

	m_Force =	 DX3 Store(Force);
	m_Velocity = DX3 Store(Velocity);
	m_Position = DX3 Store(Position);
}
