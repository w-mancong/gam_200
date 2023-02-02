/*!
file:	EventTriggerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for EventTriggerSystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"

namespace ALEngine::ECS
{
	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	/*!*********************************************************************************
		\brief
			Event Trigger System, contains functions needed to run components for EventTrigger System
	***********************************************************************************/
	class ButtonSystem : public System
	{

	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<ButtonSystem> eventSystem;
	}

	void Event_Button_Darken(Entity invoker) {
		//Darken the button
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		Button button = Coordinator::Instance()->GetComponent<Button>(invoker);
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		sprite.color = button.m_color_Tint_OnHover;
	}

	void Event_Button_Lighten(Entity invoker) {
		//Return the button to normal color
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		Button button = Coordinator::Instance()->GetComponent<Button>(invoker);
		[[maybe_unused]] EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(invoker);

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(invoker);
		sprite.color = button.m_color_Tint_Normal;
	}

	void RegisterButtonSystem() {
		eventSystem = Coordinator::Instance()->RegisterSystem<ButtonSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<EventTrigger>());
		signature.set(Coordinator::Instance()->GetComponentType<Button>());
		Coordinator::Instance()->SetSystemSignature<ButtonSystem>(signature);
	}

	void UpdateButtonSystem() {
		////Shift through each component
		//for (auto it = eventSystem->mEntities.begin(); it != eventSystem->mEntities.end(); ++it) {
		//	Button& button = Coordinator::Instance()->GetComponent<Button>(*it);
		//}
	}

	void CreateButton(Entity const& entity) {
		//Add neccesary components if needed
		//Create a background if needed
		if (!Coordinator::Instance()->HasComponent<Sprite>(entity)) {
			Transform transform;
			transform.scale = { 300,100 };
			Coordinator::Instance()->AddComponent(entity, transform);
		
			CreateSprite(entity);
		}

		//Add text component if needed
		if (!Coordinator::Instance()->HasComponent<Text>(entity)) {
			Text text;

			text.textString = "[]";
			text.scale = 1.0f;
			text.currentFont = "Roboto-Bold";
			text.colour = { 0.f, 0.f, 0.f, 1.f };

			//Transform& transform = Coordinator::Instance()->Instance

			text.position -= {50, 0};

			//text
			Coordinator::Instance()->AddComponent(entity, text);
		}

		if (Coordinator::Instance()->HasComponent<Button>(entity)) {
			return;
		}
		
		//Setup EventTrigger for custom stats
		Button button{};
		button.m_color_Tint_Normal = { 1.f, 1.f, 1.f, 1.f };
		button.m_color_Tint_OnHover = { 0.6f, 0.6f, 0.6f, 1.f };
		button.m_color_Tint_OnPress = { 0.2f, 0.2f, 0.2f, 1.f };
		Coordinator::Instance()->AddComponent(entity, button);

		if (Coordinator::Instance()->HasComponent<EventTrigger>(entity)) {
			Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
			Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
			Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
			Coordinator::Instance()->GetComponent<EventTrigger>(entity).layer = 100;
			return;
		}

		//Add eventTrigger with default settings
		EventTrigger eventTrigger;
		eventTrigger.layer = 100;
		Coordinator::Instance()->AddComponent(entity, eventTrigger);

		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Darken);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Lighten);
		Subscribe(entity, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, Event_Button_Lighten);
	}
}
