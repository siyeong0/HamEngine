module;

import Common;
import Memory;
import Math;
import ECS;
import SolbitEngine.Component;

export module SolbitEngine.System.CollisionSystem;

export namespace solbit
{
	class CollisionSystem
	{
	public:
		static constexpr FLOAT FIXED_DELTA_TIME = 1.f / 60.f;

		CollisionSystem() = default;
		virtual ~CollisionSystem() = default;

		void Execute(const ComponentPack& object1, const ComponentPack& object2);
	};
}

namespace solbit
{
	void CollisionSystem::Execute(const ComponentPack& object1, const ComponentPack& object2)
	{
		// Unpack sprite components
		Transform2D& transform1 = object1.GetComponent<Transform2D>();
		RigidBody2D& rigidBody1 = object1.GetComponent<RigidBody2D>();
		BoxCollider2D& boxCollider1 = object1.GetComponent<BoxCollider2D>();

		Transform2D& transform2 = object2.GetComponent<Transform2D>();
		RigidBody2D& rigidBody2 = object2.GetComponent<RigidBody2D>();
		BoxCollider2D& boxCollider2 = object2.GetComponent<BoxCollider2D>();

		Rect rect1 =
		{
			transform1.Position.X + boxCollider1.Offset.X - boxCollider1.Size.X / 2.f * transform1.Scale.X,
			transform1.Position.Y + boxCollider1.Offset.Y - boxCollider1.Size.Y / 2.f * transform1.Scale.Y,
			boxCollider1.Size.X * transform1.Scale.X,
			boxCollider1.Size.Y * transform1.Scale.Y,
		};
		Rect rect2 =
		{
			transform2.Position.X + boxCollider2.Offset.X - boxCollider2.Size.X / 2.f * transform2.Scale.X,
			transform2.Position.Y + boxCollider2.Offset.Y - boxCollider2.Size.Y / 2.f * transform2.Scale.Y,
			boxCollider2.Size.X * transform2.Scale.X,
			boxCollider2.Size.Y * transform2.Scale.Y,
		};

		Rect intersection = rect1.Intersect(rect2);
		ASSERT(rect1.DoIntersect(rect2) == intersection.IsValid());
		if (!intersection.IsValid())
			return;

		const FLOAT epsilon = 0.5f;
		const FLOAT friction = 0.95f;
		if (rigidBody1.BodyType == EBodyType::Dynamic && rigidBody2.BodyType == EBodyType::Dynamic)
		{
			const Vec2 velRel = rigidBody1.Velocity - rigidBody2.Velocity;	// Relative velocity
			const Vec2 normal = -(transform2.Position - transform1.Position).Normalize();
			const Vec2 impulse = normal * -(1.0f + epsilon) * velRel.Dot(normal) / ((1.f / rigidBody1.Mass) + (1.f / rigidBody2.Mass));

			transform1.Position -= rigidBody1.Velocity.Normalize() * std::fminf(intersection.W, intersection.H) * 0.5f;
			transform2.Position -= rigidBody2.Velocity.Normalize() * std::fminf(intersection.W, intersection.H) * 0.5f;

			rigidBody1.Velocity += impulse / rigidBody1.Mass;
			rigidBody2.Velocity -= impulse / rigidBody2.Mass;

			return;
		}
		else
		{
			if (rigidBody1.BodyType == EBodyType::Dynamic) // && (rigidBody2.BodyType == EBodyType::Static)
			{
				transform1.Position -= rigidBody1.Velocity.Normalize() * std::fminf(intersection.W, intersection.H);
				if (intersection.W > intersection.H)
				{
					rigidBody1.Velocity.X *= friction;
					rigidBody1.Velocity.Y *= -1.f * epsilon;
				}
				else
				{
					rigidBody1.Velocity.X *= -1.f * epsilon;
					rigidBody1.Velocity.Y *= friction;
				}
			}
			else //(rigidBody2.BodyType == EBodyType::Dynamic) // && (rigidBody1.BodyType == EBodyType::Static)
			{
				transform2.Position -= rigidBody2.Velocity.Normalize() * std::fminf(intersection.W, intersection.H);
				if (intersection.W > intersection.H)
				{
					rigidBody2.Velocity.X *= friction;
					rigidBody2.Velocity.Y *= -1.f * epsilon;
				}
				else
				{
					rigidBody2.Velocity.X *= -1.f * epsilon;
					rigidBody2.Velocity.Y *= friction;
				}
			}

			return;
		}
	}
}