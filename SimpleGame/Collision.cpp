#include "stdafx.h"
#include "Collision.h"
#include "Physics.h"
#include "CyanEngine.h"

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

void XM_CALLCONV BasicCollision::OnCollision(Physics& MyBody, Physics& CollidingBody, DX FXMVECTOR CollisionNormal)
{

	DX XMVECTOR v = DX Add(DX Scale(CollisionNormal, -1.f), DX XMVectorSplatOne());
	CollidingBody.SetDeltaPosition(DX Multiply(CollidingBody.GetDeltaPosition(), v));
	CollidingBody.SetVelocity(DX Multiply(CollidingBody.GetVelocity(), v));
	DX Print(CollidingBody.GetVelocity(), '\n');
}


void XM_CALLCONV ActorCollision::OnCollision(Physics& MyBody, Physics& CollidingBody, DX FXMVECTOR CollisionNormal)
{
	
}


void XM_CALLCONV BulletCollision::OnCollision(Physics & MyBody, Physics & CollidingBody, DX FXMVECTOR CollisionNormal)
{
	u_int OtherObjID = Engine.FindObject(CollidingBody);
	u_int ObjectID = Engine.FindObject(MyBody);
	if (Engine.GetObjectTeam(OtherObjID) != Engine.GetObjectTeam(ObjectID))
		Engine.DeleteObject(ObjectID);
}
