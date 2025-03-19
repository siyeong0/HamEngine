module;

#include <EASTL\list.h>

export module EASTL.List;

export namespace ham
{
	template <typename T>
	using List = eastl::list<T>;
}