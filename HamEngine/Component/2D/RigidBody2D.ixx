module;

import Common;
import Math;

export module HamEngine.Component.RigidBody2D;

export namespace ham
{
	extern constexpr FLOAT GRAVITY = 9.8f;
	struct RigidBody2D
	{
		enum class EBodyType;

		EBodyType BodyType;
		Vec2 Velocity;
		FLOAT Mass;
		FLOAT GravityScale;

		enum class EBodyType
		{
			Static,
			Dynamic,
		};

		RigidBody2D()
			: BodyType(EBodyType::Dynamic)
			, Velocity({ 0.f, 0.f })
			, Mass(1.f)
			, GravityScale(1.f)
		{

		}
	};
}