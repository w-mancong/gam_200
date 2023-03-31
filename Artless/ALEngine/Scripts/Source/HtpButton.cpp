/*!
file:	HtpButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for a how to play button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <HtpButton.h>
#include <PauseButtonFlag.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity htp{ ECS::MAX_ENTITIES }, htp_pages{ ECS::MAX_ENTITIES }, next{ ECS::MAX_ENTITIES }, 
			   prev{ ECS::MAX_ENTITIES }, close{ ECS::MAX_ENTITIES }, parent{ ECS::MAX_ENTITIES };
		f32 constexpr ALPHA_VALUE{ 0.925f };
		u64 index = 0;
		u64 const constexpr MIN_INDEX{ 0 }, MAX_INDEX{ 6 };
		Guid ids[MAX_INDEX]{};
		b8 isButtonClicked{ false };

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

		void WhenHtpHover(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a || PauseButtonFlag::confirmationBG)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(false, parent);
				SetActive(true, htp);

				Lighten(en);
				PauseButtonFlag::confirmationBG = true;
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenHtpExit(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Lighten(en);
		}

		void WhenCloseHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(true, parent);
				SetActive(false, htp);

				Lighten(en);
				PauseButtonFlag::confirmationBG = false;
				GameAudioManager::Play("MenuButtonPress");
				index = MIN_INDEX;
				Coordinator::Instance()->GetComponent<Sprite>(htp_pages).id = ids[index];
			}
		}

		void WhenCloseExit(Entity en)
		{
			Lighten(en);
		}

		void WhenNextHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton) && !isButtonClicked)
			{
				(++index) %= MAX_INDEX;
				Coordinator::Instance()->GetComponent<Sprite>(htp_pages).id = ids[index];
				isButtonClicked = true;
				GameAudioManager::Play("MenuButtonPress");
			}
			if (Input::KeyReleased(KeyCode::MouseLeftButton))
				isButtonClicked = false;
		}

		void WhenNextExit(Entity en)
		{
			Lighten(en);
			isButtonClicked = false;
		}

		void WhenPrevHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton) && !isButtonClicked)
			{
				index = std::clamp(--index, MIN_INDEX, MAX_INDEX - 1);
				Coordinator::Instance()->GetComponent<Sprite>(htp_pages).id = ids[index];
				isButtonClicked = true;
				GameAudioManager::Play("MenuButtonPress");
			}
			if (Input::KeyReleased(KeyCode::MouseLeftButton))
				isButtonClicked = false;
		}

		void WhenPrevExit(Entity en)
		{
			Lighten(en);
			isButtonClicked = false;
		}
	}

	void HtpButton::Load(ECS::Entity en)
	{
		/*
			pg 1 - Objective&Controls
			pg 2 - Phases
			pg 3 - Movement
			pg 4 - Combat
			pg 5 - CombatSkills
			pg 6 - UtilitySkills.
		*/
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_Objective&Controls.png");
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_Phases.png");
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_Movement.png");
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_Combat.png");
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_CombatSkills.png");
		ids[index++] = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HowToPlay_UtilitySkills.png");
	}

	void HtpButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpExit);

		parent = static_cast<Entity>(GetSceneGraph().GetParent(en));

		{
			Entity grandparent = static_cast<Entity>(GetSceneGraph().GetParent(parent));

			GetSceneGraph().FindImmediateChildren(static_cast<Entity>(grandparent));
			std::vector<s32> const& children = GetSceneGraph().GetChildren();

			for (s32 child : children)
			{
				EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
				if (ed.tag == "htp")
					htp = static_cast<Entity>(child);
			}
		}

		{
			GetSceneGraph().FindImmediateChildren(static_cast<Entity>(htp));
			std::vector<s32> const& children = GetSceneGraph().GetChildren();

			for (s32 child : children)
			{
				EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
				if (ed.tag == "htp_pages")
					htp_pages = static_cast<Entity>(child);
				else if(ed.tag == "next")
					next = static_cast<Entity>(child);
				else if (ed.tag == "prev")
					prev = static_cast<Entity>(child);
				else if (ed.tag == "close")
					close = static_cast<Entity>(child);
			}

			CreateEventTrigger(next, true);
			Subscribe(next, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenNextHover);
			Subscribe(next, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenNextExit);

			CreateEventTrigger(prev, true);
			Subscribe(prev, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenPrevHover);
			Subscribe(prev, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenPrevExit);

			CreateEventTrigger(close, true);
			Subscribe(close, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenCloseHover);
			Subscribe(close, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenCloseExit);
		}

		index = 0;
	}

	void HtpButton::Free(ECS::Entity)
	{
		PauseButtonFlag::confirmationBG = false;		
	}

	void HtpButton::Unload(ECS::Entity)
	{
		memset(ids, 0, sizeof(ids));
	}
}