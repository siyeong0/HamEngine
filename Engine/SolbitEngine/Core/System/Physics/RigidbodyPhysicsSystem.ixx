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
	class RigidbodyPhysicsSystem : public ISystem
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;

	private:
		static constexpr FLOAT FIXED_DELTA_TIME = 1.f / 60.f;
	};
}

namespace solbit
{
	void RigidbodyPhysicsSystem::OnCreate()
	{
		//EntityManager::GetActive()->
	}

	void RigidbodyPhysicsSystem::OnUpdate()
	{

	}

	void RigidbodyPhysicsSystem::OnDestroy()
	{

	}
}