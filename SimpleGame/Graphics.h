#pragma once
#include "Sprite.h"

#define PHYSICAL_UNITS	0x01
#define SHADOW_DRAW		0x02
#define FRONT_DRAW		0x04
#define PHYSICAL_DRAW	0x08
#define BACK_DRAW		0x10

class Graphics
{
	friend Cyan;

public:
	Graphics() :
		m_Color(1.f, 1.f, 1.f, 1.f)
	{}
	~Graphics() {}

	void SetColor(float r, float g, float b, float a);
	void AddSprite(const id_type& SpriteName);
	
	void Render(float Interpolation);

	Sprite& GetSprite(const id_type& SpriteName);

	void SetID(const id_type& ActorID);
	id_type GetActor() const;

protected:
	id_type					 m_Actor;
	STD map<id_type, Sprite> m_Sprites;
	DX XMFLOAT4				 m_Color;
};

class VisualGraphics : public Graphics
{
public:
	VisualGraphics(WORD Config) : 
		m_Offset(0.f, 0.f), 
		m_Size(1.f, 1.f),
		m_Config(Config) {}
	virtual ~VisualGraphics() {}

	void SetSize(float x, float y);
	void SetTexID(const id_type& TexID);
	void SetOffset(float x, float y);
	void Render();

private:
	id_type		m_TexID;
	WORD		m_Config;
	DX XMFLOAT2 m_Offset;
	DX XMFLOAT2 m_Size;
};

class EffectGraphics : public Graphics
{
	friend Cyan;

public:
	EffectGraphics() {}
	virtual ~EffectGraphics() {}

	void Render(float fInterpolation);

	void SetPosition(DX XMVECTOR pos);
	Sprite Effect;

private:
	DX XMFLOAT3 m_Position;

};
