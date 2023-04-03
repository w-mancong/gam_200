/*!
file:	CutsceneObject.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for the cutscene object.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <CutsceneObject.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		static b8 cutScenePlayedOnce{ false };
	}

	void CutsceneObject::Init(ECS::Entity en)
	{
		main_menu = Coordinator::Instance()->GetEntityByTag("main_menu");

		if (cutScenePlayedOnce)
		{
			SetActive(false, en);
			SetActive(true, main_menu);
			return;
		}
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("Opening Sequence");

		ECS::CameraPosition(0.0f, 0.0f);
	}

	void CutsceneObject::Update(ECS::Entity en)
	{
#if !EDITOR
		if (cutScenePlayedOnce)
			return;
#endif
		Engine::Scene::CutsceneManager::Instance()->Update();

#if !EDITOR
		if (!Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
		{
			SetActive(true, main_menu);
			cutScenePlayedOnce = true;
		}

		if (Input::KeyTriggered(KeyCode::Escape))
		{
			Engine::Scene::CutsceneManager::Instance()->StopSequence();
			cutScenePlayedOnce = true;
			SetActive(true, main_menu);
		}
#endif
	}
}