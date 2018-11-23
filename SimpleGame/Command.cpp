#include "stdafx.h"
#include "Command.h"
#include "CyanEngine.h"
#include "Framework.h"
#include "Gamepad.h"
#include "Scene.h"


void ForceCommand::execute(const id_type& ActorID)
{
	if (ActorID.empty()) return;
	Engine.GetActorPhysics(ActorID).ApplyForce(DX3 Load(Force));
}

void AnalogForceCommand::execute(const id_type& ActorID)
{
	if (ActorID.empty()) return;
	Engine.GetActorPhysics(ActorID).ApplyForce(DX Scale(Gamepad1.GetAnalog(AnalogStick), fForce));
}

void ShiftSceneCommand::execute(const id_type& ActorID)
{
	m_Framework.ToScene(&*m_Scene);
}

void FxCommand::execute(const id_type& ActorID)
{
	if (!exe)
	{
		fx(ActorID);
		exe = true;
	}
	
}

void FxCommand::release(const id_type& ActorID)
{
	exe = false;
}

void ShootCommand::execute(const id_type& ActorID)
{
	if (ActorID.empty()) return;
	Engine.GetActorGraphics(ActorID).Head.SetDirection(State);

		
	DX XMVECTOR v = Engine.GetActorPhysics(ActorID).GetPosition();
	
	//Adjust Position of the Ball (ontop of the right shoulder)
	switch (State)
	{
		case 0:	 Engine.GetObjectPhysics(BulletID).SetPosition(DX Add(v, {  0.25f, -0.10f, 0.25f})); break;
		case 1:	 Engine.GetObjectPhysics(BulletID).SetPosition(DX Add(v, { -0.25f,  0.10f, 0.25f})); break;
		case 2:	 Engine.GetObjectPhysics(BulletID).SetPosition(DX Add(v, {  0.25f,  0.10f, 0.25f})); break;
		case 3:  Engine.GetObjectPhysics(BulletID).SetPosition(DX Add(v, { -0.25f, -0.10f, 0.25f})); break;
	}	
}

void ShootCommand::release(const id_type& ActorID)
{
	if (ActorID.size() > 0)
	{
		Engine.GetSound("ShootRel").Play();
		Physics& BulletPhysics = Engine.GetObjectPhysics(BulletID);
		BulletPhysics.ApplyForce(DX3 Load(Force));
		DX XMVECTOR v = Engine.GetActorPhysics(ActorID).GetVelocity();
		BulletPhysics.SetVelocity(v);
		Released = true;
		Created = false;
		Size = 0.125f;
		
	}
}

void NewStateOnPressCommand::execute(const id_type & ActorID)
{
	if (ActorID.empty()) return;
	Engine.UpdateState(ActorID, NewStateID);
}

void NewStateOnReleaseCommand::release(const id_type & ActorID)
{
	if (ActorID.empty()) return;
	Engine.UpdateState(ActorID, NewStateID);
}

