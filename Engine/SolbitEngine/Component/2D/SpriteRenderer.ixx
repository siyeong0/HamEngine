module;

import Common;

export module SolbitEngine.Component.SpriteRenderer;

export namespace solbit
{
	struct SpriteRenderer
	{
		Id SpriteTexId;

		SpriteRenderer()
			: SpriteTexId(INVALID_ID)
		{

		}
	};
}