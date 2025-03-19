module;

import std.core;

export module HamSTL.HashSet;

export namespace ham
{
	template <typename T>
	using HashSet = std::unordered_set<T>;
}