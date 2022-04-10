#ifndef	MEMORY_H
#define MEMORY_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Memory
	{
		u64 const MEMORY_BUFFER = 2'097'152;
		struct Bookmark
		{
			void *head{ nullptr }, *tail{ nullptr };
		};

		/*********************************************************************************
										STATIC MEMORY
		*********************************************************************************/
		class StaticMemory
		{
		public:
			template <typename T>
			static T* New(u64 size = 1)
			{
				u64 const index = mIndex; mIndex += sizeof(T) * size;
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
				return new (&*(mPtr + index)) T();
			}

			/*!*********************************************************************************
				\brief
				Calls the destructor of ptr and set the ptr to nullptr
				(Function should only be called when unloading object)
				\param [in,out] ptr:
				Pointer to have it's destructor be called and ptr set to null so that it's
				no longer storing a memory address
			***********************************************************************************/
			template <typename T>
			static void Delete(T*& ptr)
			{
				ptr->~T();
				ptr = nullptr;
			}

		private:
			StaticMemory(void) = default;
			~StaticMemory(void) = default;

			static void Reset(void);
			static void FreeAll(void);

			friend void Reset(void);
			friend void FreeAll(void);

			static char* const mPtr;
			static u64 mIndex;	// Storing the index of the last element
		};

		/*********************************************************************************
										DYNAMIC MEMORY
		*********************************************************************************/
		class DynamicMemory
		{
		public:
			template <typename T>
			static T* New(u64 size = 1)
			{
				Bookmark free_bm, allo_bm; u64 const SIZE = sizeof(T) * size; u64 bytesBetweenBookmark{ 0 };
				u64 index = GetIndex(mFreed, free_bm, bytesBetweenBookmark, SIZE);
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
				FindBookmark(mAllocated, allo_bm);
				if (bytesBetweenBookmark)
				{
					free_bm.head = mPtr + index + SIZE;
					if (SIZE == bytesBetweenBookmark)
						free_bm.head = nullptr, free_bm.tail = nullptr;
				}
				allo_bm.head = mPtr + index, allo_bm.tail = mPtr + index + SIZE - 1;
				return new (&*(mPtr + index)) T();
			}

			template <typename T>
			static void Delete(T*& ptr)
			{

			}

		private:
			DynamicMemory(void) = default;
			~DynamicMemory(void) = default;

			static u64 GetIndex(Bookmark const* member, Bookmark& store, u64& elementsBetweenBookmark, u64 size);
			static void FindBookmark(Bookmark const* member, Bookmark& store);

			static void Reset(void);
			static void FreeAll(void);

			friend void Reset(void);
			friend void FreeAll(void);

			static char* const mPtr;
			static u64 const BOOKMARK_SIZE = 100; static u64 mIndex;
			static Bookmark mAllocated[BOOKMARK_SIZE], mFreed[BOOKMARK_SIZE];
		};

		void Reset(void);
		void FreeAll(void);
	}
}

#endif