module;

import std.core;
import Common;
import ECS.IComponent;

export module ECS.ComponentManager;

export namespace ham
{
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		ComponentManager(const ComponentManager&);
		ComponentManager(const ComponentManager&&);

		template<typename CompType>
		void Regist();

		size_t GetArchetypeTotalSize(const uint32 archetypeIdx);

	private:
		size_t mCompTypeCount;
		
	};

	ComponentManager::ComponentManager()
		: mCompTypeCount(0)
	{

	}

	ComponentManager::~ComponentManager()
	{

	}

	template<typename CompType>
	void ComponentManager::Regist()
	{
		CompType::Regist(mCompTypeCount++);
	}

	size_t ComponentManager::GetArchetypeTotalSize(const uint32 archetypeIdx)
	{
		return 0;
	}
}