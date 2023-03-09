#include <pch.h>
#include "Engine/Physics2D.h"
#include "Engine/PathFindingManager.h"
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <Utility/AudioNames.h>
#include <GameplayCamera.h>
#include <PauseLogic.h>
#include <ranges>

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr < GameplaySystem_Interface_Management_Enemy> gameplaySystem_Enemy;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
		std::shared_ptr<GameplaySystem> gameplaySystem;

		std::string room_To_Load = "Assets\\Presentation_Level.map";
	}

	/*!*********************************************************************************
	\brief
		Load Level 1
	***********************************************************************************/
	void Event_Button_LoadLevel_1(ECS::Entity invoker) {
		//Restart the gameplay
		room_To_Load = "Assets\\Presentation_Level.map";
		Engine::Scene::Restart();
	}

	/*!*********************************************************************************
	\brief
		Load Level 2
	***********************************************************************************/
	void Event_Button_LoadLevel_2(ECS::Entity invoker) {
		gameplaySystem->Toggle_Gameplay_State(false);
		//Restart the gameplay
		room_To_Load = "Assets\\Tutorial_Level.map";
		Engine::Scene::Restart();
	}

	void GameplaySystem::Load(ECS::Entity en)
	{

	}

	void GameplaySystem::Init(ECS::Entity en)
	{
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		gameplaySystem_Enemy = ECS::GetLogicComponent<GameplaySystem_Interface_Management_Enemy>(en);
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(en);
		Set_GameplayInterface_GameplayManager(en);

		StartGameplaySystem();
		InitializeRoom(room_To_Load);
		EnemyManager_LoadData();
	}

	bool ALEngine::Script::GameplaySystem::InitializeRoom(std::string map_fp)
	{
		using namespace Gameplay;

		MapManager::Instance()->SetMapPath(map_fp);
		if (!MapManager::Instance()->DeserializeMap(map_fp))
			return false;

		// Get tree
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		m_Room.width = MapManager::Instance()->GetWidth();
		m_Room.height = MapManager::Instance()->GetHeight();
		m_Room.roomSize = m_Room.width * m_Room.height;
		m_Room.roomCellsArray = new ECS::Entity[m_Room.roomSize];

		//Initialize Room Parent 
		m_Room_Parent_Entity = Coordinator::Instance()->CreateEntity();
		Coordinator::Instance()->AddComponent(m_Room_Parent_Entity, Transform{});
		Coordinator::Instance()->GetComponent<EntityData>(m_Room_Parent_Entity).tag = "Room";
		sceneGraph.Push(-1, m_Room_Parent_Entity); // first cell is parent

		// Keep track of index of current tile in the Room
		u32 counter{ 0 };
		u32 c{ 0 }, r{ 0 };

		// Sprite for empty
		Sprite empty_sprite;
		empty_sprite.color.a = 0;

		// Clear enemy list
		enemyEntityList.clear();

		// Iterate every Map
		for (auto col : MapManager::Instance()->GetMap())
		{
			r = 0;
			for (auto row : col)
			{
				assert(counter < m_Room.roomSize);

				m_Room.roomCellsArray[counter] = Coordinator::Instance()->CreateEntity();

				sceneGraph.Push(m_Room_Parent_Entity, m_Room.roomCellsArray[counter]);

				//Set default transform and scale
				Transform transform;
				transform.scale = { 100, 100 };
				transform.localScale = { 100, 100 };
				transform.position = { 450 + (f32)r * 100.f, 150 + (f32)c * 100.f };
				Coordinator::Instance()->AddComponent(m_Room.roomCellsArray[counter], transform);

				// Cell coordinates
				Cell cell;
				cell.coordinate = { (s32)r, (s32)c };

				//Create the triggers and subscribe the cell related events
				ECS::CreateEventTrigger(m_Room.roomCellsArray[counter]);
				ECS::Subscribe(m_Room.roomCellsArray[counter], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_ClickCell);
				ECS::Subscribe(m_Room.roomCellsArray[counter], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterCell);
				ECS::Subscribe(m_Room.roomCellsArray[counter], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitCell);

				// Add the child overlay
				cell.child_overlay = Coordinator::Instance()->CreateEntity();

				Transform child_overlay_transform;
				child_overlay_transform.scale = transform.scale;
				child_overlay_transform.position = { 450 + (f32)r * 100.f, 150 + (f32)c * 100.f };
				Coordinator::Instance()->AddComponent(cell.child_overlay, child_overlay_transform);

				Coordinator::Instance()->AddComponent(getEntityCell(m_Room, r, c), cell);
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).tag = "Cell_Overlay[" + std::to_string(r) + "," + std::to_string(c) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(getEntityCell(m_Room, r, c)).tag = "Cell[" + std::to_string(r) + "," + std::to_string(c) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY	
				sceneGraph.Push(m_Room_Parent_Entity, cell.child_overlay); // other cells are children of the parent

				// For the bottom
				ECS::CreateSprite(cell.child_overlay, "Assets/Images/InitialTile_v04.png");

				// Put player tile
				if (row == "Player")
				{
					m_Room.playerX = r;
					m_Room.playerY = c; 
					//Coordinator::Instance()->AddComponent<Sprite>(m_Room.roomCellsArray[counter], empty_sprite);
					ECS::CreateSprite(m_Room.roomCellsArray[counter], "Assets/Images/InitialTile_v04.png");
				}
				// Skip "Empty" tiles
				else if (row != "Empty")
				{
					// Tile image file path
					std::string tile_image{ MapManager::Instance()->GetTileImage(row) };

					// Check for enemy
					if (row == "Enemy Melee" || 
						row == "Enemy Cell Destroyer" ||
						row == "Enemy Summoner")
					{
						ENEMY_TYPE enemy_type{};

						if (row == "Enemy Melee")
							enemy_type = ENEMY_TYPE::ENEMY_MELEE;
						else if (row == "Enemy Cell Destroyer")
							enemy_type = ENEMY_TYPE::ENEMY_CELL_DESTROYER;
						else if (row == "Enemy Summoner")
							enemy_type = ENEMY_TYPE::ENEMY_SUMMONER;
						// Place Enemy
						ECS::Entity enemyEntt = gameplaySystem_Enemy->PlaceNewEnemyInRoom(r, c, enemy_type, enemyEntityList, m_Room);
						ECS::Subscribe(enemyEntt, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
						ECS::Subscribe(enemyEntt, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);

						// Empty Tile under enemy
						ECS::CreateSprite(m_Room.roomCellsArray[counter], "Assets/Images/InitialTile_v04.png");
					}
					else if (row == "Vertical_Wall" || row == "Top_Wall" || row == "Horizontal_Wall" || row == "Bottom_Wall") {
						Coordinator::Instance()->GetComponent<Cell>(getEntityCell(m_Room, r, c)).m_isAccessible = false;
						Coordinator::Instance()->GetComponent<EventTrigger>(getEntityCell(m_Room, r, c)).isEnabled = false;
						ECS::CreateSprite(m_Room.roomCellsArray[counter], tile_image.c_str());
					}
					else
					{
						ECS::CreateSprite(m_Room.roomCellsArray[counter], tile_image.c_str());
					}
				}
				else
				{
					ECS::CreateSprite(m_Room.roomCellsArray[counter], "Assets/Images/InitialTile_v04.png");
					//Coordinator::Instance()->AddComponent<Sprite>(m_Room.roomCellsArray[counter], empty_sprite);
					//ToggleCellAccessibility(m_Room, r, c, false);
				}

				++counter;
				++r;
			}
			++c;
		}

		// Place Player and Tile Below Player
		PlaceNewPlayerInRoom(m_Room.playerX, m_Room.playerY);

		return true;
	}

	void GameplaySystem::Update(ECS::Entity en)
	{

	}

	void GameplaySystem::LateUpdate(ECS::Entity en)
	{
		UpdateGameplaySystem();
		DrawGameplaySystem();	
	}

	void GameplaySystem::Free(ECS::Entity en)
	{
		ExitGameplaySystem();
	}

	void GameplaySystem::Unload(ECS::Entity en)
	{
	}


	void GameplaySystem::StartGameplaySystem() {
		AL_CORE_INFO("GAME START");
		//Set up scene
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		m_Room.width = roomSize[0];
		m_Room.height = roomSize[1];
		m_Room.roomSize = getRoomSize();
		m_Room.roomCellsArray = new ECS::Entity[getRoomSize()];

		currentGameplayStatus = GAMEPLAY_STATUS::RUNNING;
		currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;
		currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;
		currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;

		//Initialize Room Parent 
		m_Room_Parent_Entity = Coordinator::Instance()->CreateEntity();
		Coordinator::Instance()->AddComponent(m_Room_Parent_Entity, Transform{});
		Coordinator::Instance()->GetComponent<EntityData>(m_Room_Parent_Entity).tag = "Room";

		sceneGraph.Push(-1, m_Room_Parent_Entity); // first cell is parent

		//Initialize Pattern
		InitializePatterns(pattern_Default);

		// Randomize Pattern
		RandomizePatternList();

		// Set sprites for the Patterns
		for (u32 i{ 1 }; i <= 4; ++i)
		{
			std::string tile_icon = "next_tile_icon" + std::to_string(i);

			ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
			sprite.id = Engine::AssetManager::Instance()->GetGuid(pattern_List[i - 1].file_path);
		}

		//Initialize Abilities
		InitializeAbilities(Abilities_List);

		//Initialize Pattern GUI
		gameplaySystem_GUI->InitializePatternGUI(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List);

		//Initialize abilities GUI
		gameplaySystem_GUI->InitializeAbilitiesGUI(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List);

		//Initialize General GUI
		gameplaySystem_GUI->InitializeGUI();

		//Create EndTurn Button
		InitializeEndTurnButton();

		//Add events for pattern Button
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_CurrentPattern);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_1);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List[2], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_2);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List[3], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Pattern_3);

		//Add events for abilities Button
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[0], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_0);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[1], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_1);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[2], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_2);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[3], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_3);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[4], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_4);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[5], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Select_Abilities_5);

		//Subscribe the restart button
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().Lose_Button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Restart);
		ECS::Subscribe(gameplaySystem_GUI->getGuiManager().Win_Button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_LoadLevel_2);

		Toggle_Gameplay_State(true);

		//Toggle the gui 
		gameplaySystem_GUI->ToggleAbilitiesGUI(false);
		gameplaySystem_GUI->TogglePatternGUI(true);

		//***** AUDIO Initialization ******//
		CreateAudioEntityMasterSource();
		masterAudioSource = Coordinator::Instance()->GetEntityByTag("Master Audio Source");
		Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);
		Engine::Audio& ad = as.GetAudio(AUDIO_GAMEPLAY_LOOP);
		ad.m_Channel = Engine::Channel::BGM;
		ad.m_Loop = true;
		ad.Play();

		buttonClickAudio = &as.GetAudio(AUDIO_CLICK_1);
		buttonClickAudio->m_Channel = Engine::Channel::SFX;
	}

	void GameplaySystem::UpdateGameplaySystem() {
		gameplaySystem_GUI->UpdateFpsLabel(); // update fps top right corner of screen

		//If right mouse button
		if (Input::KeyDown(KeyCode::MouseRightButton)) {
			//Deselect Pattern
			if (currentPhaseStatus == PHASE_STATUS::PHASE_SETUP) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(current_Moused_Over_Cell);

				DisplayFilterPlacementGrid(m_Room, cell.coordinate, selected_Pattern, { 1.f,1.f,1.f,1.f });
				currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;

				gameplaySystem_GUI->TogglePatternGUI(true);
			}
			//Deselect Abilities
			else if (currentPhaseStatus == PHASE_STATUS::PHASE_ACTION) {
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(current_Moused_Over_Cell);

				DisplayFilterPlacementGrid(m_Room, cell.coordinate, selected_Pattern, { 1.f,1.f,1.f,1.f });
				currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;

				gameplaySystem_GUI->TogglePatternGUI(false);
				gameplaySystem_GUI->ToggleAbilitiesGUI(true);

				Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

				gameplaySystem_GUI->Update_AP_UI(playerUnit.actionPoints);
			}
		}

		//Toggle debug draw
		if (Input::KeyTriggered(KeyCode::Key_3)) {
			is_DebugDraw = !is_DebugDraw;
		}

		//******CHEAT KEYS******//
		if (Input::KeyTriggered(KeyCode::F1)) {
			Cheat_ToggleGodMode();
		}

		if (Input::KeyTriggered(KeyCode::F2)) {
			Cheat_ToggleDoubleAbilitiesDoubleDamage();
		}

		if (Input::KeyTriggered(KeyCode::F3)) {
			Cheat_ResetPlayerHealth();
		}

		if (Input::KeyTriggered(KeyCode::F4)) {
			Cheat_ResetAllEnemiesHealth();
		}

		if (Input::KeyTriggered(KeyCode::F5)) {
			Cheat_DecreaseEnemyHealthToOne();
		}

		if (Input::KeyTriggered(KeyCode::F6)) {
			Cheat_IncreasePlayerHealth(10);
		}

		if (Input::KeyTriggered(KeyCode::F7)) {
			EndTurn();
		}

		if (Input::KeyTriggered(KeyCode::F8)) {
			Cheat_EliminateAllEnemy();

		}

		if (Input::KeyTriggered(KeyCode::F9)) {
			Cheat_ClearFloorWalkability();
		}

		if (Input::KeyTriggered(KeyCode::RightShift)) {

			Engine::Scene::LoadScene("Assets\\Level_1.scene");
		}

		if (Input::KeyTriggered(KeyCode::E)) {
			RotatePattern(1);
		}
		else if (Input::KeyTriggered(KeyCode::Q)) {
			RotatePattern(-1);
		}
		//******END CHEAT KEYS******//

		RunGameState();
		UpdateUnitSpriteLayer();
	}

	void GameplaySystem::ExitGameplaySystem() {
		gameplaySystem_Enemy.reset();
		gameplaySystem_GUI.reset();
		
		Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);
		for (auto& it : as.list)
		{
			Engine::Audio& ad = it.second;
			ad.Stop();
		}
	}


	void GameplaySystem::DrawGameplaySystem() {

		if (currentGameplayStatus == GAMEPLAY_STATUS::STOP) {
			return;
		}

		if (!is_DebugDraw)
		{
			return;
		}

		//Box holder
		Engine::Vector2 bottomleft;
		Engine::Vector2 topright;

		Color color = { 0.f,0.f ,0.f ,0.f };

		//Draw all cells
		for (s32 i = 0; i < static_cast<s32>(m_Room.roomSize); ++i) {
			Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(m_Room.roomCellsArray[i]);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(m_Room.roomCellsArray[i]);

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
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		bottomleft = { playerTransform.position.x - playerTransform.scale.x * 0.5f, playerTransform.position.y - playerTransform.scale.y * 0.5f };
		topright = { playerTransform.position.x + playerTransform.scale.x * 0.5f, playerTransform.position.y + playerTransform.scale.y * 0.5f };

		color = { 1.f, 1.f, 0.f, 1.f };

		Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
		Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
		Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
		Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right

		//Draw the Pattern GUI
		for (int i = 0; i < gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List[i]);

			bottomleft = { buttonTransform.position.x - buttonTransform.scale.x * 0.5f, buttonTransform.position.y - buttonTransform.scale.y * 0.5f };
			topright = { buttonTransform.position.x + buttonTransform.scale.x * 0.5f, buttonTransform.position.y + buttonTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}

		//Draw the Pattern GUI
		for (int i = 0; i < gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List.size(); ++i) {
			Transform& buttonTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List[i]);

			bottomleft = { buttonTransform.position.x - buttonTransform.scale.x * 0.5f, buttonTransform.position.y - buttonTransform.scale.y * 0.5f };
			topright = { buttonTransform.position.x + buttonTransform.scale.x * 0.5f, buttonTransform.position.y + buttonTransform.scale.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
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
}