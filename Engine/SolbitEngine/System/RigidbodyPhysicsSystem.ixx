module;

import Common;
import Memory;
import ECS;
import SolbitEngine.Resource;
import SolbitEngine.Component;

export module SolbitEngine.System.RigidbodyPhysicsSystem;

export namespace solbit
{
	class RigidbodyPhysicsSystem
	{
	public:
		static constexpr FLOAT FIXED_DELTA_TIME = 1.f / 60.f;

		RigidbodyPhysicsSystem() = default;
		virtual ~RigidbodyPhysicsSystem() = default;

		void Execute(const ComponentPack& entityComponentPack);
	};
}

namespace solbit
{
	void RigidbodyPhysicsSystem::Execute(const ComponentPack& entityComponentPack)
	{
		// Unpack sprite components
		Transform2D& transform = entityComponentPack.GetComponent<Transform2D>();
		RigidBody2D& rigidBody = entityComponentPack.GetComponent<RigidBody2D>();

		if (rigidBody.BodyType == EBodyType::Dynamic)
		{
			rigidBody.Velocity += rigidBody.Acceleration * FIXED_DELTA_TIME;
			transform.Position += rigidBody.Velocity * FIXED_DELTA_TIME;
		}
		rigidBody.Acceleration = { 0.f, -GRAVITY * rigidBody.GravityScale };
	}
}