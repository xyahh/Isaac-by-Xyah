#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

void Input::AddKeyboardInput(int value, const id_type & CommandID)
{
	KeyboardInput.emplace_back(value, CommandID);
}

void Input::AddGamepadInput(int value, const id_type & CommandID)
{
	GamepadInput.emplace_back(value, CommandID);
}

void Input::AddAnalogInput(const id_type& CommandID)
{
	AnalogInput.emplace_back(CommandID);
}

void Input::HandleInput(const id_type& ActorID)
{
	Gamepad1.Update();

	for (auto& i : GamepadInput)
		i.HandleInput(Gamepad1.GetButton(i.Value), ActorID);
	for (auto& i : KeyboardInput)
		i.HandleInput(IS_PRESSED(i.Value), ActorID);
	for (auto& i : AnalogInput)
		i.HandleInput(ActorID);
}

void Key::HandleInput(bool Pressed, const id_type & ActorID)
{
	if (Pressed)
	{
		Engine.GetCommand(CommandID)->execute(ActorID);
		Released = false;
	}
	else if (!Released)
	{
		Engine.GetCommand(CommandID)->release(ActorID);
		Released = true;
	}
}

void Analog::HandleInput(const id_type & ActorID)
{
	Engine.GetCommand(CommandID)->execute(ActorID);
}
