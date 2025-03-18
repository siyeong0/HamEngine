module;

import Common;
import ECS.Entity;
import ECS.IComponent;
import ECS.Archetype;
import ECS.ComponentManager;
import STL.Array;

export module ECS.ArchetypeChunk;

export namespace ham
{
	class ArchetypeChunk
	{
	public:
		static constexpr size_t MEM_SIZE = 16 * 1024;
	public:
		ArchetypeChunk();
		ArchetypeChunk(const Archetype& archetype);
		~ArchetypeChunk();

		ArchetypeChunk(const ArchetypeChunk& rhs);
		ArchetypeChunk(ArchetypeChunk&& rhs);

		void Add(const Entity& entity);
		void Remove(const Entity& entity);

		template <typename CompType>
		CompType& GetComponent(const Entity& entity);

		IComponent& GetComponent(const Entity& entity, const CompTypeId& compTypeId);

		inline bool Has(const Entity& entity);
		inline bool IsFull() const;

	private:
		inline int getEntityIdx(const Entity& entity) const;

	private:
		Archetype mArchetype;
		size_t mCapacity;
		size_t mEntityCount;

		void* mBuffer;
		Array<void*> mCompBaseAddressArr;
	};

	ArchetypeChunk::ArchetypeChunk()
		: mArchetype()
		, mCapacity(MEM_SIZE / (ComponentManager::SizeOfArchetype(mArchetype)))
		, mEntityCount(0)
		, mBuffer(Alloc(MEM_SIZE))
		, mCompBaseAddressArr(0)
	{

	}

	ArchetypeChunk::ArchetypeChunk(const Archetype& archetype)
		: mArchetype(archetype)
		, mCapacity(MEM_SIZE / (ComponentManager::SizeOfArchetype(mArchetype)))
		, mEntityCount(0)
		, mBuffer(Alloc(MEM_SIZE))
		, mCompBaseAddressArr(archetype.GetNumCompTypes())
	{
		std::memset(mBuffer, 0, MEM_SIZE);

		Array<size_t> sizeArr = ComponentManager::GetSizeArrOfArchetype(mArchetype);
		size_t sizeStride = sizeof(Entity);
		for (int i = 0; i < mArchetype.GetCompTypes().Capacity(); ++i)
		{
			mCompBaseAddressArr[i] = reinterpret_cast<void*>(reinterpret_cast<uint8*>(mBuffer) + sizeStride * mCapacity);
			sizeStride += sizeArr[i];
		}
	}

	ArchetypeChunk::~ArchetypeChunk()
	{
		Free(mBuffer);
	}

	ArchetypeChunk::ArchetypeChunk(const ArchetypeChunk& rhs)
		: mArchetype(rhs.mArchetype)
		, mCapacity(rhs.mCapacity)
		, mEntityCount(rhs.mEntityCount)
		, mBuffer(Alloc(MEM_SIZE))
		, mCompBaseAddressArr(mArchetype.GetNumCompTypes())
	{
		std::memcpy(mBuffer, rhs.mBuffer, MEM_SIZE);
		for (int i = 0; i < mArchetype.GetCompTypes().Capacity(); ++i)
		{
			size_t baseAddress = reinterpret_cast<size_t>(mBuffer);
			size_t rhsBaseAddress = reinterpret_cast<size_t>(rhs.mBuffer);
			size_t rhsCompBaseAddress = reinterpret_cast<size_t>(rhs.mCompBaseAddressArr.Get(i));
			mCompBaseAddressArr[i] = reinterpret_cast<void*>(baseAddress + rhsCompBaseAddress - rhsBaseAddress);
		}
	}

	ArchetypeChunk::ArchetypeChunk(ArchetypeChunk&& rhs)
		: mArchetype(std::move(rhs.mArchetype))
		, mCapacity(rhs.mCapacity)
		, mEntityCount(rhs.mEntityCount)
		, mBuffer(rhs.mBuffer)
		, mCompBaseAddressArr(std::move(rhs.mCompBaseAddressArr))
	{
		rhs.mBuffer = nullptr;
	}

	void ArchetypeChunk::Add(const Entity& entity)
	{
		ASSERT(mEntityCount < mCapacity);

		size_t numComponents = mArchetype.GetNumCompTypes();

		Entity* entityPtr = reinterpret_cast<Entity*>(mBuffer) + mEntityCount;
		std::memcpy(entityPtr, &entity, sizeof(Entity));

		++mEntityCount;
	}

	void ArchetypeChunk::Remove(const Entity& entity)
	{
		size_t entityIdx = getEntityIdx(entity);
		ASSERT(entityIdx != -1);	// Contains

		if (entityIdx == mEntityCount)
		{
			--mEntityCount;
			return;
		}

		Entity* targetPtr = reinterpret_cast<Entity*>(mBuffer) + entityIdx;
		Entity* tailPtr = reinterpret_cast<Entity*>(mBuffer) + mEntityCount;
		std::memcpy(targetPtr, tailPtr, sizeof(Entity));
		std::memset(tailPtr, 0, sizeof(Entity));

		Array<size_t> sizeArr = ComponentManager::GetSizeArrOfArchetype(mArchetype);
		for (size_t i = 0; i < mArchetype.GetCompTypes().Capacity(); ++i)
		{
			const size_t SIZE = sizeArr[i];
			uint8* basePtr = reinterpret_cast<uint8*>(mCompBaseAddressArr[i]);
			void* targetPtr = reinterpret_cast<void*>(basePtr + SIZE * entityIdx);
			void* tailPtr = reinterpret_cast<void*>(basePtr + SIZE * mEntityCount);
			std::memcpy(targetPtr, tailPtr, SIZE);
			std::memset(tailPtr, 0, SIZE);
		}

		--mEntityCount;
		return;
	}

	template <typename CompType>
	CompType& ArchetypeChunk::GetComponent(const Entity& entity)
	{
		size_t entityIdx = getEntityIdx(entity);
		ASSERT(entityIdx != -1);

		uint32 compTypeId = TypeId<CompType>::GetId();
		Array<CompTypeId> typeIdArr = mArchetype.GetCompTypes();
		size_t idx = 0;
		while (compTypeId != typeIdArr[idx])
		{
			idx++;
		}

		return *(reinterpret_cast<CompType*>(mCompBaseAddressArr[idx]) + entityIdx);
	}

	IComponent& ArchetypeChunk::GetComponent(const Entity& entity, const CompTypeId& compTypeId)
	{
		size_t entityIdx = getEntityIdx(entity);
		ASSERT(entityIdx != -1);

		Array<CompTypeId> typeIdArr = mArchetype.GetCompTypes();
		size_t idx = 0;
		while (compTypeId != typeIdArr[idx])
		{
			idx++;
		}

		return *(reinterpret_cast<IComponent*>(mCompBaseAddressArr[idx]) + entityIdx);
	}

	inline bool ArchetypeChunk::Has(const Entity& entity)
	{
		size_t entityIdx = getEntityIdx(entity);
		return entityIdx != -1;
	}

	inline bool ArchetypeChunk::IsFull() const
	{
		return mEntityCount == mCapacity;
	}

	inline int ArchetypeChunk::getEntityIdx(const Entity& entity) const
	{
		Entity* iter = reinterpret_cast<Entity*>(mBuffer);
		for (int i = 0; i < mEntityCount; i++)
		{
			if (iter[i] == entity)
			{
				return i;
			}
		}
		return -1;
	}
}