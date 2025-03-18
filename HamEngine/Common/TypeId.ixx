module;

import std.core;
import Common.Types;
import Common.Assert;

export module Common.TypeId;

export namespace ham
{
	template <typename T>
	class TypeId
	{
	public:
		static void Regist();
		static uint32 GetId();
		static size_t GetSize();
	private:
		static uint32 mId;
		static size_t mSize;
	};

}

namespace ham
{
	static uint32 sIdCount;
	static constexpr uint32 INVALID_TYPE_ID = std::numeric_limits<uint32>::max();

	template <typename T>
	uint32 TypeId<T>::mId = INVALID_TYPE_ID;

	template <typename T>
	size_t TypeId<T>::mSize = 0;

	template <typename T>
	void TypeId<T>::Regist()
	{
		ASSERT(INVALID_TYPE_ID == mId); // 두번 Regist 방지
		mId = sIdCount++;
		mSize = sizeof(T);
	}

	template <typename T>
	uint32 TypeId<T>::GetId()
	{
		ASSERT(INVALID_TYPE_ID != mId);
		return mId;
	}

	template <typename T>
	size_t TypeId<T>::GetSize()
	{
		ASSERT(mSize != 0);
		return mSize;
	}
}