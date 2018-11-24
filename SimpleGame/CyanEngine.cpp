#include "stdafx.h"
#include "CyanEngine.h"
#include "Renderer.h"
#include "File.h"

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

void Cyan::ProcessDeletionRequests()
{
	for (auto& o : m_ObjectDeletionRequests)
	{
		u_int PhysicsIndex = m_ObjectLocator[o].PhysicsIndex;
		u_int GraphicsIndex = m_ObjectLocator[o].GraphicsIndex;
		printf("ID: %o Delete PhysicsIdx %o   and GraphicsIndex %d\n", o, PhysicsIndex, GraphicsIndex);
		m_Physics.erase(m_Physics.begin() + PhysicsIndex);
		m_ObjectGraphics.erase(m_ObjectGraphics.begin() + GraphicsIndex);

		for (auto& i : m_ObjectLocator)
		{
			if (i.second.PhysicsIndex > PhysicsIndex)
				--i.second.PhysicsIndex;
			if (i.second.GraphicsIndex > GraphicsIndex)
				--i.second.GraphicsIndex;
		}
		m_ObjectLocator.erase(o);
	}
	m_ObjectDeletionRequests.clear();
	
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
		ProcessUpdatedStates();
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
		if (m_EffectGraphics[i->second].Effect.FrameGridUpdate())
			StuffToDelete.emplace_back(i->first);
	}
	for (auto& i : StuffToDelete)
		DeleteEffect(i);

	ProcessDeletionRequests();
}

void Cyan::AddSoundsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto SFile = ReadCSV(filename, delimiter, ignore_first_row);
	m_Sounds.reserve(SFile.size());
	enum
	{
		SOUND_ID,
		SOUND_PATH,
		IS_MUSIC,
	};
	for (auto& d : SFile)
		Engine.AddSound(d[SOUND_ID], d[SOUND_PATH], STD stoi(d[IS_MUSIC]));
}

void Cyan::AddTexturesByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto TFile = ReadCSV(filename, delimiter, ignore_first_row);
	m_Textures.reserve(TFile.size());
	enum
	{
		TEX_ID,
		TEX_PATH,
	};
	for (auto& d : TFile)
		Engine.AddTexture(d[TEX_ID], d[TEX_PATH]);
}

void Cyan::AddActorsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto AFile = ReadCSV(filename, delimiter, ignore_first_row);
	m_ActorGraphics.reserve(AFile.size());
	m_Physics.reserve(AFile.size());
	m_States.reserve(AFile.size());

	enum
	{
		ID,
		STATE,
		HEAD_TEX,
		BODY_TEX,
		MASS,
		FRICTION,
		BOX_W,
		BOX_H,
		BOX_D,
		X,
		Y,
		Z,
		TEAM
	};
	
	float HeadSize = 1.25f;
	float BodySize = 0.75f;

	for (auto& d : AFile)
	{
		Engine.AddActor(d[ID], d[STATE]);

		ActorGraphics& AGraphics = Engine.GetActorGraphics(d[ID]);
		AGraphics.Head.SetTexID(d[HEAD_TEX]);
		AGraphics.Body.SetTexID(d[BODY_TEX]);
		AGraphics.Body.SetFrameRate(20);
		AGraphics.Head.SetSize({ HeadSize, HeadSize });
		AGraphics.Body.SetSize({ BodySize, BodySize });
		AGraphics.SetSpriteOffset(BodySize * 0.5f + HeadSize * 0.5, BodySize * 0.5f - 0.1f);
		AGraphics.Head.SetSpriteInfo({ 0, 0, 2,  4 });
		AGraphics.Body.SetSpriteInfo({ 0, 0, 10, 4 });
		AGraphics.Head.SetDirection(1);

		Physics& APhysics = Engine.GetActorPhysics(d[ID]);
		APhysics.SetFriction(STD stof(d[FRICTION]));
		APhysics.SetMass(STD stof(d[MASS]));
		//0.5f, 0.1f, 0.75f 
		APhysics.GetBox().SetDimensions(
			{ 
				STD stof(d[BOX_W]),
				STD stof(d[BOX_H]),
				STD stof(d[BOX_D])
			});
		APhysics.SetCollision(NULL);
		APhysics.SetPosition(
			{
				STD stof(d[X]),
				STD stof(d[Y]),
				STD stof(d[Z])
			});
		m_ActorLocator[d[ID]].Team = STD stoi(d[TEAM]);
	}
}

