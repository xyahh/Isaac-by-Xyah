#pragma once
#include "Input.h"

class State
{
	friend Cyan;

public:

	State() {}
	virtual	~State() {}

	virtual void Enter(size_t Index) = 0;
	virtual void Exit(size_t Index) = 0;
	virtual void Update(size_t Index) = 0;
	virtual State* Make() = 0;

protected:

	void HandleInput(size_t ObjectIndex);

	State* Assemble(State* pState)
	{
		pState->pInput = pInput;
		return pState;
	}

	/* 
		Prototypes hold the Data for Input. 
		ObjectStates will only hold a pointer to
		that Data because we do not want to manage the memory
		of Input each time  a State is deleted
		into every Object's state.
	*/
	Input* pInput;
};

class NullState : public State
{
public:

	NullState() {}
	virtual ~NullState() {}

private:

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make()	{	return Assemble(new NullState); }
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

private:

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make()	{ return Assemble(new IdleState); }
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

private:

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make() { return Assemble(new MovingState); }
};

class JumpState : public State
{
public:

	JumpState(float RageRate) : RageRate(RageRate)
	{
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Created!\n");
#endif 
	}
	virtual ~JumpState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("JumpState Destroyed!\n");
#endif 
	}

private:

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make() { return Assemble(new JumpState(RageRate)); }

private:
	float	RageRate;
	float	RageAmount;
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

private:

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make() { return Assemble(new InAirState); }

private:

	float GroundFriction;
};

class SlamChargingState : public State
{
public:
	SlamChargingState(float RageRate, size_t GroundSlamStateID) :
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

private:
	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);

	virtual State* Make() { return Assemble(new SlamChargingState(RageRate, GroundSlamStateID)); }

private:

	float	RageRate;
	float	RageAmount;
	float	Gravity;
	size_t GroundSlamStateID;
};

class SlammingState : public State
{
public:

	SlammingState(size_t TexID, float SlamForce) :
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

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);


private:

	virtual State* Make() { return Assemble(new SlammingState(TexID, SlamForce)); }

private:
	float SlamForce;
	size_t EffectID;
	size_t TexID;
};

class ShootingState : public State
{
public:

	ShootingState(size_t TexID, float ShootingRate, float Force) :
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

	virtual void Enter(size_t ActorID);
	virtual void Update(size_t ActorID);
	virtual void Exit(size_t ActorID);


private:
	virtual State* Make() { return Assemble(new ShootingState(TexID, GrowingRate, Force)); }
	
	float Growth;
	float GrowingRate;
	float Force;
	size_t BulletID;
	size_t TexID;
};
