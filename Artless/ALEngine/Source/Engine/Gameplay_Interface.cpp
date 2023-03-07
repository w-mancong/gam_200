#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <GameplayCamera.h>
#include <Utility/AudioNames.h>
#include <Engine/PathFindingManager.h>

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr<GameplaySystem_Interface_Management_Enemy> gameplaySystem_Enemy;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
		std::shared_ptr<GameplaySystem> gameplaySystem;

		//enemymanager struct object for enemymanagement function to access needed variables
		Script::GameplaySystem_Interface_Management_Enemy::EnemyManager enemyNeededData;
	}

	void Set_GameplayInterface_Enemy(ECS::Entity GameplaySystemEntity) {
		gameplaySystem_Enemy = ECS::GetLogicComponent<GameplaySystem_Interface_Management_Enemy>(GameplaySystemEntity);
	}

	void Set_GameplayInterface_GUI(ECS::Entity GameplaySystemEntity) {
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(GameplaySystemEntity);
	}
	
	void Set_GameplayInterface_GameplayManager(ECS::Entity GameplaySystemEntity) {
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(GameplaySystemEntity);
	}

	void GameplaySystem::CreatePlayerUnit(ECS::Entity entity) {
		//Create new unit
		Unit unit{};
		unit.unitType = UNIT_TYPE::PLAYER;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		//Initialize the transform size and position
		Transform& playertransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		playertransform.scale = { 50, 50 };
		playertransform.localScale = { 100, 100 };

		//Set unit stats
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		playerUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();
		playerUnit.unit_Name = "DROR";
		playerUnit.minRange = 2;
		playerUnit.minDamage = 5;
		playerUnit.maxDamage = 5;
		playerUnit.defense = 1;
		playerUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Player_Unit.png";

		//Set player sprite transform size & position
		Transform playerSpriteTransform;
		playerSpriteTransform.localPosition = { 0.f, 0.4f };
		playerSpriteTransform.localScale = { 1.35f, 1.35f };

		ECS::CreateSprite(playerUnit.unit_Sprite_Entity, playerSpriteTransform, "Assets/Images/Player v2.png");

		Animator an = ECS::CreateAnimator("Player");
		Coordinator::Instance()->AddComponent(playerUnit.unit_Sprite_Entity, an);

		ECS::ChangeAnimation(Coordinator::Instance()->GetComponent<Animator>(playerUnit.unit_Sprite_Entity), "PlayerIdle");

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Player";
		Coordinator::Instance()->GetComponent<EntityData>(playerUnit.unit_Sprite_Entity).tag = "Player_Sprite";

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, playerUnit.unit_Sprite_Entity);

		//Add events
		EventTrigger eventTrigger;
		eventTrigger.layer = 1;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		ECS::Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);
		//AddLogicComponent<Script::PauseLogic>(entity)

		//Camera Logic
		ECS::AddLogicComponent<Script::GameplayCamera>(entity);

		//Add physics
		ECS::CreateRigidbody(entity);
		Rigidbody2D& rigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(entity);
		rigidbody.drag = { 0,0 };
		rigidbody.mass = 0.1f;
		rigidbody.hasGravity = false;
	}

	void GameplaySystem::EndTurn()
	{
		//Reset the statuses
		currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;
		currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;

		//Disable the end turn button
		ECS::SetActive(false, gameplaySystem_GUI->getGuiManager().endTurnBtnEntity);

		//Set the turn accordingly
		switch (currentPhaseStatus) {
		case PHASE_STATUS::PHASE_SETUP:
			//Load action phase
			currentPhaseStatus = PHASE_STATUS::PHASE_ACTION;
			AL_CORE_DEBUG("Loading PHASE ACTION");
			gameplaySystem_GUI->ToggleAbilitiesGUI(true);
			gameplaySystem_GUI->TogglePatternGUI(false);
			break;

		case PHASE_STATUS::PHASE_ACTION:
			//Load enemy phase
			AL_CORE_DEBUG("Loading PHASE ENEMY");
			currentPhaseStatus = PHASE_STATUS::PHASE_ENEMY;
			enemyNeededData.enemyMoved = 0;

			gameplaySystem_GUI->ToggleAbilitiesGUI(false);
			gameplaySystem_GUI->TogglePatternGUI(false);
			MoveEnemy();	//Start enemy logic
			break;

		case PHASE_STATUS::PHASE_ENEMY:
			//Load setup phase
			AL_CORE_DEBUG("Loading PHASE SETUP");
			currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;
			gameplaySystem_GUI->TogglePatternGUI(true);

			//Reset player movement points
			Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
			playerUnit.actionPoints += 4;
			if (playerUnit.actionPoints > playerUnit.maxActionPoints) {
				playerUnit.actionPoints = playerUnit.maxActionPoints;
			}

			gameplaySystem_GUI->Update_AP_UI(playerUnit.actionPoints);

			//Reset enemy move ment points
			for (int i = 0; i < enemyEntityList.size(); ++i) {
				Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
				enemyUnit.actionPoints = enemyUnit.maxActionPoints;
			}

			//Update the GUI to select player
			gameplaySystem_GUI->UpdateGUI_OnSelectUnit(playerEntity);

			//Do an update for all walkable cell on the map
			scanRoomCellArray();

			//Check if player cell is destroyed, if yes then eliminate player
			checkPlayerPlacement();

			//Display the your turn animation 
			ECS::ParticleSystem::GetParticleSystem().DisplayYourTurn();

			for (int i = 0; i < Abilities_List.size(); ++i) {
				if (Abilities_List[i].current_Cooldown > 0) {
					Abilities_List[i].current_Cooldown--;
				}
			}

			gameplaySystem_GUI->Update_Ability_Cooldown(Abilities_List, false);
			break;
		}
		gameplaySystem_GUI->GuiUpdatePhaseIndicator(currentPhaseStatus);
	}

	uint32_t GameplaySystem::getRoomSize() {
		return roomSize[0] * roomSize[1];
	}

	u32 GameplaySystem::getEntityCell(Room& currentRoom, u32 x, u32 y)
	{
		//Get required cell's entity
		return currentRoom.roomCellsArray[y * currentRoom.width + x];
	}

	void GameplaySystem::PlaceNewPlayerInRoom(s32 x, s32 y) {
		//Create a new player entity
		playerEntity = Coordinator::Instance()->CreateEntity();

		//Create the player
		CreatePlayerUnit(playerEntity);

		//Assign it's states
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		playerUnit.coordinate[0] = x;
		playerUnit.coordinate[1] = y;

		//Set the cell it's on
		playerUnit.m_CurrentCell_Entity = getEntityCell(m_Room, x, y);

		//Set movement points
		playerUnit.maxActionPoints = 6;
		playerUnit.actionPoints = playerUnit.maxActionPoints;

		gameplaySystem_GUI->Update_AP_UI(playerUnit.actionPoints);

		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).unitEntity = playerEntity;
		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).hasUnit = true;

		//Set health
		playerUnit.maxHealth = 50;
		playerUnit.health = 50;

		//Set the cell the player is on to be walkable
		PlaceWalkableOnGrid(m_Room, { x, y }, "Assets/Images/Walkable.png");

		//Set the transform
		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]));
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		playerTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::PlaceWalkableOnGrid(Room& room, Math::Vector2Int coordinate, std::string sprite_fileName) {
		//Get Cell Entity
		ECS::Entity cellEntity = getEntityCell(room, coordinate.x, coordinate.y);

		//Get Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return;
		}

		//Set to canwalk
		cell.m_canWalk = true;
		cell.m_resetCounter = 2;

		//Change the cell sprite to filename sprite
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
		sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite_fileName);
	}

	void GameplaySystem::InitializeEndTurnButton() {
		//Get the end turn entity
		gameplaySystem_GUI->getGuiManager().endTurnBtnEntity = Coordinator::Instance()->GetEntityByTag("end_turn");
		ECS::CreateButton(gameplaySystem_GUI->getGuiManager().endTurnBtnEntity);

		//Subscribe the end turn function
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().endTurnBtnEntity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_EndTurn);
	}

	bool GameplaySystem::IsCoordinateInsideRoom(Room& currentRoom, u32 gridX, u32 gridY)
	{
		//Return if coordinate is inside room
		return (gridX >= 0) && (gridX < currentRoom.width) && (gridY >= 0) && (gridY < currentRoom.height);
	}

	void GameplaySystem::EnemyManager_LoadData()
	{
		enemyNeededData.enemyMoved = 0;
		enemyNeededData.playerEntity = playerEntity;
		enemyNeededData.startCellEntity = startCellEntity;
	}

	// Scan the entire room array to check for the tile counters and to change the sprite to the correct state of the tile
	void GameplaySystem::scanRoomCellArray() {
		//Keep track of reset counter
		s32 resetCounter;
		//Scan through each cell in the roomCellArray for the individual cell in the roomArray
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->roomSize[0]); ++i) {
			for (s32 j = 0; j < static_cast<s32>(gameplaySystem->roomSize[1]); ++j) {
				//Get the cell index
				s32 cellIndex = i * gameplaySystem->roomSize[0] + j;
				ECS::Entity cellEntity = m_Room.roomCellsArray[cellIndex];

				//Get the cell component
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				
				if (cell.m_isAccessible == false) {
					continue;
				}

				resetCounter = checkTileCounters(cell);

				//If 1 then set to crack
				if (resetCounter == 1) {

					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Cracked_Tile.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE

				}
				if (resetCounter == 0) {
					//If 0 then reset the cell sprite
					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

					//If it's wall then destroy the wall
					if (cell.has_Wall) {
						destroyWall(gameplaySystem->m_Room, cell.coordinate.x, cell.coordinate.y, false);
					}
					else if (cell.hasBomb) {
						//explode
						Bomb_Explode(gameplaySystem->m_Room, cell.coordinate.x, cell.coordinate.y);
					}
				}
			}
		}
	}

	void GameplaySystem::Bomb_Explode(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y) {
		//Range of bomb is 1 cell away
		//Destroy Walkability/wall, just reset cell
		//Units on top or adjacent is damaged 13

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				//If coordinate is out of bound
				if (!IsCoordinateInsideRoom(currentRoom, x + i, y + j)) {
					continue;
				}

				//Get offset cell component 
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x + i, y + j));
				
				//Skip if cell is not navigable
				if (!cell.m_isAccessible) {
					continue;
				}
				
				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(currentRoom, x + i, y + j));

				ECS::ParticleSystem::GetParticleSystem().UnitDmgParticles(transform.position);

				//Do damage to cells without bombs
				if (cell.hasBomb) {
					continue;
				}

				if (cell.hasUnit) {
					DoDamageToUnit(cell.unitEntity, 13);
				}

				ResetCell(gameplaySystem->m_Room, x + i, y + j);
			}
		}

		//Get center cell component 
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		Transform& transform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(currentRoom, x, y));

		ECS::ParticleSystem::GetParticleSystem().UnitDmgParticles(transform.position);

		if (cell.hasUnit) {
			DoDamageToUnit(cell.unitEntity, 13);
		}

		ResetCell(gameplaySystem->m_Room, x, y);
	}

	//Check the selected tile counters and to make amendments to them at the end of the turn
	s32 GameplaySystem::checkTileCounters(Cell& selectedCell) {
		//Decrement the rset counter
		if (selectedCell.m_resetCounter > 0) {
			selectedCell.m_resetCounter--;
		}

		if (selectedCell.m_resetCounter == 0) {
			selectedCell.m_canWalk = false;
		}

		//Return the reset counter
		return selectedCell.m_resetCounter;
	}

	//Check the tile the player is currently on to check if the tile is supposed to be destroyed
	void GameplaySystem::checkPlayerPlacement() {
		//Get the components
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		ECS::Entity cellEntity = playerUnit.m_CurrentCell_Entity;
		Cell& playerUnitCell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//If the cell is not walkable, do damage tot he player
		if (playerUnitCell.m_canWalk == false) {
			DoDamageToUnit(gameplaySystem->playerEntity, playerUnit.maxHealth + 1);
		}
	}

	void GameplaySystem::ToggleCellAccessibility(Room& currentRoom, u32 x, u32 y, b8 istrue) {
		//Get cell component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		cell.m_isAccessible = istrue;	//set accessible

		//Set it's overlay
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = istrue;

		//Toggle color accordingly
		if (!cell.m_isAccessible)
			Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 0.f,0.f,0.f,0.f };
		else
			Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 1.f,1.f,1.f,1.f };
	}

	void GameplaySystem::InitializePatterns(std::vector<Pattern>& patternList) {
		//Clear pattern
		patternList.clear();

		//Template for pattern
		Pattern newPattern;
		std::vector<Math::Vector2Int> offset;

		//****** T SHAPE START *****//
		newPattern.file_path = "Assets\\Images\\T.png";

		//Upside down T shape (Ʇ)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//rightside T shape (|-)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//downside T shape (T)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//leftside T shape (-|)s
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** T SHAPE END *****//

		//****** LINE SHAPE START *****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\I.png";

		//Horizontal Straight line(---)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		offset.push_back({ 3, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Vertical Straight line(|)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		offset.push_back({ 0, 3 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** LINE SHAPE END *****//

		//****** L SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\L.png";

		//L Shape (L)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//90° clockwise rotation L Shape
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//180° clockwise rotation L 
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 0, -2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//270° clockwise rotation L 
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ -1, 0 });
		offset.push_back({ -2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);

		//****** L SHAPE END *****//

		//****** J SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\J.png";

		//J Shape (⅃)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//90° clockwise Shape (J)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//180° clockwise Shape (J)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 0, -2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//270° clockwise Shape (J)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		offset.push_back({ -2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** J SHAPE END *****//

		//****** BOX SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\O.png";

		//Box Shape (☐)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 1, 1 });
		offset.push_back({ 0, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** BOX SHAPE END *****//

		//****** S SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\S.png";

		//S Shape (S)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//S Shape (S) (rotated)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** S SHAPE END *****//

		//****** Z SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\Z.png";

		//Z Shape (Z)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Z Shape (N)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ -1, -1 });
		offset.push_back({ 0, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** Z SHAPE END *****//
	}

	//Initialize Abilities
	void GameplaySystem::InitializeAbilities(std::vector<Abilities>& abilitiesList) {
		//Clear abilities
		abilitiesList.clear();

		//Template for abilities
		Abilities new_ability;

		//Fixed damage
		new_ability.current_Ability_Name = ABILITY_NAME::HARD_DROP;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.damage = 9;
		new_ability.max_Cooldown = 1;
		new_ability.cost = 2;
		abilitiesList.push_back(new_ability);

		//Life steal
		new_ability.current_Ability_Name = ABILITY_NAME::LIFE_DRAIN;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.damage = 8;
		new_ability.max_Cooldown = 2;
		new_ability.cost = 2;
		abilitiesList.push_back(new_ability);

		//Overhang
		new_ability.current_Ability_Name = ABILITY_NAME::OVERHANG;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.max_Cooldown = 5;
		new_ability.cost = 0;
		abilitiesList.push_back(new_ability);

		//Construct Wall
		new_ability.current_Ability_Name = ABILITY_NAME::CONSTRUCT_WALL;
		new_ability.current_Ability_Type = ABILITY_TYPE::EFFECT;
		new_ability.max_Cooldown = 4;
		new_ability.cost = 2;
		abilitiesList.push_back(new_ability);
	
		//Matrix Trap
		new_ability.current_Ability_Name = ABILITY_NAME::MATRIX_TRAP;
		new_ability.current_Ability_Type = ABILITY_TYPE::EFFECT;
		new_ability.max_Cooldown = 3;
		new_ability.cost = 2;
		abilitiesList.push_back(new_ability);
	
		//Matrix Trap
		new_ability.current_Ability_Name = ABILITY_NAME::VOLATILE;
		new_ability.current_Ability_Type = ABILITY_TYPE::EFFECT;
		new_ability.max_Cooldown = 5;
		new_ability.cost = 2;
		abilitiesList.push_back(new_ability);
	}

	void GameplaySystem::RandomizePatternList(void)
	{
		u32 num_patterns = static_cast<u32>(pattern_Default.size());

		std::random_device rd;
		std::mt19937 mt(rd());

		// Create a copy of the first few in list
		std::vector<Pattern> tempList;
		if (num_patterns <= pattern_List.size())
		{	// Copy list to a temp list
			for (u32 i{ 0 }; i < num_patterns; ++i)
				tempList.push_back(pattern_List[i]);

			// Empty Pattern List Shown
			pattern_List.clear();

			// Put the temp values into the pattern
			for (u32 i{ 0 }; i < num_patterns; ++i)
				pattern_List.push_back(tempList[i]);
		}
		else
		{	// Randomize
			pattern_List = pattern_Default;
			std::shuffle(pattern_List.begin(), pattern_List.end(), mt);
		}

		// Randomize next set
		tempList = pattern_Default;

		do {
			std::shuffle(tempList.begin(), tempList.end(), mt);
		} while (tempList.front().file_path == pattern_List.back().file_path);

		// Push back into list
		for (u32 i{ 0 }; i < tempList.size(); ++i)
		{
			pattern_List.push_back(tempList[i]);
		}
	}



	void GameplaySystem::Cheat_ToggleGodMode() {
		//Toggle the boolean
		godMode = !godMode;

		//Get the components
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		//Toggle the color accordingly from the godmode
		if (godMode) {
			playerSprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
		}
		else {
			playerSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		//Reduce al enemy damage to 0 if godmode
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			if (godMode) {
				unit.minDamage = 0;
				unit.maxDamage = 0;
			}
			else {
				unit.minDamage = 5;
				unit.maxDamage = 5;
			}
		}

		//Select the player
		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_IncreasePlayerHealth(s32 amount) {
		//Get player unit
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

		//Add health
		unit.health += amount;

		//Limit to max
		if (unit.health >= unit.maxHealth) {
			unit.health = unit.maxHealth;
		}

		//Select player
		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_ToggleDoubleAbilitiesDoubleDamage() {
		//Toggle double damage cheat
		cheat_abilitiesDoubleDamage = !cheat_abilitiesDoubleDamage;

		//Set damage accordingly
		if (cheat_abilitiesDoubleDamage) {
			Abilities_List[0].damage = 30;
			Abilities_List[1].damage = 24;
		}
		else {
			Abilities_List[0].damage = 15;
			Abilities_List[1].damage = 12;
		}

		if (currentPhaseStatus != PHASE_STATUS::PHASE_ACTION) {
			return;
		}

		//Toggle the color of the abilities
		for (s32 i = 0; i < 2; ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[i]);

			if (cheat_abilitiesDoubleDamage) {
				sprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
			}
			else {
				sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
	}

	void GameplaySystem::Cheat_DecreaseEnemyHealthToOne() {
		//Reduce all enemy health to 1
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			if (unit.health > 0) {
				unit.health = 1;
				gameplaySystem_GUI->UpdateGUI_OnSelectUnit(enemyEntityList[i]);
			}
		}
	}

	void GameplaySystem::Cheat_EliminateAllEnemy() {
		//Do damage to all enemy
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			DoDamageToUnit(enemyEntityList[i], unit.maxHealth);
		}

		//ECS::SetActive(true, getGuiManager().Win_Clear);
	}

	void GameplaySystem::Cheat_ResetAllEnemiesHealth() {
		//Reset all
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			//Limit enemy health
			if (unit.health > 0) {
				unit.health = unit.maxHealth;
				gameplaySystem_GUI->UpdateGUI_OnSelectUnit(enemyEntityList[i]);
			}
		}
	}

	void GameplaySystem::Cheat_ResetPlayerHealth() {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

		//Reset
		unit.health = unit.maxHealth;

		//Select player
		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_ClearFloorWalkability() {
		//Shift through each of the cell and toggle walkability
		for (s32 i = 0; i < static_cast<s32>(getRoomSize()); ++i) {
			ToggleCellWalkability(m_Room, m_Room.roomCellsArray[i], false);
		}

		//Toggle walkability of units
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		ToggleCellWalkability(m_Room, unit.m_CurrentCell_Entity, true);
	}

	void GameplaySystem::ToggleCellWalkability([[maybe_unused]] Room& room, ECS::Entity cellEntity, b8 istrue) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return;
		}

		//Set to toggle
		cell.m_canWalk = istrue;

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);

		//Set sprite accordingly
		if (istrue) {
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Walkable.png");
		}
		else {
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");
		}
	}

	void GameplaySystem::DoDamageToUnit(ECS::Entity unitEntity, s32 damage) {
		//Get the unit entity data
		[[maybe_unused]] EntityData& unitData = Coordinator::Instance()->GetComponent<EntityData>(unitEntity);

		//Get unit component
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		//Get Transform
		Transform& unitTrans = Coordinator::Instance()->GetComponent<Transform>(unitEntity);

		//Get Particle System
		ECS::ParticleSystem::GetParticleSystem().UnitDmgParticles(unitTrans.position);

		AL_CORE_CRITICAL("Damage " + std::to_string(damage) + " to " + unitData.tag + " which has " + std::to_string(unit.health) + " health");

		//Do damage
		unit.health -= damage;

		AL_CORE_CRITICAL(unitData.tag + " now has " + std::to_string(unit.health) + " health");

		//Get the master audio source
		ECS::Entity masterAudioSource = Coordinator::Instance()->GetEntityByTag("Master Audio Source");
		Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);

		//Play hit sound accordingly
		if (unit.unitType == UNIT_TYPE::PLAYER) {
			Engine::Audio& ad = as.GetAudio(AUDIO_HIT);
			ad.m_Channel = Engine::Channel::SFX;
			ad.Play();
		}
		else {
			Engine::Audio& ad = as.GetAudio(AUDIO_ENEMY_HURT_1);
			ad.m_Channel = Engine::Channel::SFX;
			ad.Play();
		}

		//If no health
		if (unit.health <= 0) {
			//Determinte type
			if (unit.unitType == UNIT_TYPE::PLAYER) {
				AL_CORE_INFO("Unit Died");
				ECS::Entity LoseTextEntity = Coordinator::Instance()->GetEntityByTag("Win_Clear_Text");
				Coordinator::Instance()->GetComponent<Text>(LoseTextEntity).textString = "Player lost all health, press to try again";

				ECS::SetActive(true, gameplaySystem_GUI->getGuiManager().Lose_Clear);

				unitData.active = false;
				Coordinator::Instance()->GetComponent<EntityData>(unit.unit_Sprite_Entity).active = false;
			}
			else {
				//If enemy unit
				AL_CORE_INFO("Enemy Died");
				
				b8 allEnemiesDead = true;
				for (int i = 0; i < gameplaySystem->enemyEntityList.size(); ++i) {
					Unit& enemy = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->enemyEntityList[i]);

					if (enemy.health > 0) {
						allEnemiesDead = false; 
						break;
					}
				}

				if (allEnemiesDead) {
					ECS::SetActive(true, gameplaySystem_GUI->getGuiManager().Win_Clear);
				}
			}

			Coordinator::Instance()->GetComponent<EntityData>(unitEntity).active = false;
			Coordinator::Instance()->GetComponent<EntityData>(unit.unit_Sprite_Entity).active = false;
			unit.health = 0;	//Limit to 0

			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(unit.m_CurrentCell_Entity);
			cell.hasUnit = false;	//Free it's cell
		}
	}

	void GameplaySystem::DisplayFilterPlacementGrid(Room& room, Math::Vector2Int coordinate, Pattern pattern, Color color) {
		if (pattern.offsetGroup.size() == 0) {
			return;
		}

		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (gameplaySystem->IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = gameplaySystem->getEntityCell(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isAccessible) {
					continue;
				}

				cell.m_Color_Tint = color;

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.color = color;
			}
		}//End loop through pattern body check
	}

	void GameplaySystem::Toggle_Gameplay_State(b8 istrue) {
		currentGameplayStatus = (istrue ? GAMEPLAY_STATUS::RUNNING : GAMEPLAY_STATUS::STOP);
	}


	void GameplaySystem::PlacePatternOntoGrid(Room& room, Math::Vector2Int coordinate, Pattern pattern, std::string sprite_fileName) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isAccessible || cell.has_Wall) {
					continue;
				}

				//Set can walk to true
				cell.m_canWalk = true;

				//Sets the tile counter to 2 turns before it collapses
				cell.m_resetCounter = 3;

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite_fileName);
			}
		}//End loop through pattern body check
	}


	bool GameplaySystem::CheckIfPatternCanBePlacedForTile(Room& room, Math::Vector2Int coordinate, Pattern pattern) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (gameplaySystem->IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = gameplaySystem->getEntityCell(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isAccessible) {
					return false;
				}

				if (cell.m_canWalk) {
					return false;
				}
			}
			else {
				return false;
			}
		}//End loop through pattern body check

		return true;
	}

	bool GameplaySystem::CheckIfAbilitiesCanBePlacedForTile(Room& room, Math::Vector2Int coordinate, Pattern pattern, Abilities abilities) {
		bool canPlace = false;
		bool touchedUnit = false, touchedEmpty = false;

		//Shift through and check for out of bound
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			//must connect to player
			if (!IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				return false;
			}
		} //End loop through pattern body check

		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			//must connect to player
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				
				if (!cell.m_isAccessible || cell.hasBomb || cell.hasTrap || cell.has_Wall) {
					//return false;
					canPlace = false;
					touchedEmpty = true;
					break;
				}

				//If ability is direct type
				if (abilities.current_Ability_Type == ABILITY_TYPE::DIRECT) {
					if (cell.hasUnit) {
						Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

						if (unit.unitType == UNIT_TYPE::PLAYER) {
							canPlace = true;
							break;
							//return true;
						}
					}
				}
				//If it's effect type
				//Must connect to cell that is walkable but no on cells with units
				else {
					Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

					if (cell.hasUnit) {
						canPlace = false;
						touchedUnit = true;
						break;
						//return false;
					}
				}
			}
		} //End loop through pattern body check

		//If reach here and direct, means not touching player, false
		if (abilities.current_Ability_Type == ABILITY_TYPE::DIRECT) {
			return canPlace;
		}
		//If reach here and effect, means has not touch unit or cell, true
		else {
			return !touchedUnit && !touchedEmpty;
		}
	}

	void GameplaySystem::RunAbilities_OnCells(Room& room, Math::Vector2Int coordinate, Pattern pattern, Abilities* abilities) {
		AL_CORE_CRITICAL("USE ABILITY");

		abilities->current_Cooldown = abilities->max_Cooldown;
		gameplaySystem_GUI->Update_Ability_Cooldown(Abilities_List, true);

		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[selected_Pattern_Rotation].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

				switch (abilities->current_Ability_Type)
				{
					//IF DIRECT ABILITY, AFFECT UNITS ONLY
				case ABILITY_TYPE::DIRECT:
					if (cell.hasUnit) {
						AL_CORE_CRITICAL("HAS UNIT");

						Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);
						if (unit.health <= 0) {
							continue;
						}

						u32 initialHealth = unit.health;

						ECS::Entity playerEntity = Coordinator::Instance()->GetEntityByTag("Player");
						Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

						//If unit is enemy
						if (unit.unitType == UNIT_TYPE::ENEMY) {
							//Check for ability name and run ability accordingly
							switch (abilities->current_Ability_Name)
							{
								case ABILITY_NAME::HARD_DROP:
									DoDamageToUnit(cell.unitEntity, abilities->damage);
									break;
								case ABILITY_NAME::LIFE_DRAIN:
								{
									Transform playerTrans = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
									u32 healthDrained = unit.health < 0 ? initialHealth : abilities->damage;

									DoDamageToUnit(cell.unitEntity, abilities->damage);

									playerUnit.health += healthDrained;

									if (playerUnit.health > playerUnit.maxHealth) {
										playerUnit.health = playerUnit.maxHealth;
									}

									ECS::ParticleSystem::GetParticleSystem().UnitHealParticles(playerTrans.position);

									AL_CORE_CRITICAL("Heal : " + std::to_string(healthDrained) + " to player, health before " + std::to_string(playerUnit.health - healthDrained) + ", health now " + std::to_string(playerUnit.health));
									break;
								}
							} //End switch
						}//End check if unit
						else {
							//If interacted on player
							if (abilities->current_Ability_Name == ABILITY_NAME::OVERHANG) {
								//minus 8 HP
								DoDamageToUnit(playerEntity, 8);

								playerUnit.actionPoints += 1;	//Add 1 AP
								if (playerUnit.actionPoints > playerUnit.maxActionPoints) {
									playerUnit.actionPoints = playerUnit.maxActionPoints;
								}

								gameplaySystem_GUI->Update_AP_UI(playerUnit.actionPoints);
							}
						}
					}
					break;

				case ABILITY_TYPE::EFFECT:
					//Check for ability name and run ability accordingly
					switch (abilities->current_Ability_Name)
					{
					case ABILITY_NAME::CONSTRUCT_WALL:
						constructWall(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y, true);
						break;

					case ABILITY_NAME::MATRIX_TRAP:
						constructTrap(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);
						break;

					case ABILITY_NAME::VOLATILE:
						constructBomb(room, coordinate.x + pattern.offsetGroup[selected_Pattern_Rotation][i].x, coordinate.y + pattern.offsetGroup[selected_Pattern_Rotation][i].y);
						break;

					default:
						break;
					}
					break;
				} //End switch check
			}//End check if it's inside room
		}//End loop through pattern body check

		ECS::SetActive(true, gameplaySystem_GUI->getGuiManager().endTurnBtnEntity);
	}

	void GameplaySystem::MovePlayerEntityToCell(ECS::Entity cellEntity) {
		//Clear move ord
		ClearMoveOrder();

		targetCellEntity = cellEntity;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//If cell has unit, don't continue move order
		if (cell.hasUnit) {
			return;
		}

		Unit playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		startCellEntity = getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);

		//Get path
		std::vector<ECS::Entity> pathList;
		bool isPathFound = Engine::AI::FindPath(gameplaySystem, m_Room, startCellEntity, targetCellEntity, pathList, false);

		//If path not found then stop
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			return;
		}

		//// Set the move animation for player
		Animator& an = Coordinator::Instance()->GetComponent<Animator>(playerUnit.unit_Sprite_Entity);
		ECS::ChangeAnimation(an, "PlayerRun");
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;

		movingUnitEntity = playerEntity;
		gameplaySystem_GUI->UpdateGUI_OnSelectUnit(movingUnitEntity);

		ECS::SetActive(true, gameplaySystem_GUI->getGuiManager().endTurnBtnEntity);

		//Get the audiosource
		Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);

		//Play the sound
		Engine::Audio& ad = as.GetAudio(AUDIO_PLAYER_WALK_1);
		ad.m_Channel = Engine::Channel::SFX;
		ad.m_Loop = true;
		ad.Play();
	}


	void GameplaySystem::ClearMoveOrder() {
		//Clear the path list and steps
		currentModeOrder.path.clear();
		currentModeOrder.path_step = 1;
	}

	void GameplaySystem::constructWall(Room& currentRoom, u32 x, u32 y, b8 isTrue) {
		//Get the cell entity
		ECS::Entity cellEntity = getEntityCell(currentRoom, x, y);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//IF it already has a wall, don't do anything
		if (cell.has_Wall || !cell.m_isAccessible) {
			return;
		}

		//Set stats
		cell.has_Wall = isTrue;
		cell.hasTrap = false;
		cell.hasBomb = false;
		cell.m_canWalk = !isTrue;
		cell.m_resetCounter = 2;

		//Change it's sprite overlay
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cell.child_overlay);
		sprite.layer = 1000 - static_cast<u32>(Coordinator::Instance()->GetComponent<Transform>(cellEntity).position.y);
		sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/ConstructTile_TileSprite.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = true; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void GameplaySystem::constructTrap(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y) {
		//Get the cell entity
		ECS::Entity cellEntity = getEntityCell(currentRoom, x, y);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//IF it already has a wall, don't do anything
		if (cell.has_Wall || !cell.m_isAccessible) {
			return;
		}

		//Set stats
		cell.has_Wall = false;
		cell.hasTrap = true;
		cell.hasBomb = false;
		cell.m_canWalk = false;
		cell.m_resetCounter = 1000;

		//Change it's sprite overlay
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cell.child_overlay);
		sprite.layer = 1000 - static_cast<u32>(Coordinator::Instance()->GetComponent<Transform>(cellEntity).position.y);
		sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tile_Overlay_Trap.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = true; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void GameplaySystem::constructBomb(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y) {
		//Get the cell entity
		ECS::Entity cellEntity = getEntityCell(currentRoom, x, y);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//IF it already has a wall, don't do anything
		if (cell.has_Wall || !cell.m_isAccessible) {
			return;
		}

		//Set stats
		cell.has_Wall = false;
		cell.hasTrap = false;
		cell.hasBomb = true;
		cell.m_canWalk = false;
		cell.m_resetCounter = 1;

		//Change it's sprite overlay
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cell.child_overlay);
		sprite.layer = 1000 - static_cast<u32>(Coordinator::Instance()->GetComponent<Transform>(cellEntity).position.y);
		sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tile_Overlay_Bomb.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = true; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void GameplaySystem::ResetCell(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y) {
		//Get cell component 
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));

		//Set Stats
		cell.has_Wall = false;
		cell.hasBomb = false;
		cell.hasTrap = false;
		cell.m_canWalk = false;

		//Set the overlay sprite to false
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY
	
		ToggleCellWalkability(currentRoom, getEntityCell(currentRoom, x, y), false);
	}

	void GameplaySystem::destroyWall(Room& currentRoom, u32 x, u32 y, b8 isTrue) {
		//Get cell component 
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));

		//Set Stats
		cell.has_Wall = isTrue;
		cell.m_canWalk = false;

		//Set the overlay sprite to false
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY
	}

	bool GameplaySystem::CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode)
	{
		//If contain, returns true
		for (auto it = cellList.begin(); it != cellList.end(); ++it) {
			if (*it == &cellSearchNode) {
				return true;
			}
		}

		//Else false
		return false;
	}


	void GameplaySystem::SelectPattern(Pattern pattern) {
		selected_Pattern_Rotation = 0;
		//Select pattern 
		if (currentPhaseStatus == PHASE_STATUS::PHASE_SETUP) {
			//Set the placement status to be for tile
			currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE;
			selected_Pattern = pattern;

			gameplaySystem_GUI->TogglePatternGUI(false);
		}
		else if (currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
			//Set the placement status to be for abilities
			currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES;
			selected_Pattern = pattern;

			gameplaySystem_GUI->TogglePatternGUI(false);
		}
	}

	void GameplaySystem::SelectAbility(Abilities& ability) {
		//Select abilities
		if (currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
			//Get the audiosource
			Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);

			//Play the sound
			Engine::Audio& ad = as.GetAudio(AUDIO_SELECT_SKILL);
			ad.m_Channel = Engine::Channel::SFX;
			ad.Play();

			selected_Abilities = &ability;

			//Set the gui
			gameplaySystem_GUI->ToggleAbilitiesGUI(false);
			gameplaySystem_GUI->TogglePatternGUI(true);


			Unit& playerunit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

			gameplaySystem_GUI->Update_AP_UI_For_Cost(playerunit.actionPoints, ability.cost);
		}
	}

	void GameplaySystem::SetMoveOrder(std::vector<ECS::Entity> path)
	{
		gameplaySystem->currentModeOrder.path.clear();
		gameplaySystem->currentModeOrder.path_step = 1;

		for (s32 i = static_cast<s32>(path.size()) - 1; i >= 0; --i) {
			gameplaySystem->currentModeOrder.path.push_back(path[i]);
		}
	}

	void GameplaySystem::CreateAudioEntityMasterSource(void)
	{
		using namespace ECS;
		//Create Entity
		ECS::Entity en = Coordinator::Instance()->CreateEntity();

		//Get it's data
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(en);

		//Change name
		ed.tag = "Master Audio Source";

		//Prepare audiosource component
		Engine::AudioSource as;
		as.id = 0;

		//Add BGM
		Engine::Audio ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_BGM_1));
		as.list[as.id++] = ad;

		//Add Gameplay Loop
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_GAMEPLAY_LOOP));
		as.list[as.id++] = ad;

		//Add Select Skill
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_SELECT_SKILL));
		as.list[as.id++] = ad;

		//Add Player hurt
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_PLAYER_HURT));
		as.list[as.id++] = ad;

		//Add Enemy Hit
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_HIT));
		as.list[as.id++] = ad;

		//Add Enemy Hit 2
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_ENEMY_HURT_1));
		as.list[as.id++] = ad;

		//Add Audio Click (Button)
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_CLICK_1));
		as.list[as.id++] = ad;

		//Add Player Walk
		ad = Engine::AssetManager::Instance()->GetAudio(Engine::AssetManager::Instance()->GetGuid(AUDIO_PLAYER_WALK_1));
		as.list[as.id++] = ad;

		//Add the audiosource component to the entity
		Coordinator::Instance()->AddComponent(en, as);
	}

	ECS::Entity GameplaySystem::getCurrentEntityCell() {
		return currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
	}

	bool GameplaySystem::StepUpModeOrderPath(MoveOrder& order) {
		++order.path_step;

		if (order.path_step >= order.path.size()) {
			order.path_step = 1;
			return true;
		}
		else {
			return false;
		}
	}

	void GameplaySystem::UpdateUnitSpriteLayer() {
		//Update unit sprite layer accordingly from their y position
		for (s32 i = 0; i < static_cast<s32>(enemyEntityList.size()); ++i) {
			//Get the enemy component
			Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(enemyEntityList[i]);
			Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
			Sprite& enemySprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);

			//If the time is paused, put the enemy layer to the back
			if (utils::IsEqual(Time::m_Scale, 1.0f)) {
				enemySprite.layer = 1000 - static_cast<u32>(enemyTransform.position.y);
			}
			else {
				enemySprite.layer = 1;
			}
		}

		//Get the player components
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		//If the time is paused, put the player layer to the back
		if (utils::IsEqual(Time::m_Scale, 1.0f)) {
			playerSprite.layer = 1000 - static_cast<u32>(playerTransform.position.y);
		}
		else
			playerSprite.layer = 1;
	}

	b8 GameplaySystem::CheckIfWalkableOnGrid(Room& room, u32 gridX, u32 gridY)
	{
		//Get Cell entity
		ECS::Entity cellEntity = getEntityCell(room, gridX, gridY);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return false;
		}

		//Return if it's walkable
		return cell.m_canWalk;
	}

	bool GameplaySystem::IsCoordinateCellAccessible(Room& currentRoom, u32 gridX, u32 gridY) {
		//Return if Coordinate is accessible/walkable
		if (IsCoordinateInsideRoom(currentRoom, gridX, gridY)) {
			return Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, gridX, gridY)).m_isAccessible;
		}
		return false;
	}


	void GameplaySystem::RunGameStateMoving() {
		//If the gameplay system is not running anymore
		//Don't continue
		if (currentGameplayStatus == GAMEPLAY_STATUS::STOP) {
			return;
		}

		//Keep track of next cell destination
		Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(getCurrentEntityCell());

		//Keep track of player transform
		Transform& movingTransform = Coordinator::Instance()->GetComponent<Transform>(movingUnitEntity);

		//Move player transform to it's iterated waypoint
		Math::Vector2 direction = Math::Vector3::Normalize(cellTransform.localPosition - movingTransform.localPosition);

		//Move the transform of the moving to target cel
		//movingTransform.localPosition += direction * 400.0f * Time::m_FixedDeltaTime;

		//Use force
		Rigidbody2D& rigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(movingUnitEntity);
		ECS::AddForce(rigidbody, direction * 50.0f);


		//If reached the cell
		if (Math::Vector3::Distance(movingTransform.localPosition, cellTransform.localPosition) < 10.0f) {
			rigidbody.velocity = { 0,0 };
			rigidbody.acceleration = { 0,0 };

			Unit& movinUnit = Coordinator::Instance()->GetComponent<Unit>(movingUnitEntity);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getCurrentEntityCell());
			Cell& OriginCell = Coordinator::Instance()->GetComponent<Cell>(movinUnit.m_CurrentCell_Entity);

			//Free cell unit was on
			OriginCell.hasUnit = false;

			//Update player cell to current
			movinUnit.m_CurrentCell_Entity = getCurrentEntityCell();
			cell.unitEntity = movingUnitEntity;
			cell.hasUnit = true;

			//Set the position
			movingTransform.localPosition = cellTransform.localPosition;
			movinUnit.coordinate[0] = cell.coordinate.x;
			movinUnit.coordinate[1] = cell.coordinate.y;

			//Keep track of end of path
			bool isEndOfPath = false;

			//minus movement points for enemy
			--movinUnit.actionPoints;

			if (movinUnit.unitType == UNIT_TYPE::PLAYER) {
				gameplaySystem_GUI->Update_AP_UI(movinUnit.actionPoints);
			}

			//Check if enemy stepped on trap
			if (cell.hasTrap) {
				if (movinUnit.unitType == UNIT_TYPE::ENEMY) {
					movinUnit.stunDuration = 1;
					DoDamageToUnit(movingUnitEntity, 5);
					isEndOfPath = true;

					ResetCell(m_Room, cell.coordinate.x, cell.coordinate.y);
				}
			}

			//If no more movement point
			//Stop the movement
			if (movinUnit.actionPoints <= 0) {
				isEndOfPath = true;
			}
			else {
				if(!isEndOfPath)
				isEndOfPath = StepUpModeOrderPath(currentModeOrder);
			}

			AL_CORE_INFO("Movement Points " + std::to_string(movinUnit.actionPoints));

			//If reached the end of path
			if (isEndOfPath) {
				currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;
				//If player, end turn
				if (movinUnit.unitType == UNIT_TYPE::PLAYER) {
					//Get the audiosource
					Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);

					//Stop the sound
					Engine::Audio& ad = as.GetAudio(AUDIO_PLAYER_WALK_1);
					ad.m_Channel = Engine::Channel::SFX;
					ad.m_Loop = false;
					ad.Stop();

					Animator& an = Coordinator::Instance()->GetComponent<Animator>(movinUnit.unit_Sprite_Entity);
					ECS::ChangeAnimation(an, "PlayerIdle");
					if (movinUnit.actionPoints <= 0) {
						EndTurn();
					}
				}
				//If enemy, move on to next enemy
				else if (movinUnit.unitType == UNIT_TYPE::ENEMY) {
					if (movinUnit.enemyUnitType == ENEMY_TYPE::ENEMY_MELEE) {
						//Stop movement
						Animator& an = Coordinator::Instance()->GetComponent<Animator>(movinUnit.unit_Sprite_Entity);
						ECS::ChangeAnimation(an, "GuardIdle");
						gameplaySystem_Enemy->RunEnemyAdjacentAttack(m_Room, Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved - 1]));
					}
					else if (movinUnit.enemyUnitType == ENEMY_TYPE::ENEMY_CELL_DESTROYER) {
						//Destroy the tiles when reached destination
						gameplaySystem_Enemy->Enemy_Logic_CellDestroyer_DestroyTile(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
					}

					//Run enemy logic
					MoveEnemy();
				}
				return;
			}
		}
	}

	void GameplaySystem::MoveEnemy() {
		//EnemyManager_LoadData();

		//Clear move order
		ClearMoveOrder();

		//If reached end, end turn
		if (enemyNeededData.enemyMoved >= enemyEntityList.size()) {
			AL_CORE_INFO("All Enemy Made move, ending turn");
			EndTurn();
			return;
		}

		//Get enemy unit component
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyNeededData.enemyMoved]);

		//if the health is gone
		if (enemyUnit.health <= 0)
		{
			//MOve on to the next enemy
			++enemyNeededData.enemyMoved;
			MoveEnemy();
			return;
		}

		//Check stun
		if (enemyUnit.stunDuration > 0) {
			enemyUnit.stunDuration--;
			
			//MOve on to the next enemy
			++enemyNeededData.enemyMoved;
			MoveEnemy();
			return;
		}

		//use enemy logic function pointer
		switch (enemyUnit.enemyUnitType)
		{
		case ENEMY_TYPE::ENEMY_MELEE:
			gameplaySystem_Enemy->Enemy_Logic_Update_Melee(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			break;
		case ENEMY_TYPE::ENEMY_CELL_DESTROYER:
			gameplaySystem_Enemy->Enemy_Logic_Update_CellDestroyer(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			break;

		case ENEMY_TYPE::ENEMY_SUMMONER:
			gameplaySystem_Enemy->Enemy_Logic_Update_Summoner(enemyNeededData, movingUnitEntity, currentUnitControlStatus, enemyEntityList, m_Room);
			break;

		default:
			break;
		}
		AL_CORE_INFO("after enemy move");
	}

	void GameplaySystem::RotatePattern(s32 patternRotationAmount) {
		if (currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::NOTHING) {
			return;
		}

		Event_MouseExitCell(current_Moused_Over_Cell);

		selected_Pattern_Rotation += patternRotationAmount;


		if (selected_Pattern_Rotation < 0) {
			selected_Pattern_Rotation = selected_Pattern.offsetGroup.size() - 1;
		}
		else if (selected_Pattern_Rotation > selected_Pattern.offsetGroup.size() - 1) {
			selected_Pattern_Rotation = 0;
		}

		Event_MouseEnterCell(current_Moused_Over_Cell);
	}


	//****************EVENTS*****************//
	/*!*********************************************************************************
	\brief
		Restart the level
	***********************************************************************************/
	void Event_Button_Restart([[maybe_unused]] ECS::Entity invoker) {
		//Restart the gameplay
		//gameplaySystem->Toggle_Gameplay_State(false);
		Engine::Scene::Restart();
	}


	/*!*********************************************************************************
	\brief
		Select Ability 0
	***********************************************************************************/
	void Event_Button_Select_Abilities_0([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 0");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[0]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}

	/*!*********************************************************************************
	\brief
		Select Ability 1
	***********************************************************************************/
	void Event_Button_Select_Abilities_1([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 1");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[1]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}

	/*!*********************************************************************************
	\brief
		Select Ability 2
	***********************************************************************************/
	void Event_Button_Select_Abilities_2([[maybe_unused]] ECS::Entity invoker) { //CONSTRUCT WALL SKILL
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Construct Wall");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[2]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}

	/*!*********************************************************************************
	\brief
		Select Ability 3
	***********************************************************************************/
	void Event_Button_Select_Abilities_3([[maybe_unused]] ECS::Entity invoker) { //CONSTRUCT WALL SKILL
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Construct Wall");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[3]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}

	/*!*********************************************************************************
	\brief
		Select Ability 4
	***********************************************************************************/
	void Event_Button_Select_Abilities_4([[maybe_unused]] ECS::Entity invoker) { //CONSTRUCT WALL SKILL
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Construct Wall");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[4]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}

	/*!*********************************************************************************
	\brief
		Select Ability 5
	***********************************************************************************/
	void Event_Button_Select_Abilities_5([[maybe_unused]] ECS::Entity invoker) { //CONSTRUCT WALL SKILL
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Construct Wall");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[5]);
		gameplaySystem_GUI->DisableToolTipGUI();
	}
	/*!*********************************************************************************
	\brief
		Select Ability 3
	***********************************************************************************/
	void Event_Button_Select_CurrentPattern([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Current Pattern");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[0]);
		gameplaySystem->selected_Pattern_Index = 0;
	}

	/*!*********************************************************************************
	\brief
		Select Ability 4
	***********************************************************************************/
	void Event_Button_Select_Pattern_1([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 1");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[1]);
		gameplaySystem->selected_Pattern_Index = 1;
	}

	/*!*********************************************************************************
	\brief
		Select Ability 5
	***********************************************************************************/
	void Event_Button_Select_Pattern_2([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 2");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[2]);
		gameplaySystem->selected_Pattern_Index = 2;
	}

	/*!*********************************************************************************
	\brief
		Select Ability 6
	***********************************************************************************/
	void Event_Button_Select_Pattern_3([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 3");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[3]);
		gameplaySystem->selected_Pattern_Index = 3;
	}

	/*!*********************************************************************************
	\brief
		Event for GUI button end turn
	***********************************************************************************/
	void Event_Button_Select_EndTurn([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		//End turn
		gameplaySystem->EndTurn();
		gameplaySystem->buttonClickAudio->Play();
	}

	/*!*********************************************************************************
	\brief
		Event for when mouse enter cell
	***********************************************************************************/
	void Event_MouseEnterCell(ECS::Entity invoker) {
		//Keep track of cell the mouse is interacting with
		gameplaySystem->current_Moused_Over_Cell = invoker;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		//If cell is not accessible, then ignore
		if (!cell.m_isAccessible) {
			return;
		}

		//If placement status is being used
		//Determine is setup or abilities
		if (gameplaySystem->currentPatternPlacementStatus != PATTERN_PLACEMENT_STATUS::NOTHING) {
			//If checking for setup, if so, then filter for placement
			if (gameplaySystem->currentPhaseStatus == PHASE_STATUS::PHASE_SETUP) {
				b8 canPlace = gameplaySystem->CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

				if (canPlace)
					gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
					gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
			//If checking for abilities, if so, then filter for placement
			else if (gameplaySystem->currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
				b8 canPlace = gameplaySystem->CheckIfAbilitiesCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, *gameplaySystem->selected_Abilities);

				if (canPlace)
					gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
					gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
		}//End check for pattern placement
	}

	/*!*********************************************************************************
	\brief
		Event for when  mouse exit cell
	***********************************************************************************/
	void Event_MouseExitCell(ECS::Entity invoker) {
		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		if (gameplaySystem->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::NOTHING) {
			return;
		}

		//Filter it's placement 
		gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
	}


	/*!*********************************************************************************
	\brief
		Event for when mouse enter unit
	***********************************************************************************/
	void Event_MouseEnterUnit(ECS::Entity invoker) {
		if (gameplaySystem->currentPatternPlacementStatus != PATTERN_PLACEMENT_STATUS::NOTHING) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(invoker);
			Event_MouseEnterCell(unit.m_CurrentCell_Entity);
		}
	}

	/*!*********************************************************************************
	\brief
		Event for when mouse exit unit
	***********************************************************************************/
	void Event_MouseExitUnit(ECS::Entity invoker) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(invoker);
		Event_MouseExitCell(unit.m_CurrentCell_Entity);
	}

	/*!*********************************************************************************
	\brief
		Event for when mouse click on cell
	***********************************************************************************/
	void Event_ClickCell(ECS::Entity invokerCell) {
		AL_CORE_INFO("Select Cell");

		//If the unit control is currently moving unit, ignore click order
		if (gameplaySystem->currentUnitControlStatus != UNITS_CONTROL_STATUS::NOTHING) {
			return;
		}

		//Get cell component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invokerCell);
		//If not placing, move character
		if (gameplaySystem->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::NOTHING &&
			gameplaySystem->currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
			//When it's action, for moving player
			//When click on cell, Move the player unit to the selected cell
			gameplaySystem->MovePlayerEntityToCell(invokerCell);
		}
		else if (gameplaySystem->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE) {
			//If can place
			b8 canPlace = gameplaySystem->CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

			if (!canPlace && !gameplaySystem->godMode) {
				return;
			}

			//Get the audiosource
			Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(gameplaySystem->masterAudioSource);

			//Play the sound
			Engine::Audio& ad = as.GetAudio(AUDIO_CLICK_1);
			ad.m_Channel = Engine::Channel::BGM;
			ad.Play();

			//Switch off the display pattern
			gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });

			//Place it onto the grid
			gameplaySystem->PlacePatternOntoGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, "Assets/Images/Walkable.png");

			// Remove from list
			gameplaySystem->pattern_List.erase(gameplaySystem->pattern_List.begin() + gameplaySystem->selected_Pattern_Index);

			if (gameplaySystem->pattern_List.size() <= gameplaySystem->pattern_Default.size())
				gameplaySystem->RandomizePatternList();

			// Set sprites for the Patterns
			for (u32 i{ 1 }; i <= 4; ++i)
			{
				std::string tile_icon = "next_tile_icon" + std::to_string(i);

				ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
				sprite.id = Engine::AssetManager::Instance()->GetGuid(gameplaySystem->pattern_List[i - 1].file_path);
			}

			//End turn
			gameplaySystem->EndTurn();
		}
		else if (gameplaySystem->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES) {
			Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
			
			if (playerUnit.actionPoints < 2) {
				return;
			}
			
			//If placing patern for abilities
			b8 canPlace = gameplaySystem->CheckIfAbilitiesCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, *gameplaySystem->selected_Abilities);

			if (!canPlace && !gameplaySystem->godMode) {
				return;
			}

			playerUnit.actionPoints -= gameplaySystem->selected_Abilities->cost;
			//Disable the filter
			gameplaySystem->DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });

			//Run the abilities on the cell
			gameplaySystem->RunAbilities_OnCells(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, gameplaySystem->selected_Abilities);

			// Remove from list
			gameplaySystem->pattern_List.erase(gameplaySystem->pattern_List.begin() + gameplaySystem->selected_Pattern_Index);

			if (gameplaySystem->pattern_List.size() <= gameplaySystem->pattern_Default.size())
				gameplaySystem->RandomizePatternList();

			// Set sprites for the Patterns
			for (u32 i{ 1 }; i <= 4; ++i)
			{
				std::string tile_icon = "next_tile_icon" + std::to_string(i);

				ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
				sprite.id = Engine::AssetManager::Instance()->GetGuid(gameplaySystem->pattern_List[i - 1].file_path);
			}


			if (playerUnit.actionPoints < 0) {
				playerUnit.actionPoints = 0;
			}

			gameplaySystem_GUI->Update_AP_UI(playerUnit.actionPoints);

			if (playerUnit.actionPoints <= 0) {
				gameplaySystem->EndTurn();
			}
			else {
				//Reset the statuses
				gameplaySystem->currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;
				gameplaySystem->currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;
				gameplaySystem_GUI->ToggleAbilitiesGUI(true);
				gameplaySystem_GUI->TogglePatternGUI(false);
			}

			s8 eliminatedEnemyCount = 0;
			//Check if all enemies are eliminated
			for (s8 i = 0; i < gameplaySystem->enemyEntityList.size(); ++i) {
				Unit& enemy = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->enemyEntityList[i]);

				if (enemy.health <= 0) {
					++eliminatedEnemyCount;
				}
			}

			//If all enemy is cleared, set the win to true
			if (eliminatedEnemyCount >= gameplaySystem->enemyEntityList.size()) {
				ECS::SetActive(true, gameplaySystem_GUI->getGuiManager().Win_Clear);
			}
		}
	}

	void Event_Unit_OnSelect([[maybe_unused]] ECS::Entity invoker)
	{
		//If paused then don't do anything
		if (utils::IsEqual(Time::m_Scale, 0.f))
		{
			return;
		}

		//If placement is doing something
		if (gameplaySystem->currentPatternPlacementStatus != PATTERN_PLACEMENT_STATUS::NOTHING) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(invoker);
			Event_ClickCell(unit.m_CurrentCell_Entity);
		}
		else {
			AL_CORE_INFO("DISPLAY UNIT");
			gameplaySystem_GUI->UpdateGUI_OnSelectUnit(invoker);
		}
	}
	//Event End
}