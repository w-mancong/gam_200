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
		using namespace Component;

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
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateEventTrigger(Entity const& entity);

		void Subscribe(EventCollisionTrigger& eventTrig, EVENT_COLLISION_TRIGGER_TYPE eventType, void (*fp)(u32, u32));
		void Subscribe(EventTrigger& eventTrig, EVENT_TRIGGER_TYPE eventType, void (*fp)());
		void Subscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType, void (*fp)());
		//void Unsubscribe(Entity const& entity, EVENT_TRIGGER_TYPE eventType);
	}
}
#endif