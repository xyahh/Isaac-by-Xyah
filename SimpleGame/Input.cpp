#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

void Input::DefineUsefulInput(int Value)
{
	m_FilteredInput.emplace(Value, KeyState{ false, true });
}

void Input::ProcessInput(size_t ObjectIndex)
{
	//for (auto& i : m_FilteredInput)
	//{
	//	if (KEY_PRESSED(i.first))
	//	{
	//		if (!i.second.Pressed)
	//		{
	//			printf("PRESSED [%d] IN STATE [%d]\n", i.first, Engine.m_States[ObjectIndex].top()->Name());
	//			m_PushedKeys.emplace_back(i.first);
	//			i.second.Pressed = true;
	//			i.second.Released = false;
	//		}
	//
	//	}
	//	else if (!i.second.Released && i.second.Pressed)
	//	{
	//		printf("RELEASED [%d] IN STATE [%d]\n", i.first, Engine.m_States[ObjectIndex].top()->Name());
	//		m_PushedKeys.erase(STD remove(m_PushedKeys.begin(), m_PushedKeys.end(), i.first), m_PushedKeys.end());
	//		m_ReleasedKeys.emplace_back(i.first);
	//		i.second.Released = true;
	//		i.second.Pressed = false;
	//	}
	//
	//}
	//
	//m_ReleasedKeys.clear();
}