/*!
file:	CutsceneObjectTutorial.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definitions for the cutscene object for the tutorial.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <CutsceneObjectTutorial.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void CutsceneObjectTutorial::Init(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("Tutorial Intro");
		Time::m_Scale = 0.f;

		Gameplay::TutorialManager::Instance()->BeginTutorial();
	}

	void CutsceneObjectTutorial::Update([[maybe_unused]]ECS::Entity en)
	{
		using namespace Gameplay;
		Engine::Scene::CutsceneManager::Instance()->Update();

		if (Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying() == false)
			TutorialManager::Instance()->NextState();
	}

}