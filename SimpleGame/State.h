#pragma once
#include "Input.h"


enum STATE
{
	GLOBAL,
	IDLE,
	MOVING,
	JUMPING,
	SLAMMING,
	SLAM_CHARGING
};

const STD pair<u_int, STD string> StateTypes[] =
{
	{STATE::GLOBAL			, "Global"		},
	{STATE::IDLE			, "Idle"		},
	{STATE::MOVING			, "Moving"		},
	{STATE::JUMPING			, "Jumping"		},
	{STATE::SLAMMING		, "Slamming"	},
	{STATE::SLAM_CHARGING	, "SlamCharging"}
};

/*************************************************************/
class StateStruct
{
public:
	StateStruct(const id_type& ActorID, const id_type& StateID, State* state) :
		ActorID(ActorID), StateID(StateID), pState(state) {}

	id_type ActorID;
	id_type StateID;
	State*	pState;
};
/*************************************************************/

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
	DX XMVECTOR GetActorForce(const id_type& ActorID) const;
	u_int GetActorDirection(const id_type& ActorID);

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

private:
	virtual State* Clone() { return new MovingState; }
};

class JumpingState : public State
{
public:

	JumpingState(float JumpForce) :
		JumpForce(JumpForce)
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Created!\n"); 
#endif
	}
	virtual ~JumpingState() 
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Destroyed!\n"); 
#endif
	}

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

private:

	virtual State* Clone() { return new JumpingState(JumpForce); }

	float JumpForce;
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

	SlammingState(const id_type& TexID, float SlamForce, float SlamSize) :
		TexID(TexID), SlamForce(SlamForce), SlamSize(SlamSize)
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

private:

	virtual State* Clone() { return new SlammingState(TexID, SlamForce, SlamSize); }

private:
	float SlamForce;
	float SlamSize;
	u_int EffectID;
	id_type TexID;
};

class ShootingState : public State
{
public:

	ShootingState()
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

	virtual void Enter(const id_type& ActorID);
	virtual void Update(const id_type& ActorID);
	virtual void Exit(const id_type& ActorID);

private:
	virtual State* Clone() { return new ShootingState; }

	float ShootingRate;
	u_int BulletID;
	id_type TexID;
};
