/*!
file:	WinCondition.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for checking win/lose condition

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <WinCondition.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem.h>
#include <SceneChangeHelper.h>
#include <PauseButtonFlag.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity win{ ECS::MAX_ENTITIES }, main_menu{ ECS::MAX_ENTITIES },
			   lose{ ECS::MAX_ENTITIES }, yes{ ECS::MAX_ENTITIES }, no{ ECS::MAX_ENTITIES },
			scene_transition{ ECS::MAX_ENTITIES };

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

		void WhenMenuHover(Entity en)
		{
			if (clicked)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				clicked = true;
				Lighten(en);
				SetMap(0);
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->NextScene("Assets\\Scene\\main_menu.scene");
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenMenuExit(Entity en)
		{
			Lighten(en);
		}

		void WhenYesHover(Entity en)
		{
			if (clicked)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				clicked = true;
				Lighten(en);
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->Restart();
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenYesExit(Entity en)
		{
			Lighten(en);
		}

		void WhenNoHover(Entity en)
		{
			if (clicked)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				clicked = true;
				Lighten(en);
				SetMap(0);
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->NextScene("Assets\\Scene\\main_menu.scene");
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenNoExit(Entity en)
		{
			Lighten(en);
		}
	}

	void WinCondition::Init(ECS::Entity en)
	{
		GetSceneGraph().FindChildren(static_cast<s32>(en));
		std::vector<s32> const& children = GetSceneGraph().GetChildren();

		for (s32 child : children)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "win")
				win = static_cast<Entity>(child);
			else if (ed.tag == "main_menu")
				main_menu = static_cast<Entity>(child);
			else if (ed.tag == "lose")
				lose = static_cast<Entity>(child);
			else if (ed.tag == "yes")
				yes = static_cast<Entity>(child);
			else if (ed.tag == "no")
				no = static_cast<Entity>(child);
		}

		win_sprite  = &Coordinator::Instance()->GetComponent<Sprite>(win);
		lose_sprite = &Coordinator::Instance()->GetComponent<Sprite>(lose);

		CreateEventTrigger(main_menu, true);
		Subscribe(main_menu, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenMenuHover);
		Subscribe(main_menu, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenMenuExit);
		Coordinator::Instance()->GetComponent<EventTrigger>(main_menu).layer = 100;

		CreateEventTrigger(yes, true);
		Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenYesHover);
		Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenYesExit);
		Coordinator::Instance()->GetComponent<EventTrigger>(yes).layer = 100;

		CreateEventTrigger(no, true);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenNoHover);
		Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenNoExit);
		Coordinator::Instance()->GetComponent<EventTrigger>(no).layer = 100;

		scene_transition = Coordinator::Instance()->GetEntityByTag("scene_transition");
		sceneChanging	 = false;
		clicked			 = false;
		Font::EnableTextRendering(true);
	}

	void WinCondition::Update([[maybe_unused]] ECS::Entity)
	{
		if (sceneChanging)
			return;
		if (GameplaySystem::currentGameStatus == GAME_STATUS::WIN)
		{
			Font::EnableTextRendering(false);
			PauseButtonFlag::confirmationBG = true;
			if (GameplaySystem::roomIndex + 1 < GameplaySystem::maxRooms)
			{
				std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
				ptr->Restart();
				sceneChanging = true;
				++GameplaySystem::roomIndex;
			}
			else
			{
				win_sprite->color.a += Time::m_ActualDeltaTime * SPEED;
				if (win_sprite->color.a < 1.0f)
					return;
				sceneChanging = true;
				SetActive(true, main_menu);
			}
		}
		else if (GameplaySystem::currentGameStatus == GAME_STATUS::LOSE)
		{
			Font::EnableTextRendering(false);
			PauseButtonFlag::confirmationBG = true;
			lose_sprite->color.a += Time::m_ActualDeltaTime * SPEED;
			if (lose_sprite->color.a < 1.0f)
				return;
			sceneChanging = true;
			SetActive(true, yes);
			SetActive(true, no);
		}
	}

	void WinCondition::Free(ECS::Entity)
	{
		win = main_menu = lose = yes = no = scene_transition = ECS::MAX_ENTITIES;
		clicked = false;
		PauseButtonFlag::confirmationBG = false;
	}
}