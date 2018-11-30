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

void Sprite::SetOffset(const DX XMFLOAT3 & _Offset)
{
	Offset = _Offset;
}

void Sprite::SetSpriteType(SPRITETYPE Type)
{
	SpriteType = Type;
}

void Sprite::SetFrameRate(float FrameRate_)
{
	FrameRate = FrameRate_;
}

void Sprite::SetSize(const DX XMFLOAT2 & _Size)
{
	Size = _Size;
}

void Sprite::SetTotal(const DX XMUINT2& _Total)
{
	Total = _Total;
}

void Sprite::ResetSprite()
{
	Current.x = 0;
	CurrentFrame = 0.f;
}

bool Sprite::Update()
{
	switch (SpriteType)
	{
	case SPRITETYPE::LINEAR:return LinearUpdate();
	case SPRITETYPE::GRID: return GridUpdate();
	}
}

bool Sprite::LinearNext()
{
	Current.x = (Current.x + 1) % Total.x;
	return Current.x > Total.x;
}

float Sprite::GetFrameRate() const
{
	return FrameRate;
}

bool Sprite::LinearUpdate()
{
	CurrentFrame += UPDATE_TIME *  FrameRate;
	Current.x = (int)(CurrentFrame) % Total.x;
	return CurrentFrame > Total.x;
}

bool Sprite::GridUpdate()
{
	CurrentFrame += UPDATE_TIME * FrameRate;
	Current.x = (int)(CurrentFrame) % Total.x;
	int Vframes = (int)(CurrentFrame) / Total.x;
	Current.y = Vframes % Total.y;
	return Vframes >= Total.y;
}

size_t Sprite::GetTexture() const
{
	return TexIndex;
}

u_int Sprite::GetDirection() const
{
	return Current.y;
}

DX XMVECTOR XM_CALLCONV Sprite::GetSize() const
{
	return DX2 Load(Size);
}

DX XMVECTOR XM_CALLCONV Sprite::GetCurrent() const
{
	return DX2 LoadUINT(Current);
}

DX XMVECTOR XM_CALLCONV Sprite::GetTotal() const
{
	return DX2 LoadUINT(Total);
}

DX XMVECTOR XM_CALLCONV Sprite::GetOffset() const
{
	return DX3 Load(Offset);
}
