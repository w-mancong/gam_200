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
	
		ALEngine::ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::SetEnemy01attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 20, 
		enemyUnit.maxHealth = 20;
		enemyUnit.minDamage = 8, 
		enemyUnit.maxDamage = 13;

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

	void ALEngine::Engine::GameplayInterface_Management_Enemy::PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<Entity>& enemyEntityList, Room& m_Room)
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
		case ALEngine::Engine::GameplayInterface_Management_Enemy::ENEMY_TYPE::ENEMY_TYPE01:
			SetEnemy01attributes(enemyUnit);
			break;
		case ALEngine::Engine::GameplayInterface_Management_Enemy::ENEMY_TYPE::ENEMY_TYPE02:
			SetEnemy02attributes(enemyUnit);
			break;
		case ALEngine::Engine::GameplayInterface_Management_Enemy::ENEMY_TYPE::ENEMY_TYPE03:
			SetEnemy03attributes(enemyUnit);
			break;
		case ALEngine::Engine::GameplayInterface_Management_Enemy::ENEMY_TYPE::ENEMY_TYPE04:
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
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::Enemy_Logic_Update_Melee(u32 &enemyMoved, std::vector<Entity>& enemyEntityList, Entity& playerEntity, Entity& startCellEntity, Room& m_Room, UNITS_CONTROL_STATUS& currentUnitControlStatus, ALEngine::ECS::Entity& movingUnitEntity)
	{
		AL_CORE_INFO("Enemy Making Decision");

		//If reached end, end turn
		if (enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			EndTurn();
			return;
		}

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

		if (enemyUnit.health <= 0) {
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Attacked player");
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
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

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player right");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player left");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) + 1							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player up");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) - 1							&&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player down");
		}

		//If can't find adjacent cell, end turn
		if (!hasFoundCellBesidePlayer) {
			AL_CORE_INFO("No Space Beside Player, Moving to next enemy");
			GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		startCellEntity = GameplayInterface::getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(m_Room, startCellEntity, cellToMoveTo, pathList, true);
		
		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		AL_CORE_INFO("Path Found");

		//Path found, move the enemy accordingly
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = enemyEntityList[enemyMoved];

		UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyMoved;
	}

	void ALEngine::Engine::GameplayInterface_Management_Enemy::Enemy_Logic_Update_CellDestroyer(u32& enemyMoved, std::vector<Entity>& enemyEntityList, Entity& playerEntity, Entity& startCellEntity, Room& m_Room, UNITS_CONTROL_STATUS& currentUnitControlStatus, ALEngine::ECS::Entity& movingUnitEntity)
	{

		AL_CORE_INFO("Enemy Making Decision");

		//If reached end, end turn
		if (enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			EndTurn();
			return;
		}

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

		if (enemyUnit.health <= 0) {
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Attacked player");
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		Entity cellToMoveTo{};
		b8 hasFoundCellBesidePlayer = false;

		AL_CORE_INFO("Checking for cell adjacent to player");

		//Find cell adjacent to player 
		//Then move to the cell accordingly
		if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]) &&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player right");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]) &&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player left");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) + 1 &&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player up");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) - 1 &&
			GameplayInterface::IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player down");
		}

		//If can't find adjacent cell, end turn
		if (!hasFoundCellBesidePlayer) {
			AL_CORE_INFO("No Space Beside Player, Moving to next enemy");
			GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		startCellEntity = GameplayInterface::getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(m_Room, startCellEntity, cellToMoveTo, pathList, true);

		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyMoved;
			Enemy_Logic_Update_Melee(enemyMoved, enemyEntityList, playerEntity, startCellEntity, m_Room, currentUnitControlStatus, movingUnitEntity);
			return;
		}

		AL_CORE_INFO("Path Found");

		//Path found, move the enemy accordingly
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = enemyEntityList[enemyMoved];

		UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyMoved;

	}
}