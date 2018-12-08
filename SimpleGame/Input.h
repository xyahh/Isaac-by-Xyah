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
	friend Cyan;

private:

	void DefineUsefulInput(int Value);
	void ProcessInput(size_t ObjectIndex);

private:
	STD map<int, KeyState>	m_FilteredInput;
	STD vector<int> m_PushedKeys;
	STD vector<int> m_ReleasedKeys;
};