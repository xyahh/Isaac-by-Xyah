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

public:

	Sprite();
	~Sprite() {}

	void SetTexture(const STD string&  TexIdx);
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

	const STD string& GetTexture() const;
	u_int GetDirection() const;

	SSE_VECTOR SSE_CALLCONV GetSize() const;
	SSE_VECTOR SSE_CALLCONV GetCurrent() const;
	SSE_VECTOR SSE_CALLCONV GetTotal() const;
	SSE_VECTOR SSE_CALLCONV GetOffset() const;

	void SetLayerGroup(u_int Layer);
	void SetColor(SSE_VECTOR Color);
	void SetAlpha(float Value);
	SSE_VECTOR SSE_CALLCONV GetColor() const;

	void Render(const Renderer& RenderDevice, SSE_VECTOR_PARAM1 Position);

private:

	u_int		Type;
	STD string	TexIndex;
	float		FrameRate;
	float		CurrentFrame;
	UINT2		Current;
	UINT2		Total; //How many Columns/Rows in SpriteSheet
	FLOAT2		Size;
	FLOAT3		Offset;
	FLOAT4		Color;
	u_int		Layer;
};
