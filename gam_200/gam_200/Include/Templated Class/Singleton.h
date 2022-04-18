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
			static T* GetInstance()
			{
				if (!Singleton<T>::instance)
					Singleton<T>::instance = Memory::InstanceMemory::template New<T>();
				return Singleton<T>::instance;
			}

		protected:
			Singleton() = default;
			~Singleton() = default;

			Singleton(Singleton const&) = delete;
			Singleton& operator=(Singleton const&) = delete;
			Singleton(Singleton&&) = delete;
			Singleton& operator=(Singleton&&) = delete;

			/*!*********************************************************************************
				\brief
					Function to be overriden for any derived class with additional pointers
			***********************************************************************************/
			//virtual void free_memory();

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