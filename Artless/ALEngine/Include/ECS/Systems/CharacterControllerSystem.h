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
			Register RenderSystem for ECS to work
		***********************************************************************************/
		void RegisterCharacterControllerSystem(void);

		/*!*********************************************************************************
			\brief
			Used at end of update, runs through each rigidbody, makes use of collider component to give the updated position of the entity
		***********************************************************************************/
		void UpdateCharacterControllerSystem(void);

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateCharacterController(Entity const& entity);
	}
}

#endif