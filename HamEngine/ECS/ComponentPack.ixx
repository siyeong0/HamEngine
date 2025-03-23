module;

import Common;
import HamSTL.Utility;
import HamSTL.Vector;
import ECS.Entity;
import ECS.Archetype;

export module ECS.ComponentPack;

export namespace ham
{
	class ComponentPack
	{
	public:
		ComponentPack();
		ComponentPack(uint8* entityAddress, const Vector<Pair<Id, size_t>>* archetypeSizeVec);
		~ComponentPack() = default;

		ComponentPack(const ComponentPack& other);
		ComponentPack(const ComponentPack&& other);

		template <typename T>
		T& GetComponent() const;

	public:
		uint8* mData;
		const Vector<Pair<Id, size_t>>* mArchetypeSizeVec;
	};
}

namespace ham
{
	ComponentPack::ComponentPack()
		: mData(nullptr)
		, mArchetypeSizeVec(nullptr)
	{

	}

	ComponentPack::ComponentPack(uint8* entityAddress, const Vector<Pair<Id, size_t>>* archetypeSizeVec)
		: mData(entityAddress)
		, mArchetypeSizeVec(archetypeSizeVec)
	{

	}

	ComponentPack::ComponentPack(const ComponentPack& other)
		: mData(other.mData)
		, mArchetypeSizeVec(other.mArchetypeSizeVec)
	{

	}

	ComponentPack::ComponentPack(const ComponentPack&& other)
		: mData(other.mData)
		, mArchetypeSizeVec(other.mArchetypeSizeVec)
	{

	}

	template <typename T>
	T& ComponentPack::GetComponent() const
	{
		Id componentTypeId = TypeId<T>::GetId();
		size_t offset = sizeof(Entity);
		for (const auto& sizeInfo : *mArchetypeSizeVec)
		{
			if (sizeInfo.first == componentTypeId)
				break;
			offset += sizeInfo.second;
		}

		return *reinterpret_cast<T*>(mData + offset);
	}
}