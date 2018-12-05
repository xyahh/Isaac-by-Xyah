#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

STD set<int>	Input::TotalKeys;

void Input::AddKeyMapping(int Value, size_t CommandIndex)
{
	TotalKeys.emplace(Value);
	m_Controls.emplace(Value, CommandIndex);
}

void Input::ReceiveInput(const KeyInfo & Key)
{
	if (Key.Pressed)
		m_PushedKeys.emplace_back(Key.Value);
	else
	{
		m_ReleasedKeys.emplace_back(Key.Value);
		m_PushedKeys.erase(
			STD remove
			(
				m_PushedKeys.begin(), 
				m_PushedKeys.end(),
				Key.Value
			), 
			m_PushedKeys.end()
		);
	}
}

void Input::ProcessInput(size_t ObjectIndex)
{

	for (auto& Key : m_PushedKeys)
	{
		auto& KeyRange = m_Controls.equal_range(Key);
		for (auto Iter = KeyRange.first; Iter != KeyRange.second; ++Iter)
			Engine.GetCommand(Iter->second)->execute(ObjectIndex);
	}

	for (auto& Key : m_ReleasedKeys)
	{
		auto& KeyRange = m_Controls.equal_range(Key);
		for (auto Iter = KeyRange.first; Iter != KeyRange.second; ++Iter)
			Engine.GetCommand(Iter->second)->release(ObjectIndex);
	}
	m_ReleasedKeys.clear();
}
