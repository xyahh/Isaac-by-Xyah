#pragma once

enum ObjectType
{
	None,
	Actor,
	DamagedActor,
	Projectile,
	Structure
};

enum DescriptorEvent
{
	ValueZeroNegative,
};


class Descriptor : public EventDispatcher
{
	friend Cyan;
public:

	Descriptor() : 
		Type(ObjectType::None), 
		Value(0.f)
	{}

	void Update(size_t ObjIndex);

	/*
	|---------------|-----------------|
	| ObjectType	|	TypeValue	  |
	|---------------|-----------------|
	| Actor			|	HP			  |
	| Projectile	|	Damage		  |
	| Structure		|	Friction	  |
	|---------------|-----------------|
	*/

	ObjectType Type;
	float Value;
	size_t Team;
};