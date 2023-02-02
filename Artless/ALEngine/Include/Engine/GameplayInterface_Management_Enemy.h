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
		Entity enemyMoved;
		Entity playerEntity;
		Entity startCellEntity;
		//Entity movingUnitEntity;
		//Room* m_Room;
		//ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS currentUnitControlStatus;

	};

	/*!*********************************************************************************
	\brief
	 Function for setting EnemyManager object variables for references for 
	 GameplayInterface_Management_Enemy functions
	***********************************************************************************/
	void EnemyManager_LoadData();

	/*!*********************************************************************************
	\brief
	 Function for set enemy melee type attribute
	***********************************************************************************/
	void SetEnemy01attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set enemy cell destroyer type attribute
	***********************************************************************************/
	void SetEnemy02attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set third enemy type attribute (projectile based enemy?)
	***********************************************************************************/
	void SetEnemy03attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set boss enemy type attribute (boss enemy?)
	***********************************************************************************/
	void SetEnemy04attributes(Unit& enemyUnit);

	//need enum of type of enemy, containers for attributes of different enemy types
	/*!*********************************************************************************
	\brief
	 Function for Placing type of Enemy onto room
	***********************************************************************************/
	ECS::Entity PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<Entity>& enemyEntityList, Room& m_Room);

	void CellDestroyer_CellAttack(EnemyManager& enemyNeededData, Room& m_room, Unit& enemyUnit);

	//void PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection);

	void Event_Unit_OnSelect([[maybe_unused]] Entity invoker);

	void SetMoveOrder(std::vector<Entity> path);

	void EndTurn();

	/*!*********************************************************************************
	\brief 
		enemy melee AI logic function
	\param [in]
		
	\param [in]
		
	***********************************************************************************/
	void Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, Entity &movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);

	//void Enemy_Logic_Update_Melee(EnemyManager enemyNeededData);

	/*!*********************************************************************************
	\brief
		enemy cell destroyer AI logic function
	\param [in]

	\param [in]

	***********************************************************************************/
	void Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, Entity& movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);

	/*!*********************************************************************************
	\brief
		enemy cell destroyer destroying of the cell tile logic function
	\param [in]

	\param [in]

	***********************************************************************************/
	void Enemy_Logic_CellDestroyer_DestroyTile(EnemyManager& enemyNeededData, Entity& movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);
}
#endif

