module;

#include <box2d\box2d.h>

import std.core;
import Common;
import Math;
import ECS;
import SolbitSTL;
import SolbitEngine.Physics2D.Collision2D;
import SolbitEngine.GameObject;
import SolbitEngine.Component;
import SolbitEngine.Resource;

export module SolbitEngine.PhysWorld2D;

namespace solbit
{
	struct b2BodyIdHash
	{
		std::size_t operator()(const b2BodyId& bodyId) const noexcept
		{

			return std::hash<uint64>()(*reinterpret_cast<const uint64*>(&bodyId));
		}
	};

	struct b2BodyIdEqual
	{
		bool operator()(const b2BodyId& lhs, const b2BodyId& rhs) const noexcept
		{
			return (lhs.index1 == rhs.index1) && (lhs.world0 == rhs.world0) && (lhs.generation == rhs.generation);
		}
	};

	export class PhysWorld2D
	{
	public:
		PhysWorld2D(const FVector2& gravity = {0.0f, -9.8f}, uint32 subStepCount = 4);
		~PhysWorld2D();

		void Update(FLOAT dt);

		void AddBody(GameObject& gameObject);
		void RemoveBody(GameObject& gameObject);
		void ApplyToB2Body(const GameObject& gameObject);
		void ApplyToSBBody(const GameObject& gameObject);

		bool DoContact(const GameObject& gameObject, const FVector2& normal = { 0.0f, 0.0f }) const;

	private:
		PhysWorld2D(const PhysWorld2D&) = delete;
		PhysWorld2D(const PhysWorld2D&&) = delete;

	private:
		HashMap<Entity, b2BodyId, EntityHash> mB2BodyMap;
		HashMap<b2BodyId, GameObject*, b2BodyIdHash, b2BodyIdEqual> mGameObjectMap;

		b2WorldId mWorld;
		uint32 mSubStepCount;
		FVector2 mGravity;
	};
}

namespace solbit
{
	PhysWorld2D::PhysWorld2D(const FVector2& gravity, uint32 subStepCount)
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { gravity.X, gravity.Y };
		mWorld = b2CreateWorld(&worldDef);

