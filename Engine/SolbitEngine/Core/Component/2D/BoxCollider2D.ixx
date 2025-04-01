module;

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
		FLOAT Rounded;
		bool EdgeGhost;

		BoxCollider2D()
			: Size({ 1.0f, 1.0f })
			, Offset({ 0.0f, 0.0f })
			, Rounded(0.0f)
		{

		}

		void MatchSprite(const Sprite& sprite)
		{
			// TODO: Alpha값 고려해서
			Size = FVector2{ static_cast<FLOAT>(sprite.Rectangle.W),static_cast<FLOAT>(sprite.Rectangle.H) } / static_cast<FLOAT>(sprite.PixelPerUnit);
		}
	};
}