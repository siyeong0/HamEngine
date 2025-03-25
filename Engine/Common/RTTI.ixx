module;

import std.core;
import Common.Types;
import Common.Assert;

export module Common.RTTI;

export namespace solbit
{
	template <typename T>
	class RTTI
	{
	public:
		static void Regist();
		static Id GetId();
	private:
		static Id mId;
	};

}

namespace solbit
{
	static Id sIdCount;
	static constexpr Id INVALID_TYPE_ID = std::numeric_limits<Id>::max();

	template <typename T>
	uint32 RTTI<T>::mId = INVALID_TYPE_ID;

	template <typename T>
	void RTTI<T>::Regist()
	{
		ASSERT(INVALID_TYPE_ID == mId); // 두번 Regist 방지
		mId = sIdCount++;
	}

	template <typename T>
	uint32 RTTI<T>::GetId()
	{
		ASSERT(INVALID_TYPE_ID != mId);
		return mId;
	}
}