#include "stdafx.h"
#include "Graphics.h"
#include "CyanEngine.h"

#ifdef CYAN_DEBUG_COLLISION
void DrawBoundingBoxes(const Renderer& RenderDevice,   SSE_VECTOR_PARAM1 Position,  SSE_VECTOR_PARAM1 Size)
{
	 SSE_VECTOR BBoxCenter = Collision::GetBBoxCenter(Position,  GetZ(Size));
	RenderDevice.DrawCollisionRect(BBoxCenter, Size);
}	 
#endif

Graphics::Graphics() :
	m_LayerGroup(LayerGroup::Middleground),
	m_Color(1.f, 1.f, 1.f, 1.f)
{}

void Graphics::SetLayerGroup(u_int Layer)
{
	m_LayerGroup = Layer;
}

void Graphics::SetColor( SSE_VECTOR Color)
{
	m_Color = StoreFloat4(Color);
}

void Graphics::SetAlpha(float Value)
{
	m_Color.w = Value;
}

 SSE_VECTOR SSE_CALLCONV Graphics::GetColor() const
{
	return LoadFloat4(m_Color);
}

void Graphics::Render (
	const Renderer& RenderDevice, 
	Physics& ObjectPhysics, 
	const STD vector<Sprite>& ObjectSprite, 
	float Interpolation)
{
	 SSE_VECTOR Position =  Add
	(
		 Scale(ObjectPhysics.GetPosition(), Interpolation),
		 Scale(ObjectPhysics.GetPrevPosition(), 1.f - Interpolation)
	);

	for (auto& Sprite : ObjectSprite)
	{
		 SSE_VECTOR SpriteSize = Sprite.GetSize();
		 SSE_VECTOR SpriteOffset = Sprite.GetOffset();

		RenderDevice.DrawShadow(Position, SpriteSize, m_Color.w);
		RenderDevice.DrawSprite
		(
			 Add(Position, SpriteOffset)
			, SpriteSize
			, m_Color
			, Engine.GetTexture(Sprite.GetTexture())
			, Sprite.Current
			, Sprite.Total
			, m_LayerGroup
		);
	}

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(RenderDevice, Position, ObjectPhysics.Box().GetDimensions());
#endif
}