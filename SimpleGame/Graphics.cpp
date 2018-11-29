#include "stdafx.h"
#include "Graphics.h"
#include "CyanEngine.h"
#include "Renderer.h"
#include "World.h"

#ifdef CYAN_DEBUG_COLLISION
void DrawBoundingBoxes(DX FXMVECTOR Position, Physics& p)
{
	DX XMVECTOR Dimensions = p.GetBox().GetDimensions();
	DX XMVECTOR BoxPosition = p.GetBox().ConvertPosition(Position);
	World::Convert(Dimensions);
	World::Convert(BoxPosition);
	RenderDevice.DrawCollisionRect(BoxPosition, Dimensions);
}	 
#endif

void Graphics::SetColor(float r, float g, float b, float a)
{
	m_Color = DX XMFLOAT4(r, g, b, a);
}

void Graphics::AddSprite(const id_type& SpriteName)
{
	m_Sprites[SpriteName];
}

void Graphics::Render(float Interpolation)
{
	Physics* ActorPhysics = NULL;
	if(!m_Actor.empty()) ActorPhysics = &Engine.GetPhysics(m_Actor);
	DX XMVECTOR Position = DX3 Load(m_Position);
	DX XMVECTOR Color = DX4 Load(m_Color);
	
	if(ActorPhysics)
	Position = DX Add( Position, DX Add
	(
		DX Scale(ActorPhysics->GetPosition(), Interpolation),
		DX Scale(ActorPhysics->GetPrevPosition(), 1.f - Interpolation)
	));

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(Position, ActorPhysics);
#endif

	World::ToPixels(Position);
	for (auto& Sprite : m_Sprites)
	{
		DX XMVECTOR SpriteSize = Sprite.second.GetSize();
		float SpriteOffset = Sprite.second.GetOffsetY();
		World::ToPixels(SpriteOffset);
		World::ToPixels(SpriteSize);
		RenderDevice.DrawShadow(Position, SpriteSize, Color);
		RenderDevice.DrawSprite(DX Add(Position, { 0.f, 0.f, SpriteOffset }),
			SpriteSize, Color, Engine.GetTexture(Sprite.second.GetTexID()), Sprite.second.GetSpriteInfo());
	}
}

Sprite& Graphics::GetSprite(const id_type& SpriteName)
{
	return m_Sprites[SpriteName];
}

void Graphics::SetID(const id_type & ActorID)
{
	m_Actor = ActorID;
}

id_type Graphics::GetActor() const
{
	return m_Actor;
}

void Graphics::SetPosition(DX XMVECTOR pos)
{
	m_Position = DX3 Store(pos);
}