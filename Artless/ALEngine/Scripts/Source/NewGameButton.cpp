/*!
file:	NewGameButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for new game button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <NewGameButton.h>
#include <SceneChangeHelper.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		f32 constexpr ALPHA_VALUE{ 0.925f };
		ECS::Entity scene_transition{ ECS::MAX_ENTITIES };

		void Darken(Entity en)
		{
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			sprite.color = { 0.6f, 0.6f, 0.6f, 1.0f };
		}

		void Lighten(Entity en)
		{
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		void WhenHover(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->NextScene();
			}
		}

		void WhenExit(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Lighten(en);
		}
	}

	void NewGameButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenExit);

		scene_transition = Coordinator::Instance()->GetEntityByTag("scene_transition");
	}

	void NewGameButton::Free(ECS::Entity en)
	{
		scene_transition = ECS::MAX_ENTITIES;
	}
}