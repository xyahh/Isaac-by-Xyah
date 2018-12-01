#include "stdafx.h"
#include "Descriptor.h"

void Descriptor::SetType(ObjectType Type_)
{
	Type = Type_;
}

ObjectType Descriptor::GetType() const
{
	return Type;
}

void Descriptor::SetValue(float Value)
{
	TypeValue = Value;
}

float Descriptor::GetValue() const
{
	return TypeValue;
}
