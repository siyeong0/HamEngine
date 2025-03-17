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

		inline bool operator ==(const Entity& rhs);
		inline bool operator !=(const Entity& rhs);

	private:
		uint32 mId;
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

	bool Entity::operator ==(const Entity& rhs)
	{
		return mId == rhs.mId;
	}

	bool Entity::operator !=(const Entity& rhs)
	{
		return mId != rhs.mId;
	}
}