module;

import std.core;
import Common;
import Memory.Alloc;

export module Memory.Page;

export namespace solbit
{
	template <typename T>
	class Page
	{
	public:
		Page(const size_t pageSize);
		~Page();

		T* AllocBlock();
		void FreeBlock(T* ptrBlock);

#ifdef _DEBUG
		void DbgPrint();
#endif
	private:
		T* mBaseAddress;
		uint16 mNextAllocIndex; //�ֿ켱 �Ҵ� �� �ּ�
		const size_t mPageSize;
		const size_t mNumBlocksPerPage;
	};
}

namespace solbit
{

	static constexpr uint16 END_INDEX = std::numeric_limits<uint16>::max();

	template <typename T>
	Page<T>::Page(const size_t pageSize)
		: mBaseAddress(static_cast<T*>(Alloc(pageSize)))
		, mNextAllocIndex(0)
		, mPageSize(pageSize)
		, mNumBlocksPerPage(pageSize / sizeof(T))
	{
		ASSERT(pageSize % sizeof(T) == 0);
		static_assert(sizeof(T) >= sizeof(uint16));

		std::memset(mBaseAddress, 0, pageSize);

		T* iterPtr = mBaseAddress;
		for (size_t i = 0; i < mNumBlocksPerPage - 1; ++i) //������ ���� �� �ּ� �Ҵ�(���� ����Ʈ)
		{
			uint16* uintPtr = reinterpret_cast<uint16*>(iterPtr++);
			*uintPtr = static_cast<uint16>(i + 1);
		}
		uint16* uintPtr = reinterpret_cast<uint16*>(iterPtr);
		*uintPtr = END_INDEX;
	}

	template <typename T>
	Page<T>::~Page()
	{
		ASSERT(mBaseAddress != nullptr);
		Free<T>(mBaseAddress, mNumBlocksPerPage);
	}

	template <typename T>
	T* Page<T>::AllocBlock()
	{
		if (mNextAllocIndex == END_INDEX)
		{
			return nullptr;
		}

		T* allocedBlockPtr = mBaseAddress + mNextAllocIndex; 

		mNextAllocIndex = *(reinterpret_cast<uint16*>(allocedBlockPtr));

		new(allocedBlockPtr) T();

		return allocedBlockPtr;
	}

	template <typename T>
	void Page<T>::FreeBlock(T* ptrBlock)
	{
		*(reinterpret_cast<uint16*>(ptrBlock)) = mNextAllocIndex;

		size_t decPtrBlock = reinterpret_cast<uint64>(ptrBlock);
		size_t decBaseAddress = reinterpret_cast<uint64>(mBaseAddress);

		ASSERT(mPageSize > decPtrBlock - decBaseAddress, "abc");
		mNextAllocIndex = static_cast<uint16>((decPtrBlock - decBaseAddress)/sizeof(T));
	}

#ifdef _DEBUG
	template <typename T>
	void Page<T>::DbgPrint()
	{

		std::set<uint16> setNotAllocIndex;

		uint16 notAllocIndex = mNextAllocIndex;
		while (notAllocIndex != END_INDEX)
		{
			setNotAllocIndex.insert(notAllocIndex);
			notAllocIndex = *(reinterpret_cast<uint16*>(mBaseAddress + notAllocIndex));
		}

		std::cout << "Next Alloc Index: " << mNextAllocIndex << std::endl;

		for (uint16 i = 0; i < mNumBlocksPerPage; ++i)
		{
			if (setNotAllocIndex.find(i) != setNotAllocIndex.end())
			{
				std::cout << std::setw(3) << *(reinterpret_cast<uint16*>(mBaseAddress + i));
			}
			else
			{
				std::cout << std::setw(3) << '#';
			}
			if ((i + 1) % 10 == 0) {
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
#endif
}

