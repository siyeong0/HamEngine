module;

import Common;
import Math;

export module SolbitEngine.Resource.Tileset;

export namespace solbit
{
	struct Tileset
	{
	public:
		using FLAG = uint8;

		ID TexId;
		FLAG Flag;
		// MaterialId
		ID PhysicalMaterialId;
		IVector2 UnitSize;
	public:
		Tileset();
		Tileset(ID texId, ID physMatId, const IVector2& unitSize);

		static inline IVector2 GetTexOffset(const FLAG& flag);
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

	inline IVector2 Tileset::GetTexOffset(const FLAG& flag)
	{
		FLAG variationFlag = (flag & 0xF0) >> 4;
		FLAG fillInfoFlag = flag & 0x0F;
		return IVector2{ static_cast<int>(fillInfoFlag), static_cast<int>(variationFlag) };
	}
}