/*!
file:	Memory.cpp
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Memory Arena. This class handles memory allocation and deallocation by making
		sure that memory is always streamline

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#include "Engine/Manager/MeshBuilder.h"

namespace ALEngine
{
	namespace Memory
	{
		/*********************************************************************************
										STATIC MEMORY
		*********************************************************************************/
		Bookmark StaticMemory::m_Bookmarks[]{ nullptr }; u64 StaticMemory::m_Index{ 0 }, StaticMemory::m_BookmarkIndex{ 0 };
		char* const StaticMemory::m_Ptr = new char[MEMORY_BUFFER] {};

		/*!*********************************************************************************
			\brief
				Find the bookmark that has the same address as ptr. This is so that
				I can calculate the total elements that was allocated for ptr
			\param [out] bm:
				Using this variable to store the address where the bookmark has the address
				of ptr
			\param [in] ptr:
				Finding the bookmark with the address of ptr
		***********************************************************************************/
		void StaticMemory::FindBookmark(Bookmark*& bm, void* ptr)
		{
			bm = std::find_if(m_Bookmarks, (m_Bookmarks + BOOKMARK_SIZE), [=](auto const& cmp)
			{
				return cmp.head == ptr;
			});
		}

		/*!*********************************************************************************
		\brief
				Call the destructor of all the singleton classes
		***********************************************************************************/
		void FreeInstance(void)
		{
			Coordinator::Free();
			Editor::ALEditor::Free();
			Engine::MeshBuilder::Free();
		}

		/*!*********************************************************************************
			\brief
				Deallocate the memory allocated inside the heap for mPtr
		***********************************************************************************/
		void StaticMemory::FreeAll(void)
		{
			FreeInstance();
			delete[] m_Ptr;
		}

		/*********************************************************************************
										DYNAMIC MEMORY
		*********************************************************************************/
		Bookmark DynamicMemory::m_Allocated[]{ nullptr }, DynamicMemory::m_Freed[]{ nullptr }; u64 DynamicMemory::m_Index = 0;
		char* const DynamicMemory::m_Ptr = new char[MEMORY_BUFFER] {};

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
					return static_cast<char*>(bm->head) - m_Ptr;
			}
			// No free space in between the memory stream, allocate memory at the end instead
			u64 const index = m_Index; m_Index += SIZE, bytesBetweenBookmark = 0;
			return index;
		}

		/*!*********************************************************************************
			\brief
				Helper function to find the next avaliable Bookmark
			\param [in] member:
				Either pass in mAllocated or mFreed
			\param [out] bm:
				Bookmark that is storing the address of the avaliable Bookmark in either
				mAllocated or mFreed
		***********************************************************************************/
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

		/*!*********************************************************************************
			\brief
				Find the bookmark that has the same address as ptr. This is so that
				I can calculate the total elements that was allocated for ptr
			\param [out] bm:
				Using this variable to store the address where the bookmark has the address
				of ptr
			\param [in] ptr:
				Finding the bookmark with the address of ptr
		***********************************************************************************/
		void DynamicMemory::FindAllocatedBookmark(Bookmark*& bm, void* const ptr)
		{
			bm = std::find_if(m_Allocated, (m_Allocated + BOOKMARK_SIZE), [=](auto const& cmp)
			{
				return cmp.head == ptr;
			});
		}

		/*!*********************************************************************************
			\brief
				Update mFreed Bookmarks such that if there are two bookmarks side by side,
				it will be combined into one bookmark instead
		***********************************************************************************/
		void DynamicMemory::UpdateFreedBookmark(Bookmark*& bm)
		{
			for (u64 i = 0; i < BOOKMARK_SIZE; ++i)
			{
				u64 const s1 = static_cast<char*>((m_Freed + i)->head) - static_cast<char*>(bm->tail);
				u64 const meshShader = static_cast<char*>(bm->head) - static_cast<char*>((m_Freed + i)->tail);
				if (1 != s1 && 1 != meshShader) continue;
				// When it's here, means that the two bookmarks are side by side
				(m_Freed + i)->head = std::min((m_Freed + i)->head, bm->head);
				(m_Freed + i)->tail = std::max((m_Freed + i)->tail, bm->tail); 
				bm->head = nullptr, bm->tail = nullptr; break;
			}
			Sort(m_Freed);
		}

		/*!*********************************************************************************
			\brief
				Update mIndex so that if any deletion happens, it will always be pointing
				to the end of the next available byte in the memory stream
		***********************************************************************************/
		void DynamicMemory::UpdateIndex(void)
		{
			// Finding the last allocated tail
			auto max_allo = std::max_element(m_Allocated, (m_Allocated + BOOKMARK_SIZE), [](auto const& lhs, auto const& rhs)
			{
				//if (!lhs.tail || !rhs.tail) return false;
				return lhs.tail < rhs.tail;
			});
			// Finding the first free head
			auto min_free = std::min_element(m_Freed, (m_Freed + BOOKMARK_SIZE), [](auto const& lhs, auto const& rhs)
			{
				if (!lhs.head || !rhs.head) return false;
				return lhs.head < rhs.head;
			});
			// if freed.head > allocated.tail, update mIndex to freed.head
			if (min_free->head > max_allo->tail)
			{
				m_Index = static_cast<char*>(min_free->head) - m_Ptr;
				// fill mFreed with default Bookmark value
				std::fill(m_Freed, (m_Freed + BOOKMARK_SIZE), Bookmark()); return;
			}
			// if mIndex - allocated.tail > 1, move mIndex to 1 after allocated.tail
			u64 const indexFromTail = static_cast<char*>(max_allo->tail) - m_Ptr;
			if (1 < m_Index - indexFromTail)
			{
				m_Index = indexFromTail + 1;
				// Set all bookmakrs after mIndex to default values
				void* ptr = m_Ptr + m_Index;
				auto it = std::find_if(m_Freed, (m_Freed + BOOKMARK_SIZE), [=](auto const& cmp)
				{
					return cmp.head == ptr;
				});
				std::fill(it, (m_Freed + BOOKMARK_SIZE), Bookmark()); return;
			}
		}

		/*!*********************************************************************************
			\brief
				Sorting mFreed or mAllocated bookmarks from ascending order
		***********************************************************************************/
		void DynamicMemory::Sort(Bookmark* member)
		{
			std::sort(member, (member + BOOKMARK_SIZE), [](auto const& lhs, auto const& rhs)
			{
				return lhs.head < rhs.head;
			});
			for (u64 i = 0, j = 0; i < BOOKMARK_SIZE; ++i)
			{
				if (!(member + i)->head) continue;
				std::swap(*(member + j++), *(member + i));
			}
		}

		/*!*********************************************************************************
			\brief
				Reset DynamicMemory class to it's default state
		***********************************************************************************/
		void DynamicMemory::Reset(void)
		{
			std::fill(m_Ptr, (m_Ptr + MEMORY_BUFFER), '\0');
			std::fill(m_Allocated, (m_Allocated + BOOKMARK_SIZE), Bookmark());
			std::fill(m_Freed, (m_Freed + BOOKMARK_SIZE), Bookmark());
			m_Index = 0;
		}

		/*!*********************************************************************************
			\brief
				Deallocate the memory allocated inside the heap for mPtr
		***********************************************************************************/
		void DynamicMemory::FreeAll(void)
		{
			delete[] m_Ptr;
		}

		/*!*********************************************************************************
			\brief
				Helper function to reset both StaticMemory and DynamicMemory to it's 
				default state
		***********************************************************************************/
		void Reset(void)
		{
			DynamicMemory::Reset();
		}

		/*!*********************************************************************************
			\brief
				Helper function to deallocate the memory for StaticMemory, DynamicMemory and InstanceMemory
		***********************************************************************************/
		void FreeAll(void)
		{
			StaticMemory::FreeAll();
			DynamicMemory::FreeAll();
		}
	}
}