void Cyan::AddCommandsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto CFile = ReadCSV(filename, delimiter, ignore_first_row);
	m_Commands.reserve(CFile.size());
	enum
	{
		COMMAND_ID,
		COMMAND_TYPE,
		ARGS
	};
	for (auto& d : CFile)
		Engine.AddCommandByString(d[COMMAND_ID], d[COMMAND_TYPE], d[ARGS]);
}

void Cyan::AddStatesByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto SFile = ReadCSV(filename, delimiter, ignore_first_row);
	m_StateTypes.reserve(SFile.size());
	enum
	{
		STATE_ID,
		STATE_TYPE,
		ARGS
	};
	for (auto& d : SFile)
		Engine.AddStateTypeByString(d[STATE_ID], d[STATE_TYPE], d[ARGS]);
}

void Cyan::AddInputsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto IFile = ReadCSV(filename, delimiter, ignore_first_row);

	enum
	{
		STATE_ID,
		ACTOR_ID,
		KEY_ID,
		COMMAND_ID
	};
	char Key;
	for (auto& d : IFile)
	{
		if (d[KEY_ID].size() == 1) Key = d[KEY_ID][0]; //char if size of 1
		else Key = STD stoi(d[KEY_ID]); //int 
		Engine.GetStateType(d[STATE_ID])->GetInput(d[ACTOR_ID]).AddKeyboardInput(Key, d[COMMAND_ID]);
	}
		
}

void Cyan::AddCommand(const id_type& AssignID, Command*&& pCommand)
{
	m_Commands.emplace_back(pCommand);
	m_CommandLocator[AssignID] = LastIdx(m_Commands);
}

void Cyan::AddCommandByString(const id_type& ID,  const STD string& Type, const STD string & Args, char delimiter)
{
	for (auto& c : CommandTypes)
		if (c.first == Type)
			AddCommand(ID, c.second(Args, delimiter));
}

void Cyan::AddStateType(const id_type & AssignID, State *&& pState)
{
	m_StateTypes.emplace_back(pState);
	u_int Idx = LastIdx(m_StateTypes);
	m_StateTypeLocator[AssignID] = Idx;
}

void Cyan::AddStateTypeByString(const id_type & ID, const STD string & Type, const STD string & Args, char delimiter)
{
	for (auto& c : StateTypes)
		if (c.first == Type)
			Engine.AddStateType(ID, c.second(Args, delimiter));
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

void Cyan::AddNonActorState(const id_type & StartState)
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

u_int Cyan::GetTexture(const id_type & ID)
{
	return m_Textures[m_TextureLocator[ID]];
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

void Cyan::ProcessUpdatedStates()
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
	m_EffectGraphics.erase(m_EffectGraphics.begin() + Idx);
	m_EffectLocator.erase(EffectID);
}

void Cyan::DeleteActor(const id_type& ActorID)
{
	//ToDo
}

void Cyan::DeleteVisual(const id_type& VisualID)
{
	//ToDo
}

void Cyan::DeleteObject(u_int BulletID)
{
	m_ObjectDeletionRequests.emplace(BulletID);
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

void Cyan::AddTexture(const id_type& TexID, const STD string& ImagePath)
{
	u_int tex = RenderDevice.CreatePngTexture(ImagePath);
	m_Textures.emplace_back(tex);
	m_TextureLocator[TexID] = LastIdx(m_Textures);
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
		for (auto& i : m_States) //Maybe I have to delete the m_States here as well?
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

