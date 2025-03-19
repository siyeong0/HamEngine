module;

import std.core;

export module HamSTL.Array;

export namespace ham
{
	template <typename T, size_t N = 1>
	using Array = std::array<T, N>;
}