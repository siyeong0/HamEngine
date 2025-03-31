module;

import Common;
import Math;

export module SolbitEngine.Component.ParallexBackground;

export namespace solbit
{
	struct ParallexBackground
	{
		ID TexId;
		FVector2 EffectValue;
		bool FreezeX;
		bool FreezeY;

		ParallexBackground()
			: TexId(0)
			, EffectValue({ 0.0f, 0.0f })
			, FreezeX(false)
			, FreezeY(true)
		{

		}
	};
}