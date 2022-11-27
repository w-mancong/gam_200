/*!
file:	PauseLogic.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/PauseLogic.h>

namespace ALEngine
{
	namespace
	{
		using namespace ECS;

		b8 paused{ false }, pause_found{ false };
		Entity button_resume{ MAX_ENTITIES }, button_htp{ MAX_ENTITIES }, button_quit{ MAX_ENTITIES },	// used for displaying all btns on paused
			backdrop{ MAX_ENTITIES }, quit_yes{ MAX_ENTITIES }, quit_no{ MAX_ENTITIES },	// used for quit game btn
			htp{ MAX_ENTITIES }, htp_cross_btn{ MAX_ENTITIES };								// used for how to play
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
			Engine::TerminateEngine();
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

	void PauseInit(Entity en)
	{
		if (!pause_found)
		{
			en_paused = en;

			//en_pause = Coordinator::Instance()->GetEntityByTag("pause_menu");
			button_resume = Coordinator::Instance()->GetEntityByTag("button_resume");
			button_htp = Coordinator::Instance()->GetEntityByTag("button_htp");
			button_quit = Coordinator::Instance()->GetEntityByTag("button_quit");

			CreateEventTrigger(button_resume);
			CreateEventTrigger(button_htp);
			CreateEventTrigger(button_quit);

			Subscribe(button_resume, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenResumeHover);
			Subscribe(button_resume, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenResumePointerExit);

			Subscribe(button_htp, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpHover);
			Subscribe(button_htp, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpPointerExit);

			Subscribe(button_quit, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitHover);
			Subscribe(button_quit, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitPointerExit);

			//*********************************************************************************************************
			backdrop = Coordinator::Instance()->GetEntityByTag("backdrop_cfm");
			quit_yes = Coordinator::Instance()->GetEntityByTag("button_quit_yes");
			quit_no  = Coordinator::Instance()->GetEntityByTag("button_quit_no");

			CreateEventTrigger(quit_yes);
			CreateEventTrigger(quit_no);

			Subscribe(quit_yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitYesHover);
			Subscribe(quit_yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitYesPointerExit);

			Subscribe(quit_no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitNoHover);
			Subscribe(quit_no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitNoPointerExit);

			//*********************************************************************************************************
			htp			  = Coordinator::Instance()->GetEntityByTag("htp");
			htp_cross_btn = Coordinator::Instance()->GetEntityByTag("htp_cross_btn");

			CreateEventTrigger(htp_cross_btn);
			Subscribe(htp_cross_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpCrossHover);
			Subscribe(htp_cross_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpCrossPointerExit);
		}
	}

	void PauseUpdate(Entity en)
	{
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
				SetActive(paused, en);
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

	void PauseReset([[maybe_unused]] Entity en)
	{
		button_resume = button_htp = button_quit = MAX_ENTITIES;
		backdrop = quit_yes = quit_no = MAX_ENTITIES;
		temp = MAX_ENTITIES;
		pause_found = paused = false;
	}
}