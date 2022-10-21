/*!
file:	EventTriggerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for CharacterControllerSystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		void InvokeEventListeners(Event event_trigger);
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

	void UpdateEventCollisionTriggerSystem() {
		//Shift through each component
		for (auto it = eventSystem->mEntities.begin(); it != eventSystem->mEntities.end(); ++it) {
			Collider2D collide = Coordinator::Instance()->GetComponent<Collider2D>(*it);
		}
	}

	void UpdateEventTriggerSystem() {
		//Shift through each component
		for (auto it = eventSystem->mEntities.begin(); it != eventSystem->mEntities.end(); ++it) {
			EventTrigger& event_Trigger = Coordinator::Instance()->GetComponent<EventTrigger>(*it);
			eventSystem->UpdatePointerStatus(*it);
			
			if (event_Trigger.OnPointEnter.isTriggered) {
				eventSystem->InvokeEventListeners(event_Trigger.OnPointEnter);
				event_Trigger.OnPointEnter.isTriggered = false;
			}
		}
	}

	void Subscribe(EventTrigger& eventTrig, EVENT_TRIGGER_TYPE eventType,void (*fp)()) {
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

	void EventTriggerSystem::UpdatePointerStatus(Entity entity) {
		if (Coordinator::Instance()->HasComponent<Collider2D>(entity)) {
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(entity);
			Transform& transform = Coordinator::Instance()->GetComponent<Transform>(entity);

			std::cout << Vector2{ (f32)Input::GetMousePosX(), (f32)Input::GetMousePosY() } << std::endl;
			if (Physics::Physics2D_CheckCollision_Point_To_AABBBox(Vector2 { (f32)Input::GetMousePosX(), (f32)Input::GetMousePosY() }, (Vector2)transform.position + collider.m_localPosition, collider.scale[0], collider.scale[1])) {
			}
		}
	}

	void EventTriggerSystem::InvokeEventListeners(Event event_Trigger) {
		for (auto it = event_Trigger.m_Listeners.begin(); it != event_Trigger.m_Listeners.end(); ++it) {
			it->second.invokeFunction();
		}
	}

	Event& EventTriggerSystem::GetEventFromTrigger(EventTrigger& event_trigger, EVENT_TRIGGER_TYPE trigger_type) {
		switch (trigger_type)
		{
			case EVENT_TRIGGER_TYPE::ON_POINTER_ENTER:
				return event_trigger.OnPointEnter;

			case EVENT_TRIGGER_TYPE::ON_POINTER_EXIT:
				return event_trigger.OnPointExit;

			default:
				return event_trigger.OnPointEnter;
		}
	}

	void CreateEventTrigger(Entity const& entity) {
		//Setup EventTrigger for custom stats
		EventTrigger charControl{};
		Coordinator::Instance()->AddComponent(entity, charControl);
	}
}
