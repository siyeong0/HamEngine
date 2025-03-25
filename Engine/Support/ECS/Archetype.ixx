module;

import std.core;

import Common;
import Memory;
import SolbitSTL;
import ECS.Entity;

export module ECS.Archetype;

export namespace solbit
{
	class Archetype
	{
	public:
		static constexpr size_t CHUNK_MEM_SIZE = 16 * 1024;
	public:
		Archetype();
		~Archetype() = default;

		Archetype(const Archetype& other);
		Archetype(const Archetype&& other);
		Archetype& operator=(const Archetype& other);

		const HashSet<uint32>& GetComponentTypeIdSet() const;

		Archetype& Insert(uint32 componentTypeId);
		Archetype& Insert(const Archetype& other);
		Archetype& Erase(uint32 componentTypeId);

		inline bool operator==(const Archetype& other) const;
		inline bool operator!=(const Archetype& other) const;

		inline size_t GetSize() const;

	private:
		HashSet<uint32> mSet;
	};

	struct ArchetypeHash
	{
		size_t operator()(const Archetype& archetype) const
		{
			size_t hash = 0;
			for (auto v : archetype.GetComponentTypeIdSet())
			{
				hash = 65599 * hash + v;
			}
			return hash;
		}
	};
}

namespace solbit
{
	Archetype::Archetype()
		: mSet()
	{

	}

	Archetype::Archetype(const Archetype& other)
		: mSet(other.mSet)
	{

	}

	Archetype::Archetype(const Archetype&& other)
		: mSet(std::move(other.mSet))
	{

	}

	Archetype& Archetype::operator=(const Archetype& other)
	{
		mSet = other.mSet;
		return *this;
	}

	const HashSet<uint32>& Archetype::GetComponentTypeIdSet() const
	{
		return mSet;
	}

	Archetype& Archetype::Insert(uint32 componentTypeId)
	{
		ASSERT(mSet.find(componentTypeId) == mSet.end());
		mSet.insert(componentTypeId);
		return *this;
	}

	Archetype& Archetype::Insert(const Archetype& other)
	{
		mSet.merge(HashSet<uint32>(other.mSet));
		return *this;
	}

	Archetype& Archetype::Erase(uint32 componentTypeId)
	{
		ASSERT(mSet.find(componentTypeId) != mSet.end());
		mSet.erase(componentTypeId);
		return *this;
	}

	inline bool Archetype::operator==(const Archetype& other) const
	{
		if (mSet.size() != other.mSet.size())
			return false;

		bool bResult = true;
		for (const auto& elem : mSet)
		{
			bResult &= other.mSet.find(elem) != other.mSet.end();
		}
		return bResult;
	}

	inline bool Archetype::operator!=(const Archetype& other) const
	{
		return !(operator==(other));
	}

	inline size_t Archetype::GetSize() const
	{
		return mSet.size();
	}
}