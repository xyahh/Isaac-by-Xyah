#pragma once

class World
{
	static float Scale;

public:
	World() {}
	~World() {}

	void  SetScale(float meter, unsigned int pixels);
	float GetScale();

private:
};