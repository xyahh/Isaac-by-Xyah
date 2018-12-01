#pragma once

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
	ForceCommand(DX FXMVECTOR force) :
		Force(DX3 Store(force)) {}
	virtual ~ForceCommand() {}
	
	virtual void execute(size_t Index);
	virtual void release(size_t Index) {}

private:
	DX XMFLOAT3 Force;
};

class AnalogForceCommand : public Command
{
public:
	AnalogForceCommand(int AnalogStick, float fForce) : 
		AnalogStick(AnalogStick),
		fForce(fForce)
	{}
	virtual ~AnalogForceCommand() {}

	virtual void execute(size_t Index);

private:
	int		AnalogStick;
	float	fForce;
};

class ShootCommand : public Command
{
public:
	ShootCommand(u_int Direction) :
		Direction(Direction) {}
	virtual ~ShootCommand() {}

	virtual void execute(size_t Index);
	virtual void release(size_t Index);

private:
	u_int Direction;
};

class ToSceneCommand : public Command
{
public:
	ToSceneCommand(Scene* scene) :
		m_Scene(scene) {}
	virtual ~ToSceneCommand() {}

	virtual void execute(size_t Index);

private:
	Scene*		m_Scene;
};

enum ST_CMD
{
	ON_PRESS		= 0x01,
	ON_RELEASE		= 0x02,
	PUSH_STATE		= 0x04,
	CHANGE_STATE	= 0x08,
	POP_STATE		= 0x10
};

class StateCommand : public Command
{
public:
	StateCommand(size_t StateID, DWORD Config = ST_CMD::ON_PRESS | ST_CMD::CHANGE_STATE) :
		StateIndex(StateID),
		Config(Config) {}
	~StateCommand() {}

	virtual void execute(size_t Index);
	virtual void release(size_t Index);

private:
	DWORD  Config;
	size_t StateIndex;
};