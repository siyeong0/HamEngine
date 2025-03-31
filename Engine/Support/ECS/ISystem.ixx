module;

import Common;
import Memory;
import ECS.Entity;
import ECS.Archetype;

export module ECS.ISystem;

export namespace solbit
{
	class ISystem
	{
	public:
		ISystem() {};
		virtual ~ISystem() = default;

		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	};
}
