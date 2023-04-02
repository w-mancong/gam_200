/*!
file:	PauseButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <PauseButton.h>
#include <PauseButtonFlag.h>
#include <Engine/GSM/GameStateManager.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity bd_pause{ ECS::MAX_ENTITIES };
		b8 clicked{ false };

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
			if (PauseButtonFlag::confirmationBG || Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				if (clicked)
					return;

				b8 active = Coordinator::Instance()->GetComponent<EntityData>(bd_pause).active;
				SetActive(!active, bd_pause);
				Time::m_Scale = static_cast<f32>(active);
				clicked = true;

				if (!active)
				{
					ParticleSystem::GetParticleSystem().pauseRender = true;
					GameAudioManager::Play("MenuOpen");
				}
				else
				{
					ParticleSystem::GetParticleSystem().pauseRender = false;
					GameAudioManager::Play("MenuClose");
				}
			}
			if (Input::KeyReleased(KeyCode::MouseLeftButton))
				clicked = false;
		}

		void WhenExit(Entity en)
		{
			Lighten(en);
			if(Input::KeyReleased(KeyCode::MouseLeftButton))
				clicked = false;
		}
	}

	void PauseButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenExit);

		bd_pause = Coordinator::Instance()->GetEntityByTag("bd_pause");
	}

	void PauseButton::Free(ECS::Entity en)
	{
		bd_pause = ECS::MAX_ENTITIES;
		clicked = false;
	}
}