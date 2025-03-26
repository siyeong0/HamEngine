module;

import Common;
import Math;
import SolbitEngine.Component;

export module SolbitEngine.Physics2D.Collision2D;

export namespace solbit
{
	class GameObject;
	constexpr uint32 MAX_CONTACT_POINTS = 2;

	struct Collision2D
	{
		GameObject* GameObject;
		FLOAT Impulse;
		FLOAT TangentImpulse;
		FVector2 RelativeVelocity;
		FVector2 ContactNormal;
		FVector2 ContactPoints[MAX_CONTACT_POINTS];
	};
}