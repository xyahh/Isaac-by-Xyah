#pragma once

class World
{
	static float m_Scale;

public:
	World() {}
	~World() {}

	static void SetScale(float meter, unsigned int pixels);
	static void Convert(float& fValue);
	static void XM_CALLCONV Convert(DX XMVECTOR& v);


	void SetFOV(int Meters) {}

private:
	//Bounadries, Image, Camera, Background Music

};