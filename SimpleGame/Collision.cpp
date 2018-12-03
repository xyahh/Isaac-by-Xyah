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


void XM_CALLCONV BasicCollision::OnCollision
(
	size_t MyID,
	Physics* MyBody,
	size_t CollidingID,
	Physics* CollidingBody,
	DX FXMVECTOR CollisionNormal
) 
{
	DX XMVECTOR v = DX Add(DX Scale(CollisionNormal, -1.f), DX XMVectorSplatOne());
	CollidingBody->SetDeltaPosition(DX Multiply(CollidingBody->GetDeltaPosition(), v));
	CollidingBody->SetVelocity(DX Multiply(CollidingBody->GetVelocity(), v));
}


void XM_CALLCONV ActorCollision::OnCollision
(
	size_t MyID,
	Physics* MyBody,
	size_t CollidingID,
	Physics* CollidingBody,
	DX FXMVECTOR CollisionNormal
)
{
	//Fill Here
}

void XM_CALLCONV ProjectileCollision::OnCollision
(
	size_t MyID,
	Physics* MyBody,
	size_t CollidingID,
	Physics* CollidingBody,
	DX FXMVECTOR CollisionNormal
)
{
	//Fill Here
}

void XM_CALLCONV StructureCollision::OnCollision
(
	size_t MyID,
	Physics* MyBody,
	size_t CollidingID,
	Physics* CollidingBody,
	DX FXMVECTOR CollisionNormal
) 
{
	BasicCollision::OnCollision(MyID, MyBody, CollidingID, CollidingBody, CollisionNormal);
	float MyFriction = Engine.GetDescriptor(MyID).GetValue();
	float CollidingFriction = CollidingBody->GetFriction();
	if (MyFriction > CollidingFriction)
		CollidingBody->SetFriction(MyFriction); 
		
}
