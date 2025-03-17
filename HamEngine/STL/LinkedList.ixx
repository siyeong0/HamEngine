module;

import Common;

export module STL.LinkedList;

export namespace ham
{
	template <typename T>
	class LinkedList
	{
	public:
		struct Node
		{
			T Data;
			Node* Next;
		};
	public:
		LinkedList();
		~LinkedList();

		LinkedList(const LinkedList&) = delete;
		LinkedList(const LinkedList&&) = delete;

		void Add(const T& val);
		void Remove(size_t idx);

		T& operator[] (size_t idx);

		inline size_t Size();

	private:
		Node* mHead;
		Node* mTail;
		size_t mSize;
	};

	template <typename T>
	LinkedList<T>::LinkedList()
		: mHead(nullptr)
		, mTail(nullptr)
		, mSize(0)
	{
		
	}

	template <typename T>
	LinkedList<T>::~LinkedList()
	{
		if (mSize == 0)
		{
			return;
		}

		Node* curr = mHead;
		for (size_t i = 0; i < mSize; ++i)
		{
			Node* next = curr->Next;
			Free<Node>(curr);
			curr = next;
		}
		ASSERT(curr == nullptr);
	}

	template <typename T>
	void LinkedList<T>::Add(const T& val)
	{
		if (mHead == nullptr)
		{
			mHead = Alloc<Node>();
			mHead->Data = val;
			mHead->Next = nullptr;

			mTail = mHead;
		}
		else
		{
			mTail->Next = Alloc<Node>();
			mTail = mTail->Next;

			mTail->Data = val;
			mTail->Next = nullptr;
		}
		++mSize;
	}

	template <typename T>
	void LinkedList<T>::Remove(size_t idx)
	{
		ASSERT(idx < mSize);

		if (idx == 0)	// idx == 0
		{
			Node* temp = mHead;
			mHead = mHead->Next;
			Free<Node>(temp);
		}
		else
		{
			Node* prev = nullptr;
			Node* curr = mHead;
			for (size_t i = 0; i < idx; ++i)
			{
				prev = curr;
				curr = curr->Next;
			}
			prev->Next = curr->Next;
			Free<Node>(curr);

			if (idx == mSize - 1)
			{
				mTail = prev;
			}
		}
		mSize--;
	}

	template <typename T>
	T& LinkedList<T>::operator[] (size_t idx)
	{
		ASSERT(idx < mSize);

		Node* curr = mHead;
		for (size_t i = 0; i < idx; ++i)
		{
			curr = curr->Next;
		}

		return curr->Data;
	}

	template <typename T>
	inline size_t LinkedList<T>::Size()
	{
		return mSize;
	}
}