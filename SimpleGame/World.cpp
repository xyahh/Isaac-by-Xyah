#include "stdafx.h"
#include "World.h"
#include "Physics.h"

float World::m_Scale{ 1.f } ;

void World::SetScale(float meters, unsigned int pixels)
{
	m_Scale = static_cast<float>(pixels) / meters;
}

void World::Convert(float& fValue)
{
	fValue *= m_Scale;
}

void XM_CALLCONV World::Convert(DX XMVECTOR & v)
{
	v = DX Scale(v, m_Scale);
}