#include "stdafx.h"
#include "Input.h"
#include "CyanEngine.h"

void Input::DefineInput(int Value)
{
	m_Input.emplace(Value, false);
}

void KeyboardInput::ProcessInput()
{
	for (auto& i : m_Input)
	{
		if (KEY_PRESSED(i.first))
		{
			if (!i.second)
			{
				m_Execute.emplace_back(i.first);
				i.second = true;
			}

		}
		else if (i.second)
		{
			EraseByValue(m_Execute, i.first);
			m_Release.emplace_back(i.first);
			i.second = false;
		}
	}
}

void NPCInput::ProcessInput()
{
	STD set<int> Pressed;
	Fx(Pressed);
	for (auto& i : m_Input)
	{
		if (Pressed.find(i.first) != Pressed.end())
		{
			if (!i.second)
			{
				m_Execute.emplace_back(i.first);
				i.second = true;
			}

		}
		else if (i.second)
		{
			EraseByValue(m_Execute, i.first);
			m_Release.emplace_back(i.first);
			i.second = false;
		}
	}

}
