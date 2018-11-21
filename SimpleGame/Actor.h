#pragma once
#include "Graphics.h"
#include "Physics.h"
#include "State.h"

class Actor
{
public:

	Actor();
	~Actor();

	void Render(float fInterpolation);
	void Update();

public:
	ActorGraphics	graphics;
	Physics			physics;
	State*			state;
};

