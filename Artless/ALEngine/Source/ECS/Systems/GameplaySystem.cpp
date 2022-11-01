/*!
file:	CharacterControllerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for CharacterControllerSystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"
#include "Engine/PathFindingManager.h"

namespace ALEngine::ECS
{
	//Ease of use for ray
	using Engine::Physics::Ray2D;
	//using Engine::AI;
	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	/*!*********************************************************************************
		\brief
			Character Controller System, contains functions needed to run components for CharacterController
	***********************************************************************************/
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
		uint32_t roomSize[2]{ 7, 7 };
		Entity *roomCellsArray;

		Entity playerEntity, startCellEntity, targetCellEntity;
		std::vector<Entity> pathEntityVector;

		GAMEPLAYSTATUS currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_INPUT_WAITING;

		MoveOrder currentModeOrder;
		Entity getCurrentEntityCell();

		//Return if reached end
		bool StepUpModeOrderPath(MoveOrder& order);
		uint32_t getRoomSize();
		Entity getEntityCell(uint32_t x, uint32_t y);
		void RunGameState();
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> gameplaySystem;
	}

	void RegisterGameplaySystem(void)
	{
		gameplaySystem = Coordinator::Instance()->RegisterSystem<GameplaySystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Unit>());
		Coordinator::Instance()->SetSystemSignature<GameplaySystem>(signature);
	}

	void StartGameplaySystem(void) {
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();
		gameplaySystem->roomCellsArray = new Entity[gameplaySystem->getRoomSize()];

		for (uint32_t i = 0; i < gameplaySystem->getRoomSize(); ++i) {	
			gameplaySystem->roomCellsArray[i] = Coordinator::Instance()->CreateEntity();

			if (i == 0)
			{
				sceneGraph.Push(-1, gameplaySystem->roomCellsArray[i]); // first cell is parent
			}
			else
			{
				sceneGraph.Push(gameplaySystem->roomCellsArray[0], gameplaySystem->roomCellsArray[i]); // other cells are children of the parent
			}

			CreateCollider(gameplaySystem->roomCellsArray[i]);
			Coordinator::Instance()->GetComponent<Collider2D>(gameplaySystem->roomCellsArray[i]).isTrigger = true;
			Transform transform;
			transform.scale = { 70, 70 };
			Coordinator::Instance()->AddComponent(gameplaySystem->roomCellsArray[i], transform);
		}

		for (uint32_t i = 0; i < gameplaySystem->roomSize[0]; ++i) {
			for (uint32_t j = 0; j < gameplaySystem->roomSize[1]; ++j) {
				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem->roomCellsArray[i * gameplaySystem->roomSize[0] + j]);
				transform.position = { 200 + (f32)i * 100.f, 200 + (f32)j * 100.f };
				Cell cell;
				cell.coordinate[0] = i;
				cell.coordinate[1] = j;
				
				Coordinator::Instance()->AddComponent(gameplaySystem->getEntityCell(i,j), cell);
			}
		}

		for (auto it = gameplaySystem->mEntities.begin(); it != gameplaySystem->mEntities.end(); ++it) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(*it);
			if (unit.unitType == UNIT_TYPE::PLAYER) {
				gameplaySystem->playerEntity = *it;
				break;
			}
		}			
	}

	void UpdateGameplaySystem(void)
	{
		gameplaySystem->RunGameState();
	}

	void ExitGameplaySystem(void)
	{
		delete[] gameplaySystem->roomCellsArray;
	}


	Entity GameplaySystem::getCurrentEntityCell() {
		return gameplaySystem->currentModeOrder.path[gameplaySystem->currentModeOrder.path_step];
	}

	uint32_t GameplaySystem::getRoomSize() {
		return gameplaySystem->roomSize[0] * gameplaySystem->roomSize[1];
	}

	Entity GameplaySystem::getEntityCell(uint32_t x, uint32_t y) {
		return gameplaySystem->roomCellsArray[y * gameplaySystem->roomSize[0] + x];
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
		switch (gameplaySystem->currentGameplayStatus)
		{
		case GAMEPLAYSTATUS::PLAYER_INPUT_WAITING:
			gameplaySystem->pathEntityVector.clear();

			gameplaySystem->targetCellEntity = gameplaySystem->getEntityCell(2, 2);
			Unit playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
			gameplaySystem->startCellEntity = gameplaySystem->getEntityCell(playerUnit.coordinate[0], playerUnit.coordinate[1]);


			gameplaySystem->pathEntityVector = Engine::AI::FindPath(gameplaySystem->roomCellsArray, gameplaySystem->roomSize, gameplaySystem->startCellEntity, gameplaySystem->targetCellEntity);

			std::cout << "Path Size | " <<  gameplaySystem->pathEntityVector.size() << std::endl;
			for (int i = 0; i < gameplaySystem->pathEntityVector.size(); ++i) {
				Cell c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->pathEntityVector[i]);

				std::cout << c.m_Grid << " : ";
			}

			std::cout << std::endl;

			gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_MOVING;
			break;

		case GAMEPLAYSTATUS::PLAYER_MOVING:
			//movement of player

			//gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::ENEMY_PLANNING;
			break;

		case GAMEPLAYSTATUS::ENEMY_PLANNING:
			

			gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::ENEMY_MOVING;
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
}
//for (auto it = gameplaySystem->mEntities.begin(); it != gameplaySystem->mEntities.end(); ++it) {
//	Unit& unit = Coordinator::Instance()->GetComponent<Unit>(*it);
//	if (unit.unitType == UNIT_TYPE::PLAYER) {
//		gameplaySystem->currentModeOrder.entity = *it;

//		gameplaySystem->currentModeOrder.path.clear();
//		//find path and set the path

//		//gameplaySystem->currentModeOrder.path =std::move(Engine::AI::FindPath(*gameplaySystem->roomCellsArray, gameplaySystem->getCurrentEntityCell(), gameplaySystem->getEntityCell(3, 3) , false) );
//		for (int i = 0; i < gameplaySystem->currentModeOrder.path.size(); ++i)
//		{
//			Cell& c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->currentModeOrder.path[i]);
//			std::cout << c.m_Grid<<std::endl;
//		}

//		//gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::PLAYER_MOVING;
//		break;
//	}
//}	



//
////Shift through each component
//for (auto it = gameplaySystem->mEntities.begin(); it != gameplaySystem->mEntities.end(); ++it) {
//	Unit& unit = Coordinator::Instance()->GetComponent<Unit>(*it);
//	if (unit.unitType == UNIT_TYPE::ENEMY) {
//		gameplaySystem->currentModeOrder.entity = *it;
//
//		gameplaySystem->currentModeOrder.path.clear();
//
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(0, 0));
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(0, 1));
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(1, 1));
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(1, 2));
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(2, 2));
//		gameplaySystem->currentModeOrder.path.push_back(gameplaySystem->getEntityCell(2, 3));
//		gameplaySystem->currentGameplayStatus = GAMEPLAYSTATUS::ENEMY_MOVING;
//		break;
//	}
//}