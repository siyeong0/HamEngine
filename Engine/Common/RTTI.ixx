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
		static ID GetId();
	private:
		static ID mId;
	};

}

namespace solbit
{
	static ID sIdCount;
	static constexpr ID INVALID_TYPE_ID = std::numeric_limits<ID>::max();

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