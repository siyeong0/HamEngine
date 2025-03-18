module;

import std.core;
import Common;
import STL.Array;
import STL.LinkedList;
import ECS.Entity;
import ECS.IComponent;
import ECS.ComponentManager;
import ECS.Archetype;
import ECS.ArchetypeChunk;

export module ECS.EntityManager;

namespace ham
{
	export class EntityManager
	{
	public:
		static bool Initialze();
		static void Finalize();

		static const Entity& CreateEntity();

		template <typename CompType>
		static CompType& AddComponent(const Entity& entity);

		template <typename CompType>
		static void RemoveComponent(const Entity& entity);

	public:
		EntityManager() = default;
		~EntityManager() = default;

	private:
		static EntityManager* sInstance;

	private:
		// TODO: STL.Vector로 변경
		std::vector<Entity> mEntities;
		std::unordered_map<Entity, Archetype, EntityHash> mEntitiyToArchetypeMap;
		std::unordered_map<Archetype, std::list<ArchetypeChunk>, ArchetypeHash> mArchetypeChunkMap;
	};
}

namespace ham
{
	EntityManager* EntityManager::sInstance;

	bool EntityManager::Initialze()
	{
		ASSERT(sInstance == nullptr);
		sInstance = Alloc<EntityManager>();

		sInstance->mEntities.reserve(4096); // TODO: realloc이 일어나면 안됨; FixedVector로 변경

		return true;
	}

	void EntityManager::Finalize()
	{
		ASSERT(sInstance != nullptr);
		Free<EntityManager>(sInstance);
	}

	const Entity& EntityManager::CreateEntity()
	{
		// TODO: 아이디 생성
		static uint32 entityId = 0;
		sInstance->mEntities.push_back(Entity(entityId++));

		return sInstance->mEntities[sInstance->mEntities.size() - 1];
	}

	template <typename CompType>
	CompType& EntityManager::AddComponent(const Entity& entity)
	{
		// TODO: 컴포넌트를 가지고 있지 않은지 확인
		if (sInstance->mEntitiyToArchetypeMap.find(entity) == sInstance->mEntitiyToArchetypeMap.end())	// Entity with no Components
		{
			Array<CompTypeId> comTypes(1);
			comTypes[0] = TypeId<CompType>::GetId();

			Archetype archetype(comTypes);
			sInstance->mEntitiyToArchetypeMap.insert({ entity, archetype });

			if (sInstance->mArchetypeChunkMap.find(archetype) != sInstance->mArchetypeChunkMap.end())	// Already exists
			{
				std::list<ArchetypeChunk>& archetypeChunks = sInstance->mArchetypeChunkMap[archetype];
				ASSERT(archetypeChunks.size() > 0);

				auto availableIter = archetypeChunks.begin();
				while (availableIter->IsFull())
				{
					++availableIter;
				}
				availableIter->Add(entity);

				return availableIter->GetComponent<CompType>(entity);
			}
			else // New Archetype
			{
				std::list<ArchetypeChunk> chunkList;
				chunkList.push_back(ArchetypeChunk(archetype));
				sInstance->mArchetypeChunkMap.insert({ archetype, chunkList });

				std::list<ArchetypeChunk>& archetypeChunks = sInstance->mArchetypeChunkMap[archetype];
				archetypeChunks.front().Add(entity);

				return archetypeChunks.front().GetComponent<CompType>(entity);
			}
		}
		else
		{
			const Archetype& oldArchetype = sInstance->mEntitiyToArchetypeMap[entity];
			// Build new Archetype
			const Array<CompTypeId>& oldCompTypes = oldArchetype.GetCompTypes();
			Array<CompTypeId> newCompTypes(oldCompTypes.Capacity() + 1);
			for (size_t i = 0; i < oldCompTypes.Capacity(); ++i)
			{
				newCompTypes[i] = oldCompTypes.Get(i);
			}
			newCompTypes[oldCompTypes.Capacity()] = TypeId<CompType>::GetId();
			const Archetype newArchetype(newCompTypes);

			// Find target old/new ArchetypeChunk
			std::list<ArchetypeChunk>& oldChunkList = sInstance->mArchetypeChunkMap[oldArchetype];
			auto oldChunkIter = oldChunkList.begin();
			for (; oldChunkIter != oldChunkList.end(); ++oldChunkIter)
			{
				if (oldChunkIter->Has(entity))
					break;
			}
			ASSERT(oldChunkIter != oldChunkList.end());
			ArchetypeChunk* oldChunk = &(*oldChunkIter);

			ArchetypeChunk* newChunk = nullptr;
			if (sInstance->mArchetypeChunkMap.find(newArchetype) != sInstance->mArchetypeChunkMap.end())	// Already exists
			{
				std::list<ArchetypeChunk>& newChunkList = sInstance->mArchetypeChunkMap[newArchetype];
				auto newChunkIter = newChunkList.begin();
				for (; newChunkIter != newChunkList.end(); ++newChunkIter)
				{
					if (!newChunkIter->IsFull())
						break;
				}
				if (newChunkIter == newChunkList.end())	// 꽊 찼으므로 새로운 청크 할당
				{
					newChunkList.push_back(ArchetypeChunk(newArchetype));
				}
				newChunk = &(*newChunkIter);
			}
			else // New Archetype
			{
				std::list<ArchetypeChunk> chunkList;
				chunkList.push_back(ArchetypeChunk(newArchetype));
				sInstance->mArchetypeChunkMap.insert({ newArchetype, chunkList });

				std::list<ArchetypeChunk>& newChunkList = sInstance->mArchetypeChunkMap[newArchetype];
				newChunk = &newChunkList.front();
			}

			// Add entity to new Chunk
			newChunk->Add(entity);

			ASSERT(newChunk->Has(entity));
			// Copy Components
			for (size_t i = 0; i < oldCompTypes.Capacity(); ++i)
			{
				const CompTypeId& compTypeId = oldCompTypes.Get(i);
				void* srcComp = reinterpret_cast<void*>(&(oldChunk->GetComponent(entity, compTypeId)));
				void* dstComp = reinterpret_cast<void*>(&(newChunk->GetComponent(entity, compTypeId)));
				std::memcpy(&dstComp, &dstComp, ComponentManager::SizeOfComponent(compTypeId));
			}

			// Remove old Chunk data
			oldChunk->Remove(entity);

			// Update EntityToArchetypeMap
			sInstance->mEntitiyToArchetypeMap.erase(entity);
			sInstance->mEntitiyToArchetypeMap.insert({ entity, newArchetype });

			return newChunk->GetComponent<CompType>(entity);
		}
	}

