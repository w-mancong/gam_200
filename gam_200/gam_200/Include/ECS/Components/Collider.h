/*!
file:	Collider.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for Collider.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	COLLIDER_H
#define COLLIDER_H
namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
			\brief
			Enum used for differentiating the different type of Shape to be created
			***********************************************************************************/
			enum class ColliderType
			{
				Rectangle2D_AABB,
				Rectangle2D_OOBB,
				Circle2D
			};

			/*!*********************************************************************************
				\brief
					2D Circle Collider for collision detection
			***********************************************************************************/
			struct Collider2D
			{
				//Transform const* parentTransform{ 0 };
				Math::Vector2 localPosition { 0.f, 0.f };
				Math::Vector2 globalRight{ 1.f, 0.f }, globalUp{ 0.f, 1.f };
				ColliderType colliderType { ColliderType::Rectangle2D_AABB };
				f32 rotation = 0.f;
				f32 scale[2]{ 1.f, 1.f };
				bool isColliderTriggered{ false }, isCollidedStay{ false }, isColliderExit{ false };
				bool isTrigger{ false };
			};
		}
	}
}
#endif