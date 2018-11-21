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

public:
	Cyan() {}
	~Cyan() {}
 
	/*---Game Loop--------------*/

	void Render(float fInterpolation);
	void Update();

	/*--------------------------*/

	/*---Components Functions---*/
	
	void ReserveObjects(u_int ActorNumber, u_int BulletNumber, u_int VisualNumber, u_int StateNumber);

	// RVALUES Only. Engine handles Mem Dealloc.
	void AddCommand(const id_type& AssignID, Command*&& pCommand);
	void AddCommand(const id_type& AssignID, Command*& pCommand) = delete;

	void AddStateType(const id_type& AssignID, State*&& pState);
	void AddStateType(const id_type& AssignID, State*& pState) = delete;

	void AddGlobalState(const id_type& StartState);

	u_int AddObject();
	u_int AddEffect();
	void AddActor(const id_type& AssignID, const id_type& StartStateID);
	void AddVisual(const id_type& AssignID, WORD config = 0);
	
	void AddSound(const id_type& AssignID, const STD string& ImagePath, bool isBGM);
	u_int AddTexture(const STD string& ImagePath);
	void  AddTextures(STD vector<STD pair<STD string, u_int&>> Textures);

	VisualGraphics&	GetVisualGraphics(const id_type& ID);
	ActorGraphics&	GetActorGraphics(const id_type& ID);
	ObjectGraphics&	GetObjectGraphics(u_int ID);
	EffectGraphics& GetEffect(u_int ID);

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

	template <class T>
	void EraseFromService(STD vector<T>& a, u_int Index)
	{
		T temp = a[a.size() - 1];
		a[a.size() - 1] = a[Index];
		a[Index] = temp;
		a.erase(a.end() - 1);
	}

	void UpdatePhysicsService(u_int Index);

	void ChangeStates();
	StateStruct*		ActorState{ nullptr };
	id_type				NextStateID;

private:
	STD vector<u_int>	m_Textures;
	
	/*-------------------------------------------*/
	/* Service Locators                          */
	/*-------------------------------------------*/

	STD map<id_type, Actor>	m_ActorLocator;
	STD map<u_int, Object>	m_ObjectLocator; 


	STD map<u_int,	 u_int>	m_EffectLocator;
	STD map<id_type, u_int>	m_VisualLocator;
	STD map<id_type, u_int>	m_CommandLocator;
	STD map<id_type, u_int>	m_SoundLocator;
	STD map<id_type, u_int>	m_StateTypeLocator;

	/*-------------------------------------------*/
	/* Services								     */
	/*-------------------------------------------*/
	STD vector<Physics>	m_Physics;

	STD vector<EffectGraphics>  m_EffectGraphics;
	STD vector<ActorGraphics>	m_ActorGraphics;
	STD vector<ObjectGraphics>	m_ObjectGraphics;
	STD vector<VisualGraphics>	m_VisualGraphics;

	STD vector<Command*>		m_Commands;
	STD vector<Sound>			m_Sounds;

	STD vector<StateStruct>		m_States;
	STD vector<State*>			m_StateTypes;
};

extern Cyan Engine;