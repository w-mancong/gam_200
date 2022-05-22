/*!
file:	CollisionSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for RenderSystem

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ManCong
{
	namespace ECS
	{
		/*!*********************************************************************************
			\brief
			Update Collision Check
		***********************************************************************************/
		void CollisionSystemUpdate();

		/*!*********************************************************************************
			\brief
			Update Collision Check 
		***********************************************************************************/
		void CollisionSystemLateUpdate();


		bool InitiateCollisionCheck(Collider* collider_one, Collider* collider_two);

		/*!*********************************************************************************
			\brief
			Register RenderSystem for ECS to work
		***********************************************************************************/
		void RegiserCollisionSystem(void);

		/*!*********************************************************************************
			\brief
			Adds collider component to entity

			\param [in] entity:
			Entity to add the collider component
		***********************************************************************************/
		ManCong::ECS::Collider* CreateBoxCollider2D(Entity const& entity);

		bool CollisionCheck_AABB(Collider box_one, Collider box_two);

		Collision2D_Data CollisionResponse2D_AABB(Collider &movingCollider, Collider &otherCollider);
	}
}