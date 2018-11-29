#include "stdafx.h"
#include "World.h"
#include "Physics.h"

float World::Scale{ 1.f } ;

void World::SetScale(float meters, unsigned int pixels)
{
	Scale = static_cast<float>(pixels) / meters;
}

void World::ToPixels(float& fValue)
{
	fValue *= Scale;
}

void XM_CALLCONV World::ToPixels(DX XMVECTOR & v)
{
	v = DX Scale(v, Scale);
}

void World::ToMeters(float & fValue)
{
	fValue /= Scale;
}

void XM_CALLCONV World::ToMeters(DX XMVECTOR & v)
{
	v = DX Scale(v, 1.f / Scale);
}
