#pragma once

class Sprite
{
public:

	Sprite() :
		TexID(-1),
		Size(0.f, 0.f),
		FrameRate(1), CurrentFrame(0.f),
		SpriteInfo(0, 0, 1, 1)
	{}
	~Sprite() {}

	void SetTexID(int _TexID);
	void SetDirection(u_int _State);
	void SetFrameRate(int FrameRate);
	void SetSize(const DX XMFLOAT2& _Size);
	void SetSpriteInfo(const DX XMINT4& _SpriteInfo);
	void ResetSprite();
	bool FrameLinearAdvance();
	bool FrameGridAdvance();

	int GetTexID() const;
	DX XMVECTOR XM_CALLCONV GetSize() const;
	DX XMVECTOR XM_CALLCONV GetSpriteInfo() const;

private:
	int			TexID;
	float		FrameRate;
	float		CurrentFrame;
	DX XMFLOAT2 Size;
	DX XMINT4	SpriteInfo;
};
