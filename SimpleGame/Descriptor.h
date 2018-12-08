#pragma once

enum ObjectType
{
	None,
	Actor,
	Projectile,
	Structure
};

class Descriptor
{
	friend Cyan;
public:

	Descriptor() : 
		Type(ObjectType::None), 
		TypeValue(0.f) 
	{}
	
	void SetType(ObjectType Type);
	ObjectType GetType() const;

	void SetValue(float Value);
	float GetValue() const;

	/*
	|---------------|-----------------|
	| ObjectType	|	TypeValue	  |
	|---------------|-----------------|
	| Actor			|	HP			  |
	| Projectile	|	Damage		  |
	| Structure		|	Friction	  |
	|---------------|-----------------|
	*/
private:

	ObjectType Type;
	float TypeValue;
};