module;

import std.core;
import Common;
import Memory;
import SolbitSTL;
import ECS.Entity;
import ECS.EntityDataArray;
import ECS.ComponentManager;
import ECS.Archetype;
import ECS.ArchetypeChunk;

export module ECS.EntityManager;

export namespace solbit
{
	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		static EntityManager* GetActive();
		static void SetActive(EntityManager* em);

		const Entity CreateEntity();
		void DestroyEntity(const Entity entity);

		template <typename ComponentType>
		void AddComponent(const Entity entity);
		template <typename ComponentType>
		void RemoveComponent(const Entity entity);
		template <typename ComponentType>
		ComponentType& GetComponent(const Entity entity);

	private:
		EntityManager(const EntityManager&) = delete;
		EntityManager(const EntityManager&&) = delete;
	private:
		static EntityManager* msActiveEM;

		List<EntityDataArray> mEntityDataArrayList;
		HashMap<Entity, Pair<uint32, uint32>, EntityHash> mEntityIdxMap;
		HashMap<ID, Pair<Archetype, List<ArchetypeChunk>>> mArchetypeChunkMap;
	};
}

namespace solbit
{
	EntityManager* EntityManager::msActiveEM = nullptr;

	EntityManager* EntityManager::GetActive()
	{
		return msActiveEM;
	}

	void EntityManager::SetActive(EntityManager* em)
	{
		msActiveEM = em;
	}

	const Entity EntityManager::CreateEntity()
	{
		// TODO: 아이디 생성
		static uint32 entityId = 0;
		Entity entity(entityId++);

		uint32 listIdx = 0;
		uint32 arrIdx;
		for (auto listIter = mEntityDataArrayList.begin(); listIter != mEntityDataArrayList.end(); ++listIter)
		{
			if (!listIter->IsFull())
			{
				goto LIST_LOOP_EXIT;
			}
			++listIdx;
		}
		// Insert new entity data array
		mEntityDataArrayList.push_back(EntityDataArray());
	LIST_LOOP_EXIT:

		auto dstIter = mEntityDataArrayList.begin();
		std::advance(dstIter, listIdx);

		EntityData ed;
		ed.Archetype = nullptr;
		ed.Chunk = nullptr;
		ed.IndexInChunk = 0;
		arrIdx = dstIter->Add(ed);

		mEntityIdxMap.insert({ entity, { listIdx, arrIdx } });

		return entity;
	}

	void EntityManager::DestroyEntity(const Entity entity)
	{
		ASSERT(mEntityIdxMap.find(entity) != mEntityIdxMap.end());

		auto& idxPair = mEntityIdxMap[entity];
		auto dstIter = mEntityDataArrayList.begin();
		std::advance(dstIter, idxPair.first);

		dstIter->Remove(idxPair.second);
	}

	template <typename ComponentType>
	void EntityManager::AddComponent(const Entity entity)
	{
		ASSERT(mEntityIdxMap.find(entity) != mEntityIdxMap.end());

		auto& idxPair = mEntityIdxMap[entity];
		auto dstIter = mEntityDataArrayList.begin();
		std::advance(dstIter, idxPair.first);
		EntityData& ed = dstIter->Get(idxPair.second);

		// Build destination archetype
		Archetype dstArchVal;
		if (ed.Archetype == nullptr)
		{
			dstArchVal.Insert(RTTI<ComponentType>::GetId());
		}
		else
		{
			dstArchVal = *ed.Archetype;
			dstArchVal.Insert(RTTI<ComponentType>::GetId());
		}
		// Add a chunkmap if dst archetype is a new one
		if (mArchetypeChunkMap.find(dstArchVal.GetId()) == mArchetypeChunkMap.end())
		{
			List<ArchetypeChunk> chunkList;
			chunkList.push_back(ArchetypeChunk(dstArchVal));
			uint32 idxInChunk = chunkList.back().Add(entity);
			mArchetypeChunkMap.insert({ dstArchVal.GetId(), { dstArchVal, std::move(chunkList) } });
		}
		// Copy component datas
		Archetype& dstArch = mArchetypeChunkMap[dstArchVal.GetId()].first;
		ArchetypeChunk& dstChunk = mArchetypeChunkMap[dstArch.GetId()].second.back();
		uint32 dstIdxInChunk = dstChunk.Add(entity);

		if (ed.Archetype != nullptr)
		{
			Archetype& srcArch = *ed.Archetype;
			ArchetypeChunk& srcChunk = *ed.Chunk;
			uint32 srcIdxInChunk = ed.IndexInChunk;
			for (auto compId : srcArch.GetComponentTypeIdSet())
			{
				std::memcpy(dstChunk.GetComponent(compId, dstIdxInChunk), srcChunk.GetComponent(compId, srcIdxInChunk), ComponentManager::GetSizeOfComponent(compId));
			}
			// Remove source data
			srcChunk.Remove(srcIdxInChunk, mArchetypeChunkMap[srcArch.GetId()].second.back());
		}
		// Update entity data
		ed.Archetype = &dstArch;
		ed.Chunk = &dstChunk;
		ed.IndexInChunk = dstIdxInChunk;

		new (&(this->GetComponent<ComponentType>(entity))) (ComponentType);
	}

