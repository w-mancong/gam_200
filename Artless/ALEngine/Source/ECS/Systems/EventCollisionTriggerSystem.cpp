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
		void InvokeCollisionEventTriggers(EventCollisionTrigger& collisionTrigger, EVENT_COLLISION_TRIGGER_TYPE trigger_type, Entity current);
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
		Coordinator::Instance()->SetSystemSignature<EventCollisionTriggerSystem>(signature);
	}

	void UpdateEventCollisionTriggerSystem() {
		//Shift through each component
		for (auto it = eventCollisionSystem->mEntities.begin(); it != eventCollisionSystem->mEntities.end(); ++it) {
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
			EventCollisionTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventCollisionTrigger>(*it);

			eventTrigger.otherCurrentCollidingPtr.clear();

			//Update colliders
			for (uint32_t i = 0; i < collider.collidedCollidersPtr.size(); ++i) {
				b8 exist = false;
				for (uint32_t j = 0; j < eventTrigger.otherCurrentCollidingPtr.size(); ++j) {
					if(collider.collidedCollidersPtr[i] == eventTrigger.otherCurrentCollidingPtr[j]){
						exist = true;
						break;
					}
				}

				if (!exist) {
					eventTrigger.otherCurrentCollidingPtr.push_back(collider.collidedCollidersPtr[i]);
				}
			}

			//Check for Enter and stayed
			for (uint32_t i = 0; i < eventTrigger.otherCurrentCollidingPtr.size(); ++i) {

				b8 hasFirstEntered = true;
				for (uint32_t j = 0; j < eventTrigger.otherPreviousColliderPtr.size(); ++j) {
					if (eventTrigger.otherPreviousColliderPtr[j] == eventTrigger.otherCurrentCollidingPtr[i]) {
						hasFirstEntered = false;
						break;
					}
				}

				if (hasFirstEntered) {
					eventTrigger.otherEnterColliderPtr.push_back(eventTrigger.otherCurrentCollidingPtr[i]);
				}
				else {
					eventTrigger.otherStayColliderPtr.push_back(eventTrigger.otherCurrentCollidingPtr[i]);
				}
			}

			//Check For Exited
			for (uint32_t i = 0; i < eventTrigger.otherPreviousColliderPtr.size(); ++i) {
				b8 stillCollided = false;
				for (uint32_t j = 0; j < eventTrigger.otherCurrentCollidingPtr.size(); ++j) {
					if (eventTrigger.otherPreviousColliderPtr[i] == eventTrigger.otherCurrentCollidingPtr[j]) {
						stillCollided = true;
						break;
					}
				}

				if (!stillCollided) {
					eventTrigger.otherExitColliderPtr.push_back(eventTrigger.otherPreviousColliderPtr[i]);
				}
			}

			//Update Previous
			eventTrigger.otherPreviousColliderPtr.clear();			

			for (uint32_t i = 0; i < eventTrigger.otherCurrentCollidingPtr.size(); ++i) {
				eventTrigger.otherPreviousColliderPtr.push_back(eventTrigger.otherCurrentCollidingPtr[i]);
			}

			//for (uint32_t i = 0; i < eventTrigger.otherCurrentCollidingPtr.size(); ++i) {
			//	std::cout << *it << " : " << eventTrigger.otherCurrentCollidingPtr[i] << " A ";
			//}
			//std::cout << "\n";

			//Invoke all colliders
			eventCollisionSystem->InvokeCollisionEventTriggers(eventTrigger, EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER, *it);
			eventCollisionSystem->InvokeCollisionEventTriggers(eventTrigger, EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY, *it);
			eventCollisionSystem->InvokeCollisionEventTriggers(eventTrigger, EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT, *it);

			//Clear exit
			eventTrigger.otherEnterColliderPtr.clear();
			eventTrigger.otherStayColliderPtr.clear();
			eventTrigger.otherExitColliderPtr.clear();
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

	void Subscribe(EventCollisionTrigger& eventTrig, EVENT_COLLISION_TRIGGER_TYPE eventType, void (*fp)(u32, u32)) {
		CollisionEvent& evnt = eventCollisionSystem->GetEventFromTrigger(eventTrig, eventType);

		EventCollisionListener listener; 
		listener.m_position = static_cast<u32>(evnt.m_Listeners.size());
		listener.invokeFunction = fp;
		evnt.m_Listeners.insert(std::pair(listener.m_position, listener));
	}

	void EventCollisionTriggerSystem::InvokeCollisionEventTriggers(EventCollisionTrigger& collisionTrigger, EVENT_COLLISION_TRIGGER_TYPE trigger_type, Entity current) {
		switch (trigger_type) {
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER:		
				for (auto it = collisionTrigger.OnCollisionEnter.m_Listeners.begin(); it != collisionTrigger.OnCollisionEnter.m_Listeners.end(); ++it) {
					for (uint32_t i = 0; i < collisionTrigger.otherEnterColliderPtr.size(); ++i) {
						it->second.invokeFunction(current, collisionTrigger.otherEnterColliderPtr[i]);
					}
				}
				break;
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY:		
				for (auto it = collisionTrigger.OnCollisionStay.m_Listeners.begin(); it != collisionTrigger.OnCollisionStay.m_Listeners.end(); ++it) {
					for (uint32_t i = 0; i < collisionTrigger.otherStayColliderPtr.size(); ++i) {
						it->second.invokeFunction(current, collisionTrigger.otherStayColliderPtr[i]);
					}
				}
				break;
			case EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT:	
				for (auto it = collisionTrigger.OnCollisionExit.m_Listeners.begin(); it != collisionTrigger.OnCollisionExit.m_Listeners.end(); ++it) {
					for (uint32_t i = 0; i < collisionTrigger.otherExitColliderPtr.size(); ++i) {
						it->second.invokeFunction(current, collisionTrigger.otherExitColliderPtr[i]);
					}
				}
			break;
		}
	}

	void CreateEventCollisionTrigger(Entity const& entity) {
		//Setup EventTrigger for custom stats
		EventCollisionTrigger charControl{};
		Coordinator::Instance()->AddComponent(entity, charControl);
	}
}