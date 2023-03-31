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
	}

	void CutsceneObject::Init(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("Opening Sequence");
		
		Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Audio\\Cutscene_MainMenu_BGM.wav");
		Engine::Audio ad = Engine::AssetManager::Instance()->GetAudio(id);
		ad.m_Volume = 0.4f;
		ad.m_Channel = Engine::Channel::BGM;
		ad.m_Loop = true;
		ad.Play();

		ECS::CameraPosition(0.0f, 0.0f);
	}

	void CutsceneObject::Update(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Update();

		if (!Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
			Engine::Scene::NextScene();

		if (Input::KeyTriggered(KeyCode::Escape))
		{
			Engine::Scene::CutsceneManager::Instance()->StopSequence();
			Engine::Scene::NextScene();
		}
	}
}