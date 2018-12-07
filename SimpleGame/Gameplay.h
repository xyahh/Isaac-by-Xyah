#pragma once
#include "Scene.h"

class Gameplay : public Scene
{
public:
	Gameplay() { }
	virtual ~Gameplay() {  }

	virtual void Init();
	virtual void Exit();
	virtual void Render(float fInterpolation);
	virtual void Update();

private:
};

