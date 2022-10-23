/*!
file:	CharacterControllerSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for CharacterControllerSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CHARACTERCONTROLLER_SYSTEM_H
#define CHARACTERCONTROLLER_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		using namespace Component;

		/*!*********************************************************************************
			\brief
			Register Character Controller for ECS
		***********************************************************************************/
		void RegisterCharacterControllerSystem(void);

		/*!*********************************************************************************
			\brief
			Used at end of update, runs through each Character Controller
		***********************************************************************************/
		void UpdateCharacterControllerSystem(void);

		/*!*********************************************************************************
			\brief
			Adds Character Controller component to the entity
		***********************************************************************************/
		void CreateCharacterController(Entity const& entity);


		void OnCollisionEnter_Player(u32 otherEntity);
		void OnCollisionStay_Player(u32 otherEntity);
		void OnCollisionExit_Player(u32 otherEntity);
	}
}

#endif