module;

import std.core;

export module HamSTL.Utility;

export namespace ham
{
	template <typename T1, typename T2>
	using Pair = std::pair<T1, T2>;
}