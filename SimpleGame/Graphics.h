#pragma once
class Graphics
{
	friend Cyan;

public:

	Graphics() :
		m_Color(1.f, 1.f, 1.f, 1.f)
	{}
	~Graphics() {}

	void SetColor(float r, float g, float b, float a);

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