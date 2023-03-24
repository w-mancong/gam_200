/*!
file:	OptionButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for option button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <OptionButton.h>
#include <PauseButtonFlag.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		f32 constexpr ALPHA_VALUE{ 0.925f };
		ECS::Entity sound_options{ ECS::MAX_ENTITIES }, close_btn{ ECS::MAX_ENTITIES }, parent{ ECS::MAX_ENTITIES };

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

		void WhenOptionHover(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a || PauseButtonFlag::confirmationBG)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(false, parent);
				SetActive(true, sound_options);
				Lighten(en);
			}
		}

		void WhenOptionExit(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Lighten(en);
		}

		void WhenCloseBtnHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(true, parent);
				SetActive(false, sound_options);
				Lighten(en);
			}
		}

		void WhenCloseBtnExit(Entity en)
		{
			Lighten(en);
		}
	}

	void OptionButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenOptionHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenOptionExit);

		// option button will be attached to btn_options
		parent = static_cast<Entity>(GetSceneGraph().GetParent(en));
		Entity grandparent = static_cast<Entity>(GetSceneGraph().GetParent(parent));
		GetSceneGraph().FindImmediateChildren(grandparent);
		// Find sound_options
		std::vector<s32> const& children1{ GetSceneGraph().GetChildren() };

		for (s32 child : children1)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "sound_options")
				sound_options = static_cast<Entity>(child);
		}
	
		GetSceneGraph().FindImmediateChildren(sound_options);
		// Find master_bd, sfx_bd, bgm_bd
		std::vector<s32> const& children2{ GetSceneGraph().GetChildren() };

		for (s32 child : children2)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "close_btn")
				close_btn = static_cast<Entity>(child);
		}

		CreateEventTrigger(close_btn, true);
		Subscribe(close_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenCloseBtnHover);
		Subscribe(close_btn, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenCloseBtnExit);
	}

	void OptionButton::Free([[maybe_unused]] ECS::Entity en)
	{
		sound_options = close_btn = parent = ECS::MAX_ENTITIES;
		PauseButtonFlag::confirmationBG = false;
	}
}