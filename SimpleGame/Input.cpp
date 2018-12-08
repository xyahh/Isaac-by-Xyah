#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

void Input::AddKeyMapping(int Value, size_t CommandIndex)
{
	m_Controls.emplace(Value, CommandIndex);
	m_LocalInput.emplace(Value, KeyState{ false, true });
	m_PushedKeys.reserve(m_Controls.size());
	m_ReleasedKeys.reserve(m_Controls.size());
}

void Input::ClearKeys()
{
	m_PushedKeys.clear();
	m_ReleasedKeys.clear();
	for (auto& l : m_LocalInput)
	{
		l.second.Pressed = false;
		l.second.Released = true;
	}
}

void Input::ReceiveForeignInput(const KeyInfo & Key)
{
	//if (Key.Pressed)
	//	m_Input.emplace(Key.Value);
	//else
	//{
	//	m_Input.erase(Key.Value);
	//	m_ReleasedKeys.emplace(Key.Value);
	//}
}

void Input::ProcessInput(size_t ObjectIndex)
{

	for (auto& i : m_LocalInput)
	{
		if (KEY_PRESSED(i.first))
		{
			if (!i.second.Pressed)
			{
				printf("PRESSED [%d] IN STATE [%d]\n", i.first, Engine.m_States[ObjectIndex].top()->Name());
				m_PushedKeys.emplace_back(i.first);
				i.second.Pressed = true;
				i.second.Released = false;
			}

		}
		else if (!i.second.Released && i.second.Pressed)
		{
			printf("RELEASED [%d] IN STATE [%d]\n", i.first, Engine.m_States[ObjectIndex].top()->Name());
			m_PushedKeys.erase(STD remove(m_PushedKeys.begin(), m_PushedKeys.end(), i.first), m_PushedKeys.end());
			m_ReleasedKeys.emplace_back(i.first);
			i.second.Released = true;
			i.second.Pressed = false;
		}

	}

	for (auto& k : m_PushedKeys)
	{
		auto& Keys = m_Controls.equal_range(k);
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->execute(ObjectIndex);
	}

	for (auto& k : m_ReleasedKeys)
	{
		auto& Keys = m_Controls.equal_range(k);
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->release(ObjectIndex);
	}
	m_ReleasedKeys.clear();
}