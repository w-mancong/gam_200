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
	//using Engine::AI;
	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	class GameplaySystem : public System
	{
		enum class GAMEPLAYSTATUS
		{
			PLAYER_INPUT_WAITING,
			PLAYER_MOVING,
			ENEMY_PLANNING,
			ENEMY_MOVING
		};

		struct MoveOrder {
			Entity entity;
			std::vector<Entity> path;
			uint32_t path_step = 0;
		};

	public:
		uint32_t roomSize[2]{ 6, 6 };
		Room m_Room;

		Entity playerEntity, startCellEntity, targetCellEntity;

		GAMEPLAYSTATUS currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_INPUT_WAITING;

		MoveOrder currentModeOrder;
		void ClearMoveOrder();
		Entity getCurrentEntityCell();
		void SetMoveOrder(std::vector<Entity> path);

		//Return if reached end
		bool StepUpModeOrderPath(MoveOrder& order);
		uint32_t getRoomSize();
		Entity getEntityCell(uint32_t x, uint32_t y);
		void ToggleCellToInaccessible(uint32_t x, uint32_t y, b8 istrue);
		void RunGameState();

		void MovePlayerEntityToCell(Entity cellEntity);

		void RunGameStateWaitPlayerInput();
		void RunGameStateMoving();
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> gameplaySystem;

	}

	void ClickSelectCell(Entity invokerCell) {
		//When click on cell, Move the player unit to the selected cell
		gameplaySystem->MovePlayerEntityToCell(invokerCell);

		AL_CORE_INFO("Select Cell");
	}

	void RegisterGameplaySystem(void)
	{
		//gameplaySystem = Coordinator::Instance()->RegisterSystem<GameplaySystem>();
		//Signature signature;
		//signature.set(Coordinator::Instance()->GetComponentType<Unit>());
		//Coordinator::Instance()->SetSystemSignature<GameplaySystem>(signature);
	}

	void StartGameplaySystem(void) {
		//Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		//gameplaySystem->m_Room.width = gameplaySystem->roomSize[0];
		//gameplaySystem->m_Room.height = gameplaySystem->roomSize[1];
		//gameplaySystem->m_Room.roomSize = gameplaySystem->getRoomSize();
		//gameplaySystem->m_Room.roomCellsArray = new Entity[gameplaySystem->getRoomSize()];

		//for (uint32_t i = 0; i < gameplaySystem->getRoomSize(); ++i) {	
		//	gameplaySystem->m_Room.roomCellsArray[i] = Coordinator::Instance()->CreateEntity();

		//	if (i == 0)
		//	{
		//		sceneGraph.Push(-1, gameplaySystem->m_Room.roomCellsArray[i]); // first cell is parent
		//	}
		//	else
		//	{
		//		sceneGraph.Push(gameplaySystem->m_Room.roomCellsArray[0], gameplaySystem->m_Room.roomCellsArray[i]); // other cells are children of the parent
		//	}

		//	Transform transform;
		//	transform.scale = { 70, 70 };
		//	Coordinator::Instance()->AddComponent(gameplaySystem->m_Room.roomCellsArray[i], transform);
		//}

		//for (uint32_t i = 0; i < gameplaySystem->roomSize[0]; ++i) {
		//	for (uint32_t j = 0; j < gameplaySystem->roomSize[1]; ++j) {
		//		int cellIndex = i * gameplaySystem->roomSize[0] + j;

		//		Transform& transform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[cellIndex]);
		//		transform.position = { 200 + (f32)j * 100.f, 200 + (f32)i * 100.f };
		//		Cell cell;
		//		cell.coordinate[0] = i;
		//		cell.coordinate[1] = j;

		//		CreateEventTrigger(gameplaySystem->m_Room.roomCellsArray[cellIndex]);

		//		Subscribe(gameplaySystem->m_Room.roomCellsArray[cellIndex], EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, ClickSelectCell);
		//		
		//		Coordinator::Instance()->AddComponent(gameplaySystem->getEntityCell(i,j), cell);
		//	}
		//}

		//for (auto it = gameplaySystem->mEntities.begin(); it != gameplaySystem->mEntities.end(); ++it) {
		//	Unit& unit = Coordinator::Instance()->GetComponent<Unit>(*it);
		//	if (unit.unitType == UNIT_TYPE::PLAYER) {
		//		gameplaySystem->playerEntity = *it;
		//		unit.coordinate[0] = 0;
		//		unit.coordinate[1] = 0;

		//		Transform& SpawnCellTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->getEntityCell(unit.coordinate[0], unit.coordinate[1]));
		//		Transform& playertransform = Coordinator::Instance()->GetComponent<Transform>(*it);
		//		playertransform.position = SpawnCellTransform.position;
		//		break;
		//	}
		//}	

		////Set a few blocks to be inaccessible
		//gameplaySystem->ToggleCellToInaccessible(1, 0, false);
		//gameplaySystem->ToggleCellToInaccessible(1, 1, false);
		//gameplaySystem->ToggleCellToInaccessible(1, 2, false);
		//gameplaySystem->ToggleCellToInaccessible(2, 1, false);
		//gameplaySystem->ToggleCellToInaccessible(3, 1, false);
		//gameplaySystem->ToggleCellToInaccessible(3, 2, false);

	}

	void UpdateGameplaySystem(void)
	{
		gameplaySystem->RunGameState();
	}

	void ExitGameplaySystem(void)
	{
		delete[] gameplaySystem->m_Room.roomCellsArray;
	}

	Entity GameplaySystem::getCurrentEntityCell() {
		return gameplaySystem->currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
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
		//currentModeOrder.path = std::move(path);
	}

	uint32_t GameplaySystem::getRoomSize() {
		return gameplaySystem->roomSize[0] * gameplaySystem->roomSize[1];
	}

	Entity GameplaySystem::getEntityCell(uint32_t x, uint32_t y) {
		return gameplaySystem->m_Room.roomCellsArray[y * gameplaySystem->roomSize[0] + x];
	}

	void GameplaySystem::ToggleCellToInaccessible(uint32_t x, uint32_t y, b8 istrue) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(x, y));
		cell.m_isAccesible = istrue;
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
		switch (gameplaySystem->currentGameplayStatus){
		case GAMEPLAYSTATUS::PLAYER_INPUT_WAITING:
			RunGameStateWaitPlayerInput();
			break;

		case GAMEPLAYSTATUS::PLAYER_MOVING:
			RunGameStateMoving();
			break;

		case GAMEPLAYSTATUS::ENEMY_PLANNING:
			

			//gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::ENEMY_MOVING;
			break;

		case GAMEPLAYSTATUS::ENEMY_MOVING:
			gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_INPUT_WAITING;
			break;
		}
	}

	void CreatePlayerUnit(Entity const& entity) {
		//Setup rigidbody for custom stats
		Unit unit{};
		unit.unitType = UNIT_TYPE::PLAYER;
		Coordinator::Instance()->AddComponent(entity, unit);
	}

	void CreateEnemyUnit(Entity const& entity) {
		//Setup rigidbody for custom stats
		Unit unit{};
		unit.unitType = UNIT_TYPE::ENEMY;
		Coordinator::Instance()->AddComponent(entity, unit);
	}

	void GameplaySystem::MovePlayerEntityToCell(Entity cellEntity) {
		gameplaySystem->ClearMoveOrder();

		gameplaySystem->targetCellEntity = cellEntity;

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		Unit playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		gameplaySystem->startCellEntity = gameplaySystem->getEntityCell(playerUnit.coordinate[0], playerUnit.coordinate[1]);
		
		std::vector<ECS::Entity> pathList;
		bool isPathFound = Engine::AI::FindPath(gameplaySystem->m_Room, gameplaySystem->startCellEntity, gameplaySystem->targetCellEntity, pathList);
		
		if (!isPathFound) {
			AL_CORE_INFO("No Path Found");
			return;
		}

		gameplaySystem->SetMoveOrder(pathList);

		gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_MOVING;
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
		Vector2 direction = Vector3::Normalize(cellTransform.position - playerTransform.position);

		playerTransform.position += direction * 100.0f * Time::m_DeltaTime;

		if (Vector3::Distance(playerTransform.position, cellTransform.position) < 1.0f) {
			Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getCurrentEntityCell()); 

			playerTransform.position = cellTransform.position;
			playerUnit.coordinate[0] = cell.coordinate[0];
			playerUnit.coordinate[1] = cell.coordinate[1];

			bool isEndOfPath = StepUpModeOrderPath(currentModeOrder);

			if (isEndOfPath) {
				currentGameplayStatus = GAMEPLAYSTATUS::ENEMY_PLANNING;
				return;
			}
		}
	}


	void DrawGameplaySystem() {
		//Box holder
		Vector2 bottomleft;
		Vector2 topright;

		Color color = { 0.f,0.f ,0.f ,0.f };

		//Draw all cells
		for (int i = 0; i < gameplaySystem->m_Room.roomSize; ++i) {
			Transform& cellTransform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->m_Room.roomCellsArray[i]);
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->m_Room.roomCellsArray[i]);

			if (cell.m_isAccesible) {
				color = { 0.f, 1.f, 0.f, 1.f };
			}
			else {
				color = { 1.f, 0.f, 0.f, 1.f };
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
	}
}
