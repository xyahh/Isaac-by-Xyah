#pragma once
#include "Subject.h"

enum SpriteType
{
	Linear,
	Grid,
};

enum SpriteEvent
{
	LoopEnd,
};

class Sprite : public EventDispatcher
{
	friend Cyan;

public:

	Sprite() :
		Type(SpriteType::Linear),
		FrameRate(0),
		CurrentFrame(0.f),
		Current(0, 0),
		Total(1, 1),
		Size(0.f, 0.f),
		Offset(0.f, 0.f, 0.f),
		TexIndex(0)
	{}


	~Sprite() {}

	void SetTexture(size_t TexIdx);
	void SetDirection(u_int _State);
	void XM_CALLCONV SetSize(DX FXMVECTOR _Size);
	void ResetSprite();
	void XM_CALLCONV SetTotal(DX FXMVECTOR _Total);
	void XM_CALLCONV SetOffset(DX FXMVECTOR _Offset);

	void SetSpriteType(SpriteType Type);
	void SetFrameRate(float FrameRate);
	
	bool Update();

	bool LinearUpdate();
	bool GridUpdate();

	void NextFrame();

	float GetFrameRate() const;

	size_t GetTexture() const;
	u_int GetDirection() const;
	DX XMVECTOR XM_CALLCONV GetSize() const;
	DX XMVECTOR XM_CALLCONV GetCurrent() const;
	DX XMVECTOR XM_CALLCONV GetTotal() const;
	DX XMVECTOR XM_CALLCONV GetOffset() const;

private:
	SpriteType	Type;
	size_t		TexIndex;
	float		FrameRate;
	float		CurrentFrame;
	DX XMUINT2	Current;
	DX XMUINT2	Total; //How many Columns/Rows in SpriteSheet
	DX XMFLOAT2 Size;
	DX XMFLOAT3	Offset;
};
