module;

import Common;
import HamSTL.Utility;
import HamSTL.Vector;
import ECS.Entity;
import ECS.IComponent;
import ECS.ComponentManager;
import ECS.Archetype;

export module ECS.ArchetypeChunk;

export namespace ham
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

		void Add(const Entity& entity);
		void Remove(const Entity& entity);

		template <typename ComponentType>
		ComponentType& GetComponent(const Entity& entity);
		IComponent& GetComponent(const Entity& entity, uint32 componentTypeId);

		inline int GetEntityIdx(const Entity& entity) const;

		inline bool Has(const Entity& entity);
		inline bool IsFull() const;

	private:
		const Archetype mArchetype;
		uint8* mBuffer;
		size_t mSize;
		const size_t mCapacity;
		const Vector<Pair<Id, size_t>> mArchetypeSizeVec;
	};

	ArchetypeChunk::ArchetypeChunk(const Archetype& archetype)
		: mArchetype(archetype)
		, mBuffer(reinterpret_cast<uint8*>(Alloc(MEM_SIZE)))
		, mSize(0)
		, mCapacity(MEM_SIZE / (ComponentManager::GetSizeOfArchetype(mArchetype)))
		, mArchetypeSizeVec(ComponentManager::GetSizeVectorOfArchetype(mArchetype))
	{
		std::memset(mBuffer, 0, MEM_SIZE);
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
	{
		other.mBuffer = nullptr;
	}

	void ArchetypeChunk::Add(const Entity& entity)
	{
		ASSERT(mSize < mCapacity);

		uint8* baseAddress = mBuffer + mSize * sizeof(Entity);
		std::memcpy(baseAddress, &entity, sizeof(Entity));

		size_t offset = 0;
		for (auto& typeInfo : mArchetypeSizeVec)
		{
			size_t size = typeInfo.second;
			std::memset(baseAddress + offset, 0, size);	// Component Initialization
			offset += size * mCapacity;
		}

		++mSize;
	}

	void ArchetypeChunk::Remove(const Entity& entity)
	{
		size_t entityIdx = GetEntityIdx(entity);
		ASSERT(entityIdx != -1);	// Contains

		if (entityIdx == mSize) // Remove the tail data
		{
			uint8* tailPtr = mBuffer + mSize * sizeof(Entity);
			// remove entity data
			std::memset(tailPtr, 0, sizeof(Entity));
			// remove component data
			size_t offset = 0;
			for (auto& typeInfo : mArchetypeSizeVec)
			{
				size_t size = typeInfo.second;
				std::memset(tailPtr + offset, 0, size);
				offset += size * mCapacity;
			}
		}
		else // Copy the tail data to space be removed
		{
			uint8* targetPtr = mBuffer + entityIdx * sizeof(Entity);
			uint8* tailPtr = mBuffer + mSize * sizeof(Entity);
			// copy entity data
			std::memcpy(targetPtr, tailPtr, sizeof(Entity));
			std::memset(tailPtr, 0, sizeof(Entity));
			// copy component data
			size_t offset = 0;
			for (auto& typeInfo : mArchetypeSizeVec)
			{
				size_t size = typeInfo.second;
				std::memcpy(targetPtr + offset, tailPtr + offset, size);
				std::memset(tailPtr + offset, 0, size);
				offset += size * mCapacity;
			}
		}

		--mSize;
		return;
	}

	template <typename ComponentType>
	ComponentType& ArchetypeChunk::GetComponent(const Entity& entity)
	{
		ASSERT(GetEntityIdx(entity) != -1);

		uint8* baseAddress = mBuffer + GetEntityIdx(entity) * sizeof(Entity);
		size_t offset = 0;
		for (auto& typeInfo : mArchetypeSizeVec)
		{
			if (typeInfo.first == TypeId<ComponentType>::GetId())
				break;
			offset += typeInfo.second * mCapacity;
		}

		return *reinterpret_cast<ComponentType*>(baseAddress + offset);
	}

	IComponent& ArchetypeChunk::GetComponent(const Entity& entity, uint32 componentTypeId)
	{
		ASSERT(GetEntityIdx(entity) != -1);

		uint8* baseAddress = mBuffer + GetEntityIdx(entity) * sizeof(Entity);
		size_t offset = 0;
		for (auto& typeInfo : mArchetypeSizeVec)
		{
			if (typeInfo.first == componentTypeId)
				break;
			offset += typeInfo.second * mCapacity;
		}

		return *reinterpret_cast<IComponent*>(baseAddress + offset);
	}

	inline bool ArchetypeChunk::Has(const Entity& entity)
	{
		size_t entityIdx = GetEntityIdx(entity);
		return entityIdx != -1;
	}

	inline bool ArchetypeChunk::IsFull() const
	{
		return mSize == mCapacity;
	}

	inline int ArchetypeChunk::GetEntityIdx(const Entity& entity) const
	{
		Entity* iter = reinterpret_cast<Entity*>(mBuffer);
		for (int i = 0; i < mSize; i++)
		{
			if (iter[i] == entity)
			{
				return i;
			}
		}
		return -1;
	}
}