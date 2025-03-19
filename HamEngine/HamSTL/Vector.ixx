module;

import std.core;

export module HamSTL.Vector;

export namespace ham
{
	template <typename T>
	using Vector = std::vector<T>;
}