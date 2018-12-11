#pragma once
class Graphics
{
	friend Cyan;

public:

	Graphics() :
		m_Color(1.f, 1.f, 1.f, 1.f)
	{}
	~Graphics() {}

	void SetColor(DX XMVECTOR Color);
	void SetAlpha(float Value);
	DX XMVECTOR XM_CALLCONV GetColor() const;

private:

	void Render
	(
		const Renderer& RenderDevice,
		Physics& ObjectPhysics, 
		const STD vector<Sprite>& ObjectSprite, 
		float Interpolation
	);

protected:
	DX XMFLOAT4			m_Color;
};