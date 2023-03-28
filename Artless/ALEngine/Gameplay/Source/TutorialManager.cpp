/*!
file:	TutorialManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	File that contains definitions for functions needed for the Tutorial

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameplaySystem.h>

namespace Gameplay
{
	namespace
	{

	}

	void TutorialManager::BeginTutorial(void)
	{
		m_CurrentState = TutorialState::TUTORIAL_INTRO_CS;
		m_TutorialIsPlaying = true;
	}

	void TutorialManager::NextState(void)
	{
		m_CurrentState = static_cast<TutorialState>(static_cast<u32>(m_CurrentState) + 1);

		// End of tutorial, start level 1
		if (m_CurrentState >= TutorialState::TUTORIAL_END)
		{
			return;
		}

		// Begin next state
		switch (m_CurrentState)
		{
			// ===== Cutscenes =====
		case TutorialState::TUTORIAL_ACTION_PHASE_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Action Phase");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_MELEE_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Melee Enemy Intro");
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_HARD_DROP_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Hard Drop Skill Intro");
			Time::m_Scale = 0.f;
			break;


			// ===== Scripted Gameplay =====
		case TutorialState::TUTORIAL_SELECT_TILE:
			//m_GameplaySystem->
			Time::m_Scale = 1.f;
			break;
		default:
			Time::m_Scale = 1.f;
		}
	}

	b8 TutorialManager::TutorialIsPlaying(void)
	{
		return m_TutorialIsPlaying;
	}

	TutorialState TutorialManager::GetState(void)
	{
		return m_CurrentState;
	}

	ALEngine::ECS::Entity TutorialManager::GetTutorialObject(void)
	{
		return m_TutorialObject;
	}
	
	void TutorialManager::SetTutorialObject(ALEngine::ECS::Entity tut_obj)
	{
		m_TutorialObject = tut_obj;
	}
	
	b8 TutorialManager::GetTileIsSelected(void)
	{
		return m_TileIsSelected;
	}

	void TutorialManager::SetTileIsSelected(b8 tileIsSelected)
	{
		m_TileIsSelected = tileIsSelected;
	}
	
	b8 TutorialManager::GetTileIsPlaced(void)
	{
		return m_TileIsPlaced;
	}

	void TutorialManager::SetTileIsPlaced(b8 tileIsPlaced)
	{
		m_TileIsPlaced = tileIsPlaced;
	}

	ALEngine::ECS::Entity TutorialManager::GetPlayerEntity(void)
	{
		return m_PlayerObject;
	}

	void TutorialManager::SetPlayerEntity(ALEngine::ECS::Entity en)
	{
		m_PlayerObject = en;
	}

	std::shared_ptr<ALEngine::Script::GameplaySystem> TutorialManager::GetGameplaySystem(void)
	{
		return m_GameplaySystem;
	}
	void TutorialManager::SetGameplaySystem(std::shared_ptr<ALEngine::Script::GameplaySystem> gs)
	{
		m_GameplaySystem = gs;
	}
}