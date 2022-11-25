/*!
file:	GameplaySystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for GameplaySystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"
#include "Engine/PathFindingManager.h"
#include "Engine/GamePlayInterface.h"

namespace ALEngine::ECS
{
	//Ease of use for ray
	using Engine::Physics::Ray2D;
	using Engine::GameplayInterface::Room;
	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	using GameplayInterface::Pattern;
	using GameplayInterface::Abilities;

	class GameplaySystem : public System
	{
		public:
		enum class GAMEPLAY_STATUS
		{
			PHASE_SETUP,
			PHASE_ACTION,
			PHASE_ENEMY,
		};

		enum class UNITS_CONTROL_STATUS {
			NOTHING,
			UNIT_MOVING,
			UNIT_ATTACKING
		};

		enum class PATTERN_PLACEMENT_STATUS {
			NOTHING, 
			PLACING_FOR_TILE,
			PLACING_FOR_ABILITIES
		};

		struct MoveOrder {
			Entity entity;
			std::vector<Entity> path;
			u32 path_step = 0;
		};

		//******VARIABLES**********//
		u32 roomSize[2]{ 6, 6 };
		Room m_Room;

		Entity playerEntity, startCellEntity, targetCellEntity;

		//Enemy
		std::vector<Entity> enemyEntityList;

		GAMEPLAY_STATUS currentGameplayStatus = GAMEPLAY_STATUS::PHASE_SETUP;
		UNITS_CONTROL_STATUS currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;
		PATTERN_PLACEMENT_STATUS currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;

		MoveOrder currentModeOrder;

		std::vector<Entity> GUI_Abilities_Button_List;
		std::vector<Entity> GUI_Pattern_Button_List;

		Entity m_Room_Parent_Entity;

		//Patterns
		std::vector<Pattern> pattern_List;
		Pattern selected_Pattern;

		//Abilities
		std::vector<Abilities> Abilities_List;
		Abilities selected_Abilities;

		Entity current_Moused_Over_Cell;

		//UI
		Entity endTurnBtnEntity;

		//******FUNCTIONS**********//
		void ClearMoveOrder();
		Entity getCurrentEntityCell();
		void SetMoveOrder(std::vector<Entity> path);

		//Select Pattern
		void SelectPattern(Pattern pattern);

		//Select Abilities
		void SelectAbility(Abilities ability);

		//Return if reached end
		bool StepUpModeOrderPath(MoveOrder& order);
		u32 getRoomSize();
		void RunGameState();
		void UpdateUnitSpriteLayer();

		void MovePlayerEntityToCell(Entity cellEntity);

		void RunGameStateWaitPlayerInput();
		void RunGameStateMoving();

		void EndTurn();

		void TogglePatternGUI(b8 istrue);
		void ToggleAbilitiesGUI(b8 istrue);

		void PlaceNewPlayerInRoom(s32 x, s32 y);
		void PlaceNewEnemyInRoom(s32 x, s32 y);

		//Creating Object
		void InitializeEndTurnButton();
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> gameplaySystem;

		std::string const sceneName = R"(Assets\test.scene)";

		s32 base_Layer = 10000;
	}

	void Event_Button_Darken(Entity invoker) {
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		sprite.color = { 0.6f, 0.6f, 0.6f, 1.f };
	}

	void Event_Button_Lighten(Entity invoker) {
		EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(invoker);

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		
		if(eventTrigger.isEnabled)
		sprite.color = { 1.f, 1.f, 1.f, 1.f };
	}

	void Event_Button_Select_Abilities_0(Entity invoker) {
		AL_CORE_INFO("Select Abilities 0");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[0]);
	}

	void Event_Button_Select_CurrentPattern(Entity invoker) {
		AL_CORE_INFO("Select Current Pattern");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[0]);
	}

	void Event_Button_Select_Pattern_1(Entity invoker) {
		AL_CORE_INFO("Select Pattern 1");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[1]);
	}
	
	void Event_Button_Select_Pattern_2(Entity invoker) {
		AL_CORE_INFO("Select Pattern 2");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[2]);
	}
	
	void Event_Button_Select_Pattern_3(Entity invoker) {
		AL_CORE_INFO("Select Pattern 3");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[3]);
	}

	void Event_Button_Select_EndTurn(Entity invoker) {
		//End turn
		gameplaySystem->EndTurn();
	}

	void Event_MouseEnterCell(Entity invoker) {
		//AL_CORE_INFO("Enter Cell");

		gameplaySystem->current_Moused_Over_Cell = invoker;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		if (gameplaySystem->currentPatternPlacementStatus != GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING) {
			if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_SETUP) {
				b8 canPlace = GameplayInterface::CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);
			
				if(canPlace)
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
			else if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_ACTION) {
				//GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });

				b8 canPlace = GameplayInterface::CheckIfAbilitiesCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

				if (canPlace)
					GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
					GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
		}//End check for pattern placement
	}

	void Event_MouseExitCell(Entity invoker) {
		//AL_CORE_INFO("Exit Cell");
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
	}

	void Event_ClickCell(Entity invokerCell) {
		AL_CORE_INFO("Select Cell");

		if (gameplaySystem->currentUnitControlStatus != GameplaySystem::UNITS_CONTROL_STATUS::NOTHING) {
			return;
		}

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invokerCell);
		//If not placing, move character
		if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING && 
			gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_ACTION) {
			//When click on cell, Move the player unit to the selected cell
			gameplaySystem->MovePlayerEntityToCell(invokerCell);
		}
		else if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE) {
			GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
			GameplayInterface::PlacePatternOntoGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, "Assets/Images/Walkable.png");
			gameplaySystem->EndTurn();
		}
	}

	void RegisterGameplaySystem(void)
	{
		gameplaySystem = Coordinator::Instance()->RegisterSystem<GameplaySystem>();
		Signature signature;
		Coordinator::Instance()->SetSystemSignature<GameplaySystem>(signature);
	}

	void StartGameplaySystem(void) {
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		AL_CORE_INFO("GAME START");
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		gameplaySystem->m_Room.width = gameplaySystem->roomSize[0];
		gameplaySystem->m_Room.height = gameplaySystem->roomSize[1];
		gameplaySystem->m_Room.roomSize = gameplaySystem->getRoomSize();
		gameplaySystem->m_Room.roomCellsArray = new Entity[gameplaySystem->getRoomSize()];
		
		gameplaySystem->currentGameplayStatus = GameplaySystem::GAMEPLAY_STATUS::PHASE_SETUP;
		gameplaySystem->currentUnitControlStatus = GameplaySystem::UNITS_CONTROL_STATUS::NOTHING;
		gameplaySystem->currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;
		
		//Initialize Room Parent 
		gameplaySystem->m_Room_Parent_Entity = Coordinator::Instance()->CreateEntity();
		Coordinator::Instance()->AddComponent(gameplaySystem->m_Room_Parent_Entity, Transform{});
		Coordinator::Instance()->GetComponent<EntityData>(gameplaySystem->m_Room_Parent_Entity).tag = "Room";

		sceneGraph.Push(-1, gameplaySystem->m_Room_Parent_Entity); // first cell is parent

		//Initialize Pattern
		InitializePatterns(gameplaySystem->pattern_List);

		//Initialize Abilities
		InitializeAbilities(gameplaySystem->Abilities_List);

		for (uint32_t i = 0; i < gameplaySystem->getRoomSize(); ++i) {
			gameplaySystem->m_Room.roomCellsArray[i] = Coordinator::Instance()->CreateEntity();

			sceneGraph.Push(gameplaySystem->m_Room_Parent_Entity, gameplaySystem->m_Room.roomCellsArray[i]); // other cells are children of the parent

			Transform transform;
			transform.scale = { 85, 85 };
			transform.localScale = { 100, 100 };

			Coordinator::Instance()->AddComponent(gameplaySystem->m_Room.roomCellsArray[i], transform);

			CreateSprite(gameplaySystem->m_Room.roomCellsArray[i],"Assets/Images/InitialTile_v04.png");
		}

		for (s32 i = 0; i < gameplaySystem->roomSize[0]; ++i) {
			for (s32 j = 0; j < gameplaySystem->roomSize[1]; ++j) {
				int cellIndex = i * gameplaySystem->roomSize[0] + j;

				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[cellIndex]);
				transform.position = { 550 + (f32)j * 100.f, 300 + (f32)i * 100.f };

				Cell cell;
				cell.coordinate = { i,j };

				CreateEventTrigger(gameplaySystem->m_Room.roomCellsArray[cellIndex]);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_ClickCell);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterCell);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitCell);

				Coordinator::Instance()->AddComponent(getEntityCell(gameplaySystem->m_Room, i, j), cell);

				Coordinator::Instance()->GetComponent<EntityData>(getEntityCell(gameplaySystem->m_Room,i,j)).tag = "Cell[" + std::to_string(i) + "," + std::to_string(j) + "]";
			}
		}

		//Create Player
		gameplaySystem->PlaceNewPlayerInRoom(0, 0);

		gameplaySystem->PlaceNewEnemyInRoom(0, 1);

		//Create EndTurn Button
		gameplaySystem->InitializeEndTurnButton();

		//Initialize Pattern GUI
		GameplayInterface::InitializePatternGUI(gameplaySystem->GUI_Pattern_Button_List);

		//Add events for pattern Button
		Subscribe(gameplaySystem->GUI_Pattern_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_CurrentPattern);
		Subscribe(gameplaySystem->GUI_Pattern_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_1);
		Subscribe(gameplaySystem->GUI_Pattern_Button_List[2], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_2);
		Subscribe(gameplaySystem->GUI_Pattern_Button_List[3], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_3);

		//Add visual feedback event for pattern GUI
		for (int i = 0; i < gameplaySystem->GUI_Pattern_Button_List.size(); ++i) {
			Subscribe(gameplaySystem->GUI_Pattern_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
			Subscribe(gameplaySystem->GUI_Pattern_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
			Subscribe(gameplaySystem->GUI_Pattern_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		}


		//Initialize abilities GUI
		GameplayInterface::InitializeAbilitiesGUI(gameplaySystem->GUI_Abilities_Button_List);

		//Add events for abilities Button
		Subscribe(gameplaySystem->GUI_Abilities_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_0);

		//Add visual feedback event for abilities GUI
		for (int i = 0; i < gameplaySystem->GUI_Abilities_Button_List.size(); ++i) {
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		}
		
		//Set a few blocks to be inaccessible
		ToggleCellToInaccessible(gameplaySystem->m_Room, 1, 0, false);
		ToggleCellToInaccessible(gameplaySystem->m_Room, 1, 1, false);
		ToggleCellToInaccessible(gameplaySystem->m_Room, 1, 2, false);
		ToggleCellToInaccessible(gameplaySystem->m_Room, 2, 1, false);
		ToggleCellToInaccessible(gameplaySystem->m_Room, 3, 1, false);
		ToggleCellToInaccessible(gameplaySystem->m_Room, 3, 2, false);

		//Set abilities UI off
		gameplaySystem->ToggleAbilitiesGUI(false);
	}

	void UpdateGameplaySystem(void)
	{
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive()) {
			return;
		}

		//If right mouse button
		if (Input::KeyDown(KeyCode::MouseRightButton)) {
			//Deselect Pattern
			if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_SETUP) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->current_Moused_Over_Cell);

				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
				gameplaySystem->currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

				gameplaySystem->TogglePatternGUI(true);
			}
			//Deselect Abilities
			else if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_ACTION) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->current_Moused_Over_Cell);

				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
				gameplaySystem->currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

				gameplaySystem->TogglePatternGUI(false);
				gameplaySystem->ToggleAbilitiesGUI(true);
			}
		}

		gameplaySystem->RunGameState();
		
		gameplaySystem->UpdateUnitSpriteLayer();
	}

	void ExitGameplaySystem(void)
	{
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (gameplaySystem->m_Room.roomCellsArray != nullptr) {
			delete[] gameplaySystem->m_Room.roomCellsArray;
			gameplaySystem->m_Room.roomCellsArray = nullptr;
		}

		gameplaySystem->m_Room.width = 0;
		gameplaySystem->m_Room.height = 0;
	}

	Entity GameplaySystem::getCurrentEntityCell() {
		return gameplaySystem->currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
	}

	void GameplaySystem::EndTurn() {
		currentUnitControlStatus = GameplaySystem::UNITS_CONTROL_STATUS::NOTHING;
		currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;
		
		switch (currentGameplayStatus) {
			case GAMEPLAY_STATUS::PHASE_SETUP:
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_ACTION;

				AL_CORE_INFO("Loading PHASE ACTION");

				ToggleAbilitiesGUI(true);
				TogglePatternGUI(false);
			break;

			case GAMEPLAY_STATUS::PHASE_ACTION:
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_ENEMY;

				ToggleAbilitiesGUI(false);
				TogglePatternGUI(false);
				AL_CORE_INFO("Loading PHASE ENEMY");
				break;
			
			case GAMEPLAY_STATUS::PHASE_ENEMY:
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_SETUP;

				TogglePatternGUI(true);
				AL_CORE_INFO("Loading PHASE SETUP");
				break;
		}
	}

	void GameplaySystem::TogglePatternGUI(b8 istrue) {
		for (int i = 0; i < GUI_Pattern_Button_List.size(); ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(GUI_Pattern_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(GUI_Pattern_Button_List[i]);

			eventTrigger.isEnabled = istrue;

			if(istrue)
			sprite.color = { 1.f, 1.f, 1.f, 1.f };
			else
			sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
	}
	
	void GameplaySystem::ToggleAbilitiesGUI(b8 istrue) {
		for (int i = 0; i < GUI_Abilities_Button_List.size(); ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(GUI_Abilities_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = false;
			sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}

		for (int i = 0; i < 1; ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(GUI_Abilities_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = istrue;

			if (istrue)
				sprite.color = { 1.f, 1.f, 1.f, 1.f };
			else
				sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
	}

	void GameplaySystem::PlaceNewPlayerInRoom(s32 x, s32 y) {
		gameplaySystem->playerEntity = Coordinator::Instance()->CreateEntity();
		CreatePlayerUnit(gameplaySystem->playerEntity);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		playerUnit.coordinate[0] = x;
		playerUnit.coordinate[1] = y;

		playerUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(gameplaySystem->m_Room, x, y);

		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).unitEntity = gameplaySystem->playerEntity;
		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).hasUnit = true;

		GameplayInterface::PlaceWalkableOnGrid(gameplaySystem->m_Room, { x, y }, "Assets/Images/Walkable.png");

		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(gameplaySystem->m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]));
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->playerEntity);
		playerTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::PlaceNewEnemyInRoom(s32 x, s32 y) {
		Entity newEnemy = Coordinator::Instance()->CreateEntity();

		enemyEntityList.push_back(newEnemy);

		CreateEnemyUnit(newEnemy);
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(newEnemy);
		enemyUnit.coordinate[0] = x;
		enemyUnit.coordinate[1] = y;

		enemyUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(gameplaySystem->m_Room, x, y);

		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).unitEntity = newEnemy;
		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).hasUnit = true;
		
		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(gameplaySystem->m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
		Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(newEnemy);
		enemyTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::SelectPattern(Pattern pattern) { 
		if (currentGameplayStatus == GAMEPLAY_STATUS::PHASE_SETUP) {
			currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE;
			selected_Pattern = pattern;

			TogglePatternGUI(false);
		}
		else if (currentGameplayStatus == GAMEPLAY_STATUS::PHASE_ACTION) {
			currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES;
			selected_Pattern = pattern;

			TogglePatternGUI(false);
		}
	}

	void GameplaySystem::SelectAbility(Abilities ability) {
		if (currentGameplayStatus == GAMEPLAY_STATUS::PHASE_ACTION) {
			selected_Abilities = ability;

			ToggleAbilitiesGUI(false);
			TogglePatternGUI(true);
		}
	}

	void GameplaySystem::ClearMoveOrder() {
		currentModeOrder.path.clear();
		currentModeOrder.path_step = 0;
	}

	void GameplaySystem::SetMoveOrder(std::vector<Entity> path) {
		currentModeOrder.path.clear();
		currentModeOrder.path_step = 0;

		for (int i = path.size() - 1; i >= 0; --i) {
			currentModeOrder.path.push_back(path[i]);
		}
	}

	uint32_t GameplaySystem::getRoomSize() {
		return gameplaySystem->roomSize[0] * gameplaySystem->roomSize[1];
	}

	bool GameplaySystem::StepUpModeOrderPath(MoveOrder& order) {
		++order.path_step;

		if (order.path_step >= order.path.size()) {
			order.path_step = 0;
			return true;
		}
		else {
			return false;
		}
	}

	void GameplaySystem::RunGameState() {
		switch (currentUnitControlStatus)
		{
			case UNITS_CONTROL_STATUS::NOTHING:

			break;

			case UNITS_CONTROL_STATUS::UNIT_MOVING:
				RunGameStateMoving();
				return;

			case UNITS_CONTROL_STATUS::UNIT_ATTACKING:

				break;
		}

		switch (currentGameplayStatus) {
		case GAMEPLAY_STATUS::PHASE_SETUP:
			//EndTurn();
			break;

		case GAMEPLAY_STATUS::PHASE_ACTION:
			RunGameStateWaitPlayerInput();
			break;

		case GAMEPLAY_STATUS::PHASE_ENEMY:
			//EndTurn();
			break;
		}
	}

	void GameplaySystem::UpdateUnitSpriteLayer() {
		for (int i = 0; i < enemyEntityList.size(); ++i) {
			Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(enemyEntityList[i]);
			Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
			Sprite& enemySprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);

			enemySprite.layer = base_Layer - enemyTransform.localPosition.y;
		}

		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		playerSprite.layer = base_Layer - playerTransform.localPosition.y;
	}

	void CreatePlayerUnit(Entity const& entity) {
		Unit unit{};
		unit.unitType = UNIT_TYPE::PLAYER;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		Transform& playertransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		playertransform.scale = { 50, 50 };
		playertransform.localScale = { 100, 100 };


		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		playerUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();

		Transform playerSpriteTransform;
		playerSpriteTransform.localPosition = { 0.f, 0.4f };
		playerSpriteTransform.localScale = { 1.f, 2.f };

		CreateSprite(playerUnit.unit_Sprite_Entity, playerSpriteTransform, "Assets/Images/Player v2.png");
		
		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Player";
		Coordinator::Instance()->GetComponent<EntityData>(playerUnit.unit_Sprite_Entity).tag = "Player_Sprite";

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, playerUnit.unit_Sprite_Entity);
	}

	void CreateEnemyUnit(Entity const& entity) {
		Unit unit{};
		unit.unitType = UNIT_TYPE::ENEMY;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		Transform& playertransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		playertransform.scale = { 50, 50 };
		playertransform.localScale = { 100, 100 };


		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		enemyUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();

		Transform enemySpriteTransform;
		enemySpriteTransform.localPosition = { 0.f, 0.4f };
		enemySpriteTransform.localScale = { 1.f, 2.f };

		CreateSprite(enemyUnit.unit_Sprite_Entity, enemySpriteTransform, "Assets/Images/Bishop v.02.png");

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Enemy_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);
		Coordinator::Instance()->GetComponent<EntityData>(enemyUnit.unit_Sprite_Entity).tag = "Enemy_Sprite_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, enemyUnit.unit_Sprite_Entity);
	}

	void GameplaySystem::MovePlayerEntityToCell(Entity cellEntity) {
		gameplaySystem->ClearMoveOrder();

		gameplaySystem->targetCellEntity = cellEntity;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		Unit playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		gameplaySystem->startCellEntity = getEntityCell(gameplaySystem->m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);

		std::vector<ECS::Entity> pathList;
		bool isPathFound = Engine::AI::FindPath(gameplaySystem->m_Room, gameplaySystem->startCellEntity, gameplaySystem->targetCellEntity, pathList);

		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			return;
		}

		gameplaySystem->SetMoveOrder(pathList);

		gameplaySystem->currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
	}

	void GameplaySystem::RunGameStateWaitPlayerInput() {

	}

	void GameplaySystem::RunGameStateMoving() {
		//DirectionTomove
		Entity nextPathEtity = currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];

		//Keep track of next cell destination
		Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(getCurrentEntityCell());

		//Keep track of player transform
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);

		//Move player transform to it's iterated waypoint
		Vector2 direction = Vector3::Normalize(cellTransform.localPosition - playerTransform.localPosition);

		playerTransform.localPosition += direction * 500.0f * Time::m_DeltaTime;

		if (Vector3::Distance(playerTransform.localPosition, cellTransform.localPosition) < 10.0f) {
			Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getCurrentEntityCell());

			playerTransform.localPosition = cellTransform.localPosition;
			playerUnit.coordinate[0] = cell.coordinate.x;
			playerUnit.coordinate[1] = cell.coordinate.y;

			bool isEndOfPath = StepUpModeOrderPath(currentModeOrder);

			if (isEndOfPath) {
				currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;

				Cell& OriginCell = Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity);

				OriginCell.hasUnit = false;
				OriginCell.unitEntity = 0;

				//Update player cell to current
				playerUnit.m_CurrentCell_Entity = gameplaySystem->getCurrentEntityCell();
				cell.unitEntity = gameplaySystem->playerEntity;
				cell.hasUnit = true;

				if (playerUnit.unitType == UNIT_TYPE::PLAYER) {
					EndTurn();
				}
				return;
			}
		}
	}

	void GameplaySystem::InitializeEndTurnButton() {
		//endTurnBtnEntity = Coordinator::Instance()->CreateEntity();
		endTurnBtnEntity = Coordinator::Instance()->GetEntityByTag("end_turn");
		
		EventTrigger eventTrigger;		
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_EndTurn);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		Coordinator::Instance()->AddComponent(endTurnBtnEntity, eventTrigger);
	}


	void DrawGameplaySystem() {
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
		//Box holder
		Vector2 bottomleft;
		Vector2 topright;

		Color color = { 0.f,0.f ,0.f ,0.f };

		//Draw all cells
		for (int i = 0; i < gameplaySystem->m_Room.roomSize; ++i) {
			Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[i]);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->m_Room.roomCellsArray[i]);

			if (!cell.m_isBlocked) {
				color = { 1.f, 0.f, 0.f, 1.f };
			}
			else {
				color = cell.m_Color_Tint;
			}

			bottomleft = { cellTransform.position.x - cellTransform.scale.x * 0.5f, cellTransform.position.y - cellTransform.scale.y * 0.5f };
			topright = { cellTransform.position.x + cellTransform.scale.x * 0.5f, cellTransform.position.y + cellTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}

		//Draw the pathfinder 
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->playerEntity);
		bottomleft = { playerTransform.position.x - playerTransform.scale.x * 0.5f, playerTransform.position.y - playerTransform.scale.y * 0.5f };
		topright = { playerTransform.position.x + playerTransform.scale.x * 0.5f, playerTransform.position.y + playerTransform.scale.y * 0.5f };

		color = { 1.f, 1.f, 0.f, 1.f };

		Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
		Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
		Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
		Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
	
		//Draw the Pattern GUI
		for (int i = 0; i < gameplaySystem->GUI_Pattern_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->GUI_Pattern_Button_List[i]);

			bottomleft = { buttonTransform.position.x - buttonTransform.scale.x * 0.5f, buttonTransform.position.y - buttonTransform.scale.y * 0.5f };
			topright = { buttonTransform.position.x + buttonTransform.scale.x * 0.5f, buttonTransform.position.y + buttonTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}

		//Draw the Pattern GUI
		for (int i = 0; i < gameplaySystem->GUI_Abilities_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->GUI_Abilities_Button_List[i]);

			bottomleft = { buttonTransform.position.x - buttonTransform.scale.x * 0.5f, buttonTransform.position.y - buttonTransform.scale.y * 0.5f };
			topright = { buttonTransform.position.x + buttonTransform.scale.x * 0.5f, buttonTransform.position.y + buttonTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}
	}
}
