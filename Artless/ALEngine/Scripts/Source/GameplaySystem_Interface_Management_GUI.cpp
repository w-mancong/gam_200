#include <pch.h>
#include <GameplayInterface_Management_GUI.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>

namespace ALEngine::Script
{
	namespace {
		GameplaySystem* gameplaySystem;
	}


	void GameplaySystem_Interface_Management_GUI::Load(ECS::Entity en)
	{
		GameplaySystem::Set_GameplayManager_GUI(this);
		Set_GameplayInterface_GUI(this);
	}

	void GameplaySystem_Interface_Management_GUI::Init(ECS::Entity en){

	}

	void GameplaySystem_Interface_Management_GUI::Update(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::LateUpdate(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::Free(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::Unload(ECS::Entity en)
	{

	}
}