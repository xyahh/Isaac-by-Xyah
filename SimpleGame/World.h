#pragma once

class World
{
	static float Scale;

public:
	World() {}
	~World() {}

	static void SetScale(float meter, unsigned int pixels);
	static float GetScale();

private:
	//Camera

};