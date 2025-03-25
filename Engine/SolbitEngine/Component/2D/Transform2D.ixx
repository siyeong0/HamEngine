module;

import Common;
import Math;

export module SolbitEngine.Component.Transform2D;

export namespace solbit
{
	struct Transform2D
	{
		Vec2 Position;
		FLOAT Rotation;
		Vec2 Scale;

		Transform2D()
			: Position({ 0.f, 0.f })
			, Rotation(0.f)
			, Scale({ 1.f, 1.f }) 
		{

		}
	};
}
