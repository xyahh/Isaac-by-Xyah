#pragma once
#include <Xinput.h>

/* Buttons ---------------------------------------------- */

#define GP_A	XINPUT_GAMEPAD_A
#define GP_B	XINPUT_GAMEPAD_B
#define GP_X	XINPUT_GAMEPAD_X
#define GP_Y	XINPUT_GAMEPAD_Y

#define GP_START	XINPUT_GAMEPAD_START
#define GP_BACK		XINPUT_GAMEPAD_BACK

#define GP_DPAD_UP		XINPUT_GAMEPAD_DPAD_UP
#define GP_DPAD_DOWN	XINPUT_GAMEPAD_DPAD_DOWN
#define GP_DPAD_LEFT	XINPUT_GAMEPAD_DPAD_LEFT
#define GP_DPAD_RIGHT	XINPUT_GAMEPAD_DPAD_RIGHT

#define GP_TRIGGERS		0x0400
#define GP_LEFT_THUMB	XINPUT_GAMEPAD_LEFT_THUMB
#define GP_RIGHT_THUMB	XINPUT_GAMEPAD_RIGHT_THUMB

#define GP_LEFT_SHLDR	XINPUT_GAMEPAD_LEFT_SHOULDER
#define GP_RIGHT_SHLDR	XINPUT_GAMEPAD_RIGHT_SHOULDER

/* ------------------------------------------------------ */

class Gamepad
{
	friend Cyan;

public:
	Gamepad();
	~Gamepad() = default;

	void Update();
	void Connect();
	bool IsConnected();
	bool GetButton(int Button);

	void XM_CALLCONV SetDeadzones(DX FXMVECTOR Deadzones);
	DX XMVECTOR GetAnalog(int Control);
	
	XINPUT_STATE State;
	int			 GPID;
	DX XMFLOAT2	 Deadzone;
};

extern Gamepad Gamepad1;