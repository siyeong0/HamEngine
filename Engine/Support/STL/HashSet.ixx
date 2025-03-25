module;

import std.core;

export module SolbitSTL.HashSet;

export namespace solbit
{
	template <typename T>
	using HashSet = std::unordered_set<T>;
}