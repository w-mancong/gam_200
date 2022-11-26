#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/PauseLogic.h>
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine
{
	namespace
	{
		using namespace ECS;

		b8 paused{ false }, pause_found{ false };
		Entity button_resume{ MAX_ENTITIES }, button_htp{ MAX_ENTITIES }, button_quit{ MAX_ENTITIES }, 
			backdrop{ MAX_ENTITIES }, quit_yes{ MAX_ENTITIES }, quit_no{ MAX_ENTITIES };
		Entity en_paused{ MAX_ENTITIES }, temp{ MAX_ENTITIES };
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
			SetActive(true, backdrop);
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
				Editor::ALEditor::Instance()->SetGameActive(false);
				Engine::ToggleApplicationMode();

				Coordinator::Instance()->DestroyEntities();
				ECS::ExitGameplaySystem();

				Engine::Scene::LoadState();
				Engine::GameStateManager::Next(Engine::GameState::Editor);
				Editor::ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
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
			SetActive(false, backdrop);
			WhenQuitNoPointerExit(en);
		}
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
		}
	}

	void PauseUpdate(Entity en)
	{
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
				SetActive(paused, en);
			}
			else
			{
				Lighten(backdrop);
				SetActive(false, backdrop);
			}
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