#pragma once

class World
{
	float Scale { 1.f };

public:
	World() {}
	~World() {}

	void  SetScale(float meter, unsigned int pixels);
	float GetScale();

private:
};