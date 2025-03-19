module;

import std.core;

export module HamSTL.List;

export namespace ham
{
	template <typename T>
	using List = std::list<T>;
}