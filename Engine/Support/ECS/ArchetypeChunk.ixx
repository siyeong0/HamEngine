module;

import Common;
import Memory;
import SolbitSTL;
import ECS.Entity;
import ECS.ComponentPack;
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

		void Add(const Entity entity);
		void Remove(const Entity entity);

		template <typename ComponentType>
		ComponentType* GetComponentOrNull(const Entity entity);
		void* GetComponentOrNull(const Entity entity, uint32 componentTypeId);
		bool GetComponentPack(const Entity entity, ComponentPack* outComponentPack);

		inline int GetEntityIdx(const Entity entity) const;

		inline bool Has(const Entity entity);
		inline bool IsFull() const;

	private:
		const Archetype mArchetype;
		uint8* mBuffer;
		size_t mSize;
		const size_t mCapacity;
		const Vector<Pair<Id, size_t>> mArchetypeSizeVec;
		size_t mBundleSize;
	};

	ArchetypeChunk::ArchetypeChunk(const Archetype& archetype)
		: mArchetype(archetype)
		, mBuffer(reinterpret_cast<uint8*>(Alloc(MEM_SIZE)))
		, mSize(0)
		, mCapacity(MEM_SIZE / (ComponentManager::GetSizeOfArchetype(mArchetype)))
		, mArchetypeSizeVec(ComponentManager::GetSizeVectorOfArchetype(mArchetype))
		, mBundleSize(sizeof(Entity))
	{
		std::memset(mBuffer, 0, MEM_SIZE);
		for (auto& sizeInfo : mArchetypeSizeVec)
		{
			mBundleSize += sizeInfo.second;
		}
	}

	ArchetypeChunk::~ArchetypeChunk()
	{
		Free(mBuffer);
	}

	ArchetypeChunk::ArchetypeChunk(ArchetypeChunk&& other)
		: mArchetype(other.mArchetype)
		, mBuffer(other.mBuffer)
		, mSize(other.mSize)
		, mCapacity(other.mCapacity)
		, mArchetypeSizeVec(std::move(other.mArchetypeSizeVec))
		, mBundleSize(other.mBundleSize)
	{
		other.mBuffer = nullptr;
	}

	void ArchetypeChunk::Add(const Entity entity)
	{
		ASSERT(mSize < mCapacity);

		uint8* baseAddress = mBuffer + mSize * mBundleSize;
		std::memcpy(baseAddress, &entity, sizeof(Entity));

		++mSize;
	}

	void ArchetypeChunk::Remove(const Entity entity)
	{
		size_t entityIdx = GetEntityIdx(entity);
		ASSERT(entityIdx != -1);	// Contains

		if (entityIdx == mSize) // Remove the tail data
		{
			uint8* tailPtr = mBuffer + mSize * mBundleSize;
			// remove entity/component data
			std::memset(tailPtr, 0, mBundleSize);
		}
		else // Copy the tail data to space be removed
		{
			uint8* targetPtr = mBuffer + entityIdx * mBundleSize;
			uint8* tailPtr = mBuffer + mSize * mBundleSize;
			// copy entity/component data
			std::memcpy(targetPtr, tailPtr, mBundleSize);
			std::memset(tailPtr, 0, mBundleSize);
		}

		--mSize;
		return;
	}

	template <typename ComponentType>
	ComponentType* ArchetypeChunk::GetComponentOrNull(const Entity entity)
	{
		int entityIdx = GetEntityIdx(entity);
		if (entityIdx == -1)
			return nullptr;

		uint8* baseAddress = mBuffer + entityIdx * mBundleSize;
		size_t offset = sizeof(Entity);
		for (auto& typeInfo : mArchetypeSizeVec)
		{
			if (typeInfo.first == RTTI<ComponentType>::GetId())
				break;
			offset += typeInfo.second;
		}

		return reinterpret_cast<ComponentType*>(baseAddress + offset);
	}

	void* ArchetypeChunk::GetComponentOrNull(const Entity entity, uint32 componentTypeId)
	{
		int entityIdx = GetEntityIdx(entity);
		if (entityIdx == -1)
			return nullptr;

		uint8* baseAddress = mBuffer + entityIdx * mBundleSize;
		size_t offset = sizeof(Entity);
		for (auto& typeInfo : mArchetypeSizeVec)
		{
			if (typeInfo.first == componentTypeId)
				break;
			offset += typeInfo.second;
		}

		return reinterpret_cast<void*>(baseAddress + offset);
	}

	bool ArchetypeChunk::GetComponentPack(const Entity entity, ComponentPack* outComponentPack)
	{
		int entityIdx = GetEntityIdx(entity);
		if (entityIdx == -1)
		{
			outComponentPack = nullptr;
			return false;
		}
		// TODO: placement new ¿¡·¯
		outComponentPack->mData = mBuffer + entityIdx * mBundleSize;
		outComponentPack->mArchetypeSizeVec = &mArchetypeSizeVec;

		return true;
	}

	inline int ArchetypeChunk::GetEntityIdx(const Entity entity) const
	{
		uint8* iter = mBuffer;
		for (int i = 0; i < mSize; i++)
		{
			if (*reinterpret_cast<Entity*>(iter) == entity)
				return i;
			iter += mBundleSize;
		}
		return -1;
	}

	inline bool ArchetypeChunk::Has(const Entity entity)
	{
		size_t entityIdx = GetEntityIdx(entity);
		return entityIdx != -1;
	}

	inline bool ArchetypeChunk::IsFull() const
	{
		return mSize == mCapacity;
	}
}