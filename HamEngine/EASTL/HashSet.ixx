module;

#include <EASTL\hash_set.h>

export module EASTL.HashSet;

export namespace ham
{
	template <typename T>
	using HashSet = eastl::hash_set<T>;
}