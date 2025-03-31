module;

import Common;
import Math;
import SolbitEngine.Component;

export module SolbitEngine.Physics2D.Collision2D;

export namespace solbit
{
	class GameObject;

	struct Collision2D
	{
		GameObject* GameObject;
		FLOAT NormalImpulse;
		FLOAT NormalVelocity;
		FVector2 ContactNormal;
		FVector2 ContactPoint;
	};
}