#include "stdafx.h"
#include "Graphics.h"
#include "CyanEngine.h"

#ifdef CYAN_DEBUG_COLLISION
void DrawBoundingBoxes(const Renderer& RenderDevice,  DX FXMVECTOR Position, DX FXMVECTOR Size)
{
	DX XMVECTOR BBoxCenter = Collision::GetBBoxCenter(Position, DX GetZ(Size));
	RenderDevice.DrawCollisionRect(BBoxCenter, Size);
}	 
#endif

void Graphics::SetColor(float r, float g, float b, float a)
{
	m_Color = DX XMFLOAT4(r, g, b, a);
}

void Graphics::Render (
	const Renderer& RenderDevice, 
	Physics& ObjectPhysics, 
	const STD vector<Sprite>& ObjectSprite, 
	float Interpolation)
{
	DX XMVECTOR Color = DX4 Load(m_Color);
	DX XMVECTOR Position = DX Add
	(
		DX Scale(ObjectPhysics.GetPosition(), Interpolation),
		DX Scale(ObjectPhysics.GetPrevPosition(), 1.f - Interpolation)
	);

	for (auto& Sprite : ObjectSprite)
	{
		DX XMVECTOR SpriteSize = Sprite.GetSize();
		DX XMVECTOR SpriteOffset = Sprite.GetOffset();

		RenderDevice.DrawShadow(Position, SpriteSize, Color);
		RenderDevice.DrawSprite
		(
			DX Add(Position, SpriteOffset)
			, SpriteSize
			, Color
			, Engine.GetTexture(Sprite.GetTexture())
			, Sprite.GetCurrent()
			, Sprite.GetTotal()
		);
	}

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(RenderDevice, Position, ObjectPhysics.Box().GetDimensions());
#endif
}