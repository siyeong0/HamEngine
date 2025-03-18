module;

import Common;

export module ECS.Entity;

export namespace ham
{
	class Entity
	{
	public:
		Entity() = default;
		explicit Entity(uint32 id);
		~Entity() = default;

		Entity(const Entity& rhs);
		Entity(const Entity&& rhs);

		inline operator uint32();

		inline bool operator ==(const Entity& rhs) const;
		inline bool operator !=(const Entity& rhs) const;

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

	Entity::Entity(uint32 id)
		: mId(id)
	{
	}

	Entity::Entity(const Entity& rhs)
		: mId(rhs.mId)
	{
	}

	Entity::Entity(const Entity&& rhs)
		: mId(rhs.mId)
	{
	}

	Entity::operator uint32()
	{
		return mId;
	}

	bool Entity::operator ==(const Entity& rhs)  const
	{
		return mId == rhs.mId;
	}

	bool Entity::operator !=(const Entity& rhs)  const
	{
		return mId != rhs.mId;
	}

	inline uint32 Entity::GetId() const
	{
		return mId;
	}
}