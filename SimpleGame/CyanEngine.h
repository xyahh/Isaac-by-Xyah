#pragma once
#include "Command.h"
#include "Graphics.h"
#include "Physics.h"
#include "Entities.h"
#include "State.h"
#include "Sound.h"

/* Component IDs ------------- */
#define SOUNDS		0x01
#define STATES		0x02
#define ENTITIES	0x04
#define VISUALS		0x08
#define COMMANDS	0x10
#define TEXTURES	0x20
/*----------------------------*/


class Cyan
{
	friend Framework;

	template<class T>
	using Service = STD vector<T>;

	template<class ID, class T>
	using ServiceLocator = STD map<ID, T>;

public:
	Cyan() {}
	~Cyan() {}
 
	/*---Game Loop--------------*/

	void Render(float fInterpolation);
	void Update();

	/*--------------------------*/

	//I don't like these Functions
	id_type FindActor(const Physics& physics) const
	{
		for (u_int i = 0; i < m_Physics.size(); ++i)
			if (&physics == &m_Physics[i])
				for (auto& l : m_ActorLocator)
					if (i == l.second.PhysicsIndex)
						return l.first;
		return id_type("");
	}
	u_int FindObject(const Physics& physics) const
	{
		for (u_int i = 0; i < m_Physics.size(); ++i)
			if (&physics == &m_Physics[i])
				for (auto& l : m_ObjectLocator)
					if (i == l.second.PhysicsIndex)
						return l.first;
		return u_int();
	}
	u_int GetActorTeam(const id_type& ID) const
	{
		return m_ActorLocator.at(ID).Team;
	}
	u_int GetObjectTeam(u_int ID) const
	{
		return m_ObjectLocator.at(ID).Team;
	}


	/*---File Readers-----------*/
	void AddSoundsByFile(const STD string & filename, char delimiter = ',', bool ignore_first_row = true);
	void AddTexturesByFile(const STD string & filename, char delimiter = ',', bool ignore_first_row = true);
	void AddActorsByFile(const STD string & filename, char delimiter=',', bool ignore_first_row=true);
	void AddCommandsByFile(const STD string & filename, char delimiter = ',', bool ignore_first_row = true);
	void AddStatesByFile(const STD string & filename, char delimiter = ',', bool ignore_first_row = true);
	void AddInputsByFile(const STD string & filename, char delimiter = ',', bool ignore_first_row = true);
	/*--------------------------*/

	/*---Components Functions---*/
	
	void ReserveObjects(u_int ActorNumber, u_int BulletNumber, u_int VisualNumber, u_int StateNumber);

	// RVALUES Only. Engine handles Mem Dealloc.
	void AddCommand(const id_type& AssignID, Command*&& pCommand);
	void AddCommand(const id_type& AssignID, Command*& pCommand) = delete;
	void AddCommandByString(const id_type& ID, const STD string& Type,  const STD string& Args, char delimiter = ';');

	void AddStateType(const id_type& AssignID, State*&& pState);
	void AddStateType(const id_type& AssignID, State*& pState) = delete;
	void AddStateTypeByString(const id_type& ID, const STD string& Type, const STD string& Args, char delimiter = ';');

	void AddNonActorState(const id_type& AssignID);

	u_int AddObject();
	u_int AddEffect();
	void AddActor(const id_type& AssignID, const id_type& StartStateID);
	void AddVisual(const id_type& AssignID, WORD config = 0);
	
	void AddSound(const id_type& AssignID, const STD string& ImagePath, bool isBGM);
	void AddTexture(const id_type& TexID, const STD string& ImagePath);
	
	VisualGraphics&	GetVisualGraphics(const id_type& ID);
	ActorGraphics&	GetActorGraphics(const id_type& ID);
	ObjectGraphics&	GetObjectGraphics(u_int ID);
	EffectGraphics& GetEffect(u_int ID);
	u_int			GetTexture(const id_type& ID);

	Physics& GetActorPhysics(const id_type& ID);
	Physics& GetObjectPhysics(u_int ID);
	
	void UpdateState(const id_type& ActorID, const id_type& NewStateID);

	StateStruct& GetActorState(const id_type& ID);
	Command* GetCommand(const id_type& ID);
	Sound&	 GetSound(const id_type& ID);
	State*	 GetStateType(const id_type& ID);

	void DeleteEffect(u_int EffectID);
	void DeleteActor(const id_type& ActorID);
	void DeleteVisual(const id_type& VisualID);
	void DeleteObject(u_int ObjectID);

	u_int ActorCount() const;
	u_int VisualCount() const;

	void DeleteComponents(WORD Components = SOUNDS | STATES | ENTITIES | VISUALS | COMMANDS | TEXTURES);
	/*--------------------------*/

private:
	

	bool Initialize(int WindowWidth, int WindowHeight);
	void Destroy();

	//Done at the end of Cycles to avoid Dangling pointers / skipped items in loops
	void ProcessDeletionRequests(); 
	void ProcessUpdatedStates(); 

	StateStruct*		ActorState{ nullptr };
	id_type				NextStateID;

private:
	
	STD set<u_int>					m_ObjectDeletionRequests;

	/*-------------------------------------------*/
	/* Services								     */
	/*-------------------------------------------*/
	Service<u_int>					m_Textures;
	Service<ActorGraphics>			m_ActorGraphics;
	Service<Physics>				m_Physics;
	Service<StateStruct>			m_States;
	Service<ObjectGraphics>			m_ObjectGraphics;
	Service<EffectGraphics>			m_EffectGraphics;
	Service<VisualGraphics>			m_VisualGraphics;
	Service<Command*>				m_Commands;
	Service<Sound>					m_Sounds;
	Service<State*>					m_StateTypes;


	/*-------------------------------------------*/
	/* Service Locators                          */
	/*-------------------------------------------*/
	ServiceLocator<id_type, u_int>  m_TextureLocator;
	ServiceLocator<id_type, Actor>	m_ActorLocator;
	ServiceLocator<u_int,  Object>	m_ObjectLocator; 
	ServiceLocator<u_int,	u_int>	m_EffectLocator;
	ServiceLocator<id_type, u_int>	m_VisualLocator;
	ServiceLocator<id_type, u_int>	m_CommandLocator;
	ServiceLocator<id_type, u_int>	m_SoundLocator;
	ServiceLocator<id_type, u_int>	m_StateTypeLocator;

	
};

extern Cyan Engine;