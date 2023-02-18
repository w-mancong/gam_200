#include <pch.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	void GameplaySystem::Load(ECS::Entity en)
	{

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

	}

	void GameplaySystem::UpdateGameplaySystem() {
		if (Input::KeyTriggered(KeyCode::E)) {
			EndTurn();
		}
	}


	void GameplaySystem::ExitGameplaySystem() {

	}


	void GameplaySystem::DrawGameplaySystem() {

	}
}