#pragma once
#include "Descriptor.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Physics.h"
#include "Command.h"
#include "State.h"
#include "Sound.h"
#include "Sprite.h"

enum Direction
{
	Up,
	Down,
	Left,
	Right
};

class Cyan
{
	friend Framework;

	using Action = STD function<void()>;

public:
	Cyan() {}
	~Cyan() {}
 
	/*---------Game Loop--------------------------------*/

	void Render(float fInterpolation);
	void Update();
	/*--------------------------------------------------*/

	/*---------Components Functions---------------------*/
	size_t AddObject();
	size_t AddSprite(size_t ObjectIndex);

	size_t AddStatePrototype(size_t ObjectIndex, State*&& pState);
	size_t AddStatePrototype(size_t ObjectIndex, State*& pState) = delete;

	size_t AddCommand(Command*&& pCommand);
	size_t AddCommand(Command*& pCommand) = delete;
	
	size_t AddTexture(const STD string& ImagePath);
	size_t AddSound(const STD string& ImagePath, bool isBGM);
	
	void PushState(size_t ObjectIndex, size_t StateIndex);
	void PopState(size_t ObjectIndex);
	void ChangeState(size_t ObjectIndex, size_t StateIndex);

	template<typename Func>
	void QueueAction(Func&& Fx)
	{
		m_Actions.emplace_back(Fx);
	}

	void FlushActionQueue()
	{
		for (auto& Act : m_Actions) 
			Act();
		m_Actions.clear();
	}

	/*---------Components Getters-----------------------*/
	Descriptor& GetDescriptor(size_t Index);
	
	Graphics& GetGraphics(size_t Index);
	Physics& GetPhysics(size_t Index);
	State*& GetCurrentState(size_t Index);

	Sprite& GetSprite(size_t Index, size_t SpriteNumber);

	Input* GetStateInput(size_t ObjectIndex, size_t StateIndex);
	Command*& GetCommand(size_t Index);
	size_t GetTexture(size_t Index);
	Sound& GetSound(size_t Index);

	/*--------------------------------------------------*/
	void DeleteComponents();
	/*--------------------------------------------------*/

private:

	bool Initialize();
	void Destroy();
	void ReleaseData();

private:
	Renderer						m_Renderer;

	/* Object Components */
	STD vector<Descriptor>			m_Descriptors;
	STD vector<Graphics>			m_Graphics;
	STD vector<Physics>				m_Physics;
	STD vector<STD vector<Sprite>>  m_Sprites;
	STD vector<STD stack<State*>>	m_States;
	STD vector<STD vector<State*>>	m_StatePrototypes;

	/* Integral Components */
	STD vector<Command*>			m_Commands;
	STD vector<u_int>				m_Textures;
	STD vector<Sound>				m_Sounds;
	STD vector<Action>				m_Actions;
};

extern Cyan Engine;