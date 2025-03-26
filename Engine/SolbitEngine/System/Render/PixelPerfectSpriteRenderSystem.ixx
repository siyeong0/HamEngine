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
		const int32 ppu = pixelPerfect.PixelPerUnit;
		const IVector2& refRes = pixelPerfect.RefResoulution;
		// Texture
		Texture& texture = TextureManager::GetInstance()->Get(spriteRenderer.SpriteTexId);
		const IVector2 texRes = texture.GetResoulution();
		const FVector2 texResF = static_cast<FVector2>(texture.GetResoulution());
		FLOAT scaleY = texResF.Y / texResF.X;

		FRectangle spriteRect;
		spriteRect.X = (transform.Position.X - 0.5f * transform.Scale.X) * ppu;
		spriteRect.Y = (transform.Position.Y - 0.5f * scaleY * transform.Scale.Y) * ppu;
		spriteRect.W = 1.f * transform.Scale.X * ppu;
		spriteRect.H = 1.f * scaleY * transform.Scale.Y * ppu;

		Renderer* renderer = Renderer::GetInstance();
		const IVector2 rtSize = renderer->GetRTSize();
		const FVector2 rtSizeF = static_cast<FVector2>(renderer->GetRTSize());
		FRectangle rtRect;
		rtRect.X = cameraTransform.Position.X - rtSizeF.X / 2.f;
		rtRect.Y = cameraTransform.Position.Y - rtSizeF.Y / 2.f;
		rtRect.W = rtSizeF.X;
		rtRect.H = rtSizeF.Y;

		FRectangle intersection = rtRect.Intersect(spriteRect);
		if (!intersection.IsValid())
			return;
		
		IRectangle srcRect;
		srcRect.X = intersection.X == spriteRect.X ? 0 : static_cast<int32>(spriteRect.W - intersection.W);
		srcRect.Y = intersection.Y != spriteRect.Y ? 0 : static_cast<int32>(spriteRect.H - intersection.H);
		srcRect.W = texRes.X;
		srcRect.H = texRes.Y;
		IRectangle dstRect;
		dstRect.X = static_cast<int32>(Round(intersection.X - rtRect.X));
		dstRect.Y = static_cast<int32>(Round(intersection.Y - rtRect.Y));
		dstRect.W = static_cast<int32>(Round(intersection.W));
		dstRect.H = static_cast<int32>(Round(intersection.H));
		dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

		renderer->RenderSprite(texture, srcRect, dstRect, transform.Rotation);
	}
}