/*!
file:	CollisionSystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for RenderSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ManCong
{
	namespace ECS
	{
		/*!*********************************************************************************
			\brief
			Adds collider component to entity

			\param [in] entity:
			Entity to add the collider component
		***********************************************************************************/
		ManCong::ECS::Collider* CreateCollider(Entity const& entity);
	}
}