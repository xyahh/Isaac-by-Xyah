#pragma once
#include "Scene.h"

class Menu : public Scene
{
public:
	Menu() { }
	virtual ~Menu() {  }
	
	virtual void Enter();
	virtual void Exit();
	
private:
};

