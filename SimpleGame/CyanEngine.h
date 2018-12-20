#pragma once
#include "World.h"
#include "Window.h"
#include "Descriptor.h"
#include "Renderer.h"
#include "Physics.h"
#include "State.h"
#include "Command.h"
#include "Sound.h"
#include "Timer.h"
#include "Input.h"
#include "Sprite.h"
#include "Controller.h"
#include "Direction.h"
#include "IDType.h"
#include "Event.h"

class Cyan
{
public:
	Cyan() {}
	~Cyan() {}

	const IDType& GetID(size_t Index);
	IDType LocateObject(const STD string& Name);
	IDType LocateObject(size_t Number);

	void ResetClock();
	void QueueAction(const STD function<void()>& Fx);

	/*---------Main Game Loop---------------------------*/

	bool Init(const STD string& Title, int X, int Y, const STD string& Dev = "");

	void MainLoop();

	void Update();
	void Render(float fInterpolation);

	void Destroy();

	/*---------Components Functions---------------------*/
	void ReserveObjects(size_t Number);

	size_t AddObject(const STD string& ObjectID = "");
	void AddSprite(const IDType& ObjectIndex, const STD string & SpriteID);
	void AddController(const IDType& ObjectIndex, State* pState);
	void DeleteObject(const IDType& ObjectIndex);

	void UpdateInput();

	template<class Child, class... Args>
	void AddCommand(const STD string& ID, Args&&... Ax)
	{
		m_Commands.emplace_back(new Child(STD forward<Args>(Ax)...));
		m_CommandLocator[ID] = Last(m_Commands);
	}

	void AddEvent(const STD function<void()>& Fx);

	void AddTexture(const STD string& TexName, const STD string & ImagePath);
	void AddSound(const STD string& SoundName, const STD string & ImagePath, bool isBGM);

	void PushState(const IDType& ObjectIndex, State* pState);
	void PopState(const IDType& ObjectIndex);
	void ChangeState(const IDType& ObjectIndex, State* pState);

	/*---------Components Getters-----------------------*/

	World& GetWorld();
	Window& GetWindow();
	Renderer& GetRenderer();
	Timer& GetTimer();

	Descriptor& GetDescriptor(const IDType& ObjectIndex);
	Sprite&  GetSprite(const IDType& ObjectIndex, const STD string & SpriteName);
	Physics& GetPhysics(const IDType& ObjectIndex);
	State*&  GetCurrentState(const IDType& ObjectIndex);
	Controller& GetController(const IDType& ObjectIndex, State* pState);

	Command*& GetCommand(const STD string& CommandName);
	u_int GetTexture(const STD string& TextureName);
	Sound& GetSound(const STD string& SoundName);

	/*---------Components Deletion----------------------*/

	void DeleteComponents();

private:

	void ReleaseComponentData();


	void FlushActionQueue();

private:

	template<class T>
	using Locator = STD map<T, size_t>;

	template<class T>
	using Service = STD vector<T>;

	using StateControls = STD map <StateType, Controller>;
	using Graphics = STD vector<Sprite>;
	using SpriteLocator = STD map<STD string, size_t>;
	using StateStack = STD pair<bool, STD stack<State*>>;

	/*
		The Bool in StateStack helps decide whether a
		Change in State of an Object is Queued in the
		Event Queue. This stops having two or more 
		State changes for the same object.
	*/	

	/* Core */
	Timer		m_Timer;
	Window		m_Window;
	Renderer	m_Renderer;
	World		m_World;

	/* Locators */
	Locator<IDType>			m_ObjectLocator;
	Locator<STD string>		m_CommandLocator;
	Locator<STD string>		m_TextureLocator;
	Locator<STD string>		m_SoundLocator;
	
	/* Object Components */
	Service<SpriteLocator>	m_SpriteLocator;
	Service<Descriptor>		m_Descriptor;
	Service<Graphics>		m_Graphics;
	Service<Physics>		m_Physics;
	Service<Input>			m_Input;
	Service<StateStack>		m_States;
	Service<StateControls>	m_Controllers;

	/* Integral Components */
	Service<Event>		m_ContinuousEvents;
	Service<Command*>	m_Commands;
	Service<u_int>		m_Textures;
	Service<Sound>		m_Sounds;
	STD queue<Event>	m_EventQueue;
};

extern Cyan Engine;