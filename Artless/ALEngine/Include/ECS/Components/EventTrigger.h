/*!
file:   EventTrigger.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for EventTrigger.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	EVENTTRIGGER_H
#define EVENTTRIGGER_H
namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Enum for types of Triggers on mouse
	***********************************************************************************/
	enum class EVENT_TRIGGER_TYPE { NOTHING, ON_POINTER_ENTER, ON_POINTER_STAY, ON_POINTER_EXIT, ON_POINTER_CLICK };

	/*!*********************************************************************************
	\brief
		Enum for types of Triggers on collision
	***********************************************************************************/
	enum class EVENT_COLLISION_TRIGGER_TYPE { NOTHING, ON_COLLISION_ENTER, ON_COLLISION_STAY, ON_COLLISION_EXIT };

	/*!*********************************************************************************
	\brief
		Container for a listener (function)
	***********************************************************************************/
	struct EventListener {
		void (*invokeFunction)(u32 EntityInvoking);
		uint32_t m_position = 0;	//Listener's position in the list
	};

	/*!*********************************************************************************
	\brief
		Container for a event, it's listener will be invoked when event is triggered
	***********************************************************************************/
	struct Event {
		std::unordered_map<uint32_t, EventListener> m_Listeners;
	};

	/*!*********************************************************************************
	\brief
		Component for EventTrigger
	***********************************************************************************/
	struct EventTrigger
	{
		//Component Enabled
		b8 isEnabled { true };

		s32 layer = 0;

		//Events
		Event OnPointEnter, OnPointStay, OnPointExit, OnPointClick;
	};

	/*!*********************************************************************************
	\brief
		Component for EventCollisionListener
	***********************************************************************************/
	struct EventCollisionListener {
		void (*invokeFunction)(u32, u32);
		uint32_t m_position = 0;
	};

	/*!*********************************************************************************
	\brief
		Container for a collision event, it's listener will be invoked when event is triggered
	***********************************************************************************/
	struct CollisionEvent {
		std::unordered_map<uint32_t, EventCollisionListener> m_Listeners;
	};

	/*!*********************************************************************************
	\brief
		Component for EventCollisionTrigger
	***********************************************************************************/
	struct EventCollisionTrigger
	{
		//Events
		CollisionEvent OnCollisionEnter, OnCollisionStay, OnCollisionExit;

		//State of Trigger
		std::vector<u32> otherCurrentCollidingPtr, otherPreviousColliderPtr, otherEnterColliderPtr, otherStayColliderPtr, otherExitColliderPtr;
	};
}
#endif