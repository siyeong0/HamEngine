module;

import std.core;
import Common;
import Memory;
import SolbitSTL;
import ECS.Entity;
import ECS.ComponentPack;
import ECS.ComponentManager;
import ECS.Archetype;
import ECS.ArchetypeChunk;

export module ECS.EntityManager;

export namespace solbit
{
	class EntityManager
	{
	public:
		static EntityManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		const Entity CreateEntity();
		void DestroyEntity();

		template <typename ComponentType>
		ComponentType& AddComponent(const Entity entity);
		template <typename ComponentType>
		void RemoveComponent(const Entity entity);

		ComponentPack GetComponentPack(const Entity entity);



	private:
		EntityManager() = default;
		~EntityManager() = default;
		EntityManager(const EntityManager&) = delete;
		EntityManager(const EntityManager&&) = delete;
	private:
		static EntityManager* msInstance;

		Vector<Entity> mEntities;
		HashMap<Entity, Archetype, EntityHash> mEntitiyToArchetypeMap;
		HashMap<Archetype, List<ArchetypeChunk>, ArchetypeHash> mArchetypeChunkMap;
	};
}

namespace solbit
{
	EntityManager* EntityManager::msInstance = nullptr;

	EntityManager* EntityManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool EntityManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new EntityManager;

