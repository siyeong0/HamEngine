module;

import Common;
import Math;
import ECS;
import SolbitEngine.Renderer;
import SolbitEngine.Resource;
import SolbitEngine.GameObject;
import SolbitEngine.Component;

export module SolbitEngine.System.ParallexBackgroundSys;

export namespace solbit
{
	class ParallexBackgroundSys : public ISystem
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;

		void Execute(const Entity entity, const Entity cameraEntity);

		IVector2 Resolution = { 1920, 1080 };
	};
}

namespace solbit
{
	void ParallexBackgroundSys::OnCreate()
	{
		
	}

	void ParallexBackgroundSys::OnUpdate()
	{

	}

	void ParallexBackgroundSys::OnDestroy()
	{

	}

	void ParallexBackgroundSys::Execute(const Entity entity, const Entity cameraEntity)
	{
		// TODO: 임시 코드
		if (!EntityManager::GetActive()->HasComponent<ParallexBackground>(entity))
		{
			return;
		}
		// Unpack background components
		const ParallexBackground& background = EntityManager::GetActive()->GetComponent<ParallexBackground>(entity);
		Texture& texture = TextureManager::GetInstance()->Get(background.TexId);
		// Unpack Camera components
		const Transform2D& cameraTransform = EntityManager::GetActive()->GetComponent<Transform2D>(cameraEntity);
		const PixelPerfectCamera& pixelPerfect = EntityManager::GetActive()->GetComponent<PixelPerfectCamera>(cameraEntity);

		const uint32 ppu = pixelPerfect.PixelPerUnit;
		const FVector2 renderTargetSize = static_cast<FVector2>(pixelPerfect.RefResoulution) / ppu;
		FVector2 relativePosition = -cameraTransform.Position * background.EffectValue;
		auto positiveMod = [](FLOAT x, FLOAT y) -> FLOAT
			{
				FLOAT mod = std::fmod(x, y);
				if (mod < 0) mod += std::fabsf(y);  // 음수일 경우 양수로 변환
				return mod;
			};

		FVector2 offset = { 0.0f, 0.0f };
		offset.X = positiveMod(relativePosition.X, 40.0f) / 40.0f;
		offset.Y = positiveMod(relativePosition.Y, 22.5f) / 22.5f;

		int srcOffsetX = Round((1.0f - offset.X) * texture.GetWidth());
		int srcOffsetY = Round((1.0f - offset.Y) * texture.GetHeight());
		int dstOffsetX = Round(offset.X * Resolution.X);
		int dstOffsetY = Round(offset.Y * Resolution.Y);
		ASSERT(offset.X >= 0 && offset.X <= 1.0f && offset.Y >= 0 && offset.Y <= 1.0f);
		{
			IRectangle srcRect;
			srcRect.X = srcOffsetX;
			srcRect.Y = srcOffsetY;
			srcRect.W = texture.GetWidth() - srcOffsetX;
			srcRect.H = texture.GetHeight() - srcOffsetY;
			IRectangle dstRect;
			dstRect.X = 0;
			dstRect.Y = 0;
			dstRect.W = dstOffsetX;
			dstRect.H = dstOffsetY;

			dstRect.Y = Resolution.Y - dstRect.Y - dstRect.H;	// To screen space 
			Renderer::GetInstance()->RenderFixedSprite(texture, srcRect, dstRect);
		}
		{
			IRectangle srcRect;
			srcRect.X = srcOffsetX;
			srcRect.Y = 0;
			srcRect.W = texture.GetWidth() - srcOffsetX;
			srcRect.H = srcOffsetY;
			IRectangle dstRect;
			dstRect.X = 0;
			dstRect.Y = dstOffsetY;
			dstRect.W = dstOffsetX;
			dstRect.H = Resolution.Y - dstOffsetY;

			dstRect.Y = Resolution.Y - dstRect.Y - dstRect.H;	// To screen space 
			Renderer::GetInstance()->RenderFixedSprite(texture, srcRect, dstRect);
		}
		{
			IRectangle srcRect;
			srcRect.X = 0;
			srcRect.Y = srcOffsetY;
			srcRect.W = srcOffsetX;
			srcRect.H = texture.GetHeight() - srcOffsetY;
			IRectangle dstRect;
			dstRect.X = dstOffsetX;
			dstRect.Y = 0;
			dstRect.W = Resolution.X - dstOffsetX;
			dstRect.H = dstOffsetY;

			dstRect.Y = Resolution.Y - dstRect.Y - dstRect.H;	// To screen space 
			Renderer::GetInstance()->RenderFixedSprite(texture, srcRect, dstRect);
		}
		{
			IRectangle srcRect;
			srcRect.X = 0;
			srcRect.Y = 0;
			srcRect.W = srcOffsetX;
			srcRect.H = srcOffsetY;
			IRectangle dstRect;
			dstRect.X = dstOffsetX;
			dstRect.Y = dstOffsetY;
			dstRect.W = Resolution.X - dstOffsetX;
			dstRect.H = Resolution.Y - dstOffsetY;

			dstRect.Y = Resolution.Y - dstRect.Y - dstRect.H;	// To screen space 
			Renderer::GetInstance()->RenderFixedSprite(texture, srcRect, dstRect);
		}
	}
}