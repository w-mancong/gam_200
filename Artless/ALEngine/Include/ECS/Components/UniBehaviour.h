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

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	UNI_BEHAVIOUR_H
#define UNI_BEHAVIOUR_H

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
		void Load(Entity en) {};

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(Entity en) {};

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(Entity en) {};

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		void LateUpdate(Entity en) {};

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		void Free(Entity en) {};

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		void Unload(Entity en) {};

		/*!*********************************************************************************
			\brief Pure virtual destructor 
		***********************************************************************************/
		virtual ~UniBehaviour(void) = 0 {};
	};
}

#endif