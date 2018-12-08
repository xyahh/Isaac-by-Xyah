#include "stdafx.h"
#include "Command.h"

void ForceCommand::execute(size_t ObjectIndex)
{
	Engine.GetPhysics(ObjectIndex).ApplyForce(DX3 Load(Force));
}

void FaceCommand::execute(size_t ObjectIndex)
{
	Engine.GetSprite(ObjectIndex, SpriteIndex).SetDirection(Direction);
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
			return Engine.ChangeState(ObjectIndex, StateIndex);			
		if (Config & ST_CMD::PUSH_STATE)
			return Engine.PushState(ObjectIndex, StateIndex);
		if (Config & ST_CMD::POP_STATE)
			return Engine.PopState(ObjectIndex);
	}
}
