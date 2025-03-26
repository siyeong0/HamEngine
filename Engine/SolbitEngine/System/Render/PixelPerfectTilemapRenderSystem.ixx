module;

import Common;
import Math;
import ECS;
import SolbitEngine.Renderer;
import SolbitEngine.Resource;
import SolbitEngine.GameObject;
import SolbitEngine.Component;

export module SolbitEngine.System.PixelPerfectTilemapRenderSystem;

export namespace solbit
{
	class PixelPerfectTilemapRenderSystem
	{
	public:
		PixelPerfectTilemapRenderSystem() = default;
		virtual ~PixelPerfectTilemapRenderSystem() = default;

		void Execute(const ComponentPack& tilemapComponentPack, const ComponentPack& cameraCompPack);
	};
}

namespace solbit
{
	void PixelPerfectTilemapRenderSystem::Execute(const ComponentPack& tilemapComponentPack, const ComponentPack& cameraCompPack)
	{
		// Unpack sprite components
		const Transform2D& transform = tilemapComponentPack.GetComponent<Transform2D>();
		const Tilemap& tilemap = tilemapComponentPack.GetComponent<Tilemap>();
		const TilemapRenderer& tilemapRenderer = tilemapComponentPack.GetComponent<TilemapRenderer>();
		// Unpack Camera components
		Transform2D& cameraTransform = cameraCompPack.GetComponent<Transform2D>();
		PixelPerfectCamera& pixelPerfect = cameraCompPack.GetComponent<PixelPerfectCamera>();
		const int32 ppu = pixelPerfect.PixelPerUnit;
		const IVector2& refRes = pixelPerfect.RefResoulution;

		FRectangle tmRect;
		tmRect.X = transform.Position.X - static_cast<FLOAT>(Tilemap::W) / 2.f;
		tmRect.Y = transform.Position.Y - static_cast<FLOAT>(Tilemap::H) / 2.f;
		tmRect.W = static_cast<FLOAT>(Tilemap::W);
		tmRect.H = static_cast<FLOAT>(Tilemap::H);

		Renderer* renderer = Renderer::GetInstance();
		const IVector2 rtSize = renderer->GetRTSize();
		const FVector2 rtSizeF = static_cast<FVector2>(renderer->GetRTSize());
		FRectangle rtRect;
		rtRect.X = cameraTransform.Position.X - rtSizeF.X / 2.f;
		rtRect.Y = cameraTransform.Position.Y - rtSizeF.Y / 2.f;
		rtRect.W = rtSizeF.X;
		rtRect.H = rtSizeF.Y;

		FRectangle intersection = tmRect.Intersect(tmRect);
		if (!intersection.IsValid())
			return;

		FVector2 startLocalPos = FVector2(intersection.X, intersection.Y) - FVector2{ tmRect.X, tmRect.Y };
		FVector2 endLocalPos = FVector2(intersection.X + intersection.W, intersection.Y + intersection.H) - FVector2{ tmRect.X, tmRect.Y };

		IVector2 startIdx = { static_cast<int32>(startLocalPos.X), static_cast<int32>(startLocalPos.Y) };
		IVector2 endIdx = { static_cast<int32>(endLocalPos.X), static_cast<int32>(endLocalPos.Y) };

		for (int y = startIdx.Y; y < endIdx.Y; ++y)
		{
			for (int x = startIdx.X; x < endIdx.X; ++x)
			{
				const Tile& tile = tilemap.Get(IVector2{ x,y });

				if (tile == EMPTY_TILE)
					continue;

				FVector2 tileCenter = tilemap.GetCenter(IVector2{ x,y });
				FRectangle tileRect;
				tileRect.X = (tileCenter.X - 0.5f) * ppu;
				tileRect.Y = (tileCenter.Y - 0.5f) * ppu;
				tileRect.W = ppu;
				tileRect.H = ppu;

				FRectangle intersection = rtRect.Intersect(tileRect);
				//ASSERT(intersection.IsValid());
				if (!intersection.IsValid())
					continue;

				const Tileset& tileset = TilesetManager::GetInstance()->Get(tile);
				Texture& texture = TextureManager::GetInstance()->Get(tileset.TexId);
				const IVector2 texRes = texture.GetResoulution();
				const FVector2 texResF = static_cast<FVector2>(texture.GetResoulution());
				FLOAT scaleY = texResF.Y / texResF.X;

				IRectangle srcRect;
				srcRect.X = intersection.X == tileRect.X ? 0 : static_cast<int32>(tileRect.W - intersection.W);
				srcRect.Y = intersection.Y != tileRect.Y ? 0 : static_cast<int32>(tileRect.H - intersection.H);
				srcRect.W = static_cast<int32>(Round(intersection.W));
				srcRect.H = static_cast<int32>(Round(intersection.H));
				IRectangle dstRect;
				dstRect.X = static_cast<int32>(Round(intersection.X - rtRect.X));
				dstRect.Y = static_cast<int32>(Round(intersection.Y - rtRect.Y));
				dstRect.W = static_cast<int32>(Round(intersection.W));
				dstRect.H = static_cast<int32>(Round(intersection.H));
				dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

				renderer->RenderSprite(texture, srcRect, dstRect, transform.Rotation);
			}
		}
	}
}