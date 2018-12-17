#include "stdafx.h"
#include "World.h"
#include "CyanEngine.h"

void World::SetScale(float meters, unsigned int pixels)
{
	Scale = static_cast<float>(pixels) / meters;
	Engine.GetRenderer().UpdateScale(Scale);
}

float World::GetScale()
{
	return Scale;
}