		// msInstance->mEntities.reserve(4096);
		return msInstance != nullptr;
	}

	void EntityManager::Finalize()
	{
		ASSERT(msInstance != nullptr);
		delete msInstance;
		msInstance = nullptr;
	}

	const Entity EntityManager::CreateEntity()
	{
		// TODO: 아이디 생성
		static uint32 entityId = 0;
		mEntities.push_back(Entity(entityId++));

		return mEntities[mEntities.size() - 1];
	}

	void EntityManager::DestroyEntity()
	{

	}

	template <typename ComponentType>
	ComponentType& EntityManager::AddComponent(const Entity entity)
	{
		if (mEntitiyToArchetypeMap.find(entity) == mEntitiyToArchetypeMap.end()) // Without components
		{
			// Build an archetype
			Archetype archetype;
			archetype.Insert(RTTI<ComponentType>::GetId());
			mEntitiyToArchetypeMap.insert({ entity, archetype });

			// Build chunk list for archetype if not exist;
			if (mArchetypeChunkMap.find(archetype) == mArchetypeChunkMap.end())
			{
				List<ArchetypeChunk> chunkList;
				chunkList.push_back(ArchetypeChunk(archetype));
				mArchetypeChunkMap.insert({ archetype, std::move(chunkList) });
			}

			// Find or allocate dst chunk;
			ArchetypeChunk* dstChunk = nullptr;
			List<ArchetypeChunk>& chunkList = mArchetypeChunkMap[archetype];
			for (ArchetypeChunk& chunk : chunkList)
			{
				if (!chunk.IsFull())
				{
					dstChunk = &chunk;
					goto EXIT;
				}
			}
			chunkList.push_back(ArchetypeChunk(archetype)); // All chunks are full
			dstChunk = &chunkList.back();
		EXIT:
			dstChunk->Add(entity);
			return *dstChunk->GetComponentOrNull<ComponentType>(entity);
		}
		else // With components
		{
			const Archetype& srcArchetype = mEntitiyToArchetypeMap[entity];
			Archetype dstArchetype;
			dstArchetype.Insert(srcArchetype).Insert(RTTI<ComponentType>::GetId());

			// Find src/dst chunk list
			List<ArchetypeChunk>& srcChunkList = mArchetypeChunkMap[srcArchetype];
			if (mArchetypeChunkMap.find(dstArchetype) == mArchetypeChunkMap.end()) // Chunk for archetype not exist;
			{
				List<ArchetypeChunk> chunkList;
				chunkList.push_back(ArchetypeChunk(dstArchetype));
				mArchetypeChunkMap.insert({ dstArchetype, std::move(chunkList) });
			}
			List<ArchetypeChunk>& dstChunkList = mArchetypeChunkMap[dstArchetype];

			// Find src/dst chunk
			ArchetypeChunk* srcChunk = nullptr;
			for (ArchetypeChunk& chunk : srcChunkList)
			{
				if (chunk.Has(entity))
				{
					srcChunk = &chunk;
					break;
				}
			}
			ASSERT(srcChunk != nullptr);
			ArchetypeChunk* dstChunk = nullptr;
			for (ArchetypeChunk& chunk : dstChunkList)
			{
				if (!chunk.IsFull())
				{
					dstChunk = &chunk;
					goto MOVE_ENTITY_EXIT;
				}
			}
			dstChunkList.push_back(ArchetypeChunk(dstArchetype)); // All chunks are full
			dstChunk = &dstChunkList.back();
		MOVE_ENTITY_EXIT:

			// Add an entity to dst chunk and copy component data from src
			dstChunk->Add(entity);
			const Archetype& subset = srcArchetype;
			for (auto& componentTypeId : subset.GetComponentTypeIdSet())
			{
				std::memcpy(
					dstChunk->GetComponentOrNull(entity, componentTypeId),
					srcChunk->GetComponentOrNull(entity, componentTypeId),
					ComponentManager::GetSizeOfComponent(componentTypeId));
			}

			// Remove an entity from src chunk
			srcChunk->Remove(entity);

			// Update EntityToArchetypeMap
			mEntitiyToArchetypeMap.erase(entity);
			mEntitiyToArchetypeMap.insert({ entity, dstArchetype });

			return *dstChunk->GetComponentOrNull<ComponentType>(entity);
		}
	}

	template <typename ComponentType>
	void EntityManager::RemoveComponent(const Entity entity)
	{
		ASSERT(mEntitiyToArchetypeMap.find(entity) != mEntitiyToArchetypeMap.end());	// Entity has a component

		const Archetype& srcArchetype = mEntitiyToArchetypeMap[entity];
		Archetype dstArchetype;
		dstArchetype.Insert(srcArchetype).Erase(RTTI<ComponentType>::GetId());

		if (dstArchetype.GetSize() == 0)
		{
			List<ArchetypeChunk>& srcChunkList = mArchetypeChunkMap[srcArchetype];
			ArchetypeChunk* srcChunk = nullptr;
			for (ArchetypeChunk& chunk : srcChunkList)
			{
				if (chunk.Has(entity))
				{
					srcChunk = &chunk;
					break;
				}
			}
			ASSERT(srcChunk != nullptr);

			srcChunk->Remove(entity);
			mEntitiyToArchetypeMap.erase(entity);
		}
		else
		{
			// Find src/dst chunk list
			List<ArchetypeChunk>& srcChunkList = mArchetypeChunkMap[srcArchetype];
			if (mArchetypeChunkMap.find(dstArchetype) == mArchetypeChunkMap.end()) // Chunk for archetype not exist;
			{
				List<ArchetypeChunk> chunkList;
				chunkList.push_back(ArchetypeChunk(dstArchetype));
				mArchetypeChunkMap.insert({ dstArchetype, std::move(chunkList) });
			}
			List<ArchetypeChunk>& dstChunkList = mArchetypeChunkMap[dstArchetype];

			// Find src/dst chunk
			ArchetypeChunk* srcChunk = nullptr;
			for (ArchetypeChunk& chunk : srcChunkList)
			{
				if (chunk.Has(entity))
				{
					srcChunk = &chunk;
					break;
				}
			}
			ASSERT(srcChunk != nullptr);
			ArchetypeChunk* dstChunk = nullptr;
			for (ArchetypeChunk& chunk : dstChunkList)
			{
				if (!chunk.IsFull())
				{
					dstChunk = &chunk;
					goto MOVE_ENTITY_EXIT;
				}
			}
			dstChunkList.push_back(ArchetypeChunk(dstArchetype)); // All chunks are full
			dstChunk = &dstChunkList.back();
		MOVE_ENTITY_EXIT:

			// Add an entity to dst chunk and copy component data from src
			dstChunk->Add(entity);

			const Archetype& subset = dstArchetype;
			for (auto& componentTypeId : subset.GetComponentTypeIdSet())
			{
				std::memcpy(
					dstChunk->GetComponentOrNull(entity, componentTypeId),
					srcChunk->GetComponentOrNull(entity, componentTypeId),
					ComponentManager::GetSizeOfComponent(componentTypeId));
			}

			// Remove an entity from src chunk
			srcChunk->Remove(entity);

			// Update EntityToArchetypeMap
			mEntitiyToArchetypeMap.erase(entity);
			mEntitiyToArchetypeMap.insert({ entity, dstArchetype });
		}
	}

	ComponentPack EntityManager::GetComponentPack(const Entity entity)
	{
		const Archetype& archetype = mEntitiyToArchetypeMap[entity];
		List<ArchetypeChunk>& chunkList = mArchetypeChunkMap[archetype];

		ComponentPack componentPack;
		for (auto& chunk : chunkList)
		{
			if (chunk.GetComponentPack(entity, &componentPack))
				break;
		}
		ASSERT(componentPack.mData != nullptr);

		return componentPack;
	}
}