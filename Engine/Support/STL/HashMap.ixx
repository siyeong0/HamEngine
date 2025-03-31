module;

import std.core;

export module SolbitSTL.HashMap;

export namespace solbit
{
	template <typename K, typename V, typename Hash = std::hash<K>, typename Equal = std::equal_to<K>>
	using HashMap = std::unordered_map<K, V, Hash, Equal>;
}