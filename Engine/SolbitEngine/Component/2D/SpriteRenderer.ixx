module;

import Common;

export module SolbitEngine.Component.SpriteRenderer;

export namespace solbit
{
	struct SpriteRenderer
	{
		ID SpriteTexId;

		SpriteRenderer()
			: SpriteTexId(DEFAULT_ID)
		{

		}
	};
}