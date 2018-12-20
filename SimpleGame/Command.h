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

	virtual void execute(const IDType& Index) {};
	virtual void release(const IDType& Index) {};

};

class ForceCommand : public Command
{
public:
	ForceCommand(float x, float y, float z) :
		Force(x, y, z) {}
	virtual ~ForceCommand() {}
	
	virtual void execute(const IDType& Index);
	virtual void release(const IDType& Index) {}

private:
	 FLOAT3 Force;
};

class FaceCommand : public Command
{
public:
	FaceCommand(const STD string& SpriteName, u_int Direction) :
		Direction(Direction), SpriteName(SpriteName){}
	virtual ~FaceCommand() {}

	virtual void execute(const IDType& Index);
	virtual void release(const IDType& Index) {}

private:
	STD string SpriteName;
	u_int Direction;
};

class StateCommand : public Command
{
public:
	StateCommand(const STD string& StateName, DWORD Config = ST_CMD::ON_PRESS | ST_CMD::CHANGE_STATE) :
		StateName(StateName),
		Config(Config) {}
	virtual ~StateCommand() {}

	virtual void execute(const IDType& Index);
	virtual void release(const IDType& Index);

private:
	DWORD  Config;
	STD string StateName;
};

template<class T>
class SceneCommand : public Command
{
public:
	SceneCommand() {}
	virtual ~SceneCommand() {}
	virtual void execute(size_t Index) { Engine.GetFramework().PlayScene<T>(); }
};

