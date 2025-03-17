module;

import ECS.Entity;
import ECS.IComponent;

export module ECS.EntityManager;

export namespace ham
{
	class EntityManager
	{
	public:
		static const Entity& CreateEntity();

		template <typename CompType>
		static IComponent& AddComponent(const Entity& entity);

		template <typename CompType>
		static void RemoveComponent(const Entity& entity);
	};
}