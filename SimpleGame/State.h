#pragma once
#include "Input.h"

/*---------------------------------------------------------------------------------------------*/
class StateStruct
{
public:
	StateStruct(const id_type& ActorID, const id_type& StateID, State* state) :
		ActorID(ActorID), StateID(StateID), pState(state) {}

	id_type ActorID;
	id_type StateID;
	State*	pState;
};
/*---------------------------------------------------------------------------------------------*/


class State
{
	friend Cyan;

public:
	State() {}
	virtual	~State() {}

	virtual void Enter(const id_type& ActorID) = 0;
	virtual void Exit(const id_type& ActorID) = 0;
	virtual void Update(const id_type& ActorID);

	Input&  GetInput(const id_type& ActorID);

protected:
	/* Helper Functions for Child States */
	void ChangeState(const id_type& ActorID, const id_type& NewStateID);
	u_int XM_CALLCONV GetVector2Direction(DX FXMVECTOR v);
	u_int GetActorFacingDirection(const id_type& ActorID);
	DX XMVECTOR GetDirectionVector2(u_int Direction);

private:

	virtual State* Clone() = 0;
	u_int	m_Direction;
	STD map<id_type, Input>	m_Input;
	
};

class GlobalState : public State
{
public:

	GlobalState() {}
	virtual ~GlobalState() {}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		return new GlobalState;
	}

private:

	virtual State* Clone()	{	return new GlobalState; }
};

class IdleState : public State
{
public:

	IdleState() 
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("IdleState Created!\n"); 
#endif
	}
	virtual ~IdleState() 
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("IdleState Destroyed!\n"); 
#endif
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		return new IdleState;
	}

private:
	virtual State* Clone()	{ return new IdleState; }
};

class MovingState : public State
{
public:

	MovingState() 
	{
#ifdef CYAN_DEBUG_STATES
		printf("MoveState Created!\n");
#endif 
	}
	virtual ~MovingState() 
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("MoveState Destroyed!\n"); 
#endif 
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		return new MovingState;
	}

private:
	virtual State* Clone() { return new MovingState; }
};

class InAirState : public State
{
public:

	InAirState()
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Created!\n"); 
#endif
	}
	virtual ~InAirState()
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Destroyed!\n"); 
#endif
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		return new InAirState;
	}

private:

	virtual State* Clone() { return new InAirState; }
	float GroundFriction;
};

class SlamChargingState : public State
{
public:

	SlamChargingState(float RageRate, const id_type& GroundSlamStateID) :
		RageRate(RageRate), GroundSlamStateID(GroundSlamStateID)
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeSlamState Created!\n");
#endif
	}
	virtual ~SlamChargingState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeSlamState Destroyed!\n");
#endif
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delim);
		float RageRate = STD stof(data);
		STD getline(dataline, data, delim);
		return new SlamChargingState(RageRate, data);
	}

private:
	virtual State* Clone() { return new SlamChargingState(RageRate, GroundSlamStateID); }
	float	RageRate;
	float	RageAmount;
	float	Gravity;
	id_type GroundSlamStateID;
};

class SlammingState : public State
{
public:

	SlammingState(const id_type& TexID, float SlamForce) :
		TexID(TexID), SlamForce(SlamForce)
	{
#ifdef CYAN_DEBUG_STATES
		printf("GroundSlamState Created!\n");
#endif
	}
	virtual ~SlammingState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("GroundSlamState Destroyed!\n");
#endif
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

	static State* Make(const STD string& Args, char delim)
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delim);
		std::string TexID = data;
		STD getline(dataline, data, delim);
		float SlamForce = STD stof(data);
		return new SlammingState(TexID, SlamForce);
	}

private:

	virtual State* Clone() { return new SlammingState(TexID, SlamForce); }

private:
	float SlamForce;
	u_int EffectID;
	id_type TexID;
};

class ShootingState : public State
{
public:

	ShootingState(const id_type& TexID, float ShootingRate, float Force) :
		TexID(TexID), GrowingRate(ShootingRate), Force(Force)
	{
#ifdef CYAN_DEBUG_STATES
		printf("ShootingState Created!\n");
#endif
	}
	virtual ~ShootingState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("ShootingState Destroyed!\n");
#endif
	}

	static State* Make(const STD string& Args, char delim)
	{
		STD string data;
		STD istringstream dataline(Args);
		STD getline(dataline, data, delim);
		std::string TexID = data;
		STD getline(dataline, data, delim);
		float GrowingRate = STD stof(data);
		STD getline(dataline, data, delim);
		float Force = STD stof(data);
		return new ShootingState(TexID, GrowingRate, Force);
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);


private:
	virtual State* Clone() { return new ShootingState(TexID, GrowingRate, Force); }
	
	float Growth;
	float GrowingRate;
	float Force;
	id_type BulletID;
	id_type TexID;
};


/*---------------------------------------------------------------------------------------------*/
const STD pair<STD string, State*(*)(const STD string&, char)> StateTypes[] =
{
{"Global"	, GlobalState::Make },
{"Idle"		, IdleState::Make },
{"Moving"	, MovingState::Make },
{"InAir"	, InAirState::Make },
{"Slamming"	, SlammingState::Make },
{"SlamCharging", SlamChargingState::Make },
{"Shooting", ShootingState::Make }
};
/*---------------------------------------------------------------------------------------------*/
