module;

#include <EASTL\array.h>

export module EASTL.Array;

export namespace ham
{
	template <typename T>
	using Array = eastl::array<T>;
}