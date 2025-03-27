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
		tmRect.X = (transform.Position.X - static_cast<FLOAT>(Tilemap::W) / 2.f);
		tmRect.Y = (transform.Position.Y - static_cast<FLOAT>(Tilemap::H) / 2.f);
		tmRect.W = static_cast<FLOAT>(Tilemap::W);
		tmRect.H = static_cast<FLOAT>(Tilemap::H);

		Renderer* renderer = Renderer::GetInstance();
		const IVector2 rtSize = renderer->GetRTSize();
		const FVector2 rtSizeF = static_cast<FVector2>(renderer->GetRTSize()) / static_cast<FLOAT>(ppu);
		FRectangle rtRect;
		rtRect.X = cameraTransform.Position.X - rtSizeF.X / 2.f;
		rtRect.Y = cameraTransform.Position.Y - rtSizeF.Y / 2.f;
		rtRect.W = rtSizeF.X;
		rtRect.H = rtSizeF.Y;

		FRectangle intersection = tmRect.Intersect(rtRect);
		if (!intersection.IsValid())
			return;

		const FVector2 startLocalPos = FVector2(intersection.X, intersection.Y) - FVector2{ tmRect.X, tmRect.Y };
		const FVector2 endLocalPos = FVector2(intersection.X + intersection.W, intersection.Y + intersection.H) - FVector2{ tmRect.X, tmRect.Y };

		const IVector2 startIdx = { Round(startLocalPos.X), Round(startLocalPos.Y) };
		const IVector2 endIdx = { Round(endLocalPos.X), Round(endLocalPos.Y) };

		for (int y = startIdx.Y; y < endIdx.Y; ++y)
		{
			for (int x = startIdx.X; x < endIdx.X; ++x)
			{
				const Tile tile = tilemap.Get(IVector2{ x,y });

				if (tile == EMPTY_TILE)
					continue;

				FVector2 tileCenter = tilemap.GetCenter(IVector2{ x,y });
				FRectangle tileRect;
				tileRect.X = transform.Position.X + (tileCenter.X - 0.5f);
				tileRect.Y = transform.Position.Y + (tileCenter.Y - 0.5f);
				tileRect.W = 1.0f;
				tileRect.H = 1.0f;

				const Tileset& tileset = TilesetManager::GetInstance()->Get(tile);
				Texture& texture = TextureManager::GetInstance()->Get(tileset.TexId);

				const Tile& up = tilemap.Get(IVector2{ x, y + 1 });
				const Tile& left = tilemap.Get(IVector2{ x - 1,y });
				const Tile& down = tilemap.Get(IVector2{ x, y - 1 });
				const Tile& right = tilemap.Get(IVector2{ x + 1,y });
				Tileset::FLAG neighborFlag = ((up != EMPTY_TILE) << 3) | ((left != EMPTY_TILE) << 2) | ((down != EMPTY_TILE) << 1) | ((right != EMPTY_TILE) << 0);
				Tileset::FLAG variationFlag = 0x0 << 4;
				const IVector2 texOffset = Tileset::GetTexOffset(neighborFlag | variationFlag);

				IRectangle srcRect;
				srcRect.X = texOffset.X * tileset.UnitSize.X;
				srcRect.Y = texOffset.Y * tileset.UnitSize.Y;
				srcRect.W = tileset.UnitSize.X;
				srcRect.H = tileset.UnitSize.Y;
				IRectangle dstRect;
				dstRect.X = static_cast<int32>((tileRect.X - rtRect.X) * ppu);
				dstRect.Y = static_cast<int32>((tileRect.Y - rtRect.Y) * ppu);
				dstRect.W = static_cast<int32>(tileRect.W * ppu);
				dstRect.H = static_cast<int32>(tileRect.H * ppu);
				dstRect.Y = rtSize.Y - dstRect.Y - dstRect.H;	// To screen space 

				renderer->RenderFixedSprite(texture, srcRect, dstRect);
			}
		}
	}
}