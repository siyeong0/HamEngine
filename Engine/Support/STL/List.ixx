module;

import std.core;

export module SolbitSTL.List;

export namespace solbit
{
	template <typename T>
	using List = std::list<T>;
}