		mSubStepCount = subStepCount;
	}

	PhysWorld2D::~PhysWorld2D()
	{
		b2DestroyWorld(mWorld);
	}

	void PhysWorld2D::Update(FLOAT dt)
	{
		b2World_Step(mWorld, dt, mSubStepCount);

		b2ContactEvents contactEvents = b2World_GetContactEvents(mWorld);

		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;
 			GameObject* objA = mGameObjectMap[b2Shape_GetBody(endEvent->shapeIdA)];
			GameObject* objB = mGameObjectMap[b2Shape_GetBody(endEvent->shapeIdB)];

			objA->OnCollisionExit(objB);
			objB->OnCollisionExit(objA);
		}

		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;

			GameObject* objA = mGameObjectMap[b2Shape_GetBody(beginEvent->shapeIdA)];
			GameObject* objB = mGameObjectMap[b2Shape_GetBody(beginEvent->shapeIdB)];

			const b2Manifold& manifold = beginEvent->manifold;
			FVector2 normal = { manifold.normal.x, manifold.normal.y };

			const b2ManifoldPoint* maniflodPoints = manifold.points;
			FLOAT velocity = 0.0f;
			FLOAT impulse = 0.0f;
			FVector2 contactPoint = { 0.0f, 0.0f };
			for (int i = 0; i < manifold.pointCount; ++i)
			{
				velocity += maniflodPoints[i].normalVelocity;
				impulse += maniflodPoints[i].normalImpulse;
				contactPoint.X += maniflodPoints[i].point.x;
				contactPoint.Y += maniflodPoints[i].point.y;
			}
			FLOAT fn = static_cast<FLOAT>(manifold.pointCount);
			velocity /= fn;
			impulse /= fn;
			contactPoint /= fn;

			Collision2D collisionA;
			Collision2D collisionB;
			collisionA.GameObject = objB;
			collisionB.GameObject = objA;
			collisionA.ContactNormal = normal;
			collisionB.ContactNormal = -normal;
			collisionA.NormalVelocity = velocity;
			collisionB.NormalVelocity = velocity;
			collisionA.NormalImpulse = impulse;
			collisionB.NormalImpulse = impulse;
			collisionA.ContactPoint = contactPoint;
			collisionB.ContactPoint = contactPoint;

			objA->OnCollisionEnter(collisionA);
			objB->OnCollisionEnter(collisionB);
		}
	}

	void PhysWorld2D::AddBody(GameObject& gameObject)
	{
		if (!gameObject.HasComponent<Transform2D>())
		{
			return;
		}

		b2BodyDef bodyDef = b2DefaultBodyDef();
		b2ShapeDef shapeDef = b2DefaultShapeDef();

		Transform2D& transform = gameObject.GetComponent<Transform2D>();
		bodyDef.position = b2Vec2{ transform.Position.X, transform.Position.Y };
		bodyDef.rotation = b2Rot{ std::cosf(-transform.Rotation), std::sinf(-transform.Rotation) };

		bool bHasRigidBody = gameObject.HasComponent<RigidBody2D>();
		if (bHasRigidBody)
		{
			RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
			switch (rigidbody.BodyType)
			{
			case EBodyType::Dynamic:
				bodyDef.type = b2_dynamicBody;
				break;
			case EBodyType::Kinematic:
				bodyDef.type = b2_kinematicBody;
				break;
			case EBodyType::Static:
				bodyDef.type = b2_staticBody;
				break;
			default:
				ASSERT(false);
			}
			bodyDef.linearVelocity = b2Vec2{ rigidbody.Velocity.X, rigidbody.Velocity.Y };
			bodyDef.angularVelocity = rigidbody.AngularVelocity;
			bodyDef.linearDamping = rigidbody.LinearDamping;
			bodyDef.angularDamping = rigidbody.AngularDamping;
			bodyDef.fixedRotation = rigidbody.FreezeRotation;
			bodyDef.isBullet = rigidbody.CollisionDetection == ECollisionDetection::Continuous;
			bodyDef.isEnabled = true;
			bodyDef.gravityScale = rigidbody.GravityScale;

			const PhysicalMaterial& pm = PhysicalMaterailManager::GetInstance()->Get(gameObject.GetComponent<RigidBody2D>().PhysicMaterialId);
			shapeDef.density = pm.Density;
			shapeDef.friction = pm.Friction;
			shapeDef.restitution = pm.Bounciness;
			shapeDef.enableContactEvents = true;
			shapeDef.enableHitEvents = false;
			shapeDef.isSensor = false;
		}
		else
		{
			bodyDef.type = b2_staticBody;
			bodyDef.linearDamping = 0.0f;
			bodyDef.angularDamping = 0.0f;
			bodyDef.fixedRotation = true;
		}

		b2BodyId b2Body = b2CreateBody(mWorld, &bodyDef);
		if (gameObject.HasComponent<BoxCollider2D>())
		{
			BoxCollider2D& collider = gameObject.GetComponent<BoxCollider2D>();

			b2Polygon boxShape = b2MakeRoundedBox(collider.Size.X * transform.Scale.X * 0.5f, collider.Size.Y * transform.Scale.Y * 0.5f, collider.Rounded);;
			// TODO: b2MakeOffsetBox 버그있음. vertices가 모두 0임
			/*b2Polygon boxShape = b2MakeOffsetBox(
				collider.Size.X * transform.Scale.X * 0.5f, collider.Size.Y * transform.Scale.Y * 0.5f,
				b2Vec2{ collider.Offset.X, collider.Offset.Y }, b2Rot(0.0f));*/
			b2CreatePolygonShape(b2Body, &shapeDef, &boxShape);
		}
		else if (gameObject.HasComponent<CapsuleCollider2D>())
		{
			CapsuleCollider2D& collider = gameObject.GetComponent<CapsuleCollider2D>();

			b2Capsule capsuleShape;
			capsuleShape.center1 = b2Vec2{ 0.0f, collider.Height / 2.0f };
			capsuleShape.center2 = b2Vec2{ 0.0f, -collider.Height / 2.0f };
			capsuleShape.radius = collider.Radius;
			b2CreateCapsuleShape(b2Body, &shapeDef, &capsuleShape);
		}

		if (bHasRigidBody)
		{
			RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
			if (rigidbody.UseAutoMass)
			{
				rigidbody.Mass = b2Body_GetMass(b2Body);
			}
			else
			{
				ASSERT(rigidbody.Mass > 0.f);
				b2MassData massData = b2Body_GetMassData(b2Body);
				massData.mass = rigidbody.Mass;
				b2Body_SetMassData(b2Body, massData);
			}
		}

		mB2BodyMap.insert({ gameObject.GetEntity(), b2Body });
		mGameObjectMap.insert({ b2Body, &gameObject });
	}

	void PhysWorld2D::RemoveBody(GameObject& gameObject)
	{
		ASSERT(mB2BodyMap.find(gameObject.GetEntity()) != mB2BodyMap.end());
		b2BodyId b2Body = mB2BodyMap[gameObject.GetEntity()];
		b2DestroyBody(b2Body);
		mB2BodyMap.erase(gameObject.GetEntity());

	}

	void PhysWorld2D::ApplyToB2Body(const GameObject& gameObject)
	{
		if (!gameObject.HasComponent<RigidBody2D>())
		{
			return;
		}
		RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
		if (rigidbody.BodyType == EBodyType::Static)
		{
			return;
		}
		Transform2D& transform = gameObject.GetComponent<Transform2D>();
		ASSERT(mB2BodyMap.find(gameObject.GetEntity()) != mB2BodyMap.end());
		b2BodyId b2Body = mB2BodyMap[gameObject.GetEntity()];

		b2Body_SetTransform(b2Body, b2Vec2{ transform.Position.X, transform.Position.Y }, b2Rot{ std::cosf(-transform.Rotation), std::sinf(-transform.Rotation) }); // // Box2d는 반시계방향
		b2Body_SetLinearVelocity(b2Body, b2Vec2{ rigidbody.Velocity.X, rigidbody.Velocity.Y });
		b2Body_SetAngularVelocity(b2Body, rigidbody.AngularVelocity);
	}

	void PhysWorld2D::ApplyToSBBody(const GameObject& gameObject)
	{
		if (!gameObject.HasComponent<RigidBody2D>())
		{
			return;
		}
		RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
		if (rigidbody.BodyType == EBodyType::Static)
		{
			return;
		}
		Transform2D& transform = gameObject.GetComponent<Transform2D>();

		ASSERT(mB2BodyMap.find(gameObject.GetEntity()) != mB2BodyMap.end());
		b2BodyId b2Body = mB2BodyMap[gameObject.GetEntity()];

		b2Transform b2Transform = b2Body_GetTransform(b2Body);
		transform.Position.X = b2Transform.p.x;
		transform.Position.Y = b2Transform.p.y;
		transform.Rotation = -b2Rot_GetAngle(b2Transform.q);	// Box2d는 반시계방향

		b2Vec2 b2Velocity = b2Body_GetLinearVelocity(b2Body);
		rigidbody.Velocity = FVector2{ b2Velocity.x, b2Velocity.y };
		rigidbody.AngularVelocity = b2Body_GetAngularVelocity(b2Body);
	}

	bool PhysWorld2D::DoContact(const GameObject& gameObject, const FVector2& normal) const
	{
		ASSERT(mB2BodyMap.find(gameObject.GetEntity()) != mB2BodyMap.end());
		const b2BodyId b2Body = mB2BodyMap.at(gameObject.GetEntity());

		b2ContactData contactData[10];
		int contactCount = b2Body_GetContactData(b2Body, contactData, 10);

		if (normal.X + normal.Y < 0.1f)
		{
			return contactCount > 0;
		}

		ASSERT(normal.Length() > 0.99f && normal.Length() < 1.01f);
		for (int i = 0; i < contactCount; ++i)
		{
			b2ContactData* data = contactData + i;
			FVector2 contactNormal = { data->manifold.normal.x, data->manifold.normal.y };
			float cosDist = normal.Dot(contactNormal);
			if (cosDist > 0.9f)
			{
				return true;
			}
		}

		return false;
	}
}