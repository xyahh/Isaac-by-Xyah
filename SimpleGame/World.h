#pragma once

class World
{
	static float Scale;

public:
	World() {}
	~World() {}

	static void SetScale(float meter, unsigned int pixels);
	static float ToPixels(float fValue);
	static DX XMVECTOR XM_CALLCONV ToPixels(DX FXMVECTOR v);
	static float ToMeters(float fValue);
	static DX XMVECTOR XM_CALLCONV ToMeters(DX FXMVECTOR v);

	void SetFOV(int Meters) {}

private:
	//Camera

};