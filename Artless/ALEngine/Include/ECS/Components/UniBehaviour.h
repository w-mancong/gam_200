/*!
file:	UniBehaviour.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the class declaration for UniBehaviour, a component for logic system

		The logic system will follow the following game loop.
		Game loop:
		while(...)
			if(current != restart)
				Load();

			Init();
			while(...)
				Update();
				LateUpdate();

			Free();
			if(next != restart)
				Unload();	

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	UNI_BEHAVIOUR_H
#define UNI_BEHAVIOUR_H

#include <ECS/Structure/entity.h>
#include <rttr\registration>
#include <rttr\policy.h>

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		A component class for logic system
	***********************************************************************************/
	class alignas(double) UniBehaviour 
	{
	public:
		/*!*********************************************************************************
			\brief Used to load in any resources, will only run once when new scene loads
		***********************************************************************************/
		virtual void Load([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		virtual void Init([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		virtual void Update([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		virtual void LateUpdate([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		virtual void Free([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		virtual void Unload([[maybe_unused]] Entity en) {};

		/*!*********************************************************************************
			\brief Pure virtual destructor 
		***********************************************************************************/
		virtual ~UniBehaviour(void) = 0 {};

		// For RTTR
		virtual void DeserializeComponent([[maybe_unused]] Entity en) {};

		RTTR_ENABLE()
	};
}

#endif