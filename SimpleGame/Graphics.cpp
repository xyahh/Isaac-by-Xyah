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

void ActorGraphics::Render(float fInterpolation)
{
	Physics& ActorPhysics = Engine.GetActorPhysics(ActorID);

	DX XMVECTOR HeadSize = Head.GetSize();
	DX XMVECTOR BodySize = Body.GetSize();
	DX XMVECTOR Color = DX4 Load(m_Color);
	

	DX XMVECTOR Position = DX Add
	(
		DX Scale(ActorPhysics.GetPosition(), fInterpolation),
		DX Scale(ActorPhysics.GetPrevPosition(), 1.f - fInterpolation)
	);
	

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(Position, ActorPhysics);
#endif

	World::Convert(Position);
	World::Convert(HeadSize);
	World::Convert(BodySize);

	float BodyOffY{ BodyOffsetY };
	float HeadOffY{ HeadOffsetY };

	World::Convert(BodyOffY);
	World::Convert(HeadOffY);

	RenderDevice.DrawShadow(Position, HeadSize, Color);
	RenderDevice.DrawShadow(Position, BodySize, Color);

	RenderDevice.DrawSprite(DX Add(Position, { 0.f, 0.f, BodyOffY}), 
		BodySize, Color, Engine.GetTexture(Body.GetTexID()), Body.GetSpriteInfo());
	RenderDevice.DrawSprite(DX Add(Position, { 0.f, 0.f, HeadOffY}), 
		HeadSize, Color, Engine.GetTexture(Head.GetTexID()), Head.GetSpriteInfo());
}

void ActorGraphics::SetSpriteOffset(float HeadOffY, float BodyOffY)
{
	HeadOffsetY = HeadOffY;
	BodyOffsetY = BodyOffY;
}

void ObjectGraphics::Render(float fInterpolation)
{
	Physics& BulletPhysics = Engine.GetObjectPhysics(ObjectID);

	DX XMVECTOR Size = ObjectSprite.GetSize();
	DX XMVECTOR Color = DX4 Load(m_Color);

	DX XMVECTOR Position = DX Add
	(
		DX Scale(BulletPhysics.GetPosition(), fInterpolation),
		DX Scale(BulletPhysics.GetPrevPosition(), 1.f - fInterpolation)
	);

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(Position, BulletPhysics);
#endif

	World::Convert(Position);
	World::Convert(Size);

	RenderDevice.DrawShadow(Position, Size, Color);
	RenderDevice.DrawSprite(DX Add(Position, { 0.f, 0.f, DX GetY(Size) *0.5f }), 
		Size, Color, Engine.GetTexture(ObjectSprite.GetTexID()), ObjectSprite.GetSpriteInfo());


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
