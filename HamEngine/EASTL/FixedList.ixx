module;

#include <EASTL\fixed_list.h>

export module EASTL.FixedList;

export namespace ham
{
	template <typename T, size_t numNode>
	using FixedList = eastl::fixed_list<T, numNode>;
}