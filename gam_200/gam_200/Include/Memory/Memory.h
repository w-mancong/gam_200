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
		u64 constexpr MEMORY_BUFFER = 2'097'152, INSTANCE_MEMORY_BUFFER = 524'228;
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
				Bookmark* bm = (m_Bookmarks + m_BookmarkIndex++); u64 const index = m_Index; m_Index += sizeof(T) * size;
#ifdef _DEBUG
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
#endif
				bm->head = m_Ptr + index, bm->tail = m_Ptr + m_Index - 1;
				return new (&*(m_Ptr + index)) T();
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

			static char* const m_Ptr;
			static u64 constexpr BOOKMARK_SIZE = 100; static u64 m_Index, m_BookmarkIndex;	// Storing the index of the last element
			static Bookmark m_Bookmarks[BOOKMARK_SIZE];
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
				u64 index = GetIndex(m_Freed, free_bm, bytesBetweenBookmark, SIZE);	// Get index of where mAllocated head shld be pointing to
#ifdef _DEBUG
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
#endif
				FindBookmark(m_Allocated, allo_bm);
				if (bytesBetweenBookmark)					// there is enough space in between the memory stream
				{
					free_bm->head = m_Ptr + index + SIZE;	// update the pointer where mFreed head is pointing
					if (SIZE == bytesBetweenBookmark)
						free_bm->head = nullptr, free_bm->tail = nullptr;	// There is enough space in between the memory stream
				}
				// update mAllocated's bookmark, then sort mAllocated bookmarks
				allo_bm->head = m_Ptr + index, allo_bm->tail = m_Ptr + index + SIZE - 1; Sort(m_Allocated);
				return new (&*(m_Ptr + index)) T();
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
				FindBookmark(m_Freed, free_bm);		 // Find a bookmark in freed that is not used
				FindAllocatedBookmark(allo_bm, ptr); // Find the bookmark containing ptr
				u64 const TOTAL_BYTES = static_cast<char*>(allo_bm->tail) - static_cast<char*>(allo_bm->head) + 1;
				// Calculate the total elements allocated for ptr
				u64 const TOTAL_ELEMENTS = TOTAL_BYTES / sizeof(T);
				// Call the destructor of the class
				for (u64 i = 0; i < TOTAL_ELEMENTS; ++i)
					(ptr + i)->~T();
				for (u64 i = static_cast<char*>(allo_bm->head) - m_Ptr; i < TOTAL_BYTES; ++i)
					*(m_Ptr + i) = '\0';
				ptr = nullptr; // Set the pointer to be nullptr
				free_bm->head = allo_bm->head, free_bm->tail = allo_bm->tail; UpdateFreedBookmark(free_bm);
				allo_bm->head = nullptr, allo_bm->tail = nullptr; UpdateIndex();
			}

		private:
			DynamicMemory(void) = default;
			~DynamicMemory(void) = default;

			static u64 GetIndex(Bookmark* member, Bookmark*& bm, u64& elementsBetweenBookmark, u64 size);
			static void FindBookmark(Bookmark* member, Bookmark*& bm);
			static void FindAllocatedBookmark(Bookmark*& bm, void* const ptr);
			static void UpdateFreedBookmark(Bookmark*& bm);
			static void UpdateIndex(void);
			static void Sort(Bookmark* member);

			static void Reset(void);
			static void FreeAll(void);

			friend void Reset(void);
			friend void FreeAll(void);

			static char* const m_Ptr;
			static u64 constexpr BOOKMARK_SIZE = 100; static u64 m_Index;
			static Bookmark m_Allocated[BOOKMARK_SIZE], m_Freed[BOOKMARK_SIZE];
		};

		/*********************************************************************************
										INSTANCE MEMORY
		*********************************************************************************/
		/*!*********************************************************************************
		\brief
			InstanceMemory class use to instantiate memory for Singleton and derieved classes
		***********************************************************************************/
		class InstanceMemory
		{
		public:
		/*!*********************************************************************************
			\brief
				Allocate memory address from a fixed memory buffer of a specified size
				Use this class to instantiate any memory for any Singleton classes
			\return
				The starting address pointing to the first element
		***********************************************************************************/
			template <typename T>
			static T* New(void)
			{
				u64 const index = m_Index; m_Index = m_Index += sizeof(T);
#ifdef _DEBUG
				assert(index < MEMORY_BUFFER && "Size of memory buffer is too small. Change the size of MEMORY_BUFFER");
#endif
				return new (&*(m_Ptr + index)) T();
			}

		private:
			InstanceMemory(void) = default;
			~InstanceMemory(void) = default;

			static void FreeAll(void);

			friend void FreeAll(void);

			static char* const m_Ptr; static u64 m_Index;
		};

		void Reset(void);
		void FreeAll(void);
	}
}

#endif