#include "stdafx.h"
#include "CyanEngine.h"

Cyan Engine;

IDType Cyan::LocateObject(size_t ID)
{
	return IDType("" , ID);
}

const IDType & Cyan::GetID(size_t Index)
{
	for (auto& obj : m_ObjectLocator)
		if (Index == obj.second)
			return obj.first;
	assert("Index Out of Bounds!" && false);
}

IDType Cyan::LocateObject(const STD string & ID)
{
	return IDType(ID, ULLONG_MAX);
}

void Cyan::ResetClock()
{
	m_Timer.Reset();
}

void Cyan::QueueAction(const STD function<void()>& Fx)
{
	m_EventQueue.emplace(Fx);
}

bool Cyan::Init(const STD string& Title, int X, int Y, const STD string& Dev)
{
	m_Window.Initialize(Title, X, Y, Dev);
	return m_Renderer.Initialize();
}

void Cyan::MainLoop()
{
	m_Timer.Reset();
	while (TRUE)
	{
		if (m_Window.ProcMessage()) 
			break; //Break Loop if QUIT Message was called

		m_Timer.Tick();
		while (m_Timer.FlushAccumulatedTime())
			Update();

		m_Renderer.Prepare(); 
		Render(m_Timer.Interpolation());
		m_Window.SwapBuffers();
	}
	Destroy();
}

void Cyan::Destroy()
{
	ReleaseComponentData();
	m_Window.Close();
}

void Cyan::ReserveObjects(size_t Number)
{
	m_Descriptor.reserve(Number);
	m_Controllers.reserve(Number);
	m_Physics.reserve(Number);
	m_Sprites.reserve(Number);
	m_States.reserve(Number);
	m_Input.reserve(Number);
	m_Events.reserve(Number);

	m_SpriteLocator.reserve(Number);
	m_EventLocator.reserve(Number);
}


IDType Cyan::AddObject(size_t * Out, const STD string & ObjectID)
{
	m_Descriptor.emplace_back();
	m_Controllers.emplace_back();
	m_Physics.emplace_back();
	m_Sprites.emplace_back();
	m_States.emplace_back();
	m_Input.emplace_back();
	m_Events.emplace_back();

	m_SpriteLocator.emplace_back();
	m_EventLocator.emplace_back();

	static size_t Number = 0;
	IDType ID = IDType(ObjectID, Number++);
	size_t Index = Last(m_Descriptor);
	if (Out) *Out = Number;
	m_Input[Index] = NULL;

	m_ObjectLocator.emplace(ID, Index);
	return ID;
}

void Cyan::AddSprite(const IDType& ObjectIndex, const STD string & SpriteID)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	m_Sprites[Index].emplace_back();
	size_t SpriteIndex = Last(m_Sprites[Index]);
	m_SpriteLocator[Index][SpriteID] = SpriteIndex;
}

void Cyan::AddEvent(const IDType & ObjectIndex, const STD string & BehaviorName)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	m_Events[Index].emplace_back();
	size_t BehaviorIndex = Last(m_Events[Index]);
	m_EventLocator[Index][BehaviorName] = BehaviorIndex;
}

void Cyan::AddController(const IDType & ObjectIndex, State * pState)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	m_Controllers[Index][pState->Type()];
}

void Cyan::DeleteObject(const IDType& ObjectIndex)
{
	QueueAction([this, ObjectIndex]()
	{
		auto Finder = m_ObjectLocator.find(ObjectIndex);
		if (Finder == m_ObjectLocator.end()) return;

		size_t Index = Finder->second;
		EraseByIndex(m_Descriptor, Index);
		EraseByIndex(m_Physics, Index);
		EraseByIndex(m_Input, Index);
		EraseByIndex(m_Sprites, Index);
		EraseByIndex(m_States, Index);
		EraseByIndex(m_Controllers, Index);

		for (auto& i = m_ObjectLocator.begin(); i != m_ObjectLocator.end(); ++i)
			if (i->second >= Index)
				--i->second; //Decrease by one all the Indexes added after
		m_ObjectLocator.erase(ObjectIndex);
	});
}

void Cyan::AddTexture(const STD string & TexName, const STD string & ImagePath)
{
	u_int Tex = m_Renderer.GenerateTexture(ImagePath);
	m_Textures.emplace_back(Tex);
	m_TextureLocator[TexName] = Last(m_Textures);
}

void Cyan::PushState(const IDType & ObjectIndex, State * pState)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	if (m_States[Index].first) return;
	m_States[Index].first = true;

	QueueAction([this, Index, ObjectIndex, pState]()
	{
		if (!m_States[Index].second.empty())
			m_States[Index].second.top()->Exit(ObjectIndex);
		m_States[Index].second.emplace(pState->Make());
		m_States[Index].second.top()->Enter(ObjectIndex);
		m_States[Index].first = false;
	});
}

void Cyan::PopState(const IDType&  ObjectIndex)
{

	size_t Index = m_ObjectLocator[ObjectIndex];
	if (m_States[Index].first) return;
	m_States[Index].first = true;

	QueueAction([this, Index, ObjectIndex]()
	{
		if (m_States[Index].second.size() <= 1) return;
		m_States[Index].second.top()->Exit(ObjectIndex);
		delete m_States[Index].second.top();
		m_States[Index].second.pop();
		m_States[Index].second.top()->Enter(ObjectIndex);
		m_States[Index].first = false;
	});
}

void Cyan::ChangeState(const IDType & ObjectIndex, State * pState)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	if (m_States[Index].first) return;
	m_States[Index].first = true;

	QueueAction([this, Index, ObjectIndex, pState]()
	{
		if (!m_States[Index].second.empty())
		{
			m_States[Index].second.top()->Exit(ObjectIndex);
			delete m_States[Index].second.top();
			m_States[Index].second.pop();
		}
		m_States[Index].second.emplace(pState->Make());
		m_States[Index].second.top()->Enter(ObjectIndex);
		m_States[Index].first = false;
	});
}

