#include "stdafx.h"
#include "Command.h"
#include "CyanEngine.h"
#include "Framework.h"
#include "Gamepad.h"
#include "Scene.h"


void ForceCommand::execute(const id_type& ActorID)
{
	Engine.GetActorPhysics(ActorID).ApplyForce(DX3 Load(Force));
}

void AnalogForceCommand::execute(const id_type& ActorID)
{
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
	Engine.GetActorGraphics(ActorID).Head.SetDirection(Direction);
}

void ShootCommand::release(const id_type& ActorID)
{
}

void NewStateOnPressCommand::execute(const id_type & ActorID)
{
	Engine.UpdateState(ActorID, NewStateID);
}

void NewStateOnReleaseCommand::release(const id_type & ActorID)
{
	Engine.UpdateState(ActorID, NewStateID);
}

