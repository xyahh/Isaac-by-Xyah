#pragma once
#include "Scene.h"

class Gameplay : public Scene
{
public:
	Gameplay() { }
	virtual ~Gameplay() {  }

	void AddActor(size_t* ID, size_t Team,  SSE_VECTOR Position, size_t HeadTex, size_t BodyTex);

	virtual void Enter();
	virtual void Exit();
};

