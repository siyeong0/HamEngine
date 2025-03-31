module;

import Common;
import Math;

export module SolbitEngine.Component.RigidBody2D;

export namespace solbit
{
	extern constexpr FLOAT GRAVITY = 9.8f;
	enum class EBodyType
	{
		Static,
		Kinematic,
		Dynamic,
	};
	enum class ECollisionDetection
	{
		Discrete,
		Continuous,
	};

	struct RigidBody2D
	{
		EBodyType BodyType;
		FVector2 Velocity;
		FLOAT AngularVelocity;
		FLOAT Mass;
		FLOAT GravityScale;
		FLOAT LinearDamping;
		FLOAT AngularDamping;
		bool FreezeRotation;
		bool UseAutoMass;
		ECollisionDetection CollisionDetection;
		bool Simulated;
		ID PhysicMaterialId;

		RigidBody2D()
			: BodyType(EBodyType::Dynamic)
			, Velocity({ 0.f, 0.f })
			, AngularVelocity(0.f)
			, Mass(1.f)
			, GravityScale(1.f)
			, LinearDamping(0.5f)
			, AngularDamping(0.5f)
			, FreezeRotation(false)
			, UseAutoMass(true)
			, PhysicMaterialId(DEFAULT_ID)
		{

		}
	};
}