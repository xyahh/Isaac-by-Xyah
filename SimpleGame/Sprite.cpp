#include "stdafx.h"
#include "Sprite.h"
#include "CyanEngine.h"

Sprite::Sprite() :
	Type(SpriteShape::Linear),
	FrameRate(0),
	CurrentFrame(0.f),
	Current(0, 0),
	Total(1, 1),
	Size(1.f, 1.f),
	Offset(0.f, 0.f, 0.f),
	TexIndex(""),
	Color(1.f, 1.f, 1.f, 1.f),
	Layer(LayerGroup::Middleground)
{}

void Sprite::SetTexture(const STD string& TexIdx)
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

void Sprite::SetSpriteType(SpriteShape _Type)
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
	case SpriteShape::Linear: 
		return LinearUpdate();
	case SpriteShape::Grid: 
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

const STD string& Sprite::GetTexture() const
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

void Sprite::SetLayerGroup(u_int _Layer)
{	 
	Layer = _Layer;
}

void Sprite::SetColor(SSE_VECTOR _Color)
{	 
	Color = StoreFloat4(_Color);
}

void Sprite::SetAlpha(float Value)
{	 
	Color.w = Value;
}

SSE_VECTOR SSE_CALLCONV Sprite::GetColor() const
{	 
	return LoadFloat4(Color);
}

void Sprite::Render(const Renderer & RenderDevice, SSE_VECTOR_PARAM1 Position)
{
	SSE_VECTOR SpriteSize = GetSize();
	SSE_VECTOR SpriteOffset = GetOffset();

	RenderDevice.DrawSprite
	(
		Add(Position, SpriteOffset)
		, SpriteSize
		, Color
		, Engine.GetTexture(TexIndex)
		, Current
		, Total
		, Layer
	);
}
