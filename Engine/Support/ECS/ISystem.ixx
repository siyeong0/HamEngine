module;

import Common;
import Memory;
import ECS.Entity;
import ECS.Archetype;
import ECS.ComponentPack;

export module ECS.ISystem;

export namespace solbit
{
	class ISystem
	{
	public:
		ISystem() {};
		virtual ~ISystem() = default;

	};
}
