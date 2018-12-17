#include "stdafx.h"
#include "Sprite.h"

void Sprite::SetTexture(size_t TexIdx)
{
 	TexIndex = TexIdx;
}

void Sprite::SetDirection(u_int _State)
{
	Current.y = _State;
}

void SSE_CALLCONV Sprite::SetOffset( SSE_VECTOR_PARAM1 _Offset)
{
	Offset = StoreFloat3(_Offset);
}

void Sprite::SetSpriteType(SpriteType _Type)
{
	Type = _Type;
}

void Sprite::SetFrameRate(float _FrameRate)
{
	FrameRate = _FrameRate;
}

void SSE_CALLCONV Sprite::SetSize(SSE_VECTOR_PARAM1 _Size)
{
	Size = StoreFloat2(_Size);
}

void SSE_CALLCONV Sprite::SetTotal(SSE_VECTOR_PARAM1 _Total)
{
	Total = StoreUint2(_Total);
}

void Sprite::ResetSprite()
{
	Current.x = 0;
	CurrentFrame = 0.f;
}

bool Sprite::Update()
{
	switch (Type)
	{
	case SpriteType::Linear: 
		return LinearUpdate();
	case SpriteType::Grid: 
		return GridUpdate();
	}
	return false;
}

void Sprite::NextFrame()
{
	CurrentFrame += 1.f;
}

float Sprite::GetFrameRate() const
{
	return FrameRate;
}

bool Sprite::LinearUpdate()
{
	CurrentFrame += UPDATE_TIME *  FrameRate;
	Current.x = (u_int)(CurrentFrame) % Total.x;
	return CurrentFrame >= Total.x;
}

bool Sprite::GridUpdate()
{
	CurrentFrame += UPDATE_TIME * FrameRate;
	Current.x = (u_int)(CurrentFrame) % Total.x;
	u_int Vframes = (u_int)(CurrentFrame) / Total.x;
	Current.y = Vframes % Total.y;
	if (Vframes >= Total.y)
	{
		Dispatch(SpriteEvent::LoopEnd);
		CurrentFrame = 0;
		return true;
	}
	return false;
}

size_t Sprite::GetTexture() const
{
	return TexIndex;
}

u_int Sprite::GetDirection() const
{
	return Current.y;
}

 SSE_VECTOR SSE_CALLCONV Sprite::GetSize() const
{
	return LoadFloat2(Size);
}

 SSE_VECTOR SSE_CALLCONV Sprite::GetCurrent() const
{
	return LoadUint2(Current);
}

 SSE_VECTOR SSE_CALLCONV Sprite::GetTotal() const
{
	return LoadUint2(Total);
}

 SSE_VECTOR SSE_CALLCONV Sprite::GetOffset() const
{
	return LoadFloat3(Offset);
}
