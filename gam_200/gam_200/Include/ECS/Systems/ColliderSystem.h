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
		void UpdateStartCollider(void);
		void UpdateCollider(void);
		
		void CreateCollider(Entity const& entity, Transform const& transform, ColliderType shape = ColliderType::Rectangle2D_AABB);
		void Raycast2DCollision(Math::Vector2 start, Math::Vector2 end);
	}
}

#endif