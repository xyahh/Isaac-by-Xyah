#pragma once
#include "Scene.h"

class Gameplay : public Scene
{
public:
	Gameplay() { }
	virtual ~Gameplay() {  }

	virtual void Enter();
	virtual void Exit();
};

