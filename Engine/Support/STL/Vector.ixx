module;

import std.core;

export module SolbitSTL.Vector;

export namespace solbit
{
	template <typename T>
	using Vector = std::vector<T>;
}