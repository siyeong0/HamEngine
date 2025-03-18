module;

import std.core;

import Common;
import STL;
import ECS.Entity;
import ECS.IComponent;

export module ECS.Archetype;

export namespace ham
{
	class Archetype
	{
	public:
		static constexpr size_t CHUNK_MEM_SIZE = 16 * 1024;
	public:
		Archetype();
		Archetype(const Array<CompTypeId>& compTypes);
		~Archetype() = default;

		Archetype(const Archetype& rhs);
		Archetype(const Archetype&& rhs);

		inline bool operator==(const Archetype& rhs) const;
		inline bool operator!=(const Archetype& rhs) const;
		
		inline const Array<CompTypeId>& GetCompTypes() const;
		inline size_t GetNumCompTypes() const;
		inline size_t GetHash() const;

	private:
		// TODO: Set으로 변경, 현재 중복 처리 안됨
		Array<CompTypeId> mCompTypes;
		size_t mHash;
	};

	class ArchetypeHash
	{
	public:
		std::size_t operator()(const Archetype& arche) const
		{
			return arche.GetHash();
		}
	};

	Archetype::Archetype()
		: mCompTypes(0)
		, mHash(0)
	{

	}

	Archetype::Archetype(const Array<CompTypeId>& compTypes)
		: mCompTypes(compTypes)
		, mHash(0)
	{
		std::sort(mCompTypes.Begin(), mCompTypes.End());
		mHash = mCompTypes.GenerateHash();
	}

	Archetype::Archetype(const Archetype& rhs)
		: mCompTypes(rhs.mCompTypes)
		, mHash(rhs.mHash)
	{

	}

	Archetype::Archetype(const Archetype&& rhs)
		: mCompTypes(std::move(rhs.mCompTypes))
		, mHash(rhs.mHash)
	{

	}

	inline bool Archetype::operator==(const Archetype& rhs) const
	{
		return mHash == rhs.mHash;
	}

	inline bool Archetype::operator!=(const Archetype& rhs) const
	{
		return mHash != rhs.mHash;
	}

	inline const Array<CompTypeId>& Archetype::GetCompTypes() const
	{
		return mCompTypes;
	}

	inline size_t Archetype::GetNumCompTypes() const
	{
		return mCompTypes.Capacity();
	}

	inline size_t Archetype::GetHash() const
	{
		return mHash;
	}
}