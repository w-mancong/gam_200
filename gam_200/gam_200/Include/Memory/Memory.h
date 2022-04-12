/*!
file:	Memory.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Memory Arena. This class handles memory allocation and deallocation by making
		sure that memory is always streamline

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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
			/*!*********************************************************************************
			\brief
				Allocate memory address from a fixed memory buffer of a specified size
			\param [in] size:
				Total number of elements. Similar to int arr[10]
			\return
				The starting address pointing to the first element
		***********************************************************************************/
			template <typename T>
			static T* New(u64 size = 1)
			{
				Bookmark* bm = (mBookmarks + mBookmarkIndex++); u64 const index = mIndex; mIndex += sizeof(T) * size;
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
				bm->head = mPtr + index, bm->tail = mPtr + mIndex - 1;
				return new (&*(mPtr + index)) T();
			}

			/*!*********************************************************************************
				\brief
				Calls the destructor of ptr and set ptr to nullptr
				(Function should only be called when unloading object)
				\param [in,out] ptr:
				Pointer to have it's destructor be called and ptr set to null so that it's
				no longer storing a memory address
			***********************************************************************************/
			template <typename T>
			static void Delete(T*& ptr)
			{
				Bookmark* bm; FindBookmark(bm, ptr);
				u64 const TOTAL_ELEMENTS = (static_cast<char*>(bm->tail) - static_cast<char*>(bm->head) + 1) / sizeof(T);
				for(u64 i = 0; i < TOTAL_ELEMENTS; ++i)
					(ptr + i)->~T();
				ptr = nullptr;
			}

		private:
			StaticMemory(void) = default;
			~StaticMemory(void) = default;

			static void FindBookmark(Bookmark*& bm, void* ptr);

			static void Reset(void);
			static void FreeAll(void);

			friend void Reset(void);
			friend void FreeAll(void);

			static char* const mPtr;
			static u64 const BOOKMARK_SIZE = 100; static u64 mIndex, mBookmarkIndex;	// Storing the index of the last element
			static Bookmark mBookmarks[BOOKMARK_SIZE];
		};

		/*********************************************************************************
										DYNAMIC MEMORY
		*********************************************************************************/
		class DynamicMemory
		{
		public:
			/*!*********************************************************************************
				\brief
					Allocate memory address from a fixed memory buffer of a specified size
				\param [in] size:
					Total number of elements. Similar to int arr[10]
				\return 
					The starting address pointing to the first element
			***********************************************************************************/
			template <typename T>
			static T* New(u64 size = 1)
			{
				Bookmark *free_bm = nullptr, *allo_bm = nullptr; u64 const SIZE = sizeof(T) * size; u64 bytesBetweenBookmark{ 0 };
				u64 index = GetIndex(mFreed, free_bm, bytesBetweenBookmark, SIZE);	// Get index of where mAllocated head shld be pointing to
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
				FindBookmark(mAllocated, allo_bm);
				if (bytesBetweenBookmark)					// there is enough space in between the memory stream
				{
					free_bm->head = mPtr + index + SIZE;	// update the pointer where mFreed head is pointing
					if (SIZE == bytesBetweenBookmark)
						free_bm->head = nullptr, free_bm->tail = nullptr;	// There is enough space in between the memory stream
				}
				allo_bm->head = mPtr + index, allo_bm->tail = mPtr + index + SIZE - 1;	// update mAllocated's bookmark
				return new (&*(mPtr + index)) T();
			}

			/*!*********************************************************************************
				\brief
				Calls the destructor of ptr and set ptr to nullptr
				(Function can be called anytime during runtime when neccessary)
				\param [in,out] ptr:
				Pointer to have it's destructor be called and ptr set to null so that it's
				no longer storing a memory address
			***********************************************************************************/
			template <typename T>
			static void Delete(T*& ptr)
			{
				Bookmark *free_bm = nullptr, *allo_bm = nullptr;
				FindBookmark(mFreed, free_bm);		 // Find a bookmark in freed that is not used
				FindAllocatedBookmark(allo_bm, ptr); // Find the bookmark containing ptr
				// Calculate the total elements allocated for ptr
				u64 const TOTAL_ELEMENTS = (static_cast<char*>(allo_bm->tail) - static_cast<char*>(allo_bm->head) + 1) / sizeof(T);
				// Call the destructor of the class
				for (u64 i = 0; i < TOTAL_ELEMENTS; ++i)
					(ptr + i)->~T();
				ptr = nullptr; // Set the pointer to be nullptr
				free_bm->head = allo_bm->head, free_bm->tail = allo_bm->tail;
				allo_bm->head = nullptr, allo_bm->tail = nullptr; UpdateIndex();
			}

		private:
			DynamicMemory(void) = default;
			~DynamicMemory(void) = default;

			static u64 GetIndex(Bookmark* member, Bookmark*& bm, u64& elementsBetweenBookmark, u64 size);
			static void FindBookmark(Bookmark* member, Bookmark*& bm);
			static void FindAllocatedBookmark(Bookmark*& bm, void* const ptr);
			static void UpdateIndex(void);

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