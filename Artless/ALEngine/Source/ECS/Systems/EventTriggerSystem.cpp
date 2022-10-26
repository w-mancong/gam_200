/*!
file:	EventTriggerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for CharacterControllerSystem.cpp

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"

namespace ALEngine::ECS
{
	//Ease of use for ray
	using Engine::Physics::Ray2D;

	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	/*!*********************************************************************************
		\brief
			Character Controller System, contains functions needed to run components for CharacterController
	***********************************************************************************/
	class EventTriggerSystem : public System
	{
	public:
		void InvokeTriggerListeners(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type);
		void InvokeEvent(Event& event_trigger);
		void UpdateEventTriggerState(EventTrigger& event_trigger, b8 isCurrentlyPointingOn);
		Event& GetEventFromTrigger(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type);

		void UpdatePointerStatus(Entity entity);
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<EventTriggerSystem> eventSystem;
	}

	void RegisterEventTriggerSystem(void)
	{
		eventSystem = Coordinator::Instance()->RegisterSystem<EventTriggerSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<EventTrigger>());
		Coordinator::Instance()->SetSystemSignature<EventTriggerSystem>(signature);
	}

	void UpdateEventTriggerSystem() {
		//Shift through each component
		for (auto it = eventSystem->mEntities.begin(); it != eventSystem->mEntities.end(); ++it) {
			EventTrigger& event_Trigger = Coordinator::Instance()->GetComponent<EventTrigger>(*it);
			eventSystem->UpdatePointerStatus(*it);					
			
			switch (event_Trigger.current_Trigger_State) {
				case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
					eventSystem->InvokeTriggerListeners(event_Trigger, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER);
					break;
				case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
					if (Input::KeyTriggered(KeyCode::MouseLeftButton)) {
						eventSystem->InvokeTriggerListeners(event_Trigger, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK);
					}
					else {
						eventSystem->InvokeTriggerListeners(event_Trigger, EVENT_TRIGGER_TYPE::ON_POINTER_STAY);
					}
					break;
				case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
					eventSystem->InvokeTriggerListeners(event_Trigger, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT);
					break;
			}
		}
	}

	void EventTriggerSystem::UpdatePointerStatus(Entity entity) {
		if (Coordinator::Instance()->HasComponent<Collider2D>(entity)) {
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(entity);
			Transform& transform = Coordinator::Instance()->GetComponent<Transform>(entity);

			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(entity);

			//Check if mouse position is over the collider
			b8 isPointingOver = Physics::Physics2D_CheckCollision_Point_To_AABBBox(Input::GetMouseWorldPos(), (Vector2)transform.position + collider.m_localPosition, collider.scale[0] + transform.scale.x, collider.scale[1] + transform.scale.y);
			
			//Set the state
			UpdateEventTriggerState(eventTrigger,isPointingOver);
		}
	}

	void EventTriggerSystem::InvokeEvent(Event& evnt) {
		for (auto it = evnt.m_Listeners.begin(); it != evnt.m_Listeners.end(); ++it) {
			it->second.invokeFunction();
		}
	}

	void EventTriggerSystem::InvokeTriggerListeners(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type) {
		switch (trigger_type) {
		case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
			eventSystem->InvokeEvent(event_trigger.OnPointEnter);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
			eventSystem->InvokeEvent(event_trigger.OnPointStay);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
			eventSystem->InvokeEvent(event_trigger.OnPointExit);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_CLICK:
			eventSystem->InvokeEvent(event_trigger.OnPointClick);
			break;
		}
	}

	Event& EventTriggerSystem::GetEventFromTrigger(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type) {
		switch (trigger_type)
		{
			case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
				return event_trigger.OnPointEnter;

			case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
				return event_trigger.OnPointStay;

			case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
				return event_trigger.OnPointExit;

			case EVENT_TRIGGER_TYPE::ON_POINTER_CLICK:
				return event_trigger.OnPointClick;

			default:
				return event_trigger.OnPointEnter;
		}
	}

	void EventTriggerSystem::UpdateEventTriggerState(EventTrigger& event_trigger, b8 isCurrentlyPointingOn) {
		switch (event_trigger.current_Trigger_State)
		{
		case EVENT_TRIGGER_TYPE::NOTHING:
			if (isCurrentlyPointingOn)
				event_trigger.current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_ENTER;
			break;

		case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
			if (isCurrentlyPointingOn)
				event_trigger.current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_STAY;
			else
				event_trigger.current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_EXIT;
			break;

		case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
			if (!isCurrentlyPointingOn)
				event_trigger.current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_EXIT;
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
			event_trigger.current_Trigger_State = EVENT_TRIGGER_TYPE::NOTHING;
			break;
		}
	}

	void CreateEventTrigger(Entity const& entity) {
		//Setup EventTrigger for custom stats
		EventTrigger charControl{};
		Coordinator::Instance()->AddComponent(entity, charControl);
	}

	void Subscribe(EventTrigger& eventTrig, EVENT_TRIGGER_TYPE eventType, void (*fp)()) {
		Event& evnt = eventSystem->GetEventFromTrigger(eventTrig, eventType);

		EventListener listener;
		listener.m_position = evnt.m_Listeners.size();
		listener.invokeFunction = fp;
		evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
	}

	void Subscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType, void (*fp)()) {
		if (Coordinator::Instance()->HasComponent<EventTrigger>(entity)) {
			EventTrigger& tempEventTrig = Coordinator::Instance()->GetComponent<EventTrigger>(entity);
			Event& evnt = eventSystem->GetEventFromTrigger(tempEventTrig, eventType);

			EventListener listener;
			listener.m_position = evnt.m_Listeners.size();
			listener.invokeFunction = fp;
			evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
		}
	}
}