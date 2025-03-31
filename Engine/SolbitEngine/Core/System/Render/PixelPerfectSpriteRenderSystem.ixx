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
	class PixelPerfectSpriteRenderSystem : public ISystem
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;

		void Execute(const Entity entity, const Entity cameraEntity);
	};
}

namespace solbit
{
	void PixelPerfectSpriteRenderSystem::OnCreate()
	{

	}

	void PixelPerfectSpriteRenderSystem::OnUpdate()
	{
		
	}

	void PixelPerfectSpriteRenderSystem::OnDestroy()
	{

	}

	void PixelPerfectSpriteRenderSystem::Execute(const Entity entity, const Entity cameraEntity)
	{
		// TODO: 임시 코드
		if (!EntityManager::GetActive()->HasComponent<Transform2D>(entity) || !EntityManager::GetActive()->HasComponent<SpriteRenderer>(entity))
		{
			return;
		}
		// Unpack sprite components
		const Transform2D& transform = EntityManager::GetActive()->GetComponent<Transform2D>(entity);
		const SpriteRenderer& spriteRenderer = EntityManager::GetActive()->GetComponent<SpriteRenderer>(entity);
		// Unpack Camera components
		Transform2D& cameraTransform = EntityManager::GetActive()->GetComponent<Transform2D>(cameraEntity);
		PixelPerfectCamera& pixelPerfect = EntityManager::GetActive()->GetComponent<PixelPerfectCamera>(cameraEntity);
		// Sprite
		Sprite& sprite = SpriteManager::GetInstance()->Get(spriteRenderer.SpriteTexId);
		const uint32 ppu = sprite.PixelPerUnit;
		const FVector2 spriteResolution = static_cast<FVector2>(sprite.GetResoulution());
		FVector2 spriteSize = transform.Scale * FVector2{static_cast<FLOAT>(sprite.Rectangle.W), static_cast<FLOAT>(sprite.Rectangle.H) } / static_cast<FLOAT>(ppu);

		FRectangle entityRect;
		entityRect.X = transform.Position.X - 0.5f * spriteSize.X;
		entityRect.Y = transform.Position.Y - 0.5f * spriteSize.Y;
		entityRect.W = spriteSize.X;
		entityRect.H = spriteSize.Y;

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

		FRectangle intersection = rtRect.Intersect(entityRect);
		if (!intersection.IsValid())
			return;

		IRectangle srcRect;
		srcRect.X = sprite.Rectangle.X;
		srcRect.Y = sprite.Rectangle.Y;
		srcRect.W = sprite.Rectangle.W;
		srcRect.H = sprite.Rectangle.H;
		IRectangle dstRect;
		dstRect.X = static_cast<int32>((entityRect.X - rtRect.X) * ppu);
		dstRect.Y = static_cast<int32>((entityRect.Y - rtRect.Y) * ppu);
		dstRect.W = static_cast<int32>(entityRect.W * ppu);
		dstRect.H = static_cast<int32>(entityRect.H * ppu);
		dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

		renderer->RenderSprite(sprite.GetTexture(), srcRect, dstRect, transform.Rotation);
	}
}