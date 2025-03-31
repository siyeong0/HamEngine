module;

import Common;
import Memory;

export module ECS.Entity;

export namespace solbit
{
	class Entity
	{
	public:
		Entity() = default;
		explicit Entity(uint32 id);
		~Entity() = default;

		Entity(const Entity& other);
		Entity(const Entity&& other);
		Entity& operator=(const Entity& other);

		inline operator uint32();

		inline bool operator ==(const Entity& other) const;
		inline bool operator !=(const Entity& other) const;

		inline uint32 GetId() const;

	private:
		uint32 mId;
	};

	class EntityHash
	{
	public:
		std::size_t operator()(const Entity& ent) const
		{
			return static_cast<std::size_t>(ent.GetId());
		}
	};
}

namespace solbit
{
	Entity::Entity(uint32 id)
		: mId(id)
	{
	}

	Entity::Entity(const Entity& other)
		: mId(other.mId)
	{
	}

	Entity::Entity(const Entity&& other)
		: mId(other.mId)
	{
	}

	Entity& Entity::operator=(const Entity& other)
	{
		mId = other.mId;
		return *this;
	}

	Entity::operator uint32()
	{
		return mId;
	}

	bool Entity::operator ==(const Entity& other)  const
	{
		return mId == other.mId;
	}

	bool Entity::operator !=(const Entity& other)  const
	{
		return mId != other.mId;
	}

	inline uint32 Entity::GetId() const
	{
		return mId;
	}
}