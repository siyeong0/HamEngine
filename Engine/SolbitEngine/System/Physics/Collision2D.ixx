module;

import Common;
import Math;
import ECS;
import SolbitEngine.Component;

export module SolbitEngine.Collision2D;

export namespace solbit
{
	struct Collision2D
	{
		Entity OtherEntity;
		FVector2 Impulse;
		FVector2 RelativeVelocity;
	};
}