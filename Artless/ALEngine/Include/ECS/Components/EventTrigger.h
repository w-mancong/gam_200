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
	enum class EVENT_TRIGGER_TYPE { NOTHING, ON_POINTER_ENTER, ON_POINTER_STAY, ON_POINTER_EXIT, ON_POINTER_CLICK };
	enum class EVENT_COLLISION_TRIGGER_TYPE { NOTHING, ON_COLLISION_ENTER, ON_COLLISION_STAY, ON_COLLISION_EXIT };

	struct EventListener {
		void (*invokeFunction)(u32 EntityInvoking);
		uint32_t m_position = 0;
	};

	struct Event {
		std::unordered_map<uint32_t, EventListener> m_Listeners;
	};

	struct EventTrigger
	{
		b8 isEnabled { true };
		Event OnPointEnter, OnPointStay, OnPointExit, OnPointClick;
		EVENT_TRIGGER_TYPE current_Trigger_State = EVENT_TRIGGER_TYPE::NOTHING;
	};

	struct EventCollisionListener {
		void (*invokeFunction)(u32, u32);
		uint32_t m_position = 0;
	};

	struct CollisionEvent {
		std::unordered_map<uint32_t, EventCollisionListener> m_Listeners;
	};

	struct EventCollisionTrigger
	{
		CollisionEvent OnCollisionEnter, OnCollisionStay, OnCollisionExit;
		std::vector<u32> otherCurrentCollidingPtr, otherPreviousColliderPtr, otherEnterColliderPtr, otherStayColliderPtr, otherExitColliderPtr;
	};
}
#endif