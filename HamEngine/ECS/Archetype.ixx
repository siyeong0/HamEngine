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
		Archetype(const Array<CompTypeId>& compTypes);
		~Archetype() = default;

		Archetype(const Archetype&) = delete;
		Archetype(const Archetype&&) = delete;

		inline bool operator==(const Archetype& rhs);
		inline bool operator!=(const Archetype& rhs);
		
		inline const Array<CompTypeId>& GetCompTypes() const;
		inline size_t GetNumCompTypes() const;
		inline size_t GetHash() const;

	private:
		Array<CompTypeId> mCompTypes;
		size_t mHash;
	};

	Archetype::Archetype(const Array<CompTypeId>& compTypes)
		: mCompTypes(compTypes)
		, mHash(mCompTypes.GenerateHash())
	{

	}

	inline bool Archetype::operator==(const Archetype& rhs)
	{
		return mHash == rhs.mHash;
	}

	inline bool Archetype::operator!=(const Archetype& rhs)
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