module;

#include <EASTL\stack.h>

export module EASTL.Stack;

export namespace ham
{
	template <typename T>
	using Stack = eastl::stack<T>;
}