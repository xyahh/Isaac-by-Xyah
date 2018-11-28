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

void Cyan::HandleEvents()
{
	for (auto& Event : m_Events)
		switch (Event.first)
		{
		case EVENT::EFFECT_DELETE:
			DeleteEffect(STD stoi(Event.second));
			break;
		}
	m_Events.clear();
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
		m_Graphics.clear();
		for (auto& i : m_States) //Maybe I have to delete the m_States here as well?
			i.ActorID.clear();
		m_EntityLocator.clear();
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

void Cyan::Render(float fInterpolation)
{
	for (auto& g : m_VisualGraphics)
		g.Render();

	for (auto& g : m_Graphics)
		g.Render(fInterpolation);

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
		Obj->SetDeltaPosition(DX Subtract(Obj->GetPosition(), Obj->GetPrevPosition()));
		for (auto Nearby = Obj+1; Nearby != m_Physics.end();  ++Nearby)
			Obj->HandleCollision(&*Nearby);
		Obj->SetPosition(DX Add(Obj->GetPrevPosition(), Obj->GetDeltaPosition()));
	}

	for (auto i = m_EffectLocator.begin(); i != m_EffectLocator.end(); ++i)
	{
		if (m_EffectGraphics[i->second].Effect.FrameGridUpdate())
			AddEvent(EVENT::EFFECT_DELETE, STD to_string(i->first));
	}
	HandleEvents();
}

/* FILE READERS */

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
	m_Graphics.reserve(AFile.size());
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
		Z
	};
	
	float HeadSize = 1.25f;
	float BodySize = 0.75f;

	for (auto& d : AFile)
	{
		Engine.AddActor(d[ID], d[STATE]);

		Graphics& AGraphics = Engine.GetEntityGraphics(d[ID]);
		AGraphics.AddSprite("Head");
		AGraphics.AddSprite("Body");

		Sprite& Head = AGraphics.GetSprite("Head");
		Sprite& Body = AGraphics.GetSprite("Body");

		Head.SetTexID(d[HEAD_TEX]);
		Head.SetOffsetY(BodySize * 0.5f + HeadSize * 0.5);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetSpriteInfo({ 0, 0, 2,  4 });
		Head.SetDirection(1);

		Body.SetTexID(d[BODY_TEX]);
		Body.SetOffsetY(BodySize * 0.5f - 0.1f);
		Body.SetSize({ BodySize, BodySize });
		Body.SetSpriteInfo({ 0, 0, 10, 4 });
		Body.SetFrameRate(20);

		Physics& APhysics = Engine.GetEntityPhysics(d[ID]);
		APhysics.SetFriction(STD stof(d[FRICTION]));
		APhysics.SetMass(STD stof(d[MASS]));
		//0.5f, 0.1f, 0.75f 
		APhysics.GetBox().SetDimensions(
			{ 
				STD stof(d[BOX_W]),
				STD stof(d[BOX_H]),
				STD stof(d[BOX_D])
			});
		APhysics.SetCollision(&Collision::Actor);
		APhysics.SetPosition(
			{
				STD stof(d[X]),
				STD stof(d[Y]),
				STD stof(d[Z])
			});
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
	return m_States[m_EntityLocator[ID].StateIndex];
}

Command * Cyan::GetCommand(const id_type& ID)
{
	return Engine.m_Commands[m_CommandLocator[ID]];
}

State* Cyan::GetStateType(const id_type& ID)
{
	return m_StateTypes[m_StateTypeLocator[ID]];
}

void Cyan::AddEvent(u_int Event, const id_type & ID)
{
	m_Events.emplace_back(Event, ID);
}

void Cyan::ReserveObjects(u_int ActorNumber, u_int BulletNumber, u_int VisualNumber, u_int StateNumber)
{
	/* Use Swap if memory needs to be reduced for each Scene */
	m_VisualGraphics.reserve(VisualNumber);
	m_EffectGraphics.reserve(ActorNumber);
	m_States.reserve(ActorNumber + 1);
	m_StateTypes.reserve(StateNumber);
	m_Graphics.reserve(ActorNumber + BulletNumber);
	m_Physics.reserve(ActorNumber + BulletNumber);
}

void Cyan::AddNonActorState(const id_type & StartState)
{
	m_States.emplace_back("", StartState, GetStateType(StartState)->Clone());
}

id_type Cyan::AddObject(const id_type& ObjectType)
{
	static u_int Idx = 0;
	id_type ObjectID = ObjectType + STD to_string(Idx++);
	m_Graphics.emplace_back();
	m_Graphics[LastIdx(m_Graphics)].SetID(ObjectID);
	m_Physics.emplace_back();
	m_EntityLocator[ObjectID].GraphicsIndex = LastIdx(m_Graphics);
	m_EntityLocator[ObjectID].PhysicsIndex  = LastIdx(m_Physics);
	return ObjectID;
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
	m_Graphics.emplace_back();
	m_Graphics[LastIdx(m_Graphics)].SetID(AssignID);
	m_States.emplace_back(AssignID, StartStateID, GetStateType(StartStateID)->Clone());
	m_EntityLocator[AssignID].GraphicsIndex = LastIdx(m_Graphics);
	m_EntityLocator[AssignID].PhysicsIndex = LastIdx(m_Physics);
	m_EntityLocator[AssignID].StateIndex = LastIdx(m_States);
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

Graphics& Cyan::GetEntityGraphics(const id_type& ID)
{
	return m_Graphics[m_EntityLocator[ID].GraphicsIndex];
}

EffectGraphics & Cyan::GetEffect(u_int ID)
{
	return m_EffectGraphics[m_EffectLocator[ID]];
}

u_int Cyan::GetTexture(const id_type & ID)
{
	return m_Textures[m_TextureLocator[ID]];
}

Physics & Cyan::GetEntityPhysics(const id_type& ID)
{
	return m_Physics[m_EntityLocator[ID].PhysicsIndex];
}

void Cyan::UpdateState(const id_type& ActorID, const id_type& NewStateID)
{
	if (ActorState != NULL || !NextStateID.empty()) return;
	ActorState = &m_States[m_EntityLocator[ActorID].StateIndex];
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

void Cyan::DeleteEffect(u_int EffectID)
{
	u_int Idx = m_EffectLocator[EffectID];

	for (auto& l : m_EffectLocator)
		if (l.second >= Idx)
			--l.second;
	m_EffectGraphics.erase(m_EffectGraphics.begin() + Idx);
	m_EffectLocator.erase(EffectID);
}

/* Texture Functions -----------------------------------------------------------------*/

void Cyan::AddTexture(const id_type& TexID, const STD string& ImagePath)
{
	u_int tex = RenderDevice.CreatePngTexture(ImagePath);
	m_Textures.emplace_back(tex);
	m_TextureLocator[TexID] = LastIdx(m_Textures);
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

