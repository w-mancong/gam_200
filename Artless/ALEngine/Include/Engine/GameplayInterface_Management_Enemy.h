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
	\param [in] enemyUnit
	 Reference to enemyUnit to set the attribtues variables in enemyUnit
	***********************************************************************************/
	void SetEnemy01attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set enemy cell destroyer type attribute
	 \param [in] enemyUnit
	 Reference to enemyUnit to set the attribtues variables in enemyUnit
	***********************************************************************************/
	void SetEnemy02attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set third enemy type attribute (future projectile based enemy?)
	\param [in] enemyUnit
	 Reference to enemyUnit to set the attribtues variables in enemyUnit
	***********************************************************************************/
	void SetEnemy03attributes(Unit& enemyUnit);

	/*!*********************************************************************************
	\brief
	 Function for set boss enemy type attribute (future boss enemy?)
	\param [in] enemyUnit
	 Reference to enemyUnit to set the attribtues variables in enemyUnit
	***********************************************************************************/
	void SetEnemy04attributes(Unit& enemyUnit);

	//need enum of type of enemy, containers for attributes of different enemy types
	/*!*********************************************************************************
	\brief
	 Function for Placing type of Enemy onto room
	 \param [in] x
	 x pos to place enemy 
	 \param [in] y
	 y pos to place enemy 
	 \param [in] enemySelection
	 Enum to select enemy type
	 \param [in] enemyEntityList
	 Reference to enemyEntityList for adding new enemy to list
	 \param [in] m_Room
	 Reference to m_Room which is the game room variables for use
	***********************************************************************************/
	ECS::Entity PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<Entity>& enemyEntityList, Room& m_Room);

	//void PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection);

	/*!*********************************************************************************
	\brief
	 Function for display the unit if not placing anything otherwise intereact with the cell(declared here, defined in gameplaysystem.cpp)
	\param [in] invoker

	***********************************************************************************/
	void Event_Unit_OnSelect([[maybe_unused]] Entity invoker);

	/*!*********************************************************************************
	\brief
	 Function for setting move order(declared here, defined in gameplaysystem.cpp)
	\param [in] path
	 vector of path to set for move order
	***********************************************************************************/
	void SetMoveOrder(std::vector<Entity> path);

	void EndTurn();

	/*!*********************************************************************************
	\brief 
	 enemy melee AI logic function for handling update of enemy AI
	\param [in] enemyNeededData
	Reference to enemy Struct object of datas from the level
	\param [in] movingUnitEntity
	Reference to enemyunit that is moving	
	\param [in] currentUnitControlStatus
	Reference the level unit control statis 
	\param [in] enemyEntityList
	 Reference to enemyEntityList for use
	\param [in] m_Room
	 Reference to m_Room which is the game room variables for use
	***********************************************************************************/
	void Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, Entity &movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);

	//void Enemy_Logic_Update_Melee(EnemyManager enemyNeededData);

	/*!*********************************************************************************
	\brief
	enemy cell destroyer AI logic function
	\param [in] enemyNeededData
	Reference to enemy Struct object of datas from the level
	\param [in] movingUnitEntity
	Reference to enemyunit that is moving
	\param [in] currentUnitControlStatus
	Reference the level unit control statis
	\param [in] enemyEntityList
	 Reference to enemyEntityList for use
	\param [in] m_Room
	 Reference to m_Room which is the game room variables for use
	***********************************************************************************/
	void Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, Entity& movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);

	/*!*********************************************************************************
	\brief
	enemy cell destroyer destroying of the placed walkable cell tile logic function
	\param [in] enemyNeededData
	Reference to enemy Struct object of datas from the level
	\param [in] movingUnitEntity
	Reference to enemyunit that is moving
	\param [in] currentUnitControlStatus
	Reference the level unit control statis
	\param [in] enemyEntityList
	 Reference to enemyEntityList for use
	\param [in] m_Room
	 Reference to m_Room which is the game room variables for use
	***********************************************************************************/
	void Enemy_Logic_CellDestroyer_DestroyTile(EnemyManager& enemyNeededData, Entity& movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room);
}
#endif

