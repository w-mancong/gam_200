/*!
file:	EventTriggerSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for EventTriggerSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	EVENT_TRIGGER_SYSTEM_H
#define EVENT_TRIGGER_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		/*!*********************************************************************************
			\brief
			Register RigidbodySystem for ECS
		***********************************************************************************/
		void RegisterEventTriggerSystem(void);

		/*!*********************************************************************************
			\brief
			Update EventTrigger System
		***********************************************************************************/
		void UpdateEventTriggerSystem();

		/*!*********************************************************************************
			\brief
			Adds Event Trigger component to the entity
		***********************************************************************************/
		void CreateEventTrigger(Entity const& entity);

		/*!*********************************************************************************
			\brief Subscribe a function to listen to a Collision based trigger

			\param [out] eventTrig: The Collision Event Trigger component
			\param [in] eventType: Type of Trigger for the function to listen to
			\param [in] fp: function that will be subscribed to the trigger
		***********************************************************************************/
		void Subscribe(EventCollisionTrigger& eventTrig, EVENT_COLLISION_TRIGGER_TYPE eventType, void (*fp)(u32, u32));

		/*!*********************************************************************************
			\brief Subscribe a function to listen to a Pointer based trigger

			\param [out] eventTrig: The Event Trigger component
			\param [in] eventType: Type of Trigger for the function to listen to
			\param [in] fp: function that will be subscribed to the trigger
		***********************************************************************************/
		void Subscribe(EventTrigger& eventTrig, EVENT_TRIGGER_TYPE eventType, void (*fp)());

		/*!*********************************************************************************
			\brief Subscribe a function to listen to a Pointer based trigger via passing of entity instead of EventTrigger Component

			\param [out] eventTrig: Entity with the Event Trigger component (must have Event Trigger Component)
			\param [in] eventType: Type of Trigger for the function to listen to
			\param [in] fp: function that will be subscribed to the trigger
		***********************************************************************************/
		void Subscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType, void (*fp)());
		//void Unsubscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType);
	}
}
#endif