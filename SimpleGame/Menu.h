#pragma once
#include "Scene.h"

class Menu : public Scene
{
public:
	Menu() { }
	virtual ~Menu() {  }
	
	virtual void Init();
	virtual void Exit();
	virtual void Render(float fInterpolation);
	virtual void Update();
	
private:
};

