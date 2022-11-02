/*!
file:	EventTriggerSystem.h
author:	Tan Zhen Xiong
co-author: Chan Jie Ming Stanley
email:	t.zhenxiong@digipen.edu
email: c.jiemingstanley\@digipen.edu
brief:	This file contains the function declarations for EventTriggerSystem.h

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			Start GamePlay System to initialize variables for gameplay logic
		***********************************************************************************/
		void StartGameplaySystem();

		/*!*********************************************************************************
			\brief
			Update GamePlay System to update gameplay logic
		***********************************************************************************/
		void UpdateGameplaySystem();

		/*!*********************************************************************************
			\brief
			Exit GamePlay System to clean up
		***********************************************************************************/
		void ExitGameplaySystem();

		/*!*********************************************************************************
			\brief
			create player unit entity
		***********************************************************************************/
		void CreatePlayerUnit(Entity const& entity);

		/*!*********************************************************************************
			\brief
	        create enemy unit entity
		***********************************************************************************/
		void CreateEnemyUnit(Entity const& entity);

	}
}
#endif