void Cyan::Update()
{
	m_Sound.Update();

	for (size_t i = 0; i < m_Events.size(); ++i)
		for (auto& B : m_Events[i])
			B.Execute();

	/* Sprite Update */
	for (size_t i = 0; i < m_Sprites.size(); ++i)
	{
		for (auto& Sprite : m_Sprites[i])
			Sprite.Update();
	}

	/* Descriptor Update */
	for (size_t i = 0; i < m_Descriptor.size(); ++i)
	{
		m_Descriptor[i].Update();
	}

	/* State & Input Update */
	for (size_t i = 0; i < m_Input.size(); ++i)
	{
		if (m_States[i].second.empty())
			continue;
		if (m_Input[i])
		{
			m_Input[i]->ProcessInput();
			m_Controllers[i][m_States[i].second.top()->Type()].HandleControls
			(GetID(i), m_Input[i]->m_Execute, m_Input[i]->m_Release);
			m_Input[i]->m_Release.clear(); // Call only once;
		}
		m_States[i].second.top()->Update(GetID(i));
		FlushActionQueue();
	}

	/* Collision Check */
	for (size_t i = 0; i < m_Physics.size(); ++i)
	{
		m_Physics[i].Update();
		m_Physics[i].SetDeltaPosition( Subtract(m_Physics[i].GetPosition(), m_Physics[i].GetPrevPosition()));
		for (size_t j = i + 1; j < m_Physics.size(); ++j)
			m_Physics[i].HandleCollision(GetID(i), &m_Physics[j], GetID(j));
		m_Physics[i].SetPosition(Add(m_Physics[i].GetPrevPosition(), m_Physics[i].GetDeltaPosition()));
	}

	FlushActionQueue();
}

void Cyan::Render(float fInterpolation)
{
	for (size_t i = 0; i < m_Sprites.size(); ++i)
	{
		SSE_VECTOR Position = Add
		(
			Scale(m_Physics[i].GetPosition(), fInterpolation),
			Scale(m_Physics[i].GetPrevPosition(), 1.f - fInterpolation)
		);

		for (auto& S : m_Sprites[i])
			S.Render(m_Renderer, Position);
	}
}

void Cyan::UpdateInput()
{
	for (int i = 0; i < m_Input.size(); ++i)
	{
		if (!m_Input[i]) continue;
		for (auto& Controller : m_Controllers[i])
		{
			for(auto& Control : Controller.second.m_Controls)
				m_Input[i]->DefineInput(Control.first);
		}
	}
}

World & Cyan::GetWorld()
{
	return m_World;
}

Window & Cyan::GetWindow()
{
	return m_Window;
}

Renderer & Cyan::GetRenderer()
{
	return m_Renderer;
}

Timer & Cyan::GetTimer()
{
	return m_Timer;
}

Descriptor & Cyan::GetDescriptor(const IDType & ObjectIndex)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	return m_Descriptor[Index];
}

Sprite & Cyan::GetSprite(const IDType& ObjectIndex, const STD string & SpriteName)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	size_t SpriteIndex = m_SpriteLocator[Index][SpriteName];
	return m_Sprites[Index][SpriteIndex];
}

Physics & Cyan::GetPhysics(const IDType & ObjectIndex)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	return m_Physics[Index];
}

State* Cyan::GetCurrentState(const IDType & ObjectIndex)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	if (m_States[Index].second.empty()) return nullptr;
	return m_States[Index].second.top();
}

Controller & Cyan::GetController(const IDType & ObjectIndex, State * pState)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	return m_Controllers[Index][pState->Type()];
}

Event & Cyan::GetEvent(const IDType & ObjectIndex, const STD string & EventName)
{
	size_t Index = m_ObjectLocator[ObjectIndex];
	return m_Events[Index][m_EventLocator[Index][EventName]];
}

Command * Cyan::GetCommand(const STD string& CommandName)
{
	size_t Idx = m_CommandLocator[CommandName];
	return m_Commands[Idx];
}

u_int Cyan::GetTexture(const STD string & TextureName)
{
	return m_Textures[m_TextureLocator[TextureName]];
}

Sound & Cyan::GetSound()
{
	return m_Sound;
}

void Cyan::DeleteComponents()
{
	/* Release Data */
	ReleaseComponentData();

	m_Sound.ReleaseSounds();

	/* Clear Data */
	m_ObjectLocator.clear();
	m_CommandLocator.clear();
	m_TextureLocator.clear();

	m_SpriteLocator.clear();
	m_EventLocator.clear();

	m_Descriptor.clear();
	m_Sprites.clear();
	m_Physics.clear();
	m_Input.clear();
	m_Events.clear();

	m_Controllers.clear();
	m_States.clear();

	m_Commands.clear();
	m_Textures.clear();
}

void Cyan::ReleaseComponentData()
{
	for (auto& ObjStateStack : m_States)
	{
		while (!ObjStateStack.second.empty())
		{
			State* pState = ObjStateStack.second.top();
			delete pState;
			ObjStateStack.second.pop();
		}
	}

	for (auto& Commands : m_Commands)
	{
		delete Commands;
		Commands = nullptr;
	}

	for (auto& Texture : m_Textures)
	{
		m_Renderer.DeleteTexture(Texture);
	}
}

void Cyan::FlushActionQueue()
{
	while (!m_EventQueue.empty())
	{
		m_EventQueue.front().Execute();
		m_EventQueue.pop();
	}
}
