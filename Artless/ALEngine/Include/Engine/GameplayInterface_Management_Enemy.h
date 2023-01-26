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

	struct EnemyManager {
		//std::vector<Entity>* enemyEntityList;
		Entity* enemyMoved;
		Entity* playerEntity;
		Entity* startCellEntity;
		Entity* movingUnitEntity;
		//Room* m_Room;
		ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS* currentUnitControlStatus;

	};

	void EnemyManager_LoadData();

	enum class ENEMY_TYPE
	{
		ENEMY_TYPE01,   //0 Enemy Melee
		ENEMY_TYPE02,   //1 Cell Destroyer
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

	void SetMoveOrder(std::vector<Entity> path);

	void EndTurn();

	void Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, std::vector<Entity>& enemyEntityList, Room& m_Room);

	//void Enemy_Logic_Update_Melee(EnemyManager enemyNeededData);

	void Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, std::vector<Entity>& enemyEntityList, Room& m_Room);

}
#endif

