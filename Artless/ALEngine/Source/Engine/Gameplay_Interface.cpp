#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <Engine/GameplayInterface_Management_GUI.h>
#include <Engine/GameplayInterface_Management_Enemy.h>
#include <ECS/Systems/LogicSystem.h>
#include <GameplayCamera.h>

namespace ALEngine::Script
{
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

		//Animator an = ECS::CreateAnimator("Player");
		//Coordinator::Instance()->AddComponent(playerUnit.unit_Sprite_Entity, an);

		Coordinator::Instance()->GetComponent<EntityData>(entity).tag = "Player";
		Coordinator::Instance()->GetComponent<EntityData>(playerUnit.unit_Sprite_Entity).tag = "Player_Sprite";

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		sceneGraph.Push(-1, entity); // first cell is parent
		sceneGraph.Push(entity, playerUnit.unit_Sprite_Entity);

		//Add events
		EventTrigger eventTrigger;
		eventTrigger.layer = 1;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);
		//Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Unit_OnSelect);
		//Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_MouseEnterUnit);
		//Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_MouseExitUnit);
		//AddLogicComponent<Script::PauseLogic>(entity);

		//Camera Logic
		ECS::AddLogicComponent<Script::GameplayCamera>(entity);

		//Add physics
		ECS::CreateRigidbody(entity);
		Rigidbody2D& rigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(entity);
		rigidbody.drag = { 0,0 };
		rigidbody.mass = 0.1f;
		rigidbody.hasGravity = false;
	}

	void GameplaySystem::CreateEnemyUnit(ECS::Entity entity) {

	}

	void GameplaySystem::EndTurn()
	{
		//Set the turn accordingly
		switch (currentPhaseStatus) {
		case PHASE_STATUS::PHASE_SETUP:
			currentPhaseStatus = PHASE_STATUS::PHASE_ACTION;
			AL_CORE_DEBUG("Loading PHASE ACTION");
			break;

		case PHASE_STATUS::PHASE_ACTION:
			currentPhaseStatus = PHASE_STATUS::PHASE_ENEMY;
			AL_CORE_DEBUG("Loading PHASE ENEMY");
			break;

		case PHASE_STATUS::PHASE_ENEMY:
			currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;
			AL_CORE_DEBUG("Loading PHASE SETUP");
			break;
		}
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
		playerUnit.maxMovementPoints = 4;
		playerUnit.movementPoints = playerUnit.maxMovementPoints;

		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).unitEntity = playerEntity;
		Coordinator::Instance()->GetComponent<Cell>(playerUnit.m_CurrentCell_Entity).hasUnit = true;

		//Set health
		playerUnit.maxHealth = 50;
		playerUnit.health = 50;

		//Set the cell the player is on to be walkable
		PlaceWalkableOnGrid(gameplaySystem->m_Room, { x, y }, "Assets/Images/Walkable.png");

		//Set the transform
		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(getEntityCell(m_Room, playerUnit.coordinate[0], playerUnit.coordinate[1]));
		Transform& playerTransform = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
		playerTransform.localPosition = SpawnCellTransform.position;
	}

	void GameplaySystem::PlaceWalkableOnGrid(Room& room, Math::Vector2Int coordinate, std::string sprite_fileName) {
		////Get Cell Entity
		//ECS::Entity cellEntity = getEntityCell(room, coordinate.x, coordinate.y);

		////Get Component
		//Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		////if is completely block, dont need set
		//if (!cell.m_isAccessible) {
		//	return;
		//}

		////Set to canwalk
		//cell.m_canWalk = true;
		//cell.m_resetCounter = 2;

		////Change the cell sprite to filename sprite
		//Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
		//sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite_fileName);
	}

	void Event_ClickCell(ECS::Entity invoker) {
		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);
	}
	void Event_MouseEnterCell(ECS::Entity invoker) {
		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);
	}

	void Event_MouseExitCell(ECS::Entity invoker) {
		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(invoker);
		//Filter it's placement 
		//DisplayFilterPlacementGrid(gameplaySytem->m_Room, cell.coordinate, gameplaySytem->selected_Pattern, { 1.f,1.f,1.f,1.f });
	}
}