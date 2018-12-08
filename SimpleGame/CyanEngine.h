#pragma once
#include "Descriptor.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Physics.h"
#include "Command.h"
#include "State.h"
#include "Sound.h"
#include "Sprite.h"

/* Reversed order from Image TopDown. It is read from Bottom to Top */
enum Direction
{
	Right,
	Left,
	Down,
	Up,
};

class Cyan
{
	friend Input;
	friend Framework;
	using Action = STD function<void()>;

public:
	Cyan() {}
	~Cyan() {}
 
	/*---------Game Loop--------------------------------*/
	void Update();
	void Render(float fInterpolation);
	/*--------------------------------------------------*/

	/*---------Components Functions---------------------*/
	void AddObject(size_t * Out);
	void AddSprite(size_t * Out, size_t ObjectIndex);
	void AddObjectState(size_t ObjectIndex, size_t StatePrototypeIndex); //Map of Object's Input

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
	Input& GetStateInput(size_t ObjectIndex, size_t StateIndex);

	Sprite& GetSprite(size_t Index, size_t SpriteNumber);

	Command*& GetCommand(size_t Index);
	u_int GetTexture(size_t Index);
	Sound& GetSound(size_t Index);

	/*--------------------------------------------------*/
	void DeleteComponents();
	/*--------------------------------------------------*/

private:

	template<class T, class V, class... Args>
	void Add(STD vector<V>& v, size_t* Out, Args&&... Ax)
	{
		v.emplace_back(new T(STD forward<Args>(Ax)...));
		*Out = Last(v);
	}


	bool Initialize();
	void Destroy();
	void ReleaseData();

private:
	Renderer						m_Renderer;

	/* Object Components */
	STD vector<Descriptor>				m_Descriptors;
	STD vector<Graphics>				m_Graphics;
	STD vector<Physics>					m_Physics;
	STD vector<STD vector<Sprite>>		m_Sprites;
	STD vector<STD stack<State*>>		m_States;
	STD vector<STD map<size_t, Input>>	m_Input;


	/* Integral Components */
	STD vector<State*>		m_StatePrototypes;
	STD vector<Command*>	m_Commands;
	STD vector<u_int>		m_Textures;
	STD vector<Sound>		m_Sounds;
	STD vector<Action>		m_Actions;
};

extern Cyan Engine;