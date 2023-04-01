/*!
file:	SplashScreen.cpp
author: Chan Jie Ming Stanley
email:	c.jiemingstanley\@digipen.edu
brief:	This file contain function definition for Splash Screen

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include <pch.h>
#include "SplashScreen.h"


namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void SplashScreen::Init(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("Splash Screen");

		ECS::CameraPosition(0.0f, 0.0f);
	}

	void SplashScreen::Update(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Update();

		if (!Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
		{
			Engine::Scene::NextScene();
		}

	}

}
