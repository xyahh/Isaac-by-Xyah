#include "stdafx.h"
#include "CyanEngine.h"

Cyan Engine;

bool Cyan::Init(const STD string& Title, int X, int Y,bool Dev)
{
	m_Window.Initialize(Title, X, Y, Dev);
	Sound::Initialize();
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
		{
			Update();
		}

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
	Sound::Destroy();
}

void Cyan::PushState(size_t ObjectIndex, size_t StateIndex)
{ 
	if (m_States[ObjectIndex].first) return;
	m_States[ObjectIndex].first = true;
	QueueAction([this, ObjectIndex, StateIndex]() 
	{
		if (!m_States[ObjectIndex].second.empty())
			m_States[ObjectIndex].second.top()->Exit(ObjectIndex);
		m_States[ObjectIndex].second.emplace(m_StatePrototypes[StateIndex]->Make());
		m_States[ObjectIndex].second.top()->Enter(ObjectIndex);
		m_States[ObjectIndex].first = false;
	});
}

void Cyan::PopState(size_t ObjectIndex)
{
	if (m_States[ObjectIndex].first) return;
	m_States[ObjectIndex].first = true;
	QueueAction([this, ObjectIndex]()
	{
		if (m_States[ObjectIndex].second.size() <= 1) return;
		m_States[ObjectIndex].second.top()->Exit(ObjectIndex);
		delete m_States[ObjectIndex].second.top();
		m_States[ObjectIndex].second.pop();
		m_States[ObjectIndex].second.top()->Enter(ObjectIndex);
		m_States[ObjectIndex].first = false;
	});
}

void Cyan::ChangeState(size_t ObjectIndex, size_t StateIndex)
{

	if (m_States[ObjectIndex].first) return;
	m_States[ObjectIndex].first = true;
	QueueAction([this, ObjectIndex, StateIndex]()
	{
		if (!m_States[ObjectIndex].second.empty())
		{
			m_States[ObjectIndex].second.top()->Exit(ObjectIndex);
			delete m_States[ObjectIndex].second.top();
			m_States[ObjectIndex].second.pop();
		}
		m_States[ObjectIndex].second.emplace(m_StatePrototypes[StateIndex]->Make());
		m_States[ObjectIndex].second.top()->Enter(ObjectIndex);
		m_States[ObjectIndex].first = false;
	});
}

void Cyan::Update()
{
	/* Sprite Update */
	for (size_t i = 0; i < m_Sprites.size(); ++i)
	{
		for (auto& Sprite : m_Sprites[i])
			Sprite.Update();
	}

	/* State & Input Update */
	for (size_t i = 0; i < m_Input.size(); ++i)
	{
		if (m_States[i].second.empty())
			continue;
		m_Input[i].ProcessInput();
		m_Controllers[i][m_States[i].second.top()->Name()].HandleControls(i,
			m_Input[i].m_Pushed, m_Input[i].m_Released);
		m_Input[i].m_Released.clear(); // Call only once;
		m_States[i].second.top()->Update(i);
	}

	/* Collision Check */
	for (size_t i = 0; i < m_Physics.size(); ++i)
	{
		m_Physics[i].Update();
		m_Physics[i].SetDeltaPosition(DX Subtract(m_Physics[i].GetPosition(), m_Physics[i].GetPrevPosition()));
		for (size_t j = i + 1; j < m_Physics.size(); ++j)
			m_Physics[i].HandleCollision(i, &m_Physics[j], j);
		m_Physics[i].SetPosition(DX Add(m_Physics[i].GetPrevPosition(), m_Physics[i].GetDeltaPosition()));

	}

	FlushActionQueue();
}

void Cyan::Render(float fInterpolation)
{
	for (size_t i = 0; i < m_Graphics.size(); ++i)
		m_Graphics[i].Render(m_Renderer, m_Physics[i], m_Sprites[i], fInterpolation);
}

void Cyan::AddObject(size_t * Out)
{
	m_Descriptors.emplace_back();
	m_Controllers.emplace_back();
	m_Graphics.emplace_back();
	m_Physics.emplace_back();
	m_Sprites.emplace_back();
	m_States.emplace_back();
	m_Input.emplace_back();
	*Out = Last(m_Descriptors);
}

void Cyan::AddSprite(size_t * Out, size_t ObjectIndex)
{
	m_Sprites[ObjectIndex].emplace_back();
	*Out = Last(m_Sprites[ObjectIndex]);
}

void Cyan::AddController(size_t ObjectIndex, size_t StateIndex)
{
	m_Controllers[ObjectIndex][StateIndex];
}

void Cyan::UpdateInput()
{
	for (int i = 0; i < m_Input.size(); ++i)
	{
		for (auto& Controller : m_Controllers[i])
		{
			for(auto& Control : Controller.second.m_Controls)
				m_Input[i].DefineInput(Control.first);
		}
	}
}

void Cyan::AddTexture(size_t * Out, const STD string & ImagePath)
{
	u_int Tex = m_Renderer.GenerateTexture(ImagePath);
	m_Textures.emplace_back(Tex);
	*Out = Last(m_Textures);
}

void Cyan::AddSound(size_t * Out, const STD string & ImagePath, bool isBGM)
{
	m_Sounds.emplace_back(ImagePath, isBGM);
	*Out = Last(m_Sounds);
}

World & Cyan::GetWorld()
{
	return m_World;
}

Window & Cyan::GetFramework()
{
	return m_Window;
}

Descriptor & Cyan::GetDescriptor(size_t Index)
{
	return m_Descriptors[Index];
}

Graphics& Cyan::GetGraphics(size_t Index)
{
	return m_Graphics[Index];
}

Physics & Cyan::GetPhysics(size_t Index)
{
	return m_Physics[Index];
}

State *& Cyan::GetCurrentState(size_t Index)
{
	return m_States[Index].second.top();
}

Controller & Cyan::GetController(size_t ObjectIndex, size_t StateIndex)
{
	return m_Controllers[ObjectIndex][StateIndex];
}

Sprite& Cyan::GetSprite(size_t Index, size_t SpriteNumber)
{
	return m_Sprites[Index][SpriteNumber];
}

Command *& Cyan::GetCommand(size_t Index)
{
	return m_Commands[Index];
}

u_int Cyan::GetTexture(size_t Index)
{
	return m_Textures[Index];
}

Sound & Cyan::GetSound(size_t Index)
{
	return m_Sounds[Index];
}

void Cyan::DeleteComponents()
{
	/* Release Data */
	ReleaseComponentData();

	/* Clear Data */
	m_Descriptors.clear();
	m_Graphics.clear();
	m_Physics.clear();
	m_Input.clear();

	m_Sprites.clear();
	m_States.clear();
	m_Controllers.clear();

	m_StatePrototypes.clear();
	m_Commands.clear();
	m_Textures.clear();
	m_Sounds.clear();
}

void Cyan::ReleaseComponentData()
{
	for (auto& Sound : m_Sounds)
	{
		Sound.Release();
	}

	for (auto& ObjStateStack : m_States)
	{
		while (!ObjStateStack.second.empty())
		{
			State* pState = ObjStateStack.second.top();
			delete pState;
			ObjStateStack.second.pop();
		}
	}

	for (auto& ObjState : m_StatePrototypes)
	{
		delete ObjState;
		ObjState = nullptr;
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
