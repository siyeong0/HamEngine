module;

#include <box2d\box2d.h>

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

		void AddBody(GameObject& gameObject);
		void RemoveBody(GameObject& gameObject);
		void ApplyToB2Body(const GameObject& gameObject);
		void ApplyToSBBody(const GameObject& gameObject);

		void RegistContactListener(b2ContactListener* contactListener);
		inline GameObject* GetGameObject(b2Body* body);

	private:
		Physics2D() = default;
		~Physics2D() = default;
		Physics2D(const Physics2D&) = delete;
		Physics2D(const Physics2D&&) = delete;

	private:
		static Physics2D* msInstance;

		HashMap<Entity, b2Body*, EntityHash> mB2BodyMap;
		HashMap<b2Body*, GameObject*> mGameObjectMap;

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

	void Physics2D::AddBody(GameObject& gameObject)
	{
		if (!gameObject.HasComponent<Transform2D>())
		{
			return;
		}

		b2BodyDef bodyDef;
		b2Body* b2Body = mWorld->CreateBody(&bodyDef);

		mB2BodyMap.insert({ gameObject.GetEntity(), b2Body });
		mGameObjectMap.insert({ b2Body, &gameObject });

		Transform2D& transform = gameObject.GetComponent<Transform2D>();
		b2Body->SetTransform(b2Vec2{ transform.Position.X, transform.Position.Y }, transform.Rotation);

		bool bHasRigidBody = gameObject.HasComponent<RigidBody2D>();
		if (bHasRigidBody)
		{
			RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
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
		}
		else
		{
			b2Body->SetType(b2_staticBody);
			b2Body->SetLinearDamping(0.0f);
			b2Body->SetAngularDamping(0.0f);
			b2Body->SetFixedRotation(true);
			b2Body->SetBullet(false);
			b2Body->SetEnabled(true);
		}

		bool bHasCollider = gameObject.HasComponent<BoxCollider2D>();
		if (bHasCollider)
		{
			BoxCollider2D& collider = gameObject.GetComponent<BoxCollider2D>();
			b2PolygonShape boxShape;
			boxShape.SetAsBox(collider.Size.X * transform.Scale.X * 0.5f, collider.Size.Y * transform.Scale.Y * 0.5f, b2Vec2{ collider.Offset.X, collider.Offset.Y }, 0.0f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			if (bHasRigidBody)
			{
				const PhysicalMaterial& pm = PhysicalMaterailManager::GetInstance()->Get(gameObject.GetComponent<RigidBody2D>().PhysicMaterialId);
				fixtureDef.density = pm.Density;
				fixtureDef.friction = pm.Friction;
				fixtureDef.restitution = pm.Bounciness;
				fixtureDef.isSensor = false;
			}
			else
			{
				fixtureDef.isSensor = true;
			}

			b2Body->CreateFixture(&fixtureDef);
		}

		if (bHasRigidBody)
		{
			RigidBody2D& rigidbody = gameObject.GetComponent<RigidBody2D>();
			if (rigidbody.UseAutoMass)
			{
				b2Body->ResetMassData();
				rigidbody.Mass = b2Body->GetMass();
			}
		}
	}

	void Physics2D::RemoveBody(GameObject& gameObject)
	{
		ASSERT(mB2BodyMap.find(gameObject.GetEntity()) != mB2BodyMap.end());
		b2Body* b2Body = mB2BodyMap[gameObject.GetEntity()];
		mWorld->DestroyBody(b2Body);
		mB2BodyMap.erase(gameObject.GetEntity());
		
	}

	void Physics2D::ApplyToB2Body(const GameObject& gameObject)
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
		b2Body* b2Body = mB2BodyMap[gameObject.GetEntity()];

		b2Body->SetTransform(b2Vec2{ transform.Position.X, transform.Position.Y }, -transform.Rotation);	// Box2d는 반시계방향
		b2Body->SetLinearVelocity(b2Vec2{ rigidbody.Velocity.X, rigidbody.Velocity.Y });
		b2Body->SetAngularVelocity(rigidbody.AngularVelocity);
		b2Body->SetLinearDamping(rigidbody.LinearDamping);
	}

	void Physics2D::ApplyToSBBody(const GameObject& gameObject)
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
		b2Body* b2Body = mB2BodyMap[gameObject.GetEntity()];

		transform.Position.X = b2Body->GetTransform().p.x;
		transform.Position.Y = b2Body->GetTransform().p.y;
		transform.Rotation = -b2Body->GetAngle();	// Box2d는 반시계방향

		rigidbody.Velocity = FVector2{ b2Body->GetLinearVelocity().x, b2Body->GetLinearVelocity().y };
		rigidbody.AngularVelocity = b2Body->GetAngularVelocity();
	}

	void Physics2D::RegistContactListener(b2ContactListener* contactListener)
	{
		mWorld->SetContactListener(contactListener);
	}

	inline GameObject* Physics2D::GetGameObject(b2Body* body)
	{
		return mGameObjectMap[body];
	}
}