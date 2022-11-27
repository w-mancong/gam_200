/*!
file:	EventCollisionTriggerSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for EventTriggerSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	EVENT_COLLISION_TRIGGER_SYSTEM_H
#define EVENT_COLLISION_TRIGGER_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		/*!*********************************************************************************
			\brief
			Register RigidbodySystem for ECS
		***********************************************************************************/
		void RegisterEventCollisionTriggerSystem(void);

		/*!*********************************************************************************
			\brief
			Update EventTrigger System for colliders
		***********************************************************************************/
		void UpdateEventCollisionTriggerSystem();

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateEventCollisionTrigger(Entity const& entity);
	}
}
#endif