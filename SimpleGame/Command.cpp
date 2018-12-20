#include "stdafx.h"
#include "Command.h"

void ForceCommand::execute(const IDType& ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).ApplyForce(LoadFloat3(Force));
}

void FaceCommand::execute(const IDType& ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, SpriteName).SetDirection(Direction);
}

void StateCommand::execute(const IDType& ObjectIndex)
{
	if (Config & ST_CMD::ON_PRESS)
	{
		if (Config & ST_CMD::CHANGE_STATE)
			return Engine.ChangeState(ObjectIndex, StateName);
		if(Config & ST_CMD::PUSH_STATE)
			return Engine.PushState(ObjectIndex, StateName);
		if (Config & ST_CMD::POP_STATE)
			return Engine.PopState(ObjectIndex);
	}
	
}

void StateCommand::release(const IDType& ObjectIndex)
{
	if (Config & ST_CMD::ON_RELEASE)
	{
		if (Config & ST_CMD::CHANGE_STATE)
			return Engine.ChangeState(ObjectIndex, StateName);
		if (Config & ST_CMD::PUSH_STATE)
			return Engine.PushState(ObjectIndex, StateName);
		if (Config & ST_CMD::POP_STATE)
			return Engine.PopState(ObjectIndex);
	}
}
