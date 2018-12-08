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

class Controller
{

};


class Input
{
	friend State;
	friend Cyan;

public:
	Input() = default;
	~Input() = default;

	void AddKeyMapping(int Value, size_t CommandIndex);

private:

	void ClearKeys();
	void ReceiveForeignInput(const KeyInfo& Key); //Network Support
	void ProcessInput(size_t ObjectIndex);

private:
	STD multimap<int, size_t>	m_Controls;
	STD map<int, KeyState>		m_LocalInput;
	STD vector<int>				m_PushedKeys;
	STD vector<int>				m_ReleasedKeys;

};