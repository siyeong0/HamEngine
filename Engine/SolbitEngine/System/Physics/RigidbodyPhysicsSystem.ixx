module;

import Common;
import Memory;
import ECS;
import SolbitEngine.Resource;
import SolbitEngine.Component;
import SolbitEngine.Physics2D;

export module SolbitEngine.System.RigidbodyPhysicsSystem;

export namespace solbit
{
	class RigidbodyPhysicsSystem
	{
	public:
		static constexpr FLOAT FIXED_DELTA_TIME = 1.f / 60.f;

		RigidbodyPhysicsSystem() = default;
		virtual ~RigidbodyPhysicsSystem() = default;

		void Execute(const Entity entity, Transform2D* transform, RigidBody2D* rigidbodyOrNull, BoxCollider2D* colliderOrNull);
	};
}

namespace solbit
{
	void RigidbodyPhysicsSystem::Execute(const Entity entity, Transform2D* transform, RigidBody2D* rigidbodyOrNull, BoxCollider2D* colliderOrNull)
	{
		Physics2D::GetInstance()->ApplyToSBBody(entity, transform, rigidbodyOrNull, colliderOrNull);
		Physics2D::GetInstance()->Update(FIXED_DELTA_TIME);
		Physics2D::GetInstance()->ApplyToSBBody(entity, transform, rigidbodyOrNull, colliderOrNull);
	}
}