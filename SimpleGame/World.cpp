#include "stdafx.h"
#include "World.h"
#include "Physics.h"

float World::Scale{ 1.f } ;

void World::SetScale(float meters, unsigned int pixels)
{
	Scale = static_cast<float>(pixels) / meters;
}

float World::GetScale()
{
	return Scale;
}
