#include "stdafx.h"
#include "Descriptor.h"

void Descriptor::Update()
{
	if (Value <= 0)
		Dispatch(DescriptorEvent::ValueZeroNegative);
}
