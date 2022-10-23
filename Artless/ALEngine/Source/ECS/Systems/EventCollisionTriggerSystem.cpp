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
	class EventCollisionTriggerSystem : public System
	{
	public:
		EVENT_COLLISION_TRIGGER_TYPE UpdateCollisionTypeCycle(EventCollisionTrigger& collisionTrigger, bool isCollided);
		void UpdateCollisionEventTriggers(EventCollisionTrigger& collisionTrigger);
		CollisionEvent& GetEventFromTrigger(EventCollisionTrigger& event_trigger, EVENT_COLLISION_TRIGGER_TYPE trigger_type);
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<EventCollisionTriggerSystem> eventCollisionSystem;
	}

	void RegisterEventCollisionTriggerSystem(void)
	{
		eventCollisionSystem = Coordinator::Instance()->RegisterSystem<EventCollisionTriggerSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<EventCollisionTrigger>());
		signature.set(Coordinator::Instance()->GetComponentType<Collider2D>());
		Coordinator::Instance()->SetSystemSignature<EventCollisionTriggerSystem>(signature);
	}

	void UpdateEventCollisionTriggerSystem() {
		//Shift through each component
		for (auto it = eventCollisionSystem->mEntities.begin(); it != eventCollisionSystem->mEntities.end(); ++it) {
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
			EventCollisionTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventCollisionTrigger>(*it);

			//Update Trigger Status
			EVENT_COLLISION_TRIGGER_TYPE collision_Trigger_Cycle = eventCollisionSystem->UpdateCollisionTypeCycle(eventTrigger, collider.isCollided);

			if (collider.collidedCollidersPtr.size() > eventTrigger.otherColliderPtr.size()) {
				for (int i = 0; i < collider.collidedCollidersPtr.size(); ++i) {
					eventTrigger.otherColliderPtr.push_back(*it);
				}
			}

			eventCollisionSystem->UpdateCollisionEventTriggers(eventTrigger);
		}
	}

	CollisionEvent& EventCollisionTriggerSystem::GetEventFromTrigger(EventCollisionTrigger& event_trigger, EVENT_COLLISION_TRIGGER_TYPE trigger_type) {
		switch (trigger_type)
		{
		case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER:
			return event_trigger.OnCollisionEnter;

		case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY:
			return event_trigger.OnCollisionStay;

		case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT:
			return event_trigger.OnCollisionExit;

		default:
			return event_trigger.OnCollisionEnter;
		}
	}

	void Subscribe(EventCollisionTrigger& eventTrig, EVENT_COLLISION_TRIGGER_TYPE eventType, void (*fp)(u32)) {
		CollisionEvent& evnt = eventCollisionSystem->GetEventFromTrigger(eventTrig, eventType);

		EventCollisionListener listener;
		listener.m_position = evnt.m_Listeners.size();
		listener.invokeFunction = fp;
		evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
	}

	void EventCollisionTriggerSystem::UpdateCollisionEventTriggers(EventCollisionTrigger& collisionTrigger) {
		switch (collisionTrigger.currentCollisionTrigger_Type) {
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER:		
				for (auto it = collisionTrigger.OnCollisionEnter.m_Listeners.begin(); it != collisionTrigger.OnCollisionEnter.m_Listeners.end(); ++it) {
					for (int i = 0; i < collisionTrigger.otherColliderPtr.size(); ++i) {
						it->second.invokeFunction(collisionTrigger.otherColliderPtr[i]);
					}
				}
				break;
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY:		
				for (auto it = collisionTrigger.OnCollisionStay.m_Listeners.begin(); it != collisionTrigger.OnCollisionStay.m_Listeners.end(); ++it) {
					for (int i = 0; i < collisionTrigger.otherColliderPtr.size(); ++i) {
						it->second.invokeFunction(collisionTrigger.otherColliderPtr[i]);
					}
				}
				break;
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT:	
				for (auto it = collisionTrigger.OnCollisionExit.m_Listeners.begin(); it != collisionTrigger.OnCollisionExit.m_Listeners.end(); ++it) {
					for (int i = 0; i < collisionTrigger.otherColliderPtr.size(); ++i) {
						it->second.invokeFunction(collisionTrigger.otherColliderPtr[i]);
					}
				}
			collisionTrigger.otherColliderPtr.clear();
			break;
		}
	}

	EVENT_COLLISION_TRIGGER_TYPE EventCollisionTriggerSystem::UpdateCollisionTypeCycle(EventCollisionTrigger& collisionTrigger, bool isCollided) {
		if (collisionTrigger.currentCollisionTrigger_Type == EVENT_COLLISION_TRIGGER_TYPE::NOTHING) 
		{
			collisionTrigger.currentCollisionTrigger_Type = isCollided ? EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER : EVENT_COLLISION_TRIGGER_TYPE::NOTHING;
		}
		else if (collisionTrigger.currentCollisionTrigger_Type == EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER) 
		{
			collisionTrigger.currentCollisionTrigger_Type = isCollided ? EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY : EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT;
		}
		else if (collisionTrigger.currentCollisionTrigger_Type == EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY)
		{
			collisionTrigger.currentCollisionTrigger_Type = isCollided ? EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY : EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT;
		}
		else if (collisionTrigger.currentCollisionTrigger_Type == EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT) 
		{
			collisionTrigger.currentCollisionTrigger_Type = EVENT_COLLISION_TRIGGER_TYPE::NOTHING;
		}
		return collisionTrigger.currentCollisionTrigger_Type;
	}

	void CreateEventCollisionTrigger(Entity const& entity) {
		//Setup EventTrigger for custom stats
		EventCollisionTrigger charControl{};
		Coordinator::Instance()->AddComponent(entity, charControl);
	}
}

//switch (collision_Trigger_Cycle) {
//	case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER:
//		std::cout << "COLLISION ENTER\n";
//	break;
//	case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY:
//		std::cout << "COLLISION STAY\n";
//		break;
//	case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT:
//		std::cout << "COLLISION EXIT\n";
//		break;
//}