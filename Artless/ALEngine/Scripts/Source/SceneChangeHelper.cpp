/*!
file:	SceneChangeHelper.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a helper class to change scene

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <SceneChangeHelper.h>
#include <SceneManager/SceneManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		f32 constexpr ALPHA_SPEED{ 0.75f };
	}
	b8 SceneChangeHelper::changingScene{false};
	f32 SceneChangeHelper::alphaValue{};

	void SceneChangeHelper::Init(ECS::Entity en)
	{
		changeScene = false;
		sceneName = "";
		sceneIndex = -1;
		sprite = &Coordinator::Instance()->GetComponent<Sprite>(en);
		sprite->color.a = 0.0f;
		changingScene = false;
	}

	void SceneChangeHelper::Update([[maybe_unused]] ECS::Entity en)
	{
		if (!changeScene)
			return;
		UpdateAlpha();
		if (sprite->color.a < 1.0f)
			return;
		if (restart)
			Engine::Scene::Restart();
		else if (!sceneName.empty() && sceneIndex == -1)
			Engine::Scene::NextScene(sceneName);
		else if (sceneName.empty() && sceneIndex != -1)
			Engine::Scene::NextScene( static_cast<u64>(sceneIndex) );
		else
			Engine::Scene::NextScene();
		Engine::StopChannel(Engine::Channel::Master);
	}

	void SceneChangeHelper::NextScene(std::string const& _sceneName)
	{
		changeScene = true;
		changingScene = true;
		sceneName = _sceneName;
	}

	void SceneChangeHelper::NextScene(u64 _sceneIndex)
	{
		changeScene = true;
		changingScene = true;
		sceneIndex = static_cast<s64>(_sceneIndex);
	}

	void SceneChangeHelper::NextScene(void)
	{
		changeScene = true;
		changingScene = true;
	}

	void SceneChangeHelper::Restart(void)
	{
		changeScene = true;
		restart = true;
		changingScene = true;
	}

	void SceneChangeHelper::UpdateAlpha(void)
	{
		sprite->color.a += Time::m_ActualDeltaTime * ALPHA_SPEED;
		alphaValue = sprite->color.a;
	}
}