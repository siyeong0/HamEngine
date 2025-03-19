module;

import std.core;

export module HamSTL.Queue;

export namespace ham
{
	template <typename T>
	using Queue = std::queue<T>;
}