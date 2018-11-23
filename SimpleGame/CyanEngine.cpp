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
		for (auto& r : s)
		{
			r.pState->Update(r.ActorID);
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

void Cyan::AddSoundsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto SFile = ReadCSV(filename, delimiter, ignore_first_row);
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
	
	float HeadSize = 0.75f;
	float BodySize = 0.375f;

	for (auto& d : AFile)
	{
		Engine.AddActor(d[ID], d[STATE]);

		ActorGraphics& AGraphics = Engine.GetActorGraphics(d[ID]);
		AGraphics.Head.SetTexID(d[HEAD_TEX]);
		AGraphics.Body.SetTexID(d[BODY_TEX]);
		AGraphics.Body.SetFrameRate(20);
		AGraphics.Head.SetSize({ HeadSize, HeadSize });
		AGraphics.Body.SetSize({ BodySize, BodySize });
		AGraphics.SetSpriteOffset(BodySize* 0.5f + HeadSize * 0.5f, BodySize*0.5f);
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
	}
}

void Cyan::AddCommandsByFile(const STD string & filename, char delimiter, bool ignore_first_row)
{
	auto CFile = ReadCSV(filename, delimiter, ignore_first_row);
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

/* Command & State Functions ---------------------------------------------------------*/

void Cyan::AddCommand(const id_type& AssignID, Command*&& pCommand)
{
	m_Commands.emplace_back(pCommand);
	m_CommandLocator[AssignID] = LastIdx(m_Commands);
}

void Cyan::AddCommandByString(const id_type& ID,  const STD string& Type, const STD string & Args, char delimiter)
{
	STD string data;
	STD istringstream dataline(Args);

	u_int TypeNo;
	for (auto& c : CommandTypes)
		if (c.second == Type)
			TypeNo = c.first;

	switch (TypeNo)
	{
	case COMMAND::FORCE:
	{
		STD getline(dataline, data, delimiter);
		float x = STD stof(data);
		STD getline(dataline, data, delimiter);
		float y = STD stof(data);
		STD getline(dataline, data, delimiter);
		float z = STD stof(data);
		AddCommand(ID, new ForceCommand({x , y, z}));
		break;
	}	
	case COMMAND::STATE_ON_PRESS:
	{
		STD getline(dataline, data, delimiter);
		AddCommand(ID, new NewStateOnPressCommand(data));
		break;
	}
	case COMMAND::STATE_ON_RELEASE:
	{
		STD getline(dataline, data, delimiter);
		AddCommand(ID, new NewStateOnReleaseCommand(data));
		break;
	}
	case COMMAND::SHIFT_SCENE:
	{
		printf("Adding SHIFT_SCENE command via StringArgs is not Supported!\n");
		break;
	}
	case COMMAND::FUNCTION:
	{
		printf("Adding FUNCTION command via StringArgs is not Supported!\n");
		break;
	}
	}
}

void Cyan::AddConcurrentStateLevel()
{
	m_States.emplace_back();
}

void Cyan::AddStateType(const id_type & AssignID, State *&& pState)
{
	m_StateTypes.emplace_back(pState);
	u_int Idx = LastIdx(m_StateTypes);
	m_StateTypeLocator[AssignID] = Idx;
}

void Cyan::AddStateTypeByString(const id_type & ID, const STD string & Type, const STD string & Args, char delimiter)
{
	STD string data;
	STD istringstream dataline(Args);

	u_int TypeNo;
	for (auto& c : StateTypes)
		if (c.second == Type)
			TypeNo = c.first;

	switch (TypeNo)
	{
	case STATE::GLOBAL:
	{
		Engine.AddStateType(ID, new GlobalState);
		Engine.AddNonActorState("Global");
		break;
	}
	case STATE::IDLE:
	{
		Engine.AddStateType(ID, new IdleState);
		break;
	}
	case STATE::MOVING:
	{
		Engine.AddStateType(ID, new MovingState);
		break;
	}
	case STATE::JUMPING:
	{
		STD getline(dataline, data, delimiter);
		Engine.AddStateType(ID, new JumpingState(STD stof(data)));
		break;
	}
	case STATE::SLAMMING:
	{
		STD getline(dataline, data, delimiter);
		std::string TexID = data;
		STD getline(dataline, data, delimiter);
		float SlamForce = STD stof(data);
		STD getline(dataline, data, delimiter);
		float SlamSize = STD stof(data);
		Engine.AddStateType(ID, new SlammingState(TexID, SlamForce, SlamSize));
		break;
	}
	case STATE::SLAM_CHARGING:
	{
		STD getline(dataline, data, delimiter);
		float RageRate = STD stof(data);
		STD getline(dataline, data, delimiter);
		Engine.AddStateType(ID, new SlamChargingState(RageRate, data));
		break;
	}
	}
}

StateStruct& Cyan::GetActorState(const id_type & ID)
{
	return m_States[0][m_ActorLocator[ID].StateIndex];
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

void Cyan::AddNonActorState(const id_type & StartState)
{
	m_States[0].emplace_back("", StartState, GetStateType(StartState)->Clone());
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
	
	for (auto& s : m_States)
		s.emplace_back(AssignID, StartStateID, GetStateType(StartStateID)->Clone());
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

void Cyan::UpdateState(const id_type& ActorID, u_int WhoseState, const id_type& NewStateID)
{
	if (ActorState != NULL || !NextStateID.empty()) return;

	ActorState = &m_States[WhoseState][m_ActorLocator[ActorID].StateIndex];
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
		for(auto& j : i)
			if(ActorID == j.ActorID)
				j.ActorID.clear();

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
		for (auto OuterIter = m_States.rbegin(); OuterIter != m_States.rend(); ++OuterIter)
			for (auto InnerIter = OuterIter->begin(); InnerIter != OuterIter->end(); ++InnerIter)
			{
				InnerIter->StateID.clear();
				InnerIter->ActorID.clear();
				delete InnerIter->pState;
				InnerIter->pState = nullptr;
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
			for(auto& j : i)
				j.ActorID.clear();

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

