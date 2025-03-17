module;

import ECS.Archetype;

export module ECS.ArchetypeChunk;

export namespace ham
{
	class ArchetypeChunk
	{
	public:
		ArchetypeChunk();
		~ArchetypeChunk();

		ArchetypeChunk(const ArchetypeChunk&);
		ArchetypeChunk(const ArchetypeChunk&&);


	private:
		Archetype mArchetype;
		size_t mCapacity;
		size_t mEntityCount;
	};
}