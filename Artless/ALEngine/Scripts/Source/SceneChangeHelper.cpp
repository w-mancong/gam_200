/*!
file:	SceneChangeHelper.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a helper class to change scene

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <SceneChangeHelper.h>
#include <SceneManager/SceneManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		f32 constexpr ALPHA_SPEED{ 1.15f };
	}

	void SceneChangeHelper::Init(ECS::Entity en)
	{
		changeScene = false;
		sceneName = "";
		sceneIndex = -1;
		sprite = &Coordinator::Instance()->GetComponent<Sprite>(en);
		sprite->color.a = 0.0f;
	}

	void SceneChangeHelper::Update([[maybe_unused]] ECS::Entity en)
	{
		if (!changeScene)
			return;
		sprite->color.a += Time::m_DeltaTime * ALPHA_SPEED;
		if (sprite->color.a < 1.0f)
			return;
		if (!sceneName.empty() && sceneIndex == -1)
			Engine::Scene::NextScene(sceneName);
		else if (sceneName.empty() && sceneIndex != -1)
			Engine::Scene::NextScene( static_cast<u64>(sceneIndex) );
		else
			Engine::Scene::NextScene();
	}

	void SceneChangeHelper::ChangeScene(std::string const& sceneName)
	{
		changeScene = true;
		this->sceneName = sceneName;
	}

	void SceneChangeHelper::ChangeScene(u64 sceneIndex)
	{
		changeScene = true;
		this->sceneIndex = static_cast<s64>(sceneIndex);
	}

	void SceneChangeHelper::NextScene(void)
	{
		changeScene = true;
	}
}