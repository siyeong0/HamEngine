module;

import Common;
import Math;

export module SolbitEngine.Resource.Tileset;

export namespace solbit
{
	struct Tileset
	{
		ID TexId;
		// MaterialId
		ID PhysicalMaterialId;
		IVector2 UnitSize;

		Tileset();
		Tileset(ID texId, ID physMatId, const IVector2& unitSize);
	};
}

namespace solbit
{
	Tileset::Tileset()
		: TexId(0)
		, PhysicalMaterialId(0)
		, UnitSize(IVector2{ 0, 0 })
	{

	}

	Tileset::Tileset(ID texId, ID physMatId, const IVector2& unitSize)
		: TexId(texId)
		, PhysicalMaterialId(physMatId)
		, UnitSize(unitSize)
	{

	}
}