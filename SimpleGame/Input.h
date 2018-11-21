#pragma once
#include "Gamepad.h"

/* Devices */
#define KEYBOARD	0x01
#define GAMEPAD		0x02
#define IS_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)

class Key
{
	friend Input;
public:
	Key(int Value, const id_type& CommandID) :
		Value(Value),
		Released(true),
		CommandID(CommandID) {}

	void HandleInput(bool Pressed, const id_type & ActorID);

private:

	int Value;
	int Released;
	id_type CommandID;

};

using Button = Key;

class Analog
{
	friend Input;
public:
	Analog(const id_type& CommandID) :
		CommandID(CommandID) {}

	void HandleInput(const id_type & ActorID);

private:
	id_type CommandID;
};

class Input
{
	friend Cyan;
	friend State;

public:
	Input()  {}
	~Input() {}

	void AddKeyboardInput(int value, const id_type& CommandID);
	void AddGamepadInput(int value, const id_type& CommandID);
	void AddAnalogInput(const id_type& CommandID);

private:

	void HandleInput(const id_type& ActorID);

private:
	STD vector<Key>		KeyboardInput;	// Keyboard Key Input
	STD vector<Button>	GamepadInput;	// Gamepad Key (button) Input
	STD vector<Analog>	AnalogInput;	// Analog Input
};