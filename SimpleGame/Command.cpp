#include "stdafx.h"
#include "Command.h"
#include "CyanEngine.h"
#include "Framework.h"
#include "Gamepad.h"
#include "Scene.h"


void ForceCommand::execute(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).ApplyForce(DX3 Load(Force));
}

void AnalogForceCommand::execute(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).ApplyForce(DX Scale(Gamepad1.GetAnalog(AnalogStick), fForce));
}

void ToSceneCommand::execute(size_t ObjectIndex)
{
	Fw.ToScene(&*m_Scene);
}

void ShootCommand::execute(size_t ObjectIndex)
{
	//Engine.GetGraphics(ObjectIndex).GetSprite("Head").SetDirection(Direction);
}

void ShootCommand::release(size_t ObjectIndex)
{
}

void StateCommand::execute(size_t ObjectIndex)
{
	if (Config & ST_CMD::ON_PRESS)
	{
		if (Config & ST_CMD::CHANGE_STATE)
			return Engine.ChangeState(ObjectIndex, StateIndex);
		if(Config & ST_CMD::PUSH_STATE)
			return Engine.PushState(ObjectIndex, StateIndex);
		if (Config & ST_CMD::POP_STATE)
			return Engine.PopState(ObjectIndex);
	}
	
}

void StateCommand::release(size_t ObjectIndex)
{
	if (Config & ST_CMD::ON_RELEASE)
	{
		if (Config & ST_CMD::CHANGE_STATE)
		{
			printf("CHANGE ON RELEASE!\n");
			return Engine.ChangeState(ObjectIndex, StateIndex);
		}
			
		if (Config & ST_CMD::PUSH_STATE)
			return Engine.PushState(ObjectIndex, StateIndex);
		if (Config & ST_CMD::POP_STATE)
			return Engine.PopState(ObjectIndex);
	}
}
