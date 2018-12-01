#include "stdafx.h"
#include "World.h"
#include "Physics.h"

float World::Scale{ 1.f } ;

void World::SetScale(float meters, unsigned int pixels)
{
	Scale = static_cast<float>(pixels) / meters;
}

float World::ToPixels(float fValue)
{
	return fValue * Scale;
}

DX XMVECTOR XM_CALLCONV World::ToPixels(DX FXMVECTOR v)
{
	return DX Scale(v, Scale);
}

float World::ToMeters(float fValue)
{
	return fValue / Scale;
}

DX XMVECTOR XM_CALLCONV World::ToMeters(DX FXMVECTOR v)
{
	return DX Scale(v, 1.f / Scale);
}
