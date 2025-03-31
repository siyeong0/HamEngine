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

		inline ID GetId() const;
		inline size_t GetNumTypes() const;
		inline bool HasType(ID typeId) const;

	private:
		HashSet<uint32> mSet;
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

	inline ID Archetype::GetId() const
	{
		// TODO: optimization
		uint32 hash = 0;
		Vector<int> sorted(mSet.begin(), mSet.end()); // Á¤·Ä
		std::sort(sorted.begin(), sorted.end());

		for (auto v : sorted)
		{
			hash = 65599 * hash + v;
		}
		return hash;
	}

	inline size_t Archetype::GetNumTypes() const
	{
		return mSet.size();
	}

	inline bool Archetype::HasType(ID typeId) const
	{
		for (auto id : mSet)
		{
			if (typeId == id)
			{
				return true;
			}
		}
		return false;
	}
}