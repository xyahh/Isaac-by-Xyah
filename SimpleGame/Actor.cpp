#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Render(float fInterpolation)
{
}

void Actor::Update()
{
	state->Update(graphics, physics);
}
