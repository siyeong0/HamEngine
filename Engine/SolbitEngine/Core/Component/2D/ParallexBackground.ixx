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

		ParallexBackground()
			: TexId(0)
			, EffectValue({ 0.0f, 0.0f })
		{

		}
	};
}