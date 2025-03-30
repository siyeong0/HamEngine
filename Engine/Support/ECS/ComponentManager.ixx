module;

import std.core;
import Common;
import Memory;
import SolbitSTL;
import ECS.Archetype;

export module ECS.ComponentManager;

export namespace solbit
{
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager() = default;

		static bool Initialize();
		static void Finalize();

		template<typename ComponentType>
		static void Regist(const String& name);

		static inline size_t GetSizeOfComponent(const ID& componentTypeId);
		static inline size_t GetSizeOfArchetype(const Archetype& archetype);
		static inline const Vector<Pair<ID, size_t>> GetSizeVectorOfArchetype(const Archetype& archetype);

	private:
		static ComponentManager* sInstance;
	private:
		uint32 mComponentTypeCount;
		std::unordered_map<uint32, size_t> mComponentTypeSizeMap;
	};
}

namespace solbit
{
	ComponentManager* ComponentManager::sInstance = nullptr;

	ComponentManager::ComponentManager()
		: mComponentTypeCount(0)
		, mComponentTypeSizeMap()
	{

	}

	bool ComponentManager::Initialize()
	{
		ASSERT(sInstance == nullptr);
		sInstance = Alloc<ComponentManager>();

		return true;
	}
	void ComponentManager::Finalize()
	{
		ASSERT(sInstance != nullptr);
		Free<ComponentManager>(sInstance);
	}

	template<typename ComponentType>
	void ComponentManager::Regist(const String& name)
	{
		RTTI<ComponentType>::Regist(name);
		sInstance->mComponentTypeSizeMap[RTTI<ComponentType>::GetId()] = sizeof(ComponentType);
		sInstance->mComponentTypeCount++;
	}

	inline size_t ComponentManager::GetSizeOfComponent(const ID& componentTypeId)
	{
		return sInstance->mComponentTypeSizeMap[componentTypeId];
	}

	inline size_t ComponentManager::GetSizeOfArchetype(const Archetype& archetype)
	{
		const HashSet<ID>& componentTypeIdSet = archetype.GetComponentTypeIdSet();

		size_t totalSize = 0;
		for (auto iter = componentTypeIdSet.begin(); iter != componentTypeIdSet.end(); ++iter)
		{
			totalSize += GetSizeOfComponent(*iter);
		}

		return totalSize;
	}

	inline const Vector<Pair<ID, size_t>> ComponentManager::GetSizeVectorOfArchetype(const Archetype& archetype)
	{
		const HashSet<ID>& componentTypeIdSet = archetype.GetComponentTypeIdSet();

		Vector<Pair<ID, size_t>> sizeVector;
		sizeVector.reserve(componentTypeIdSet.size());

		for (auto iter = componentTypeIdSet.begin(); iter != componentTypeIdSet.end(); ++iter)
		{
			sizeVector.push_back({ *iter, GetSizeOfComponent(*iter) });
		}

		return sizeVector;
	}
}