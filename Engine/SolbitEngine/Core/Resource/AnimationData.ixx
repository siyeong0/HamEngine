module;

import Common;
import Math;
import SolbitSTL;

export module SolbitEngine.Resource.AnimationData;

export namespace solbit
{
	struct AnimFrameData
	{
		ID SpriteId;
		FLOAT TimeOffset;
	};

	struct AnimationData
	{
		Vector<AnimFrameData> SpriteVec;
		FLOAT TimeLength;
	};
}