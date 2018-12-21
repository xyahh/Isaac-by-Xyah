#pragma once
//#include "Gamepad.h"
#define KEY_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)

class Input
{
	friend Cyan;

protected:

	void DefineInput(int Value);
	virtual void ProcessInput() = 0;

	/* map <KeyValue, Pressed> */
	STD map<int, bool>	m_Input;
	STD vector<int>		m_Execute;
	STD vector<int>		m_Release;
};

class KeyboardInput : public Input
{
public:

	virtual void ProcessInput();
};

class NPCInput : public Input
{
	STD function<void(STD set<int>&)> Fx;

public:

	NPCInput(const STD function<void(STD set<int>&)> Pressed)
		: Fx(Pressed) {}
	virtual	void ProcessInput();
};

