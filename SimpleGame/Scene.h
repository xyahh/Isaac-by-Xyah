#pragma once
#include "World.h"
#include "CyanEngine.h"

class Scene
{
public:

	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void Exit() {}
	virtual void Render(float fInterpolation) {}
	virtual void Update() { }
};