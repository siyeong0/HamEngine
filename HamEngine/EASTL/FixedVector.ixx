module;

#include <EASTL\fixed_vector.h>

export module EASTL.FixedVector;

export namespace ham
{
	template <typename T, size_t numNode, bool bEnableOverflow = true>
	using FixedVector = eastl::fixed_vector<T, numNode, bEnableOverflow>;
}