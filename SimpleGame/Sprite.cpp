#include "stdafx.h"
#include "Sprite.h"

void Sprite::SetTexID(const id_type& _TexID)
{
	TexID = _TexID;
}

void Sprite::SetDirection(u_int _State)
{
	SpriteInfo.y = _State;
}

void Sprite::SetOffsetY(float offset_y)
{
	OffsetY = offset_y;
}

float Sprite::GetOffsetY() const
{
	return OffsetY;
}

u_int Sprite::GetDirection() const
{
	return SpriteInfo.y;
}

void Sprite::SetFrameRate(int FrameRate_)
{
	FrameRate = static_cast<float>(FrameRate_);
}

void Sprite::SetSize(const DX XMFLOAT2 & _Size)
{
	Size = _Size;
}

void Sprite::SetSpriteInfo(const DX XMINT4 & _SpriteInfo)
{
	SpriteInfo = _SpriteInfo;
	CurrentFrame = (float) SpriteInfo.x;
}

void Sprite::ResetSprite()
{
	SpriteInfo.x = 0;
	CurrentFrame = 0.f;
}

bool Sprite::FrameLinearNext()
{
	SpriteInfo.x = (SpriteInfo.x + 1) % SpriteInfo.z;
	return SpriteInfo.x > SpriteInfo.z;
}

bool Sprite::FrameLinearUpdate()
{
	CurrentFrame += UPDATE_TIME *  FrameRate;
	SpriteInfo.x = (int)(CurrentFrame) % SpriteInfo.z;
	return CurrentFrame > SpriteInfo.z;
}

bool Sprite::FrameGridUpdate()
{
	CurrentFrame += UPDATE_TIME * FrameRate;
	SpriteInfo.x = (int)(CurrentFrame) % SpriteInfo.z;

	int Vframes = (int)(CurrentFrame) / SpriteInfo.z;

	SpriteInfo.y = Vframes % SpriteInfo.w;
	return Vframes >= SpriteInfo.w;
}

id_type Sprite::GetTexID() const
{
	return TexID;
}

DX XMVECTOR XM_CALLCONV Sprite::GetSize() const
{
	return DX2 Load(Size);
}

DX XMVECTOR XM_CALLCONV Sprite::GetSpriteInfo() const
{
	return DX4 LoadInt(SpriteInfo);
}
