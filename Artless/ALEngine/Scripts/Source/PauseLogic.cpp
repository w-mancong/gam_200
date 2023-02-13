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

		b8 paused{ false };
		Entity button_resume{ MAX_ENTITIES }, button_htp{ MAX_ENTITIES }, button_quit{ MAX_ENTITIES },	// used for displaying all btns on paused
			backdrop{ MAX_ENTITIES }, quit_yes{ MAX_ENTITIES }, quit_no{ MAX_ENTITIES },	// used for quit game btn
			htp{ MAX_ENTITIES }, htp_cross_btn{ MAX_ENTITIES },								// used for displaying how to play
			button_story{ MAX_ENTITIES }, story_bg{ MAX_ENTITIES }, 
			story_cross{ MAX_ENTITIES };													// used for displaying storyline
		Entity en_paused{ MAX_ENTITIES }, temp{ MAX_ENTITIES };								// main paused background
	}

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

	void WhenResumeHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			paused = !paused;
			Time::m_Scale = static_cast<f32>(!paused);
			SetActive(paused, en_paused);
		}
	}

	void WhenResumePointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenHtpHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(true, htp);
			SetActive(false, en_paused);
		}
	}

	void WhenHtpPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenQuitHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(true, backdrop);
		}
	}

	void WhenQuitPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenQuitYesPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenQuitYesHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
#if EDITOR
			if (Editor::ALEditor::Instance()->GetImGuiEnabled())
			{
				Time::m_Scale = 1.0f;

				Editor::ALEditor::Instance()->SetGameActive(false);
				Engine::ToggleApplicationMode();

				ECS::ExitGameplaySystem();
				Coordinator::Instance()->DestroyEntities();

				Engine::Scene::LoadState();
				Engine::GameStateManager::Next(Engine::GameState::Editor);
				Editor::ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
				ECS::ResetEventTriggerSystem();
			}
			else
				Engine::TerminateEngine();
#else
			Engine::TerminateEngine();
#endif
		}
	}

	void WhenQuitNoPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenQuitNoHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(false, backdrop);
			WhenQuitNoPointerExit(en);
		}
	}

	void WhenHtpCrossHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(false, htp);
			SetActive(true, en_paused);
		}
	}

	void WhenHtpCrossPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenStoryHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(false, en_paused);
			SetActive(true, story_bg);
		}
	}

	void WhenStoryPointerExit(Entity en)
	{
		Lighten(en);
	}

	void WhenStoryCrossHover(Entity en)
	{
		Darken(en);
		if (Input::KeyDown(KeyCode::MouseLeftButton))
		{
			Lighten(en);
			SetActive(true, en_paused);
			SetActive(false, story_bg);
		}
	}

	void WhenStoryCrossPointerExit(Entity en)
	{
		Lighten(en);
	}

	void PauseLogic::Init([[maybe_unused]] ECS::Entity en)
	{
		en_paused = Coordinator::Instance()->GetEntityByTag("pause_menu");

		//en_pause = Coordinator::Instance()->GetEntityByTag("pause_menu");
		button_resume = Coordinator::Instance()->GetEntityByTag("button_resume");
		button_htp = Coordinator::Instance()->GetEntityByTag("button_htp");
		button_quit = Coordinator::Instance()->GetEntityByTag("button_quit");

		CreateEventTrigger(button_resume, true);
		CreateEventTrigger(button_htp, true);
		CreateEventTrigger(button_quit, true);

		Subscribe(button_resume, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenResumeHover);
		Subscribe(button_resume, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenResumePointerExit);

		Subscribe(button_htp, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpHover);
		Subscribe(button_htp, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpPointerExit);

		Subscribe(button_quit, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitHover);
		Subscribe(button_quit, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitPointerExit);

		//*********************************************************************************************************
		backdrop = Coordinator::Instance()->GetEntityByTag("backdrop_cfm");
		quit_yes = Coordinator::Instance()->GetEntityByTag("button_quit_yes");
		quit_no = Coordinator::Instance()->GetEntityByTag("button_quit_no");

		CreateEventTrigger(quit_yes, true);
		CreateEventTrigger(quit_no, true);

		Subscribe(quit_yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitYesHover);
		Subscribe(quit_yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitYesPointerExit);

		Subscribe(quit_no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitNoHover);
		Subscribe(quit_no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitNoPointerExit);

		//*********************************************************************************************************
		htp = Coordinator::Instance()->GetEntityByTag("htp");
		htp_cross_btn = Coordinator::Instance()->GetEntityByTag("htp_cross_btn");

		CreateEventTrigger(htp_cross_btn, true);
		Subscribe(htp_cross_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpCrossHover);
		Subscribe(htp_cross_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpCrossPointerExit);

		//*********************************************************************************************************
		story_bg = Coordinator::Instance()->Instance()->GetEntityByTag("story_bg");
		story_cross = Coordinator::Instance()->GetEntityByTag("story_cross");
		button_story = Coordinator::Instance()->GetEntityByTag("button_story");

		CreateEventTrigger(button_story, true);
		CreateEventTrigger(story_cross, true);

		Subscribe(button_story, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenStoryHover);
		Subscribe(button_story, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenStoryPointerExit);

		Subscribe(story_cross, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenStoryCrossHover);
		Subscribe(story_cross, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenStoryCrossPointerExit);
	}

	void PauseLogic::Update([[maybe_unused]] ECS::Entity en)
	{
#if EDITOR
		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
#endif
		if (Coordinator::Instance()->GetComponent<EntityData>(htp).active) {
			return;
		}

		if (Input::KeyTriggered(KeyCode::Escape))
		{
			if (temp >= MAX_ENTITIES)
			{
				Transform xform = Transform{ Math::Vector2(0.f, 0.f),
				Math::Vector2(50.f, 50.f) };

				// Create Entity
				temp = Coordinator::Instance()->CreateEntity();
				ECS::CreateSprite(temp, xform);

				ECS::GetSceneGraph().Push(-1, temp);

				EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(temp);
				ed.tag = "hotfix"; SetActive(false, temp);
			}

			if (!Coordinator::Instance()->GetComponent<EntityData>(backdrop).active)
			{
				paused = !paused;
				Time::m_Scale = static_cast<f32>(!paused);
				SetActive(paused, en_paused);
			}
			else
			{
				SetActive(false, backdrop);
			}

			Lighten(button_resume), Lighten(button_htp), Lighten(button_quit);
			Lighten(backdrop), Lighten(quit_yes), Lighten(quit_no);
			Lighten(htp), Lighten(htp_cross_btn);
			Lighten(en_paused);
		}
	}

	void PauseLogic::Free([[maybe_unused]] ECS::Entity en)
	{
		button_resume = button_htp = button_quit = MAX_ENTITIES;
		backdrop = quit_yes = quit_no = MAX_ENTITIES;
		temp = MAX_ENTITIES;
		paused = false;
	}
}