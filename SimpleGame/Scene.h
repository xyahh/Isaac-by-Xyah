#pragma once
#include "CyanEngine.h"

class Scene
{
public:

	Scene() {}
	virtual ~Scene() {}

	virtual void Enter() {}
	virtual void Exit() {}

};