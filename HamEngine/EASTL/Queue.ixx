module;

#include <EASTL\queue.h>

export module EASTL.Queue;

export namespace ham
{
	template <typename T>
	using Queue = eastl::queue<T>;
}