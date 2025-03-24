module;

import Common;
import Math;

export module HamEngine.Component.BoxCollider2D;

export namespace ham
{
	struct BoxCollider2D
	{
		Vec2 Size;
		Vec2 Offset;

		BoxCollider2D()
			: Size({ 1.f, 1.f })
			, Offset({ 0.f, 0.f })
		{

		}
	};
}