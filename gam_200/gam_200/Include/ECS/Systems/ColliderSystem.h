/*!
file:	ColliderSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for ColliderSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

namespace ManCong
{
	namespace ECS
	{
		using namespace Component;

		/*!*********************************************************************************
			\brief
			Register RenderSystem for ECS to work
		***********************************************************************************/
		void RegisterColliderSystem(void);

		/*!*********************************************************************************
			\brief
			Used on start of update, runs through each rigidbody and prepares them for beginning of collision cycle
		***********************************************************************************/
		void UpdateStartColliderSystem(void);

		/*!*********************************************************************************
			\brief
			Used at end of update, runs through each collider and conduct collision checks
		***********************************************************************************/
		void UpdateColliderSystem(void);

		/*!*********************************************************************************
			\brief
			Adds Collider component to the entity
		***********************************************************************************/
		void CreateCollider(Entity const& entity, ColliderType shape = ColliderType::Rectangle2D_AABB);

		/*!*********************************************************************************
			\brief
			Adds a raycast to the overall collision check, will check through every collider with the ray
		***********************************************************************************/
		void Raycast2DCollision(Math::Vector2 start, Math::Vector2 end);
	}
}

#endif