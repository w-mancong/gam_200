/*!
file:	MainMenuButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for main menu button when paused

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <MainMenuButton.h>
#include <SceneChangeHelper.h>
#include <GameplaySystem.h>
#include <PauseButtonFlag.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity menu_confirmation{ MAX_ENTITIES }, yes{ MAX_ENTITIES }, no{ MAX_ENTITIES }, scene_transition{ MAX_ENTITIES };
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

		void WhenMenuHover(Entity en)
		{
			if (PauseButtonFlag::confirmationBG)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(true, menu_confirmation);
				Lighten(en);
				PauseButtonFlag::confirmationBG = true;
			}
		}

		void WhenMenuExit(Entity en)
		{
			Lighten(en);
		}

		void WhenYesHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->NextScene("Assets\\Scene\\main_menu.scene");
				SetMap(0);
				Time::m_Scale = 1.0f;
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
				SetActive(false, menu_confirmation);
				Lighten(en);
				PauseButtonFlag::confirmationBG = false;
			}
		}

		void WhenNoExit(Entity en)
		{
			Lighten(en);
		}
	}

	void MainMenuButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenMenuHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenMenuExit);

		// Quit button will be attached to btn_quit
		Entity parent = static_cast<Entity>(GetSceneGraph().GetParent(en));
		Entity grandparent = static_cast<Entity>(GetSceneGraph().GetParent(parent));
		GetSceneGraph().FindImmediateChildren(grandparent);
		std::vector<s32> const& children1{ GetSceneGraph().GetChildren() };
		for (s32 child : children1)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "menu_confirmation")
				menu_confirmation = static_cast<Entity>(child);
			else if (ed.tag == "scene_transition")
				scene_transition = static_cast<Entity>(child);
		}

		GetSceneGraph().FindImmediateChildren(menu_confirmation);
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

		Coordinator::Instance()->GetComponent<EventTrigger>(yes).layer = 50;

		CreateEventTrigger(no, true);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenNoHover);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenNoExit);

		Coordinator::Instance()->GetComponent<EventTrigger>(no).layer = 50;
	}

	void MainMenuButton::Free(ECS::Entity en)
	{
		menu_confirmation = yes = no = scene_transition = MAX_ENTITIES;
		PauseButtonFlag::confirmationBG = false;
	}
}