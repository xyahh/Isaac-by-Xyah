#include "stdafx.h"
#include "CyanEngine.h"
#include "Renderer.h"

Cyan Engine;

/* Engine Core Functions -------------------------------------------------------------*/
bool Cyan::Initialize(int WindowWidth, int WindowHeight)
{
	Sound::Initialize();
	return RenderDevice.Initialize(WindowWidth, WindowHeight);
}

void Cyan::Destroy()
{
	DeleteComponents();
	Sound::Destroy();
}

void Cyan::Render(float fInterpolation)
{
	for (auto& g : m_VisualGraphics)
		g.Render();

	for (auto& g : m_ActorGraphics)
		g.Render(fInterpolation);

	for (auto& b : m_ObjectGraphics)
		b.Render(fInterpolation);

	for (auto& e : m_EffectGraphics)
		e.Render(fInterpolation);
}

void Cyan::Update()
{
	for (auto& s : m_States)
	{
		s.pState->Update(s.ActorID);
		ChangeStates();
	}

	for (auto Obj = m_Physics.begin(); Obj != m_Physics.end(); ++Obj)
	{

		Obj->Update();
		for (auto Nearby = Obj+1; Nearby != m_Physics.end();  ++Nearby)
			Obj->HandleCollision(&*Nearby);
	}

	

	//Move this later pls;
	STD vector<u_int> StuffToDelete;
	for (auto i = m_EffectLocator.begin(); i != m_EffectLocator.end(); ++i)
	{
		if (m_EffectGraphics[i->second].Effect.FrameGridAdvance())
			StuffToDelete.emplace_back(i->first);
	}
	for (auto& i : StuffToDelete)
		DeleteEffect(i);
}

/* Command & State Functions ---------------------------------------------------------*/

void Cyan::AddCommand(const id_type& AssignID, Command*&& pCommand)
{
	m_Commands.emplace_back(pCommand);
	m_CommandLocator[AssignID] = LastIdx(m_Commands);
}

void Cyan::AddStateType(const id_type & AssignID, State *&& pState)
{
	m_StateTypes.emplace_back(pState);
	u_int Idx = LastIdx(m_StateTypes);
	m_StateTypeLocator[AssignID] = Idx;
}

StateStruct& Cyan::GetActorState(const id_type & ID)
{
	return m_States[m_ActorLocator[ID].StateIndex];
}

Command * Cyan::GetCommand(const id_type& ID)
{
	return Engine.m_Commands[m_CommandLocator[ID]];
}

State* Cyan::GetStateType(const id_type& ID)
{
	return m_StateTypes[m_StateTypeLocator[ID]];
}

/* Actor & Visual Functions ----------------------------------------------------------*/

void Cyan::ReserveObjects(u_int ActorNumber, u_int BulletNumber, u_int VisualNumber, u_int StateNumber)
{
	/* Use Swap if memory needs to be reduced for each Scene */
	m_VisualGraphics.reserve(VisualNumber);
	m_EffectGraphics.reserve(ActorNumber);
	m_States.reserve(ActorNumber + 1);
	m_StateTypes.reserve(StateNumber);
	m_ActorGraphics.reserve(ActorNumber);
	m_ObjectGraphics.reserve(BulletNumber);
	m_Physics.reserve(ActorNumber + BulletNumber);
}

void Cyan::AddGlobalState(const id_type & StartState)
{
	m_States.emplace_back("", StartState, GetStateType(StartState)->Clone());
}

u_int Cyan::AddObject()
{
	static u_int Idx = 0;
	m_ObjectGraphics.emplace_back(Idx);
	m_Physics.emplace_back();
	m_ObjectLocator[Idx].GraphicsIndex = m_ObjectGraphics.size() - 1;
	m_ObjectLocator[Idx].PhysicsIndex  = LastIdx(m_Physics);
	return Idx++;
}

u_int Cyan::AddEffect()
{
	static u_int Idx = 0;
	m_EffectGraphics.emplace_back();
	m_EffectLocator[Idx] = m_EffectGraphics.size()-1;
	return Idx++;
}

void Cyan::AddActor(const id_type & AssignID, const id_type & StartStateID)
{
	m_Physics.emplace_back();
	m_ActorGraphics.emplace_back(AssignID);	

	m_States.emplace_back(AssignID, StartStateID, GetStateType(StartStateID)->Clone());
	m_ActorLocator[AssignID].GraphicsIndex =	LastIdx(m_ActorGraphics);
	m_ActorLocator[AssignID].PhysicsIndex =		LastIdx(m_Physics);

	m_ActorLocator[AssignID].StateIndex =		LastIdx(m_States);
}

void Cyan::AddVisual(const id_type& AssignID, WORD config)
{
	m_VisualGraphics.emplace_back(config);
	m_VisualLocator[AssignID] = LastIdx(m_VisualGraphics);
}

void Cyan::UpdatePhysicsService(u_int Index)
{
	for (auto& a : m_ActorLocator)
		if (a.second.PhysicsIndex == LastIdx(m_Physics))
		{
			a.second.PhysicsIndex = Index;
			break;
		}

	for (auto& b : m_ObjectLocator)
		if (b.second.PhysicsIndex == LastIdx(m_Physics))
		{
			b.second.PhysicsIndex = Index;
			break;
		}
}

VisualGraphics& Cyan::GetVisualGraphics(const id_type& ID)
{
	return m_VisualGraphics[m_VisualLocator[ID]];
}

ObjectGraphics & Cyan::GetObjectGraphics(u_int ID)
{
	return m_ObjectGraphics[m_ObjectLocator[ID].GraphicsIndex];
}

