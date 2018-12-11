#include "stdafx.h"
#include "Collision.h"
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
	Descriptor& MyDesc = Engine.GetDescriptor(MyID);
	Descriptor& CollidingDesc = Engine.GetDescriptor(CollidingID);

	if (MyDesc.Team != CollidingDesc.Team && CollidingDesc.Type == ObjectType::Actor)
	{
		CollidingDesc.Value -= MyDesc.Value;
		Engine.ChangeState(CollidingID, ST::DAMAGED);
		Engine.DeleteObject(MyID);
	}
	else if (CollidingDesc.Type == ObjectType::Structure)
	{
		Engine.DeleteObject(MyID);
	}
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
}

void XM_CALLCONV ExplosionCollision::OnCollision
(
	size_t MyID, 
	Physics * MyBody, 
	size_t CollidingID, 
	Physics * CollidingBody, 
	DX FXMVECTOR CollisionNormal
)
{
	Descriptor& MyDesc = Engine.GetDescriptor(MyID);
	Descriptor& CollidingDesc = Engine.GetDescriptor(CollidingID);
	
	if (MyDesc.Team != CollidingDesc.Team && CollidingDesc.Type == ObjectType::Actor)
	{
		DX XMVECTOR v = DX Subtract(CollidingBody->GetPosition(), MyBody->GetPosition());
		v = DX2 Normalize(v);
		v = DX Scale(v, 100'000.f);
		v = DX Add(v, { 0.f, 0.f, 1'000.f });
		CollidingBody->ApplyForce(v);
		Engine.ChangeState(CollidingID, ST::DAMAGED);
		CollidingDesc.Value -= MyDesc.Value;
	}

}
