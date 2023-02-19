#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <Engine/GameplayInterface_Management_GUI.h>
#include <Engine/GameplayInterface_Management_Enemy.h>

namespace ALEngine::Script
{
	void GameplaySystem::CreatePlayerUnit(ECS::Entity entity) {

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

		u32 testValue = currentRoom.roomCellsArray[y * currentRoom.width + x];

		std::cout << testValue << std::endl;
		return currentRoom.roomCellsArray[y * currentRoom.width + x];
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