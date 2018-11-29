#pragma once

class World
{
	static float Scale;

public:
	World() {}
	~World() {}

	static void SetScale(float meter, unsigned int pixels);
	static void ToPixels(float& fValue);
	static void XM_CALLCONV ToPixels(DX XMVECTOR& v);
	static void ToMeters(float& fValue);
	static void XM_CALLCONV ToMeters(DX XMVECTOR& v);

	void SetFOV(int Meters) {}

private:
	//Camera

};