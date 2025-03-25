module;

import std.core;

export module SolbitSTL.HashMap;

export namespace solbit
{
	template <typename K, typename V, typename Hash = std::hash<K>>
	using HashMap = std::unordered_map<K, V, Hash>;
}