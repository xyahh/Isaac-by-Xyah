#include "stdafx.h"
#include "Collision.h"
#include "Physics.h"
#include "CyanEngine.h"

void XM_CALLCONV Collision::BBox::SetDimensions(DX FXMVECTOR v)
{
	Size = DX3 Store(v);
}

DX XMVECTOR XM_CALLCONV Collision::BBox::GetDimensions() const
{
	return DX3 Load(Size);
}

DX XMVECTOR XM_CALLCONV Collision::BBox::ConvertPosition(DX FXMVECTOR Position, DX FXMVECTOR Size) const
{
	DX XMVECTOR TransformedPos = Position;
	DX SetZ(&TransformedPos, DX GetZ(TransformedPos) + DX GetZ(Size) * 0.5f);
	return TransformedPos;
}

void XM_CALLCONV BasicCollision::OnCollision(Physics* MyBody, Physics* CollidingBody, DX FXMVECTOR CollisionNormal)
{
	DX XMVECTOR v = DX Add(DX Scale(CollisionNormal, -1.f), DX XMVectorSplatOne());
	CollidingBody->SetDeltaPosition(DX Multiply(CollidingBody->GetDeltaPosition(), v));
	CollidingBody->SetVelocity(DX Multiply(CollidingBody->GetVelocity(), v));
}


void XM_CALLCONV ActorCollision::OnCollision(Physics* MyBody, Physics* CollidingBody, DX FXMVECTOR CollisionNormal)
{
	printf("Actor Collision\n");
}

void XM_CALLCONV BulletCollision::OnCollision(Physics* MyBody, Physics* CollidingBody, DX FXMVECTOR CollisionNormal)
{
	//id_type ActorID = Engine.FindActor(CollidingBody);
	//id_type ObjectID = Engine.FindObject(MyBody);
	//if (ActorID.empty()) 
	//	Engine.DeleteObject(ObjectID);
}
