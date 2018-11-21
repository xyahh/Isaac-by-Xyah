#include "stdafx.h"
#include "Collision.h"
#include "Physics.h"


void XM_CALLCONV Collision::Box::SetDimensions(DX FXMVECTOR v)
{
	Size = DX3 Store(v);
}

DX XMVECTOR XM_CALLCONV Collision::Box::GetDimensions() const
{
	return DX3 Load(Size);
}

DX XMVECTOR XM_CALLCONV Collision::Box::ConvertPosition(DX FXMVECTOR Position) const
{
	DX XMVECTOR TransformedPos = Position;
	DX SetZ(&TransformedPos, DX GetZ(TransformedPos) + Size.z * 0.5f);
	return TransformedPos;
}

void BaseCollision::OnCollision(Physics& MyBody, Physics& CollidingBody)
{
	CollidingBody.SetPosition(CollidingBody.GetPrevPosition());
	CollidingBody.SetVelocity(DX Multiply(CollidingBody.GetVelocity(), { 0.f, 0.f, 1.f }));
}


void ActorCollision::OnCollision(Physics& MyBody, Physics& CollidingBody)
{
	CollidingBody.SetPosition(CollidingBody.GetPrevPosition());
	CollidingBody.SetVelocity(DX Multiply(CollidingBody.GetVelocity(), { 0.f, 0.f, 1.f }));
}


void VentCollision::OnCollision(Physics& MyBody, Physics& CollidingBody)
{
	//CollidedEntity->SetForce({ 0.f, 0.f, 1'000.f });
}
