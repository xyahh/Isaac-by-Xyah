#pragma once
#include "Scene.h"

class Logo : public Scene
{
public:
	Logo() { }
	virtual ~Logo() { }

	virtual void Enter();
	virtual void Exit();
};

