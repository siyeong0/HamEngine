module;

import Common;
import ECS;
import HamEngine.Component;

export module HamEngine.System.RigidbodyPhysicsSystem;

export namespace ham
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

namespace ham
{
	void RigidbodyPhysicsSystem::Execute(const ComponentPack& entityComponentPack)
	{
		// Unpack sprite components
		Transform2D& transform = entityComponentPack.GetComponent<Transform2D>();
		RigidBody2D& rigidBody = entityComponentPack.GetComponent<RigidBody2D>();

		rigidBody.Velocity += Vec2{ 0.f, -GRAVITY * rigidBody.GravityScale };
		transform.Position += rigidBody.Velocity * FIXED_DELTA_TIME;
	}
}