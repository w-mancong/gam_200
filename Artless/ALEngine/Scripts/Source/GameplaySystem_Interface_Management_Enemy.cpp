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

		s32 summonerCD = 3;
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
		enemyUnit.maxActionPoints = 4;
		enemyUnit.actionPoints = enemyUnit.maxActionPoints;

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
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy02attributes(Unit& enemyUnit)
	{
		enemyUnit.health = 5,
		enemyUnit.maxHealth = 5;
		enemyUnit.minDamage = 8,
		enemyUnit.maxDamage = 13;
		enemyUnit.maxActionPoints = 1;
		enemyUnit.actionPoints = 1;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_CELL_DESTROYER;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy03attributes([[maybe_unused]] Unit& enemyUnit)
	{
		enemyUnit.health = 10,
		enemyUnit.maxHealth = 10;
		enemyUnit.minDamage = 0,
		enemyUnit.maxDamage = 0;
		enemyUnit.maxActionPoints = 4;
		enemyUnit.actionPoints = 4; 
		enemyUnit.abilityCooldown_Enemy = summonerCD;
		enemyUnit.enemyUnitType = ENEMY_TYPE::ENEMY_SUMMONER;
	}

	void GameplaySystem_Interface_Management_Enemy::SetEnemy04attributes([[maybe_unused]] Unit& enemyUnit)
	{
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
			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Summoner.png";
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Summoner.png");
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

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_Summoner(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room) {
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn();
			return;
		}

		AL_CORE_INFO("SUMMONER Making Decision");

		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);
		ECS::Entity cellToMoveTo = enemyUnit.m_CurrentCell_Entity;

		//Find path
		std::vector<ECS::Entity> pathList;

		if (enemyUnit.actionPoints <= 0) {
			enemyUnit.abilityCooldown_Enemy--;
			if (enemyUnit.abilityCooldown_Enemy <= 0) {
				enemyUnit.abilityCooldown_Enemy = summonerCD;
				Enemy_Cast_Summoner(enemyEntityList[enemyNeededData.enemyMoved]);
			}

			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		//Find whether need to go towards or away from player
		//Use heuristic values from int vector coordinate to determine this action
		//If distance is >= 5, go towards player, else, go away
		b8 goTowardsPlayer = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1])) >= 5;

		//Move right
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1])).hasUnit)
		{
			Cell& previousCell = Coordinator::Instance()->GetComponent<Cell>(cellToMoveTo);
			f32 newDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]));
			f32 oldDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(previousCell.coordinate.x, previousCell.coordinate.y));

			if (goTowardsPlayer) {
				if(newDistance < oldDistance)
				cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]);
			}
			else {
				if (newDistance >= oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]);
			}//End else
		}

		//Move left
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1])).hasUnit)
		{
			Cell& previousCell = Coordinator::Instance()->GetComponent<Cell>(cellToMoveTo);
			f32 newDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]));
			f32 oldDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(previousCell.coordinate.x, previousCell.coordinate.y));

			if (goTowardsPlayer) {
				if (newDistance < oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]);
			}
			else {
				if (newDistance >= oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]);
			}//End else
		}

		//Move up
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1)).hasUnit)
		{
			Cell& previousCell = Coordinator::Instance()->GetComponent<Cell>(cellToMoveTo);
			f32 newDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1));
			f32 oldDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(previousCell.coordinate.x, previousCell.coordinate.y));

			if (goTowardsPlayer) {
				if (newDistance < oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1);
			}
			else {
				if (newDistance >= oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1);
			}//End else
		}

		//Move down
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1) &&
			gameplaySystem->IsCoordinateCellAccessible(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1) &&
			!Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1)).hasUnit)
		{
			Cell& previousCell = Coordinator::Instance()->GetComponent<Cell>(cellToMoveTo);
			f32 newDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1));
			f32 oldDistance = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(previousCell.coordinate.x, previousCell.coordinate.y));

			if (goTowardsPlayer) {
				if (newDistance < oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1);
			}
			else {
				if (newDistance >= oldDistance)
					cellToMoveTo = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1);
			}//End else
		}

		b8 isPathFound = ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyUnit.m_CurrentCell_Entity, cellToMoveTo, pathList, true);

		if (!isPathFound) {
			enemyUnit.abilityCooldown_Enemy--;
			if (enemyUnit.abilityCooldown_Enemy <= 0) {
				enemyUnit.abilityCooldown_Enemy = summonerCD;
				Enemy_Cast_Summoner(enemyEntityList[enemyNeededData.enemyMoved]);
			}

			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];

		//Path found, move the enemy accordingly
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		//gameplaySystem->MoveEnemy();
	}
	
	void GameplaySystem_Interface_Management_Enemy::Enemy_Cast_Summoner(ECS::Entity& summoner_Entity) {
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(summoner_Entity);

		//Shift through all adjacent and find one that is accessible and have no unit, then spawn

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				//If coordinate is out of bound
				if (!gameplaySystem->IsCoordinateInsideRoom(gameplaySystem->m_Room, enemyUnit.coordinate[0] + i, enemyUnit.coordinate[1] + j)) {
					continue;
				}

				//Get offset cell component 
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(gameplaySystem->m_Room, enemyUnit.coordinate[0] + i, enemyUnit.coordinate[1] + j));

				//Skip if cell is not navigable, has unit or wall
				if (!cell.m_isAccessible || cell.hasUnit || cell.has_Wall) {
					continue;
				}
				
				bool spawnMelee = (rand() % 100) <= 60;
				PlaceNewEnemyInRoom(enemyUnit.coordinate[0] + i, enemyUnit.coordinate[1] + j, spawnMelee ? ENEMY_TYPE::ENEMY_MELEE : ENEMY_TYPE::ENEMY_CELL_DESTROYER, gameplaySystem->enemyEntityList, gameplaySystem->m_Room);
				return;
			} //End j loop
		} //End i loop
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn();
			return;
		}
		AL_CORE_INFO("MELEE Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Attacked player");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
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
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		enemyNeededData.startCellEntity = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellToMoveTo, pathList, true);

		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		// Setting move animation for bishop
		Animator& an = Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity);
		//ECS::ChangeAnimation(an, "BishopMove");


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

		AL_CORE_INFO("CELL DESTROYER Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
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
			Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
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
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = (enemyEntityList)[enemyNeededData.enemyMoved];
		movingUnitEntity = enemyEntityList[enemyNeededData.enemyMoved];

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;
	}
}