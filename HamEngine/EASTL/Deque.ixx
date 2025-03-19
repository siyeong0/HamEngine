module;

#include <EASTL\deque.h>

export module EASTL.Deque;

export namespace ham
{
	template <typename T>
	using Deque = eastl::deque<T>;
}