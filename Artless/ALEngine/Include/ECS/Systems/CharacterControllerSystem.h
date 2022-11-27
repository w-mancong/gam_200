/*!
file:   CharacterControllerSystem.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for CharacterControllerSystem.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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

		/*!*********************************************************************************
			\brief
			Collsion Enter Event To Subscribe for player
		***********************************************************************************/
		void OnCollisionEnter_Player(u32 currentEntity, u32 otherEntity);

		/*!*********************************************************************************
			\brief
			Collsion Stay Event To Subscribe for player
		***********************************************************************************/
		void OnCollisionStay_Player(u32 currentEntity, u32 otherEntity);

		/*!*********************************************************************************
			\brief
			Collsion Exit Event To Subscribe for player
		***********************************************************************************/
		void OnCollisionExit_Player(u32 currentEntity, u32 otherEntity);
	}
}

#endif