/*!
file:	Singleton.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Template class for creating a Singleton
		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SINGLETON_H
#define SINGLETON_H

#include "pch.h"
namespace ManCong
{
	namespace Templates
	{
		// template class for any singleton design pattern
		template <typename T>
		class Singleton
		{
		public:

			/*!*********************************************************************************
				\brief
					Static function to return the instance of the this singleton class
			***********************************************************************************/
			static T* Instance()
			{
				if (!Singleton<T>::instance)
					Singleton<T>::instance = Memory::InstanceMemory::template New<T>();
				return Singleton<T>::instance;
			}

			/*!*********************************************************************************
				\brief
					Normal function to free instance pointer
			***********************************************************************************/
			static void Free(void)
			{
				Memory::InstanceMemory::Delete(instance);
			}

		protected:
			Singleton() = default;
			virtual ~Singleton() = default;

			Singleton(Singleton const&) = delete;
			Singleton& operator=(Singleton const&) = delete;
			Singleton(Singleton&&) = delete;
			Singleton& operator=(Singleton&&) = delete;

			static T* instance;
		};

		template<typename T>
		T* Singleton<T>::instance = nullptr;

		//template <typename T>
		//void Singleton<T>::free_memory() // base function to delete instance from heap
		//{
		//	if (Singleton<T>::instance)
		//	{
		//		delete Singleton<T>::instance;
		//		Singleton<T>::instance = nullptr;
		//	}
		//}
	}
}

#endif