EffectGraphics & Cyan::GetEffect(u_int ID)
{
	return m_EffectGraphics[m_EffectLocator[ID]];
}

Physics & Cyan::GetActorPhysics(const id_type& ID)
{
	return m_Physics[m_ActorLocator[ID].PhysicsIndex];
}

Physics & Cyan::GetObjectPhysics(u_int ID)
{
	return m_Physics[m_ObjectLocator[ID].PhysicsIndex];
}

void Cyan::UpdateState(const id_type& ActorID, const id_type& NewStateID)
{
	if (ActorState != NULL || !NextStateID.empty()) return;

	ActorState = &m_States[m_ActorLocator[ActorID].StateIndex];
	NextStateID = NewStateID;
}

void Cyan::ChangeStates()
{
	if (ActorState == NULL || NextStateID.empty() || NextStateID == ActorState->StateID) return;

	ActorState->pState->Exit(ActorState->ActorID);	
	u_int CopyDir = ActorState->pState->m_Direction;
	delete ActorState->pState;
	ActorState->pState = GetStateType(NextStateID)->Clone();
	ActorState->StateID = NextStateID;
	ActorState->pState->Enter(ActorState->ActorID);
	ActorState->pState->m_Direction = CopyDir;
	ActorState = NULL;
	NextStateID.clear();
}

ActorGraphics & Cyan::GetActorGraphics(const id_type& ID)
{
	return m_ActorGraphics[m_ActorLocator[ID].GraphicsIndex];
}

void Cyan::DeleteEffect(u_int EffectID)
{
	u_int Idx = m_EffectLocator[EffectID];

	for (auto& l : m_EffectLocator)
		if (l.second >= Idx)
			--l.second;

	EraseFromService(m_EffectGraphics, Idx);
	m_EffectLocator.erase(EffectID);
}

void Cyan::DeleteActor(const id_type& ActorID)
{
	for (auto& i : m_States)
		if(ActorID == i.ActorID)
			i.ActorID.clear();

	u_int Index = m_ActorLocator[ActorID].PhysicsIndex;
	UpdatePhysicsService(Index);
	
	EraseFromService(m_ActorGraphics, m_ActorLocator[ActorID].GraphicsIndex);
	EraseFromService(m_States, m_ActorLocator[ActorID].StateIndex);
	EraseFromService(m_Physics, Index);
}

void Cyan::DeleteVisual(const id_type& VisualID)
{
	EraseFromService(m_VisualGraphics, m_VisualLocator[VisualID]);
}

void Cyan::DeleteObject(u_int BulletID)
{
	u_int Index = m_ObjectLocator[BulletID].PhysicsIndex;
	UpdatePhysicsService(Index);
	EraseFromService(m_ObjectGraphics, m_ObjectLocator[BulletID].GraphicsIndex);
	EraseFromService(m_Physics, Index);
}

u_int Cyan::ActorCount() const
{
	return m_ActorGraphics.size();
}

u_int Cyan::VisualCount() const
{
	return m_VisualGraphics.size();
}

/* Texture Functions -----------------------------------------------------------------*/

u_int Cyan::AddTexture(const STD string& ImagePath)
{
	u_int TexID = RenderDevice.CreatePngTexture(ImagePath);
	m_Textures.emplace_back(TexID);
	return TexID;
}

void Cyan::AddTextures(STD vector<STD pair<STD string, u_int&>> Textures)
{
	for (auto& t : Textures)
		t.second = AddTexture(t.first);
}

void Cyan::DeleteComponents(WORD Components)
{
	if (Components & SOUNDS)
	{
		for (auto Iter = m_Sounds.rbegin(); Iter != m_Sounds.rend(); ++Iter)
			Iter->Release();
		m_Sounds.clear();
		m_SoundLocator.clear();
	}	
	if (Components & STATES)
	{
		for (auto Iter = m_States.rbegin(); Iter != m_States.rend(); ++Iter)
		{
			Iter->StateID.clear();
			Iter->ActorID.clear();
			delete Iter->pState;
			Iter->pState = nullptr;
		}

		for (auto Iter = m_StateTypes.rbegin(); Iter != m_StateTypes.rend(); ++Iter)
		{
			delete (*Iter);
			(*Iter) = nullptr;
		}

		m_States.clear();
		m_StateTypes.clear();
		m_StateTypeLocator.clear();
	}	
	if (Components & ENTITIES)
	{
		m_Physics.clear();
		m_ActorGraphics.clear();
		m_ObjectGraphics.clear();

		for (auto& i : m_States) 
			i.ActorID.clear();

		m_ActorLocator.clear();
		m_ObjectLocator.clear();
	}
	if (Components & VISUALS)
	{
		m_VisualGraphics.clear();
		m_VisualLocator.clear();
	}
	if (Components & COMMANDS)
	{
		for (auto Iter = m_Commands.rbegin(); Iter != m_Commands.rend(); ++Iter)
			delete (*Iter);
		m_Commands.clear();
		m_CommandLocator.clear();
	}
	if (Components & TEXTURES)
	{
		for (auto Iter = m_Textures.rbegin(); Iter != m_Textures.rend(); ++Iter)
			RenderDevice.DeleteTexture(*Iter);
		m_Textures.clear();
	}
}

/* Sound Functions -------------------------------------------------------------------*/

void Cyan::AddSound(const id_type& AssignID, const STD string& ImagePath, bool isBGM)
{
	m_Sounds.emplace_back(ImagePath, isBGM);
	m_SoundLocator[AssignID] = LastIdx(m_Sounds);
}

Sound & Cyan::GetSound(const id_type& ID)
{
	return m_Sounds[m_SoundLocator[ID]];
}

