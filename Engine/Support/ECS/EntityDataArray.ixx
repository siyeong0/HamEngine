module;

import Common;
import Memory;
import SolbitSTL;
import ECS.Archetype;
import ECS.ArchetypeChunk;

export module ECS.EntityDataArray;

export namespace solbit
{
	struct EntityData
	{
		Archetype* Archetype;
		ArchetypeChunk* Chunk;
		uint32 IndexInChunk;
	};

	class EntityDataArray
	{
	public:
		static constexpr size_t CAPACITY = 1024;
	public:
		EntityDataArray();
		~EntityDataArray();

		EntityDataArray(const EntityDataArray&) = delete;
		EntityDataArray(EntityDataArray&&);

		EntityData& Get(uint32 idx);
		uint32 Add(const EntityData& entityData);
		void Remove(uint32 idx);

		inline bool IsFull() const;

	private:
		EntityData* mData;
		uint32 mFreeIdx;
	};
}

namespace solbit
{
	EntityDataArray::EntityDataArray()
		: mData(reinterpret_cast<EntityData*>(Alloc(sizeof(EntityData) * CAPACITY)))
		, mFreeIdx(0)
	{
		for (int i = 0; i < CAPACITY; ++i)	// 자유공간리스트
		{
			*reinterpret_cast<int*>(mData + i) = i + 1;
		}
	}

	EntityDataArray::~EntityDataArray()
	{
		Free(mData);
	}

	EntityDataArray::EntityDataArray(EntityDataArray&& other)
		: mData(other.mData)
		, mFreeIdx(other.mFreeIdx)
	{
		other.mData = nullptr;
	}

	EntityData& EntityDataArray::Get(uint32 idx)
	{
		ASSERT(idx < CAPACITY);
		return mData[idx];
	}

	uint32 EntityDataArray::Add(const EntityData& entityData)
	{
		ASSERT(!IsFull());

		uint32 idx = mFreeIdx;
		EntityData* freePtr = mData + idx;
		mFreeIdx = *reinterpret_cast<uint32*>(freePtr);

		std::memcpy(freePtr, &entityData, sizeof(EntityData));

		return idx;
	}

	void EntityDataArray::Remove(uint32 idx)
	{
		ASSERT(idx < CAPACITY);

		EntityData* rmPtr = mData + idx;
		*reinterpret_cast<int*>(rmPtr) = mFreeIdx;
		mFreeIdx = idx;
	}

	inline bool EntityDataArray::IsFull() const
	{
		return mFreeIdx == CAPACITY;
	}
}