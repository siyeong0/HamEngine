module;

import Common;
import Math;
import ECS;
import SolbitEngine.Renderer;
import SolbitEngine.Resource;
import SolbitEngine.GameObject;
import SolbitEngine.Component;

export module SolbitEngine.System.PixelPerfectSpriteRenderSystem;

export namespace solbit
{
	class PixelPerfectSpriteRenderSystem
	{
	public:
		PixelPerfectSpriteRenderSystem() = default;
		virtual ~PixelPerfectSpriteRenderSystem() = default;

		void Execute(const ComponentPack& entityComponentPack, const ComponentPack& cameraCompPack);
	};
}

namespace solbit
{
	void PixelPerfectSpriteRenderSystem::Execute(const ComponentPack& entityComponentPack, const ComponentPack& cameraCompPack)
	{
		// Unpack sprite components
		const Transform2D& transform = entityComponentPack.GetComponent<Transform2D>();
		const SpriteRenderer& spriteRenderer = entityComponentPack.GetComponent<SpriteRenderer>();
		// Unpack Camera components
		Transform2D& cameraTransform = cameraCompPack.GetComponent<Transform2D>();
		PixelPerfectCamera& pixelPerfect = cameraCompPack.GetComponent<PixelPerfectCamera>();
		// Sprite
		Sprite& sprite = SpriteManager::GetInstance()->Get(spriteRenderer.SpriteTexId);
		const uint32 ppu = sprite.PixelPerUnit;
		const FVector2 spriteResolution = static_cast<FVector2>(sprite.GetResoulution());
		FVector2 spriteSize = transform.Scale * static_cast<FVector2>(spriteResolution) / static_cast<FLOAT>(ppu);

		FRectangle spriteRect;
		spriteRect.X = transform.Position.X - 0.5f * spriteSize.X;
		spriteRect.Y = transform.Position.Y - 0.5f * spriteSize.Y;
		spriteRect.W = spriteSize.X;
		spriteRect.H = spriteSize.Y;

		Renderer* renderer = Renderer::GetInstance();
		const IVector2 rtSize = renderer->GetRTSize();
		const FVector2 rtSizeF = static_cast<FVector2>(renderer->GetRTSize()) / static_cast<FLOAT>(ppu);
		FRectangle rtRect;
		rtRect.X = cameraTransform.Position.X - rtSizeF.X / 2.f;
		rtRect.Y = cameraTransform.Position.Y - rtSizeF.Y / 2.f;
		rtRect.W = rtSizeF.X;
		rtRect.H = rtSizeF.Y;

		ASSERT(rtSize == pixelPerfect.RefResoulution);
		ASSERT(ppu == pixelPerfect.PixelPerUnit);

		FRectangle intersection = rtRect.Intersect(spriteRect);
		if (!intersection.IsValid())
			return;

		IRectangle srcRect;
		srcRect.X = 0;
		srcRect.Y = 0;
		srcRect.W = spriteResolution.X;
		srcRect.H = spriteResolution.Y;
		IRectangle dstRect;
		dstRect.X = static_cast<int32>((spriteRect.X - rtRect.X) * ppu);
		dstRect.Y = static_cast<int32>((spriteRect.Y - rtRect.Y) * ppu);
		dstRect.W = static_cast<int32>(spriteRect.W * ppu);
		dstRect.H = static_cast<int32>(spriteRect.H * ppu);
		dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

		renderer->RenderSprite(sprite.GetTexture(), srcRect, dstRect, transform.Rotation);
	}
}