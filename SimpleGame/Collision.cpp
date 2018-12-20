#include "stdafx.h"
#include "Collision.h"
#include "CyanEngine.h"

void SSE_CALLCONV Collision::BBox::SetDimensions(SSE_VECTOR_PARAM1 v)
{
	Size = StoreFloat3(v);
}


 SSE_VECTOR SSE_CALLCONV Collision::BBox::GetDimensions() const
{
	return LoadFloat3(Size);
}


void SSE_CALLCONV BasicCollision::OnCollision
(
	const IDType& MyID,
	Physics* MyBody,
	const IDType& CollidingID,
	Physics* CollidingBody,
	SSE_VECTOR_PARAM1 CollisionNormal
) 
{
	 SSE_VECTOR v =  Add( Scale(CollisionNormal, -1.f),  VectorOne());
	CollidingBody->SetDeltaPosition( Multiply(CollidingBody->GetDeltaPosition(), v));
	CollidingBody->SetVelocity( Multiply(CollidingBody->GetVelocity(), v));
}


void SSE_CALLCONV PlayerCollision::OnCollision
(
	const IDType& MyID,
	Physics* MyBody,
	const IDType& CollidingID,
	Physics* CollidingBody,
	SSE_VECTOR_PARAM1 CollisionNormal
)
{
}

void SSE_CALLCONV ProjectileCollision::OnCollision
(
	const IDType& MyID,
	Physics* MyBody,
	const IDType& CollidingID,
	Physics* CollidingBody,
	SSE_VECTOR_PARAM1 CollisionNormal
)
{
	Descriptor& MyDesc = Engine.GetDescriptor(MyID);
	Descriptor& CollidingDesc = Engine.GetDescriptor(CollidingID);

	if (MyDesc.Team != CollidingDesc.Team && CollidingDesc.Type == ObjectType::Actor 
		&& Engine.GetCurrentState(CollidingID)->Name() != "Damaged")
	{
		CollidingDesc.Value -= MyDesc.Value;
		Engine.ChangeState(CollidingID, "Damaged");
		Engine.DeleteObject(MyID);
	}
	else if (CollidingDesc.Type == ObjectType::Structure)
	{
		Engine.DeleteObject(MyID);
	}
}

void SSE_CALLCONV StructureCollision::OnCollision
(
	const IDType& MyID,
	Physics* MyBody,
	const IDType& CollidingID,
	Physics* CollidingBody,
	SSE_VECTOR_PARAM1 CollisionNormal
) 
{
	BasicCollision::OnCollision(MyID, MyBody, CollidingID, CollidingBody, CollisionNormal);
}

void SSE_CALLCONV ExplosionCollision::OnCollision
(
	const IDType& MyID,
	Physics * MyBody, 
	const IDType& CollidingID,
	Physics * CollidingBody, 
	SSE_VECTOR_PARAM1 CollisionNormal
)
{
	Descriptor& MyDesc = Engine.GetDescriptor(MyID);
	Descriptor& CollidingDesc = Engine.GetDescriptor(CollidingID);
	
	if (MyDesc.Team != CollidingDesc.Team && CollidingDesc.Type == ObjectType::Actor 
		&& Engine.GetCurrentState(CollidingID)->Name() != "Damaged")
	{
		SSE_VECTOR v =  Subtract(CollidingBody->GetPosition(), MyBody->GetPosition());
		v = Normalize2(v);
		v =  Scale(v, 100'000.f);
		v =  Add(v, { 0.f, 0.f, 1'000.f });
		CollidingBody->ApplyForce(v);
		Engine.ChangeState(CollidingID, "Damaged");
		CollidingDesc.Value -= MyDesc.Value;
	}
}

void SSE_CALLCONV MonsterCollision::OnCollision
(
	const IDType& MyID,
	Physics * MyBody, 
	const IDType& CollidingID,
	Physics * CollidingBody, 
	SSE_VECTOR_PARAM1 CollisionNormal)
{
	Descriptor& MyDesc = Engine.GetDescriptor(MyID);
	Descriptor& CollidingDesc = Engine.GetDescriptor(CollidingID);
	if (MyDesc.Team != CollidingDesc.Team && CollidingDesc.Type == ObjectType::Actor 
		&& Engine.GetCurrentState(CollidingID)->Name() != "Damaged")
	{
		SSE_VECTOR Coll_Vel = CollidingBody->GetVelocity();
		SSE_VECTOR Resultant = Subtract(Coll_Vel, Scale(CollisionNormal, 2.f * Dot2(Coll_Vel, CollisionNormal)));
		CollidingBody->SetVelocity(Resultant);
		Engine.ChangeState(CollidingID, "Damaged");
		CollidingDesc.Value -= 5.f;
	}
}
