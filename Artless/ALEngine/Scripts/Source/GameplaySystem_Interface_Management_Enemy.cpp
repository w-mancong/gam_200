#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <Engine/PathFindingManager.h>

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr<GameplaySystem> gameplaySystem;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
	}

	void GameplaySystem_Interface_Management_Enemy::Load(ECS::Entity en)
	{
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(en);	
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		Set_GameplayInterface_Enemy(this);
	}

	void GameplaySystem_Interface_Management_Enemy::Init(ECS::Entity en) {

	}

	void GameplaySystem_Interface_Management_Enemy::Update(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_Enemy::LateUpdate(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_Enemy::Free(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_Enemy::Unload(ECS::Entity en)
	{
		gameplaySystem = nullptr;
	}	

	void GameplaySystem_Interface_Management_Enemy::CreateEnemyUnit(ECS::Entity entity, std::vector<ECS::Entity>& enemyEntityList) {
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
		enemyUnit.unit_Name = "GUARD";
		enemyUnit.minRange = 1;
		enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Unit.png";
		enemyUnit.maxMovementPoints = 4;
		enemyUnit.movementPoints = enemyUnit.maxMovementPoints;

		//Set enemy sprite position and size
		Transform enemySpriteTransform;
		enemySpriteTransform.localPosition = { 0.f, 0.4f };
		enemySpriteTransform.localScale = { 1.25f, 1.25f };

		ALEngine::ECS::CreateSprite(enemyUnit.unit_Sprite_Entity, enemySpriteTransform, "Assets/Images/Bishop v.02.png");


		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Enemy_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);
		Coordinator::Instance()->GetComponent<EntityData>(enemyUnit.unit_Sprite_Entity).tag = "Enemy_Sprite_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, enemyUnit.unit_Sprite_Entity);

		//Set enemy events
		EventTrigger eventTrigger;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);

		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);

		//Add physics
		ECS::CreateRigidbody(entity);
		Rigidbody2D& rigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(entity);
		rigidbody.drag = { 0,0 };
		rigidbody.mass = 0.1f;

		rigidbody.hasGravity = false;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy01attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 10,
			enemyUnit.maxHealth = 10;
		enemyUnit.minDamage = 8,
			enemyUnit.maxDamage = 13;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_MELEE;
		enemyUnit.TriggeredByPlayer = false;
		//set enemy logic function pointer
		//enemyUnit.logic
		//enemyUnit.UpdateEnemyLogic = &Enemy_Logic_Update_Melee;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy02attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 5,
			enemyUnit.maxHealth = 5;
		enemyUnit.minDamage = 8,
			enemyUnit.maxDamage = 13;
		enemyUnit.maxMovementPoints = 1;
		enemyUnit.movementPoints = 1;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_CELL_DESTROYER;
		enemyUnit.TriggeredByPlayer = false;
		//set enemy logic function pointer
		//enemyUnit.logic
		//enemyUnit.UpdateEnemyLogic = &Enemy_Logic_Update_CellDestroyer;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy03attributes([[maybe_unused]] Unit& enemyUnit)
	{
		enemyUnit.health = 10,
		enemyUnit.maxHealth = 10;
		enemyUnit.minDamage = 0,
		enemyUnit.maxDamage = 0;
		enemyUnit.maxMovementPoints = 4;
		enemyUnit.movementPoints = 4;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_SUMMONER;
		enemyUnit.TriggeredByPlayer = false;
		enemyUnit.m_CurrentStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
		enemyUnit.TurnCounter = 0;
		return;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy04attributes([[maybe_unused]] Unit& enemyUnit)
	{
		enemyUnit.health = 10,
		enemyUnit.maxHealth = 10;
		enemyUnit.minDamage = 0,
		enemyUnit.maxDamage = 0;
		enemyUnit.maxMovementPoints = 4;
		enemyUnit.movementPoints = 4;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_SUMMONER;
		enemyUnit.TriggeredByPlayer = false;
		enemyUnit.m_CurrentStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
		enemyUnit.TurnCounter = 0;
		return;
	}

	ECS::Entity GameplaySystem_Interface_Management_Enemy::PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		//std::cout << (GameplaySystem::Instance() == nullptr ? "Enemy : system not set up\n" : "Enemy : system is up\n");
		//Create Enemy entity 
		ECS::Entity newEnemy = Coordinator::Instance()->CreateEntity();

		//Add it to the enemy list
		enemyEntityList.push_back(newEnemy);

		//Create Enemy
		CreateEnemyUnit(newEnemy, gameplaySystem->enemyEntityList);
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(newEnemy);
		enemyUnit.coordinate[0] = x;
		enemyUnit.coordinate[1] = y;

		//Set it's cell
		enemyUnit.m_CurrentCell_Entity = gameplaySystem->getEntityCell(m_Room, x, y);

		//switch based on enemy for calling respective function for setting respective enemy unit
		switch (enemySelection)
		{
		case ENEMY_TYPE::ENEMY_MELEE:
		{

			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Unit.png";
			Animator an = ECS::CreateAnimator("Guard");
			Coordinator::Instance()->AddComponent(enemyUnit.unit_Sprite_Entity, an);
		}
		SetEnemy01attributes(enemyUnit);
		break;
		case ENEMY_TYPE::ENEMY_CELL_DESTROYER:
		{
			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/TileBreaker.png";
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/TileBreaker.png");
		}
		SetEnemy02attributes(enemyUnit);
		break;
		case ENEMY_TYPE::ENEMY_SUMMONER:
		{
			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/TileBreaker.png";
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/TileBreaker.png");
		}
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

		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
		Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(newEnemy);
		enemyTransform.localPosition = SpawnCellTransform.position;

		return newEnemy;
	}

	bool GameplaySystem_Interface_Management_Enemy::RunEnemyAdjacentAttack(Room& room, Unit& enemy) {
		//Check 4 adjacent
		//If player is beside, do damage to it
		if (gameplaySystem->IsCoordinateInsideRoom(room, enemy.coordinate[0] + 1, enemy.coordinate[1])) {
			//RIGHT
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(room, enemy.coordinate[0] + 1, enemy.coordinate[1]));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (gameplaySystem->IsCoordinateInsideRoom(room, enemy.coordinate[0] - 1, enemy.coordinate[1])) {
			//LEFT
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(room, enemy.coordinate[0] - 1, enemy.coordinate[1]));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (gameplaySystem->IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] + 1)) {
			//UP
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] + 1));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (gameplaySystem->IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] - 1)) {
			//DOWN
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] - 1));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		return false;
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_CellDestroyer_DestroyTile(EnemyManager& enemyNeededData, [[maybe_unused]] ECS::Entity& movingUnitEntity, [[maybe_unused]] UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room) {
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved - 1]);

		AL_CORE_INFO("Run destroy block Attack");
		bool ifEnemyIsOnWalkableCell = true;

		AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed block");
		//destroy the walkable block here important
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
			ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");
		}

		//up
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
			ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}
		}

		//down
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
			ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}
		}

		//left
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
			ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}
		}

		//right
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			s32 cellIndex = enemyUnit.coordinate[0] * enemyUnit.coordinate[1];
			ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}
		}
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn();
			return;
		}
		AL_CORE_INFO("Enemy Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		std::vector<ECS::Entity> pathdistance;
		ECS::Entity cellplayerin = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);
		
		enemyNeededData.startCellEntity = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		//check distance from player
		if (ALEngine::Engine::AI::FindPath(gameplaySystem,m_Room, enemyNeededData.startCellEntity, cellplayerin, pathdistance, true) && !enemyUnit.TriggeredByPlayer)
		{
			if (pathdistance.size() < 7)
			{
				enemyUnit.TriggeredByPlayer = true;
			}
		}

		if (!enemyUnit.TriggeredByPlayer)
		{
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Attacked player");
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		ECS::Entity cellToMoveTo{};
		b8 hasFoundCellBesidePlayer = false;

		AL_CORE_INFO("Checking for cell adjacent to player");

		//Find cell adjacent to player 
		//Then move to the cell accordingly
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player right");
		}
		else if (gameplaySystem->IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player left");
		}
		else if (gameplaySystem->IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) + 1 &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player up");
		}
		else if (gameplaySystem->IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) - 1 &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)).hasUnit)
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Found path at player down");
		}

		//If can't find adjacent cell, end turn
		if (!hasFoundCellBesidePlayer) {
			AL_CORE_INFO("No Space Beside Player, Moving to next enemy");
			RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}


		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellToMoveTo, pathList, true);

		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		// Setting move animation for bishop
		Animator& an = Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity);
		ECS::ChangeAnimation(an, "BishopMove");

		AL_CORE_INFO("Path Found");

		//Path found, move the enemy accordingly
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];
		//enemyNeededData.movingUnitEntity = enemyNeededData.enemyEntityList[enemyNeededData.enemyMoved];

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;

		AL_CORE_INFO("enemy moved");
		return;
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn();
			return;
		}

		AL_CORE_INFO("Enemy Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		std::vector<ECS::Entity> pathdistance;
		ECS::Entity cellplayerin = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);

		enemyNeededData.startCellEntity = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		//check distance from player
		if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellplayerin, pathdistance, true) && !enemyUnit.TriggeredByPlayer)
		{
			if (pathdistance.size() < 7)
			{
				enemyUnit.TriggeredByPlayer = true;
			}
		}

		if (!enemyUnit.TriggeredByPlayer)
		{
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		//vector to store the adjacent cells with placed tile
		std::vector<ECS::Entity> cellsMoveableTo{};

		//Entity cellToMoveTo{};
		b8 hasFoundWalkableCells = false;

		//AL_CORE_INFO("Checking for cell adjacent to player");

		AL_CORE_INFO("Checking for walkable cell placed by player");
		//check for all walkable cell placed by player
		for (u32 x = 0; x < m_Room.width; x++)
		{
			for (u32 y = 0; y < m_Room.height; y++)
			{
				if (gameplaySystem->CheckIfWalkableOnGrid(m_Room, x, y) && !Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).hasUnit)
				{
					cellsMoveableTo.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
				}
			}
		}

		if (cellsMoveableTo.size() > 0)
		{
			hasFoundWalkableCells = true;
		}

		//If can't find any walkable, end turn
		if (!hasFoundWalkableCells) {
			AL_CORE_INFO("No walkable cells");
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		enemyNeededData.startCellEntity = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path and compare which is closet
		std::vector<ECS::Entity> tempPathList;
		std::vector<std::vector<ECS::Entity>> listofpathlists{};

		//push the found paths for all the walkable cell
		for (std::vector<ECS::Entity>::iterator i = cellsMoveableTo.begin(); i != cellsMoveableTo.end(); ++i)
		{
			//clear the temp vector before find path
			if (tempPathList.size() > 0)
			{
				tempPathList.clear();
			}

			if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, *i, tempPathList, true))
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
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
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
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];
		movingUnitEntity = enemyEntityList[enemyNeededData.enemyMoved];

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;
		return;
	}

	void ALEngine::Script::GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_Summoner(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn();
			return;
		}

		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);
		if (enemyUnit.health <= 0)
		{
			++enemyNeededData.enemyMoved;
			return;
		}

		ECS::Entity cellplayerin{};
		cellplayerin = gameplaySystem->getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);
		std::vector<ECS::Entity> pathList;
		std::vector<ECS::Entity> pathdistance;

		//check distance from player to see if player triggered enemy
		if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellplayerin, pathdistance, true) && !enemyUnit.TriggeredByPlayer)
		{
			if (pathdistance.size() < 7)
			{
				enemyUnit.TriggeredByPlayer = true;
			}
		}

		//for storing all the cells with no units or player 
		std::vector<ECS::Entity> allEmptyCells{};
		//check for all cells with no units 
		for (u32 x = 0; x < m_Room.width; x++)
		{
			for (u32 y = 0; y < m_Room.height; y++)
			{
				if (!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).hasUnit)
				{
					allEmptyCells.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
				}
			}
		}

		if (enemyUnit.TurnCounter >= 3)
		{
			//check for available cells with no units near summoner
			std::vector<ECS::Entity> enemyToCellDistance{};
			bool foundposition{ false };
			ECS::Entity spawnPosition{};
			for (std::vector<ECS::Entity>::iterator i = allEmptyCells.begin(); i != allEmptyCells.end(); ++i)
			{
				//clear the temp vector before find path
				if (enemyToCellDistance.size() > 0)
				{
					enemyToCellDistance.clear();
				}

				if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, *i, enemyToCellDistance, true))
				{
					if (enemyToCellDistance.size() <5)
					{
						spawnPosition=*i;
						foundposition = true;
						break;
					}
				}
			}

			if (!foundposition)
			{
				AL_CORE_INFO("No available free cell to spawn enemy");
			}
			else
			{
				Unit& spawnUnitPosition = Coordinator::Instance()->GetComponent<Unit>(spawnPosition);
				//do the spawning of enemy with probability of 60% for tile destroyer & 40% for melee enemy

				f64 spawnPercentage = (f64)rand() / RAND_MAX;

				if (spawnPercentage < 0.60)//60% for tile destroyer to spawn
				{
					ECS::Entity enemyEntity = PlaceNewEnemyInRoom(spawnUnitPosition.coordinate[0], spawnUnitPosition.coordinate[1], ENEMY_TYPE::ENEMY_CELL_DESTROYER, enemyEntityList, m_Room);
					ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
					ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);
				}
				else //  40% for melee enemyto spawn
				{
					ECS::Entity enemyEntity = PlaceNewEnemyInRoom(spawnUnitPosition.coordinate[0], spawnUnitPosition.coordinate[1], ENEMY_TYPE::ENEMY_MELEE, enemyEntityList, m_Room);
					ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
					ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);
				}

				//reset counter
				enemyUnit.TurnCounter = 0;
			}

		}

		if (!enemyUnit.TriggeredByPlayer)
		{
			++enemyUnit.TurnCounter;
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}
		else  //if player in range which triggered the summoner to follow player
		{
			//if health <=3
			if (enemyUnit.health <= 3)
			{
				if (enemyUnit.m_PreviousStateId == SUMMONER_ENEMY_STATE::SES_RETREAT)
				{
					AL_CORE_INFO("Spawner transit to Heal State");
					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_HEAL;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
				}
				else
				{
					AL_CORE_INFO("Spawner transit to Retreat State");
					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_RETREAT;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
				}
			}
			else
			{
				//check distance from player
				if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellplayerin, pathdistance, true))
				{
					if (pathdistance.size() <= 4) //player close to enemy
					{
						AL_CORE_INFO("Spawner transit to Move away State");
						enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_MOVE_AWAY;
						enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
						enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					}

					if (pathdistance.size() > 7)//player far from from enemy
					{
						AL_CORE_INFO("Spawner transit to Move closer State");
						enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_MOVE_CLOSER;
						enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
						enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					}
				}
			}

			//for move states to use to storing path from player to tile get final tile to check for path from enemy
			std::vector<ECS::Entity> playerToCellPathList{}; 
			//enemy path to final tile obtain from playerToCellPathList
			std::vector<ECS::Entity> enemyToCellPathList{};
			//list of available paths to valid tiles
			std::vector<std::vector<ECS::Entity>> listofpathlists{};

			std::vector<ECS::Entity> pathList;

			b8 isPathFound = false;

			switch (enemyUnit.m_CurrentStateId)
			{
			    case SUMMONER_ENEMY_STATE::SES_IDLE:	
					break;
				case SUMMONER_ENEMY_STATE::SES_MOVE_CLOSER:

				    //push the found paths for all the walkable cell
					for (std::vector<ECS::Entity>::iterator i = allEmptyCells.begin(); i != allEmptyCells.end(); ++i)
					{
						//clear the temp vector before find path
						if (playerToCellPathList.size() > 0)
						{
							playerToCellPathList.clear();
						}

						if (ALEngine::Engine::AI::FindPath(gameplaySystem,m_Room, cellplayerin, *i, playerToCellPathList, true))
						{
							if (!playerToCellPathList.size() > 7 && !playerToCellPathList.size() < 5)
							{
								if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, playerToCellPathList[0], enemyToCellPathList, true))
								{
									if (enemyToCellPathList.size() == 4)
									{
										listofpathlists.push_back(enemyToCellPathList);
									}
								}
							}
						}
					}

					if (listofpathlists.size() > 0)
					{
						isPathFound = true;
					}

					if (!isPathFound) {
						AL_CORE_INFO("No Path Found");
						++enemyNeededData.enemyMoved;
						gameplaySystem->MoveEnemy();
						return;
					}

					//compare paths sizes
					for (std::vector<std::vector<ECS::Entity>>::iterator i = listofpathlists.begin(); i != listofpathlists.end(); ++i)
					{
						if (pathList.size() <= 0)
						{
							pathList = *i;
						}
						else if (pathList.size() < i->size()) //get closest cell to enemy path
						{
							pathList = *i;
						}
					}

					//move to near player
					gameplaySystem->SetMoveOrder(pathList);

					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					break;
				case SUMMONER_ENEMY_STATE::SES_MOVE_AWAY:

					//find suitable tiles of cell
					//push the found paths for all the walkable cell
					for (std::vector<ECS::Entity>::iterator i = allEmptyCells.begin(); i != allEmptyCells.end(); ++i)
					{
						//clear the temp vector before find path
						if (playerToCellPathList.size() > 0)
						{
							playerToCellPathList.clear();
						}

						if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, cellplayerin, *i, playerToCellPathList, true))
						{
							if (playerToCellPathList.size() > 6 && playerToCellPathList.size() < 8)
							{
								if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, playerToCellPathList[0], enemyToCellPathList, true))
								{
									if (enemyToCellPathList.size() == 4)
									{
										listofpathlists.push_back(enemyToCellPathList);
									}
								}
							}
						}
					}

					if (listofpathlists.size() > 0)
					{
						isPathFound = true;
					}

					if (!isPathFound) {
						AL_CORE_INFO("No Path Found");
						++enemyNeededData.enemyMoved;
						gameplaySystem->MoveEnemy();
						return;
					}

					//compare paths sizes
					for (std::vector<std::vector<ECS::Entity>>::iterator i = listofpathlists.begin(); i != listofpathlists.end(); ++i)
					{
						if (pathList.size() <= 0)
						{
							pathList = *i;
						}
						else if (pathList.size() > i->size()) //get closest cell to enemy path
						{
							pathList = *i;
						}
					}
					//move away from player
					gameplaySystem->SetMoveOrder(pathList);

					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					break;
				case SUMMONER_ENEMY_STATE::SES_RETREAT:
					AL_CORE_INFO("Spawner Retreat State");
					//move to tile furthest from player
					for (std::vector<ECS::Entity>::iterator i = allEmptyCells.begin(); i != allEmptyCells.end(); ++i)
					{
						//clear the temp vector before find path
						if (playerToCellPathList.size() > 0)
						{
							playerToCellPathList.clear();
						}

						if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, cellplayerin, *i, playerToCellPathList, true))
						{
							if (playerToCellPathList.size() > 6)
							{
								if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, playerToCellPathList[0], enemyToCellPathList, true))
								{
									if (enemyToCellPathList.size() == 4)
									{
										listofpathlists.push_back(enemyToCellPathList);
									}
								}
							}
						}
					}

					if (listofpathlists.size() > 0)
					{
						isPathFound = true;
					}

					if (!isPathFound) {
						AL_CORE_INFO("No Path Found");
						++enemyNeededData.enemyMoved;
						gameplaySystem->MoveEnemy();
						return;
					}

					//compare paths sizes
					for (std::vector<std::vector<ECS::Entity>>::iterator i = listofpathlists.begin(); i != listofpathlists.end(); ++i)
					{
						if (pathList.size() <= 0)
						{
							pathList = *i;
						}
						else if (pathList.size() > i->size()) //get closest cell to enemy path
						{
							pathList = *i;
						}
					}
					
					//retreat from player
					gameplaySystem->SetMoveOrder(pathList);

					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					break;
				case SUMMONER_ENEMY_STATE::SES_HEAL:
					//enemy heals
					enemyUnit.health += 3;

					enemyUnit.m_NextStateId = SUMMONER_ENEMY_STATE::SES_IDLE;
					enemyUnit.m_PreviousStateId = enemyUnit.m_CurrentStateId;
					enemyUnit.m_CurrentStateId = enemyUnit.m_NextStateId;
					break;
				default:
					break;
			}

			++enemyUnit.TurnCounter;
			++enemyNeededData.enemyMoved;
			return;
		}
	}
}