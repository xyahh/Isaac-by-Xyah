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
	Physics& ActorPhysics = Engine.GetEntityPhysics(m_Actor);
	DX XMVECTOR Color = DX4 Load(m_Color);
	DX XMVECTOR Position = DX Add
	(
		DX Scale(ActorPhysics.GetPosition(), Interpolation),
		DX Scale(ActorPhysics.GetPrevPosition(), 1.f - Interpolation)
	);
#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(Position, ActorPhysics);
#endif

	World::Convert(Position);

	for (auto& Sprite : m_Sprites)
	{
		DX XMVECTOR SpriteSize = Sprite.second.GetSize();
		float SpriteOffset = Sprite.second.GetOffsetY();
		World::Convert(SpriteOffset);
		World::Convert(SpriteSize);
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

/*-------------------------------------------------------------------------*/

void VisualGraphics::SetSize(float x, float y)
{
	m_Size = DX XMFLOAT2(x, y);
}

void VisualGraphics::SetTexID(const id_type& TexID)
{
	m_TexID = TexID;
}

void VisualGraphics::SetOffset(float x, float y)
{
	m_Offset = DX XMFLOAT2(x, y);
}

void VisualGraphics::Render()
{
	DX XMVECTOR Size = DX2 Load(m_Size);
	DX XMVECTOR Color = DX4 Load(m_Color);
	DX XMVECTOR Position = DX2 Load(m_Offset);

	// Convert the units to pixels if the Visual uses Physical Units.
	if (m_Config & PHYSICAL_UNITS)
	{
		World::Convert(Position);
		World::Convert(Size);
	}

	// Change PosZ if configured to draw at the Back or at the Front
	if (m_Config & FRONT_DRAW & ~BACK_DRAW & ~PHYSICAL_DRAW)
		Position = DX XMVectorSetZ(Position, NEAREST);
	if (m_Config & BACK_DRAW & ~FRONT_DRAW & ~PHYSICAL_DRAW)
		Position = DX XMVectorSetZ(Position, FARTHEST);
	RenderDevice.DrawTexRect(Position, Size, Color
		, Engine.GetTexture(m_TexID));
}

void EffectGraphics::Render(float fInterpolation)
{
	DX XMVECTOR Position = DX3 Load(m_Position);
	DX XMVECTOR Size = Effect.GetSize();
	DX XMVECTOR Color = DX4 Load(m_Color);
	World::Convert(Position);
	World::Convert(Size);
	RenderDevice.DrawSprite(Position, Size, Color, 
		Engine.GetTexture(Effect.GetTexID()), Effect.GetSpriteInfo());
	Effect.FrameGridUpdate();
}

void EffectGraphics::SetPosition(DX XMVECTOR pos)
{
	m_Position = DX3 Store(pos);
}
