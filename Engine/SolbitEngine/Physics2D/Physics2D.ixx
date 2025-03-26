module;

#include <box2d\box2d.h>
#include <box2d\b2_body.h>

import Common;
import Math;
import ECS;
import SolbitSTL;
import SolbitEngine.GameObject;
import SolbitEngine.Component;
import SolbitEngine.Resource;

export module SolbitEngine.Physics2D;

export namespace solbit
{
	class Physics2D
	{
	public:
		static Physics2D* GetInstance();
		static bool Initialize(const FVector2& gravity = { 0.f, -9.8f }, uint32 velIterations = 6, uint32 posIterations = 2);
		static void Finalize();

		void Update(FLOAT dt);

		void AddBody(const Entity entity, const Transform2D* transform, const RigidBody2D* rigidbodyOrNull, const BoxCollider2D* colliderOrNull);
		void ApplyToB2Body(const Entity entity, const Transform2D* transform, const RigidBody2D* rigidbodyOrNull, const BoxCollider2D* colliderOrNull);
		void ApplyToSBBody(const Entity entity, Transform2D* transform, RigidBody2D* rigidbodyOrNull, BoxCollider2D* colliderOrNull);

	private:
		Physics2D() = default;
		~Physics2D() = default;
		Physics2D(const Physics2D&) = delete;
		Physics2D(const Physics2D&&) = delete;

	private:
		static Physics2D* msInstance;

		HashMap<Entity, b2Body*, EntityHash> mB2BodyMap;

		b2World* mWorld;
		uint32 mVelocityIterations;
		uint32 mPositionIterations;
		FVector2 mGravity;
	};
}

namespace solbit
{
	Physics2D* Physics2D::msInstance = nullptr;

	Physics2D* Physics2D::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool Physics2D::Initialize(const FVector2& gravity, uint32 velIterations, uint32 posIterations)
	{
		ASSERT(msInstance == nullptr);
		msInstance = new Physics2D;

		b2Vec2 b2Gravity(gravity.X, gravity.Y);
		msInstance->mWorld = new b2World(b2Gravity);

		msInstance->mVelocityIterations = velIterations;
		msInstance->mPositionIterations = posIterations;

		return msInstance != nullptr;
	}

	void Physics2D::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	void Physics2D::Update(FLOAT dt)
	{
		mWorld->Step(dt, mVelocityIterations, mPositionIterations);
	}

	void Physics2D::AddBody(const Entity entity, const Transform2D* transform, const RigidBody2D* rigidbodyOrNull, const BoxCollider2D* colliderOrNull)
	{
		b2BodyDef bodyDef;
		b2Body* b2Body = mWorld->CreateBody(&bodyDef);

		mB2BodyMap.insert({ entity, b2Body });
		ASSERT(mB2BodyMap.find(entity) != mB2BodyMap.end());

		ASSERT(transform != nullptr);
		b2Body->SetTransform(b2Vec2{ transform->Position.X, transform->Position.Y }, transform->Rotation);

		if (rigidbodyOrNull == nullptr)
		{
			ASSERT(colliderOrNull != nullptr);

			b2Body->SetType(b2_staticBody);

			const BoxCollider2D& collider = *colliderOrNull;
			b2PolygonShape boxShape;
			boxShape.SetAsBox(collider.Size.X * transform->Scale.X, collider.Size.Y * transform->Scale.Y, b2Vec2{ collider.Offset.X, collider.Offset.Y }, 0.0f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.isSensor = true;

			b2Body->CreateFixture(&fixtureDef);
		}
		else
		{
			const RigidBody2D& rigidbody = *rigidbodyOrNull;
			switch (rigidbody.BodyType)
			{
			case EBodyType::Dynamic:
				b2Body->SetType(b2_dynamicBody);
				break;
			case EBodyType::Kinematic:
				b2Body->SetType(b2_kinematicBody);
				break;
			case EBodyType::Static:
				b2Body->SetType(b2_staticBody);
				break;
			default:
				ASSERT(false);
			}
			b2Body->SetLinearVelocity(b2Vec2{ rigidbody.Velocity.X, rigidbody.Velocity.Y });
			b2Body->SetAngularVelocity(rigidbody.AngularVelocity);
			b2Body->SetLinearDamping(rigidbody.LinearDamping);
			b2Body->SetAngularDamping(rigidbody.AngularDamping);
			b2Body->SetFixedRotation(rigidbody.FreezeRotation);
			b2Body->SetBullet(rigidbody.CollisionDetection == ECollisionDetection::Continuous);
			b2Body->SetEnabled(true);
			b2Body->SetGravityScale(rigidbody.GravityScale);

			if (colliderOrNull != nullptr)
			{
				const BoxCollider2D& collider = *colliderOrNull;
				const PhysicalMaterial& pm = PhysicalMaterailManager::GetInstance()->Get(rigidbody.PhysicMaterialId);
				b2PolygonShape boxShape;
				boxShape.SetAsBox(collider.Size.X * transform->Scale.X * 0.5f, collider.Size.Y * transform->Scale.Y * 0.5f, b2Vec2{ collider.Offset.X, collider.Offset.Y }, 0.0f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = pm.Density;
				fixtureDef.friction = pm.Friction;
				fixtureDef.restitution = pm.Bounciness;
				fixtureDef.isSensor = false;

				b2Body->CreateFixture(&fixtureDef);
			}

			if (rigidbody.UseAutoMass)
			{
				b2Body->ResetMassData();
				// Consta cast for update mass
				const_cast<RigidBody2D&>(rigidbody).Mass = b2Body->GetMass();
			}
		}
	}

	void Physics2D::ApplyToB2Body(const Entity entity, const Transform2D* transform, const RigidBody2D* rigidbodyOrNull, const BoxCollider2D* colliderOrNull)
	{
		ASSERT(mB2BodyMap.find(entity) != mB2BodyMap.end());
		b2Body* b2Body = mB2BodyMap[entity];

		ASSERT(transform != nullptr);
		b2Body->SetTransform(b2Vec2{ transform->Position.X, transform->Position.Y }, transform->Rotation);
		b2Body->SetLinearVelocity(b2Vec2{ rigidbodyOrNull->Velocity.X, rigidbodyOrNull->Velocity.Y });
		b2Body->SetAngularVelocity(rigidbodyOrNull->AngularVelocity);
		b2Body->SetLinearDamping(rigidbodyOrNull->LinearDamping);
	}

	void Physics2D::ApplyToSBBody(const Entity entity, Transform2D* transform, RigidBody2D* rigidbodyOrNull, BoxCollider2D* colliderOrNull)
	{
		ASSERT(mB2BodyMap.find(entity) != mB2BodyMap.end());
		ASSERT(transform != nullptr);

		b2Body* b2Body = mB2BodyMap[entity];

		transform->Position.X = b2Body->GetTransform().p.x;
		transform->Position.Y = b2Body->GetTransform().p.y;
		transform->Rotation = b2Body->GetAngle();

		if (rigidbodyOrNull != nullptr)
		{
			rigidbodyOrNull->Velocity = FVector2{ b2Body->GetLinearVelocity().x, b2Body->GetLinearVelocity().y };
			rigidbodyOrNull->AngularVelocity = b2Body->GetAngularVelocity();
			rigidbodyOrNull->Mass = b2Body->GetMass();
		}
	}
}