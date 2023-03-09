/*!
file:	OptionButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for option button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <OptionButton.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		ECS::Entity sound_options{ ECS::MAX_ENTITIES },
			master_right{ ECS::MAX_ENTITIES }, master_left{ ECS::MAX_ENTITIES }, master_text{ ECS::MAX_ENTITIES },
			sfx_right{ ECS::MAX_ENTITIES }, sfx_left{ ECS::MAX_ENTITIES }, sfx_text{ ECS::MAX_ENTITIES },
			bgm_right{ ECS::MAX_ENTITIES }, bgm_left{ ECS::MAX_ENTITIES }, bgm_text{ ECS::MAX_ENTITIES };

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

		}

		void WhenOptionExit(Entity en)
		{

		}
	}

	void OptionButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenOptionHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenOptionExit);

		// option button will be attached to btn_options
		Entity parent = static_cast<Entity>(GetSceneGraph().GetParent(en));
		Entity grandparent = static_cast<Entity>(GetSceneGraph().GetParent(parent));
		GetSceneGraph().FindImmediateChildren(grandparent);
		std::vector<s32> const& children1{ GetSceneGraph().GetChildren() };
	}

	void OptionButton::Free(ECS::Entity en)
	{
		sound_options = 
		master_right  = master_left = master_text = 
		sfx_right     = sfx_left    = sfx_text    =
		bgm_right     = bgm_left    = bgm_text    = ECS::MAX_ENTITIES;
	}
}