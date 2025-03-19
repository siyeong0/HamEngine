module;

#include <EASTL\hash_map.h>

export module EASTL.HashMap;

export namespace ham
{
	template <typename K, typename V>
	using HashMap = eastl::hash_map<K, V>;
}