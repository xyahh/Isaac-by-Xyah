#pragma once
#include "CyanEngine.h"

enum ST_CMD
{
	ON_PRESS		= 0x01,
	ON_RELEASE		= 0x02,
	PUSH_STATE		= 0x04,
	CHANGE_STATE	= 0x08,
	POP_STATE		= 0x10,
};

class Command
{
public:
	Command() = default;
	virtual ~Command() {}

	virtual void execute(size_t Index) {};
	virtual void release(size_t Index) {};

};

class ForceCommand : public Command
{
public:
	ForceCommand(float x, float y, float z) :
		Force(x, y, z) {}
	virtual ~ForceCommand() {}
	
	virtual void execute(size_t Index);
	virtual void release(size_t Index) {}

private:
	DX XMFLOAT3 Force;
};

class FaceCommand : public Command
{
public:
	FaceCommand(size_t SpriteIndex, u_int Direction) :
		Direction(Direction), SpriteIndex(SpriteIndex){}
	virtual ~FaceCommand() {}

	virtual void execute(size_t Index);
	virtual void release(size_t Index) {}

private:
	size_t SpriteIndex;
	u_int Direction;
};

class StateCommand : public Command
{
public:
	StateCommand(size_t StateID, DWORD Config = ST_CMD::ON_PRESS | ST_CMD::CHANGE_STATE) :
		StateIndex(StateID),
		Config(Config) {}
	virtual ~StateCommand() {}

	virtual void execute(size_t Index);
	virtual void release(size_t Index);

private:
	DWORD  Config;
	size_t StateIndex;
};

template<class T>
class SceneCommand : public Command
{
public:
	SceneCommand() {}
	virtual ~SceneCommand() {}
	virtual void execute(size_t Index) { Engine.GetFramework().PlayScene<T>(); }
};

