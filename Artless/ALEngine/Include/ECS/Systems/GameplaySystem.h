/*!
file:	GameplaySystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for GameplaySystem.h

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
			Draws the gameplay's grid and pathfinder
		***********************************************************************************/
		void DrawGameplaySystem();

		/*!*********************************************************************************
			\brief
			create player unit entity
		***********************************************************************************/
		void CreatePlayerUnit(Entity entity);

		/*!*********************************************************************************
			\brief
	        create enemy unit entity
		***********************************************************************************/
		void CreateEnemyUnit(Entity entity);
	}
}
#endif