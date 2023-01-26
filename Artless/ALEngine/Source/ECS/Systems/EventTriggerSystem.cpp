/*!
file:	EventTriggerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for EventTriggerSystem.cpp

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
			Event Trigger System, contains functions needed to run components for EventTrigger System
	***********************************************************************************/
	class EventTriggerSystem : public System
	{
	public:
		/*!*********************************************************************************
		\brief
			Invoke the listeners from a trigger
		***********************************************************************************/
		void InvokeActiveEventTriggers();

		/*!*********************************************************************************
		\brief
			Invoke an Event
		***********************************************************************************/
		void InvokeEvent(Entity invokerEntity, Event& event_trigger);

		/*!*********************************************************************************
		\brief
			Get the current Event from the event Trigger
		***********************************************************************************/
		Event& GetEventFromTrigger(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type);

		/*!*********************************************************************************
		\brief
			Update Trigger Stat from based on input mouse 
		***********************************************************************************/
		void UpdatePointerStatus(Entity entity, bool previousStillOverlapping);

		//State of Trigger
		EVENT_TRIGGER_TYPE current_Trigger_State = EVENT_TRIGGER_TYPE::NOTHING;
		Entity m_interactedEventTrigger_Entity;
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
		//Get if triggered
		bool isClickTriggered = Input::KeyTriggered(KeyCode::MouseLeftButton);
		b8 previousStillOverlapping = false;

		//Do a check in case the current interacted trigger deactivates itself while being used
		if (eventSystem->current_Trigger_State != EVENT_TRIGGER_TYPE::NOTHING) {
			EventTrigger& current_event_Trigger = Coordinator::Instance()->GetComponent<EventTrigger>(eventSystem->m_interactedEventTrigger_Entity);

			//Update the system state to nothing is that is the case
			if (!current_event_Trigger.isEnabled || !Coordinator::Instance()->GetComponent<EntityData>(eventSystem->m_interactedEventTrigger_Entity).active) {
				eventSystem->current_Trigger_State = EVENT_TRIGGER_TYPE::NOTHING;
			}
		}		
		
		//If the system was interacting with an eventTrigger
		if (eventSystem->current_Trigger_State != EVENT_TRIGGER_TYPE::NOTHING) {
			//Keep track of whether it's still being interacted/overlapping
			EventTrigger& current_interacted_eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(eventSystem->m_interactedEventTrigger_Entity);
			Transform& previousTransform = Coordinator::Instance()->GetComponent<Transform>(eventSystem->m_interactedEventTrigger_Entity);

			//Get the calcaltion
			if (Coordinator::Instance()->HasComponent<Collider2D>(eventSystem->m_interactedEventTrigger_Entity)) {
				Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(eventSystem->m_interactedEventTrigger_Entity);
				previousStillOverlapping = Physics::Physics2D_CheckCollision_Point_To_AABBBox(Input::GetMouseWorldPos(), (Vector2)previousTransform.position + collider.m_localPosition, collider.scale[0] + previousTransform.scale.x, collider.scale[1] + previousTransform.scale.y);
			}
			else {
				previousStillOverlapping = Physics::Physics2D_CheckCollision_Point_To_AABBBox(Input::GetMouseWorldPos(), (Vector2)previousTransform.position, previousTransform.scale.x, previousTransform.scale.y);
			}
		}

		//Shift through each component
		for (auto it = eventSystem->mEntities.begin(); it != eventSystem->mEntities.end(); ++it) {
			EventTrigger& event_Trigger = Coordinator::Instance()->GetComponent<EventTrigger>(*it);

			if (!event_Trigger.isEnabled|| !Coordinator::Instance()->GetComponent<EntityData>(*it).active) {
				continue;
			}

			//If game is paused
			if (utils::IsEqual(Time::m_Scale, 0.f)) {
				//If the event trigger cannot run when paused, skip
				if (!event_Trigger.isRunWhenPaused) {
					continue;
				}
			}

			//Update the latest interacted event trigger, pass in the check for previous event trigger overlap as well
			eventSystem->UpdatePointerStatus(*it, previousStillOverlapping);
		}

		//Run the invoking of the event trigger that is interacted with
		//Nothing will happen if the state is nothing
		eventSystem->InvokeActiveEventTriggers();
	}

	void EventTriggerSystem::UpdatePointerStatus(Entity entity, bool previousStillOverlapping) {
		Transform& transform = Coordinator::Instance()->GetComponent<Transform>(entity);
		EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(entity);

		//Check if mouse position is over the collider
		b8 isPointingOver = false;

		if (Coordinator::Instance()->HasComponent<Collider2D>(entity)) {
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(entity);
			isPointingOver = Physics::Physics2D_CheckCollision_Point_To_AABBBox(Input::GetMouseWorldPos(), (Vector2)transform.position + collider.m_localPosition, collider.scale[0] + transform.scale.x, collider.scale[1] + transform.scale.y);
		}
		else {
			isPointingOver = Physics::Physics2D_CheckCollision_Point_To_AABBBox(Input::GetMouseWorldPos(), (Vector2)transform.position, transform.scale.x, transform.scale.y);
		}

		//If system is currently idle and checked entity is not pointed over
		if (current_Trigger_State == EVENT_TRIGGER_TYPE::NOTHING && !isPointingOver) {
			return;
		}

		//If the system is already interacting with something
		if (current_Trigger_State != EVENT_TRIGGER_TYPE::NOTHING && entity != m_interactedEventTrigger_Entity && isPointingOver) {
			EventTrigger& current_interacted_eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(m_interactedEventTrigger_Entity);

			//Take the entity that has the higher layer
			if (!previousStillOverlapping || (previousStillOverlapping && eventTrigger.layer > current_interacted_eventTrigger.layer)) {
				//Run the exit event for previous even trigger
				eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, current_interacted_eventTrigger.OnPointExit);
				
				//Replace and run the on enter event for the next event trigger.
				m_interactedEventTrigger_Entity = entity;
				eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, eventTrigger.OnPointEnter);
				current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_ENTER;
				return;
			}
		}

		//return if the check fails and the check entity is not what the system is already interacting with
		//In case previous checks fails
		if (!isPointingOver && entity != m_interactedEventTrigger_Entity) {
			return;
		}

		//If reached here, means the pointer entered an entity and the system is fresh.
		//usual trigger flow
		switch (current_Trigger_State) {
		case EVENT_TRIGGER_TYPE::NOTHING:
			current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_ENTER;
			m_interactedEventTrigger_Entity = entity;
			break;

		case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
			if (isPointingOver) {
					current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_STAY;
				}
			else {
				current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_EXIT;
			}
			break;

		case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
			if (Input::KeyTriggered(KeyCode::MouseLeftButton)) {
				current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_CLICK;
				break;
			}

			if (!isPointingOver) {
				current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_EXIT;
			}
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
				current_Trigger_State = EVENT_TRIGGER_TYPE::NOTHING;
			break;
		}

		return;
	}

	void EventTriggerSystem::InvokeEvent(Entity invokerEntity, Event& evnt) {
		for (auto it = evnt.m_Listeners.begin(); it != evnt.m_Listeners.end(); ++it) {
			it->second.invokeFunction(invokerEntity);
		}
	}

	void EventTriggerSystem::InvokeActiveEventTriggers() {
		if (current_Trigger_State == EVENT_TRIGGER_TYPE::NOTHING) {
			return;
		}

		EventTrigger& event_trigger = Coordinator::Instance()->GetComponent<EventTrigger>(m_interactedEventTrigger_Entity);

		//Invoke event based on input trigger type
		switch (current_Trigger_State) {
		case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
			eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, event_trigger.OnPointEnter);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_STAY:
			eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, event_trigger.OnPointStay);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
			eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, event_trigger.OnPointExit);
			break;
		case EVENT_TRIGGER_TYPE::ON_POINTER_CLICK:
			eventSystem->InvokeEvent(m_interactedEventTrigger_Entity, event_trigger.OnPointClick);
			current_Trigger_State = EVENT_TRIGGER_TYPE::ON_POINTER_STAY;
			break;
		}
	}

	Event& EventTriggerSystem::GetEventFromTrigger(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type) {
		//Get the event from event Trigger
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

	void CreateEventTrigger(Entity const& entity, bool runWhenPaused) {
		//Setup EventTrigger for custom stats
		EventTrigger charControl{};
		charControl.isRunWhenPaused = runWhenPaused;
		Coordinator::Instance()->AddComponent(entity, charControl);
	}

	void Subscribe(EventTrigger& eventTrig, EVENT_TRIGGER_TYPE eventType, void (*fp)(u32)) {
		Event& evnt = eventSystem->GetEventFromTrigger(eventTrig, eventType);

		EventListener listener;
		listener.m_position = static_cast<u32>(evnt.m_Listeners.size());
		listener.invokeFunction = fp;
		evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
	}

	void Subscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType, void (*fp)(u32)) {
		if (Coordinator::Instance()->HasComponent<EventTrigger>(entity)) {
			EventTrigger& tempEventTrig = Coordinator::Instance()->GetComponent<EventTrigger>(entity);
			Event& evnt = eventSystem->GetEventFromTrigger(tempEventTrig, eventType);

			EventListener listener;
			listener.m_position = static_cast<u32>(evnt.m_Listeners.size());
			listener.invokeFunction = fp;
			evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
		}
	}
}
