module;

import Common;
import ECS.Archetype;

export module ECS.ISystem;

export namespace ham
{
	class ISystem
	{
	public:
		ISystem(const Archetype& targetArchetype);
		virtual ~ISystem() = 0;

		virtual void Update(float dt) = 0;

	private:
		Archetype mTargetArchetype;
	};
}

namespace ham
{
	ISystem::ISystem(const Archetype& targetArchetype)
		: mTargetArchetype(targetArchetype)
	{

	}
}