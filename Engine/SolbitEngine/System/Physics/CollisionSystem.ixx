module;

import Common;
import Memory;
import Math;
import ECS;
import SolbitEngine.Component;
import SolbitEngine.Resource;

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

	}
}