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

	IDType AddObject(size_t * Out = NULL, const STD string& ObjectID = "");
	void AddSprite(const IDType& ObjectIndex, const STD string & SpriteID);
	void AddEvent(const IDType& ObjectIndex, const STD string& BehaviorName);
	void AddController(const IDType& ObjectIndex, State* pState);
	void DeleteObject(const IDType& ObjectIndex);

	void UpdateInput();

	template<class Child, class... Args>
	void AddCommand(const STD string& ID, Args&&... Ax)
	{
		m_Commands.emplace_back(new Child(STD forward<Args>(Ax)...));
		m_CommandLocator[ID] = Last(m_Commands);
	}

	void AddTexture(const STD string& TexName, const STD string & ImagePath);

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
	State*  GetCurrentState(const IDType& ObjectIndex);
	Controller& GetController(const IDType& ObjectIndex, State* pState);
	Event&	GetEvent(const IDType& ObjectIndex, const STD string& EventName);

	Command* GetCommand(const STD string& CommandName);
	u_int GetTexture(const STD string& TextureName);
	Sound& GetSound();

	/*---------Components Deletion----------------------*/

	void DeleteComponents();

private:

	void ReleaseComponentData();


	void FlushActionQueue();

private:
	/* Core */
	Timer		m_Timer;
	Window		m_Window;
	Renderer	m_Renderer;
	World		m_World;
	Sound		m_Sound;

	/* Locators */
	STD map<IDType, size_t>			m_ObjectLocator;
	STD map<STD string, size_t>		m_CommandLocator;
	STD map<STD string, size_t>		m_TextureLocator;

	STD vector<STD map<STD string, size_t>>	m_SpriteLocator;
	STD vector<STD map<STD string, size_t>>	m_EventLocator;

	/* Object Components */
	STD vector <Descriptor>			m_Descriptor;
	STD vector <STD vector<Sprite>>	m_Sprites;
	STD vector <Physics>			m_Physics;
	STD vector <Input>				m_Input;
	STD vector<STD vector<Event>>	m_Events;

	STD vector <STD map <StateType, Controller>>	m_Controllers;
	STD vector <STD pair<bool, STD stack<State*>>>	m_States; 
	/* The Bool in StateStack helps decide whether a change in State of an Object is Queued in the
	Event Queue. This stops having two or more State changes for the same object.*/	

	/* Integral Components */
	STD vector<Command*>	m_Commands;
	STD vector<u_int>		m_Textures;
	STD queue<Event>		m_EventQueue;
};

extern Cyan Engine;