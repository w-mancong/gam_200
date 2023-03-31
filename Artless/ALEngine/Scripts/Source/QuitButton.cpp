/*!
file:	QuitButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for quit game button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <QuitButton.h>
#include <PauseButtonFlag.h>
#include <GameAudioManager.h>
#if EDITOR
#include <Engine/GSM/GameStateManager.h>
#include <GameplaySystem.h>
#endif

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		//Entity htp{ MAX_ENTITIES }, close{ MAX_ENTITIES }, parent{ MAX_ENTITIES };
		Entity quit_confirmation{ MAX_ENTITIES }, yes{ MAX_ENTITIES }, no{ MAX_ENTITIES };
		f32 constexpr ALPHA_VALUE{ 0.925f };

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

		void WhenQuitHover(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a || PauseButtonFlag::confirmationBG)
				return;
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(true, quit_confirmation);
				Lighten(en);
				PauseButtonFlag::confirmationBG = true;
				GameAudioManager::Play("MenuButtonPress");
			}
			Darken(en);
		}

		void WhenQuitExit(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Lighten(en);
		}

		void WhenYesHover(Entity en)
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
					Engine::GameStateManager::Next(Engine::GameState::Editor);
					Editor::ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
					Engine::StopChannel(Engine::Channel::Master);
					ECS::ResetEventTriggerSystem();
					SetMap(0);
				}
				else
					Engine::TerminateEngine();
#else
				Engine::TerminateEngine();
#endif
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenYesExit(Entity en)
		{
			Lighten(en);
		}

		void WhenNoHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(false, quit_confirmation);
				Lighten(en);
				PauseButtonFlag::confirmationBG = false;
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenNoExit(Entity en)
		{
			Lighten(en);
		}
	}

	void QuitButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenQuitHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenQuitExit);

		// Quit button will be attached to btn_quit
		Entity parent = static_cast<Entity>( GetSceneGraph().GetParent(en) );
		Entity grandparent = static_cast<Entity>( GetSceneGraph().GetParent(parent) );
		GetSceneGraph().FindImmediateChildren(grandparent);
		std::vector<s32> const& children1{ GetSceneGraph().GetChildren() };
		for (s32 child : children1)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "quit_confirmation")
				quit_confirmation = static_cast<Entity>(child);
		}

		GetSceneGraph().FindImmediateChildren(quit_confirmation);
		std::vector<s32> const& children2{ GetSceneGraph().GetChildren() };
		for (s32 child : children2)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "yes")
				yes = static_cast<Entity>(child);
			else if (ed.tag == "no")
				no = static_cast<Entity>(child);
		}

		CreateEventTrigger(yes, true);
		Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenYesHover);
		Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenYesExit);

		CreateEventTrigger(no, true);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenNoHover);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenNoExit);
	}

	void QuitButton::Free(ECS::Entity en)
	{
		quit_confirmation = yes = no = MAX_ENTITIES;
		PauseButtonFlag::confirmationBG = false;
	}
}