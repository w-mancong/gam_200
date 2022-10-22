/*!
file:	EventTrigger.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for EventTrigger.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	EVENTTRIGGER_H
#define EVENTTRIGGER_H
namespace ALEngine::ECS::Component
{
	enum class EVENT_TRIGGER_TYPE { ON_POINTER_ENTER, ON_POINTER_EXIT };
	enum class EVENT_COLLISION_TRIGGER_TYPE { NOTHING, ON_COLLISION_ENTER, ON_COLLISION_STAY, ON_COLLISION_EXIT };

	struct EventListener {
		void (*invokeFunction)();
		uint32_t m_position = 0;
	};

	struct Event {
		std::unordered_map<uint32_t, EventListener> m_Listeners;
		b8 isTriggered = 0;
	};

	struct EventTrigger
	{
		Event OnPointEnter, OnPointExit;
	};

	struct EventCollisionListener {
		void (*invokeFunction)(Collider2D*);
		uint32_t m_position = 0;
	};

	struct CollisionEvent {
		std::unordered_map<uint32_t, EventCollisionListener> m_Listeners;

		b8 isTriggered = 0;
	};

	struct EventCollisionTrigger
	{
		CollisionEvent OnCollisionEnter, OnCollisionStay, OnCollisionExit;
		EVENT_COLLISION_TRIGGER_TYPE currentCollisionTrigger_Type;
		std::vector<Collider2D*> otherColliderPtr;
	};
}
#endif