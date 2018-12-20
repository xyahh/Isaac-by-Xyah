#pragma once
#define NS_STATE_START namespace STATE {
#define NS_STATE_END }

enum StateType
{
	None,
	Global,
	Idle,
	Move,
	ChargeJump,
	InAir,
	ChargeSlam,
	Slam,
	Shoot,
	Damaged,
};

class State
{
	friend Cyan;

public:

	State() = default;
	virtual	~State() = default;

	virtual void Enter(const IDType& Index) = 0;
	virtual void Exit(const IDType& Index) = 0;
	virtual void Update(const IDType& Index) = 0;
	virtual State* Make() = 0;

	virtual StateType Type() const = 0;


protected:
	State* Assemble(State* pState)
	{
		return pState;
	}
};

class GlobalState : public State
{
public:

	GlobalState() = default;
	virtual ~GlobalState() = default;

private:

	virtual void Enter(const IDType& ObjectIndex) {}
	virtual void Update(const IDType& ObjectIndex) {}
	virtual void Exit(const IDType& ObjectIndex) {}

	virtual StateType Type() const { return StateType::Global; }

	virtual State* Make() { return Assemble(new GlobalState); }
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

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::Idle; }

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

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::Move; }

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

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);
	
	virtual StateType Type() const { return StateType::ChargeJump; }

	virtual State* Make() { return Assemble(new ChargeJumpState(RageRate, Force)); }

private:
	float	RageRate;
	float	RageAmount;
	float	Force;
};

class InAirState : public State
{
public:

	InAirState(float AirResistance) :
		AirResistance(AirResistance)
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

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::InAir; }

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
	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::ChargeSlam; }

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

private:

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::Slam; }

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

private:

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::Shoot; }

	virtual State* Make() { return Assemble(new ShootState(ShootingRate, ShootingForce)); }
	
	float Time;
	float ShootingRate;
	float ShootingForce;
	u_int TexID;
};

class DamagedState : public State
{
public:

	DamagedState(float Duration, float BlinkingRate) :
		Duration(Duration), BlinkingRate(BlinkingRate)
	{
#ifdef CYAN_DEBUG_STATES
		printf("DamagedState Created!\n");
#endif
	}
	virtual ~DamagedState()
	{
#ifdef CYAN_DEBUG_STATES
		printf("DamagedState Destroyed!\n");
#endif
	}

private:

	virtual void Enter(const IDType& ObjectIndex);
	virtual void Update(const IDType& ObjectIndex);
	virtual void Exit(const IDType& ObjectIndex);

	virtual StateType Type() const { return StateType::Damaged; }
	virtual State* Make() { return Assemble(new DamagedState(Duration, BlinkingRate)); }


	float Duration;
	float BlinkingRate;
	float BlinkingTimer;
	float DurationTimer;
	int Alpha;
};

NS_STATE_START

extern GlobalState Global;
extern IdleState Idle;
extern MoveState Move;
extern ChargeJumpState ChargeJump;
extern InAirState InAir;
extern ChargeSlamState ChargeSlam;
extern SlamState Slam;
extern ShootState Shoot;
extern DamagedState Damaged;

NS_STATE_END