	template <typename CompType>
	void EntityManager::RemoveComponent(const Entity& entity)
	{
		// TODO: 컴포넌트를 가지고 있는지 확인
		ASSERT(sInstance->mEntitiyToArchetypeMap.find(entity) != sInstance->mEntitiyToArchetypeMap.end());

		const Archetype& oldArchetype = sInstance->mEntitiyToArchetypeMap[entity];
		// Build new Archetype
		const Array<CompTypeId>& oldCompTypes = oldArchetype.GetCompTypes();
		Array<CompTypeId> newCompTypes(oldCompTypes.Capacity() - 1);
		const CompTypeId& rmCompType = TypeId<CompType>::GetId();
		size_t newIdx = 0;
		for (size_t i = 0; i < oldCompTypes.Capacity(); ++i)
		{
			if (oldCompTypes.Get(i) != rmCompType)
				newCompTypes[newIdx] = oldCompTypes.Get(i);
		}
		const Archetype newArchetype(newCompTypes);

		// Find target old/new ArchetypeChunk
		std::list<ArchetypeChunk>& oldChunkList = sInstance->mArchetypeChunkMap[oldArchetype];
		auto oldChunkIter = oldChunkList.begin();
		for (; oldChunkIter != oldChunkList.end(); ++oldChunkIter)
		{
			if (oldChunkIter->Has(entity))
				break;
		}
		ASSERT(oldChunkIter != oldChunkList.end());
		ArchetypeChunk* oldChunk = &(*oldChunkIter);

		ASSERT(sInstance->mArchetypeChunkMap.find(newArchetype) != sInstance->mArchetypeChunkMap.end());	// Already exists
		std::list<ArchetypeChunk>& newChunkList = sInstance->mArchetypeChunkMap[newArchetype];
		auto newChunkIter = newChunkList.begin();
		for (; newChunkIter != newChunkList.end(); ++newChunkIter)
		{
			if (!newChunkIter->IsFull())
				break;
		}
		if (newChunkIter == newChunkList.end())	// 꽊 찼으므로 새로운 청크 할당
		{
			newChunkList.push_back(ArchetypeChunk(newArchetype));
		}
		ArchetypeChunk* newChunk = &(*newChunkIter);

		// Add entity to new Chunk
		newChunk->Add(entity);

		ASSERT(newChunk->Has(entity));
		// Copy Components
		newIdx = 0;
		for (size_t i = 0; i < newCompTypes.Capacity(); ++i)
		{
			const CompTypeId& compTypeId = newCompTypes.Get(i);
			void* srcComp = reinterpret_cast<void*>(&(oldChunk->GetComponent(entity, compTypeId)));
			void* dstComp = reinterpret_cast<void*>(&(newChunk->GetComponent(entity, compTypeId)));
			std::memcpy(&dstComp, &dstComp, ComponentManager::SizeOfComponent(compTypeId));
		}

		// Remove old Chunk data
		oldChunk->Remove(entity);

		// Update EntityToArchetypeMap
		sInstance->mEntitiyToArchetypeMap.erase(entity);
		sInstance->mEntitiyToArchetypeMap.insert({ entity, newArchetype });
	}
}