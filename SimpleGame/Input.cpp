#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

void Input::AddKey(int Value, size_t CommandIndex)
{
	m_Controls.emplace(Value, CommandIndex);
}

void Input::EmplaceLocalInput()
{
	m_LocalInput.clear();
	for (auto& Keys : m_Controls)
		m_LocalInput.emplace(Keys.first, KeyState{ false, true });
}

void Input::ClearLocalInput()
{
	m_LocalInput.clear();
}

void Input::ReceiveLocalInput()
{
	for (auto& i : m_LocalInput)
	{
		if (KEY_PRESSED(i.first))
		{
			if (!i.second.Pressed)
			{
				m_Input.emplace(i.first);
				i.second.Pressed = true;
				i.second.Released = false;
			}

		}
		else if (!i.second.Released && i.second.Pressed)
		{
			m_Input.erase(i.first);
			m_ReleasedKeys.emplace(i.first);
			i.second.Released = true;
			i.second.Pressed = false;
		}

	}

}

void Input::ReceiveForeignInput(const KeyInfo & Key)
{
	if (Key.Pressed)
		m_Input.emplace(Key.Value);
	else
	{
		m_Input.erase(Key.Value);
		m_ReleasedKeys.emplace(Key.Value);
	}
}

void Input::ProcessInput(size_t ObjectIndex)
{
	for (auto& Keys : m_Input)
		m_PushedKeys.emplace(Keys);

	while (!m_PushedKeys.empty())
	{
		auto& Keys = m_Controls.equal_range(m_PushedKeys.top());
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->execute(ObjectIndex);
		m_PushedKeys.pop();
	}
	while (!m_ReleasedKeys.empty())
	{
		auto& Keys = m_Controls.equal_range(m_ReleasedKeys.top());
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->release (ObjectIndex);
		m_ReleasedKeys.pop();
	}

}
