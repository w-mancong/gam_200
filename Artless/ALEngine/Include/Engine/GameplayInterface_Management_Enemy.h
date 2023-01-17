/*!
file:   GameplayInterface_Management_Enemy.h
author: Chan Jie Ming Stanley
email:   c.jiemingstanley\@digipen.edu
brief:	This file contains the function definition for GameplayInterface_Management_Enemy.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <Engine/GamePlayInterface.h>

#ifndef	GAMEPLAY_INTERFACE_MANAGEMENT_ENEMY_H
#define GAMEPLAY_INTERFACE_MANAGEMENT_ENEMY_H

namespace ALEngine::Engine::GameplayInterface_Management_Enemy
{
	using ALEngine::ECS::Entity;
	using Engine::GameplayInterface::Room;

	enum class ENEMY_TYPE
	{
		ENEMY_TYPE01,   //0
		ENEMY_TYPE02,   //1
		ENEMY_TYPE03,   //2
		ENEMY_TYPE04    //3
	};

	void SetEnemy01attributes(Unit& enemyUnit);

	void SetEnemy02attributes(Unit& enemyUnit);

	void SetEnemy03attributes(Unit& enemyUnit);

	void SetEnemy04attributes(Unit& enemyUnit);

	//need enum of type of enemy, containers for attributes of different enemy types
	/*!*********************************************************************************
	\brief
	 Function for Placing type of Enemy onto room
	***********************************************************************************/
	void PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<Entity>& enemyEntityList, Room& m_Room);

	//void PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection);

	void Event_Unit_OnSelect([[maybe_unused]] Entity invoker);
}
#endif

