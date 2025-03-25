module;

import std.core;

export module SolbitSTL.Stack;

export namespace solbit
{
	template <typename T>
	using Stack = std::stack<T>;
}