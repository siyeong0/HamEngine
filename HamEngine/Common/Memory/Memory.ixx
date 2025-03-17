module;

import std.core;
import Common.Assert;

export module Common.Memory;

export namespace ham
{
	// TODO: malloc을 커스텀 메모리 관리자로 변경

	inline void* Alloc(size_t size)
	{
		return malloc(size);
	}

	inline void Free(void* ptr)
	{
		if (ptr == nullptr)
			return;
		free(ptr);
	}

	template<typename T, typename... Args>
	inline T* Alloc(Args&&... args)
	{
		T* ptr = static_cast<T*>(Alloc(sizeof(T)));	// TODO: malloc을 커스텀 메모리 관리자로 변경
		ASSERT(ptr != nullptr);
		new(ptr) T(std::forward<Args>(args)...);		// **placement new	p에 할당된 메모리에 생성자 호출

		return ptr;
	}

	template<typename T>
	inline T* Alloc(size_t numElements)
	{
		T* ptr = static_cast<T*>(Alloc(sizeof(T) * numElements));
		ASSERT(ptr != nullptr);
		for (int i = 0; i < numElements; i++)
			new(ptr + i) T();

		return ptr;
	}

	template<typename T>
	inline void Free(T* ptr)
	{
		if (ptr == nullptr)
			return;

		ptr->~T();
		Free(reinterpret_cast<void*>(ptr));

		return;
	}

	template<typename T>
	inline void Free(T* ptr, size_t numElements)
	{
		if (ptr == nullptr)
			return;

		for (int i = 0; i < numElements; i++)
			(ptr + i)->~T();
		Free(reinterpret_cast<void*>(ptr));

		return;
	}
}