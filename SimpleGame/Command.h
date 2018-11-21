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
	ShootCommand(u_int TexID, DX FXMVECTOR Force, int State) :
		TexID(TexID), Created(false),
		Force(DX3 Store(Force)), 
		State(State), Released(true), Size(0.125f) {}
	virtual ~ShootCommand() {}

	virtual void execute(const id_type& ActorID);
	virtual void release(const id_type& ActorID);

private:
	bool Released;
	bool Created;
	DX XMFLOAT3 Force;
	u_int BulletID;
	u_int TexID;
	float Size;
	int State;
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

	virtual void execute(const id_type& ActorID);

private:
	id_type NewStateID;
};

class NewStateOnReleaseCommand : public Command
{
public:
	NewStateOnReleaseCommand(const id_type& NewStateID) : NewStateID(NewStateID) {}
	~NewStateOnReleaseCommand() {}

	virtual void release(const id_type& ActorID);

private:
	id_type NewStateID;
};