#pragma once

class Sprite
{
public:

	Sprite() :
		Size(0.f, 0.f),
		FrameRate(1), CurrentFrame(0.f),
		SpriteInfo(0, 0, 1, 1)
	{}
	~Sprite() {}

	void SetTexID(const id_type& _TexID);
	void SetDirection(u_int _State);
	
	u_int GetDirection() const;

	void SetFrameRate(int FrameRate);
	void SetSize(const DX XMFLOAT2& _Size);
	void SetSpriteInfo(const DX XMINT4& _SpriteInfo);
	void ResetSprite();

	bool FrameLinearNext();

	bool FrameLinearUpdate();
	bool FrameGridUpdate();

	id_type GetTexID() const;
	DX XMVECTOR XM_CALLCONV GetSize() const;
	DX XMVECTOR XM_CALLCONV GetSpriteInfo() const;

private:
	id_type		TexID;
	float		FrameRate;
	float		CurrentFrame;
	DX XMFLOAT2 Size;
	DX XMINT4	SpriteInfo;
};