	template <typename ComponentType>
	void EntityManager::RemoveComponent(const Entity entity)
	{
		ASSERT(mEntityIdxMap.find(entity) != mEntityIdxMap.end());

		auto& idxPair = mEntityIdxMap[entity];
		auto dstIter = mEntityDataArrayList.begin();
		std::advance(dstIter, idxPair.first);
		EntityData& ed = dstIter->Get(idxPair.second);

		// Build destination archetype
		ASSERT(ed.Archetype != nullptr);
		Archetype dstArchVal;
		dstArchVal = *ed.Archetype;
		dstArchVal.Erase(RTTI<ComponentType>::GetId());
		// Add a chunkmap if dst archetype is a new one
		if (mArchetypeChunkMap.find(dstArchVal.GetId()) == mArchetypeChunkMap.end())
		{
			List<ArchetypeChunk> chunkList;
			chunkList.push_back(ArchetypeChunk(dstArchVal));
			uint32 idxInChunk = chunkList.back().Add(entity);
			mArchetypeChunkMap.insert(dstArchVal.GetId(), { dstArchVal, std::move(chunkList) });
		}
		// Copy component datas
		Archetype& dstArch = mArchetypeChunkMap[dstArch.GetId()].first;
		ArchetypeChunk& dstChunk = mArchetypeChunkMap[dstArch.GetId()].second.back();
		uint32 dstIdxInChunk = dstChunk.Add(entity);
		Archetype& srcArch = *ed.Archetype;
		ArchetypeChunk& srcChunk = *ed.Chunk;
		uint32 srcIdxInChunk = ed.IndexInChunk;
		for (auto compId : srcArch.GetComponentTypeIdSet)
		{
			std::memcpy(dstChunk.GetComponent(compId, dstIdxInChunk), dstChunk.GetComponent(compId, srcIdxInChunk), ComponentManager::GetSizeOfComponent(compId));
		}
		// Remove source datab
		srcChunk.Remove(srcIdxInChunk, mArchetypeChunkMap[srcArch].second.back());

		// Update entity data
		ed.Archetype = dstArch;
		ed.Chunk = dstChunk;
		ed.IndexInChunk = dstIdxInChunk;
	}

	template <typename ComponentType>
	ComponentType& EntityManager::GetComponent(const Entity entity)
	{
		ASSERT(mEntityIdxMap.find(entity) != mEntityIdxMap.end());

		auto& idxPair = mEntityIdxMap[entity];
		auto dstIter = mEntityDataArrayList.begin();
		std::advance(dstIter, idxPair.first);

		EntityData ed = dstIter->Get(idxPair.second);
		ArchetypeChunk* chunk = ed.Chunk;
		const uint32 idxInChunk = ed.IndexInChunk;

		return chunk->GetComponent<ComponentType>(idxInChunk);
	}
}