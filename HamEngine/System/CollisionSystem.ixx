module;

import Common;
import Math;
import ECS;
import HamEngine.Component;

export module HamEngine.System.CollisionSystem;

export namespace ham
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

namespace ham
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

		Vec2 v1 = ((rigidBody1.Mass - rigidBody2.Mass) * rigidBody1.Velocity + (rigidBody1.Mass + rigidBody2.Mass) * rigidBody2.Velocity) / (rigidBody1.Mass + rigidBody2.Mass);
		Vec2 v2 = ((rigidBody1.Mass - rigidBody2.Mass) * rigidBody2.Velocity + (rigidBody1.Mass + rigidBody2.Mass) * rigidBody1.Velocity) / (rigidBody1.Mass + rigidBody2.Mass);

		if (rigidBody1.BodyType == EBodyType::Dynamic)
		{
			transform1.Position -= rigidBody1.Velocity.Normalize() * Vec2{ intersection.W, intersection.H };
			rigidBody1.Velocity = v1;
		}

		if (rigidBody2.BodyType == EBodyType::Dynamic)
		{
			transform2.Position -= rigidBody2.Velocity.Normalize() * Vec2 { intersection.W, intersection.H };
			rigidBody2.Velocity = v2;
		}
	}
}