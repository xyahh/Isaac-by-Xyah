#pragma once
#include "IDType.h"

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

	Descriptor(const IDType& ID) : 
		Type(ObjectType::Default), 
		Value(0.f), ID(ID)
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

	IDType		ID;
	ObjectType	Type;
	float		Value;
	size_t		Team;
};