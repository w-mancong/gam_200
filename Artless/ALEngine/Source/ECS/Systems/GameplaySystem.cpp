/*!
file:	GameplaySystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for GameplaySystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"
#include "Engine/PathFindingManager.h"
#include "Engine/GamePlayInterface.h"
#include "Engine/GameplayInterface_Management_Enemy.h"
#include "Engine/GameplayInterface_Management_GUI.h"

namespace ALEngine::ECS
{
	//Ease of use for ray
	using Engine::Physics::Ray2D;
	using Engine::GameplayInterface::Room;

	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;
	using GameplayInterface::Pattern;
	using GameplayInterface::Abilities;
	using namespace Engine::GameplayInterface_Management_Enemy;
	using namespace GameplayInterface_Management_GUI;

	/*!*********************************************************************************
	\brief
		Contains most logic and data to run game PAVE
	***********************************************************************************/
	class GameplaySystem : public System
	{
		public:
		/*!*********************************************************************************
		\brief
			Status of game, STOP if game is not playing anymore
							RUNNING if game is still running
		***********************************************************************************/
		enum class GAMEPLAY_STATUS
		{
			STOP,
			RUNNING
		};


		/*!*********************************************************************************
		\brief
			State of phase of game
		***********************************************************************************/
		enum class PHASE_STATUS
		{
			PHASE_SETUP,
			PHASE_ACTION,
			PHASE_ENEMY,
		};


		/*!*********************************************************************************
		\brief
			State of controlling/movement of unit
		***********************************************************************************/
		enum class UNITS_CONTROL_STATUS {
			NOTHING,
			UNIT_MOVING,
			UNIT_ATTACKING
		};

		/*!*********************************************************************************
		\brief
			State of placing pattern onto room
		***********************************************************************************/
		enum class PATTERN_PLACEMENT_STATUS {
			NOTHING, 
			PLACING_FOR_TILE,
			PLACING_FOR_ABILITIES
		};

		/*!*********************************************************************************
		\brief
			Container for a move order
		***********************************************************************************/
		struct MoveOrder {
			Entity entity;				//unit
			std::vector<Entity> path;	//path, list of cells
			u32 path_step = 1;			//step into the path
		};

		//******VARIABLES**********//
		u32 roomSize[2]{ 6, 6 };		//Size to initialize the room with
		Room m_Room;					//Room COntainer

		//Entities to keep track of
		Entity playerEntity, startCellEntity, targetCellEntity;
		Entity movingUnitEntity;

		//Enemy
		std::vector<Entity> enemyEntityList;
		u32 enemyMoved = 0;	//Step into enemy that are moving

		//Keep track of status
		GAMEPLAY_STATUS currentGameplayStatus = GAMEPLAY_STATUS::RUNNING;							//Keep track of gameplay status, running or stopped
		PHASE_STATUS currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;								//Keep track of phase
		UNITS_CONTROL_STATUS currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;				//Keep track of status of unit control
		PATTERN_PLACEMENT_STATUS currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;	//Keep track of what the pattern is being placed for

		//Keep track of what move order the unit has
		MoveOrder currentModeOrder;

		//Keep track of the parent of all the cells
		Entity m_Room_Parent_Entity;

		//Patterns
		std::vector<Pattern> pattern_List;
		Pattern selected_Pattern;

		//Abilities
		std::vector<Abilities> Abilities_List;
		Abilities selected_Abilities;

		//Cell that the mouse is hovering over
		Entity current_Moused_Over_Cell;

		//Tracks debug drawing for room
		b8 is_DebugDraw = false;

		//******FUNCTIONS**********//
		/*!*********************************************************************************
		\brief
			Clear the move order
		***********************************************************************************/
		void ClearMoveOrder();

		/*!*********************************************************************************
		\brief
			Get current cell the unit is moving to
		***********************************************************************************/
		Entity getCurrentEntityCell();
		void SetMoveOrder(std::vector<Entity> path);

		/*!*********************************************************************************
		\brief
			Select Pattern
		***********************************************************************************/
		void SelectPattern(Pattern pattern);

		/*!*********************************************************************************
		\brief
			Select Abilities
		***********************************************************************************/
		void SelectAbility(Abilities& ability);

		/*!*********************************************************************************
		\brief
			Move the step for move order (Take step forward in path)
		***********************************************************************************/
		bool StepUpModeOrderPath(MoveOrder& order);

		/*!*********************************************************************************
		\brief
			Get size of room
		***********************************************************************************/
		u32 getRoomSize();

		/*!*********************************************************************************
		\brief
			Run Game State (the update of the game states)
		***********************************************************************************/
		void RunGameState();

		/*!*********************************************************************************
		\brief
			Update unit sprite layer based on their y position
		***********************************************************************************/
		void UpdateUnitSpriteLayer();

		/*!*********************************************************************************
		\brief
			Set Move player entity to cell
		***********************************************************************************/
		void MovePlayerEntityToCell(Entity cellEntity);

		/*!*********************************************************************************
			\brief
				Run enemy decision and move the enemy accordingly	
		***********************************************************************************/
		void MoveEnemy();

		/*!*********************************************************************************
		\brief
			Run update for moving units
		***********************************************************************************/
		void RunGameStateMoving();

		/*!*********************************************************************************
		\brief
			End current turn
		***********************************************************************************/
		void EndTurn();

		/*!*********************************************************************************
		\brief
			Place Player onto room
		***********************************************************************************/
		void PlaceNewPlayerInRoom(s32 x, s32 y);
		
		/*!*********************************************************************************
		\brief
			Place Enemy onto room
		***********************************************************************************/
		void PlaceNewEnemyInRoom(s32 x, s32 y);

		/*!*********************************************************************************
		\brief
			Initialize End turn button
		***********************************************************************************/
		void InitializeEndTurnButton();

		/*!*********************************************************************************
		\brief
			Update the unit information GUI when select the unit
		***********************************************************************************/
		void UpdateGUI_OnSelectUnit(ECS::Entity unitEntity);

		//Cheats
		b8 godMode = false, cheat_abilitiesDoubleDamage = false;
		void Cheat_ToggleGodMode();
		void Cheat_IncreasePlayerHealth(s32 amount);
		void Cheat_ToggleDoubleAbilitiesDoubleDamage();
		void Cheat_DecreaseEnemyHealthToOne();
		void Cheat_EliminateAllEnemy();
		void Cheat_ResetAllEnemiesHealth();
		void Cheat_ResetPlayerHealth();
		void Cheat_ClearFloorWalkability();


		/*!*********************************************************************************
		\brief
			Set gameplay system to running or not
		***********************************************************************************/
		void Toggle_Gameplay_State(b8 istrue);

		/*!*********************************************************************************
		\brief
			Scan the entire room array to check for the tile counters and to 
			change the sprite to the correct state of the tile
		***********************************************************************************/
		void scanRoomCellArray();

		/*!*********************************************************************************
		\brief
			Check the tile the player is currently on to check if the 
			tile is supposed to be destroyed
		***********************************************************************************/
		void checkPlayerPlacement();

		/*!*********************************************************************************
		\brief
			Check the selected tile counters and to make 
			amendments to them at the end of the turn
		***********************************************************************************/
		s32 checkTileCounters(Cell& selectedCell);


	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> gameplaySystem;

		std::string const sceneName = R"(Assets\test.scene)";
		s32 base_Layer = 10000;		//Base layer
	}

	//****************EVENTS*****************//
	void Event_Button_Restart([[maybe_unused]] Entity invoker) {
		//Restart the gameplay
		gameplaySystem->Toggle_Gameplay_State(false);
		Scene::Restart();
	}


	void Event_Button_Select_Abilities_0([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 0");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[0]);
		DisableToolTipGUI();
	}

	void Event_Button_Select_Abilities_1([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 1");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[1]);
		DisableToolTipGUI();
	}

	void Event_Button_Select_Abilities_2([[maybe_unused]] Entity invoker) { //CONSTRUCT WALL SKILL
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Construct Wall");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[1]);
		DisableToolTipGUI();
	}

	void Event_Button_Select_CurrentPattern([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Current Pattern");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[0]);
	}

	void Event_Button_Select_Pattern_1([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 1");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[1]);
	}
	
	void Event_Button_Select_Pattern_2([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 2");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[2]);
	}
	
	void Event_Button_Select_Pattern_3([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Pattern 3");
		gameplaySystem->SelectPattern(gameplaySystem->pattern_List[3]);
	}

	void Event_Button_Select_EndTurn([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		//End turn
		gameplaySystem->EndTurn();
	}

	//void Event_Unit_OnSelect([[maybe_unused]] Entity invoker) {
	//	if (utils::IsEqual(Time::m_Scale, 0.f)) {
	//		return;
	//	}

	//	AL_CORE_INFO("DISPLAY UNIT");
	//	gameplaySystem->UpdateGUI_OnSelectUnit(invoker);
	//}

	

	void Event_MouseEnterCell(Entity invoker) {
		//Keep track of cell the mouse is interacting with
		gameplaySystem->current_Moused_Over_Cell = invoker;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		//If cell is not accessible, then ignore
		if (!cell.m_isAccessible) {
			return;
		}

		//If placement status is being used
		//Determine is setup or abilities
		if (gameplaySystem->currentPatternPlacementStatus != GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING) {
			//If checking for setup, if so, then filter for placement
			if (gameplaySystem->currentPhaseStatus == GameplaySystem::PHASE_STATUS::PHASE_SETUP) {
				b8 canPlace = GameplayInterface::CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);
			
				if(canPlace)
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
			//If checking for abilities, if so, then filter for placement
			else if (gameplaySystem->currentPhaseStatus == GameplaySystem::PHASE_STATUS::PHASE_ACTION) {
				b8 canPlace = GameplayInterface::CheckIfAbilitiesCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

				if (canPlace)
					GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
					GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
		}//End check for pattern placement
	}

	void Event_MouseExitCell(Entity invoker) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);
		GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
	}

	void Event_ClickCell(Entity invokerCell) {
		AL_CORE_INFO("Select Cell");

		//If the unit control is currently moving unit, ignore click order
		if (gameplaySystem->currentUnitControlStatus != GameplaySystem::UNITS_CONTROL_STATUS::NOTHING) {
			return;
		}

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invokerCell);
		//If not placing, move character
		if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING && 
			gameplaySystem->currentPhaseStatus == GameplaySystem::PHASE_STATUS::PHASE_ACTION) {
			//When click on cell, Move the player unit to the selected cell
			gameplaySystem->MovePlayerEntityToCell(invokerCell);
		}
		else if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE) {
			b8 canPlace = GameplayInterface::CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

			if (!canPlace && !gameplaySystem->godMode) {
				return;
			}

			GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
			GameplayInterface::PlacePatternOntoGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, "Assets/Images/Walkable.png");
			gameplaySystem->EndTurn();
		}
		else if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES) {
			b8 canPlace = GameplayInterface::CheckIfAbilitiesCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);

			if (!canPlace && !gameplaySystem->godMode) {
				return;
			}

			GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
			GameplayInterface::RunAbilities_OnCells(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, gameplaySystem->selected_Abilities);
			gameplaySystem->EndTurn();

			s8 eliminatedEnemyCount = 0;
			//Check if all enemies are eliminated
			for (s8 i = 0; i < gameplaySystem->enemyEntityList.size(); ++i) {
				Unit& enemy = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->enemyEntityList[i]);

				if (enemy.health <= 0) {
					++eliminatedEnemyCount;
				}
			}
			
			if (eliminatedEnemyCount >= gameplaySystem->enemyEntityList.size()) {
				ECS::SetActive(true, getGuiManager().Win_Clear);
			}
		}
	}

	//****************END EVENTS**********************//

	void RegisterGameplaySystem(void)
	{
		gameplaySystem = Coordinator::Instance()->RegisterSystem<GameplaySystem>();
		Signature signature;
		Coordinator::Instance()->SetSystemSignature<GameplaySystem>(signature);
	}

	void StartGameplaySystem(void) {
#if EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}
#endif
		if (gameplaySystem->m_Room.roomCellsArray)
			return;

		AL_CORE_INFO("GAME START");
		//Set up scene
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		gameplaySystem->m_Room.width = gameplaySystem->roomSize[0];
		gameplaySystem->m_Room.height = gameplaySystem->roomSize[1];
		gameplaySystem->m_Room.roomSize = gameplaySystem->getRoomSize();
		gameplaySystem->m_Room.roomCellsArray = new Entity[gameplaySystem->getRoomSize()];

		gameplaySystem->currentGameplayStatus = GameplaySystem::GAMEPLAY_STATUS::RUNNING;
		gameplaySystem->currentPhaseStatus = GameplaySystem::PHASE_STATUS::PHASE_SETUP;
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

		//Initialize the room and cells
		for (uint32_t i = 0; i < gameplaySystem->getRoomSize(); ++i) {
			gameplaySystem->m_Room.roomCellsArray[i] = Coordinator::Instance()->CreateEntity();

			sceneGraph.Push(gameplaySystem->m_Room_Parent_Entity, gameplaySystem->m_Room.roomCellsArray[i]); // other cells are children of the parent

			Transform transform;
			transform.scale = { 100, 100 };
			transform.localScale = { 100, 100 };

			Coordinator::Instance()->AddComponent(gameplaySystem->m_Room.roomCellsArray[i], transform);

			CreateSprite(gameplaySystem->m_Room.roomCellsArray[i],"Assets/Images/InitialTile_v04.png");
		}

		//Set up the event triggers for the cells
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->roomSize[0]); ++i) {
			for (s32 j = 0; j < static_cast<s32>(gameplaySystem->roomSize[1]); ++j) {
				s32 cellIndex = i * gameplaySystem->roomSize[0] + j;

				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[cellIndex]);
				transform.position = { 550 + (f32)j * 100.f, 300 + (f32)i * 100.f };

				Cell cell;
				cell.coordinate = { i,j };

				CreateEventTrigger(gameplaySystem->m_Room.roomCellsArray[cellIndex]);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_ClickCell);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterCell);
				Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitCell);

				//Add the child overlay
				cell.child_overlay = Coordinator::Instance()->CreateEntity();

				sceneGraph.Push(gameplaySystem->m_Room_Parent_Entity, cell.child_overlay); // other cells are children of the parent
				CreateSprite(cell.child_overlay,"Assets/Images/InitialTile_v04.png");

				Transform child_overlay_transform;
				child_overlay_transform.scale = transform.scale;
				child_overlay_transform.scale.y += 50;
				child_overlay_transform.position = { 550 + (f32)i * 100.f, 300 + (f32)j * 100.f };
				child_overlay_transform.position.y += 50 >> 2;
				Coordinator::Instance()->AddComponent(cell.child_overlay, child_overlay_transform);

				Coordinator::Instance()->AddComponent(getEntityCell(gameplaySystem->m_Room, i, j), cell);
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).tag = "Cell_Overlay[" + std::to_string(i) + "," + std::to_string(j) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(getEntityCell(gameplaySystem->m_Room,i,j)).tag = "Cell[" + std::to_string(i) + "," + std::to_string(j) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY
			
			}
		}

		//Create Player
		gameplaySystem->PlaceNewPlayerInRoom(0, 5);

		gameplaySystem->enemyEntityList.clear();
		//gameplaySystem->PlaceNewEnemyInRoom(0, 1);
		//gameplaySystem->PlaceNewEnemyInRoom(4, 4);
		PlaceNewEnemyInRoom(0, 1, ENEMY_TYPE::ENEMY_TYPE01, gameplaySystem->enemyEntityList, gameplaySystem->m_Room);
        PlaceNewEnemyInRoom(4, 4, ENEMY_TYPE::ENEMY_TYPE01, gameplaySystem->enemyEntityList, gameplaySystem->m_Room);

		////Create EndTurn Button
		gameplaySystem->InitializeEndTurnButton();

		//Initialize Pattern GUI
		InitializePatternGUI(getGuiManager().GUI_Pattern_Button_List);

		//Initialize abilities GUI
		InitializeAbilitiesGUI(getGuiManager().GUI_Abilities_Button_List);

		//Initialize General GUI
		InitializeGUI();

		//Add events for pattern Button
		Subscribe(getGuiManager().GUI_Pattern_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_CurrentPattern);
		Subscribe(getGuiManager().GUI_Pattern_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_1);
		Subscribe(getGuiManager().GUI_Pattern_Button_List[2], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_2);
		Subscribe(getGuiManager().GUI_Pattern_Button_List[3], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_3);


		//Add events for abilities Button
		Subscribe(getGuiManager().GUI_Abilities_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_0);
		Subscribe(getGuiManager().GUI_Abilities_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_1);
		Subscribe(getGuiManager().GUI_Abilities_Button_List[2], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_2);

		
		//Set a few blocks to be inaccessible
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 0, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 2, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 2, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 3, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 3, 2, false);
		
		Subscribe(getGuiManager().Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Restart);

		gameplaySystem->Toggle_Gameplay_State(true);
		ToggleAbilitiesGUI(false);
		TogglePatternGUI(true);
	}

	void UpdateGameplaySystem(void)
	{
		#if EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive()) {
			return;
		}
		#endif
		
		if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::STOP) {
			return;
		}

		//If right mouse button
		if (Input::KeyDown(KeyCode::MouseRightButton)) {
			//Deselect Pattern
			if (gameplaySystem->currentPhaseStatus == GameplaySystem::PHASE_STATUS::PHASE_SETUP) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->current_Moused_Over_Cell);

				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
				gameplaySystem->currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

				TogglePatternGUI(true);
			}
			//Deselect Abilities
			else if (gameplaySystem->currentPhaseStatus == GameplaySystem::PHASE_STATUS::PHASE_ACTION) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->current_Moused_Over_Cell);

				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
				gameplaySystem->currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

				TogglePatternGUI(false);
				ToggleAbilitiesGUI(true);
			}
		}
		//TEMPORARY TROUBLESHOOTING FEATURE FOR BUILDING WALL ABILITY
		if (Input::KeyTriggered(KeyCode::B)) {
			std::cout << "B IS PRESSED" << std::endl;
			GameplayInterface::constructWall(gameplaySystem->m_Room, 0, 2, true);
			GameplayInterface::constructWall(gameplaySystem->m_Room, 2, 0, true);
		}

		if (Input::KeyTriggered(KeyCode::N)) {
			std::cout << "N IS PRESSED" << std::endl;
			GameplayInterface::destroyWall(gameplaySystem->m_Room, 0, 2, true);
			GameplayInterface::destroyWall(gameplaySystem->m_Room, 2, 0, true);
		}


		//Toggle debug draw
		if (Input::KeyTriggered(KeyCode::Key_3)) {
			gameplaySystem->is_DebugDraw = !gameplaySystem->is_DebugDraw;
		}

		//******CHEAT KEYS******//
		if (Input::KeyTriggered(KeyCode::F1)) {
			gameplaySystem->Cheat_ToggleGodMode();
		}

		if (Input::KeyTriggered(KeyCode::F2)) {
			gameplaySystem->Cheat_ToggleDoubleAbilitiesDoubleDamage();
		}

		if (Input::KeyTriggered(KeyCode::F3)) {
			gameplaySystem->Cheat_ResetPlayerHealth();
		}

		if (Input::KeyTriggered(KeyCode::F4)) {
			gameplaySystem->Cheat_ResetAllEnemiesHealth();
		}

		if (Input::KeyTriggered(KeyCode::F5)) {
			gameplaySystem->Cheat_DecreaseEnemyHealthToOne();
		}

		if (Input::KeyTriggered(KeyCode::F6)) {
			gameplaySystem->Cheat_IncreasePlayerHealth(10);
		}

		if (Input::KeyTriggered(KeyCode::F7)) {
			gameplaySystem->EndTurn();
		}

		if (Input::KeyTriggered(KeyCode::F8)) {
			gameplaySystem->Cheat_EliminateAllEnemy();
		}

		if (Input::KeyTriggered(KeyCode::F9)) {
			gameplaySystem->Cheat_ClearFloorWalkability();
		}
		//******END CHEAT KEYS******//

		gameplaySystem->RunGameState();
		gameplaySystem->UpdateUnitSpriteLayer();
	}

	void ExitGameplaySystem(void)
	{
#if EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}
#endif

		if (gameplaySystem->m_Room.roomCellsArray != nullptr) {
			delete[] gameplaySystem->m_Room.roomCellsArray;
			gameplaySystem->m_Room.roomCellsArray = nullptr;
		}

		gameplaySystem->m_Room.width = 0;
		gameplaySystem->m_Room.height = 0;
		gameplaySystem->godMode = false;
		gameplaySystem->cheat_abilitiesDoubleDamage = false;
	}

	Entity GameplaySystem::getCurrentEntityCell() {
		return currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
	}

	void GameplaySystem::EndTurn() {
		//Reset the statuses
		currentUnitControlStatus = GameplaySystem::UNITS_CONTROL_STATUS::NOTHING;
		currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

		//Disable the end turn button
		ECS::SetActive(false, getGuiManager().endTurnBtnEntity);

		//Set the turn accordingly
		switch (currentPhaseStatus) {
		case PHASE_STATUS::PHASE_SETUP:
			currentPhaseStatus = PHASE_STATUS::PHASE_ACTION;

			AL_CORE_DEBUG("Loading PHASE ACTION");

			ToggleAbilitiesGUI(true);
			TogglePatternGUI(false);
			break;

		case PHASE_STATUS::PHASE_ACTION:
			AL_CORE_DEBUG("Loading PHASE ENEMY");
			currentPhaseStatus = PHASE_STATUS::PHASE_ENEMY;
			enemyMoved = 0;

			ToggleAbilitiesGUI(false);
			TogglePatternGUI(false);
			MoveEnemy();
			break;

		case PHASE_STATUS::PHASE_ENEMY:
			AL_CORE_DEBUG("Loading PHASE SETUP");
			currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;
			TogglePatternGUI(true);

			Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
			playerUnit.movementPoints = playerUnit.maxMovementPoints;

			for (int i = 0; i < enemyEntityList.size(); ++i) {
				Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
				enemyUnit.movementPoints = enemyUnit.maxMovementPoints;
			}
			UpdateGUI_OnSelectUnit(playerEntity);

			scanRoomCellArray();

			checkPlayerPlacement();

			break;
		}
	}

	// Scan the entire room array to check for the tile counters and to change the sprite to the correct state of the tile
	void GameplaySystem::scanRoomCellArray() {
		s32 resetCounter;
		//Scan through each cell in the roomCellArray for the individual cell in the roomArray
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->roomSize[0]); ++i) {
			for (s32 j = 0; j < static_cast<s32>(gameplaySystem->roomSize[1]); ++j) {
				s32 cellIndex = i * gameplaySystem->roomSize[0] + j;
				Entity cellEntity = m_Room.roomCellsArray[cellIndex];

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				resetCounter = checkTileCounters(cell);

				if (resetCounter == 1) {

					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/CloseButton.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE

				}
				if (resetCounter == 0) {
					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");
				}

			}
		}
	}

	//Check the tile the player is currently on to check if the tile is supposed to be destroyed
	void GameplaySystem::checkPlayerPlacement() {
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		Entity cellEntity = playerUnit.m_CurrentCell_Entity;
		Cell& playerUnitCell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		if (playerUnitCell.m_canWalk == false) {
			playerUnit.health = 0;
		}
	}


	//Check the selected tile counters and to make amendments to them at the end of the turn
	s32 GameplaySystem::checkTileCounters(Cell& selectedCell) {
		if (selectedCell.m_resetCounter > 0) {
			selectedCell.m_resetCounter--;
		}

		if (selectedCell.m_resetCounter == 0) {
			selectedCell.m_canWalk = false;	
		}

		return selectedCell.m_resetCounter;
	}


	void GameplaySystem::PlaceNewPlayerInRoom(s32 x, s32 y) {
		//Create a new player entity
		gameplaySystem->playerEntity = Coordinator::Instance()->CreateEntity();

		//Create the player
		CreatePlayerUnit(gameplaySystem->playerEntity);
		
		//Assign it's states
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		playerUnit.coordinate[0] = x;
		playerUnit.coordinate[1] = y;

		//Set the cell it's on
		playerUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(gameplaySystem->m_Room, x, y);

		playerUnit.maxMovementPoints = 4;
		playerUnit.movementPoints = playerUnit.maxMovementPoints;

		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).unitEntity = gameplaySystem->playerEntity;
		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).hasUnit = true;

		playerUnit.maxHealth = 50;
		playerUnit.health = 50;

		GameplayInterface::PlaceWalkableOnGrid(gameplaySystem->m_Room, { x, y }, "Assets/Images/Walkable.png");

		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(gameplaySystem->m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]));
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->playerEntity);
		playerTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::PlaceNewEnemyInRoom(s32 x, s32 y) {
		//Create Enemy entity 
		Entity newEnemy = Coordinator::Instance()->CreateEntity();

		//Add it to the enemy list
		gameplaySystem->enemyEntityList.push_back(newEnemy);

		//Craete enemy
		CreateEnemyUnit(newEnemy);
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(newEnemy);
		enemyUnit.coordinate[0] = x;
		enemyUnit.coordinate[1] = y;

		//Set it's cell
		enemyUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(gameplaySystem->m_Room, x, y);

		//Set it's stats
		enemyUnit.health = 20, enemyUnit.maxHealth = 20;
		enemyUnit.minDamage = 8, enemyUnit.maxDamage = 13;

		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).unitEntity = newEnemy;
		Coordinator::Instance()->GetComponent<Cell>(enemyUnit.m_CurrentCell_Entity).hasUnit = true;
		
		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(gameplaySystem->m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]));
		Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(newEnemy);
		enemyTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::SelectPattern(Pattern pattern) { 
		//Select pattern 
		if (currentPhaseStatus == PHASE_STATUS::PHASE_SETUP) {
			currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_TILE;
			selected_Pattern = pattern;

			TogglePatternGUI(false);
		}
		else if (currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
			currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES;
			selected_Pattern = pattern;

			TogglePatternGUI(false);
		}
	}

	void GameplaySystem::SelectAbility(Abilities& ability) {
		//Select abilities
		if (currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
			selected_Abilities = ability;

			ToggleAbilitiesGUI(false);
			TogglePatternGUI(true);
		}
	}

	void GameplaySystem::ClearMoveOrder() {
		currentModeOrder.path.clear();
		currentModeOrder.path_step = 1;
	}

	void GameplaySystem::SetMoveOrder(std::vector<Entity> path) {
		currentModeOrder.path.clear();
		currentModeOrder.path_step = 1;

		for (s32 i = static_cast<s32>(path.size()) - 1; i >= 0; --i) {
			currentModeOrder.path.push_back(path[i]);
		}
	}

	uint32_t GameplaySystem::getRoomSize() {
		return gameplaySystem->roomSize[0] * gameplaySystem->roomSize[1];
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

	void GameplaySystem::RunGameState() {
		//Run updates accordingly to current game states
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
	}

	void GameplaySystem::UpdateUnitSpriteLayer() {
		//Update unit sprite layer accordingly from their y position
		for (s32 i = 0; i < static_cast<s32>(enemyEntityList.size()); ++i) {
			Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(enemyEntityList[i]);
			Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
			Sprite& enemySprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);

			//If the time is paused, put the enemy layer to the back
			if (utils::IsEqual(Time::m_Scale, 1.0f))
				enemySprite.layer = base_Layer - static_cast<s32>(enemyTransform.localPosition.y);
			else
				enemySprite.layer = 1;
		}

		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		//If the time is paused, put the player layer to the back
		if (utils::IsEqual(Time::m_Scale, 1.0f))
			playerSprite.layer = base_Layer - static_cast<s32>(playerTransform.localPosition.y);
		else
			playerSprite.layer = 1;
	}

	void CreatePlayerUnit(Entity entity) {
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
		playerSpriteTransform.localScale = { 1.f, 2.f };

		CreateSprite(playerUnit.unit_Sprite_Entity, playerSpriteTransform, "Assets/Images/Player v2.png");
		
		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Player";
		Coordinator::Instance()->GetComponent<EntityData>(playerUnit.unit_Sprite_Entity).tag = "Player_Sprite";

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, playerUnit.unit_Sprite_Entity);

		//Add events
		EventTrigger eventTrigger;
		eventTrigger.layer = 1;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void CreateEnemyUnit(Entity entity) {
		//Create enemy unit
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

		CreateSprite(enemyUnit.unit_Sprite_Entity, enemySpriteTransform, "Assets/Images/Bishop v.02.png");

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Enemy_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);
		Coordinator::Instance()->GetComponent<EntityData>(enemyUnit.unit_Sprite_Entity).tag = "Enemy_Sprite_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, enemyUnit.unit_Sprite_Entity);

		//Set enemy events
		EventTrigger eventTrigger;
		eventTrigger.layer = 1;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void GameplaySystem::MovePlayerEntityToCell(Entity cellEntity) {
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
		bool isPathFound = Engine::AI::FindPath(m_Room, startCellEntity, targetCellEntity, pathList, false);

		//If path not found then stop
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			return;
		}

		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;

		movingUnitEntity = playerEntity;
		UpdateGUI_OnSelectUnit(movingUnitEntity);

		ECS::SetActive(true, getGuiManager().endTurnBtnEntity);
	}	
	
	void GameplaySystem::MoveEnemy() {
		//Clear move order
		ClearMoveOrder();

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
			MoveEnemy();
			return;
		}

		AL_CORE_INFO("Run Adjacent Attack");
		bool ifPlayerIsAlreadyBeside = GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);

		AL_CORE_INFO("Check player inside");
		if (ifPlayerIsAlreadyBeside) {
			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Attacked player");
			++enemyMoved;
			MoveEnemy();
			return;
		}

		Entity cellToMoveTo{};
		b8 hasFoundCellBesidePlayer = false;

		AL_CORE_INFO("Checking for cell adjacent to player");

		//Find cell adjacent to player 
		//Then move to the cell accordingly
		if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])						&&
							   IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])					&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1])).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] + 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player right");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])							&&
									IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1])).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0] - 1, playerUnit.coordinate[1]);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player left");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) + 1							&&
									IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)						&&
			!Coordinator::Instance()->GetComponent<Cell>(GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1)).hasUnit) 
		{
			hasFoundCellBesidePlayer = true;
			cellToMoveTo = GameplayInterface::getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] + 1);

			AL_CORE_INFO("Enemy " + std::to_string(enemyMoved) + " Found path at player up");
		}
		else if (GameplayInterface::IsCoordinateInsideRoom(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]) - 1							&&
									IsCoordinateCellAccessible(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1] - 1)						&&
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
			MoveEnemy();
			return;
		}

		startCellEntity = getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		//Find path
		std::vector<ECS::Entity> pathList;
		b8 isPathFound = Engine::AI::FindPath(m_Room, startCellEntity, cellToMoveTo, pathList, true);
		
		//Path not found then end turn
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			++enemyMoved;
			MoveEnemy();
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

	void GameplaySystem::RunGameStateMoving() {
		//If the gameplay system is not running anymore
		//Don't continue
		if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::STOP) {
			return;
		}

		//Keep track of next cell destination
		Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(getCurrentEntityCell());

		//Keep track of player transform
		Transform& movingTransform = Coordinator::Instance()->GetComponent<Transform>(movingUnitEntity);

		//Move player transform to it's iterated waypoint
		Vector2 direction = Vector3::Normalize(cellTransform.localPosition - movingTransform.localPosition);

		//Move the transform of the moving to target cel
		movingTransform.localPosition += direction * 400.0f * Time::m_FixedDeltaTime;

		//If reached the cell
		if (Vector3::Distance(movingTransform.localPosition, cellTransform.localPosition) < 10.0f) {
			Unit& movinUnit = Coordinator::Instance()->GetComponent<Unit>(movingUnitEntity);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getCurrentEntityCell());
			Cell& OriginCell = Coordinator::Instance()->GetComponent<Cell>(movinUnit.m_CurrentCell_Entity);

			//Free cell unit was on
			OriginCell.hasUnit = false;

			//Update player cell to current
			movinUnit.m_CurrentCell_Entity = gameplaySystem->getCurrentEntityCell();
			cell.unitEntity = movingUnitEntity;
			cell.hasUnit = true;

			movingTransform.localPosition = cellTransform.localPosition;
			movinUnit.coordinate[0] = cell.coordinate.x;
			movinUnit.coordinate[1] = cell.coordinate.y;

			bool isEndOfPath = true;

			--movinUnit.movementPoints;
			
			//If no more movement point
			//Stop the movement
			if (movinUnit.movementPoints <= 0) {
				isEndOfPath = true;
			}
			else {
				isEndOfPath = StepUpModeOrderPath(currentModeOrder);
			}

			AL_CORE_INFO("Movement Points " + std::to_string(movinUnit.movementPoints));

			//If reached the end of path
			if (isEndOfPath) {
				currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;

				//If player, end turn
				if (movinUnit.unitType == UNIT_TYPE::PLAYER) {
					if (movinUnit.movementPoints <= 0) {
						EndTurn();
					}
				}
				//If enemy, move on to next enemy
				else if (movinUnit.unitType == UNIT_TYPE::ENEMY) {
					GameplayInterface::RunEnemyAdjacentAttack(m_Room, Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyMoved-1]));
					MoveEnemy();
				}
				return;
			}
		}
	}

	void GameplaySystem::InitializeEndTurnButton() {
		getGuiManager().endTurnBtnEntity = Coordinator::Instance()->GetEntityByTag("end_turn");
		CreateButton(getGuiManager().endTurnBtnEntity);

		Subscribe(getGuiManager().endTurnBtnEntity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_EndTurn);
	}

	void GameplaySystem::UpdateGUI_OnSelectUnit(ECS::Entity unitEntity) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		Text& health_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Health);
		Text& name_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Name);
		Text& attack_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Attack);
		Text& defense_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Defense);
		Text& movement_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Movement);
		Text& range_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Range);
		Sprite& profile = Coordinator::Instance()->GetComponent<Sprite>(getGuiManager().Unit_Profile);

		health_text.textString = std::to_string(unit.health) + "/" + std::to_string(unit.maxHealth);
		attack_text.textString = std::to_string(unit.minDamage) + "/" + std::to_string(unit.maxDamage);
		defense_text.textString = std::to_string(unit.defense);
		movement_text.textString = std::to_string(unit.movementPoints) + "/" + std::to_string(unit.maxMovementPoints);
		range_text.textString = std::to_string(unit.minRange);
		name_text.textString = unit.unit_Name;
		
		profile.id = AssetManager::Instance()->GetGuid(unit.unit_Profile_Sprite_File);

		Transform& healthbar_transform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().Unit_Healthbar);
		healthbar_transform.localScale.x = (unit.health <= 0 ? 0 : ((f32)unit.health / (f32)unit.maxHealth));
	}

	void GameplaySystem::Cheat_ToggleGodMode() {
		godMode = !godMode;

		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		if (godMode) {
			playerSprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
		}
		else {
			playerSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}		
		
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			if (godMode){
				unit.minDamage = 0;
				unit.maxDamage = 0;
			}
			else {
				unit.minDamage = 5;
				unit.maxDamage = 5;
			}
		}
		UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_IncreasePlayerHealth(s32 amount) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		unit.health += amount;

		if (unit.health >= unit.maxHealth) {
			unit.health = unit.maxHealth;
		}
		UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_ToggleDoubleAbilitiesDoubleDamage() {
		cheat_abilitiesDoubleDamage = !cheat_abilitiesDoubleDamage;
		
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

		for (s32 i = 0; i < 2; ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(getGuiManager().GUI_Abilities_Button_List[i]);

			if (cheat_abilitiesDoubleDamage) {
				sprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
			}
			else {
				sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
	}

	void GameplaySystem::Cheat_DecreaseEnemyHealthToOne() {
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			if (unit.health > 0) {
				unit.health = 1;
				UpdateGUI_OnSelectUnit(enemyEntityList[i]);
			}
		}
	}	
	
	void GameplaySystem::Cheat_EliminateAllEnemy() {
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			GameplayInterface::DoDamageToUnit(enemyEntityList[i], unit.maxHealth);
		}

		ECS::SetActive(true, getGuiManager().Win_Clear);
	}

	void GameplaySystem::Cheat_ResetAllEnemiesHealth() {
		for (s8 i = 0; i < enemyEntityList.size(); ++i) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);

			if (unit.health > 0) {
				unit.health = unit.maxHealth;
				UpdateGUI_OnSelectUnit(enemyEntityList[i]);
			}
		}
	}

	void GameplaySystem::Cheat_ResetPlayerHealth() {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		unit.health = unit.maxHealth;
		UpdateGUI_OnSelectUnit(playerEntity);
	}

	void GameplaySystem::Cheat_ClearFloorWalkability() {
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->getRoomSize()); ++i) {
			GameplayInterface::ToggleCellWalkability(gameplaySystem->m_Room, gameplaySystem->m_Room.roomCellsArray[i], false);
		}

		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		GameplayInterface::ToggleCellWalkability(gameplaySystem->m_Room, unit.m_CurrentCell_Entity, true);
	}

	void GameplaySystem::Toggle_Gameplay_State(b8 istrue) {
		currentGameplayStatus = (istrue ? GAMEPLAY_STATUS::RUNNING : GAMEPLAY_STATUS::STOP);
	}

	void DrawGameplaySystem() {
	#if EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
	#endif

	if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::STOP) {
		return;
	}

	if (!gameplaySystem->is_DebugDraw)
	{
		return;
	}

		//Box holder
		Vector2 bottomleft;
		Vector2 topright;

		Color color = { 0.f,0.f ,0.f ,0.f };

		//Draw all cells
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->m_Room.roomSize); ++i) {
			Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[i]);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->m_Room.roomCellsArray[i]);

			if (!cell.m_isAccessible) {
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
		for (int i = 0; i < getGuiManager().GUI_Pattern_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().GUI_Pattern_Button_List[i]);

			bottomleft = { buttonTransform.position.x - buttonTransform.scale.x * 0.5f, buttonTransform.position.y - buttonTransform.scale.y * 0.5f };
			topright = { buttonTransform.position.x + buttonTransform.scale.x * 0.5f, buttonTransform.position.y + buttonTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}

		//Draw the Pattern GUI
		for (int i = 0; i < getGuiManager().GUI_Abilities_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().GUI_Abilities_Button_List[i]);

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

void ALEngine::Engine::GameplayInterface_Management_Enemy::Event_Unit_OnSelect([[maybe_unused]] Entity invoker)
{
	if (utils::IsEqual(Time::m_Scale, 0.f))
	{
		return;
	}

	AL_CORE_INFO("DISPLAY UNIT");
	ALEngine::ECS::gameplaySystem->UpdateGUI_OnSelectUnit(invoker);
}
