/*!
file:	PauseLogic.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <PauseLogic.h>
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity bd_pause{ ECS::MAX_ENTITIES };
	}

	void PauseLogic::Init(ECS::Entity en)
	{
		GetSceneGraph().FindImmediateChildren(en);
		bd_pause = GetSceneGraph().GetChildren()[0];
		paused	 = false;
		SetActive(paused, bd_pause);	// to make sure that bd_pause is always false when init
	}

	void PauseLogic::Update(ECS::Entity en)
	{
		if (Input::KeyTriggered(KeyCode::Escape))
		{
			paused = !paused;
			SetActive(paused, bd_pause);
			Time::m_Scale = static_cast<f32>(paused);
		}
	}

	void PauseLogic::Free(ECS::Entity en)
	{
		bd_pause = ECS::MAX_ENTITIES;
	}
}