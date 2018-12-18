#pragma once
//#include "Gamepad.h"
#define KEY_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)

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