#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>

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
}