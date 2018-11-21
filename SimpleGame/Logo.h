#pragma once
#include "Scene.h"

class Logo : public Scene
{
public:
	Logo() { }
	virtual ~Logo() { }

	virtual void Init();
	virtual void Exit();
	virtual void Render(float fInterpolation);
	virtual void Update();

private:
	float Fade;
	float Timer;
	float FadeMax;
};

