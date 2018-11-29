#pragma once
#include "Sprite.h"

class Graphics
{
	friend Cyan;

public:
	Graphics() :
		m_Color(1.f, 1.f, 1.f, 1.f),
		m_Position(0.f, 0.f, 0.f)
	{}
	~Graphics() {}

	void SetColor(float r, float g, float b, float a);
	void AddSprite(const id_type& SpriteName);
	
	void Render(float Interpolation);

	Sprite& GetSprite(const id_type& SpriteName);
	void SetID(const id_type& ActorID);
	id_type GetActor() const;

	void SetPosition(DX XMVECTOR pos);

protected:
	id_type	m_Actor;
	DX XMFLOAT3 m_Position;
	DX XMFLOAT4	m_Color;
	STD map<id_type, Sprite> m_Sprites;
};