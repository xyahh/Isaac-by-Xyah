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

class Input
{
	friend Cyan;

private:

	void DefineInput(int Value);
	void ProcessInput();

private:

	/* map <KeyValue, Pressed> */
	STD map<int, bool>	m_Input;
	STD vector<int>		m_Pushed;
	STD vector<int>		m_Released;
};