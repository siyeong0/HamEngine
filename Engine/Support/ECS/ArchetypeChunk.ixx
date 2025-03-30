module;

import Common;
import Memory;
import SolbitSTL;
import ECS.Entity;
import ECS.ComponentManager;
import ECS.Archetype;

export module ECS.ArchetypeChunk;

export namespace solbit
{
	class ArchetypeChunk
	{
	public:
		static constexpr size_t MEM_SIZE = 16 * 1024;
	public:
		ArchetypeChunk(const Archetype& archetype);
		~ArchetypeChunk();

		ArchetypeChunk(const ArchetypeChunk&) = delete;
		ArchetypeChunk(ArchetypeChunk&& other);

		uint32 Add(const Entity entity);
		void Remove(uint32 idx, ArchetypeChunk& tailChunk);

		Entity& GetEntity(uint32 idx);

		void* GetComponent(ID compId, uint32 idx);

		template <typename ComponentType>
		ComponentType& GetComponent(uint32 idx);

		inline bool IsFull() const;

	private:
		uint8* mBuffer;
		uint32 mSize;
		const size_t mCapacity;
		Vector<Pair<ID, uint8*>> mBaseAddressVec;
	};

	ArchetypeChunk::ArchetypeChunk(const Archetype& archetype)
		: mBuffer(reinterpret_cast<uint8*>(Alloc(MEM_SIZE)))
		, mSize(0)
		, mCapacity(MEM_SIZE / (sizeof(Entity) + ComponentManager::GetSizeOfArchetype(archetype)))
		, mBaseAddressVec()
	{
		std::memset(mBuffer, 0, MEM_SIZE);
		mBaseAddressVec.reserve(archetype.GetSize());
		uint8* baseAddress = mBuffer + mCapacity * sizeof(Entity);
		for (auto& sizeInfo : ComponentManager::GetSizeVectorOfArchetype(archetype))
		{
			mBaseAddressVec.push_back({ sizeInfo.first, baseAddress });
			baseAddress += sizeInfo.second * mCapacity;
		}
	}

	ArchetypeChunk::~ArchetypeChunk()
	{
		Free(mBuffer);
	}

	ArchetypeChunk::ArchetypeChunk(ArchetypeChunk&& other)
		: mBuffer(other.mBuffer)
		, mSize(other.mSize)
		, mCapacity(other.mCapacity)
		, mBaseAddressVec(std::move(other.mBaseAddressVec))
	{
		other.mBuffer = nullptr;
	}

	uint32 ArchetypeChunk::Add(const Entity entity)
	{
		ASSERT(!IsFull());
		std::memcpy(mBuffer + mSize * sizeof(Entity), &entity, sizeof(Entity));
		return mSize++;
	}

	void ArchetypeChunk::Remove(uint32 idx, ArchetypeChunk& tailChunk)
	{
		ASSERT(idx < mSize);

		uint32 removedIdx = idx;
		uint32 copiedIdx = tailChunk.mSize - 1;

		if (!(&tailChunk == this && removedIdx == copiedIdx))
		{
			uint8* dstPtr = mBuffer + idx * sizeof(Entity);
			uint8* srcPtr = tailChunk.mBuffer + copiedIdx * sizeof(Entity);
			std::memcpy(dstPtr, srcPtr, sizeof(Entity));
			for (auto& pair : mBaseAddressVec)
			{
				uint8* baseAddress = pair.second;
				size_t size = ComponentManager::GetSizeOfComponent(pair.first);
				uint8* dstPtr = mBuffer + idx * size;
				uint8* srcPtr = tailChunk.mBuffer + copiedIdx * size;
				std::memcpy(dstPtr, srcPtr, size);
			}
		}
		--tailChunk.mSize;;
	}

	Entity& ArchetypeChunk::GetEntity(uint32 idx)
	{
		ASSERT(idx < mSize);
		return *reinterpret_cast<Entity*>(mBuffer + idx * sizeof(Entity));
	}

	void* ArchetypeChunk::GetComponent(ID compId, uint32 idx)
	{
		ASSERT(idx < mSize);

		uint8* baseAddress = nullptr;
		for (auto& pair : mBaseAddressVec)
		{
			if (pair.first == compId)
			{
				baseAddress = pair.second;
			}
		}
		ASSERT(baseAddress != nullptr);

		return reinterpret_cast<void*>(baseAddress + idx * ComponentManager::GetSizeOfComponent(compId));
	}

	template <typename ComponentType>
	ComponentType& ArchetypeChunk::GetComponent(uint32 idx)
	{
		ASSERT(idx < mSize);

		uint8* baseAddress = nullptr;
		for (auto& pair : mBaseAddressVec)
		{
			if (pair.first == RTTI<ComponentType>::GetId())
			{
				baseAddress = pair.second;
			}
		}
		ASSERT(baseAddress != nullptr);

		return *reinterpret_cast<ComponentType*>(baseAddress + idx * sizeof(ComponentType));
	}

	inline bool ArchetypeChunk::IsFull() const
	{
		return mSize == mCapacity;
	}
}