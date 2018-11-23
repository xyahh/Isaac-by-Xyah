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
		m_Velocity(0.f, 0.f, 0.f),
		m_Position(0.f, 0.f, 0.f),
		m_PrevPos(0.f, 0.f, 0.f),
		m_Force(0.f, 0.f, 0.f),
		m_Gravity(EARTH_GRAVITY),
		m_Collision(NULL)
	{}
	~Physics() {}

	void XM_CALLCONV ApplyForce(DX FXMVECTOR force);
	void XM_CALLCONV SetForce(DX FXMVECTOR force);
	DX XMVECTOR GetForce() const;

	void SetPosition(float x, float y, float z);
	void XM_CALLCONV SetPosition(DX FXMVECTOR v);
	DX XMVECTOR XM_CALLCONV GetPosition() const;
	DX XMVECTOR XM_CALLCONV GetPrevPosition() const;

	void XM_CALLCONV SetVelocity(DX FXMVECTOR v);
	DX XMVECTOR XM_CALLCONV GetVelocity() const;

	void SetMass(float mass);
	float GetMass() const;

	void SetFriction(float Coeff);
	float GetFriction() const;

	float GetGravity() const;
	void SetGravity(float Gravity);

	void SetCollision(BaseCollision* collision);
	BaseCollision* GetCollision() const;

	void HandleCollision(Physics* OtherPhysics);
	Collision::Box& GetBox();


	void Update();

private:

	float			m_Mass;
	DX XMFLOAT3		m_Position;
	DX XMFLOAT3		m_PrevPos;
	DX XMFLOAT3		m_Force;
	DX XMFLOAT3		m_Velocity;
	float			m_Gravity;
	float			m_Friction;
	Collision::Box	m_Box;
	BaseCollision*	m_Collision;
	
};