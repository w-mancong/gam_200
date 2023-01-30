/*!
file:   GameplayInterface_Management_Enemy.cpp
author: Chan Jie Ming Stanley
email:   c.jiemingstanley\@digipen.edu
brief:	This file contains the function definition for GameplayInterface_Management_Enemy.cpp

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Engine/GamePlayInterface_Management_Enemy.h>
#include <Engine/PathFindingManager.h>
#include <Engine/GameplayInterface_Management_GUI.h>

namespace ALEngine::Engine::GameplayInterface_Management_Enemy
{
	using ALEngine::ECS::Entity;
	using namespace GameplayInterface;
	using namespace GameplayInterface_Management_GUI;

	void CreateEnemyUnit(Entity entity, std::vector<Entity>& enemyEntityList)
	{
		Unit unit{};
		unit.unitType = UNIT_TYPE::ENEMY;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		//Set enemy position & size
		Transform& enemytransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		enemytransform.scale = { 50, 50 };
		enemytransform.localScale = { 100, 100 };

		//Set enemy stats
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		enemyUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();
		enemyUnit.unit_Name = "BISHOP";
		enemyUnit.minRange = 1;
		enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Unit.png";
		enemyUnit.maxMovementPoints = 4;
		enemyUnit.movementPoints = enemyUnit.maxMovementPoints;

		//Set enemy sprite position and size
		Transform enemySpriteTransform;
		enemySpriteTransform.localPosition = { 0.f, 0.4f };
		enemySpriteTransform.localScale = { 1.f, 2.f };

		ALEngine::ECS::CreateSprite(enemyUnit.unit_Sprite_Entity, enemySpriteTransform, "Assets/Images/Bishop v.02.png");

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Enemy_" + std::to_string(enemyEntityList.size() - 1);
		Coordinator::Instance()->GetComponent<EntityData>(enemyUnit.unit_Sprite_Entity).tag = "Enemy_Sprite_" + std::to_string(enemyEntityList.size() - 1);

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, enemyUnit.unit_Sprite_Entity);

		//Set enemy events
		EventTrigger eventTrigger;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
	
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::SetEnemy01attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 20, 
		enemyUnit.maxHealth = 20;
		enemyUnit.minDamage = 8, 
		enemyUnit.maxDamage = 13;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_MELEE;
		//set enemy logic function pointer
		//enemyUnit.logic
		//enemyUnit.UpdateEnemyLogic = &Enemy_Logic_Update_Melee;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::SetEnemy02attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 20,
		enemyUnit.maxHealth = 20;
		enemyUnit.minDamage = 8,
		enemyUnit.maxDamage = 13;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_CELL_DESTROYER;
		//set enemy logic function pointer
		//enemyUnit.logic
		//enemyUnit.UpdateEnemyLogic = &Enemy_Logic_Update_CellDestroyer;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::SetEnemy03attributes(Unit& enemyUnit)
	{
		return;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::SetEnemy04attributes(Unit& enemyUnit)
	{
		return;
	}

	ECS::Entity ALEngine::Engine::GameplayInterface_Management_Enemy::PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<Entity>& enemyEntityList, Room& m_Room)
	{
		//Create Enemy entity 
		Entity newEnemy = Coordinator::Instance()->CreateEntity();

		//Add it to the enemy list
		enemyEntityList.push_back(newEnemy);

		//Create Enemy
		CreateEnemyUnit(newEnemy, enemyEntityList);
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(newEnemy);
		enemyUnit.coordinate[0] = x;
		enemyUnit.coordinate[1] = y;

		//Set it's cell
		enemyUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(m_Room, x, y);

		//switch based on enemy for calling respective function for setting respective enemy unit
		switch (enemySelection)
		{
		case ENEMY_TYPE::ENEMY_MELEE:
			SetEnemy01attributes(enemyUnit);
			break;
		case ENEMY_TYPE::ENEMY_CELL_DESTROYER:
			SetEnemy02attributes(enemyUnit);
			break;
		case ENEMY_TYPE::ENEMY_TYPE03:
			SetEnemy03attributes(enemyUnit);
			break;
		case ENEMY_TYPE::ENEMY_TYPE04:
			SetEnemy04attributes(enemyUnit);
			break;
		default:
			break;
		}

		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).unitEntity = newEnemy;
		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).hasUnit = true;

		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
		Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(newEnemy);
		enemyTransform.localPosition = SpawnCellTransform.position;

		return newEnemy;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, Entity &movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			EndTurn();
			return;
		}
		AL_CORE_INFO("Enemy Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Attacked player");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		Entity cellToMoveTo{};
		b8 hasFoundCellBesidePlayer = false;

		AL_CORE_INFO("Checking for cell adjacent to player");

		//Find cell adjacent to player 
		//Then move to the cell accordingly
		if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])						&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])					&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player right");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player left");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) + 1							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player up");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) - 1							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player down");
		}

		//If can't find adjacent cell, end turn
		if (!hasFoundCellBesidePlayer) {
			AL_CORE_INFO("No Space Beside Player, Moving to next enemy");
			GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		enemyNeededData.startCellEntity = GameplayInterface::getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(m_Room, enemyNeededData.startCellEntity, cellToMoveTo, pathList, true);
		
		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		AL_CORE_INFO("Path Found");

		//Path found, move the enemy accordingly
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];
		//enemyNeededData.movingUnitEntity = enemyNeededData.enemyEntityList[enemyNeededData.enemyMoved];

		UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;

		AL_CORE_INFO("enemy moved");
		return;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, Entity& movingUnitEntity, ALEngine::Engine::GameplayInterface::UNITS_CONTROL_STATUS& currentUnitControlStatus,  std::vector<Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			EndTurn();
			return;
		}
		AL_CORE_INFO("Enemy Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		//Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(*enemyNeededData.playerEntity);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		//AL_CORE_INFO("Run Adjacent Attack");
		//bool ifPlayerIsAlreadyBeside = GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Run destroy block Attack");
		bool ifEnemyIsOnWalkableCell = GameplayInterface::CheckIfWalkableOnGrid(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//AL_CORE_INFO("Check player inside");
		//if (ifPlayerIsAlreadyBeside) {
		//	AL_CORE_INFO("Enemy " + std::to_string(*enemyNeededData.enemyMoved) + " Attacked player");
		//	++* enemyNeededData.enemyMoved;
		//	Enemy_Logic_Update_CellDestroyer(enemyNeededData, enemyEntityList, m_Room);
		//	return;
		//}

		AL_CORE_INFO("Check enemy on walkable cell");
		if (ifEnemyIsOnWalkableCell)
		{
			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed block");
			++enemyNeededData.enemyMoved;
			//destroy the walkable block here important
			if (GameplayInterface::IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1])) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
				cell.m_canWalk = false;
				
				s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
				Entity cellEntity = m_Room.roomCellsArray[cellIndex];

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");
			}

			//EndTurn(); //remove once destroy block section is in and then uncomment bottom code
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}
		
		//vector to store the adjacent cells with placed tile
		std::vector<Entity> cellsMoveableTo{};

		//Entity cellToMoveTo{};
		b8 hasFoundWalkableCells = false;

	    //AL_CORE_INFO("Checking for cell adjacent to player");

		AL_CORE_INFO("Checking for walkable cell placed by player");
		//check for all walkable cell placed by player
		for (u32 x = 0; x < m_Room.width; x++)
		{
			for(u32 y=0; y<m_Room.height; y++)
			{
				if(GameplayInterface::CheckIfWalkableOnGrid(m_Room, x, y) && !Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room,x, y)).hasUnit)
				{
					cellsMoveableTo.push_back(GameplayInterface::getEntityCell(m_Room,x, y));
				}
			}
		}

		if (cellsMoveableTo.size()>0)
		{
			hasFoundWalkableCells = true;
		}

		//If can't find any walkable, end turn
		if (!hasFoundWalkableCells) {
			AL_CORE_INFO("No walkable cells");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		enemyNeededData.startCellEntity = GameplayInterface::getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path and compare which is closet
		std::vector<ECS::Entity> tempPathList;
		std::vector<std::vector<ECS::Entity>> listofpathlists{};

		//push the found paths for all the walkable cell
		for (std::vector<ECS::Entity>::iterator i = cellsMoveableTo.begin(); i != cellsMoveableTo.end(); ++i)
		{
			//clear the temp vector before find path
			if (tempPathList.size()>0)
			{
				tempPathList.clear();
			}

			if (ALEngine::Engine::AI::FindPath(m_Room, enemyNeededData.startCellEntity, *i, tempPathList, true))
			{
				listofpathlists.push_back(tempPathList);
			}
		}
		std::vector<ECS::Entity> pathList;
		//b8 isPathFound = ALEngine::Engine::AI::FindPath(m_Room, *enemyNeededData.startCellEntity, cellToMoveTo, pathList, true);

		b8 isPathFound = false;

		if (listofpathlists.size() > 0)
		{
			isPathFound = true;
		}

		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		AL_CORE_INFO("Path Found");

		//compare paths sizes
		for (std::vector<std::vector<ECS::Entity>>::iterator i = listofpathlists.begin(); i != listofpathlists.end(); ++i)
		{
			if (pathList.size() <= 0)
			{
				pathList = *i;
			}
			else if (pathList.size() > i->size())
			{
				pathList = *i;
			}
		}

		//Path found, move the enemy accordingly
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];
		movingUnitEntity = enemyEntityList[enemyNeededData.enemyMoved];

		UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;
	}
}