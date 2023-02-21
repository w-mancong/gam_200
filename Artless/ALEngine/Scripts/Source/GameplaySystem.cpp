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

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr < GameplaySystem_Interface_Management_Enemy> gameplaySystem_Enemy;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
	}

	void GameplaySystem::Load(ECS::Entity en)
	{
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		gameplaySystem_Enemy = ECS::GetLogicComponent<GameplaySystem_Interface_Management_Enemy>(en);
		//Set_GameplayInterface_GameplayManager(this);
		//GameplaySystem_Interface_Management_Enemy::Set_GameplayManager(this);
		std::cout << "setting up the system\n";
	}

	void GameplaySystem::Init(ECS::Entity en)
	{
		std::cout << "initializing system\n";
		StartGameplaySystem();
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
	
	}

	void GameplaySystem::Unload(ECS::Entity en)
	{
		ExitGameplaySystem();
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

		//Initialize Pattern GUI
		gameplaySystem_GUI->InitializePatternGUI(gameplaySystem_GUI->getGuiManager().GUI_Pattern_Button_List);

		//Initialize abilities GUI
		gameplaySystem_GUI->InitializeAbilitiesGUI(gameplaySystem_GUI->getGuiManager().GUI_Abilities_Button_List);

		//Initialize General GUI
		gameplaySystem_GUI->InitializeGUI();

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

		//Initialize the room and cells
		for (uint32_t i = 0; i < getRoomSize(); ++i) {
			m_Room.roomCellsArray[i] = Coordinator::Instance()->CreateEntity();

			sceneGraph.Push(m_Room_Parent_Entity, m_Room.roomCellsArray[i]); // other cells are children of the parent

			//Set default transform and scale
			Transform transform;
			transform.scale = { 100, 100 };
			transform.localScale = { 100, 100 };

			Coordinator::Instance()->AddComponent(m_Room.roomCellsArray[i], transform);

			ECS::CreateSprite(m_Room.roomCellsArray[i], "Assets/Images/InitialTile_v04.png");
		}

		//Set up the event triggers for the cells
		for (s32 i = 0; i < static_cast<s32>(roomSize[0]); ++i) {
			for (s32 j = 0; j < static_cast<s32>(roomSize[1]); ++j) {
				//Determine the cell index
				s32 cellIndex = i * roomSize[0] + j;

				//Get the transform component
				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(m_Room.roomCellsArray[cellIndex]);

				//Set the physical position according from the coordinates
				transform.position = { 450 + (f32)j * 100.f, 150 + (f32)i * 100.f };

				//Set the coordinates for the cell
				Cell cell;
				cell.coordinate = { i,j };

				//Create the triggers and subscribe the cell related events
				ECS::CreateEventTrigger(m_Room.roomCellsArray[cellIndex]);
				ECS::Subscribe(m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_ClickCell);
				ECS::Subscribe(m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterCell);
				ECS::Subscribe(m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitCell);

				//Add the child overlay
				cell.child_overlay = Coordinator::Instance()->CreateEntity();

				sceneGraph.Push(m_Room_Parent_Entity, cell.child_overlay); // other cells are children of the parent
				ECS::CreateSprite(cell.child_overlay, "Assets/Images/InitialTile_v04.png");

				Transform child_overlay_transform;
				child_overlay_transform.scale = transform.scale;
				//child_overlay_transform.scale.y += 50;
				child_overlay_transform.position = { 450 + (f32)i * 100.f, 150 + (f32)j * 100.f };
				//child_overlay_transform.position.y += 50 >> 2;
				Coordinator::Instance()->AddComponent(cell.child_overlay, child_overlay_transform);

				Coordinator::Instance()->AddComponent(getEntityCell(m_Room, i, j), cell);
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).tag = "Cell_Overlay[" + std::to_string(i) + "," + std::to_string(j) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(getEntityCell(m_Room, i, j)).tag = "Cell[" + std::to_string(i) + "," + std::to_string(j) + "]";
				Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY	
			}
		}

		//Create Player
		PlaceNewPlayerInRoom(0, 2);

		//std::cout << (Gameplay_Mangement_Singleton::Instance()->Instance()->gameplaySystem == nullptr ? "system not set up\n" : "system is up\n");
		//printf("Address of a: %p\n", *this);
		//Place enemy
		enemyEntityList.clear();
		
		ECS::Entity enemyEntity = gameplaySystem_Enemy->PlaceNewEnemyInRoom(5, 1, ENEMY_TYPE::ENEMY_MELEE, enemyEntityList, m_Room);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);

		//Place enemy
		enemyEntity = gameplaySystem_Enemy->PlaceNewEnemyInRoom(8, 5, ENEMY_TYPE::ENEMY_MELEE, enemyEntityList, m_Room);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);

		//Place enemy
		enemyEntity = gameplaySystem_Enemy->PlaceNewEnemyInRoom(4, 2, ENEMY_TYPE::ENEMY_CELL_DESTROYER, enemyEntityList, m_Room);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		ECS::Subscribe(enemyEntity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);

		//Create EndTurn Button
		//InitializeEndTurnButton();

		//******* Set inaccessible cells ********//
		ToggleCellAccessibility(m_Room, 2, 3, false);
		ToggleCellAccessibility(m_Room, 2, 4, false);
		ToggleCellAccessibility(m_Room, 3, 3, false);
		ToggleCellAccessibility(m_Room, 3, 4, false);

		ToggleCellAccessibility(m_Room, 4, 0, false);
		ToggleCellAccessibility(m_Room, 4, 1, false);
		ToggleCellAccessibility(m_Room, 5, 0, false);
		ToggleCellAccessibility(m_Room, 6, 1, false);

		ToggleCellAccessibility(m_Room, 0, 7, false);
		ToggleCellAccessibility(m_Room, 1, 7, false);

		ToggleCellAccessibility(m_Room, 5, 7, false);
		ToggleCellAccessibility(m_Room, 6, 7, false);
		ToggleCellAccessibility(m_Room, 7, 7, false);
		ToggleCellAccessibility(m_Room, 7, 6, false);
		ToggleCellAccessibility(m_Room, 7, 5, false);


		//***** AUDIO Initialization ******//
		//GameplayInterface_Management_Enemy::EnemyManager_LoadData();
		CreateAudioEntityMasterSource();
		masterAudioSource = Coordinator::Instance()->GetEntityByTag("Master Audio Source");
		Engine::AudioSource& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(masterAudioSource);
		Engine::Audio& ad = as.GetAudio(AUDIO_GAMEPLAY_LOOP);
		ad.m_Channel = Engine::Channel::BGM;
		ad.m_Loop = true;
		ad.Play();
	}

	void GameplaySystem::UpdateGameplaySystem() {
		gameplaySystem_GUI->UpdateFpsLabel(); // update fps top right corner of screen

		if (Input::KeyTriggered(KeyCode::E)) {
			EndTurn();
		}
	}

	void GameplaySystem::ExitGameplaySystem() {
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
}