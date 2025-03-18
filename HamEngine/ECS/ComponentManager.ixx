module;

import std.core;
import Common;
import STL.Array;
import STL.LinkedList;
import ECS.IComponent;
import ECS.Archetype;

export module ECS.ComponentManager;

namespace ham
{
	export class ComponentManager
	{
	public:
		struct CompTypeInfo
		{
			uint32 Id;
			size_t Size;
		};
	public:
		ComponentManager();
		~ComponentManager() = default;

		static bool Initialize();
		static void Finalize();

		template<typename CompType>
		static void Regist();

		static inline size_t NumComponents();
		static inline size_t SizeOfComponent(const CompTypeId& compTypeId);
		static inline Array<size_t> GetSizeArrOfArchetype(const Archetype& archetype);
		static inline size_t SizeOfArchetype(const Archetype& archetype);

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

	inline size_t ComponentManager::SizeOfComponent(const CompTypeId& compTypeId)
	{
		return sInstance->mCompTypeSizeMap[compTypeId];
	}

	inline Array<size_t> ComponentManager::GetSizeArrOfArchetype(const Archetype& archetype)
	{
		const Array<CompTypeId>& compTypes = archetype.GetCompTypes();

		Array<size_t> sizeArr(archetype.GetNumCompTypes());
		for (size_t i = 0; i < compTypes.Capacity(); ++i)
		{
			const CompTypeId& compTypeId = compTypes.Get(i);
			sizeArr[i] = sInstance->mCompTypeSizeMap[compTypeId];
		}

		return sizeArr;
	}

	inline size_t ComponentManager::SizeOfArchetype(const Archetype& archetype)
	{
		const Array<CompTypeId>& compTypes = archetype.GetCompTypes();

		size_t totalSize = 0;
		for (size_t i = 0; i < compTypes.Capacity(); ++i)
		{
			const CompTypeId& compTypeId = compTypes.Get(i);
			totalSize += sInstance->mCompTypeSizeMap[compTypeId];
		}

		return totalSize;
	}
}