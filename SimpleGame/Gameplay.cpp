#include "stdafx.h"
#include "Gameplay.h"
#include "Logo.h"

enum TEAM
{
	RED,
	BLUE,
};

u_int	MonsterNumber = 0;
int		SpawnNumber;
float	SpawnRate;
float	SoundRate;
bool	win;

void AddActor(const STD string& ActorName, size_t Team,  SSE_VECTOR Position, 
	const STD string& HeadTex, const STD string& BodyTex, BasicCollision* pCollision)
{
	Engine.AddObject(NULL, ActorName);

	IDType& ActorID = Engine.LocateObject(ActorName);

	auto& ActorDescriptor = Engine.GetDescriptor(ActorID);
	auto& ActorPhysics = Engine.GetPhysics(ActorID);

	ActorDescriptor.Type = ObjectType::Actor;
	ActorDescriptor.Value = 100.f; // 100 HP
	ActorDescriptor.Team = Team;

	ActorPhysics.SetCollision(pCollision);
	ActorPhysics.Box().SetDimensions({ 0.5f, 0.5f, 1.5f });
	ActorPhysics.SetPosition(Position);
	ActorPhysics.SetMass(70.f);

	Engine.AddSprite(ActorID, "Shadow");
	Engine.AddSprite(ActorID, "Body");
	Engine.AddSprite(ActorID, "Head");

	auto& Shadow = Engine.GetSprite(ActorID, "Shadow");
	auto& Body =   Engine.GetSprite(ActorID, "Body");
	auto& Head =   Engine.GetSprite(ActorID, "Head");

	float HeadSize = 1.25f;
	float BodySize = 0.75f;

	Shadow.SetTexture("Shadow");
	Shadow.SetLayerGroup(LayerGroup::Background);
	Shadow.SetSize({ HeadSize, HeadSize });

	Body.SetTexture(BodyTex);
	Body.SetSize({ BodySize, BodySize });
	Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
	Body.SetTotal({ 10, 4 });
	Body.SetDirection(Direction::Down);

	Head.SetTexture(HeadTex);
	Head.SetSize({ HeadSize, HeadSize });
	Head.SetTotal({ 2, 4 });
	Head.SetDirection(Direction::Down);
	Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
}

void SpawnMonster()
{
	IDType& MonsterID = Engine.AddObject();

	

	auto& ActorDescriptor = Engine.GetDescriptor(MonsterID);
	auto& ActorPhysics	= Engine.GetPhysics(MonsterID);

	ActorDescriptor.Type = ObjectType::Actor;
	ActorDescriptor.Value = 10;
	ActorDescriptor.Team = TEAM::BLUE;
	++MonsterNumber;
	ActorDescriptor.AddEvent(DescriptorEvent::ValueZeroNegative, [MonsterID]()
	{
		--MonsterNumber;
		Engine.DeleteObject(MonsterID);
	});

	ActorPhysics.SetCollision(&Collision::Monster);
	ActorPhysics.Box().SetDimensions({ 0.5f, 0.5f, 1.5f });

	float X = static_cast<float>(rand() % 15 - 7);
	float Y = static_cast<float>(rand() % 15 - 7);

	ActorPhysics.SetPosition({X, Y, 5.f});
	ActorPhysics.SetMass(70.f);

	Engine.AddSprite(MonsterID, "Body");
	Engine.AddSprite(MonsterID, "Head");
	Engine.AddSprite(MonsterID, "Shadow");

	auto& Shadow = Engine.GetSprite(MonsterID, "Shadow");
	auto& Body = Engine.GetSprite(MonsterID, "Body");
	auto& Head = Engine.GetSprite(MonsterID, "Head");

	float HeadSize = 1.25f;
	float BodySize = 0.75f;

	Shadow.SetTexture("Shadow");
	Shadow.SetLayerGroup(LayerGroup::Background);
	Shadow.SetSize({ HeadSize, HeadSize });

	Body.SetTexture("ZombieBody");
	Body.SetSize({ BodySize, BodySize });
	Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
	Body.SetTotal({ 10, 4 });
	Body.SetDirection(Direction::Down);

	Head.SetTexture("ZombieHead");
	Head.SetSize({ HeadSize, HeadSize });
	Head.SetTotal({ 2, 4 });
	Head.SetDirection(Direction::Down);
	Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });

	Engine.ChangeState(MonsterID, &STATE::InAir);

	Engine.AddController(MonsterID, &STATE::Idle);	
	Engine.AddController(MonsterID, &STATE::Move);

	Controller& IdleInput = Engine.GetController(MonsterID, &STATE::Idle);

	IdleInput.MapControl('W', "StartMove");
	IdleInput.MapControl('S', "StartMove");
	IdleInput.MapControl('A', "StartMove");
	IdleInput.MapControl('D', "StartMove");

	Controller& MoveInput = Engine.GetController(MonsterID, &STATE::Move);

	MoveInput.MapControl('W', "SlowMoveUp");
	MoveInput.MapControl('S', "SlowMoveDown");
	MoveInput.MapControl('A', "SlowMoveLeft");
	MoveInput.MapControl('D', "SlowMoveRight");

	MoveInput.MapControl('W', "HeadFaceUp");
	MoveInput.MapControl('S', "HeadFaceDown");
	MoveInput.MapControl('A', "HeadFaceLeft");
	MoveInput.MapControl('D', "HeadFaceRight");

	MoveInput.MapControl('W', "BodyFaceUp");
	MoveInput.MapControl('S', "BodyFaceDown");
	MoveInput.MapControl('A', "BodyFaceLeft");
	MoveInput.MapControl('D', "BodyFaceRight");

	Engine.SetInput<NPCInput>(MonsterID, [MonsterID](STD set<int>& Pressed)
	{
		FLOAT2 Diff = StoreFloat2(Subtract
		(
			Engine.GetPhysics(MonsterID).GetPosition(),
			Engine.GetPhysics(Engine.LocateObject("Player")).GetPosition()
		));

		if (fabs(Diff.x) > 0.001)
		{
			if (Diff.x > 0) Pressed.emplace('A');
			else Pressed.emplace('D');
		}
		if(fabs(Diff.y) > 0.001)
		{
			if (Diff.y > 0) Pressed.emplace('S');
			else Pressed.emplace('W');
		}
	});

	Engine.UpdateInput();
	Engine.GetSound().Play("Zombie");
}

