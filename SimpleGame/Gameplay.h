#pragma once
#include "Scene.h"

class Gameplay : public Scene
{
public:
	Gameplay() { }
	virtual ~Gameplay() {  }

	void AddActor(const STD string& ActorName, size_t Team,  SSE_VECTOR Position, 
		const STD string& HeadTex, const STD string& BodyTex, BasicCollision* pCollision);

	virtual void Enter();
	virtual void Exit();
};

