#pragma once
class Command
{
public:
	Command() = default;
	virtual ~Command() {}

	virtual void execute(const id_type& ActorID) {};
	virtual void release(const id_type& ActorID) {};

};

class ForceCommand : public Command
{
public:
	ForceCommand(DX FXMVECTOR force) :
		Force(DX3 Store(force)) {}
	virtual ~ForceCommand() {}
	
	static Command* Make(const STD string& Args, char delimiter)
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delimiter);
		float x = STD stof(data);
		STD getline(dataline, data, delimiter);
		float y = STD stof(data);
		STD getline(dataline, data, delimiter);
		float z = STD stof(data);
		return new ForceCommand({ x, y, z });
	}

	virtual void execute(const id_type& ActorID);
	virtual void release(const id_type& ActorID) {}

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

	virtual void execute(const id_type& ActorID);

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

	static Command* Make(const STD string& Args, char delimiter)
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delimiter);
		return new ShootCommand(STD stoi(data));
	}

	virtual void execute(const id_type& ActorID);
	virtual void release(const id_type& ActorID);

private:
	u_int Direction;
};

class ShiftSceneCommand : public Command
{
public:
	ShiftSceneCommand(Framework& fw, Scene* scene) :
		m_Framework(fw),
		m_Scene(scene) {}
	virtual ~ShiftSceneCommand() {}

	virtual void execute(const id_type& ActorID);

private:
	Framework&  m_Framework;
	Scene*		m_Scene;
};

class FxCommand : public Command
{
	using Fx = void(*)(const id_type&);

public:
	
	FxCommand(Fx pFunc) :
		fx(pFunc) {}
	~FxCommand() {}

	virtual void execute(const id_type& ActorID);
	virtual void release(const id_type& ActorID);

private:
	Fx fx;
	bool exe;
};

class NewStateOnPressCommand : public Command
{
public:
	NewStateOnPressCommand(const id_type& NewStateID) : NewStateID(NewStateID) {}
	~NewStateOnPressCommand() {}

	static Command* Make(const STD string& Args, char delimiter = ';')
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delimiter);
		return new NewStateOnPressCommand(data);
	}

	virtual void execute(const id_type& ActorID);

private:
	id_type NewStateID;
};

class NewStateOnReleaseCommand : public Command
{
public:
	NewStateOnReleaseCommand(const id_type& NewStateID) : NewStateID(NewStateID) {}
	~NewStateOnReleaseCommand() {}

	static Command* Make(const STD string& Args, char delimiter = ';')
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delimiter);
		return new NewStateOnReleaseCommand(data);
	}

	virtual void release(const id_type& ActorID);

private:
	id_type NewStateID;
};

/*---------------------------------------------------------------------------------------------*/
const STD pair<STD string, Command*(*)(const STD string& Args, char delimiter)> CommandTypes[] =
{
{"Force", ForceCommand::Make },
{"StateOnPress", NewStateOnPressCommand::Make },
{"StateOnRelease", NewStateOnReleaseCommand::Make },
{"Shoot", ShootCommand::Make }
};
/* Notes: ShiftScene & FxCommand are not Supported. 
(as they need pointers [Scene ptr & Function ptr respectively] to work)*/
/*---------------------------------------------------------------------------------------------*/
