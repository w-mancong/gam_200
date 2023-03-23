/*!
file:	TutorialManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	File that contains definitions for functions needed for the Tutorial

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

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
		if (m_CurrentState == TutorialState::TUTORIAL_END)
		{
			return;
		}

		// Begin next state
		switch (m_CurrentState)
		{
			// ===== Cutscenes =====
		case TutorialState::TUTORIAL_ACTION_PHASE_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Action Phase");
			break;
		case TutorialState::TUTORIAL_MELEE_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Melee Enemy Intro");
			break;
		case TutorialState::TUTORIAL_HARD_DROP_CS:
			ALEngine::Engine::Scene::CutsceneManager::Instance()->PlaySequence("Hard Drop Skill Intro");
			break;


			// ===== Scripted Gameplaye =====
		}
	}

	TutorialState TutorialManager::GetState(void)
	{
		return m_CurrentState;
	}

	b8 TutorialManager::TutorialIsPlaying(void)
	{
		return m_TutorialIsPlaying;
	}
}