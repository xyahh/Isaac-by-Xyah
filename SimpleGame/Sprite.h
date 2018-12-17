#pragma once

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
	friend Graphics;

public:

	Sprite() :
		Type(SpriteType::Linear),
		FrameRate(0),
		CurrentFrame(0.f),
		Current(0, 0),
		Total(1, 1),
		Size(1.f, 1.f),
		Offset(0.f, 0.f, 0.f),
		TexIndex(0)
	{}

	~Sprite() {}

	void SetTexture(size_t TexIdx);
	void SetDirection(u_int _State);
	void SSE_CALLCONV SetSize(SSE_VECTOR_PARAM1 _Size);
	void ResetSprite();
	void SSE_CALLCONV SetTotal(SSE_VECTOR_PARAM1 _Total);
	void SSE_CALLCONV SetOffset(SSE_VECTOR_PARAM1 _Offset);

	void SetSpriteType(SpriteType Type);
	void SetFrameRate(float FrameRate);
	
	bool Update();

	bool LinearUpdate();
	bool GridUpdate();

	void NextFrame();

	float GetFrameRate() const;

	size_t GetTexture() const;
	u_int GetDirection() const;

	 SSE_VECTOR SSE_CALLCONV GetSize() const;
	 SSE_VECTOR SSE_CALLCONV GetCurrent() const;
	 SSE_VECTOR SSE_CALLCONV GetTotal() const;
	 SSE_VECTOR SSE_CALLCONV GetOffset() const;

private:
	u_int	Type;
	size_t	TexIndex;
	float	FrameRate;
	float	CurrentFrame;
	UINT2	Current;
	UINT2	Total; //How many Columns/Rows in SpriteSheet
	FLOAT2	Size;
	FLOAT3	Offset;
};
