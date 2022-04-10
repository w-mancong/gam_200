#include "pch.h"

namespace ManCong
{
	namespace Memory
	{
		/*********************************************************************************
										STATIC MEMORY
		*********************************************************************************/
		Bookmark StaticMemory::mBookmarks[]{ nullptr }; u64 StaticMemory::mIndex{ 0 }, StaticMemory::mBookmarkIndex{ 0 };
		char* const StaticMemory::mPtr = new char[MEMORY_BUFFER];

		void StaticMemory::FindBookmark(Bookmark*& bm, void* ptr)
		{
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)
			{
				if (ptr != (mBookmarks + i)->head)
					continue;
				bm = (mBookmarks + i); break;
			}
		}

		void StaticMemory::Reset(void)
		{
			for (u64 i = 0; i < MEMORY_BUFFER; ++i)
				*(mPtr + i) = '\0';
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)
				(mBookmarks + i)->head = nullptr, (mBookmarks + i)->tail = nullptr;
			mIndex = 0;
		}

		void StaticMemory::FreeAll(void)
		{
			delete[] mPtr;
		}

		/*********************************************************************************
										DYNAMIC MEMORY
		*********************************************************************************/
		Bookmark DynamicMemory::mAllocated[]{ nullptr }, DynamicMemory::mFreed[]{ nullptr }; u64 DynamicMemory::mIndex = 0;
		char* const DynamicMemory::mPtr = new char[MEMORY_BUFFER];

		/*!*********************************************************************************
			\brief
			Get the index to the first available address
			\param [in] member:
			Member array of bookmarks in DynamicMemory class
			\param [out] bm:
			Variable to store the available bookmark
			\param [out] bytesBetweenBookmark:
			Total bytes between the bookmark
			\param [in] SIZE:
			Size in byte that it is trying to allocate the memory for
		***********************************************************************************/
		u64 DynamicMemory::GetIndex(Bookmark* member, Bookmark*& bm, u64& bytesBetweenBookmark, u64 SIZE)
		{
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)	// Find any bookmark that have enough bytes to store the memory
			{
				if (!(member + i)->head)
					continue;
				bm = (member + i);
				bytesBetweenBookmark = static_cast<char*>(bm->tail) - static_cast<char*>(bm->head) + 1;
				if (SIZE <= bytesBetweenBookmark)
					return static_cast<char*>(bm->head) - mPtr;
			}
			// No free space in between the memory stream, allocate memory at the end instead
			u64 const index = mIndex; mIndex += SIZE, bytesBetweenBookmark = 0;
			return index;
		}

		void DynamicMemory::FindBookmark(Bookmark* member, Bookmark*& bm)
		{
			u64 i = 0;
			do {
				if ((member + i)->head)
					continue;
				bm = (member + i); return;
			} while (++i < BOOKMARK_SIZE);
			assert(i < BOOKMARK_SIZE && "Increase bookmark size");
		}

		void DynamicMemory::FindAllocatedBookmark(Bookmark*& bm, void* ptr)
		{
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)
			{
				if (ptr != (mAllocated + i)->head)
					continue;
				bm = (mAllocated + i); break;
			}
		}

		void DynamicMemory::Reset(void)
		{
			for (u64 i = 0; i < MEMORY_BUFFER; ++i)
				*(mPtr + i) = '\0';
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)
				(mAllocated + i)->head = nullptr, (mAllocated + i)->tail = nullptr, (mFreed + i)->head = nullptr, (mFreed + i)->tail = nullptr;
			mIndex = 0;
		}

		void DynamicMemory::FreeAll(void)
		{
			delete[] mPtr;
		}

		void Reset(void)
		{
			StaticMemory::Reset();
			DynamicMemory::Reset();
		}

		void FreeAll(void)
		{
			StaticMemory::FreeAll();
			DynamicMemory::FreeAll();
		}
	}
}