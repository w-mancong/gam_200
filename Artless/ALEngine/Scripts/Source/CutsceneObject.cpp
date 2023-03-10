/*!
file:	CutsceneObject.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for fading in alpha of entities

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
	}

	void CutsceneObject::Update(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Update();
	}
}