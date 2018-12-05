#pragma once
#include "Gamepad.h"

/* Devices */
#define KEYBOARD	0x01
#define GAMEPAD		0x02

struct KeyInfo
{
	int		Value;
	bool	Pressed;
	int		ClientNum;
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

	void ReceiveInput(const KeyInfo& Key);
	void ProcessInput(size_t ObjectIndex);

private:
	STD multimap<int, size_t>	m_Controls;
	STD vector<int>				m_PushedKeys;
	STD vector<int>				m_ReleasedKeys;

	static STD set<int>			TotalKeys;

};