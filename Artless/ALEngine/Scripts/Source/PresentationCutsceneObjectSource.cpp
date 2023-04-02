/*!
file:	PresentationCutsceneObject.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for the presentation cutscene object.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <PresentationCutsceneObject.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void PresentationCutsceneObject::Init(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("01_Presentation");

		ECS::CameraPosition(0.0f, 0.0f);
	}

	void PresentationCutsceneObject::Update(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Update();
	}
}