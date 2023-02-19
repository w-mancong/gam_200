#include <pch.h>
#include "Engine/Physics2D.h"
#include "Engine/PathFindingManager.h"
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <Utility/AudioNames.h>
#include <GameplayCamera.h>
#include <PauseLogic.h>

namespace ALEngine::Script
{
	void GameplaySystem::Load(ECS::Entity en)
	{
		gameplaySystem = this;
	}

	void GameplaySystem::Init(ECS::Entity en)
	{
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
		//InitializePatterns(gameplaySystem->pattern_Default);

		// Randomize Pattern
		//gameplaySystem->RandomizePatternList();

		//// Set sprites for the Patterns
		//for (u32 i{ 1 }; i <= 4; ++i)
		//{
		//	std::string tile_icon = "next_tile_icon" + std::to_string(i);

		//	ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);
		//	Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
		//	sprite.id = Engine::AssetManager::Instance()->GetGuid(pattern_List[i - 1].file_path);
		//}

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
	}

	void GameplaySystem::UpdateGameplaySystem() {
		if (Input::KeyTriggered(KeyCode::E)) {
			EndTurn();
		}
	}


	void GameplaySystem::ExitGameplaySystem() {
		gameplaySystem = nullptr;
	}


	void GameplaySystem::DrawGameplaySystem() {

	}
}