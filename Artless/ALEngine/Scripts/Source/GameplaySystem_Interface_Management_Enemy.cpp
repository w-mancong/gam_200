#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <Engine/PathFindingManager.h>
#include <Utility/AudioNames.h>

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
		Set_GameplayInterface_Enemy(en);
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
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);

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
		enemyUnit.abilityCooldown_Enemy = 0;
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
			ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "GuardIdle");
		}
		SetEnemy01attributes(enemyUnit);
		break;

		case ENEMY_TYPE::ENEMY_CELL_DESTROYER:
		{
			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Unit_2.png";
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/TileBreaker.png");
			Animator an = ECS::CreateAnimator("Tile Destroyer");
			Coordinator::Instance()->AddComponent(enemyUnit.unit_Sprite_Entity, an);
			ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "TileDestroyerIdle");
		}
		SetEnemy02attributes(enemyUnit);
		break;

		case ENEMY_TYPE::ENEMY_SUMMONER:
		{
			enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Summoner.png";
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Summoner.png");
			Animator an = ECS::CreateAnimator("Summoner");
			Coordinator::Instance()->AddComponent(enemyUnit.unit_Sprite_Entity, an);
			ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "SummonerIdle");
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

	void GameplaySystem_Interface_Management_Enemy::Audio_PlayEnemyAttack(Unit& enemy) {
		if (enemy.enemyUnitType == ENEMY_TYPE::ENEMY_MELEE) {
			s32 randomVal = rand() % 100;

			if (randomVal > 50) {
				gameplaySystem->PlayAudio(AUDIO_GUARD_ATTACK_1);
			}
			else {
				gameplaySystem->PlayAudio(AUDIO_GUARD_ATTACK_2);
			}
		}
		else if (enemy.enemyUnitType == ENEMY_TYPE::ENEMY_CELL_DESTROYER) {

		}
		else {

		}
	}
	
	void GameplaySystem_Interface_Management_Enemy::Audio_PlayEnemyMove(Unit& enemy) {
		if (enemy.enemyUnitType == ENEMY_TYPE::ENEMY_MELEE) {
			s32 randomVal = rand() % 100;

			if (randomVal < 33) {
				gameplaySystem->PlayAudio(AUDIO_GUARD_MOVE_1);
			}
			else if (randomVal >= 33 && randomVal < 67) {
				gameplaySystem->PlayAudio(AUDIO_GUARD_MOVE_2);
			}
			else {
				gameplaySystem->PlayAudio(AUDIO_GUARD_MOVE_3);
			}
		}
		else if (enemy.enemyUnitType == ENEMY_TYPE::ENEMY_CELL_DESTROYER) {

		}
		else {

		}
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

					Audio_PlayEnemyAttack(Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity));
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
					Audio_PlayEnemyAttack(Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity));
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
					Audio_PlayEnemyAttack(Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity));
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
					Audio_PlayEnemyAttack(Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity));
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
		
		//destroy the walkable block here important
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			gameplaySystem->ResetCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);
		}

		//up
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			gameplaySystem->ResetCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] + 1);

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed  up block");
		}

		//down
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			gameplaySystem->ResetCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1] - 1);

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed  down block");
		}

		//left
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			gameplaySystem->ResetCell(m_Room, enemyUnit.coordinate[0] - 1, enemyUnit.coordinate[1]);

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed  left block");
		}

		//right
		if (gameplaySystem->IsCoordinateInsideRoom(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]));
			cell.m_resetCounter = 0;
			cell.m_canWalk = false;

			gameplaySystem->ResetCell(m_Room, enemyUnit.coordinate[0] + 1, enemyUnit.coordinate[1]);

			if (cell.hasUnit) {
				Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

				if (unit.unitType == UNIT_TYPE::PLAYER) {
					gameplaySystem->DoDamageToUnit(cell.unitEntity, unit.maxHealth);
				}
			}

			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " destroyed right block");
		}
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_Summoner(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room) {
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn_Enemy();
			return;
		}

		AL_CORE_INFO("SUMMONER Making Decision");

		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);
		//ECS::Entity cellToMoveTo = enemyUnit.m_CurrentCell_Entity;

		if (enemyUnit.actionPoints <= 0) {
			ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "SummonerIdle");

			enemyUnit.abilityCooldown_Enemy--;
			if (enemyUnit.abilityCooldown_Enemy <= 0) {
				enemyUnit.abilityCooldown_Enemy = summonerCD;
				Enemy_Cast_Summoner(enemyEntityList[enemyNeededData.enemyMoved]);
			}

			//If health <= 3, heal 3
			if (enemyUnit.health <= 3) {
				enemyUnit.health += 3;
				gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);
			}

			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		//Find whether need to go towards or away from player
		//Use heuristic values from int vector coordinate to determine this action
		//If distance is >= 5, go towards player, else, go away
		b8 goTowardsPlayer = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1])) >= 5;
		b8 retreatFromPlayer{ false };
		//If health <= 3, run away from player
		if (enemyUnit.health <= 3) {
			goTowardsPlayer = false;
			retreatFromPlayer = true;
		}

		//distance between enemy and player
		s32 distanceBetweenEnemyandPlayer = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
		AL_CORE_INFO("dist enemy & player:" + std::to_string(distanceBetweenEnemyandPlayer));

		if (!enemyUnit.playerTriggeredEnemy && distanceBetweenEnemyandPlayer <= 4)
		{
			enemyUnit.playerTriggeredEnemy = true;
			AL_CORE_INFO("AI triggered by player");
		}

		if (!enemyUnit.playerTriggeredEnemy)
		{
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}


		//vector to store the cells no unit with tile
		std::vector<ECS::Entity> cellsMoveableTo{};
		std::vector<ECS::Entity> backUpCellsMoveableTo{};

		b8 hasFoundCells = false;

		if(!retreatFromPlayer)//not retreating from player
		{
			if (distanceBetweenEnemyandPlayer < 5 || distanceBetweenEnemyandPlayer > 5)
			{
				AL_CORE_INFO("enemy relocating");

				//check for all cell with no unit and distance from player is 5 and from enemy is <3
				for (u32 x = 0; x < m_Room.width; x++)
				{
					for (u32 y = 0; y < m_Room.height; y++)
					{
						if (Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).m_isAccessible && !Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).has_Wall && !Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).hasUnit)
						{
							if (Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(x, y)) >= 5)
							{
								if (Math::Vector2Int::Distance(Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]), Math::Vector2Int(x, y)) <= 3)
								{
									cellsMoveableTo.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
								}
							}
						}
						if (Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(x, y)) > 1)
						{
							if (Math::Vector2Int::Distance(Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]), Math::Vector2Int(x, y)) > 2 )
							{
								backUpCellsMoveableTo.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
							}
						}
					}
				}

			}
		}
		else
		{
		   AL_CORE_INFO("enemy retreating");
		   if (distanceBetweenEnemyandPlayer < 10 )
		   {
			   for (u32 x = 0; x < m_Room.width; x++)
			   {
				   for (u32 y = 0; y < m_Room.height; y++)
				   {
					   if (Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).m_isAccessible && !Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).has_Wall && !Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(m_Room, x, y)).hasUnit)
					   {
						   if (Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(x, y)) > 5)
						   {
							   if (Math::Vector2Int::Distance(Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]), Math::Vector2Int(x, y)) > 2)
							   {
								   cellsMoveableTo.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
							   }
						   }
					   }
					   if (Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(x, y)) > 1)
					   {
						   if (Math::Vector2Int::Distance(Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]), Math::Vector2Int(x, y)) >2)
						   {
							   backUpCellsMoveableTo.push_back(gameplaySystem->getEntityCell(m_Room, x, y));
						   }
					   }
				   }
			   }
		   }
        }

		if (!(cellsMoveableTo.size() > 0) && backUpCellsMoveableTo.size() > 0)//if cellsMoveableTo is empty then check if backUpCellsMoveableTo has any position.
		{
			for (u32 i = 0; i < backUpCellsMoveableTo.size(); i++)
			{
				cellsMoveableTo.push_back(backUpCellsMoveableTo[i]);
			}
		}

		if (cellsMoveableTo.size() > 0)//if cellsMoveableTo is not empty
		{
			hasFoundCells = true;
		}

		std::vector<ECS::Entity> tempPathList;
		std::vector<std::vector<ECS::Entity>> listofpathlists{};
		std::vector<ECS::Entity> pathList;

		if (!hasFoundCells)
		{
			ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "SummonerIdle");
			enemyUnit.abilityCooldown_Enemy--;
			if (enemyUnit.abilityCooldown_Enemy <= 0) {
				enemyUnit.abilityCooldown_Enemy = summonerCD;
				Enemy_Cast_Summoner(enemyEntityList[enemyNeededData.enemyMoved]);
			}

			//If health <= 3, heal 3
			if (enemyUnit.health <= 3) {
				enemyUnit.health += 3;
				gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);
			}

			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		
		}

		//push the found paths for all the walkable cell
		for (std::vector<ECS::Entity>::iterator i = cellsMoveableTo.begin(); i != cellsMoveableTo.end(); ++i)
		{
			//clear the temp vector before find path
			if (tempPathList.size() > 0)
			{
				tempPathList.clear();
			}

			if (ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyUnit.m_CurrentCell_Entity, *i, tempPathList, true))
			{
				listofpathlists.push_back(tempPathList);
			}
		}

		//get smallest/shortest path
		for (std::vector<std::vector<ECS::Entity>>::iterator i = listofpathlists.begin(); i != listofpathlists.end(); ++i)
		{
			if (pathList.size() <= 0)
			{
				pathList = *i;
			}
			else if (pathList.size() > i->size() && i->size() > 0)
			{
				pathList = *i;
			}
		}
	
		movingUnitEntity = enemyEntityList[enemyNeededData.enemyMoved];

		ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "SummonerMove");

		//Path found, move the enemy accordingly
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		if (enemyUnit.actionPoints == enemyUnit.maxActionPoints) {
			Audio_PlayEnemyMove(enemyUnit);
		}
		AL_CORE_INFO("summon debug point");
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
			gameplaySystem->EndTurn_Enemy();
			return;
		}

		AL_CORE_INFO("MELEE Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		//distance between enemy and player
		s32 distanceBetweenEnemyandPlayer = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]));

		if (!enemyUnit.playerTriggeredEnemy && distanceBetweenEnemyandPlayer <=4)
		{
			enemyUnit.playerTriggeredEnemy = true;
			AL_CORE_INFO("AI triggered by player");
		}

		if (!enemyUnit.playerTriggeredEnemy)
		{
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}


		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyNeededData.enemyMoved) + " Attacked player");
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
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
			//Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, gameplaySystem->currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		enemyNeededData.startCellEntity = gameplaySystem->getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		AL_CORE_CRITICAL("Player CELL " + Coordinator::Instance()->GetComponent<EntityData>(playerUnit.m_CurrentCell_Entity).tag);
		AL_CORE_CRITICAL("MOVING TO CELL " + Coordinator::Instance()->GetComponent<EntityData>(cellToMoveTo).tag);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = ALEngine::Engine::AI::FindPath(gameplaySystem, m_Room, enemyNeededData.startCellEntity, cellToMoveTo, pathList, true);

		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			return;
		}

		Audio_PlayEnemyMove(enemyUnit);

		// Setting move animation for bishop
		Animator& an = Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity);
		ECS::ChangeAnimation(an, "GuardMove");

		AL_CORE_INFO("Path Found");

		//Path found, move the enemy accordingly
		gameplaySystem->SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = enemyEntityList[enemyNeededData.enemyMoved];

		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyNeededData.enemyMoved;

		AL_CORE_INFO("enemy moved");
		return;
	}

	void GameplaySystem_Interface_Management_Enemy::Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room)
	{
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			gameplaySystem->EndTurn_Enemy();
			return;
		}


		AL_CORE_INFO("CELL DESTROYER Making Decision");

		AL_CORE_INFO("Finding Target Cell");
		//Find a target cell
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(enemyNeededData.playerEntity);

		//distance between enemy and player
		s32 distanceBetweenEnemyandPlayer = Math::Vector2Int::Distance(Math::Vector2Int(playerUnit.coordinate[0], playerUnit.coordinate[1]), Math::Vector2Int(enemyUnit.coordinate[0], enemyUnit.coordinate[1]));

		if (!enemyUnit.playerTriggeredEnemy && distanceBetweenEnemyandPlayer <= 4)
		{
			enemyUnit.playerTriggeredEnemy = true;
			AL_CORE_INFO("AI triggered by player");
		}

		if (!enemyUnit.playerTriggeredEnemy)
		{
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			return;
		}

		if (enemyUnit.health <= 0) {
			++enemyNeededData.enemyMoved;
			gameplaySystem->MoveEnemy();
			//Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
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

		Audio_PlayEnemyMove(enemyUnit);
		ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(enemyUnit.unit_Sprite_Entity), "TileDestroyerMoving");

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