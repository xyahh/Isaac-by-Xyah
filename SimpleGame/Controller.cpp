#include "stdafx.h"
#include "Controller.h"
#include "CyanEngine.h"

void Controller::MapControl(int Value, size_t CommandIndex)
{
	m_Controls.emplace(Value, CommandIndex);
}

void Controller::HandleControls(size_t ObjectIndex,
	const STD vector<int>& Pressed, 
	const STD vector<int>& Released)
{
	for (auto& k : Pressed)
	{
		auto& Keys = m_Controls.equal_range(k);
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->execute(ObjectIndex);
	}

	for (auto& k : Released)
	{
		auto& Keys = m_Controls.equal_range(k);
		for (auto Iter = Keys.first; Iter != Keys.second; ++Iter)
			Engine.GetCommand(Iter->second)->release(ObjectIndex);
	}
}
