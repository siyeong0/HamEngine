module;

import Common;
import Math;
import SolbitSTL;

export module SolbitEngine.Component.Tilemap;

export namespace solbit
{
	using Tile = ID;
	constexpr Tile EMPTY_TILE = 0;

	struct Tilemap
	{
		static constexpr uint32 W = 32;
		static constexpr uint32 H = 32;

		Array<Tile, W * H> Map;

		Tilemap();

		inline const Tile& Get(const IVector2& idx) const;
		inline Tile& operator[](const IVector2& idx);
		inline FVector2 GetCenter(const IVector2& idx) const;
		inline void Clear(Tile tile = EMPTY_TILE);
	};
}

namespace solbit
{
	Tilemap::Tilemap()
		: Map()
	{
		Clear(EMPTY_TILE);
	}

	inline const Tile& Tilemap::Get(const IVector2& idx) const
	{
		return Map[idx.Y * W + idx.X];
	}

	inline Tile& Tilemap::operator[](const IVector2& idx)
	{
		return Map[idx.Y * W + idx.X];
	}

	inline FVector2 Tilemap::GetCenter(const IVector2& idx) const
	{
		return -(static_cast<FVector2>(IVector2{ W, H }) / 2.f) + static_cast<const FVector2>(idx);
	}

	inline void Tilemap::Clear(Tile tile)
	{
		Map.fill(EMPTY_TILE);
	}
}