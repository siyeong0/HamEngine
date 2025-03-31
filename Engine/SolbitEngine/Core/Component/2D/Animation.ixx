module;

import Common;
import Math;
import ECS;

export module SolbitEngine.Component.Animation;

export namespace solbit
{
	struct Animation
	{
		ID AnimId;
		uint32 AnimIdx;
		FLOAT Time;

		Animation()
			: AnimId(0)
			, AnimIdx(0)
			, Time(0.0f)
		{

		}
	};
}