#include "stdafx.h"
#include "Gamepad.h"

Gamepad Gamepad1;

Gamepad::Gamepad()
{
	Connect();
	Deadzone.x = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	Deadzone.y = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	Deadzone.x /= 32767.f;
	Deadzone.y /= 32767.f;
}

void Gamepad::Connect()
{
	GPID = -1;
	for (DWORD i = 0; i < XUSER_MAX_COUNT && GPID == -1; i++)
	{
		ZeroMemory(&State, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &State) == ERROR_SUCCESS)
			GPID = i;
	}
}

void Gamepad::Update()
{
	ZeroMemory(&State, sizeof(XINPUT_STATE));
	XInputGetState(GPID, &State);
}

bool Gamepad::GetButton(int Button)
{
	return State.Gamepad.wButtons & Button;
}

void XM_CALLCONV Gamepad::SetDeadzones(DX FXMVECTOR Deadzones)
{
	Deadzone = DX2 Store(Deadzones);
}

DX XMVECTOR Gamepad::GetAnalog(int Control)
{
	float x = 0.f, y = 0.f;

	switch (Control)
	{
	case GP_TRIGGERS:
		x = (float)State.Gamepad.bLeftTrigger / 255.f;
		y = (float)State.Gamepad.bRightTrigger / 255.f;
		break;
	case GP_LEFT_THUMB:
		x = fmaxf(-1, (float)State.Gamepad.sThumbLX / 32767.f);
		y = fmaxf(-1, (float)State.Gamepad.sThumbLY / 32767.f);
		break;
	case GP_RIGHT_THUMB:
		x = fmaxf(-1, (float)State.Gamepad.sThumbRX / 32767.f);
		y = fmaxf(-1, (float)State.Gamepad.sThumbRY / 32767.f);
		break;
	}

	return
	{
		fabs(x) < Deadzone.x ? 0 : x,
		fabs(y) < Deadzone.y ? 0 : y
	};
}

bool Gamepad::IsConnected()
{
	ZeroMemory(&State, sizeof(XINPUT_STATE));
	return XInputGetState(GPID, &State) == ERROR_SUCCESS;
}

//void Gamepad::Vibrate(float LeftMotor, float RightMotor)
//{
//	XINPUT_VIBRATION Vibration;
//	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
//
//	Vibration.wLeftMotorSpeed = (int)(LeftMotor * 65535.f);
//	Vibration.wRightMotorSpeed = (int)(RightMotor * 65535.f);
//
//	XInputSetState(GPID, &Vibration);
//}
