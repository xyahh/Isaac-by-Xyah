#pragma once
#include "World.h"
#include "Window.h"
#include "Descriptor.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Physics.h"
#include "Command.h"
#include "State.h"
#include "Sound.h"
#include "Timer.h"
#include "Input.h"
#include "Sprite.h"
#include "Controller.h"
#include "Direction.h"

class Cyan
{
	using Action = STD function<void()>;
	
public:
	Cyan() {}
	~Cyan() {}
 
	size_t GetID(size_t Index)
	{
		for (auto& obj : m_ObjectLocator)
			if (Index == obj.second)
				return obj.first;
		assert("Hello" && false);
	}
	
	size_t GetIndex(size_t ID)
	{
		auto& obj = m_ObjectLocator.find(ID);
		if (obj != m_ObjectLocator.end())
			return obj->second;
		assert("Hello" && false);
	}

	/*---------Action Queue-----------------------------*/

	template<typename Func>
	void QueueAction(Func&& Fx)
	{
		m_Actions.emplace(Fx);
	}

	/*---------Game Loop--------------------------------*/

	bool Init(const STD string& Title, int X, int Y, bool Dev = false);

	void MainLoop();

	void Update();
	void Render(float fInterpolation);

	void Destroy();

	/*---------Components Functions---------------------*/

	void AddObject(size_t * Out);
	void AddSprite(size_t * Out, size_t ObjectIndex);
	void AddController(size_t ObjectIndex, size_t StateIndex); 

	void DeleteObject(size_t ObjectIndex);

	void UpdateInput();

	template<class T, class... Args>
	void AddStatePrototype(size_t* Out, Args&&... Ax)
	{
		Add<T>(m_StatePrototypes, Out, Ax...);
	}

	template<class T, class... Args>
	void AddCommand(size_t* Out, Args&&... Ax)
	{
		Add<T>(m_Commands, Out, Ax...);
	}

	void AddTexture(size_t * Out, const STD string& ImagePath);
	void AddSound(size_t * Out, const STD string& ImagePath, bool isBGM);

	void PushState(size_t ObjectIndex, size_t StateIndex);
	void PopState(size_t ObjectIndex);
	void ChangeState(size_t ObjectIndex, size_t StateIndex);

	/*---------Components Getters-----------------------*/
	World& GetWorld();
	Window& GetFramework();

	Descriptor& GetDescriptor(size_t Index);
	Graphics& GetGraphics(size_t Index);
	Physics& GetPhysics(size_t Index);
	State*& GetCurrentState(size_t Index);
	Controller& GetController(size_t ObjectIndex, size_t StateIndex);

	Sprite& GetSprite(size_t Index, size_t SpriteNumber);

	Command*& GetCommand(size_t Index);
	u_int GetTexture(size_t Index);
	Sound& GetSound(size_t Index);

	/*---------Components Deletion----------------------*/

	void DeleteComponents();

private:

	void ReleaseComponentData();

	template<class T, class V, class... Args>
	void Add(STD vector<V>& v, size_t* Out, Args&&... Ax)
	{
		v.emplace_back(new T(STD forward<Args>(Ax)...));
		*Out = Last(v);
	}

	void FlushActionQueue()
	{
		while (!m_Actions.empty())
		{
			m_Actions.front()();
			m_Actions.pop();
		}
	}


private:

	using ObjController = STD map <size_t, Controller>;
	using ObjSprite = STD vector<Sprite>;
	using ObjState = STD pair<bool, STD stack<State*>>;
	/*
		The Bool in State Pair helps decide whether a
		Change in state of an Object is Queued in the
		Action Queue. This stops having two or more 
		state changes of the same objects.
	*/	

	/* Core */
	Timer		m_Timer;
	Window		m_Window;
	Renderer	m_Renderer;
	World		m_World; //Scale

	/* Object Components */
	STD map<size_t, size_t>		m_ObjectLocator;

	STD vector<Descriptor>		m_Descriptors;
	STD vector<Graphics>		m_Graphics;
	STD vector<Physics>			m_Physics;
	STD vector<Input>			m_Input;
	STD vector<ObjSprite>		m_Sprites;
	STD vector<ObjState>		m_States;
	STD vector<ObjController>	m_Controllers;

	/* Integral Components */
	STD vector<State*>		m_StatePrototypes;
	STD vector<Command*>	m_Commands;
	STD vector<u_int>		m_Textures;
	STD vector<Sound>		m_Sounds;
	STD queue<Action>		m_Actions;

};

extern Cyan Engine;