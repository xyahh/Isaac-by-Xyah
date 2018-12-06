#include "stdafx.h"
#include "CyanEngine.h"
#include "World.h"

Cyan Engine;

bool Cyan::Initialize()
{
	Sound::Initialize();
	return m_Renderer.Initialize();
}

void Cyan::Destroy()
{
	ReleaseData();
	Sound::Destroy();
}



void Cyan::PushState(size_t ObjectIndex, size_t StateIndex)
{
	QueueAction([this, ObjectIndex, StateIndex]()
	{
		if (!m_States[ObjectIndex].empty())
			m_States[ObjectIndex].top()->Exit(ObjectIndex);
		m_States[ObjectIndex].emplace(m_StatePrototypes[StateIndex]->Make());
		m_States[ObjectIndex].top()->Enter(ObjectIndex);
	});
}

void Cyan::PopState(size_t ObjectIndex)
{
	if (m_States[ObjectIndex].size() < 2) return;

	QueueAction([this, ObjectIndex]()
	{
		m_States[ObjectIndex].top()->Exit(ObjectIndex);
		delete m_States[ObjectIndex].top();
		m_States[ObjectIndex].pop();
		m_States[ObjectIndex].top()->Enter(ObjectIndex);
	});
}

void Cyan::ChangeState(size_t ObjectIndex, size_t StateIndex)
{
	QueueAction([this, ObjectIndex, StateIndex]()
	{
		if (!m_States[ObjectIndex].empty())
		{
			m_States[ObjectIndex].top()->Exit(ObjectIndex);
			delete m_States[ObjectIndex].top();
			m_States[ObjectIndex].pop();
		}
		m_States[ObjectIndex].emplace(m_StatePrototypes[StateIndex]->Make());
		m_States[ObjectIndex].top()->Enter(ObjectIndex);	
	});
}

void Cyan::Update()
{

	for (size_t i = 0; i < m_Input.size(); ++i)
	{
		if (m_States[i].empty()) continue;
		m_Input[i][m_States[i].top()->Name()].ProcessInput(i);
		m_States[i].top()->Update(i);
		FlushActionQueue();
		
	}
	/* State & Input Update */
	for (size_t i = 0; i < m_States.size(); ++i)
	{
		if (m_States[i].empty()) continue;
		//m_States[i].top()->pInput->ReceiveForeignInput(KeyInfo{VK_SPACE, true, 0});
		
		
		
	}
	
	/* Sprite Update */
	for (size_t i = 0; i < m_Sprites.size(); ++i)
	{
		for (auto& Sprite : m_Sprites[i])
			Sprite.Update();
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

void Cyan::HandleInput(int KeyValue, bool Pressed)
{
	if (Input::TotalKeys.find(KeyValue) == Input::TotalKeys.end()) return;

	printf("AddKey\n");

	KeyInfo Key{ KeyValue, Pressed, 0 };
	for (size_t i = 0; i < m_States.size(); ++i)
		if(!m_States[i].empty())
			m_Input[i][m_States[i].top()->Name()].ReceiveInput(Key);
}

void Cyan::AddObject(size_t * Out)
{
	m_Descriptors.emplace_back();
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

void Cyan::AddObjectState(size_t ObjectIndex, size_t StatePrototypeIndex)
{
	m_Input[ObjectIndex][StatePrototypeIndex];
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
	return m_States[Index].top();
}

Input & Cyan::GetStateInput(size_t ObjectIndex, size_t StateIndex)
{
	return m_Input[ObjectIndex][StateIndex];
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

void Cyan::ReleaseData()
{
	for (auto& Sound : m_Sounds)
	{
		Sound.Release();
	}

	for (auto& ObjStateStack : m_States)
	{
		while (!ObjStateStack.empty())
		{
			State* pState = ObjStateStack.top();
			delete pState;
			ObjStateStack.pop();
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

void Cyan::DeleteComponents()
{
	ReleaseData();

	m_Sprites.clear();
	m_Sounds.clear();
	m_States.clear();
	m_StatePrototypes.clear();
	m_Physics.clear();
	m_Graphics.clear();
	m_Descriptors.clear();
	m_Commands.clear();
	m_Textures.clear();
	m_Input.clear();
}