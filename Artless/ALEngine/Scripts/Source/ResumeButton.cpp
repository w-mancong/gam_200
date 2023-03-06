/*!
file:	ResumeButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a resume button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <ResumeButton.h>
#include <PauseLogic.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity bd_pause{ ECS::MAX_ENTITIES }, bd_pause_parent{ ECS::MAX_ENTITIES };

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
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				std::shared_ptr<PauseLogic> pl = GetLogicComponent<PauseLogic>(bd_pause_parent);
				pl->paused = false;
				SetActive(false, bd_pause);
				Time::m_Scale = 1.0f;
				Lighten(en);
			}
		}

		void WhenExit(Entity en)
		{
			Lighten(en);
		}
	}

	void ResumeButton::Init(ECS::Entity en)
	{
		bd_pause = static_cast<Entity>( GetSceneGraph().GetParent(en) );
		bd_pause_parent = static_cast<Entity>( GetSceneGraph().GetParent(bd_pause) );

		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenExit);
	}

	void ResumeButton::Free(ECS::Entity en)
	{
		bd_pause = bd_pause_parent = MAX_ENTITIES;
	}
}