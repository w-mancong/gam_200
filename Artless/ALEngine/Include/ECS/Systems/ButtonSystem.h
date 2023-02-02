/*!
file:   ButtonSystem.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for ButtonSystem.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	EVENT_BUTTON_SYSTEM_H
#define EVENT_BUTTON_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		/*!*********************************************************************************
			\brief
			Register RigidbodySystem for ECS
		***********************************************************************************/
		void RegisterButtonSystem(void);

		/*!*********************************************************************************
			\brief
			Update EventTrigger System for colliders
		***********************************************************************************/
		void UpdateButtonSystem();

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateButton(Entity const& entity);

		/*!*********************************************************************************
		\brief
			Event for darkening button
		***********************************************************************************/
		void Event_Button_Darken(Entity invoker);

		/*!*********************************************************************************
		\brief
			Event for lightening the button
		***********************************************************************************/
		void Event_Button_Lighten(Entity invoker);
	}
}
#endif