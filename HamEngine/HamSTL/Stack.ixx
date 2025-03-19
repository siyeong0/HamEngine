module;

import std.core;

export module HamSTL.Stack;

export namespace ham
{
	template <typename T>
	using Stack = std::stack<T>;
}