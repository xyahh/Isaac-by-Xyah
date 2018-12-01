#pragma once
#include "Gamepad.h"

/* Devices */
#define KEYBOARD	0x01
#define GAMEPAD		0x02
#define KEY_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)


struct KeyInfo
{
	int		Value;
	bool	Pressed;
	int		ClientNum;
};

struct KeyState
{
	bool Pressed;
	bool Released;
};

class Input
{
	friend State;
	friend Cyan;

public:
	Input()  {}
	~Input() {}

	void AddKey(int Value, size_t CommandIndex);
	void EmplaceLocalInput();
	void ClearLocalInput();

private:

	void ReceiveLocalInput(); 
	void ReceiveForeignInput(const KeyInfo& Key); //Network Support
	void ProcessInput(size_t ObjectIndex);

private:
	STD map<int, KeyState>		m_LocalInput;
	STD multimap<int, size_t>	m_Controls;
	STD set<int>	m_Input;
	STD stack<int>	m_PushedKeys;
	STD stack<int>	m_ReleasedKeys;

};