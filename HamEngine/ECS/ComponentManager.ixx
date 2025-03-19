module;

import std.core;
import Common;
import HamSTL.Utility;
import HamSTL.Vector;
import HamSTL.HashSet;
import ECS.IComponent;
import ECS.Archetype;

export module ECS.ComponentManager;

export namespace ham
{
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager() = default;

		static bool Initialize();
		static void Finalize();

		template<typename CompType>
		static void Regist();

		static inline size_t NumComponents();
		static inline size_t GetSizeOfComponent(const CompTypeId& compTypeId);
		static inline size_t GetSizeOfArchetype(const Archetype& archetype);
		static inline const Vector<Pair<CompTypeId, size_t>> GetSizeVectorOfArchetype(const Archetype& archetype);

	private:
		static ComponentManager* sInstance;
	private:
		uint32 mCompTypeCount;
		std::unordered_map<uint32, size_t> mCompTypeSizeMap;
	};
}

namespace ham
{
	ComponentManager* ComponentManager::sInstance = nullptr;

	ComponentManager::ComponentManager()
		: mCompTypeCount(0)
		, mCompTypeSizeMap()
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

	template<typename CompType>
	void ComponentManager::Regist()
	{
		TypeId<CompType>::Regist();
		sInstance->mCompTypeSizeMap[TypeId<CompType>::GetId()] = TypeId<CompType>::GetSize();
		sInstance->mCompTypeCount++;
	}

	inline size_t ComponentManager::NumComponents()
	{
		return sInstance->mCompTypeCount;
	}

	inline size_t ComponentManager::GetSizeOfComponent(const CompTypeId& compTypeId)
	{
		return sInstance->mCompTypeSizeMap[compTypeId];
	}

	inline size_t ComponentManager::GetSizeOfArchetype(const Archetype& archetype)
	{
		const HashSet<CompTypeId>& componentTypeIdSet = archetype.GetComponentTypeIdSet();

		size_t totalSize = 0;
		for (auto iter = componentTypeIdSet.begin(); iter != componentTypeIdSet.end(); ++iter)
		{
			totalSize += GetSizeOfComponent(*iter);
		}

		return totalSize;
	}

	inline const Vector<Pair<CompTypeId, size_t>> ComponentManager::GetSizeVectorOfArchetype(const Archetype& archetype)
	{
		const HashSet<CompTypeId>& componentTypeIdSet = archetype.GetComponentTypeIdSet();

		Vector<Pair<CompTypeId, size_t>> sizeVector;
		sizeVector.reserve(componentTypeIdSet.size());

		for (auto iter = componentTypeIdSet.begin(); iter != componentTypeIdSet.end(); ++iter)
		{
			sizeVector.push_back({ *iter, GetSizeOfComponent(*iter) });
		}

		return sizeVector;
	}
}