module;

import std.core;

export module SolbitSTL.Queue;

export namespace solbit
{
	template <typename T>
	using Queue = std::queue<T>;
}