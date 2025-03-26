module;

#include <box2d\box2d.h>

import Common;
import Math;

export module SolbitEngine.Component.BoxCollider2D;

export namespace solbit
{
	struct BoxCollider2D
	{
		FVector2 Size;
		FVector2 Offset;
		b2Body* B2Body;

		BoxCollider2D()
			: Size({ 1.f, 1.f })
			, Offset({ 0.f, 0.f })
			, B2Body(nullptr)
		{

		}
	};
}