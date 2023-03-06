/*!
file:	PauseLogic.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <HtpButton.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity htp{ MAX_ENTITIES }, close{ MAX_ENTITIES }, parent{ MAX_ENTITIES };

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
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				// Entities to be set to true
				SetActive(true, htp);
				SetActive(true, close);

				// Entities to be set to false
				SetActive(false, parent);
				Lighten(en);
			}
		}

		void WhenHtpExit(Entity en)
		{
			Lighten(en);
		}

		void WhenCloseHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				// Entities to be set to true
				SetActive(true, parent);

				// Entities to be set to false
				SetActive(false, htp);
				Coordinator::Instance()->GetComponent<EntityData>(close).active = false;
				Lighten(close);
			}
		}

		void WhenCloseExit(Entity en)
		{
			Lighten(en);
		}
	}

	void HtpButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHtpHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenHtpExit);

		// Htp button will be attached to btn_htp
		parent = static_cast<Entity>( GetSceneGraph().GetParent(en) );
		Entity grandparent = static_cast<Entity>( GetSceneGraph().GetParent(parent) );
		GetSceneGraph().FindImmediateChildren(grandparent);
		std::vector<s32> const& children{ GetSceneGraph().GetChildren() };

		for (s32 child : children)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>( static_cast<Entity>( child ) );
			if (ed.tag == "htp")
				htp = static_cast<Entity>(child);
			else if (ed.tag == "close")
				close = static_cast<Entity>(child);
		}

		CreateEventTrigger(close, false);
		Subscribe(close, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenCloseHover);
		Subscribe(close, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenCloseExit);
	}

	void HtpButton::Free(ECS::Entity en)
	{
		htp = close = parent = MAX_ENTITIES;
	}
}