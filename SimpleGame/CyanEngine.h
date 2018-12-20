#pragma once
#include "World.h"
#include "Window.h"
#include "Descriptor.h"
#include "Renderer.h"
#include "Physics.h"
#include "Command.h"
#include "State.h"
#include "Sound.h"
#include "Timer.h"
#include "Input.h"
#include "Sprite.h"
#include "Controller.h"
#include "Direction.h"
#include "IDType.h"

class Cyan
{
	using Action = STD function<void()>;

public:
	Cyan() {}
	~Cyan() {}

	const IDType& GetID(size_t Index)
	{
		for (auto& obj : m_ObjectLocator)
			if (Index == obj.second)
				return obj.first;
		assert("Index Out of Bounds!" && false);
	}

	IDType LocateObject(const STD string& Name);
	IDType LocateObject(size_t Number);

	void ResetClock();

	/*---------Action Queue-----------------------------*/

	template<typename Func>
	void QueueAction(Func&& Fx)
	{
		m_Actions.emplace(Fx);
	}

	/*---------Game Loop--------------------------------*/

	bool Init(const STD string& Title, int X, int Y, const STD string& Dev = "");

	void MainLoop();

	void Update();
	void Render(float fInterpolation);

	void Destroy();

	/*---------Components Functions---------------------*/

	size_t AddObject(const STD string& ObjectID = "");
	void AddSprite(const IDType& ObjectIndex, const STD string & SpriteID);
	void AddController(const IDType& ObjectIndex, const STD string & StateID);
	void DeleteObject(const IDType& ObjectIndex);

	void UpdateInput();

	template<class T, class... Args>
	void AddStatePrototype(STD string&& ID, Args&&... Ax)
	{
		m_StatePTLocator[ID] = AddComponent<T>(m_StatePrototypes, Ax...);
	}

	template<class T, class... Args>
	void AddCommand(STD string&& ID, Args&&... Ax)
	{
		m_CommandLocator[ID] = AddComponent<T>(m_Commands, Ax...);
	}

	void AddTexture(const STD string& TexName, const STD string & ImagePath);
	void AddSound(const STD string& SoundName, const STD string & ImagePath, bool isBGM);

	void PushState(const IDType& ObjectIndex, const STD string & StateIndex);
	void PopState(const IDType& ObjectIndex);
	void ChangeState(const IDType& ObjectIndex, const STD string & StateIndex);

	/*---------Components Getters-----------------------*/

	World& GetWorld();
	Window& GetFramework();
	Renderer& GetRenderer();

	Descriptor& GetDescriptor(const IDType& ObjectIndex);
	Sprite&  GetSprite(const IDType& ObjectIndex, const STD string & SpriteName);
	Physics& GetPhysics(const IDType& ObjectIndex);
	State*&  GetCurrentState(const IDType& ObjectIndex);
	Controller& GetController(const IDType& ObjectIndex, const STD string & StateName);

	Command*& GetCommand(const STD string& CommandName);
	u_int GetTexture(const STD string& TextureName);
	Sound& GetSound(const STD string& SoundName);

	/*---------Components Deletion----------------------*/

	void DeleteComponents();

private:

	void ReleaseComponentData();

	template<class T, class V, class... Args>
	size_t AddComponent(STD vector<V>& v, Args&&... Ax)
	{
		v.emplace_back(new T(STD forward<Args>(Ax)...));
		return Last(v);
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

	using StateController = STD map <STD string, Controller>;
	using Graphics = STD vector<Sprite>;
	using StateStack = STD pair<bool, STD stack<State*>>;
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

	/* Locators */
	STD map<IDType, size_t>		m_ObjectLocator;
	

	STD map<STD string, size_t> m_StatePTLocator;
	STD map<STD string, size_t> m_CommandLocator;
	STD map<STD string, size_t> m_TextureLocator;
	STD map<STD string, size_t> m_SoundLocator;

	STD vector<STD map<STD string, size_t>>	m_SpriteLocator;

	/* Object Components */
	STD vector<Descriptor>		m_Descriptor;
	STD vector<Graphics>		m_Graphics;
	STD vector<Physics>			m_Physics;
	STD vector<Input>			m_Input;
	STD vector<StateStack>		m_States;
	STD vector<StateController>	m_Controllers;

	/* Integral Components */
	STD vector<State*>		m_StatePrototypes;
	STD vector<Command*>	m_Commands;
	STD vector<u_int>		m_Textures;
	STD vector<Sound>		m_Sounds;
	STD queue<Action>		m_Actions;

};

extern Cyan Engine;