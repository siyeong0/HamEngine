module;

import std.core;
import Common;

export module STL.Array;

export namespace ham
{
	template <typename T>
	class Array
	{
	public:
		Array(size_t capacity);
		~Array();

		Array(const Array& rhs);
		Array(const Array&& rhs);

		T& operator[](size_t idx);

		inline size_t Capacity() const;

		size_t GenerateHash() const;

	private:
		T* mData;
		const size_t mCapacity;
	};

	template <typename T>
	Array<T>::Array(size_t size)
		: mData(Alloc<T>(size))
		, mCapacity(size)
	{

	}

	template <typename T>
	Array<T>::~Array()
	{
		Free<T>(mData, mCapacity);
	}

	template <typename T>
	Array<T>::Array(const Array& rhs)
		: mData(Alloc<T>(mCapacity))
		, mCapacity(rhs.mCapacity)
	{
		std::memcpy(mData, rhs.mData, sizeof(T) * mCapacity);
	}

	template <typename T>
	Array<T>::Array(const Array&& rhs)
		: mData(rhs.mData)
	{
		rhs.mData = nullptr;
	}

	template <typename T>
	T& Array<T>::operator[](size_t idx)
	{
		ASSERT(idx < mCapacity);
		return mData[idx];
	}

	template <typename T>
	inline size_t Array<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	size_t Array<T>::GenerateHash() const
	{
		size_t hash = mCapacity;
		for (int i = 0; i < mCapacity; ++i)
		{
			T& val = mData[i];
			hash ^= val + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}
}