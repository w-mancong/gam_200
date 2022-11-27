/*!
file:	ColliderSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for ColliderSystem.h
		It also contains interface to prepare a gameobject for physics and collision
		
		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		using namespace Component;

		/*!*********************************************************************************
			\brief
			Register ColliderSystem for ECS
		***********************************************************************************/
		void RegisterColliderSystem(void);

		/*!*********************************************************************************
			\brief
			Used at end of update, runs through each collider and conduct collision checks
		***********************************************************************************/
		void UpdateColliderSystem(void);

		/*!*********************************************************************************
			\brief
			An interface to prepare a gameobject to be usable for physics and collision
			Any gameobject that rely on colliders for now would need rigidbody as well, so this should be used whenever collision is needed
			Adds Rigidbody and Collider component to the entity
		***********************************************************************************/
		void CreatePhysics2D(Entity const& entity, ColliderType shape = ColliderType::Rectangle2D_AABB);

		/*!*********************************************************************************
			\brief
			Adds Collider component to the entity
		***********************************************************************************/
		void CreateCollider(Entity const& entity, ColliderType shape = ColliderType::Rectangle2D_AABB);


		/*!*********************************************************************************
			\brief
			Draw colliders of entity
		***********************************************************************************/
		void DebugDrawCollider(void);

		/*!*********************************************************************************
			\brief
			Adds a raycast to the overall collision check, will check through every collider with the ray
		***********************************************************************************/
		void Raycast2DCollision(Math::Vector2 start, Math::Vector2 end);
	}
}

#endif