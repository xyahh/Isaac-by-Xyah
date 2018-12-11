#include "stdafx.h"
#include "Descriptor.h"

void Descriptor::Update(size_t ObjIndex)
{
	switch (Type)
	{
	case ObjectType::DamagedActor:
		if (Value <= 0)
			Dispatch(DescriptorEvent::ValueZeroNegative);
		break;
	}

}
