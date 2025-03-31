module;

#include <box2d\box2d.h>

import Common;
import Math;
import SolbitEngine.Resource.Sprite;

export module SolbitEngine.Component.BoxCollider2D;

export namespace solbit
{
	struct BoxCollider2D
	{
		FVector2 Size;
		FVector2 Offset;

		BoxCollider2D()
			: Size({ 1.f, 1.f })
			, Offset({ 0.f, 0.f })
		{

		}

		void MatchSprite(const Sprite& sprite)
		{
			// TODO: Alpha값 고려해서
			Size = FVector2{ static_cast<FLOAT>(sprite.Rectangle.W),static_cast<FLOAT>(sprite.Rectangle.H) } / static_cast<FLOAT>(sprite.PixelPerUnit);
		}
	};
}