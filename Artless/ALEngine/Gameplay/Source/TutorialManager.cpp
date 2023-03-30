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
		m_TileIsSelected = false;
		m_TileIsPlaced = false;
		m_PlayerMoveFinished = false;
		m_EndTurnPressed = false;
		m_PlayerTurnStart = false;
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
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_CRACKING_TILES_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Cracking Tiles");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_HARD_DROP_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Hard Drop Skill Intro");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_ABILITIES_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Ability Explanation");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_SHOW_HOVER_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Show Hover");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_TILE_DESTROYER_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Tile Destroyer");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_UTILITIES_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Utility Skills Explanation");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_DEFEAT_TILE_DESTROYER_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Construct Tile Placed");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_SUMMONER_INTRO_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Summoner Intro");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;
		case TutorialState::TUTORIAL_FINAL_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Tutorial Final");
			ALEngine::Engine::Scene::CutsceneManager::Instance()->SetJustTriggered();
			Time::m_Scale = 0.f;
			break;


			// ===== Scripted Gameplay =====
		default:
			Time::m_Scale = 1.f;
		}
	}

	b8 TutorialManager::TutorialIsPlaying(void) const
	{
		return m_TutorialIsPlaying;
	}

	void TutorialManager::SetTutorialIsPlaying(b8 playing)
	{
		m_TutorialIsPlaying = playing;
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
	
	b8 TutorialManager::GetPlayerMoveFinished(void)
	{
		return m_PlayerMoveFinished;
	}
	
	void TutorialManager::SetPlayerMoveFinished(b8 moveOver)
	{
		m_PlayerMoveFinished = moveOver;
	}

	b8 TutorialManager::GetEndTurnPressed(void)
	{
		return m_EndTurnPressed;
	}
	
	void TutorialManager::SetEndTurnPressed(b8 isPressed)
	{
		m_EndTurnPressed = isPressed;
	}
	
	b8 TutorialManager::GetPlayerTurnStart(void)
	{
		return m_PlayerTurnStart;
	}
	
	void TutorialManager::SetPlayerTurnStart(b8 hasStarted)
	{
		m_PlayerTurnStart = hasStarted;
	}
	
	u16 TutorialManager::GetNumEnemiesKilled(void)
	{
		return m_EnemiesKilled;
	}
	
	void TutorialManager::IncrementNumberOfEnemiesKilled(void)
	{
		++m_EnemiesKilled;
	}
	
	void TutorialManager::SetAllAbilitiesOff(void)
	{
		for (u16 i{ 0 }; i < 6; ++i)
		{
			EventTrigger& et = Coordinator::Instance()->GetComponent<EventTrigger>(m_AbilityList[i]);
			Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_AbilityList[i]);

			et.isEnabled = false;
			spr.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
	}
	
	void TutorialManager::SetAllAbilitiesButHardDropOff(void)
	{
		if (m_AbilityList.empty())
			return;

		for (u16 i{ 1 }; i < 6; ++i)
		{
			EventTrigger& et = Coordinator::Instance()->GetComponent<EventTrigger>(m_AbilityList[i]);
			Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_AbilityList[i]);

			et.isEnabled = false;
			spr.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
		
	}

	void TutorialManager::SetAllAbilitiesButConstructTileOff(void)
	{
		if (m_AbilityList.empty())
			return;

		for (u16 i{ 0 }; i < 6; ++i)
		{
			if (i == 3)
				continue;

			EventTrigger& et = Coordinator::Instance()->GetComponent<EventTrigger>(m_AbilityList[i]);
			Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_AbilityList[i]);

			et.isEnabled = false;
			spr.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}

	}
}