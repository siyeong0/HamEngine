module;

import Common;
import ECS.Entity;
import ECS.Archetype;
import ECS.ComponentPack;

export module ECS.ISystem;

export namespace ham
{
	class ISystem
	{
	public:
		ISystem() {};
		virtual ~ISystem() = default;

	};
}
