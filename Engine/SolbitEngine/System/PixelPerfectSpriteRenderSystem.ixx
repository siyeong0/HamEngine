module;

import Common;
import Memory;
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
		const Vec2i& refRes = pixelPerfect.RefResoulution;
		// Texture
		Texture& texture = TextureManager::GetInstance()->GetTexture(spriteRenderer.SpriteTexId);
		const Vec2i texRes = texture.GetResoulution();
		const Vec2 texResF = static_cast<Vec2>(texture.GetResoulution());
		FLOAT scaleY = texResF.Y / texResF.X;

		Rect spriteRect;
		spriteRect.X = (transform.Position.X - 0.5f * transform.Scale.X) * ppu;
		spriteRect.Y = (transform.Position.Y - 0.5f * scaleY * transform.Scale.Y) * ppu;
		spriteRect.W = 1.f * transform.Scale.X * ppu;
		spriteRect.H = 1.f * scaleY * transform.Scale.Y * ppu;

		Renderer* renderer = Renderer::GetInstance();
		const Vec2i rtSize = renderer->GetRTSize();
		const Vec2 rtSizeF = static_cast<Vec2>(renderer->GetRTSize());
		Rect rtRect;
		rtRect.X = cameraTransform.Position.X - rtSizeF.X / 2.f;
		rtRect.Y = cameraTransform.Position.Y - rtSizeF.Y / 2.f;
		rtRect.W = rtSizeF.X;
		rtRect.H = rtSizeF.Y;

		Rect intersection = rtRect.Intersect(spriteRect);
		if (!intersection.IsValid())
			return;
		
		Recti srcRect;
		srcRect.X = intersection.X == spriteRect.X ? 0 : static_cast<int32>(spriteRect.W - intersection.W);
		srcRect.Y = intersection.Y != spriteRect.Y ? 0 : static_cast<int32>(spriteRect.H - intersection.H);
		srcRect.W = static_cast<int32>(Round(intersection.W * texRes.X / transform.Scale.X));
		srcRect.H = static_cast<int32>(Round(intersection.H * texRes.Y / transform.Scale.Y));
		Recti dstRect;
		dstRect.X = static_cast<int32>(Round(intersection.X - rtRect.X));
		dstRect.Y = static_cast<int32>(Round(intersection.Y - rtRect.Y));
		dstRect.W = static_cast<int32>(Round(intersection.W));
		dstRect.H = static_cast<int32>(Round(intersection.H));
		dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

		renderer->RenderSprite(texture, srcRect, dstRect, transform.Rotation);
	}
}