#pragma once
class Graphics
{
	friend Cyan;
	
public:

	Graphics();
	~Graphics() {}

	void SetLayerGroup(u_int Layer);

	void SetColor( SSE_VECTOR Color);
	void SetAlpha(float Value);
	 SSE_VECTOR SSE_CALLCONV GetColor() const;

private:

	void Render
	(
		const Renderer& RenderDevice,
		Physics& ObjectPhysics, 
		const STD vector<Sprite>& ObjectSprite, 
		float Interpolation
	);

protected:
	 FLOAT4		m_Color;
	u_int			m_LayerGroup;
};