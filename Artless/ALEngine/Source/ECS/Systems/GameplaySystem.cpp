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
			u32 path_step = 1;
		};

		//******VARIABLES**********//
		u32 roomSize[2]{ 6, 6 };
		Room m_Room;

		Entity playerEntity, startCellEntity, targetCellEntity;

		Entity movingUnitEntity;

		//Enemy
		std::vector<Entity> enemyEntityList;
		u32 enemyMoved = 0;

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

		Entity GUI_Unit_Health, GUI_Unit_Name, GUI_Unit_Attack, GUI_Unit_Defense, GUI_Unit_Movement, GUI_Unit_Range;
		Entity GUI_Unit_Profile;
		Entity GUI_Unit_Healthbar;

		//Win
		Entity GUI_Win_Clear, GUI_Win_Button;

		b8 is_DebugDraw = false;

		//******FUNCTIONS**********//
		void ClearMoveOrder();
		Entity getCurrentEntityCell();
		void SetMoveOrder(std::vector<Entity> path);

		//Select Pattern
		void SelectPattern(Pattern pattern);

		//Select Abilities
		void SelectAbility(Abilities& ability);

		//Return if reached end
		bool StepUpModeOrderPath(MoveOrder& order);
		u32 getRoomSize();
		void RunGameState();
		void UpdateUnitSpriteLayer();

		void MovePlayerEntityToCell(Entity cellEntity);
		void MoveEnemy();

		void RunGameStateMoving();

		void EndTurn();

		void TogglePatternGUI(b8 istrue);
		void ToggleAbilitiesGUI(b8 istrue);

		void PlaceNewPlayerInRoom(s32 x, s32 y);
		void PlaceNewEnemyInRoom(s32 x, s32 y);

		//Creating Object
		void InitializeEndTurnButton();

		void UpdateGUI_OnSelectUnit(ECS::Entity unitEntity);

		void DisableToolTip();

		//Cheats
		b8 godMode = false, cheat_abilitiesDoubleDamage = false;
		void Cheat_ToggleGodMode();
		void Cheat_IncreasePlayerHealth(s32 amount);
		void Cheat_ToggleDoubleAbilitiesDoubleDamage();
		void Cheat_DecreaseEnemyHealthToOne();
		void Cheat_ResetAllEnemiesHealth();
		void Cheat_ResetPlayerHealth();
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> gameplaySystem;

		std::string const sceneName = R"(Assets\test.scene)";

		s32 base_Layer = 10000;
	}

	void Event_Button_Restart([[maybe_unused]] Entity invoker) {
		Scene::Restart();
	}

	void Event_Button_Darken([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		sprite.color = { 0.6f, 0.6f, 0.6f, 1.f };
	}

	void Event_Button_Lighten([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(invoker);

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		
		if(eventTrigger.isEnabled)
		//sprite.color = { 1.f, 1.f, 1.f, 1.f };

		if (gameplaySystem->cheat_abilitiesDoubleDamage) {
			sprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
		}
		else {
			sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	void Event_Button_Select_Abilities_0([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 0");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[0]);
		gameplaySystem->DisableToolTip();
	}

	void Event_Button_Select_Abilities_1([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("Select Abilities 1");
		gameplaySystem->SelectAbility(gameplaySystem->Abilities_List[1]);
		gameplaySystem->DisableToolTip();
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

	void Event_Unit_OnSelect([[maybe_unused]] Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		AL_CORE_INFO("DISPLAY UNIT");
		gameplaySystem->UpdateGUI_OnSelectUnit(invoker);
	}

	void Event_MouseEnterCell(Entity invoker) {
		//AL_CORE_INFO("Enter Cell");

		gameplaySystem->current_Moused_Over_Cell = invoker;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);

		if (!cell.m_isAccessible) {
			return;
		}

		if (gameplaySystem->currentPatternPlacementStatus != GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING) {
			if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_SETUP) {
				b8 canPlace = GameplayInterface::CheckIfPatternCanBePlacedForTile(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern);
			
				if(canPlace)
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 0.f,1.f,0.f,1.f });
				else
				GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,0.f,0.f,1.f });
			}
			else if (gameplaySystem->currentGameplayStatus == GameplaySystem::GAMEPLAY_STATUS::PHASE_ACTION) {
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
		else if (gameplaySystem->currentPatternPlacementStatus == GameplaySystem::PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES) {
			GameplayInterface::DisplayFilterPlacementGrid(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, { 1.f,1.f,1.f,1.f });
			GameplayInterface::RunAbilities_OnCells(gameplaySystem->m_Room, cell.coordinate, gameplaySystem->selected_Pattern, gameplaySystem->selected_Abilities);
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
#if _EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}
#endif
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

				Coordinator::Instance()->AddComponent(getEntityCell(gameplaySystem->m_Room, i, j), cell);

				Coordinator::Instance()->GetComponent<EntityData>(getEntityCell(gameplaySystem->m_Room,i,j)).tag = "Cell[" + std::to_string(i) + "," + std::to_string(j) + "]";
			}
		}

		//Create Player
		gameplaySystem->PlaceNewPlayerInRoom(0, 5);

		gameplaySystem->enemyEntityList.clear();
		gameplaySystem->PlaceNewEnemyInRoom(0, 1);
		gameplaySystem->PlaceNewEnemyInRoom(4, 4);

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
		Subscribe(gameplaySystem->GUI_Abilities_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_1);

		//Add visual feedback event for abilities GUI
		for (int i = 0; i < gameplaySystem->GUI_Abilities_Button_List.size(); ++i) {
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
			Subscribe(gameplaySystem->GUI_Abilities_Button_List[i], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		}
		
		//Set a few blocks to be inaccessible
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 0, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 1, 2, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 2, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 3, 1, false);
		ToggleCellAccessibility(gameplaySystem->m_Room, 3, 2, false);

		//Set abilities UI off
		gameplaySystem->ToggleAbilitiesGUI(false);

		//Initialize GUI Text and Sprites
		gameplaySystem->GUI_Unit_Name = Coordinator::Instance()->GetEntityByTag("text_playername");
		gameplaySystem->GUI_Unit_Health = Coordinator::Instance()->GetEntityByTag("text_bar_hp");
		gameplaySystem->GUI_Unit_Profile = Coordinator::Instance()->GetEntityByTag("profile_player");
		gameplaySystem->GUI_Unit_Attack = Coordinator::Instance()->GetEntityByTag("text_attack_output");
		gameplaySystem->GUI_Unit_Defense = Coordinator::Instance()->GetEntityByTag("text_defense_output");
		gameplaySystem->GUI_Unit_Movement = Coordinator::Instance()->GetEntityByTag("text_move_output");
		gameplaySystem->GUI_Unit_Range = Coordinator::Instance()->GetEntityByTag("text_range_output");
		gameplaySystem->GUI_Unit_Healthbar = Coordinator::Instance()->GetEntityByTag("red health bar");

		gameplaySystem->GUI_Win_Clear = Coordinator::Instance()->GetEntityByTag("Win_Clear_Text");
		gameplaySystem->GUI_Win_Button = Coordinator::Instance()->GetEntityByTag("Win_Button");

		ECS::SetActive(false, gameplaySystem->endTurnBtnEntity);

		CreateEventTrigger(gameplaySystem->GUI_Win_Button);

		Subscribe(gameplaySystem->GUI_Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
		Subscribe(gameplaySystem->GUI_Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
		Subscribe(gameplaySystem->GUI_Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		Subscribe(gameplaySystem->GUI_Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Restart);
	}

	void UpdateGameplaySystem(void)
	{
#if _EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive()) {
			return;
		}
#endif
		
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

		if (Input::KeyTriggered(KeyCode::Key_3)) {
			gameplaySystem->is_DebugDraw = !gameplaySystem->is_DebugDraw;
		}

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

		gameplaySystem->RunGameState();
		
		gameplaySystem->UpdateUnitSpriteLayer();
	}

	void ExitGameplaySystem(void)
	{
#if _EDITOR
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
	}

	Entity GameplaySystem::getCurrentEntityCell() {
		return currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
	}

	void GameplaySystem::EndTurn() {
		currentUnitControlStatus = GameplaySystem::UNITS_CONTROL_STATUS::NOTHING;
		currentPatternPlacementStatus = GameplaySystem::PATTERN_PLACEMENT_STATUS::NOTHING;

		ECS::SetActive(false, gameplaySystem->endTurnBtnEntity);

		switch (currentGameplayStatus) {
			case GAMEPLAY_STATUS::PHASE_SETUP:
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_ACTION;

				AL_CORE_DEBUG("Loading PHASE ACTION");

				ToggleAbilitiesGUI(true);
				TogglePatternGUI(false);
			break;

			case GAMEPLAY_STATUS::PHASE_ACTION:
				AL_CORE_DEBUG("Loading PHASE ENEMY");
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_ENEMY;
				enemyMoved = 0;

				ToggleAbilitiesGUI(false);
				TogglePatternGUI(false);
				MoveEnemy();
				break;
			
			case GAMEPLAY_STATUS::PHASE_ENEMY:
				AL_CORE_DEBUG("Loading PHASE SETUP");
				currentGameplayStatus = GAMEPLAY_STATUS::PHASE_SETUP;
				TogglePatternGUI(true);

				Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
				playerUnit.movementPoints = playerUnit.maxMovementPoints;

				for (int i = 0; i < enemyEntityList.size(); ++i) {
					Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
					enemyUnit.movementPoints = enemyUnit.maxMovementPoints;
				}
				UpdateGUI_OnSelectUnit(playerEntity);
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

		for (int i = 0; i < 2; ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(GUI_Abilities_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = istrue;

			if (istrue) {
				if (cheat_abilitiesDoubleDamage) {
					sprite.color = { 1.0f, 1.0f, 0.2f, 1.0f };
				}
				else {
					sprite.color = { 1.f, 1.f, 1.f, 1.f };
				}
			}
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
		Entity newEnemy = Coordinator::Instance()->CreateEntity();

		gameplaySystem->enemyEntityList.push_back(newEnemy);

		CreateEnemyUnit(newEnemy);
		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(newEnemy);
		enemyUnit.coordinate[0] = x;
		enemyUnit.coordinate[1] = y;

		enemyUnit.m_CurrentCell_Entity = GameplayInterface::getEntityCell(gameplaySystem->m_Room, x, y);

		enemyUnit.health = 20, enemyUnit.maxHealth = 20;
		enemyUnit.minDamage = 8, enemyUnit.maxDamage = 13;

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

	void GameplaySystem::SelectAbility(Abilities& ability) {
		if (currentGameplayStatus == GAMEPLAY_STATUS::PHASE_ACTION) {
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
		for (s32 i = 0; i < static_cast<s32>(enemyEntityList.size()); ++i) {
			Transform& enemyTransform = Coordinator::Instance()->GetComponent<Transform>(enemyEntityList[i]);
			Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[i]);
			Sprite& enemySprite = Coordinator::Instance()->GetComponent<Sprite>(enemyUnit.unit_Sprite_Entity);

			if (utils::IsEqual(Time::m_Scale, 1.0f))
				enemySprite.layer = base_Layer - static_cast<s32>(enemyTransform.localPosition.y);
			else
				enemySprite.layer = 1;
		}

		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		Sprite& playerSprite = Coordinator::Instance()->GetComponent<Sprite>(playerUnit.unit_Sprite_Entity);

		if (utils::IsEqual(Time::m_Scale, 1.0f))
			playerSprite.layer = base_Layer - static_cast<s32>(playerTransform.localPosition.y);
		else
			playerSprite.layer = 1;
	}

	void CreatePlayerUnit(Entity entity) {
		Unit unit{};
		unit.unitType = UNIT_TYPE::PLAYER;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		Transform& playertransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		playertransform.scale = { 50, 50 };
		playertransform.localScale = { 100, 100 };

		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		playerUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();
		playerUnit.unit_Name = "DROR";
		playerUnit.minRange = 2;
		playerUnit.minDamage = 5;
		playerUnit.maxDamage = 5;
		playerUnit.defense = 1;
		playerUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Player_Unit.png";

		Transform playerSpriteTransform;
		playerSpriteTransform.localPosition = { 0.f, 0.4f };
		playerSpriteTransform.localScale = { 1.f, 2.f };

		CreateSprite(playerUnit.unit_Sprite_Entity, playerSpriteTransform, "Assets/Images/Player v2.png");
		
		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Player";
		Coordinator::Instance()->GetComponent<EntityData>(playerUnit.unit_Sprite_Entity).tag = "Player_Sprite";

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, playerUnit.unit_Sprite_Entity);

		EventTrigger eventTrigger;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void CreateEnemyUnit(Entity entity) {
		Unit unit{};
		unit.unitType = UNIT_TYPE::ENEMY;
		Coordinator::Instance()->AddComponent(entity, unit);
		Coordinator::Instance()->AddComponent(entity, Transform{});

		Transform& enemytransform = Coordinator::Instance()->GetComponent<Transform>(entity);
		enemytransform.scale = { 50, 50 };
		enemytransform.localScale = { 100, 100 };

		Unit& enemyUnit = Coordinator::Instance()->GetComponent<Unit>(entity);
		enemyUnit.unit_Sprite_Entity = Coordinator::Instance()->CreateEntity();
		enemyUnit.unit_Name = "BISHOP";
		enemyUnit.minRange = 1;
		enemyUnit.unit_Profile_Sprite_File = "Assets/Images/Profile_Enemy_Unit.png";
		enemyUnit.maxMovementPoints = 4;
		enemyUnit.movementPoints = enemyUnit.maxMovementPoints;

		Transform enemySpriteTransform;
		enemySpriteTransform.localPosition = { 0.f, 0.4f };
		enemySpriteTransform.localScale = { 1.f, 2.f };

		CreateSprite(enemyUnit.unit_Sprite_Entity, enemySpriteTransform, "Assets/Images/Bishop v.02.png");

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Enemy_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);
		Coordinator::Instance()->GetComponent<EntityData>(enemyUnit.unit_Sprite_Entity).tag = "Enemy_Sprite_" + std::to_string(gameplaySystem->enemyEntityList.size() - 1);

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, enemyUnit.unit_Sprite_Entity);

		EventTrigger eventTrigger;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
	}

	void GameplaySystem::MovePlayerEntityToCell(Entity cellEntity) {
		ClearMoveOrder();

		targetCellEntity = cellEntity;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		if (cell.hasUnit) {
			return;
		}

		Unit playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);
		startCellEntity = getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]);

		std::vector<ECS::Entity> pathList;
		bool isPathFound = Engine::AI::FindPath(m_Room, startCellEntity, targetCellEntity, pathList, false);

		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			return;
		}

		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;

		movingUnitEntity = playerEntity;
		UpdateGUI_OnSelectUnit(movingUnitEntity);

		ECS::SetActive(true, gameplaySystem->endTurnBtnEntity);
	}	
	
	void GameplaySystem::MoveEnemy() {
		ClearMoveOrder();

		AL_CORE_INFO("Enemy Making Decision");

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

		//adjacent to player
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

		if (!hasFoundCellBesidePlayer) {
			AL_CORE_INFO("No Space Beside Player, Moving to next enemy");
			GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyMoved;
			return;
		}

		startCellEntity = getEntityCell(m_Room, enemyUnit.coordinate[0], enemyUnit.coordinate[1]);

		std::vector<ECS::Entity> pathList;
		b8 isPathFound = Engine::AI::FindPath(m_Room, startCellEntity, cellToMoveTo, pathList, true);
		
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			GameplayInterface::RunEnemyAdjacentAttack(m_Room, enemyUnit);
			++enemyMoved;
			return;
		}

		AL_CORE_INFO("Path Found");
		SetMoveOrder(pathList);

		currentUnitControlStatus = UNITS_CONTROL_STATUS::UNIT_MOVING;
		movingUnitEntity = enemyEntityList[enemyMoved];

		UpdateGUI_OnSelectUnit(movingUnitEntity);

		++enemyMoved;
	}

	void GameplaySystem::RunGameStateMoving() {

		//Keep track of next cell destination
		Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(getCurrentEntityCell());

		//Keep track of player transform
		Transform& movingTransform = Coordinator::Instance()->GetComponent<Transform>(movingUnitEntity);

		//Move player transform to it's iterated waypoint
		Vector2 direction = Vector3::Normalize(cellTransform.localPosition - movingTransform.localPosition);

		movingTransform.localPosition += direction * 400.0f * Time::m_FixedDeltaTime;

		if (Vector3::Distance(movingTransform.localPosition, cellTransform.localPosition) < 10.0f) {
			Unit& movinUnit = Coordinator::Instance()->GetComponent<Unit>(movingUnitEntity);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getCurrentEntityCell());

			movingTransform.localPosition = cellTransform.localPosition;
			movinUnit.coordinate[0] = cell.coordinate.x;
			movinUnit.coordinate[1] = cell.coordinate.y;

			bool isEndOfPath = true;

			--movinUnit.movementPoints;

			if (movinUnit.movementPoints <= 0) {
				isEndOfPath = true;
			}
			else {
				isEndOfPath = StepUpModeOrderPath(currentModeOrder);
			}

			AL_CORE_INFO("Movement Poitns " + std::to_string(movinUnit.movementPoints));

			if (isEndOfPath) {
				currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;

				Cell& OriginCell = Coordinator::Instance()->GetComponent<Cell>(movinUnit.m_CurrentCell_Entity);

				OriginCell.hasUnit = false;
				OriginCell.unitEntity = 0;

				//Update player cell to current
				movinUnit.m_CurrentCell_Entity = gameplaySystem->getCurrentEntityCell();
				cell.unitEntity = movingUnitEntity;
				cell.hasUnit = true;

				if (movinUnit.unitType == UNIT_TYPE::PLAYER) {
					if (movinUnit.movementPoints <= 0) {
						EndTurn();
					}
				}
				else if (movinUnit.unitType == UNIT_TYPE::ENEMY) {
					GameplayInterface::RunEnemyAdjacentAttack(m_Room, Coordinator::Instance()->GetComponent<Unit>(enemyEntityList[enemyMoved-1]));
					MoveEnemy();
				}
				return;
			}
		}
	}

	void GameplaySystem::InitializeEndTurnButton() {
		endTurnBtnEntity = Coordinator::Instance()->GetEntityByTag("end_turn");
		
		EventTrigger eventTrigger;		
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_EndTurn);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
		Subscribe(eventTrigger, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
		Coordinator::Instance()->AddComponent(endTurnBtnEntity, eventTrigger);
	}

	void GameplaySystem::DisableToolTip() {
		Entity en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
		Entity en_skillicon = Coordinator::Instance()->GetEntityByTag("skill_icon");
		Entity en_textskill = Coordinator::Instance()->GetEntityByTag("text_skillname");
		Entity en_hard_drop = Coordinator::Instance()->GetEntityByTag("hard_drop_des1");
		Entity en_life_drain = Coordinator::Instance()->GetEntityByTag("life_drain_des1");

		ECS::SetActive(false, en_tooltip);
		ECS::SetActive(false, en_skillicon);
		ECS::SetActive(false, en_textskill);
		ECS::SetActive(false, en_hard_drop);
		ECS::SetActive(false, en_life_drain);
	}

	void GameplaySystem::UpdateGUI_OnSelectUnit(ECS::Entity unitEntity) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		Text& health_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Health);
		Text& name_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Name);
		Text& attack_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Attack);
		Text& defense_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Defense);
		Text& movement_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Movement);
		Text& range_text = Coordinator::Instance()->GetComponent<Text>(GUI_Unit_Range);
		Sprite& profile = Coordinator::Instance()->GetComponent<Sprite>(GUI_Unit_Profile);

		health_text.textString = std::to_string(unit.health) + "/" + std::to_string(unit.maxHealth);
		attack_text.textString = std::to_string(unit.minDamage) + "/" + std::to_string(unit.maxDamage);
		defense_text.textString = std::to_string(unit.defense);
		movement_text.textString = std::to_string(unit.movementPoints) + "/" + std::to_string(unit.maxMovementPoints);
		range_text.textString = std::to_string(unit.minRange);
		name_text.textString = unit.unit_Name;
		
		profile.id = AssetManager::Instance()->GetGuid(unit.unit_Profile_Sprite_File);

		Transform& healthbar_transform = Coordinator::Instance()->GetComponent<Transform>(GUI_Unit_Healthbar);
		healthbar_transform.localScale.x = (unit.health <= 0 ? 0 : ((f32)unit.health / (f32)unit.maxHealth));
		AL_CORE_CRITICAL("SIZE " + std::to_string(healthbar_transform.scale.x));
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

		if (currentGameplayStatus != GAMEPLAY_STATUS::PHASE_ACTION) {
			return;
		}

		for (s32 i = 0; i < 2; ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(GUI_Abilities_Button_List[i]);

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

	void DrawGameplaySystem() {
#if _EDITOR
		if (ALEngine::Editor::ALEditor::Instance()->GetCurrentSceneName() != sceneName) {
			return;
		}

		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
#endif
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