void Update()
{
	static float time = 0.f;
	time += UPDATE_TIME;

	if (time < SpawnRate) return;
	++SpawnNumber;
	time = 0.f;
	if (SpawnNumber % 5 == 0 && SpawnRate > 1.f)
		SpawnRate -= 1.f;

	Timer& T = Engine.GetTimer();
	if (T.GetElapsedTime() < 0.f) return SpawnMonster();
	if (MonsterNumber == 0 && !win)
	{
		Engine.GetSound().Stop();
		win = true;
		Engine.GetSound().Play("Win");

		IDType& Win = Engine.AddObject();

		Engine.AddSprite(Win, "Win");

		Sprite& s = Engine.GetSprite(Win, "Win");
		s.SetSize({ 15.f, 3.f });
		s.SetTotal({ 4, 15 });
		s.SetTexture("Win");
		s.SetLayerGroup(LayerGroup::Foreground);
		s.SetFrameRate(10.f);

		Engine.SetInput<KeyboardInput>(Win);

		Engine.AddController(Win, &STATE::Global);
		Controller& c = Engine.GetController(Win, &STATE::Global);
		c.MapControl(VK_RETURN, "Restart");

		Engine.UpdateInput();

		Engine.ChangeState(Win, &STATE::Global);
	}
}

void Gameplay::Enter()
{
	Engine.ReserveObjects(300);

	Engine.AddTexture("BasicBody", "./Resources/Characters/basic_body.png");
	Engine.AddTexture("IsaacHead", "./Resources/Characters/isaac_head.png");
	Engine.AddTexture("ZombieBody", "./Resources/Characters/zombie_body.png");
	Engine.AddTexture("ZombieHead", "./Resources/Characters/zombie_head.png");
	Engine.AddTexture("Depths", "./Resources/Levels/Depths.png");
	Engine.AddTexture("Explosion", "./Resources/explosion.png");
	Engine.AddTexture("Shadow", "./Resources/shadow.png");
	Engine.AddTexture("Tear", "./Resources/tear.png");
	Engine.AddTexture("Win", "./Resources/win.png");
	
	Sound& Sounds = Engine.GetSound();
	Sounds.Add("Main", "./Resources/Sounds/Main.mp3", true);
	Sounds.Add("Pop", "./Resources/Sounds/Pop.mp3", false);
	Sounds.Add("Zombie", "./Resources/Sounds/Zombie.mp3", false);
	Sounds.Add("Win", "./Resources/Sounds/Win.mp3", true);
	Sounds.Add("Explosion", "./Resources/Sounds/Explosion.mp3", false);
	Sounds.Add("HitEnemy", "./Resources/Sounds/HitEnemy.mp3", false);
	Sounds.Add("HitWall", "./Resources/Sounds/HitWall.mp3", false);

	Engine.GetSound().Play("Main");

	//Map
	{
		IDType& DEPTHS = Engine.AddObject();
		Engine.AddSprite(DEPTHS, "Depths");
		Sprite& Map = Engine.GetSprite(DEPTHS, "Depths");
		Map.SetSize({20.f, 20.f });
		Map.SetTexture("Depths");
		Map.SetLayerGroup(LayerGroup::Background);
	}

	//Engine.GetSound(SOUND_TEST).Play();
	AddActor("Player", RED, { 0.f, 0.f, 0.f }, "IsaacHead", "BasicBody", &Collision::Actor);
	IDType& Player = Engine.LocateObject("Player");

	//Commands
	{
		float Move = 1'500;
		Engine.AddCommand<ForceCommand>("MoveUp"   , 0.f, Move, 0.f);
		Engine.AddCommand<ForceCommand>("MoveDown" , 0.f, -Move, 0.f);
		Engine.AddCommand<ForceCommand>("MoveLeft" , -Move, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>("MoveRight", Move, 0.f, 0.f);

		float SlowMove = 750.f;
		Engine.AddCommand<ForceCommand>("SlowMoveUp"   , 0.f, SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveDown"  , 0.f, -SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveLeft" , -SlowMove, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>("SlowMoveRight", SlowMove, 0.f, 0.f);

		Engine.AddCommand<StateCommand>("StartMove",		&STATE::Move);
		Engine.AddCommand<StateCommand>("StartChargeJump",	&STATE::ChargeJump);
		Engine.AddCommand<StateCommand>("StartChargeSlam",	&STATE::ChargeSlam);
		Engine.AddCommand<StateCommand>("StartInAir",		&STATE::InAir, ST_CMD::ON_RELEASE | ST_CMD::CHANGE_STATE);
		Engine.AddCommand<StateCommand>("StartShoot",		&STATE::Shoot, ST_CMD::ON_PRESS | ST_CMD::PUSH_STATE);
		Engine.AddCommand<StateCommand>("EndShoot",			&STATE::Idle, ST_CMD::ON_RELEASE | ST_CMD::POP_STATE);

		Engine.AddCommand<SceneCommand<Logo>>("Restart");

		Engine.AddCommand<FaceCommand>("HeadFaceUp"   , "Head", Direction::Up);
		Engine.AddCommand<FaceCommand>("HeadFaceDown" , "Head", Direction::Down);
		Engine.AddCommand<FaceCommand>("HeadFaceLeft" , "Head", Direction::Left);
		Engine.AddCommand<FaceCommand>("HeadFaceRight", "Head", Direction::Right);

		Engine.AddCommand<FaceCommand>("BodyFaceUp"		, "Body", Direction::Up);
		Engine.AddCommand<FaceCommand>("BodyFaceDown"	, "Body", Direction::Down);
		Engine.AddCommand<FaceCommand>("BodyFaceLeft"	, "Body", Direction::Left);
		Engine.AddCommand<FaceCommand>("BodyFaceRight"	, "Body", Direction::Right);
	}

	//Actor Input
	{
		Engine.SetInput<KeyboardInput>(Player);

		Engine.AddController(Player, &STATE::Idle);
		Engine.AddController(Player, &STATE::Move);
		Engine.AddController(Player, &STATE::ChargeJump);
		Engine.AddController(Player, &STATE::InAir);
		Engine.AddController(Player, &STATE::Shoot);

		Controller& IdleInput = Engine.GetController(Player, &STATE::Idle);

		IdleInput.MapControl('W', "StartMove");
		IdleInput.MapControl('A', "StartMove");
		IdleInput.MapControl('S', "StartMove");
		IdleInput.MapControl('D', "StartMove");

		IdleInput.MapControl(VK_SPACE, "StartChargeJump");
		IdleInput.MapControl(VK_RIGHT, "StartShoot");
		IdleInput.MapControl(VK_LEFT,  "StartShoot");
		IdleInput.MapControl(VK_UP,    "StartShoot");
		IdleInput.MapControl(VK_DOWN,  "StartShoot");

		Controller&  MoveInput = Engine.GetController(Player, &STATE::Move);

		MoveInput.MapControl('W', "MoveUp");
		MoveInput.MapControl('S', "MoveDown");
		MoveInput.MapControl('A', "MoveLeft");
		MoveInput.MapControl('D', "MoveRight");

		MoveInput.MapControl('W', "HeadFaceUp");
		MoveInput.MapControl('S', "HeadFaceDown");
		MoveInput.MapControl('A', "HeadFaceLeft");
		MoveInput.MapControl('D', "HeadFaceRight");

		MoveInput.MapControl('W', "BodyFaceUp");
		MoveInput.MapControl('S', "BodyFaceDown");
		MoveInput.MapControl('A', "BodyFaceLeft");
		MoveInput.MapControl('D', "BodyFaceRight");

		MoveInput.MapControl(VK_SPACE, "StartChargeJump");
		MoveInput.MapControl(VK_RIGHT,	"StartShoot");
		MoveInput.MapControl(VK_LEFT,	"StartShoot");
		MoveInput.MapControl(VK_UP,		"StartShoot");
		MoveInput.MapControl(VK_DOWN,	"StartShoot");
		
		Controller&  ChargeJump = Engine.GetController(Player, &STATE::ChargeJump);

		ChargeJump.MapControl('W', "SlowMoveUp");
		ChargeJump.MapControl('S', "SlowMoveDown");
		ChargeJump.MapControl('A', "SlowMoveLeft");
		ChargeJump.MapControl('D', "SlowMoveRight");

		ChargeJump.MapControl('W', "BodyFaceUp");
		ChargeJump.MapControl('S', "BodyFaceDown");
		ChargeJump.MapControl('A', "BodyFaceLeft");
		ChargeJump.MapControl('D', "BodyFaceRight");

		ChargeJump.MapControl('W', "HeadFaceUp");
		ChargeJump.MapControl('S', "HeadFaceDown");
		ChargeJump.MapControl('A', "HeadFaceLeft");
		ChargeJump.MapControl('D', "HeadFaceRight");
		ChargeJump.MapControl(VK_SPACE, "StartInAir");
		
		Controller&  InAirInput = Engine.GetController(Player, &STATE::InAir);

		InAirInput.MapControl(VK_SPACE, "StartChargeSlam");
		
		Controller& ShootInput = Engine.GetController(Player, &STATE::Shoot);

		ShootInput.MapControl('W', "MoveUp");
		ShootInput.MapControl('S', "MoveDown");
		ShootInput.MapControl('A', "MoveLeft");
		ShootInput.MapControl('D', "MoveRight");;

		ShootInput.MapControl('W', "BodyFaceUp");
		ShootInput.MapControl('S', "BodyFaceDown");
		ShootInput.MapControl('A', "BodyFaceLeft");
		ShootInput.MapControl('D', "BodyFaceRight");

		ShootInput.MapControl(VK_RIGHT, "HeadFaceRight");
		ShootInput.MapControl(VK_LEFT,  "HeadFaceLeft");
		ShootInput.MapControl(VK_UP,	"HeadFaceUp");
		ShootInput.MapControl(VK_DOWN,  "HeadFaceDown");

		ShootInput.MapControl(VK_RIGHT, "EndShoot");
		ShootInput.MapControl(VK_LEFT,	"EndShoot");
		ShootInput.MapControl(VK_UP,	"EndShoot");
		ShootInput.MapControl(VK_DOWN,	"EndShoot");

		Controller& DamagedInput = Engine.GetController(Player, &STATE::Damaged);

		DamagedInput.MapControl('W', "MoveUp");
		DamagedInput.MapControl('S', "MoveDown");
		DamagedInput.MapControl('A', "MoveLeft");
		DamagedInput.MapControl('D', "MoveRight");

		DamagedInput.MapControl('W', "HeadFaceUp");
		DamagedInput.MapControl('S', "HeadFaceDown");
		DamagedInput.MapControl('A', "HeadFaceLeft");
		DamagedInput.MapControl('D', "HeadFaceRight");

		DamagedInput.MapControl('W', "BodyFaceUp");
		DamagedInput.MapControl('S', "BodyFaceDown");
		DamagedInput.MapControl('A', "BodyFaceLeft");
		DamagedInput.MapControl('D', "BodyFaceRight");

		Engine.ChangeState(Player, &STATE::Idle);
	}

	//Events
	{
		SpawnNumber = 0;
		SpawnRate = 5.f;
		SoundRate = 5.f;
		win = false;
		Engine.AddEvent(Player, "MonsterSpawn");
		Event& B = Engine.GetEvent(Player, "MonsterSpawn");
		B.Set(Update);
	}

	//Boundaries
	{

		float Min = 15.f;
		float Max = 30.f;
		{
			IDType& Boundary = Engine.AddObject();
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ 16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			IDType& Boundary = Engine.AddObject();
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Min, Max + Min, Max });
			BPhysics.SetPosition({ -16.5f, 0.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			IDType& Boundary = Engine.AddObject();
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Max, Min, Max });
			BPhysics.SetPosition({ 0.f, 16.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

		{
			IDType& Boundary = Engine.AddObject();
			Engine.GetDescriptor(Boundary).Type = ObjectType::Structure;
			Physics& BPhysics = Engine.GetPhysics(Boundary);
			BPhysics.Box().SetDimensions({ Max, Min, 10.f });
			BPhysics.SetPosition({ 0.f, -16.f, 0.f });
			BPhysics.SetCollision(&Collision::Structure);
		}

	}

	Engine.UpdateInput();
}

void Gameplay::Exit()
{
	Engine.DeleteComponents();
}