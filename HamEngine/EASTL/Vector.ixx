module;

#include <EASTL\vector.h>

export module EASTL.Vector;

export namespace ham
{
	template <typename T>
	using Vector = eastl::vector<T>;
}