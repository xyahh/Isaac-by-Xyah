#include "stdafx.h"
#include "CyanEngine.h"

Cyan Engine;

bool Cyan::Initialize(int WindowWidth, int WindowHeight)
{
	Sound::Initialize();
	return m_Renderer.Initialize(WindowWidth, WindowHeight);
}

void Cyan::Destroy()
{
	DeleteComponents();
	Sound::Destroy();
}

void Cyan::PushState(size_t ObjectIndex, size_t StateIndex)
{
	QueueAction([this, ObjectIndex, StateIndex]()
	{
		if (!m_States[ObjectIndex].empty())
			m_States[ObjectIndex].top()->Exit(ObjectIndex);
		m_States[ObjectIndex].emplace(m_StatePrototypes[ObjectIndex][StateIndex]->Make());
		m_States[ObjectIndex].top()->Enter(ObjectIndex);
	});
}

void Cyan::PopState(size_t ObjectIndex)
{
	if (m_States[ObjectIndex].size() < 2) return;

	QueueAction([this, ObjectIndex]()
	{
		m_States[ObjectIndex].top()->Exit(ObjectIndex);
		SafeDelete(m_States[ObjectIndex].top());
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
			SafeDelete(m_States[ObjectIndex].top());
			m_States[ObjectIndex].pop();
		}
		m_States[ObjectIndex].emplace(m_StatePrototypes[ObjectIndex][StateIndex]->Make());
		m_States[ObjectIndex].top()->Enter(ObjectIndex);	
	});
}

void Cyan::Render(float fInterpolation)
{
	FOR_EACH_OBJECT
		m_Graphics[i].Render(m_Renderer, m_Physics[i], m_Sprites[i], fInterpolation);
	END_FOR
}

void Cyan::Update()
{
	FOR_EACH_OBJECT
		if (m_States[i].empty()) continue;
		m_States[i].top()->Update(i);
		FlushActionQueue();
	END_FOR

	FOR_EACH_OBJECT 
		for (auto& Sprite : m_Sprites[i]) 
			Sprite.Update();
	END_FOR

	FOR_EACH_OBJECT
		m_Physics[i].Update();
		m_Physics[i].SetDeltaPosition(DX Subtract(m_Physics[i].GetPosition(), m_Physics[i].GetPrevPosition())); //Change this to a local Variable "Delta".
		for (size_t j = i + 1; j < m_Physics.size(); ++j)
			m_Physics[i].HandleCollision(&m_Physics[j]);
		m_Physics[i].SetPosition(DX Add(m_Physics[i].GetPrevPosition(), m_Physics[i].GetDeltaPosition()));
	END_FOR

	FlushActionQueue();
}

size_t Cyan::AddObject()
{
	m_Graphics.emplace_back();
	m_Physics.emplace_back();
	m_States.emplace_back();
	m_Sprites.emplace_back();
	m_Descriptors.emplace_back();
	m_StatePrototypes.emplace_back();
	return Last(m_Descriptors);
}

size_t Cyan::AddSprite(size_t ObjectIndex)
{
	m_Sprites[ObjectIndex].emplace_back();
	return Last(m_Sprites[ObjectIndex]);
}

size_t Cyan::AddStatePrototype(size_t ObjectIndex, State *&& pState)
{
	m_StatePrototypes[ObjectIndex].emplace_back(pState);
	size_t Index = Last(m_StatePrototypes[ObjectIndex]);
	m_StatePrototypes[ObjectIndex][Index]->pInput = new Input;
	return Index;
}

size_t Cyan::AddCommand(Command *&& pCommand)
{
	m_Commands.emplace_back(pCommand);
	return Last(m_Commands);
}

size_t Cyan::AddTexture(const STD string & ImagePath)
{
	size_t Tex = m_Renderer.CreatePngTexture(ImagePath);
	m_Textures.emplace_back(Tex);
	return  Last(m_Textures);
}

size_t Cyan::AddSound(const STD string & ImagePath, bool isBGM)
{
	m_Sounds.emplace_back(ImagePath, isBGM);
	return Last(m_Sounds);
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

Sprite& Cyan::GetSprite(size_t Index, size_t SpriteNumber)
{
	return m_Sprites[Index][SpriteNumber];
}

Input* Cyan::GetStateInput(size_t ObjectIndex, size_t StateIndex)
{
	return m_StatePrototypes[ObjectIndex][StateIndex]->pInput;
}

Command *& Cyan::GetCommand(size_t Index)
{
	return m_Commands[Index];
}

size_t Cyan::GetTexture(size_t Index)
{
	return m_Textures[Index];
}

Sound & Cyan::GetSound(size_t Index)
{
	return m_Sounds[Index];
}

void Cyan::DeleteComponents()
{
	for (auto& Sound : m_Sounds)
		Sound.Release();
	m_Sounds.clear();

	for (auto& ObjState : m_StatePrototypes)
	{
		for (auto& StatePrototype : ObjState)
		{
			SafeDelete(StatePrototype->pInput);
			SafeDelete(StatePrototype);
		}
		ObjState.clear();
	}

	for (auto& ObjStateStack : m_States)
	{
		while (!ObjStateStack.empty())
		{
			SafeDelete(ObjStateStack.top());
			ObjStateStack.pop();
		}
	}
		

	m_States.clear();
	m_StatePrototypes.clear();
	
	m_Physics.clear();
	m_Graphics.clear();
	m_States.clear();
	m_Sprites.clear();
	m_Descriptors.clear();
	
	for (auto& Commands : m_Commands)
		SafeDelete(Commands);
	m_Commands.clear();

	for (auto& Texture : m_Textures)
		m_Renderer.DeleteTexture(Texture);
	m_Textures.clear();
}

//void Cyan::UpdateState(const id_type& ActorID, const id_type& NewStateID)
//{
//	if (ActorState != NULL || !NextStateID.empty()) return;
//	ActorState = &m_States[m_StateLocator[ActorID]];
//	NextStateID = NewStateID;
//}
//
//void Cyan::ProcessUpdatedStates()
//{
//	if (ActorState == NULL || NextStateID.empty() || NextStateID == ActorState->StateID) return;
//
//	ActorState->pState->Exit(ActorState->ObjectID);	
//	size_t CopyDir = ActorState->pState->m_Direction;
//	delete ActorState->pState;
//	ActorState->pState = GetStateType(NextStateID)->Clone();
//	ActorState->StateID = NextStateID;
//	ActorState->pState->Enter(ActorState->ObjectID);
//	ActorState->pState->m_Direction = CopyDir;
//	ActorState = NULL;
//	NextStateID.clear();
//}
