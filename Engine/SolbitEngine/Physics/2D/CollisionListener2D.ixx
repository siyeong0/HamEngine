module;

#include <box2d\box2d.h>

import std.core;
import Common;
import Math;
import SolbitEngine.GameObject;
import SolbitEngine.Physics2D;
import SolbitEngine.Physics2D.Collision2D;

export module SolbitEngine.Physics2D.CollisionListener2D;

export namespace solbit
{
	class CollisionListener2D : public b2ContactListener
	{
	public:
		virtual void BeginContact(b2Contact* contact) override
		{
			
		}

		virtual void EndContact(b2Contact* contact) override
		{
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();
			GameObject* gameObjectA = Physics2D::GetInstance()->GetGameObject(bodyA);
			GameObject* gameObjectB = Physics2D::GetInstance()->GetGameObject(bodyB);

			FVector2 velocityA = FVector2{ bodyA->GetLinearVelocity().x, bodyA->GetLinearVelocity().y };
			FVector2 velocityB = FVector2{ bodyB->GetLinearVelocity().x, bodyB->GetLinearVelocity().y };

			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			FVector2 contactNormal = FVector2{ worldManifold.normal.x, worldManifold.normal.y };

			FVector2 contactPoints[MAX_CONTACT_POINTS];
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				contactPoints[i] = FVector2{ worldManifold.points[i].x, worldManifold.points[i].y };
			};

			Collision2D collisionA;
			collisionA.GameObject = gameObjectB;
			collisionA.RelativeVelocity = velocityB - velocityA;
			collisionA.ContactNormal = contactNormal;
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				collisionA.ContactPoints[i] = contactPoints[i];
			}

			Collision2D collisionB;
			collisionB.GameObject = gameObjectA;
			collisionB.RelativeVelocity = velocityA - velocityB;
			collisionB.ContactNormal = contactNormal;
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				collisionB.ContactPoints[i] = contactPoints[i];
			}

			gameObjectA->OnCollisionExit(collisionA);
			gameObjectB->OnCollisionExit(collisionB);
		}

		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
		{
			
		}

		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
		{
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();
			GameObject* gameObjectA = Physics2D::GetInstance()->GetGameObject(bodyA);
			GameObject* gameObjectB = Physics2D::GetInstance()->GetGameObject(bodyB);

			FVector2 velocityA = FVector2{ bodyA->GetLinearVelocity().x, bodyA->GetLinearVelocity().y };
			FVector2 velocityB = FVector2{ bodyB->GetLinearVelocity().x, bodyB->GetLinearVelocity().y };

			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			FVector2 contactNormal = FVector2{ worldManifold.normal.x, worldManifold.normal.y };

			FVector2 contactPoints[MAX_CONTACT_POINTS];
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				contactPoints[i] = FVector2{ worldManifold.points[i].x, worldManifold.points[i].y };
			}
			
			FLOAT contactImpulse[MAX_CONTACT_POINTS];
			std::memcpy(contactImpulse, impulse->normalImpulses, sizeof(float) * MAX_CONTACT_POINTS);
			FLOAT contactTangentImpulse[MAX_CONTACT_POINTS];
			std::memcpy(contactTangentImpulse, impulse->tangentImpulses, sizeof(float) * MAX_CONTACT_POINTS);

			Collision2D collisionA;
			collisionA.GameObject = gameObjectB;
			collisionA.RelativeVelocity = velocityB - velocityA;
			collisionA.Impulse = contactImpulse[0];
			collisionA.TangentImpulse = contactTangentImpulse[0];
			collisionA.ContactNormal = contactNormal;
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				collisionA.ContactPoints[i] = contactPoints[i];
			}

			Collision2D collisionB;
			collisionB.GameObject = gameObjectA;
			collisionB.RelativeVelocity = velocityA - velocityB;
			collisionB.Impulse = contactImpulse[0];
			collisionB.TangentImpulse = contactTangentImpulse[0];
			collisionB.ContactNormal = contactNormal;
			for (int i = 0; i < MAX_CONTACT_POINTS; ++i)
			{
				collisionB.ContactPoints[i] = contactPoints[i];
			}

			gameObjectA->OnCollisionEnter(collisionA);
			gameObjectB->OnCollisionEnter(collisionB);
		}
	};
}