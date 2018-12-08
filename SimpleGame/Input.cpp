#include "stdafx.h"
#include "Input.h"
#include "CyanEngine.h"

void Input::DefineInput(int Value)
{

	m_Input.emplace(Value, false);
}

void Input::ProcessInput()
{
	for (auto& i : m_Input)
	{
		if (KEY_PRESSED(i.first))
		{
			if (!i.second)
			{
				m_Pushed.emplace_back(i.first);
				i.second = true;
			}
	
		}
		else if (i.second)
		{
			EraseByValue(m_Pushed, i.first);
			m_Released.emplace_back(i.first);
			i.second = false;
		}
	}
}