/*!
file:	ResumeButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for pause menu's resume button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <ResumeButton.h>
#include <PauseLogic.h>
#include <PauseButtonFlag.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity bd_pause{ ECS::MAX_ENTITIES };

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
			if (PauseButtonFlag::confirmationBG)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(false, bd_pause);
				Time::m_Scale = 1.0f;
				Lighten(en);
				GameAudioManager::Play("MenuButtonPress");
				ParticleSystem::GetParticleSystem().pauseRender = false;
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

		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenExit);
	}

	void ResumeButton::Free(ECS::Entity en)
	{
		bd_pause = MAX_ENTITIES;
		PauseButtonFlag::confirmationBG = false;
	}
}