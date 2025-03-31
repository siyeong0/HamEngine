module;

import Common;
import Math;
import SolbitEngine.Resource.Sprite;

export module SolbitEngine.Component.CapsuleCollider2D;

export namespace solbit
{
	struct CapsuleCollider2D
	{
		FLOAT Radius;
		FLOAT Height;

		CapsuleCollider2D()
			: Radius(0.5f)
			, Height(1.0f)
		{

		}
	};
}