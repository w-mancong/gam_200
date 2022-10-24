/*!
file:	EventTriggerSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for EventTriggerSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GAMEPLAY_SYSTEM_H
#define GAMEPLAY_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		using namespace Component;

		/*!*********************************************************************************
			\brief
			Register RigidbodySystem for ECS
		***********************************************************************************/
		void RegisterGameplaySystem(void);

		/*!*********************************************************************************
			\brief
			Update EventTrigger System
		***********************************************************************************/
		void UpdateGameplaySystem();

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreatePlayerUnit(Entity const& entity);

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateEnemyUnit(Entity const& entity);
	}
}
#endif