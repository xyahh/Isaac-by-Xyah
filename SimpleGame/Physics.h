#pragma once
#include "Collision.h"
#define EARTH_GRAVITY 9.81f

class Physics
{
	friend Cyan;
	friend State;

public:

	Physics() :
		m_Mass(1.f),
		m_PrevPos(0.f, 0.f, 0.f),
		m_Position(0.f, 0.f, 0.f),
		m_Velocity(0.f, 0.f, 0.f),
		m_Acceleration(0.f, 0.f, 0.f),
		m_Friction(1.f),
		m_Gravity(EARTH_GRAVITY * -1.f),
		m_Collision(NULL)
	{}
	~Physics() {}

	void SSE_CALLCONV ApplyForce(SSE_VECTOR_PARAM1 force);
	void SSE_CALLCONV SetForce(SSE_VECTOR_PARAM1 force);
	SSE_VECTOR GetForce() const;

	void SetPosition(float x, float y, float z);
	void SSE_CALLCONV SetPosition(SSE_VECTOR_PARAM1 v);
	SSE_VECTOR SSE_CALLCONV GetPosition() const;
	SSE_VECTOR SSE_CALLCONV GetPrevPosition() const;

	SSE_VECTOR SSE_CALLCONV GetDeltaPosition() const;
	void SSE_CALLCONV SetDeltaPosition(SSE_VECTOR_PARAM1 v);

	void SSE_CALLCONV SetVelocity(SSE_VECTOR_PARAM1 v);
	SSE_VECTOR SSE_CALLCONV GetVelocity() const;

	void SetMass(float mass);
	float GetMass() const;

	void SetFriction(float Coeff);
	float GetFriction() const;

	float GetGravity() const;
	void SetGravity(float Gravity);

	void SetCollision(BasicCollision* collision);

	BasicCollision* GetCollision() const;
	Collision::BBox& Box();


private:

	void HandleCollision(size_t MyID, Physics* OtherPhysics, size_t OtherID);
	void Update();

private:

	float			m_Mass;
	FLOAT3			m_Position;
	FLOAT3			m_DeltaPos; //For Collision
	FLOAT3			m_PrevPos;  //For Interpolation
	FLOAT3			m_Acceleration;
	FLOAT3			m_Velocity;
	float			m_Gravity;
	float			m_Friction;
	Collision::BBox	m_Box;
	BasicCollision*	m_Collision;
	
};