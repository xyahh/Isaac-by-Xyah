#pragma once

enum ObjectType
{
	Default,
	Actor,
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
		Type(ObjectType::Default), 
		Value(0.f)
	{}

	void Update();

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