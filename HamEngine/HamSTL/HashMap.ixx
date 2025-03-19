module;

import std.core;

export module HamSTL.HashMap;

export namespace ham
{
	template <typename K, typename V, typename Hash = std::hash<K>>
	using HashMap = std::unordered_map<K, V, Hash>;
}