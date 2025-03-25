module;

import std.core;

export module SolbitSTL.Array;

export namespace solbit
{
	template <typename T, size_t N = 1>
	using Array = std::array<T, N>;
}