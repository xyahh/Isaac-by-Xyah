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

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

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

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

	virtual State* Make()	{ return Assemble(new IdleState); }
};

class MoveState : public State
{
public:

	MoveState() 
	{
#ifdef CYAN_DEBUG_STATES
		printf("MoveState Created!\n");
#endif 
	}
	virtual ~MoveState() 
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("MoveState Destroyed!\n"); 
#endif 
	}

private:

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

	virtual State* Make() { return Assemble(new MoveState); }
};

class ChargeJumpState : public State
{
public:

	ChargeJumpState(float RageRate, float Force) : RageRate(RageRate), Force(Force)
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeJumpState Created!\n");
#endif 
	}
	virtual ~ChargeJumpState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeJumpState Destroyed!\n");
#endif 
	}

private:

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

	virtual State* Make() { return Assemble(new ChargeJumpState(RageRate, Force)); }

private:
	float	RageRate;
	float	RageAmount;
	float	Force;
};

class InAirState : public State
{
public:

	InAirState(float AirResistance)
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("InAirState Created!\n"); 
#endif
	}
	virtual ~InAirState()
	{ 
#ifdef CYAN_DEBUG_STATES
		printf("InAirState Destroyed!\n"); 
#endif
	}

private:

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

	virtual State* Make() { return Assemble(new InAirState(AirResistance)); }

private:
	float AirResistance;
	float GroundFriction;
};

class ChargeSlamState : public State
{
public:
	ChargeSlamState(float RageRate) :
		RageRate(RageRate)
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeSlamState Created!\n");
#endif
	}
	virtual ~ChargeSlamState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("ChargeSlamState Destroyed!\n");
#endif
	}

private:
	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);

	virtual State* Make() { return Assemble(new ChargeSlamState(RageRate)); }

private:
	float Force;
	float RageRate;
	float RageAmount;
	float ObjGravity;
};

class SlamState : public State
{
public:

	SlamState(float SlamForce) :
		SlamForce(SlamForce)
	{
#ifdef CYAN_DEBUG_STATES
		printf("SlamState Created!\n");
#endif
	}
	virtual ~SlamState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("SlamState Destroyed!\n");
#endif
	}

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);


private:

	virtual State* Make() { return Assemble(new SlamState(SlamForce)); }

private:
	float SlamForce;
};

class ShootState : public State
{
public:

	ShootState(float ShootingRate, float ShootingForce) :
		TexID(TexID), ShootingRate(ShootingRate), ShootingForce(ShootingForce)
	{
#ifdef CYAN_DEBUG_STATES
		printf("ShootingState Created!\n");
#endif
	}
	virtual ~ShootState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("ShootingState Destroyed!\n");
#endif
	}

	virtual void Enter(size_t ObjectIndex);
	virtual void Update(size_t ObjectIndex);
	virtual void Exit(size_t ObjectIndex);


private:
	virtual State* Make() { return Assemble(new ShootState(ShootingRate, ShootingForce)); }
	
	float Time;
	float ShootingRate;
	float ShootingForce;
	size_t BulletID;
	size_t TexID;
};
