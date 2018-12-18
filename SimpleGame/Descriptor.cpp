#include "stdafx.h"
#include "Descriptor.h"

void Descriptor::Update(size_t ObjIndex)
{
	if (Value <= 0)
		Dispatch(DescriptorEvent::ValueZeroNegative);
}
